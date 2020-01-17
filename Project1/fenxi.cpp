#include"head.h"

int token_ptr = 0; //token索引
SymbolTa tmpsym; //初始化全局符号表
vector<vector<SymbolTa> > symb;       //各级符号表  ！
vector<SymbolTa> tmpvector;
Fund1 f1;
vector<Fund1> obj1;
stack<pair<string, int>> SEEM;            //语义栈
int off=0 ;
bool is_exist_nsy(string k);

int paramId = 1; //标记第几个参数
pair<string, int> funcName;
int qtID = 1;
int nowSynlayer = 0;						 //当前所在符号表的层次
int preSynlayer = -1;						 //前一层符号表的层次
string tmpstr;
bool isel = false;         //判断是否为else if 
pair<string, int> tmp1;       //用于语义栈的一个小暂存 
enum Action_id {
	_push,
	_geq,
	_geq_unary,
	_relation,
	_laber,
	_goto,
	_assi,
	_if,
	_else,
	_while,
	_do,
	_for,
	_for_inc,
	_for_sec_pre,
	_for_sec_post,
	_for_third_pre,
	_for_third_post,
	_do_while,
	_do_while_cmp,
	_end_dowh,
	_logic,
	_scope_init,
	_param,
	_call,
	_end,
	_return,
	_cin,
	_cout,
	_init_decl,
	_init_arr,
	_get_func_name,
	_struct_scopeinit,
	_pop
};

map<string, int> Action_eq;             //语义动作标志映射
void equal()
{
	tmpsym.prelayer = -1;
	tmpvector.push_back(tmpsym);
	symb.push_back(tmpvector);

	Action_eq["PUSH"] = _push;
	Action_eq["GEQ"] = _geq;
	Action_eq["GEQ_UNARY"] = _geq_unary;
	Action_eq["RELATION"] = _relation;
	Action_eq["LABER"] = _laber;
	Action_eq["GOTO"] = _goto;
	Action_eq["ASSI"] = _assi;
	Action_eq["IF"] = _if;
	Action_eq["ELSE"] = _else;
	Action_eq["WHILE"] = _while;
	Action_eq["DO"] = _do;
	Action_eq["FOR"] = _for;
	Action_eq["FORINC"] = _for_inc;
	Action_eq["FORSECPRE"] = _for_sec_pre;
	Action_eq["FORSECPST"] = _for_sec_post;
	Action_eq["FORTHDPRE"] = _for_third_pre;
	Action_eq["FORTHDPST"] = _for_third_post;
	Action_eq["DOWHILE"] = _do_while;
	Action_eq["DOWHCMP"] = _do_while_cmp;
	Action_eq["ENDDOWH"] = _end_dowh;
	Action_eq["LOGIC"] = _logic;
	Action_eq["SCOPEINIT"] = _scope_init;
	Action_eq["PARAM"] = _param;
	Action_eq["CALL"] = _call;
	Action_eq["END"] = _end;
	Action_eq["RETURN"] = _return;
	Action_eq["SCANF"] = _cin;
	Action_eq["PRINT"] = _cout;
	Action_eq["INITDECL"] = _init_decl;
	Action_eq["INITARR"] = _init_arr;
	Action_eq["GETFUNCNM"] = _get_func_name;
	Action_eq["STRUCTINIT"] = _struct_scopeinit;
	Action_eq["POP"] = _pop;

}

pair<int, int> getTypeCode(string type) //返回类码及变量所占内存单元的长度
{
	if (type == "int") return make_pair(0, 2);
	else if (type == "float") return make_pair(1, 2);
	else if (type == "double") return make_pair(2, 4);
	else if (type == "long") return make_pair(3, 4);
	else if (type == "short") return make_pair(4, 1);
	else if (type == "char") return make_pair(5, 2);
	else if (type == "void") return make_pair(-1, 0); //用void定义变量，错
	/*else {
	//数组类型
	Tapel[TapelIndex++].Tval =
	}*/
}

int getType_from_name(string type) //返回类码及变量所占内存单元的长度
{
	int temp_nowlev = nowSynlayer;
	//for (int i = temp_nowlev; i > -1;) {
	while (symb[temp_nowlev][0].prelayer != -1) {
		//先在当前层,找
		for (int j = 1; j < symb[temp_nowlev].size(); j++) {
			if (type == symb[temp_nowlev][j].name) {
				int a = symb[temp_nowlev][j].ty;
				return a;
			}
		}
		//这一层没找到就去找它的外层,所以更新这个temp_nowlev为它的外层的值
		temp_nowlev = symb[temp_nowlev][0].prelayer;
		//如果它的外层的前作用域不是-1,那么它就会重复操作,否则跳出循环(没找到),程序报错
	}
	cout << "Line " << tokenList[SEEM.top().second].second.row << ": ";
	cout << "error L132: “" << SEEM.top().first << "”: 未声明的标识符" << endl;
	exit(1);
	//}
		//它的外层是这个
	return -2;
}
int change(int a)
{
	if (a == 0) return 2;
	else if (a == 1) return 2;
	else if (a == 2) return 4;
	else if (a == 3) return 4;
	else if (a == 4) return 1;
	else if (a == 5) return 2;
	else if (a == -1) return 0;
}


bool isExistInSymb(string id, int index) {                            //查找符号表中是否有  并且融入作用域在其中 
	if (!symb.empty() && index <= symb.size()) {
		if (index >= symb.size()) index = symb.size() - 1;
		/*		vector<SymbolTa>::iterator it = symb[index].begin(), stor_it = it;        */
		int it = 0;
		for (it = 0; it != symb[index].size(); it++) {
			if (symb[index][it].name == id) {
				return true;
			}
		}
		if (it == symb[index].size() || symb[index][0].prelayer != -1) {
			//当前作用域的符号表中未找到该标识符，且该作用域非全局作用域
			return isExistInSymb(id, symb[index][0].prelayer);
		}
		else if (it == symb[index].size() || symb[index][0].prelayer == -1) {
			return false;
		}
	}
	else return false;
}

int getOffset(string id, int index) {
	if (!symb.empty() && index <= symb.size()) {
		if (index >= symb.size()) index = symb.size() - 1;
		int it = 0;
		for (it = 0; it != symb[index].size(); it++) {
			if (symb[index][it].name == id) {
				return symb[index][it].offset;
			}
		}
		if (it == symb[index].size() || symb[index][0].prelayer != -1) {
			//当前作用域的符号表中未找到该标识符，且该作用域非全局作用域
			return getOffset(id, symb[index][0].prelayer);
		}
		else if (it == symb[index].size() || symb[index][0].prelayer == -1) {
			return -1;
		}
	}
	else return -1;
}


void action_fun(string Act, int Tokenstation, string nowstr)
{
	vector<SymbolTa> tmptable; //临时单符号表
	SymbolTa tmpNode; //临时单符号表结点
	stack<pair<string, int>> tmpStack;
	switch (Action_eq[Act])
	{
	case Action_id::_push:
		SEEM.push(make_pair(nowstr, Tokenstation));
		break;
	case Action_id::_cin:
		if (tokenList[SEEM.top().second].second.type == _identifier) {
			f1 = { "cin","_","_",SEEM.top().first };
			obj1.push_back(f1);
		}
		break;
	case Action_id::_cout:
		if (!is_exist_nsy(SEEM.top().first))
		{
			if (!SEEM.empty() && (tokenList[SEEM.top().second].second.type == _identifier || tokenList[SEEM.top().second].second.type == _constnum)) {
				tmp1 = SEEM.top();
				if (tokenList[SEEM.top().second].second.type = _identifier) {
					//输出的是变量的值
					if (!isExistInSymb(SEEM.top().first, nowSynlayer)) {
						cout << "Line " << tokenList[SEEM.top().second].second.row << ": ";
						cout << "error C2065: “" << SEEM.top().first << "”: 未声明的标识符" << endl;
						exit(1);
					}
				}

				SEEM.pop(); //输出内容出栈
				if (!SEEM.empty() && tokenList[SEEM.top().second].second.type == _constnum)
				{
					f1 = { "cout",SEEM.top().first,"_",tmp1.first };
					obj1.push_back(f1);
					SEEM.pop();
				}
				//else SEM.push(tmp);
				else
				{
					f1 = { "cout","10","_",tmp1.first };
					obj1.push_back(f1);
				}
			}
		}
		break;
	

	case Action_id::_geq:
		tmpstr = SEEM.top().first ; //cout << tmpstr << "is out!" << endl;//for test
		SEEM.pop();
		f1.p = SEEM.top().first;
		/*		quat << SEEM.top().first << " ";   //cout << SEM.top().first << "is out!" << endl;//for test*/
		SEEM.pop();
		f1.a1 = SEEM.top().first;
		f1.a2 = tmpstr;
		f1.ob = "t" + to_string(qtID);
		obj1.push_back(f1);
//		for (int i = 0;i < obj1.size();i++)
//			cout << obj1[i].p << " " << obj1[i].a1 << " " << obj1[i].a2 << " " << obj1[i].ob << endl;

//将临时变量填写到符号表中
		tmpNode.prelayer = preSynlayer;
		tmpNode.name = f1.ob;
		//tmpNode.ty =  getTypeCode(tokenList[SEEM.top().second].second.type).first;
		//一定是从我现在的作用域找,如果再不到,在往它的外层找,如果都找不到就报错.所以用一个函数获取
		if (isnumm(f1.a1)) tmpNode.ty = 0;
		else tmpNode.ty = getType_from_name(f1.a1);//返回 -1 应该报错
		tmpNode.if_temp = 't';
		tmpNode.offset = off;
		off += change(tmpNode.ty);
		//tmptable.push_back(tmpNode);
		symb[nowSynlayer].push_back(tmpNode);


		SEEM.pop();
		SEEM.push(make_pair("t" + to_string(qtID), -1));
		qtID++;
		break;

	case Action_id::_relation:
		if (SEEM.top().second == -1 || (tokenList[SEEM.top().second]).second.type == _identifier || (tokenList[SEEM.top().second]).second.type == _constnum) {
			if ((tokenList[SEEM.top().second]).second.type == _identifier) {
				//检查是否未定义
				if (SEEM.top().second != -1 && !isExistInSymb(SEEM.top().first, nowSynlayer)) {
					cout << "Line " << (tokenList[SEEM.top().second]).second.row<< ": ";
					cout << "error C2065: “" << SEEM.top().first << "”: 未声明的标识符" << endl;
					exit(1);
				}
			}
			tmp1 = SEEM.top();
			SEEM.pop();
			if (SEEM.top().first == "<" || SEEM.top().first == ">" || SEEM.top().first == "<=" || SEEM.top().first == ">="
				|| SEEM.top().first == "==" || SEEM.top().first == "!=") {
				f1 = {SEEM.top().first," "," "," " };
				SEEM.pop();
				if ((tokenList[SEEM.top().second]).second.type == _identifier) {
					//检查是否未定义
					if (SEEM.top().second != -1 && !isExistInSymb(SEEM.top().first, nowSynlayer)) {
						//cout << "!isExistInSynbl(" << SEM.top().first << ", " << curSynblLev << ")" << endl;
						cout << "Line " << (tokenList[SEEM.top().second]).second.row << ": ";
						cout << "error C2065: “" << SEEM.top().first << "”: 未声明的标识符" << endl;
						exit(1);
					}
				}
				f1.a1 = SEEM.top().first;
				f1.a2 = tmp1.first;
				f1.ob = "t" + to_string(qtID);
				obj1.push_back(f1);

				//将临时变量填写到符号表中
				tmpNode.prelayer = preSynlayer;
				tmpNode.name = f1.ob;
				//tmpNode.ty =  getTypeCode(tokenList[SEEM.top().second].second.type).first;
				//一定是从我现在的作用域找,如果再不到,在往它的外层找,如果都找不到就报错.所以用一个函数获取
				if (isnumm(f1.a1)) tmpNode.ty = 0;
				else tmpNode.ty = getType_from_name(f1.a1);//返回 -1 应该报错
				tmpNode.if_temp = 't';
				tmpNode.offset = off;
				off += change(tmpNode.ty);
				//tmptable.push_back(tmpNode);
				symb[nowSynlayer].push_back(tmpNode);
				SEEM.pop();
				SEEM.push(make_pair("t" + to_string(qtID), -1));
				qtID++;
			}
			else SEEM.push(tmp1);
		}
		break;
	case Action_id::_geq_unary:
		if (is_exist_nsy(SEEM.top().first)|| SEEM.top().first=="return_val") break;
		if (!SEEM.empty() && (SEEM.top().first == "++" || SEEM.top().first == "--")) {
			f1.p = SEEM.top().first[0];
			SEEM.pop();
			f1.a1 = SEEM.top().first;
			f1.a2 = "1";
			f1.ob = "t" + to_string(qtID);
			obj1.push_back(f1);

			//将临时变量填写到符号表中
			tmpNode.prelayer = preSynlayer;
			tmpNode.name = f1.ob;
			//tmpNode.ty =  getTypeCode(tokenList[SEEM.top().second].second.type).first;
			//一定是从我现在的作用域找,如果再不到,在往它的外层找,如果都找不到就报错.所以用一个函数获取
			if (isnumm(f1.a1)) tmpNode.ty = 0;
			else tmpNode.ty = getType_from_name(f1.a1);//返回 -1 应该报错
			tmpNode.if_temp = 't';
			tmpNode.offset = off;
			off += change(tmpNode.ty);
			//tmptable.push_back(tmpNode);
			symb[nowSynlayer].push_back(tmpNode);


			tmp1 = SEEM.top();
			SEEM.pop();
			f1.p = "=";
			f1.a1 = "t" + to_string(qtID);
			f1.a2 = "_";
			f1.ob = tmp1.first;
			obj1.push_back(f1);
			SEEM.push(tmp1);
			qtID++;
		}
		else if (!SEEM.empty() && (tokenList[SEEM.top().second].second.type == _identifier || SEEM.top().second == -1)) {
			tmp1 = SEEM.top();
			SEEM.pop();
			if (!SEEM.empty() && (SEEM.top().first == "++" || SEEM.top().first == "--")) {
				f1.p = SEEM.top().first[0];
				f1.a1 = tmp1.first;
				f1.a2 = "1";
				f1.ob = "t" + to_string(qtID);
				obj1.push_back(f1);


				//将临时变量填写到符号表中
				tmpNode.prelayer = preSynlayer;
				tmpNode.name = f1.ob;
				//tmpNode.ty =  getTypeCode(tokenList[SEEM.top().second].second.type).first;
				//一定是从我现在的作用域找,如果再不到,在往它的外层找,如果都找不到就报错.所以用一个函数获取
				if (isnumm(f1.a1)) tmpNode.ty = 0;
				else tmpNode.ty = getType_from_name(f1.a1);//返回 -1 应该报错
				tmpNode.if_temp = 't';
				tmpNode.offset = off;
				off += change(tmpNode.ty);
				//tmptable.push_back(tmpNode);
				symb[nowSynlayer].push_back(tmpNode);

				f1.p = "=";
				f1.a1 = "t" + to_string(qtID);
				f1.a2 = "_";
				f1.ob = tmp1.first;
				obj1.push_back(f1);
				SEEM.pop();
				SEEM.push(tmp1);
				qtID++;
			}
			else SEEM.push(tmp1);
		}
		else if (!SEEM.empty() && (SEEM.top().second == -1 || tokenList[SEEM.top().second].second.type == _identifier || tokenList[SEEM.top().second].second.type == _constnum)) {
			tmp1 = SEEM.top();
			SEEM.pop();
			if (!SEEM.empty() && SEEM.top().first == "~") {
				f1.p = "not";
				f1.a1 = tmp1.first;
				f1.a2 = "_";
				f1.ob = "t" + to_string(qtID);;
				obj1.push_back(f1);
				//将临时变量填写到符号表中
				tmpNode.prelayer = preSynlayer;
				tmpNode.name = f1.ob;
				//tmpNode.ty =  getTypeCode(tokenList[SEEM.top().second].second.type).first;
				//一定是从我现在的作用域找,如果再不到,在往它的外层找,如果都找不到就报错.所以用一个函数获取
				if (isnumm(f1.a1)) tmpNode.ty = 0;
				else tmpNode.ty = getType_from_name(f1.a1);//返回 -1 应该报错
				tmpNode.if_temp = 't';
				tmpNode.offset = off;
				off += change(tmpNode.ty);
				//tmptable.push_back(tmpNode);
				symb[nowSynlayer].push_back(tmpNode);

				SEEM.pop();
				SEEM.push(make_pair("t" + to_string(qtID), -1));
				qtID++;
			}
			else SEEM.push(tmp1);
		}
		break;
	case Action_id::_logic:
		if (!SEEM.empty() && (SEEM.top().second == -1 || (tokenList[SEEM.top().second]).second.type == _identifier || (tokenList[SEEM.top().second]).second.type == _constnum))
		{
			tmp1 = SEEM.top();
			SEEM.pop();
			if (!SEEM.empty() && (SEEM.top().first == "&&" || SEEM.top().first == "&")) {
				SEEM.pop(); //&& is out!
				f1 = { "and",SEEM.top().first,tmp1.first,"t" + to_string(qtID) };
				obj1.push_back(f1);
				SEEM.pop();
				SEEM.push(make_pair("t" + to_string(qtID), -1));
				qtID++;
			}
			else if (!SEEM.empty() && (SEEM.top().first == "||" || SEEM.top().first == "|")) {
				SEEM.pop(); //|| is out!
				f1 = { "or",SEEM.top().first,tmp1.first,"t" + to_string(qtID) };
				obj1.push_back(f1);
				SEEM.pop();
				SEEM.push(make_pair("t" + to_string(qtID), -1));
				qtID++;
			}
			else if (!SEEM.empty() && (SEEM.top().first == "^")) {
				SEEM.pop(); //^ is out!
				f1 = { "or",SEEM.top().first,tmp1.first,"t" + to_string(qtID) };
				obj1.push_back(f1);
				SEEM.pop();
				SEEM.push(make_pair("t" + to_string(qtID), -1));
				qtID++;
			}
			else if (!SEEM.empty() && (SEEM.top().first == "<<")) {
				SEEM.pop();
				f1 = { "shl",SEEM.top().first,tmp1.first,"t" + to_string(qtID) };
				obj1.push_back(f1);
				SEEM.pop();
				SEEM.push(make_pair("t" + to_string(qtID), -1));
				qtID++;
			}
			else if (!SEEM.empty() && (SEEM.top().first == ">>")) {
				SEEM.pop();
				f1 = { "shr",SEEM.top().first,tmp1.first,"t" + to_string(qtID) };
				obj1.push_back(f1);
				SEEM.pop();
				SEEM.push(make_pair("t" + to_string(qtID), -1));
				qtID++;
			}
			else SEEM.push(tmp1);
		}
		break;
	case Action_id::_if:
		if (!SEEM.empty() && SEEM.top().second == -1) {         //说明是tmp 
			if (isel == false) { //只有if
				preSynlayer = nowSynlayer;
				//cout << "preSynblLev = " << preSynblLev << endl;//for test
				nowSynlayer = symb.size(); //出了函数作用域后，要cur = pre;
				//cout << "curSynblLev = " << curSynblLev << endl;//for test
				//tmpNode.name = nullptr;
				tmpNode.prelayer = preSynlayer;
				tmpNode.domainName = "if";
				tmptable.push_back(tmpNode);
				symb.push_back(tmptable);
				//cout << "符号总表创建新节点" << synbl.size() - 1 << ": prelev = " << tmpNode.prelev << endl;//for test
				//cout << "------------IF------------" << endl;//for test
				f1.p = "if";
				f1.a1 = SEEM.top().first;
				f1.a2 = "_";
				f1.ob = to_string(nowSynlayer);
				obj1.push_back(f1);
				SEEM.pop();
			}
			else {
				//else if...
				f1.p = "if";
				f1.a1 = SEEM.top().first;
				f1.a2 = "_";
				f1.ob = to_string(nowSynlayer);
				obj1.push_back(f1);
				SEEM.pop();
			}
		}
		break;
	case Action_id::_else:
		preSynlayer = nowSynlayer;
		nowSynlayer = symb.size(); //出了函数作用域后，要cur = pre;
		tmpNode.prelayer = preSynlayer;
		tmpNode.domainName = "else";
		tmptable.push_back(tmpNode);
		symb.push_back(tmptable);
		isel = true;
		f1.p = "el";
		f1.a1 = "_";
		f1.a2 = "_";
		f1.ob = to_string(nowSynlayer);
		obj1.push_back(f1);
		break;
	case Action_id::_while:
		preSynlayer = nowSynlayer;
		nowSynlayer = symb.size(); //出了函数作用域后，要cur = pre;
		tmpNode.prelayer = preSynlayer;
		tmpNode.domainName = "while";
		tmptable.push_back(tmpNode);
		symb.push_back(tmptable);
		f1.p = "wh";
		f1.a1 = "_";
		f1.a2 = "_";
		f1.ob = to_string(nowSynlayer);
		obj1.push_back(f1);
		break;
	case Action_id::_do:
		if (SEEM.top().second == -1) {
			f1.p = "do";
			f1.a2 = "_";
			f1.ob = "_";
			f1.a1 = SEEM.top().first;
			obj1.push_back(f1);
			SEEM.pop();
		}
		break;
	case Action_id::_laber:
		f1 = {"label","_","_",SEEM.top().first};
		obj1.push_back(f1);
		SEEM.pop();
		break;
	case Action_id::_goto:
		f1 = { "goto","_","_",nowstr };
		obj1.push_back(f1);
		break;
	case Action_id::_scope_init:
		if ((tokenList[SEEM.top().second]).second.type == _identifier) {
			preSynlayer = nowSynlayer;   //当前层次变为上一层
			nowSynlayer = symb.size();  //当前层为符号总表中最新开辟的一层
			tmpNode.prelayer = preSynlayer;//新作用域表的指针域记录上一层的索引号
			tmpNode.domainName = SEEM.top().first;//记录新作用域的域名
			tmptable.push_back(tmpNode);
			symb.push_back(tmptable);   //在符号总表中为新作用域开辟空间

			tmp1 = SEEM.top();
			SEEM.pop();
			if ((tokenList[SEEM.top().second]).second.type == _keyword) {
				f1.p = "fun_do";
				f1.a1 = tmp1.first;
				f1.a2 = "_";
				f1.ob = to_string(nowSynlayer);
				obj1.push_back(f1);
				//quat << "func "<<curSynblLev<<" # " << tmp1.first << endl; //generate function quat
				SEEM.pop(); //data type is out of stack!
				while (!SEEM.empty()) { //开启一个新的作用域--函数，把SEM栈中之前的内容全部清空
					SEEM.pop();
				}
			}
			else SEEM.push(tmp1);
		}
		break;
	case Action_id::_end:
		//cout << "---------END---------" << endl;//for test
		/*if (synbl[curSynblLev][0].domainName == "max")
			cout << "stop";*/
		f1.p = "end";
		f1.a1 = "_";
		f1.a2 = "_";
		f1.ob = symb[nowSynlayer][0].domainName;
		obj1.push_back(f1);
		//cout<< "end # # " << synbl[curSynblLev][0].domainName << endl;//for test
		nowSynlayer = preSynlayer;					//恢复当前作用域为上一层作用域
		preSynlayer = symb[preSynlayer][0].prelayer; //恢复上一层作用为上上层作用域
		//cout << "preSynblLev = " << preSynblLev << endl;//for test
		//cout << "---------END---------" << endl;//for test
//		for (int i = 0;i < obj1.size();i++)
//						cout << obj1[i].p << " " << obj1[i].a1 << " " << obj1[i].a2 << " " << obj1[i].ob << endl;
		if (!SEEM.empty())
		{
			SEEM.pop() ;
		}
		break;
	case Action_id::_return:
		if (!SEEM.empty() && (SEEM.top().second == -1 || (tokenList[SEEM.top().second]).second.type == _identifier || (tokenList[SEEM.top().second]).second.type == _constnum)) {
			f1.p = "re";
			f1.a2 = "_";
			f1.ob = "_";
			f1.a1 = SEEM.top().first;
			obj1.push_back(f1);
		}
		else if (SEEM.empty()) {
			f1.p = "re";
			f1.a2 = "_";
			f1.ob = "_";
			f1.a1 = "void";
			obj1.push_back(f1);
		}
		break;
			case Action_id::_for:
				preSynlayer = nowSynlayer;
				nowSynlayer = symb.size();
				tmpNode.prelayer = preSynlayer;
				tmpNode.domainName = "for";
				tmptable.push_back(tmpNode);
				symb.push_back(tmptable);
				f1 = {"for","_","_",to_string(nowSynlayer) };
				obj1.push_back(f1);
				break;
			case Action_id::_for_inc:
				if (symb[nowSynlayer][0].domainName == "for") {
					f1 = { "jmp","_","_","inc" };
					obj1.push_back(f1);
				}
				break;
			case Action_id::_for_sec_pre:
				if (symb[nowSynlayer][0].domainName == "for") {
					f1 = { "label","_","_","cmp" };
					obj1.push_back(f1);
				}
				break;
			case Action_id::_for_sec_post:
				if (symb[nowSynlayer][0].domainName == "for") {
					if (SEEM.top().second == -1) {
						f1 = { "do",SEEM.top().first,"_","_" };
						obj1.push_back(f1);
						f1 = { "jmp","_","_" ,"end" };
						obj1.push_back(f1);
						f1 = { "jmp","_","_" ,"start" };
						obj1.push_back(f1);
					}
				}
				break;
			case Action_id::_for_third_pre:
				if (symb[nowSynlayer][0].domainName == "for") {
					f1 = { "label","_","_","inc" };
					obj1.push_back(f1);
				}
				break;
			case Action_id::_for_third_post:
				if (symb[nowSynlayer][0].domainName == "for") {
					f1 = { "jmp","_","_" ,"cmp" };
					obj1.push_back(f1);
					f1 = { "label","_","_","start" };
					obj1.push_back(f1);
				}
				break;
	case Action_id::_do_while:
		preSynlayer = nowSynlayer;
		nowSynlayer = symb.size();
		tmpNode.prelayer = preSynlayer;
		tmpNode.domainName = "dowhile";
		tmptable.push_back(tmpNode);
		symb.push_back(tmptable);
		f1 = { "dowhile","_","_",to_string(nowSynlayer) };
		obj1.push_back(f1);
		f1 = { "lable","_","_","start" };
		obj1.push_back(f1);
		break;
	case Action_id::_do_while_cmp:
		if (symb[nowSynlayer][0].domainName == "dowhile") {
			f1 = { "dowhcmp","_","_","_" };
			obj1.push_back(f1);
		}
		break;

	case Action_id::_end_dowh:
		if (SEEM.top().second == -1) {
			f1 = { "do",SEEM.top().first,"_","_" };
			obj1.push_back(f1);
			f1 = { "jmp","_","_","start" };
			obj1.push_back(f1);
			//quat << "end # # dowhile" << endl;
		}
		if (!SEEM.empty())
		{
			SEEM.pop() ;
		}
		break;
	case Action_id::_assi:
		if (!SEEM.empty()) {
			tmp1 = SEEM.top();
			SEEM.pop(); //number is out!
		/*	if (tmp1.first == "1")
				cout << "stop";*/
			if (tmp1.first == "return_val") {
				SEEM.pop(); //pop =
				if (!SEEM.empty() && (tokenList[SEEM.top().second]).second.type == _identifier) {
					f1 = { "mov",SEEM.top().first,"_","DX" };
					obj1.push_back(f1);
				}
			}
			else if (!SEEM.empty() && SEEM.top().first == "=") {
				//quat << SEM.top().first << " " << tmp1.first << " # ";
				SEEM.pop(); //= is out
				//cout << "curSynblLev = " << curSynblLev << endl;//for test
				//cout << "preSynblLev = " << preSynblLev << endl;//for test
				/*if (SEM.top().first == "b")
					cout << "stop";*/
				if (!SEEM.empty() && (tokenList[SEEM.top().second]).second.type == _constnum) {
					//数组赋值
					pair<string, int> arroffset = SEEM.top();
					SEEM.pop(); //a[index]的index出栈
					//if(stoi(arroffset.first)>=)//@@@@@@未解决用户操作数组越界的情况
					f1 = { "movs",to_string(getOffset(SEEM.top().first, nowSynlayer) + stoi(arroffset.first) * 2),"_",tmp1.first };
					obj1.push_back(f1);
					//目前支持int型数组，int长度为2
				}

				else if (!SEEM.empty() && ((tokenList[SEEM.top().second]).second.type == _identifier || SEEM.top().second == -1)) {
					f1 = { "=",tmp1.first,"_","" };
					if (SEEM.top().second != -1 && !isExistInSymb(SEEM.top().first, nowSynlayer)) {
						//常规变量赋值
						cout << "Line " << (tokenList[SEEM.top().second]).second.row << ": ";
						cout << "error C2065: “" << SEEM.top().first << "”: 未声明的标识符" << endl;
						exit(1);
					}
					f1.ob = SEEM.top().first;
					obj1.push_back(f1);
				}
				//SEM.pop(); //delete it!
			}
			/*else if (!SEEM.empty() && regex_match(SEM.top().first, regex("\\+=|-=|\\*=|/=|%=|&=|\\|=|^=|>>=|<<="))) {
						if (regex_match(SEM.top().first, regex("\\+=|-=|\\*=|/=|%=")))
							quat << SEM.top().first[0] << " ";
						else if (SEM.top().first == "&=") quat << "and ";
						else if (SEM.top().first == "|=") quat << "or ";
						else if (SEM.top().first == "^=") quat << "not ";
						else if (SEM.top().first == ">>=") quat << "shr ";
						else if (SEM.top().first == "<<=") quat << "shl ";
						SEM.pop();
						if (SEEM.top().second != -1 && !isExistInSymb(SEEM.top().first, nowSynlayer)) {
							cout << "Line " << tokenSeq[SEM.top().second].row << ": ";
							cout << "error C2065: “" << SEM.top().first << "”: 未声明的标识符" << endl;
							exit(1);
						}
						quat << SEM.top().first << " " << tmp1.first << " qtmp" + to_string(qtmpID) << endl;
						quat << "= qtmp" + to_string(qtmpID) << " # " << SEM.top().first << endl;
						//SEM.pop();//delete it!
						qtmpID++;
					}*/
			else if (!SEEM.empty() && !is_exist_nsy(SEEM.top().first)&& !is_exist_nsy(tmp1.first))
			{
				//cout << SEEM.top().first << endl;
			if (!SEEM.empty() && tokenList[SEEM.top().second].second.type == _identifier && tokenList[tmp1.second].second.type == _constnum) {
				//数组元素赋值给一个变量,arr_index is out in tmp1, int b = a .1
				int memPos = getOffset(SEEM.top().first, nowSynlayer) + stoi(tmp1.first) * 2;
				pair<string, int> tmpa = SEEM.top();
				SEEM.pop(); //a is out
				if (!SEEM.empty() && SEEM.top().first == "=") {
					SEEM.pop();//= is out
					f1 = { "movs",SEEM.top().first ,"_",to_string(memPos) };
					obj1.push_back(f1);
				}
				else {
					SEEM.push(tmpa);
					SEEM.push(tmp1);
				}
			}
			else SEEM.push(tmp1);
		}
			else SEEM.push(tmp1);
		}
		break;

	case Action_id::_init_decl:
	{
		if (!SEEM.empty() && (tokenList[SEEM.top().second]).second.type != _keyword) 
		{
			int i;
			for (i = 1; !symb.empty() && nowSynlayer < symb.size() && i < symb[nowSynlayer].size(); i++) {
				//检查该变量是否已经定义过
				if (symb[nowSynlayer][i].name == SEEM.top().first) {
					//重复定义
					cout << "Line " << (tokenList[SEEM.top().second]).second.row << ": ";
					cout << "error C2086: “int " << SEEM.top().first << "”: 重定义." << endl;
					exit(1); //重定义直接停止编译
				}
			}
			tmp1 = SEEM.top();
		//	cout << "测试" << tmp1.first << endl;
			tmpNode.prelayer = preSynlayer;
			tmpNode.name = tmp1.first;
			tmpNode.if_temp = "nt";
			stack<pair<string, int>> tmpSEM;
			while (!SEEM.empty() && (tokenList[SEEM.top().second]).second.type != _keyword) {
				tmpSEM.push(SEEM.top());
				SEEM.pop();
			}
			tmpNode.ty = getTypeCode(SEEM.top().first).first; //填写符号表的TYP类型区
			if (tmpNode.ty == -1) {
				//void类型被用于定义变量，错误处理
				cout << "Line " << (tokenList[tmp1.second]).second.row << ": ";
				cout << "error C2182: “" << tmpNode.name << "”: 非法使用“void”类型." << endl;
				exit(1);	
			} 
			tmpNode.offset = off; //记录之前的偏移量
			off += getTypeCode(SEEM.top().first).second; //更新当前偏移量
			//cout << "$$$$$$$$$$$$$$$$$$$$$$$$$ off = " << off << endl; //for test
			//SEM.pop(); //int出栈，数据类型出栈????????????????????????
			if (!symb.empty() && nowSynlayer < symb.size()) {
				symb[nowSynlayer].push_back(tmpNode); //单符号表已经存在的情况
			//	cout << endl << tmpNode.name << "进入synbl[" << curSynblLev << "]" << endl; //for test
			}
			else {
				//当前层次是首次填写，即单符号表不存在的情况
				tmptable.push_back(tmpNode); //当前结点加入单符号表中
				symb.push_back(tmptable);
				//	cout << "创建总表新节点" << synbl.size() - 1 << ": " << tmpNode.name << "进入";
				//	cout << ", prevel = " << tmpNode.prelev << endl;//for test
			}

			while (!tmpSEM.empty()) { //暂存的项倒回原语义栈中
				SEEM.push(tmpSEM.top());
				tmpSEM.pop();
			}
		}
	}
//	cout<<"看符号表是否填入"<<(symb[1]).size();
/*	for(int i=0;i<(symb[1]).size()-1;i++)
	{
	cout << symb[1][0].domainName <<symb[1][i+1].name << " " << symb[1][i+1].prelayer << " " << symb[1][i+1].ty << " "<<symb[1][i+1].offset << endl;
	}*/
	break;
	case Action_id::_init_arr:
		if (!SEEM.empty() && (tokenList[SEEM.top().second]).second.type == _constnum) {
			//	cout << "$$$$$$$$$$$$$$$$$$$$$$$$$ off = " << off << endl; //for test
			tmp1 = SEEM.top();
			SEEM.pop();
			if (!SEEM.empty() && (tokenList[SEEM.top().second]).second.type == _identifier) {
				//vector<SymbolTable> tmpSynbl; //临时单符号表
				//SymbolTable tmpNode; //临时单符号表结点
				tmpNode.name = SEEM.top().first;
				tmpNode.if_temp = "nt" ;
				pair<string, int> idtmp = SEEM.top();
				SEEM.pop();//id is out!
				tmpNode.prelayer = preSynlayer;
				tmpNode.offset = off;
				if (!SEEM.empty() && (tokenList[SEEM.top().second]).second.type == _keyword) {
					tmpNode.ty = getTypeCode(SEEM.top().first).first;
					off += getTypeCode(SEEM.top().first).second * (stoi(tmp1.first));
					symb[nowSynlayer].push_back(tmpNode);
					//SEM.pop();//int is out
					//SEM.push(idtmp);
				}
			}
		}
		break;
	case Action_id::_get_func_name:
		if (!SEEM.empty() && tokenList[SEEM.top().second].second.type == _identifier) {
			paramId = 1;
			funcName = SEEM.top();
		}
		break;
	case Action_id::_param:
		//生成实参四元式
		if (SEEM.top().second == -1 || tokenList[SEEM.top().second].second.type == _identifier || tokenList[SEEM.top().second].second.type == _constnum) {
			//quat << "param # # " << SEM.top().first << endl;
			int i;
			for (i = 0; i < symb.size(); i++) {
				if (symb[i][0].domainName == funcName.first) {
					f1 = { "mov", to_string(symb[i][paramId].offset) ,"_",SEEM.top().first };
					obj1.push_back(f1);
					break;
				}
			}
			if (i < symb.size()) //找到了
				SEEM.pop();
			else {
				//没找到该函数
				cout << "Line " << tokenList[funcName.second].second.row << ": ";
				cout << "'" << funcName.first << "' was not declared in this scope" << endl;
				exit(1);
			}
			paramId++;
		}
		break;
	case Action_id::_call:
		cout << "调用函数" << endl;
		if (tokenList[SEEM.top().second].second.type == _identifier) {
			f1 = { "call", "_" ,"_",SEEM.top().first };
			obj1.push_back(f1);
			SEEM.pop();
			SEEM.push(make_pair("return_val", -3));
		}
		else {
			cout << "Line " << tokenList[funcName.second].second.row << ": ";
			cout << "'" << funcName.first << "' was not declared as an identifier" << endl;
			exit(1);
		}
		break;
/*	case Action_id::_pop:
		while (!SEEM.empty()) {
			if (tokenList[SEEM.top().second].second.type == _constnum)
			{	//赋值语句 stack<pair<string, int>> SEM;
				string constnum = SEEM.top().first; //记录下该常数
				SEEM.pop(); //值出栈
				token_ptr = SEEM.top().second; //id的Token索引
				int i;
				for (i = 0; !symb.empty() && nowSynlayer < symb.size() && i < symb[nowSynlayer].size(); i++) {
					//检查该变量是否已经定义过
					if (symb[nowSynlayer][i].name == identifier_table[tokenList[SEEM.top().second].second.id].name) {
						//重复定义
						cout << "Line " << tokenList[SEEM.top().second].second.row << ": ";
						cout << "error C2086: “int " << identifier_table[tokenList[SEEM.top().second].second.id].name << "”: 重定义." << endl;
						//SEM.pop();
						exit(1); //重定义直接停止编译
						//break;
					}
				}
				//if (!synbl.empty() && i < synbl[curSynblLev].size()) continue; //存在重定义
				tmpNode.name = SEEM.top().first;
				tmpNode.prelayer = preSynlayer;
				//cout << "tmpNode.prelev = " << tmpNode.prelev << endl;//for test
				SEM.pop(); //id出栈
				stack<pair<string, int>> tmpSEM;
				while (!SEEM.empty() && tokenList[SEEM.top().second].second.type != _keyword) {
					tmpSEM.push(SEEM.top());
					SEEM.pop();
				} //暂且过滤掉同一行代码前面的定义性语句
				string tmptype = "";
				while (!SEM.empty() && tokenList[SEEM.top().second].second.type == _keyword) {
					//填符号表的Type类型区，该区链接到类型表
					tmptype += (SEEM.top().first + " ");
					tmpSEM.push(SEEM.top());
					SEM.pop();
				}
				tmptype = tmptype.substr(0, tmptype.size() - 1); //去掉最后面的空格
				tmpNode.ty = getTypeCode(tmptype).first; //填写符号表的TYP类型区 
				//identifier_table[synbl_ptr].typ = getTypeCode(tmptype);   
				if (tmpNode.ty == -1) {
					//void类型被用于定义变量，错误处理
					cout << "Line " << tokenList[token_ptr].second.row << ": " << endl;
					cout << "error C2182: “" << SEEM.top().first << "”: 非法使用“void”类型" << endl;
					exit(1);
				}
				tmpNode.offset = off;
				off += getTypeCode(tmptype).second;
				//cout << "$$$$$$$$$$$$$$$$$$$$$$$$$ off = " << off << endl; //for test
				if (!symb.empty() && nowSynlayer < symb.size()) {
					symb[nowSynlayer].push_back(tmpNode); //单符号表已经存在的情况
				}
				//if (curSynblLev >= synbl.size()) {
				else {
					//当前层次是首次填写，即单符号表不存在的情况
					tmptable.push_back(tmpNode); //当前结点加入单符号表中
					symb.push_back(tmptable);
					//cout << "创建新节点" << synbl.size() - 1 << ": prelev = " << tmpNode.prelev << endl;//for test
				}
				f1 = { "=",constnum,"#",tmpNode.name };
				obj1.push_back(f1);
				while (!tmpSEM.empty()) { //暂存的项倒回原语义栈中
					SEEM.push(tmpSEM.top());
					tmpSEM.pop();
				}
			}
			else if (tokenList[SEEM.top().second].second.type == _identifier) {
				//定义性语句 int b;
				int i;
				for (i = 0; !symb.empty() && nowSynlayer < symb.size() && i < symb[nowSynlayer].size(); i++) {
					//检查该变量是否已经定义过
					if (symb[nowSynlayer][i].name == SEEM.top().first) {
						//重复定义
						cout << "Line " << tokenList[SEEM.top().second].second.row << ": ";
						cout << "error C2086: “int " << SEEM.top().first << "”: 重定义." << endl;
						//SEM.pop();
						//break;
						exit(1); //重定义直接停止编译
					}
				}
				//if (!synbl.empty() && i < synbl[curSynblLev].size()) continue; //存在重定义
				tmpNode.name = SEEM.top().first;
				tmpNode.prelayer = preSynlayer;
				//cout << "tmpNode.prelev = " << tmpNode.prelev << endl; //for test
				//synbl_ptr = tokenSeq[SEM.top().second].id;
				token_ptr = SEEM.top().second;
				SEM.pop(); //id出栈
				stack<pair<string, int>> tmpSEM;
				while (!SEM.empty() && tokenList[SEEM.top().second].second.type != _keyword) {
					tmpSEM.push(SEEM.top());
					SEM.pop();
				}
				string tmptype = "";
				while (!SEM.empty() && tokenList[SEEM.top().second].second.type == _keyword) {
					//填符号表的Type类型区，该区链接到类型表
					tmptype += (SEEM.top().first + " ");
					tmpSEM.push(SEEM.top());
					SEEM.pop();
				}
				tmptype = tmptype.substr(0, tmptype.size() - 1); //去掉最后面的空格
				tmpNode.ty = getTypeCode(tmptype).first; //填写符号表的TYP类型区
														  //identifier_table[synbl_ptr].typ = getTypeCode(tmptype);   //填写符号表的TYP类型区
				if (tmpNode.ty == -1) {
					//void类型被用于定义变量，错误处理
					cout << "Line " << tokenList[token_ptr].second.row << ": " << endl;
					cout << "error C2182: “" << tmpNode.name << "”: 非法使用“void”类型." << endl;
					exit(1);
				}

				//for test
				//cout << tmpNode.typ << "填入符号表TYP区, " << endl;
				//for test
				tmpNode.offset = off; //记录之前的偏移量
				//cout << "当前偏移量为：" << tmpNode.offset << endl; //for test
				off += getTypeCode(tmptype).second; //更新当前偏移量
				//cout << "$$$$$$$$$$$$$$$$$$$$$$$$$ off = " << off << endl; //for test
				if (!symb.empty() && nowSynlayer < symb.size()) {
					symb[nowSynlayer].push_back(tmpNode); //单符号表已经存在的情况
				//	cout << endl << tmpNode.name << "进入synbl[" << curSynblLev << "]" << endl; //for test
				}
				//if (curSynblLev >= synbl.size()) {
				else {
					//当前层次是首次填写，即单符号表不存在的情况
					tmptable.push_back(tmpNode); //当前结点加入单符号表中
					symb.push_back(tmptable);
					//	cout << "创建总表新节点" << synbl.size() - 1 << ": " << tmpNode.name << "进入";
					//	cout << ", prevel = " << tmpNode.prelev << endl;//for test
				}

				while (!tmpSEM.empty()) { //暂存的项倒回原语义栈中
					SEEM.push(tmpSEM.top());
					tmpSEM.pop();
				}
			}
			else SEM.pop();
		}
		break;*/
	}
}
