#include "head.h"
#include "lex.cpp"

using namespace std;

typedef struct production {
	string left; //产生式左部
	vector<string> right;// [10]; //产生式右部
	vector<string> right1;
}_production;
_production prod1[500];

class Token {		  //Token类
public:
	enum TokenType {  //类码对应值
		_keyword = 1, //关键字
		_identifier,  //标识符
		_delimiter,   //界符
		_constnum,    //算术常数
		_character,   //字符
		_string,      //字符串
		_space
	};
	TokenType type;   //类码
	int id;			  //对应符号表中的下标
	int row;          //该单词在源程序文件中的行数，用于检错时输出错误出现的行数
	Token() { row = 0; } 		    //无参构造方法
	Token(TokenType type, int id) { //有参构造方法
		this->type = type, this->id = id;
		this->row = 0;
	}
};

class Lex : public Token { //语法分析，包含各种符号表
public:
	struct re_table {
		re_table() {}
		vector<pair<regex, TokenType>> table{
			make_pair(regex("void|char|int|float"), _keyword),
			make_pair(regex("[a-zA-Z_][0-9a-zA-Z_]*"), _identifier),
			make_pair(regex("-?[0-9]+((\\.[0-9]+)([eE]-?[0-9]+)|(\\.[0-9]+)|([eE]-?[0-9]+))?[fF]?"), _constnum), //匹配浮点数
			make_pair(regex("-?(0|[1-9][0-9]*|0[xX][0-9a-fA-F]+|0[0-7]+)[uU]?[lL]{0,2}"), _constnum), //匹配整型数（含十进制、十六进制、八进制）
			make_pair(regex("\\+\\+|--|\\+=|-=|\\*=|/=|%=|<<=|>>=|\\|=|\\&=|\\^=|\\+|-|\\*|/|==|>>|<<|=|>=|>|<=|<|;|\\{|\\}|\\(|\\)|\\[|\\]|,|!|%|\\|\\||\\&\\&|\\||\\&|:|#|\\.|->|\\?|~|\\^"), _delimiter),
			make_pair(regex("['][\\s\\S^']?'"), _character),
			make_pair(regex("\".*\""), _string),
			make_pair(regex("\\s+"), _space) //匹配空白符
		};
	};
	Token tokenSeq[100]; //开辟token序列
	int index_of_token = 0;
	string keyword_table[100] = { "void","float","char","int" }; //关键字表
	string delimiter_table[100] = { "++","--","+=","-=","*=", "/=","%=", "<<=", ">>=","|=","&=",
		"^=","+","-","*","/","==",">>","<<","=",">=",">","<=","<",";","{","}","(",")","[","]",",","!","%",
		"||","&&","|","&",":","#",".","->","?","~","^" }; //界符表
	int index_of_idtfer = 0;//标识符表
	string constant_table[100];
	int index_of_const = 0;//常数表
	string character_table[100];
	int index_of_char = 0;//字符表
	string string_table[100];
	int index_of_string = 0;//字符串表
};
class Syntax : public Lex {
public:
	typedef struct production {
		string left; //产生式左部
		vector<string> right;// [10]; //产生式右部
		vector<string> right1;
	}production;
	production prod[500];      //产生式数组，用于存放预处理后的产生式
	int size_of_prod = 0;                    //产生式数组下标
	set<string> terminators, non_terminators;//终结符集合和非终结符集合	 
	map<string, set<string>> First, Follow;  //first集合、follow集合 [for example: First(alpha), map(alpha, {First(alpha})
	map<int, set<string>> Select;            //Select set
	map<string, bool> is_deduced_epsilon;    //非终结符是否可以推空
	map<string, vector<string>> tmpStorage;  //临时存储求解一个左部非终结符的first集合所需的右部非终结符first集合
	map<string, bool> visited;               //访问标记，用于深搜
	map<string, int> Action_map;             //语义动作标志映射
	stack<pair<string, int>> SEM;            //语义栈      
	int TapelIndex;                          //类型表访问下标
	Syntax();                                //构造函数
	void preProcess();
	void dataRevision();
	void DFS_First();
	void getFirst(string curSymbol);
	set<string> int_first(vector<string> alpha);
	void DFS_Follow();
	void getFollow(string curSymbol);
	void getSelect();
	int list_of_LL1(string A, string a);
	map<pair<string, string>, int>  CreateLL1List();
	void Enter();
	void neigui();
	void file_enter();
};
Syntax::Syntax()
{
}
/*---------------------------------------------------------------------------------------------------------------
											预处理函数(扫描一遍文法规则文件）
功能: 1. 填非终结符集合、填终结符集合; 2. 填终结符的first集合; 3. 判断每个非终结符是否可以推空,填表; 4. 分割产生式的左右部.
----------------------------------------------------------------------------------------------------------------*/
void Syntax::preProcess() {
	ifstream in(C_RULES, ios::in); //读取C文法文件
	if (!in.is_open()) { //文法文件打开出错，可能是该文件被用户意外删除所致
		cout << "文法规则文件读取失败，找不到该文件！" << endl;
		return;
	}
	string line_in_rules;
	int index_of_prod = 0;
	while (getline(in, line_in_rules))
	{
		int ptr_of_rules = 0; //一行文法规则内每个字符的访问指针
		while (line_in_rules[ptr_of_rules] == ' ' || line_in_rules[ptr_of_rules] == '\t') ptr_of_rules++;//滤掉空白符
		if (line_in_rules[ptr_of_rules] == '#') continue; //每一条文法规则的结束
		if (isalpha(line_in_rules[ptr_of_rules]) == 2)
		{ //提取产生式左部的非终结符
			string nonTerm = ""; //临时存储非终结符
			while (line_in_rules[ptr_of_rules] != ' ' && line_in_rules[ptr_of_rules] != '\t')
			{	//分割出一个非终结符
				nonTerm += line_in_rules[ptr_of_rules];
				ptr_of_rules++;
			}
			non_terminators.insert(nonTerm); //将该非终结符加入到非终结符表中
			prod[index_of_prod].left = nonTerm; //记录该产生式的左部
			size_of_prod++; //产生式数量+1
			while (line_in_rules[ptr_of_rules] == ' ' || line_in_rules[ptr_of_rules] == '\t' || line_in_rules[ptr_of_rules] == ':')
				ptr_of_rules++; //滤掉冒号和空白符
			while (ptr_of_rules < line_in_rules.size()) { //记录产生式右部符号，顺便填写终结符集合
														  //一直走到该行结束
				string tmp = ""; //拼出右部一个符号
				if (line_in_rules[ptr_of_rules] == '\'') {
					//当前出现了终结符，需要记录该终结符
					ptr_of_rules++; //略去单引号
					while (line_in_rules[ptr_of_rules] != '\'') {
						tmp += line_in_rules[ptr_of_rules];
						ptr_of_rules++;
					}//拼出一个终结符
					terminators.insert(tmp); //将该终结符记录在终结符表中
					set<string> tmpset; //临时集合，用于存储终结符的First集
					tmpset.insert(tmp); //暂存终结符
					First.insert(make_pair(tmp, tmpset)); //终结符的First集还是其本身
					prod[index_of_prod].right.push_back(tmp); //产生式右部记录该符号

					ptr_of_rules += 2; //略去终结符后面的单引号和空格
				}
				else { //当前符号是一个非终结符或是一个语义动作标志
					while (ptr_of_rules < line_in_rules.size() && line_in_rules[ptr_of_rules] != ' ') {
						tmp += line_in_rules[ptr_of_rules];
						ptr_of_rules++;
					}//拼出一个非终结符
					ptr_of_rules++; //略去文法符号之间的空格
					prod[index_of_prod].right.push_back(tmp); //将该文法符号或语义动作符号存到产生式右部vector中

					if (tmp == "epsilon") {
						//该产生式左部的非终结符可以直接推空
						is_deduced_epsilon[prod[index_of_prod].left] = true;
					}
				}
			}
			index_of_prod++;
		}
		else if (line_in_rules[ptr_of_rules] == '|') {
			//一个非终结符推出的其他产生式
			ptr_of_rules += 2; //略去'|'和空格
			prod[index_of_prod].left = prod[index_of_prod - 1].left; //该产生式左部必定和上一个产生式是一样的	
			size_of_prod++;
			while (ptr_of_rules < line_in_rules.size()) { //记录产生式右部符号，顺便填写终结符集合
														  //一直走到该行结束
				string tmp = ""; //拼出右部一个符号
				if (line_in_rules[ptr_of_rules] == '\'') {
					//当前出现了终结符，需要记录该终结符
					ptr_of_rules++; //略去单引号
					while (line_in_rules[ptr_of_rules] != '\'') {
						tmp += line_in_rules[ptr_of_rules];
						ptr_of_rules++;
					}//拼出一个终结符
					terminators.insert(tmp); //将该终结符记录在终结符表中
					set<string> tmpset;
					tmpset.insert(tmp);
					First.insert(make_pair(tmp, tmpset));
					prod[index_of_prod].right.push_back(tmp); //产生式右部记录该符号

					ptr_of_rules += 2; //略去终结符后面的单引号和空格
				}
				else { //当前符号是一个非终结符或语义动作标志
					while (ptr_of_rules < line_in_rules.size() && line_in_rules[ptr_of_rules] != ' ') {
						tmp += line_in_rules[ptr_of_rules];
						ptr_of_rules++;
					}//拼出一个非终结符
					ptr_of_rules++; //略去文法符号之间的空格
					prod[index_of_prod].right.push_back(tmp); //将该文法符号存到产生式右部vector中
					string tmp1 = " ";
					prod[index_of_prod].right1.push_back(tmp1);
					if (tmp == "epsilon") {
						//该产生式左部的非终结符可以直接推空
						is_deduced_epsilon[prod[index_of_prod].left] = true;
					}
				}
			}
			index_of_prod++;
		}
	}
	in.close(); //关闭文件
	/* terminators.insert("int_const"); //额外添加几个终结符
	First["int_const"].insert("int_const");
	terminators.insert("char_const");
	First["char_const"].insert("char_const");
	terminators.insert("id");
	First["id"].insert("id");
	terminators.insert("float_const");
	First["float_const"].insert("float_const");
	terminators.insert("string");
	First["string"].insert("string"); */

}

//判断一个字符串是否全为大写字母,以此区分文法符号和语义动作符号
bool isUpper(string str) {
	if (regex_match(str, regex("[A-Z_0-9]+")))return true;
	else return false;
}
/*---------------------------------------------------------------------------------------------------------------
						第二遍扫描经过预处理的产生式数组，修正完善判空表，将可空情况加入first集合.
----------------------------------------------------------------------------------------------------------------*/
void Syntax::dataRevision() { //数据修正--第二遍预处理，解决第一遍没有解决的问题
	//先倒着扫一次
	for (int i = size_of_prod - 1; i >= 0; i--) {
		if (!is_deduced_epsilon[prod[i].left]) { //原可推空标志为false
			int index_of_right = 0;
			//bool isAction = false; //是否为语义动作标志
			while (index_of_right < prod[i].right.size() &&
				(non_terminators.find(prod[i].right[index_of_right]) != non_terminators.end()
					|| isUpper(prod[i].right[index_of_right]))) {
				//如果产生式右端为非终结符或语义动作标志
				if (isUpper(prod[i].right[index_of_right])) index_of_right++; //语义动作标志直接跳过
				else { //是非终结符
					if (!is_deduced_epsilon[prod[i].right[index_of_right]]) {
						is_deduced_epsilon[prod[i].left] = false;
						break;
					}
					else index_of_right++;
				}
			}
			if (index_of_right >= prod[i].right.size()) { //产生式右端所有符号都可以推空
				is_deduced_epsilon[prod[i].left] = true;
				First[prod[i].left].insert("epsilon");
			}
		}
	}
	//再正着扫一遍
	for (int i = 0; i < size_of_prod; i++) {
		if (!is_deduced_epsilon[prod[i].left]) { //原可推空标志为false
			int index_of_right = 0;
			while (index_of_right < prod[i].right.size() &&
				(non_terminators.find(prod[i].right[index_of_right]) != non_terminators.end()
					|| isUpper(prod[i].right[index_of_right]))) {
				//如果产生式右端为非终结符或语义动作标志
				if (isUpper(prod[i].right[index_of_right])) index_of_right++; //语义动作标志直接跳过
				else { //是非终结符
					if (!is_deduced_epsilon[prod[i].right[index_of_right]]) {
						is_deduced_epsilon[prod[i].left] = false;
						break;
					}
					else index_of_right++;
				}
			}
			if (index_of_right >= prod[i].right.size()) { //产生式右端所有符号都可以推空
				is_deduced_epsilon[prod[i].left] = true;
				First[prod[i].left].insert("epsilon");
			}
		}
	}
}
/*---------------------------------------------------------------------------------------------------------------
					最后一遍深度优先搜索，扫描产生式，将first集填充完整, 程序调用入口DFS_First()
										求First(T), T -> Vn or Vt
----------------------------------------------------------------------------------------------------------------*/
void Syntax::DFS_First() {
	for (set<string>::iterator it = non_terminators.begin(); it != non_terminators.end(); it++)
		visited.insert(make_pair(*it, false)); //初始化
	for (set<string>::iterator it = non_terminators.begin(); it != non_terminators.end(); it++)
	{
		if (!visited[*it]) //如果该符号未被访问
			getFirst(*it);//访问之
	}
}
void Syntax::getFirst(string curSymbol) {

	visited[curSymbol] = true; //将该符号的访问标志置为true，表明已被访问
	for (int index_prod = 0; index_prod < size_of_prod; index_prod++)
	{	//遍历产生式组

		if (prod[index_prod].left == curSymbol) { //找到该符号对应的产生式
			int index_right = 0; //当前产生式右部符号集合索引
			if (isUpper(prod[index_prod].right[index_right])) index_right++; //如果右部首符号是一个语义标志，跳过他
			if (non_terminators.find(prod[index_prod].right[index_right]) != non_terminators.end()) {
				//产生式右部首符号为非终结符
				vector<string> tmpvec;
				tmpvec.push_back(prod[index_prod].right[index_right]); //格式转换 string -> vector<string>
				tmpStorage.insert(make_pair(prod[index_prod].left, tmpvec)); //暂存之
				if (!visited[prod[index_prod].right[index_right]]) //如果该符号未被访问过!!!!!!!!
					getFirst(prod[index_prod].right[index_right]); //获取该符号的first集合
				set<string> t_set;
				t_set = First[prod[index_prod].right[index_right]];
				if (is_deduced_epsilon[prod[index_prod].right[index_right]]) {
					//如果该符号可以推空  First type is: map<string, set<string>>
					t_set.erase("epsilon");
				}
				First[prod[index_prod].left].insert(t_set.begin(), t_set.end()); //合并到左部符号的first集合
				while (is_deduced_epsilon[prod[index_prod].right[index_right]]) {
					//如果该产生式右部首符号可以推空 
				next_symb: index_right++; //向后走一个，看下一个符号
					if (index_right < prod[index_prod].right.size())
					{	//如果右部该符号后面还有符号
						if (isUpper(prod[index_prod].right[index_right])) {
							//遇到语义动作标志,跳过
							goto next_symb;
						}
						if (non_terminators.find(prod[index_prod].right[index_right]) != non_terminators.end()) {
							//如果后面这个符号也是非终结符
							tmpvec.push_back(prod[index_prod].right[index_right]);
							tmpStorage[prod[index_prod].left] = tmpvec; //暂存之
							if (!visited[prod[index_prod].right[index_right]]) //如果该符号未被访问过!!!!!!
								getFirst(prod[index_prod].right[index_right]); //访问之
							//else { //如果该符号已被访问过
							set<string> _set;
							_set = First[prod[index_prod].right[index_right]];
							if (is_deduced_epsilon[prod[index_prod].right[index_right]]) {
								//如果该符号可以推空  First type is: map<string, set<string>>
								_set.erase("epsilon");
							}
							First[prod[index_prod].left].insert(_set.begin(), _set.end()); //合并到左部符号的first集合
						}
						else First[prod[index_prod].left].insert(prod[index_prod].right[index_right]);
						//后面这个符号是终结符，存入左部符号first集合中
					}
					else break;
				}
			}
			//产生式右部首符号为终结符或@, 将之加入左部符号的first集中
			else {
				First[prod[index_prod].left].insert(prod[index_prod].right[index_right]);
			}
		}
	}
}
/*---------------------------------------------------------------------------------------------------------------
						求First(alpha),alpha -> (Vn + Vt)*，用于求Select集的First入口函数。
----------------------------------------------------------------------------------------------------------------*/
set<string> Syntax::int_first(vector<string> alpha)//first接口--入口函数
{
	int index_right = 0;
	if (regex_match(alpha[index_right], regex("[A-Z_0-9]+")))index_right++; //首符为语义动作标志，跳过
	if (alpha[index_right] == "epsilon" || terminators.find(alpha[index_right]) != terminators.end()
		|| (non_terminators.find(alpha[index_right]) != non_terminators.end() && !is_deduced_epsilon[alpha[index_right]]))
	{
		//如果产生式右部首符号是ε 或 终结符 或 不可以推空的非终结符
		set<string> tmpset;
		if (alpha[index_right] == "epsilon")
		{ //如果某个产生式右部仅有一个ε
			tmpset.insert("epsilon");
			return tmpset;
		}
		return First[alpha[index_right]];
	}
	else {//产生式右部首符号是可以推空的非终结符
		set<string> tmpset;
		while (index_right < alpha.size() &&
			((non_terminators.find(alpha[index_right]) != non_terminators.end() &&
				is_deduced_epsilon[alpha[index_right]]) || isUpper(alpha[index_right]))) {
			//如果产生式右部是可以推空的非终结符
			if (regex_match(alpha[index_right], regex("[A-Z_0-9]+"))) {
				//遇到语义动作符号，跳过（直接匹配可以在一定程度上提高效率，减少程序跳转）
				index_right++;
				continue;
			}
			tmpset.insert(First[alpha[index_right]].begin(), First[alpha[index_right]].end());
			tmpset.erase("epsilon");
			index_right++;
		}
		if (index_right >= alpha.size()) {
			//右部全可以推空
			tmpset.insert("epsilon");
		}
		else { //遇到了无法推空的符号
			tmpset.insert(First[alpha[index_right]].begin(), First[alpha[index_right]].end());
		}
		return tmpset;
	}
}
/*---------------------------------------------------------------------------------------------------------------
												求Follow集合
----------------------------------------------------------------------------------------------------------------*/
void Syntax::DFS_Follow()
{
	for (set<string>::iterator it = non_terminators.begin(); it != non_terminators.end(); it++)
		visited[*it] = false; //再次初始化访问标志数组
	for (set<string>::iterator it = non_terminators.begin(); it != non_terminators.end(); it++)
		if (!visited[*it])
			getFollow(*it);
}
void Syntax::getFollow(string curSymbol) {//First type is: map<string, set<string>>
	visited[curSymbol] = true;
	if (curSymbol == prod[0].left)//如果当前符号为开始符号，且该符号的follow集还未建立
		Follow[curSymbol].insert("￥");
	for (int i = 0; i < size_of_prod; i++) {
		//遍历产生式组
		int index_right = 0;
		vector<string>::iterator it = find(prod[i].right.begin(), prod[i].right.end(), curSymbol);
		while (it != prod[i].right.end())
		{ //如果该产生式右部可以找到待求符号
			index_right = it - prod[i].right.begin(); //迭代器转换为索引值
			index_right++;
			if (index_right < prod[i].right.size() && regex_match(prod[i].right[index_right], regex("[A-Z_0-9]+"))) index_right++; //该符号后面是一个语义标志，跳过
			while (index_right < prod[i].right.size() && (is_deduced_epsilon[prod[i].right[index_right]]
				|| isUpper(prod[i].right[index_right]))) {
				//如果产生式右部该非终结符后面还有符号 且后面的符号可以推空或者后面是一个语义标志
				if (regex_match(prod[i].right[index_right], regex("[A-Z_0-9]+"))) {
					index_right++;
					continue;
				} //遇到语义动作标志，则跳过
				set<string> tmp;
				tmp = First[prod[i].right[index_right]];
				tmp.erase("epsilon");
				Follow[prod[i].right[it - prod[i].right.begin()]].insert(tmp.begin(), tmp.end());
				index_right++;
			}
			if (index_right < prod[i].right.size() && !is_deduced_epsilon[prod[i].right[index_right]]) //如果右面遇到一个符号推不空了, 把它的First集也加进来
			{
				Follow[prod[i].right[it - prod[i].right.begin()]].insert(First[prod[i].right[index_right]].begin(), First[prod[i].right[index_right]].end());
				it = find(prod[i].right.begin() + index_right, prod[i].right.end(), curSymbol);
			}
			if (index_right >= prod[i].right.size()) //如果后面的符号全部可以推空
			{
				if (!visited[prod[i].left]) getFollow(prod[i].left); //没有访问过，访问之
				Follow[prod[i].right[it - prod[i].right.begin()]].insert(Follow[prod[i].left].begin(), Follow[prod[i].left].end()); //访问过，把该产生式左部符号的Follow集加进来
				it = prod[i].right.end();
			}

		}
	}
}
/*---------------------------------------------------------------------------------------------------------------
												求Select集合
----------------------------------------------------------------------------------------------------------------*/
void Syntax::getSelect() {
	//求select集合
	for (int i = 0; i < size_of_prod; i++) {
		//遍历产生式集合
		//bool is_all_epsilon = false;
		int index_right = 0;
		while (index_right < prod[i].right.size()
			&& ((non_terminators.find(prod[i].right[index_right]) != non_terminators.end()
				&& is_deduced_epsilon[prod[i].right[index_right]])
				|| prod[i].right[0] == "epsilon" || isUpper(prod[i].right[index_right]))) {
			//如果该产生式右部首符号为非终结符 且 可以推空
			index_right++;
		}
		if (index_right >= prod[i].right.size()) //如果产生式右部符号全部可以推空, alpha =>* epsilon
		{
			set<string> tmpset, first, follow;
			first = int_first(prod[i].right);
			first.erase("epsilon");
			follow = Follow[prod[i].left];
			set_union(first.begin(), first.end(), follow.begin(), follow.end(), inserter(tmpset, tmpset.begin()));
			Select[i] = tmpset;
		}
		else { //alpha ≠>* epsilon
			Select[i] = int_first(prod[i].right);
		}
	}
}
/*---------------------------------------------------------------------------------------------------------------
												求LL1分析表项
----------------------------------------------------------------------------------------------------------------*/
int Syntax::list_of_LL1(string A, string a) //L(A, a) = i ('i' is index of production)
{
	for (int i = 0; i < size_of_prod; i++)
	{
		if (prod[i].left == A && Select[i].find(a) != Select[i].end())
		{
			return i;
		}
	}
	return -1;
}
map<pair<string, string>, int>  Syntax::CreateLL1List()
{
	preProcess();
	dataRevision();
	DFS_First();
	DFS_Follow();
	getSelect();

	set<string> column_of_LL1_list = terminators;
	column_of_LL1_list.insert("￥");
	for (set<string>::iterator it_nt = non_terminators.begin(); it_nt != non_terminators.end(); it_nt++)
		for (set<string>::iterator it_t = column_of_LL1_list.begin(); it_t != column_of_LL1_list.end(); it_t++)
		{
			tmp.insert(make_pair(make_pair(*it_nt, *it_t), list_of_LL1(*it_nt, *it_t)));
		}
	return tmp;
}

void Syntax::Enter()
{
	ifstream in(C_RULES, ios::in); //读取C文法文件
	if (!in.is_open()) { //文法文件打开出错，可能是该文件被用户意外删除所致
		cout << "文法规则文件读取失败，找不到该文件！" << endl;
		return;
	}
	map<pair<string, string>, int> LL1;
	LL1 = CreateLL1List();
	string line_rules;
	set<string> column_of_LL1_list = terminators;
	column_of_LL1_list.insert("￥");
/*	for (set<string>::iterator it_nt = non_terminators.begin(); it_nt != non_terminators.end(); it_nt++)
		for (set<string>::iterator it_t = column_of_LL1_list.begin(); it_t != column_of_LL1_list.end(); it_t++)
		{
			int t = LL1[make_pair(*it_nt, *it_t)];
			if (t > -1)
			{
				cout << *it_nt << "  " << *it_t << "  " << LL1[make_pair(*it_nt, *it_t)] << endl;
				// vector<string>::iterator  ji= prod[t].right;
				/*cout << prod[t].left << "->" ;

				for (int m = 0; m < prod[t].right.size();m++)
				{
					cout << prod[t].right[m];
					cout << " ";
				}
				cout << endl;
				cout << endl;
			}
		}*/
	neigui();
/*	cout << "非终结符" << endl;
	for (set<string>::iterator it_nt = non_terminators.begin(); it_nt != non_terminators.end(); it_nt++)
	{
		cout << *it_nt;
		cout << endl;
	}
	cout << "终结符" << endl;
	for (set<string>::iterator it_t = column_of_LL1_list.begin(); it_t != column_of_LL1_list.end(); it_t++)
	{
		cout << *it_t;
		cout << endl;

	}
	int y = 0; int x = 0;
	for (set<string>::iterator it_nt = non_terminators.begin(); it_nt != non_terminators.end(); it_nt++, y++)
	{
		for (set<string>::iterator it_t = column_of_LL1_list.begin(); it_t != column_of_LL1_list.end(); it_t++, x++)
		{
			a[y][x] = LL1[make_pair(*it_nt, *it_t)] + 1;
		}
		x = 0;
	}
	for (int q = 0; q < 17; q++)
	{
		for (int p = 0; p < 15; p++)
		{
			cout << a[q][p] << "  ";
		}
		cout << endl;
	}*/

}
void Syntax::neigui()
{
	non_ter = this->non_terminators;
	for (int i = 0; i < 500; i++)
	{
		prod1[i].left = this->prod[i].left;
		prod1[i].right = this->prod[i].right;
		prod1[i].right1 = this->prod[i].right1;
	}
	ter = this->terminators;
}

void Syntax::file_enter()
{
	ifstream in(C_RULES, ios::in); //读取C文法文件
	if (!in.is_open()) { //文法文件打开出错，可能是该文件被用户意外删除所致
		cout << "文法规则文件读取失败，找不到该文件！" << endl;
		return;
	}
	fstream datafile;
	datafile.open("test_1.txt", ios::out);
	if (!datafile)
	{
		cout << "文法规则文件读取失败，找不到该文件！" << endl;
		exit(0);
	}
	map<pair<string, string>, int> LL1;
	LL1 = CreateLL1List();
	string line_rules;
	set<string> column_of_LL1_list = terminators;
	column_of_LL1_list.insert("￥");
	for (set<string>::iterator it_nt = non_terminators.begin(); it_nt != non_terminators.end(); it_nt++)
		for (set<string>::iterator it_t = column_of_LL1_list.begin(); it_t != column_of_LL1_list.end(); it_t++)
		{
			int t = LL1[make_pair(*it_nt, *it_t)];
			if (t > -1)
			{
				cout << *it_nt << "  " << *it_t << "  " << LL1[make_pair(*it_nt, *it_t)] + 1 << endl;
				datafile << *it_nt << "  " << *it_t << "  " << LL1[make_pair(*it_nt, *it_t)] + 1 << endl;
			}
		}
	cout << "非终结符" << endl;
	datafile << "非终结符" << endl << endl;
	for (set<string>::iterator it_nt = non_terminators.begin(); it_nt != non_terminators.end(); it_nt++)
	{
		datafile << *it_nt << endl;
		cout << *it_nt;
		cout << endl;
	}
	cout << "终结符" << endl;
	datafile << "终结符" << endl << endl;
	for (set<string>::iterator it_t = column_of_LL1_list.begin(); it_t != column_of_LL1_list.end(); it_t++)
	{
		datafile << *it_t << endl;
		cout << *it_t;
		cout << endl;

	}


	int y = 0; int x = 0; int g;
	for (set<string>::iterator it_nt = non_terminators.begin(); it_nt != non_terminators.end(); it_nt++, y++)
	{
		for (set<string>::iterator it_t = column_of_LL1_list.begin(); it_t != column_of_LL1_list.end(); it_t++, x++)
		{
			a[y][x] = LL1[make_pair(*it_nt, *it_t)] + 1;
			g = x;
		}
		x = 0;
	}
	for (int q = 0; q < y; q++)
	{
		for (int p = 0; p <= g; p++)
		{
			cout << a[q][p] << "  ";
			datafile << a[q][p] << "  ";
		}
		cout << endl;
		datafile << endl;
	}



}




void find()                                             //查找分析表 
{

	Fund m0;

	/*	for(int t1=0; t1<non_ter.size(); t1++)
		{
			if(x.token==non_ter[t1])
			{
				k=t1;
				break;
			}
		}
		for(int t1=0; t1<ter.size(); t1++)
		{
			if(w.token==ter[t1])
			{
				l=t1;
				break;
			}
		}*/
	for (int t1 = ((prod1[value]).right).size() - 1; t1 >= 0; t1--)
	{
		set<string>::iterator it;
		set<string>::iterator it1;
		it = ter.find(prod1[value].right[t1]);
		it1 = non_ter.find(prod1[value].right[t1]);
		if (it != ter.end() || prod1[value].right[t1] == "id" || prod1[value].right[t1] == "int_const"
			|| prod1[value].right[t1] == "float_const" || prod1[value].right[t1] == "char_const" || prod1[value].right[t1] == "string")           //判断产生式右部 
		{
			m0.token = prod1[value].right[t1];
			m0.ty = 't';
			m0.num = 100;
			s1.push(m0);
		}
		else if (it1 != non_ter.end())
		{
			m0.token = prod1[value].right[t1];
			m0.ty = 'n';
			m0.num = 100;
			s1.push(m0);
		}
		else
		{
			m0.token = prod1[value].right[t1];
			m0.ty = 'g';
			m0.num = 100;
			s1.push(m0);
		}
	}

}

void start2() {
	Syntax a;
	a.file_enter();
	cout << "文法分析完成!" << endl;
}
void start()
{
	equal();
	string tm;
	Syntax tty;                                                 //文法自动生成LL1表 
	tty.Enter();
	Fund m0 = { "#",'q',100 };
	s1.push(m0);

	m0 = { "translation_unit",'n',100 };
	s1.push(m0);
//===============词法分析器 open 等方式修改了
	lex1 a;
	a.openfile(in_path);
	while (flag2 == 1)
	{

		flag3 = 1;
		//	w=obj[t++];               //读取下一个
		
		pair<string, token_struct> w1 = a.char_select_word(in_path);
		//cout << "[" << w1.first << "]"<<endl;
		w.token = w1.first;
		if(w1.second.type== tokenType(1))
		{
			w.ty = '1';
		}
		else if(w1.second.type == tokenType(2))
		{
			w.ty = '2';
		}
		else if (w1.second.type == tokenType(3))
		{
			w.ty = '3';
		}
		else if (w1.second.type == tokenType(4))
		{
			w.ty = '4';
		}
		else if (w1.second.type == tokenType(5))
		{
			w.ty = '5';
		}
		else if (w1.second.type == tokenType(6))
		{
			w.ty = '6';
		}

		//cout << w1.second.type << endl;
		w.num = 100;
		while (flag3 == 1)
		{

			x = s1.top();      //将栈顶赋给x并将栈顶弹出
			s1.pop();
			//cout << "栈" << x.token << "   W" << w.token << w.ty<< endl;
			//if (!SEEM.empty()) cout << SEEM.top().first << endl;
			if (x.ty == 'g')            //如果弹出的为语义动作
			{
				if(x.token!="epsilon")
				action_fun(x.token, tokenList.size() - 1, w.token);
				//if(!SEEM.empty())cout << "注意"<<SEEM.top().first << endl;
			}
			else if (x.ty == 't') //如果是终结符
			{
				if (x.token == "id")
				{
					if ((w.ty == 2))
					{
						/*			j=w.token;*/
						flag2 = 1;
					}
					else flag2 = 2;
				}
				else if (x.token == "int_const" || x.token == "float_const")
				{
					if ((w.ty == 4))
					{
						/*		j=w.token;*/
						flag2 = 1;
					}
					else flag2 = 2;
				}
				else if (x.token == "char_const")
				{
					if ((w.ty == 5))
					{
						/*		j=w.token;*/
						flag2 = 1;
					}
					else flag2 = 2;
				}
				else if (x.token == "string")
				{
					if ((w.ty == 6))
					{
						/*		j=w.token;*/
						flag2 = 1;
					}
					else flag2 = 2;
				}
				else if (x.token == w.token)
				{
					flag2 = 1;
					j = w.token;
				}
				else
				{
					flag1 = 2;
					cout << "Line " << tokenList[SEEM.top().second].second.row << ": 语法错误.";
					exit(1);
					//return;
				}
				flag3 = 2;
				flag2 = 1;
			}
			else
			{
				if (x.ty == 'n')        //如果是非终结符
				{

					if ((w.ty == '2'))
					{
						/*			j=w.token;*/
						tm = "id";
					}
					else if ((w.ty == '4'))
					{
						/*		j=w.token;*/
						tm = "int_const";
					}
					else if ((w.ty == '5'))
					{
						tm = "char_con";
					}
					else if ((w.ty == '6'))
					{
						tm = "string_con";
					}
					else
						tm = w.token;
				
					value = tty.list_of_LL1(x.token, tm);
	
					find();
					flag3 = 1;
					flag2 = 2;
				}
				else
				{
					if (w.token == "#")
					{
						flag1 = 1;
						flag2 = 2;
						flag3 = 2;
					}
					else
					{
						flag1 = 4;
						cout << "Line " << tokenList[SEEM.top().second].second.row << ": 语法错误.";
						exit(1);
					}
				}
			}
		}
	}

}




