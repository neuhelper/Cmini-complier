#include "head.h"
int kkk = 0;
vector<vector<Fund1>> oobj;
vector<vector<Fund1>> oobj2;
vector<vector<Fund1>> oobj3;
stack<string>ss1;     //存放函数名的语义栈
vector<int> divi_v;
vector<string> dev;
vector <Node> DAG;
int kk1, kk2;
int k1, l1, l2, k2;
int nn;
vector<Code> targe;
vector<Code> tar;
pair<string, string> RX("NULL", "L");                 //寄存器以及对应值相应的活跃信息                //寄存器以及对应值相应的活跃信息
stack<int> SEMM;
int pree = -1;            //标志出四元式进行到了哪个作用域
int noww = 0;
void logi(int ttt, int n)
{
	Code c1;
	if (oobj2[ttt][n - 1].p == "==")
	{
		c1 = { "JNE"," "," " };
		targe.push_back(c1);
	}
	else if (oobj2[ttt][n - 1].p == ">")
	{
		c1 = { "JNA"," "," " };
		targe.push_back(c1);
	}
	else if (oobj2[ttt][n - 1].p == "<")
	{
		c1 = { "JNB"," "," " };
		targe.push_back(c1);
	}
	else if (oobj2[ttt][n - 1].p == ">=")
	{
		c1 = { "JB"," "," " };
		targe.push_back(c1);
	}
	else if (oobj2[ttt][n - 1].p == "<=")
	{
		c1 = { "JA"," "," " };
		targe.push_back(c1);
	}
	else if (oobj2[ttt][n - 1].p == "!=")
	{
		c1 = { "JE"," "," " };
		targe.push_back(c1);
	}
}
void logi1(int ttt, int n) {
	Code c1;
	if (oobj2[ttt][n - 1].p == "==")
	{
		c1 = { "JE"," "," " };
		targe.push_back(c1);
	}
	else if (oobj2[ttt][n - 1].p == ">")
	{
		c1 = { "JA"," "," " };
		targe.push_back(c1);
	}
	else if (oobj2[ttt][n - 1].p == "<")
	{
		c1 = { "JB"," "," " };
		targe.push_back(c1);
	}
	else if (oobj2[ttt][n - 1].p == ">=")
	{
		c1 = { "JNB"," "," " };
		targe.push_back(c1);
	}
	else if (oobj2[ttt][n - 1].p == "<=")
	{
		c1 = { "JNA"," "," " };
		targe.push_back(c1);
	}
	else if (oobj2[ttt][n - 1].p == "!=")
	{
		c1 = { "JNE"," "," " };
		targe.push_back(c1);
	}
}
bool is_func(string k)
{
	for (int i = 0; i < symb.size(); i++)
	{
		if (symb[i][0].domainName == k) return true;
	}
	return false;
}
void extra(int ttt, int n)
{

}
void write1();
bool isnumm(string s)
{
	stringstream sin(s);
	double t;
	char p;
	if (!(sin >> t))
		/*解释：
			sin>>t表示把sin转换成double的变量（其实对于int和float型的都会接收），如果转换成功，则值为非0，如果转换不成功就返回为0
		*/
		return false;
	if (sin >> p)
		/*解释：此部分用于检测错误输入中，数字加字符串的输入形式（例如：34.f），在上面的的部分（sin>>t）已经接收并转换了输入的数字部分，在stringstream中相应也会把那一部分给清除，如果此时传入字符串是数字加字符串的输入形式，则此部分可以识别并接收字符部分，例如上面所说的，接收的是.f这部分，所以条件成立，返回false;如果剩下的部分不是字符，那么则sin>>p就为0,则进行到下一步else里面
		  */
		return false;
	else
		return true;
}

bool find1(int k)
{
	for (int n = 0; n < divi_v.size(); n++)
	{
		if (k == divi_v[n]) return true;
	}
	return false;
}

bool is_exist_sy(string a)                 //查看是否为非临时变量 
{
	for (int n = 0; n < symb.size(); n++)
	{
		for (int k = 0; k < symb[n].size(); k++)
		{
			if (a == symb[n][k].name && symb[n][k].if_temp == "nt")
				return true;
		}
	}
	return false;
}

int find_sy_off1(string a)                 //查看在符号表中中的offset值
{
	for (int n = 0; n < symb.size(); n++)
	{
		for (int k = 0; k < symb[n].size(); k++)
		{
			if (a == symb[n][k].name)
				return symb[n][k].offset;
		}
	}
	return 0;
}
int find_sy_off(string id, int index)                 //查看在符号表中中的offset值
{
	if (!symb.empty() && index <= symb.size()) {
		if (index >= symb.size()) index = symb.size() - 1;
		/*		vector<SymbolTa>::iterator it = symb[index].begin(), stor_it = it;        */
		int it = 0;
		for (it = 0; it != symb[index].size(); it++) {
			if (symb[index][it].name == id) {
				return symb[index][it].offset;
			}
		}
		if (it == symb[index].size() || symb[index][0].prelayer != -1) {
			//当前作用域的符号表中未找到该标识符，且该作用域非全局作用域
			return find_sy_off(id, symb[index][0].prelayer);
		}
		else if (it == symb[index].size() || symb[index][0].prelayer == -1) {
			return -1;
		}
	}
	else return -1;
}

/*bool find2(string k)
{
	for (int n = 0;n < dev.size();n++)
	{
		if (k == dev[n]) return true;
	}
	return false;
}*/
extern bool is_exist_nsy(string k)
{
	for (int i = 0; i <= qtID; i++)
	{
		if (k == "t" + to_string(i)) return true;
	}
	return false;
}
/*bool is_exist_nsy(string a)                 //查看是否在变量表中
{
	for (int num = 0;num < obj1.size();num++)
	{
		if (is_nsy(a) && (!find2(a))) dev.push_back(a);
	}
	return false;
}*/


void finall(int ttt)
{
	Code c1;
	for (int n = 0; n < oobj2[ttt].size(); n++)            //逐一取得优化过后的四元式 
	{
		string aa1;
		string aa2;
		string oob;
		string rxx;
		if (oobj2[ttt][n].p == "if")
		{
			for (int k = noww; k < symb.size(); k++)
			{
				if (symb[k][0].domainName == "if" && noww == symb[k][0].prelayer)
				{
					noww = k;
					pree = symb[k][0].prelayer;
				}

			}
		}
		else if (oobj2[ttt][n].p == "el")
		{
			for (int k = noww; k < symb.size(); k++)
			{
				if (symb[k][0].domainName == "else" && noww == symb[k][0].prelayer)
				{
					noww = k;
					pree = symb[k][0].prelayer;
				}

			}
		}
		else if (oobj2[ttt][n].p == "wh")
		{
			for (int k = noww; k < symb.size(); k++)
			{
				if (symb[k][0].domainName == "while" && noww == symb[k][0].prelayer)
				{
					noww = k;
					pree = symb[k][0].prelayer;
				}

			}
		}
		else if (oobj2[ttt][n].p == "dowhile")
		{
			for (int k = noww; k < symb.size(); k++)
			{
				if (symb[k][0].domainName == "dowhile" && noww == symb[k][0].prelayer)
				{
					noww = k;
					pree = symb[k][0].prelayer;
				}

			}
		}
		else if (oobj2[ttt][n].p == "for")
		{
			for (int k = noww; k < symb.size(); k++)
			{
				if (symb[k][0].domainName == "for" && noww == symb[k][0].prelayer)
				{
					noww = k;
					pree = symb[k][0].prelayer;
				}

			}
		}
		else if (oobj2[ttt][n].p == "fun_do")
		{
			for (int k = noww; k < symb.size(); k++)
			{
				if (symb[k][0].domainName == oobj2[ttt][n].a1 && noww == symb[k][0].prelayer)
				{
					noww = k;
					pree = symb[k][0].prelayer;
				}

			}
		}
		else if (oobj2[ttt][n].p == "end")
		{
			k = symb[noww][0].prelayer;
			noww = k;
			pree = symb[k][0].prelayer;
		}

		if (oobj2[ttt][n].a1 == "_")                            //预处理操作
		{
			aa1 = oobj2[ttt][n].a1;
		}
		else if (isnumm(oobj2[ttt][n].a1))
			aa1 = oobj2[ttt][n].a1 + "D";
		else
			aa1 = "DS:[" + to_string(find_sy_off(oobj2[ttt][n].a1, noww)) + "D" + "]";


		if (oobj2[ttt][n].a2 == "_")
		{
			aa2 = oobj2[ttt][n].a2;
		}
		else if (isnumm(oobj2[ttt][n].a2))
			aa2 = oobj2[ttt][n].a2 + "D";
		else
			aa2 = "DS:[" + to_string(find_sy_off(oobj2[ttt][n].a2, noww)) + "D" + "]";

		if (oobj2[ttt][n].ob == "_")
		{
			oob = oobj2[ttt][n].ob;
		}
		else if (isnumm(oobj2[ttt][n].ob))
			oob = oobj2[ttt][n].ob + "D";
		else
			oob = "DS:[" + to_string(find_sy_off(oobj2[ttt][n].ob, noww)) + "D" + "]";
		rxx = "DS:[" + to_string(find_sy_off(RX.first, noww)) + "D" + "]";

		string oop;
		if (oobj2[ttt][n].p == "+") oop = "ADD";
		else if (oobj2[ttt][n].p == "-") oop = "SUB";
		else if (oobj2[ttt][n].p == "*") oop = "MUL";
		else if (oobj2[ttt][n].p == "/" || oobj2[ttt][n].p == "%") oop = "DIV";
		else if (oobj2[ttt][n].p == "shl") oop = "SHL";
		else if (oobj2[ttt][n].p == "shr") oop = "SHR";
		else if (oobj2[ttt][n].p == "==" || oobj2[ttt][n].p == ">" || oobj2[ttt][n].p == "<" || oobj2[ttt][n].p == "<=" || oobj2[ttt][n].p == ">=" || oobj2[ttt][n].p == "!=") oop = "cmp";
		if (oobj2[ttt][n].p == "+" || oobj2[ttt][n].p == "-" || oobj2[ttt][n].p == "==" || oobj2[ttt][n].p == ">" || oobj2[ttt][n].p == "<" || oobj2[ttt][n].p == "<=" || oobj2[ttt][n].p == ">=" || oobj2[ttt][n].p == "!=")                //当前扫描的四元式是表达式四元式
		{
			if (RX.first == "NULL")
			{
				c1 = { "MOV","DX,",aa1 };
				targe.push_back(c1);
				c1 = { oop,"DX,",aa2 };
				targe.push_back(c1);
			}
			else if (RX.first == oobj2[ttt][n].a1)          //如果寄存器里面存的数是第一个操作数 
			{
				if (oobj3[ttt][n].a1 == "Y")
				{
					c1 = { "MOV",aa1 + ",","DX" };           //先对B进行保存 
					targe.push_back(c1);
					c1 = { oop,"DX," ,aa2 };
					targe.push_back(c1);
				}
				else
				{
					c1 = { oop,"DX,",aa2 };
					targe.push_back(c1);
				}
			}
			else if (RX.first == oobj2[ttt][n].a2 && (oobj2[ttt][n].p == "+" || oobj2[ttt][n].p == "*"))          //如果寄存器里面存的数是第一个操作数 
			{
				if (oobj3[ttt][n].a2 == "Y")
				{
					c1 = { "MOV",aa2 + ",","DX" };           //先对B进行保存 
					targe.push_back(c1);
					c1 = { oop,"DX," ,aa1 };
					targe.push_back(c1);
				}
				else
				{
					c1 = { oop,"DX," ,aa1 };
					targe.push_back(c1);
				}
			}
			else
			{
				if (RX.second == "Y")
				{
					c1 = { "MOV", rxx + ",","DX" };           //先对B进行保存 
					targe.push_back(c1);
					c1 = { "MOV","DX," ,aa1 };
					targe.push_back(c1);
					c1 = { oop,"DX," ,aa2 };
					targe.push_back(c1);
				}
				else
				{
					c1 = { "MOV","DX,",aa1 };
					targe.push_back(c1);
					c1 = { oop,"DX," ,aa2 };
					targe.push_back(c1);
				}
			}
			RX.first = oobj2[ttt][n].ob;
			RX.second = oobj3[ttt][n].ob;
		}
		else if (oobj2[ttt][n].p == "shl" || oobj2[ttt][n].p == "shr")
		{
			c1 = { "MOV","CX,",aa2 };
			targe.push_back(c1);
			if (RX.first == "NULL")
			{
				c1 = { "MOV","DX,",aa1 };
				targe.push_back(c1);
				c1 = { oop,"DX,","CL" };
				targe.push_back(c1);
			}
			else if (RX.first == oobj2[ttt][n].a1)          //如果寄存器里面存的数是第一个操作数 
			{
				if (oobj3[ttt][n].a1 == "Y")
				{
					c1 = { "MOV",aa1 + ",","DX" };           //先对B进行保存 
					targe.push_back(c1);
					c1 = { oop,"DX,","CL" };
					targe.push_back(c1);
				}
				else
				{
					c1 = { oop,"DX,","CL" };
					targe.push_back(c1);
				}
			}
			else
			{
				if (RX.second == "Y")
				{
					c1 = { "MOV", rxx + ",","DX" };           //先对B进行保存 
					targe.push_back(c1);
					c1 = { "MOV","DX," ,aa1 };
					targe.push_back(c1);
					c1 = { oop,"DX,","CL" };
					targe.push_back(c1);
				}
				else
				{
					c1 = { "MOV","DX,",aa1 };
					targe.push_back(c1);
					c1 = { oop,"DX," ,"CL" };
					targe.push_back(c1);
				}
			}
			RX.first = oobj2[ttt][n].ob;
			RX.second = oobj3[ttt][n].ob;
		}
		else if (oobj2[ttt][n].p == "/")
		{
			if (RX.first != "NULL" && RX.second == "Y")
			{
				string rxx = "DS:[" + to_string(find_sy_off(RX.first, noww)) + "D" + "]";
				Code c1 = { "MOV",rxx + ",","DX" };
				targe.push_back(c1);
			}
			RX.first = "NULL";
			RX.second = "L";
			c1 = { "MOV",  "AX,",aa1 };
			targe.push_back(c1);
			c1 = { "MOV",  "CX,",aa2 };
			targe.push_back(c1);
			c1 = { "DIV",  "CL" };
			targe.push_back(c1);
			c1 = { "XOR", "AH,","AH" };
			targe.push_back(c1);
			c1 = { "MOV", "DX,","AX" };
			targe.push_back(c1);
			RX.first = oobj2[ttt][n].ob;
			RX.second = oobj3[ttt][n].ob;
		}
		else if (oobj2[ttt][n].p == "%")
		{
			if (RX.first != "NULL" && RX.second == "Y")
			{
				string rxx = "DS:[" + to_string(find_sy_off(RX.first, noww)) + "D" + "]";
				Code c1 = { "MOV",rxx + ",","DX" };
				targe.push_back(c1);
			}
			RX.first = "NULL";
			RX.second = "L";
			c1 = { "MOV",  "AX,",aa1 };
			targe.push_back(c1);
			c1 = { "MOV",  "CX,",aa2 };
			targe.push_back(c1);
			c1 = { "DIV",  "CL" };
			targe.push_back(c1);
			c1 = { "MOV", "AL,","AH" };
			targe.push_back(c1);
			c1 = { "XOR", "AH,","AH" };
			targe.push_back(c1);
			c1 = { "MOV", "DX,","AX" };
			targe.push_back(c1);
			RX.first = oobj2[ttt][n].ob;
			RX.second = oobj3[ttt][n].ob;
		}
		else if (oobj2[ttt][n].p == "*")
		{
			if (RX.first != "NULL" && RX.second == "Y")
			{
				string rxx = "DS:[" + to_string(find_sy_off(RX.first, noww)) + "D" + "]";
				Code c1 = { "MOV",rxx + ",","DX" };
				targe.push_back(c1);
			}
			RX.first = "NULL";
			RX.second = "L";

			c1 = { "MOV",  "AX,",aa1 };
			targe.push_back(c1);
			c1 = { "MOV",  "CL,",aa2 };
			targe.push_back(c1);
			c1 = { "MUL",  "CL" };
			targe.push_back(c1);
			c1 = { "MOV", "DX,","AX" };
			targe.push_back(c1);
			targe.push_back(c1);
			RX.first = oobj2[ttt][n].ob;
			RX.second = oobj3[ttt][n].ob;
		}

		else if (oobj2[ttt][n].p == "=" && !ss1.empty() && is_func(oobj2[ttt][n].a1))
		{
			if (RX.first != "NULL" && RX.second == "Y")
			{
				c1 = { "MOV",rxx + ",","DX" };
				targe.push_back(c1);
				RX.first = "NULL";
				RX.second = "L";
			}
			c1 = { "CALL","FAR PTR" ,oobj2[ttt][n].a1 };
			targe.push_back(c1);
			//	c1 = { "MOV","DX," ,"CX" };
			//	targe.push_back(c1);
			c1 = { "MOV", oob + ",","DX" };
			targe.push_back(c1);
			RX.first = "NULL";
			RX.second = "L";
		}
		else if (oobj2[ttt][n].p == "=")               //对应赋值四元式 
		{

			if (RX.first == "NULL")
			{
				c1 = { "MOV","DX," ,aa1 };
				targe.push_back(c1);
			}
			else if (RX.first == oobj2[ttt][n].a1)          //如果寄存器里面存的数是第一个操作数 
			{
				if (oobj3[ttt][n].a1 == "Y")
				{
					c1 = { "MOV",aa1 + "," ,"DX" };           //先对B进行保存 
					targe.push_back(c1);
				}

			}
			else if (RX.first != oobj2[ttt][n].a1 && RX.first != oobj2[ttt][n].ob)
			{
				if (RX.second == "Y")
				{
					c1 = { "MOV",rxx + ",","DX" };           //先对B进行保存 
					targe.push_back(c1);
					c1 = { "MOV","DX,",aa1 };
					targe.push_back(c1);
				}
				else
				{
					c1 = { "MOV","DX,",aa1 };
					targe.push_back(c1);
				}
			}
			RX.first = oobj2[ttt][n].ob;
			RX.second = oobj3[ttt][n].ob;
		}
		else if (oobj2[ttt][n].p == "movs")                    //对数组进行目标代码生成
		{
			if (isnumm(oobj2[ttt][n].a1))
			{
				aa1 = "DS:[" + oobj2[ttt][n].a1 + "D" + "]";

				if (RX.first == "NULL")
				{
					c1 = { "MOV","DX," ,oob };
					targe.push_back(c1);
				}
				else if (RX.first != oobj2[ttt][n].ob)
				{
					if (RX.second == "Y")
					{
						c1 = { "MOV",rxx + ",","DX" };           //先对B进行保存 
						targe.push_back(c1);
						c1 = { "MOV","DX,",oob };
						targe.push_back(c1);
					}
					else
					{
						c1 = { "MOV","DX,",oob };
						targe.push_back(c1);
					}
				}
				else
				{
					c1 = { "MOV",rxx + ",","DX" };           //先对B进行保存 
					targe.push_back(c1);
				}

				c1 = { "MOV",aa1 + ",","DX" };
				targe.push_back(c1);
				RX.first = "NULL";
				RX.second = "L";
			}
			else if (isnumm(oobj2[ttt][n].ob))
			{
				oob = "DS:[" + oobj2[ttt][n].ob + "D" + "]";
				if (RX.first == "NULL")
				{
					c1 = { "MOV","DX," ,oob };
					targe.push_back(c1);
				}
				else if (RX.first != "NULL" && RX.second == "Y")
				{
					c1 = { "MOV",rxx + ",","DX" };
					targe.push_back(c1);
					RX.first = "NULL";
					RX.second = "L";
					c1 = { "MOV","DX," ,oob };
					targe.push_back(c1);
				}
				c1 = { "MOV",aa1 + ",","DX" };
				targe.push_back(c1);
				RX.first = "NULL";
				RX.second = "L";
			}

		}
		else if (oobj2[ttt][n].p == "cin")
		{
			if (RX.first != "NULL" && RX.second == "Y")
			{
				c1 = { "MOV",rxx + ",","DX" };
				targe.push_back(c1);
				RX.first = "NULL";
				RX.second = "L";
			}
			c1 = { "MOV","AH,","01H" };
			targe.push_back(c1);
			c1 = { "INT","21H" };
			targe.push_back(c1);
			c1 = { "SUB","AX,","30H" };
			targe.push_back(c1);
			c1 = { "XOR","AH,","AH" };
			targe.push_back(c1);
			c1 = { "MOV","DX,","AX" };
			targe.push_back(c1);
			RX.first = oobj2[ttt][n].ob;
			RX.second = oobj3[ttt][n].ob;
		}
		else if (oobj2[ttt][n].p == "if")               //对应if 
		{                                                            //
			if (RX.first == "NULL")
			{
				c1 = { "MOV","DX,",aa1 };
				targe.push_back(c1);
				logi(ttt, n);
			}
			else if (RX.first == oobj2[ttt][n].a1)          //如果寄存器里面存的数是第一个操作数 
			{
				if (oobj3[ttt][n].a1 == "Y")
				{
					c1 = { "MOV",aa1 + ",","DX" };           //先对B进行保存 
					targe.push_back(c1);
					logi(ttt, n);
				}
				else
				{
					logi(ttt, n);
				}
				SEMM.push(targe.size() - 1);
				RX.first = "NULL";
				RX.second = "L";
			}
			else if (RX.first != oobj2[ttt][n].a1)
			{
				if (RX.second == "Y")
				{
					c1 = { "MOV",rxx + "," ,"DX" };           //先对B进行保存 
					targe.push_back(c1);
					c1 = { "MOV","DX,",aa1 };
					targe.push_back(c1);
					logi(ttt, n);
				}
				else
				{
					c1 = { "MOV","DX,",aa1 };
					targe.push_back(c1);
					logi(ttt, n);
				}
				SEMM.push(targe.size() - 1);
				RX.first = "NULL";
				RX.second = "L";
			}
		}
		else if (oobj2[ttt][n].p == "el")
		{
			if (RX.first != "NULL" && RX.second == "Y")
			{
				c1 = { "MOV",rxx + ",","DX" };
				targe.push_back(c1);
				RX.first = "NULL";
				RX.second = "L";
			}
			int p1 = SEMM.top();
			SEMM.pop();
			targe[p1].target = "A" + to_string(kkk);
			c1 = { "JMP"," "," " };
			targe.push_back(c1);
			SEMM.push(targe.size() - 1);

			c1 = { "A" + to_string(kkk),":" };
			targe.push_back(c1);
			kkk++;
		}
		else if (oobj2[ttt][n].p == "end" && oobj2[ttt][n].ob == "else")
		{
			if (RX.first != "NULL" && RX.second == "Y")
			{
				c1 = { "MOV",rxx + ",","DX" };
				targe.push_back(c1);
			}
			int p1 = SEMM.top();
			SEMM.pop();
			targe[p1].target = "A" + to_string(kkk);
			c1 = { "A" + to_string(kkk),":" };
			targe.push_back(c1);
			kkk++;
			RX.first = "NULL";
			RX.second = "L";
		}
		else if (oobj2[ttt][n].p == "wh")
		{
			if (RX.first != "NULL" && RX.second == "Y")
			{
				c1 = { "MOV",rxx + ",","DX" };
				targe.push_back(c1);
				RX.first = "NULL";
				RX.second == "L";
			}
			c1 = { "A" + to_string(kkk),":" };
			kkk++;
			targe.push_back(c1);
			SEMM.push(targe.size() - 1);
		}
		else if (oobj2[ttt][n].p == "do" && n == oobj2[ttt].size() - 1)
		{
			if (RX.first == "NULL")
			{
				c1 = { "MOV","DX,",aa1 };
				targe.push_back(c1);
				logi(ttt, n);
			}
			else if (RX.first == oobj2[ttt][n].a1)          //如果寄存器里面存的数是第一个操作数 
			{
				if (oobj3[ttt][n].a1 == "Y")
				{
					c1 = { "MOV",aa1 + ",","DX" };           //先对B进行保存 
					targe.push_back(c1);
					logi(ttt, n);
				}
				else
				{
					logi(ttt, n);
				}
				SEMM.push(targe.size() - 1);
				RX.first = "NULL";
				RX.second = "L";
			}
			else if (RX.first != oobj2[ttt][n].a1)
			{
				if (RX.second == "Y")
				{
					c1 = { "MOV",rxx + "," ,"DX" };           //先对B进行保存 
					targe.push_back(c1);
					c1 = { "MOV","DX,",aa1 };
					targe.push_back(c1);
					logi(ttt, n);
				}
				else
				{
					c1 = { "MOV","DX,",aa1 };
					targe.push_back(c1);
					logi(ttt, n);
				}
				SEMM.push(targe.size() - 1);
				RX.first = "NULL";
				RX.second = "L";
			}
		}
		else if (oobj2[ttt][n].p == "end" && oobj2[ttt][n].ob == "while")
		{
			if (RX.first != "NULL" && RX.second == "Y")
			{
				c1 = { "MOV",rxx + ",","DX" };
				targe.push_back(c1);
			}
			RX.first = "NULL";
			RX.second = "L";
			int p1 = SEMM.top();
			SEMM.pop();
			int p2 = SEMM.top();
			SEMM.pop();
			c1 = { "JMP"," ",targe[p2].p };
			targe.push_back(c1);
			c1 = { "A" + to_string(kkk),":" };
			targe[p1].target = "A" + to_string(kkk);
			targe.push_back(c1);
			kkk++;
		}
		else if (oobj2[ttt][n].p == "cout")
		{
			//cout << RX.first << endl;
			if (RX.first != "NULL")
			{
				c1 = { "MOV",rxx + ",","DX" };
				targe.push_back(c1);
				RX.first = "NULL";
				RX.second = "L";

			}
			/*	c1 = { "MOV",rxx + ",","DX" };
				targe.push_back(c1);*/
				//增加的换行符
			c1 = { "MOV" ,"DX,"," 0AH;为显示区别输入增加的换行符" };
			targe.push_back(c1);
			c1 = { "MOV" ,"AH,","02H" };
			targe.push_back(c1);
			c1 = { "INT" ,"21H","" };
			targe.push_back(c1);
			//
			c1 = { "MOV" ,"DX,",oob };
			targe.push_back(c1);
			c1 = { "ADD" ,"DX,"," 30H" };
			targe.push_back(c1);
			c1 = { "MOV" ,"AH,","02H" };
			targe.push_back(c1);
			c1 = { "INT" ,"21H","" };
			targe.push_back(c1);
		}
		else if (oobj2[ttt][n].p == "lable" && oobj2[ttt][n].ob == "start")
		{
			if (RX.first != "NULL" && RX.second == "Y")
			{
				c1 = { "MOV",rxx + ",","DX" };
				targe.push_back(c1);
				RX.first = "NULL";
				RX.second = "L";
			}
			c1 = { "A" + to_string(kkk),":" };
			kkk++;
			targe.push_back(c1);
			SEMM.push(targe.size() - 1);
		}
		else if (oobj2[ttt][n].p == "end" && (oobj2[ttt][n].ob == "while") || (oobj2[ttt][n].ob == "dowhile"))
		{
			if (RX.first != "NULL" && RX.second == "Y")
			{
				c1 = { "MOV",rxx + ",","DX" };
				targe.push_back(c1);
				RX.first = "NULL";
				RX.second = "L";
			}
		}
		else if (oobj2[ttt][n].p == "do" && oobj2[ttt][n + 1].p == "jmp" && oobj2[ttt][n + 1].ob != "end")
		{
			if (RX.first == "NULL")
			{
				c1 = { "MOV","DX,",aa1 };
				targe.push_back(c1);
				logi1(ttt, n);
			}
			else if (RX.first == oobj2[ttt][n].a1)          //如果寄存器里面存的数是第一个操作数 
			{
				if (oobj3[ttt][n].a1 == "Y")
				{
					c1 = { "MOV",aa1 + ",","DX" };           //先对B进行保存 
					targe.push_back(c1);
					logi1(ttt, n);
				}
				else
				{
					logi1(ttt, n);
				}
				RX.first = "NULL";
				RX.second = "L";
			}
			else if (RX.first != oobj2[ttt][n].a1)
			{
				if (RX.second == "Y")
				{
					c1 = { "MOV",rxx + "," ,"DX" };           //先对B进行保存 
					targe.push_back(c1);
					c1 = { "MOV","DX,",aa1 };
					targe.push_back(c1);
					logi1(ttt, n);
				}
				else
				{
					c1 = { "MOV","DX,",aa1 };
					targe.push_back(c1);
					logi1(ttt, n);
				}

				RX.first = "NULL";
				RX.second = "L";
			}
			int p1 = SEMM.top();
			SEMM.pop();
			targe[targe.size() - 1].target = targe[p1].p;
		}
		else if (oobj2[ttt][n].p == "label")
		{
			if (RX.first != "NULL" && RX.second == "Y")
			{
				c1 = { "MOV",rxx + ",","DX" };
				targe.push_back(c1);
				RX.first = "NULL";
				RX.second = "L";
			}
			c1 = { oobj2[ttt][n].ob + to_string(1) + ":" };
			targe.push_back(c1);
		}
		else if (oobj2[ttt][n].p == "jmp" && oobj2[ttt][n].ob == "end")
		{
		//	cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
			RX.first = "NULL";
			RX.second = "L";

			if (oobj2[ttt][n - 2].p == "==")
			{
				c1 = { "JNE"," ","end1" };
				targe.push_back(c1);
			}
			else if (oobj2[ttt][n - 2].p == ">")
			{
				c1 = { "JNA"," ","end1" };
				targe.push_back(c1);
			}
			else if (oobj2[ttt][n - 2].p == "<")
			{
				c1 = { "JNB"," ","end1" };
				targe.push_back(c1);
			}
			else if (oobj2[ttt][n - 2].p == ">=")
			{
				c1 = { "JB"," ","end1" };
				targe.push_back(c1);
			}
			else if (oobj2[ttt][n - 2].p == "<=")
			{
				c1 = { "JA"," ","end1" };
				targe.push_back(c1);
			}
			else if (oobj2[ttt][n - 2].p == "!=")
			{
				c1 = { "JE"," ","end1" };
				targe.push_back(c1);
			}
		}
		else if (oobj2[ttt][n].p == "jmp" && n == 0)
		{
			if (RX.first != "NULL" && RX.second == "Y")
			{
				c1 = { "MOV",rxx + ",","DX" };
				targe.push_back(c1);
				RX.first = "NULL";
				RX.second = "L";
			}
			c1 = { "JMP",oobj2[ttt][n].ob + to_string(1) };
			targe.push_back(c1);
		}
		else if (oobj2[ttt][n].p == "jmp" && oobj2[ttt][n - 1].p != "do")
		{
			if (RX.first != "NULL" && RX.second == "Y")
			{
				c1 = { "MOV",rxx + ",","DX" };
				targe.push_back(c1);
				RX.first = "NULL";
				RX.second = "L";
			}
			c1 = { "JMP",oobj2[ttt][n].ob + to_string(1) };
			targe.push_back(c1);
		}
		else if (oobj2[ttt][n].p == "end" && oobj2[ttt][n].ob == "for")
		{
			c1 = { "end1:" };
			targe.push_back(c1);
		}
		else if (oobj2[ttt][n].p == "fun_do" && oobj2[ttt][n].a1 == "main")
		{
			c1 = { "START:" };
			targe.push_back(c1);
			c1 = { "MOV   AX,DATA" };
			targe.push_back(c1);
			c1 = { "MOV DS, AX" };
			targe.push_back(c1);
			ss1.push("main");
		}
		else if (oobj2[ttt][n].p == "fun_do")
		{
			c1 = { oobj2[ttt][n].a1,"PROC","FAR" };
			targe.push_back(c1);
			ss1.push(oobj2[ttt][n].a1);
		}
		else if (oobj2[ttt][n].p == "re")                  //继续填写返回值
		{
			if (RX.first != "NULL" && RX.second == "Y")
			{
				c1 = { "MOV",rxx + ",","DX" };
				targe.push_back(c1);
				RX.first = "NULL";
				RX.second = "L";
			}
			c1 = { "MOV","DX,",aa1 };
			targe.push_back(c1);
		}
		else if (oobj2[ttt][n].p == "end" && oobj2[ttt][n].ob == ss1.top() && oobj2[ttt][n].ob == "main")
		{
			if (RX.first != "NULL" && RX.second == "Y")
			{
				c1 = { "MOV",rxx + ",","DX" };
				targe.push_back(c1);
				RX.first = "NULL";
				RX.second = "L";
			}
			ss1.pop();
			c1 = { "MOV","AH,","4CH" };
			targe.push_back(c1);
			c1 = { "INT","21H" };
			targe.push_back(c1);
			c1 = { "CODE","ENDS" };
			targe.push_back(c1);
			c1 = { "END","START" };
			targe.push_back(c1);
		}
		else if (oobj2[ttt][n].p == "end" && oobj2[ttt][n].ob == ss1.top() && oobj2[ttt][n].ob != "main")          //
		{
			if (RX.first != "NULL" && RX.second == "Y")
			{
				c1 = { "MOV",rxx + ",","DX" };
				targe.push_back(c1);
				RX.first = "NULL";
				RX.second = "L";
			}
			ss1.pop();
			c1 = { "RET" };
			targe.push_back(c1);
			c1 = { oobj2[ttt][n].ob,"ENDP" };
			targe.push_back(c1);

		}

		/*
		cout << oobj2[ttt][n].p << " " << oobj2[ttt][n].a1 << " " << oobj2[ttt][n].a2 << " " << oobj2[ttt][n].ob << endl;
		for (int i = 0; i < targe.size(); i++)
		{
			cout << targe[i].p << " " << targe[i].source << " " << targe[i].target << endl;

		}*/

	}
}

void divide()
{

	int fla;
	divi_v.push_back(0);
	for (int n = 1; n < obj1.size(); n++)
	{
		if (obj1[n].p == "goto")
		{
			if (find1(n + 1) == false) divi_v.push_back(n + 1);
			for (int k = 0; k < obj1.size(); k++)
			{
				if (obj1[k].p == "label" && obj1[k].ob == obj1[n].ob)       //找到转到的地方了
				{
					if (find1(k) == false) divi_v.push_back(k);
				}
			}
		}
		else if (obj1[n].p == "fun_do")
		{
			if (find1(n) == false) divi_v.push_back(n);
		}
		else if (obj1[n].p == "if")
		{
			if (find1(n + 1) == false) divi_v.push_back(n + 1);
		}
		else if (obj1[n].p == "el")
		{
			if (find1(n + 1) == false) divi_v.push_back(n + 1);
		}
		else if (obj1[n].p == "wh")
		{
			if (find1(n + 1) == false) divi_v.push_back(n + 1);
		}
		else if (obj1[n].p == "do" && obj1[n + 1].p != "jmp")
		{
			if (find1(n + 1) == false) divi_v.push_back(n + 1);
		}
		else if ((obj1[n].p == "end" && obj1[n].ob == "else") || (obj1[n].p == "end" && obj1[n].ob == "while") || (obj1[n].p == "end" && obj1[n].ob == "dowhile"))
		{
			if (find1(n + 1) == false) divi_v.push_back(n + 1);
		}
		else if (obj1[n].p == "lable" && obj1[n].ob == "start")
		{
			if (find1(n + 1) == false) divi_v.push_back(n + 1);
		}
		else if (obj1[n].p == "label" && obj1[n].ob == "cmp")
		{
			if (find1(n) == false) divi_v.push_back(n + 1);
		}
		else if (obj1[n].p == "jmp")
		{
			if (find1(n + 1) == false) divi_v.push_back(n + 1);
		}
		else if (obj1[n].p == "for")
		{
			if (find1(n + 1) == false) divi_v.push_back(n + 1);
		}

	}
	if (find1(obj1.size() - 1) == false) divi_v.push_back(obj1.size() - 1);
	/*for (int n = 0;n < divi_v.size();n++)
	{
		cout << divi_v[n] << endl;
	}*/
	sort(divi_v.begin(), divi_v.end());           //进行排序
/*	for (int n = 0;n < divi_v.size();n++)
	{
		cout << divi_v[n] << endl;
	}*/

	for (int n = 0; n < divi_v.size() - 1; n++)              //划分基本块后的四元式
	{
		vector <Fund1> vvc;
		for (int k = divi_v[n]; k < divi_v[n + 1]; k++)
		{
			vvc.push_back(obj1[k]);
		}
		if (n == divi_v.size() - 2)
		{
			vvc.push_back(obj1[divi_v[n + 1]]);
		}
		oobj.push_back(vvc);
	}
	/*
	cout << "======================================" << "此处是划分后的基本块" << endl;
	for (int i = 0; i < oobj.size(); i++)
	{
		cout << "基本块" << i << "开始" << endl;
		for (int m = 0; m < oobj[i].size(); m++)
		{
			cout << oobj[i][m].p << " " << oobj[i][m].a1 << " " << oobj[i][m].a2 << " " << oobj[i][m].ob << endl;
		}
		cout << "基本块" << i << "结束" << endl;
	}
	*/
}

int is_exist_DAG(string a)                             //查看是否在DAG某个结点中 
{
	for (k1 = DAG.size() - 1; k1 >= 0; k1--)
	{
		for (l1 = 0; l1 < DAG[k1].v.size(); l1++)
		{
			if (a == DAG[k1].v[l1]) return 1;
		}
	}
	return 0;
}

int is_exist1_DAG(string a)                             //查看是否在DAG某个结点中 (为了删除用)
{
	for (k1 = DAG.size() - 1; k1 >= 0; k1--)
	{
		for (l1 = 0; l1 < DAG[k1].v.size(); l1++)
		{
			if ((a == DAG[k1].v[l1]) && (k1 != kk1 || l1 != kk2))
			{
				return 1;
			}
		}
	}
	return 0;
}

int is_exp_DAG1(string a, string b)                             //查看是否在DAG某个结点中有公共表达式 1
{
	int fl1 = 0, fl2 = 0;
	int l;
	for (k2 = DAG.size() - 1; k2 >= 0; k2--)
	{
		for (int l2 = 0; l2 < DAG[k2].v.size(); l2++)
		{
			if (b == DAG[k2].v[l2])
			{
				l = k2 + 1;
				fl1 = 1;
				break;
			}
		}
	}
	if (fl1 == 1) {
		for (k1 = DAG.size() - 1; k1 >= 0; k1--)
		{

			if (a == DAG[k1].op)
			{
				if ((DAG[k1].left == l) || (DAG[k1].right == l))
				{
					fl2 = 1;
					break;
				}
			}
		}
	}
	if (fl1 == 1 && fl2 == 1)
		return 1;
	else return 0;
}

int is_exp_DAG2(string a, string b, string c)                             //查看是否在DAG某个结点中有公共表达式 2
{
	int fl1 = 0, fl2 = 0, fl3 = 0;
	int l, r;
	for (k2 = DAG.size() - 1; k2 >= 0; k2--)
	{
		for (int l2 = 0; l2 < DAG[k2].v.size(); l2++)
		{
			if (b == DAG[k2].v[l2])
			{
				l = k2 + 1;
				fl1 = 1;
				break;
			}
		}
	}
	for (k2 = DAG.size() - 1; k2 >= 0; k2--)
	{
		for (int l2 = 0; l2 < DAG[k2].v.size(); l2++)
		{
			if (c == DAG[k2].v[l2])
			{
				r = k2 + 1;
				fl2 = 1;
				break;
			}
		}
	}
	if (fl1 == 1 && fl2 == 1) {
		for (k1 = DAG.size() - 1; k1 >= 0; k1--)
		{

			if (a == DAG[k1].op)
			{
				if ((DAG[k1].left == l && DAG[k1].right == r) || (DAG[k1].left == r && DAG[k1].right == l))
				{
					fl3 = 1;
					break;
				}
			}
		}
	}
	if (fl1 == 1 && fl2 == 1 && fl3 == 1)
		return 1;
	else return 0;
}

int xall(int n, int ttt)
{
	if (is_exist_DAG(oobj[ttt][n].a1))
	{
	}
	else
	{
		vector <string> v1;
		v1.push_back(oobj[ttt][n].a1);
		nn++;
		Node n1 = { nn,-1,-1,v1,1,"NO" };
		DAG.push_back(n1);
		int q = is_exist_DAG(oobj[ttt][n].a1);
	}
	return n;
}

int xall1(int n, int ttt)
{
	if (is_exist_DAG(oobj[ttt][n].a1)) {}

	else
	{
		vector <string> v1;
		v1.push_back(oobj[ttt][n].a1);
		nn++;
		Node n1 = { nn,-1,-1,v1,1,"NO" };
		DAG.push_back(n1);
		int q = is_exist_DAG(oobj[ttt][n].a1);
	}

	if (is_exist_DAG(oobj[ttt][n].a2)) {}

	else
	{
		vector <string> v1;
		v1.push_back(oobj[ttt][n].a2);
		nn++;
		Node n1 = { nn,-1,-1,v1,1,"NO" };
		DAG.push_back(n1);
		int q = is_exist_DAG(oobj[ttt][n].a2);
	}

	return n;
}

void change_station(string a)
{
	if (is_exist_sy(a))
	{
		if (is_exist_nsy(DAG[k1].v[0]))                       //需要交换位置的情况 
		{
			string b = DAG[k1].v[0];
			DAG[k1].v[0] = a;
			DAG[k1].v[DAG[k1].v.size() - 1] = b;
			kk1 = k1;
			kk2 = 0;
		}
	}
	return;
}

void resett()
{
	vector< Fund1 > obj2;
	for (int n = 0; n < DAG.size(); n++)
	{
		if (DAG[n].v.size() >= 1)
		{
			if (DAG[n].left == -1 && DAG[n].right == -1)       //结点为叶结点 
			{
				for (int m = 0; m < DAG[n].v.size(); m++)
				{
					if (is_exist_sy(DAG[n].v[m]) && m != 0)
					{
						f1 = { "=",DAG[n].v[0],"_",DAG[n].v[m] };
						obj2.push_back(f1);//生成四元式并存入
					}
				}

			}
			else                                //结点为非叶结点 
			{
				if (DAG[n].right == -1)             //单目运算的情况 
				{
					f1 = { DAG[n].op,DAG[(DAG[n].left) - 1].v[0],"_",DAG[n].v[0] };
					obj2.push_back(f1);//生成四元式并存入
				}
				else if (DAG[n].left == -1)
				{
					f1 = { DAG[n].op,DAG[(DAG[n].right) - 1].v[0],"_",DAG[n].v[0] };
					obj2.push_back(f1);//生成四元式并存入
				}
				else                                             //双目运算的情况 
				{
					f1 = { DAG[n].op,DAG[(DAG[n].left) - 1].v[0],DAG[(DAG[n].right) - 1].v[0],DAG[n].v[0] };
					obj2.push_back(f1);//生成四元式并存入
				}
				for (int m = 0; m < DAG[n].v.size(); m++)
				{
					if (is_exist_sy(DAG[n].v[m]) && m != 0)
					{
						f1 = { "=",DAG[n].v[0],"_",DAG[n].v[m] };
						obj2.push_back(f1);//生成四元式并存入
					}
				}



			}

		}
	}
	oobj2.push_back(obj2);
}

void is_active(int ttt)              //每一块需要的操作
{
	for (int n = 0; n < symb.size(); n++)        //初始非临时变量都填Y 
	{
		for (int k = 0; k < symb[n].size(); k++)
		{
			if (symb[n][k].if_temp == "nt") symb[n][k].mark = "Y";
		}

	}
	for (int n = 0; n < symb.size(); n++)        //初始临时变量都填N 
	{
		for (int k = 0; k < symb[n].size(); k++)
		{
			if (symb[n][k].if_temp == "t") symb[n][k].mark = "N";
		}

	}

	for (int n = oobj2[ttt].size() - 1; n >= 0; n--)
	{
		for (int m = 0; m < symb.size(); m++)
		{
			for (int k = 0; k < symb[m].size(); k++)
			{
				if (oobj2[ttt][n].ob == symb[m][k].name)
				{
					oobj3[ttt][n].ob = symb[m][k].mark;
					if (oobj2[ttt][n].p == "movs" || oobj2[ttt][n].p == "cout") symb[m][k].mark = "Y";
					else symb[m][k].mark = "N";
				}
				if (oobj2[ttt][n].a1 == symb[m][k].name)
				{
					oobj3[ttt][n].a1 = symb[m][k].mark;
					if (oobj2[ttt][n].p == "movs" || oobj2[ttt][n].p == "cout") symb[m][k].mark = "N";
					else symb[m][k].mark = "Y";

				}
				if (oobj2[ttt][n].a2 == symb[m][k].name)
				{
					oobj3[ttt][n].a2 = symb[m][k].mark;
					if (oobj2[ttt][n].p == "movs" || oobj2[ttt][n].p == "cout") symb[m][k].mark = "N";
					else symb[m][k].mark = "Y";
				}
			}
		}
	}
}

void better(int ttt)
{
	int L, R;
	for (int n = 0; n < oobj[ttt].size(); n++)
	{
		int q;               //标出在DAG表中的第几个 
		if ((oobj[ttt][n].a2 == "_") && (oobj[ttt][n].p == "=") && (is_exist_sy(oobj[ttt][n].a1) || (is_exist_nsy(oobj[ttt][n].a1))))
		{
			//cout << "case0" << endl;
			if (is_exist_DAG(oobj[ttt][n].a1))
			{
				DAG[k1].v.push_back(oobj[ttt][n].ob);              //把A作为副标记放在B后面 
				kk1 = k1; kk2 = DAG[k1].v.size() - 1;
				change_station(oobj[ttt][n].ob);
			}
			else
			{
				vector <string> v1;
				v1.push_back(oobj[ttt][n].a1);
				nn++;
				Node n1 = { nn,-1,-1,v1,1,"NO" };
				DAG.push_back(n1);
				q = is_exist_DAG(oobj[ttt][n].a1);
				DAG[k1].v.push_back(oobj[ttt][n].ob);              //把A作为副标记放在B后面 
				kk1 = k1; kk2 = DAG[k1].v.size() - 1;
				change_station(oobj[ttt][n].ob);
			}

			if (is_exist1_DAG(oobj[ttt][n].ob))
			{
				if (l1 != 0)                                         //主标记免删 
				{
					DAG[k1].v.erase(DAG[k1].v.begin() + l1);
				}
			}

			/*for(int num1=0;num1<DAG.size();num1++)
			{
			cout<<"num:"<<DAG[num1].num<<" "<<"op:"<<DAG[num1].op<<" "<<"left:"<<DAG[num1].left<<" "<<"right:"<<DAG[num1].right<<" "<<"ty:"<<DAG[num1].ty<<" "<<endl;
			for(int num2=0;num2<DAG[num1].v.size();num2++)
			{
			cout<<"第"<<num2<<"个符号（主+副）是"<<DAG[num1].v[num2]<<endl;
			 }
			}*/
		}
		else if (((oobj[ttt][n].p == "=") && (isnumm(oobj[ttt][n].a1))) || (isnumm(oobj[ttt][n].a1) && (isnumm(oobj[ttt][n].a2))))
		{
		//	cout << "case1" << endl;
			float kk;
			int km;
			float a, b;
			bool flag = true;
			//运算的第一个数从字符串处理成数字 
			string res, res1, res2;
			res = oobj[ttt][n].a1;
			if (oobj[ttt][n].p != "=")
			{
				a = stof(oobj[ttt][n].a1);
				b = stof(oobj[ttt][n].a2);


				if (oobj[ttt][n].p == "+")                //运算的第二个数从字符串处理成数字 
				{
					kk = a + b;
				}
				else if (oobj[ttt][n].p == "-")
				{
					kk = a - b;
				}
				else if (oobj[ttt][n].p == "*")
				{
					kk = a * b;
				}
				else if (oobj[ttt][n].p == "/")
				{
					kk = a / b;
				}                   //运算的第二个数从字符串处理成数字
				else if (oobj[ttt][n].p == "%")
				{
					km = (int(a) % int(b));
					flag = false;
				}
				/*	else if (oobj[ttt][n].p == ">")
					{
						km = (a > b);
						flag = false;
					}
					else if (oobj[ttt][n].p == "<")
					{
						km = (a < b);
						flag = false;
					}
					else if (oobj[ttt][n].p == ">=")
					{
						km = (a > b || a == b);
						flag = false;
					}
					else if (oobj[ttt][n].p == "<=")
					{
						km = (a < b || a == b);
						flag = false;
					}
					else if (oobj[ttt][n].p == "==")
					{
						km = (a == b);
						flag = false;
					}
					else if (oobj[ttt][n].p == "or")
					{
						km = (int(a) || int(b));
						flag = false;
					}
					else if (oobj[ttt][n].p == "and")
					{
						km = (int(a) && int(b));
						flag = false;
					}
					else if (oobj[ttt][n].p == "not")
					{
						km = (~int(a));
						flag = false;
					}*/
				stringstream ss;
				if (flag)
					ss << kk;
				else ss << km;
				ss >> res;
			}
			if (is_exist_DAG(res))
			{
				DAG[k1].v.push_back(oobj[ttt][n].ob);              //把A作为副标记放在常数后面 
				kk1 = k1; kk2 = DAG[k1].v.size() - 1;
				//！！！写一个交换函数 
			}
			else
			{
				vector <string> v1;
				v1.push_back(res);
				nn++;
				Node n1 = { nn,-1,-1,v1,1,"NO" };
				DAG.push_back(n1);
				q = is_exist_DAG(res);
				DAG[k1].v.push_back(oobj[ttt][n].ob);              //把A作为副标记放在常数后面 
				kk1 = k1; kk2 = DAG[k1].v.size() - 1;

			}
			if (is_exist1_DAG(oobj[ttt][n].ob))
			{
				if (l1 != 0)                                         //主标记免删 
				{
					DAG[k1].v.erase(DAG[k1].v.begin() + l1);
				}
			}
			/*
			for (int num1 = 0; num1 < DAG.size(); num1++)
			{
				cout << "num:" << DAG[num1].num << " " << "op:" << DAG[num1].op << " " << "left:" << DAG[num1].left << " " << "right:" << DAG[num1].right << " " << "ty:" << DAG[num1].ty << " " << endl;
				for (int num2 = 0; num2 < DAG[num1].v.size(); num2++)
				{
					cout << "第" << num2 << "个符号（主+副）是" << DAG[num1].v[num2] << endl;
				}


			}*/
		}
		else if (oobj[ttt][n].p == "movs" || oobj[ttt][n].p == "cin" || oobj[ttt][n].p == "cout" || oobj[ttt][n].p == "end" || oobj[ttt][n].p == "fun_do")
		{
			//cout << "case111111" << endl;
			vector <string> v1;
			v1.push_back(oobj[ttt][n].a1);
			nn++;
			Node n1 = { nn,-1,-1,v1,1,"NO" };
			DAG.push_back(n1);


			vector <string> v2;
			v2.push_back(oobj[ttt][n].ob);
			nn++;
			q = is_exist_DAG(oobj[ttt][n].a1);
			L = k1 + 1;
			n1 = { nn,L,-1,v2,1,oobj[ttt][n].p };
			DAG.push_back(n1);
		}
		else
		{
			//cout << "case2" << endl;
			if (oobj[ttt][n].a2 == "_")
			{
				if (q = is_exp_DAG1(oobj[ttt][n].p, oobj[ttt][n].a1))
				{
					DAG[k1].v.push_back(oobj[ttt][n].ob);              //把A作为副标记放在后面 
					kk1 = k1; kk2 = DAG[k1].v.size() - 1;
					change_station(oobj[ttt][n].ob);
				}
				else
				{
					n = xall(n, ttt);
					vector <string> v1;
					v1.push_back(oobj[ttt][n].ob);
					nn++;
					q = is_exist_DAG(oobj[ttt][n].a1);
					L = k1 + 1;
					Node n1 = { nn,L,-1,v1,1,oobj[ttt][n].p };
					DAG.push_back(n1);
				}
			}
			else
			{
				if (q = is_exp_DAG2(oobj[ttt][n].p, oobj[ttt][n].a1, oobj[ttt][n].a2))
				{
					DAG[k1].v.push_back(oobj[ttt][n].ob);              //把A作为副标记放在后面  
					kk1 = k1; kk2 = DAG[k1].v.size() - 1;
					change_station(oobj[ttt][n].ob);
				}
				else
				{
					n = xall1(n, ttt);                 //看是否要创建子结点 
					vector <string> v1;
					v1.push_back(oobj[ttt][n].ob);
					nn++;
					q = is_exist_DAG(oobj[ttt][n].a1);
					L = k1 + 1;
					q = is_exist_DAG(oobj[ttt][n].a2);
					R = k1 + 1;
					Node n1 = { nn,L,R,v1,1,oobj[ttt][n].p };
					DAG.push_back(n1);
				}
			}
			/*
			for (int num1 = 0; num1 < DAG.size(); num1++)
			{
				cout << "num:" << DAG[num1].num << " " << "op:" << DAG[num1].op << " " << "left:" << DAG[num1].left << " " << "right:" << DAG[num1].right << " " << "ty:" << DAG[num1].ty << " " << endl;
				for (int num2 = 0; num2 < DAG[num1].v.size(); num2++)
				{
					cout << "第" << num2 << "个符号（主+副）是" << DAG[num1].v[num2] << endl;
				}
			}*/
			if (is_exist1_DAG(oobj[ttt][n].ob))
			{
				if (l1 != 0)                                         //主标记免删 
				{
					DAG[k1].v.erase(DAG[k1].v.begin() + l1);
				}
			}

		}
	}
	/*
	for (int num1 = 0; num1 < DAG.size(); num1++)
	{
		cout << "num:" << DAG[num1].num << " " << "op:" << DAG[num1].op << " " << "left:" << DAG[num1].left << " " << "right:" << DAG[num1].right << " " << "ty:" << DAG[num1].ty << " " << endl;
		for (int num2 = 0; num2 < DAG[num1].v.size(); num2++)
		{
			cout << "第" << num2 << "个符号（主+副）是" << DAG[num1].v[num2] << endl;
		}
	}*/

}

void better1()
{
	divide();
	for (int ttt = 0; ttt < oobj.size(); ttt++)
	{
		DAG.clear();
		better(ttt);
		resett();
	}
	/*
	cout << "--------------------------------------------------------------" << "此处是优化后的目标代码" << endl;
	for (int i = 0; i < oobj2.size(); i++)
	{
		cout << "基本块" << i << "开始" << endl;

		for (int m = 0; m < oobj2[i].size(); m++)
		{
			cout << oobj2[i][m].p << " " << oobj2[i][m].a1 << " " << oobj2[i][m].a2 << " " << oobj2[i][m].ob << endl;
		}
		cout << "基本块" << i << "结束" << endl;
	}
	*/
	for (int n = 0; n < oobj2.size(); n++)         //初始化四元式对应的活跃信息 
	{
		vector<Fund1> ff;
		for (int k = 0; k < oobj2[n].size(); k++)
		{
			f1 = { "L","L","L","L" };
			ff.push_back(f1);
		}
		oobj3.push_back(ff);
	}
	for (int ttt = 0; ttt < oobj2.size(); ttt++)
	{
		is_active(ttt);  //填写活跃信息
		finall(ttt);
		if (RX.first != "NULL" && RX.second == "Y")
		{
			string rxx = "DS:[" + to_string(find_sy_off(RX.first, noww)) + "D" + "]";
			Code c1 = { "MOV",rxx + ",","DX" };
			targe.push_back(c1);
		}
		RX.first = "NULL";
		RX.second = "L";
	}
/*
	cout << "--------------------------------------------------------------" << "此处是检查活跃信息表的" << endl;
	for (int i = 0; i < oobj3.size(); i++)
	{
		cout << "基本块" << i << "开始" << endl;

		for (int m = 0; m < oobj3[i].size(); m++)
		{
			cout << oobj3[i][m].p << " " << oobj3[i][m].a1 << " " << oobj3[i][m].a2 << " " << oobj3[i][m].ob << endl;
		}
		cout << "基本块" << i << "结束" << endl;
	}
*/
	write1();
	/*
	cout << "++++++++++++++++++++++++++++++++++++++++++" << "此处是目标代码的生成" << endl;
	for (int i = 0; i < targe.size(); i++)
	{
		cout << targe[i].p << " " << targe[i].source << " " << targe[i].target << endl;
	}
	*/
}

void write1() {
	fstream targ("target.asm");
	if (!targ) {
		cout << "文件打开错误！" << endl;
	}
	targ << "DATA" << "   " << "SEGMENT" << "\n";
	targ << "STO" << " " << "DW  " << off << " DUP(?)" << "\n";
	targ << "DATA" << "   " << "ENDS" << "\n";
	targ << "CODE" << "   " << "SEGMENT" << "\n";
	targ << "ASSUME" << "   " << "CS: CODE, DS: DATA" << "\n";

	for (int i = 0; i < targe.size(); i++)
	{
		targ << targe[i].p << " " << targe[i].source << " " << targe[i].target << " " << "\n";
	}
}

/*void extra(int ttt,int n)
{
	if (oobj2[ttt][n - 1].p == "or")
	{
		if((oobj2[ttt][n - 2].a2== oobj2[ttt][n - 3].a2) &&((oobj2[ttt][n - 2].p == ">"&& oobj2[ttt][n - 2].p == "==")|| (oobj2[ttt][n - 2].p == "==" && oobj2[ttt][n - 2].p == ">")))
		{
			Code c1;
			c1 = { "JB"," "," " };
			targe.push_back(c1);
		}
		else if ((oobj2[ttt][n - 2].a2 == oobj2[ttt][n - 3].a2) && ((oobj2[ttt][n - 2].p == "<" && oobj2[ttt][n - 2].p == "==") || (oobj2[ttt][n - 2].p == "==" && oobj2[ttt][n - 2].p == "<")))
		{
			Code c1;
			c1 = { "JA"," "," " };
			targe.push_back(c1);
		}
	}

}*/
