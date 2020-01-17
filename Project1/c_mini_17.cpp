#include"head.h"
#include<string.h>
#include<iostream>
#include"lex.cpp"
void dox();
void save_file(int num);
//#include"Syn2_detail.cpp"
string in_path;
string pt[45] = { "++","--","+=","-=","*=", "/=","%=", "<<=", ">>=","|=","&=",
		"^=","+","-","*","/","==",">>","<<","=",">=",">","<=","<",";","{","}","(",")","[","]",",","!","%",
		"||","&&","|","&",":","#",".","->","?","~","^" };
string kt[33] = { "typedef","void","const","char","int","unsigned","signed","short",
		"long","float","double","sizeof","struct","enum","union","if","else","do","for","while","return",
		"switch","case","default","break","continue","static","include","define","pragma","goto","cin","cout" };
extern string it[100];                                              //瀹氫箟鏍囪瘑绗?
string ct[100];                                              //瀹氫箟甯告暟
string st[100];                                              //瀹氫箟瀛楃涓?
string zt[100];                                              //瀹氫箟瀛楃 	
vector < pair<string, token_struct> > tokenList;
int _charnum = 0;//璇诲彇鐨勫瓧绗︿綅缃?
using namespace std;
string j, t1, t2;
int f = 49;
int t = 0;
FILE* fp;
int state = 0;
int i = 0;
int state_f;
int numkt = 0;
int numpt = 0;
int numit = 0;
int numct = 0;
int numst = 0;
int numzt = 0;
int flag1 = 1;
int token_id = 0;
int flag2 = 1, flag3 = 1;            //浣滀负鍑犲眰寰幆鐨勬潯浠?
int k = 20, l = 20;
int line_num = 1;
int get_tag = 0;
int value;              //鏍囪鏄摢涓骇鐢熷紡 
map<pair<string, string>, int> tmp;                 //鑷姩鐢熸垚鍒嗘瀽琛ㄧ殑閮ㄥ垎 
int a[100][100];
set<string> ter, non_ter;
vector<Fund> obj;
Fund x, w;
stack<Fund> s1;


/*
int s[17][15]={{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},                     //涔嬪悗鍒嗘瀽鏌ヨ〃闇€瑕佺敤鍒?
			 {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			 {0,3,3,3,0,0,0,0,0,0,0,0,0,0,0},
			 {0,0,0,0,10,0,0,0,0,0,0,0,0,0,0},
			 {0,4,4,4,0,0,0,0,0,0,0,0,0,0,0},
			 {0,5,6,7,0,0,0,0,0,0,0,0,0,0,0},
			 {0,0,0,0,8,0,0,0,0,0,0,0,0,0,0},
			 {0,0,0,0,9,0,0,0,0,0,0,0,0,0,0},
			 {0,0,0,0,11,0,0,0,0,0,0,0,0,0,0},
			 {0,23,23,23,24,0,0,0,0,0,0,0,0,0,0},
			 {0,0,0,0,0,0,0,0,0,0,0,0,26,0,25},
			 {0,0,0,0,27,0,0,0,0,0,0,0,0,28,0},
			 {0,0,0,0,12,12,12,0,0,0,0,0,0,0,0},
			 {0,0,0,0,16,16,16,0,0,0,0,0,0,0,0},
			 {0,0,0,0,0,0,0,15,13,14,0,0,15,0,0},
			 {0,0,0,0,0,0,0,0,19,19,17,18,19,0,0},
			 {0,0,0,0,20,21,22,0,0,0,0,0,0,0,0}};
typedef struct fund
{
	string token;
	char ty;
	int num;
} Fund;

vector<Fund> obj;
Fund x,w;
stack<Fund> s1;
stack<string> SEM;
stack<string> Sy;
stack<string> Type;

typedef struct symbol                              //绗﹀彿琛?
{
	string token;
	string TYPE;
	int num;
} Symbol;
vector<Symbol> SyList;

typedef struct fund1
{
	string p;
	string a1;
	string a2;
	string ob;
} Fund1;
Fund1 f1;
vector<Fund1> obj1;
*/


int main(int argc, char** argv) {
	in_path = "test.txt";

	/*
	//lex
	lex1 a;
	a.openfile(in_path);
	pair<string, token_struct> w = a.char_select_word(in_path);
	while (w.first != "#")
	{

		cout << w.first << "\t" << w.second.type << "\t"
			<< w.second.id << "\t" << w.second.row << "\n";
		w = a.char_select_word(in_path);

	}
	save_file(0);//Token串信息
	cout << "词法分析完成" << endl;
	*/
	//syntax
	/*
	start();
	cout << "生成的四元式为:\n" << endl;
	for (int i = 0; i < obj1.size(); i++)
	{
		cout << "(" << obj1[i].p << " " << obj1[i].a1 << " " << obj1[i].a2 << " " << obj1[i].ob << ")\n";
	}
	cout << "生成的符号表为:\n" << endl;
	for (int i = 0; i < symb.size(); i++) {
		cout << "作用域" << symb[i][0].domainName << ":" << "\n";
		for (int j = 1; j < symb[i].size(); j++) {
			cout << symb[i][j].name << "\t" << symb[i][j].ty << "\t"
				<< symb[i][j].offset << "\t" << symb[i][j].prelayer << "\t"
				<< symb[i][j].mark << "\t" << symb[i][j].if_temp << "\t"
				<< "\n";
		}
	}
	save_file(0);//Token串信息
	save_file(1);//四元式信息
	save_file(2);//各类符号表信息
	cout << "语法分析完成" << endl;
	return 0;
	*/
	//complier
	start();
	save_file(0);//Token串信息
	save_file(1);//四元式信息
	save_file(2);//各类符号表信息
	cout << "前端处理结果:语法无误!" << endl;
	better1();
	save_file(3);//优化四元式信息
	cout << "后端端处理结果:正确优化与翻译!" << endl;
	//cout << "目标代码正确性检验程序准备启动.." << endl;
	
	dox();
	//cout << "启动汇编程序成功!!!" << endl;
	
	return 0;
}

void dox() {
	int t;
	FILE* fp;

	fp = fopen("target.exe", "r+b");
	if (fp == NULL)
		t = 0; // not exist
	else
	{
		fclose(fp);
		t = 1; //exist
	}
	if (t == 1)
	{
		remove("target.exe");
		//printf("Bin File Exist and Deleted!");
	}
	else printf("Bin File no Exist");
	system("DOSBox.exe");
}

void save_file(int num) {
	switch (num)
	{
	case 0: {
		fstream _file("Token.txt", ios::out | ios::trunc);
		if (!_file) {
			cout << "Token.txt 文件打开错误！" << endl;
			exit(1);
		}
		_file << "TOKEN流信息相关介绍:\n"
			<< "数据结构为:1、TOKEN的单词 2、类型 3、在对应符号表中的位置下标 4、在源程序中出现的行数\n"
			<< "类型码分别对应类型:1、关键字 2、标识符 3、界符 4、常数 5、字符串 5、字符 " << "\n";
		for (int i = 0; i < tokenList.size(); i++) {
			_file << tokenList[i].first << "\t" << tokenList[i].second.type << "\t"
				<< tokenList[i].second.id << "\t" << tokenList[i].second.row << "\n";
		}
	}
		break;
	case 1: {
		fstream _file("四元式.txt", ios::out | ios::trunc);
		if (!_file) {
			cout << "四元式.txt 文件打开错误！" << endl;
			exit(1);
		}
		_file << "此四元式是未经优化处理过的四元式!\n";
		for (int i = 0; i < obj1.size(); i++)
		{
			_file <<"("<< obj1[i].p << " " << obj1[i].a1 << " " << obj1[i].a2 << " " << obj1[i].ob <<")\n";
		}
	}
		  break;
	case 2: {
		fstream _file("符号表.txt", ios::out | ios::trunc);
		if (!_file) {
			cout << "符号表.txt 文件打开错误！" << endl;
			exit(1);
		}
		_file << "符号表系统由词法分析表和语义分析表构成:\n"
			<<"词法分析表有关键字表、标识符表、界符表、常数表、字符串表、字符表\n"
			<<"语义分析表分作用域存放,每个域都有域名,主要信息有标识符、类型表下标、偏移量、前一级作用域、活跃信息标注、临时变量记录\n";
		for (int i = 0; i < symb.size(); i++) {
			_file <<"作用域" <<symb[i][0].domainName <<":"<<"\n";
			for (int j = 1; j < symb[i].size(); j++) {
				_file << symb[i][j].name << "\t" << symb[i][j].ty << "\t" 
					<< symb[i][j].offset << "\t" << symb[i][j].prelayer << "\t" 
					<< symb[i][j].mark << "\t" << symb[i][j].if_temp << "\t"
					<< "\n";
			}
		}
	}
		  break;
	case 3: {
		fstream _file("优化四元式.txt", ios::out | ios::trunc);
		if (!_file) {
			cout << "优化四元式.txt 文件打开错误！" << endl;
			exit(1);
		}
		_file << "此处为优化四元式:\n";
		for (int i = 0; i < oobj2.size(); i++)
		{
			for (int m = 0; m < oobj2[i].size(); m++)
			{
				_file << "(" <<oobj2[i][m].p << " " << oobj2[i][m].a1 << " " << oobj2[i][m].a2 << " " << oobj2[i][m].ob << ")\n";
			}
		}
	}
		  break;
	default:
		cout << "编译过程信息文件保存失败!"<<endl;
		exit(1);
		break;
	}
		
}
