#pragma once
#ifndef _TEST_H_
#define _TEST_H_
#pragma once
#include<iostream>
#include<fstream>
#include <sstream>
#include<string.h>
#include<stack>
#include<vector>
#include<regex>
#include<map>
#include<list>
#include<set>
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<iomanip>
//#include "Syn2_detail.cpp"
using namespace std;
#define C_RULES "Cgrammer.txt"
///==============================================================================
extern string kt[33]; //关键字表
extern string pt[45]; //界符表
extern string it[100];                                              //定义标识符
extern string ct[100];                                              //定义常数
extern string st[100];                                              //定义字符串
extern string zt[100];                                              //定义字符 	


enum tokenType {  //类码对应值
	_keyword = 1, //关键字
	_identifier,  //标识符
	_delimiter,   //界符
	_constnum,    //算术常数
	_character,   //字符
	_string,     //字符串
	//_notes, //注释(不会出现在Token中)	_space  //空白符(不会出现在Token中)
};

typedef struct token {
	tokenType type;   //类码
	int id;			  //对应符号表中的下标
	int row;         //该单词在源程序文件中的行数，用于检错时输出错误出现的行数

}token_struct;

extern vector < pair<string, token_struct> > tokenList;
extern int numkt;
extern int numit;
extern int numpt;
extern int numct;
extern int numst;
extern int numzt;
extern int line_num;
extern int _charnum;//记录读取文件时句柄的位置 
extern int get_tag;
extern string in_path;
///===========================================================================



typedef struct fund
{
	string token;
	char ty;
	int num;
} Fund;

extern vector<Fund> obj;
extern Fund x, w;
extern stack<Fund> s1;//

extern int k, l;                 //为了查分析表 

extern int flag1;
extern int flag2, flag3;            //作为几层循环的条件

typedef struct symbolTa {   //符号表（标识符表）
	string name;       //标识符源码
	int ty;           //指向类型表下标
	int offset;        //偏移量
	int prelayer;        //前一级符号表
	string domainName; //域名
	string mark;      //活跃信息标注   "Y"/"N"
	string if_temp;      //标志出是否为临时变量     "t"/"nt"
} SymbolTa;
extern SymbolTa tmpsym; //初始化全局符号表


extern vector<vector<SymbolTa> > symb;       //各级符号表！



typedef struct fund1
{
	string p;
	string a1;
	string a2;
	string ob;
} Fund1;

extern Fund1 f1;
extern vector<Fund1> obj1;//四元式表
extern vector<vector<Fund1>> oobj,oobj2,oobj3;//??

extern stack<pair<string, int>> SEEM;            //语义栈






extern string j, t1, t2;
extern int f;

extern int off;

extern int s[17][15];

extern stack<string> SEM;
extern stack<string> Sy;
extern stack<string> Type;

extern int t;
//==========================

extern int value;              //标记是哪个产生式 
extern map<pair<string, string>, int> tmp;                 //自动生成分析表的部分 
extern int a[100][100];//LL1分析表
extern set<string> ter, non_ter;
extern int qtID;

//==============================以下用于优化
typedef struct node
{
	int num;
	int left, right;//对应的 
	vector<string> v;                //记录其中的主标记（下标为0）+次标记（下标非0） 
	int ty;               //标记是否有效 
	string op;
} Node;

extern vector <Node> DAG;
extern int kk1, kk2;
extern int k1, l1, l2, k2;
extern int k, l;
extern int nn;

//==============================以下用于生成目标代码
typedef struct code                      //生成的目标代码 
{
	string p;
	string source;
	string target;
} Code;
extern vector<Code> targe;
extern vector<Code> tar;

extern pair<string, string> RX;                 //寄存器以及对应值相应的活跃信息
extern stack<int> SEMM;                    //对应目标代码生成的语义栈（用于返填）


void start();
void equal();
void action_fun(string Act, int Tokenstation, string nowstr);
void better1();
bool isnumm(string s);
bool is_exist_nsy(string k);

void start2();
#endif
