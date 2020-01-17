#include"head.h"
#include<fstream> 
class lex1 {
public:
	char ch;
	string path;//文件地址 
	string text;//实际文本信息(不是真实文本)


	//token_word是生成一个token节点的函数,返回一个pair型, 
/*		pair<string,token_struct>  token_word(){

	token_struct token_tmp;
	token_tmp.type = tokenType(1);
	token_tmp.id = 1;
	token_tmp.row = 2;
	pair<string,token_struct> token_singleword("word",token_tmp);

	//return token_singleword;
	}*/
	//对文本每一行的信息处理
	string getit(string a) {
		string target1 = "//";
		string target2 = "/*";
		string target3 = "*/";
		//int n = target.size();
		int pos1 = a.find(target1);
		int pos2 = a.find(target2);
		int pos3 = a.find(target3);
		//	cout << get_tag<<"  "<<pos3 <<endl;
		//get_tag标记为是否识别到了 /* 这个,只有 */ 配对成功才为0否则为1
		if (get_tag == 0) {
			if (pos1 != a.npos) {//找到双斜线
				//cout<<pos<<endl; 
			   //	cout<<"找到"<<endl;
				a = a.substr(0, pos1);
				get_tag = 0;
			}
			if (pos2 != a.npos) {
				if (pos3 != a.npos) {
					get_tag = 0;
					//  cout<<"标记成功f"<<get_tag<<endl;
				}
				else {
					get_tag = 1;
					//cout<<"标记成功"<<get_tag<<endl;
				}
				a = a.substr(0, pos2);
			}
		}
		else {
			//	cout<<"标记成功"<<get_tag<<endl;
			if (pos3 == a.npos) {
				a = "";
				get_tag = 1;
			}
			else {
				if (pos3 < a.length() - 1) {// /*  */配对成功,到有效文本区
					a = a.substr(pos3 + 2, a.length() - 1);
				}
				else {
					a = "";
				}

				get_tag = 0;
			}
		}


		return a;
	}
	string find_include(string pre_string) {
		//只查一行
		int pos = pre_string.find('\n');
		if (pos != pre_string.npos) {
			//cout << pre_string << endl;
			string temp1 = pre_string;
			temp1 = temp1.substr(0,pos);
			string temp2 = pre_string.substr(pos, pre_string.length() - 1);
			pre_string = get_include(temp1, temp2);
		}
		//cout << "含有#,进行增加头文件结束" << endl;
		return pre_string;
	}
	string get_include(string _defind, string _trueString) {
		string target = "\"";
		int pos1 = _defind.find(target);
		if (pos1 != _defind.npos) {//找到引号
			_defind= _defind.substr(pos1 + 1, _defind.length() - 1);
			int pos2 = _defind.find(target);
			if (pos2 != _defind.npos) {//找到引号
				_defind = _defind.substr(0, pos2);
				fstream include_head;
				include_head.open("head.txt");
				if (!include_head) {
					cout << "open head.txt failed! 头文件打开错误!" << endl;
					exit(1);
				}
				string s="";
				_defind = "";
				while (!include_head.eof()) {
					getline(include_head, s);
					_defind += s + '\n';
					//	cout<<word<<endl;

				}
				include_head.close();
				_trueString = _defind + _trueString;
				//cout << _trueString << endl;
				//cout << "头文件添加成功" << endl;
				return _trueString;
			}
			else {
				cout << "Include a head file is fail!!!" << endl;
				exit(1);
			}
		}
		else {
			cout << "Include a head file is fail!!!"<<endl;
			exit(1);
		}
		
		//return _trueString;
	}
	string openfile(string path) {
		string word;
		fstream in;
		char* _path = const_cast<char*>(path.c_str());
		in.open(_path);
		if (!in) {
			cout << "open file failed! 源文件打开错误!" << endl;
			exit(1);
		}
		string s;
		while (!in.eof()) {
			string c;
			getline(in, c);
			s = this->getit(c);
			word += s + ' ' + '\n';
				//cout<<word<<endl;

		}
		//word += '#';
	//	cout<<word<<endl;
		in.close();
		//cout << "下一步应该查找头文件" << endl;
		if (word[0] == '#') {
			//cout << "应该查找头文件" << endl;
			word=find_include(word);
		}
	//	cout << "[" << word << "]" << endl;
		text = word;//这里修改了
		return word;
	}
	pair<string, token_struct> char_select_word(string path) {
		pair<string, token_struct> token_singleword;
		string word;
		//text改成类的public
		
		//text = this->openfile(path);
		//	cout<<text<<endl;
		int pre_charnum;
		while (_charnum < text.length())
		{
			int state;
			string word = "";
			t = text[_charnum];
			//	cout<<"当前识别第"<<line_num<<"行的字符: "<<text[_charnum]<<(int)text[_charnum]<<endl;
			if ((int)t == 32)
				state = 0;// 当前字符为空格时,状态标记为0
			else if (((int)t > 64 && (int)t < 91) || ((int)t > 96 && (int)t < 123))// 65~90 A-Z,97~122 a~z
				state = 1;// 如果当前字符为字符时,状态标记为1
			else if ((int)t >= 48 && (int)t <= 57)
				state = 2;// 如果当前字符为数字时,状态标记为2
			else if ((int)t == 34)
				state = 3;// 如果当前字符为双引号时,状态标记为3
			else if ((int)t == 39)
				state = 4;// 如果当前字符为单引号时,状态标记为4
			else if (((int)t >= 33 && (int)t <= 47 && (int)t != 34 && (int)t != 39)
				|| ((int)t >= 58 && (int)t <= 64) || ((int)t >= 91 && (int)t <= 96)
				|| ((int)t >= 123 && (int)t <= 126))
				state = 5;// 其它字符,状态标记为5
			else if (t == '\t' || t == '\v' || t == '\f') {
				state = 0;
			}
			else if ((int)t == 10) {
				line_num++;
				state = 0;
			}
			else {
				// 检测到非法字符时输出异常信息
				cout << "输出异常" << endl;
				cout << t << endl;
				break;
			}


			//state_f = state;
			//state = state_change(state,ch);
			switch (state) {
			case 0:
				_charnum++;
				break;
			case 1:
				pre_charnum = _charnum;
				if (_charnum < text.length()) {
					_charnum++;
					t = text[_charnum];
					while (((t > 64 && t < 91) || (t > 96 && t < 123) || (t > 47 && t < 58))
						&& (_charnum < (text.length() - 1))) {// 65~90 A-Z,97~122 a~z,48~57 0-9
					// 如果当前字符为字母或者数字时,当前字符下标+1,否则将一直读到最后一个单词
						_charnum++;// 更新下标
						t = text[_charnum];// 更新当前字符值
					}
				}
				for (int j = pre_charnum; j < _charnum; j++) {
					word = word + text[j];
				}
				//	cout<<"case1:生成单词<"<<word<<">"<<endl;
				if (IsKeywords(word) >= 0) {
					//numkt++; 
					token_struct token_tmp;
					token_tmp.type = tokenType(1);
					token_tmp.id = IsKeywords(word);
					token_tmp.row = line_num;
					token_singleword.first = word;
					token_singleword.second = token_tmp;
					tokenList.push_back(token_singleword);

					//cout<<"是关键字"<<token_singleword.second.row<<endl;
				}
				else {
					if (IsKeywords(word) >= 0) {// 如果i标识符表不为空,且表中有这个单词
						token_struct token_tmp;
						token_tmp.type = tokenType(2);
						token_tmp.id = IsKeywords(word);
						token_tmp.row = line_num;
						token_singleword.first = word;
						token_singleword.second = token_tmp;
						tokenList.push_back(token_singleword);
						//return token_singleword;
					//	cout<<"是现存标识符"<<token_singleword.first<<endl;
					}
					else {

						token_struct token_tmp;
						token_tmp.type = tokenType(2);
						token_tmp.id = numit;
						token_tmp.row = line_num;
						token_singleword.first = word;
						token_singleword.second = token_tmp;
						tokenList.push_back(token_singleword);
						numit++;
						//return token_singleword;
				//		cout<<"不是现存标识符"<<token_singleword.first<<endl;
					}

				}
				//	_charnum++;
				return token_singleword;
				break;
			case 2:
				pre_charnum = _charnum;
				if (_charnum <= text.length()) {// 如果当前字符在文本信息内
					while ((int)t >= 48 && (int)t <= 57 && _charnum < text.length() - 1) { // 48~57 0-9
						if ((int)text[_charnum + 1] == 46) {// 如果后面是小数点
							_charnum += 2;
							t = text[_charnum];
						}
						else {// 如果数字后面是数字
							while ((int)t >= 48 && (int)t <= 57 && _charnum <= text.length()) {
								_charnum++;
								if (_charnum < text.length()) {
									t = text[_charnum];
								}
								else {
									t = 'c';//强制退出循环
								}

								if ((int)t == 46) {
									_charnum++;
									t = text[_charnum];
								}
							}
						}
					}
				}

				// 获取该改长度的字符,得到一个字
				for (int j = pre_charnum; j < _charnum; j++) {
					word = word + text[j];
				}
				// cout<<"case:2生成word"<<endl;
				// System.out.println(word);
				 // 查找c常数表,如果有则直接插入位置
				if (IsConstnum(word) >= 0) {// 如果表不为空,且表中有这个单词
					token_struct token_tmp;
					token_tmp.type = tokenType(4);
					token_tmp.id = IsConstnum(word);
					token_tmp.row = line_num;
					token_singleword.first = word;
					token_singleword.second = token_tmp;
					tokenList.push_back(token_singleword);
					//	return token_singleword;
					//	cout<<"是常数表中的值"<<token_singleword.first<<endl;
				}
				else {

					token_struct token_tmp;
					token_tmp.type = tokenType(4);
					token_tmp.id = numct;
					token_tmp.row = line_num;
					token_singleword.first = word;
					token_singleword.second = token_tmp;
					tokenList.push_back(token_singleword);
					numct++;

					//	cout<<"是常数但是表中没有"<<token_singleword.first<<endl;
				}
				return token_singleword;
				break;
			case 3:
				pre_charnum = _charnum;
				_charnum++;
				// 从双引号后一位开始识别,非双引号字符一律划为字符串内容
				while ((int)text[_charnum] != 34 && _charnum < text.length() - 1)
					_charnum++;
				//cout<<text[_charnum+1]<<endl;
				// 如果匹配不到成对的双引号,则报错
				if (text[_charnum + 1] == ';')
				{
					if (text[_charnum] == '\"') {
						//cout<<"匹配成功"<<text[_charnum]<<endl;
					}
					else {
						cout << text[_charnum + 1] << "双引号不成对错误" << text[_charnum] << endl;
						exit(1);
					}
				}
				// 字符串需要将双引号一起包括进来
				for (int j = pre_charnum; j < _charnum + 1; j++) {
					word = word + text[j];
				}
				_charnum++;

				// 查找S表,如果S表中含有这个单词
				if (IsString(word) >= 0) {// 如果S表不为空,且表中有这个单词
					token_struct token_tmp;
					token_tmp.type = tokenType(6);
					token_tmp.id = IsString(word);
					token_tmp.row = line_num;
					token_singleword.first = word;
					token_singleword.second = token_tmp;
					tokenList.push_back(token_singleword);
					//	cout<<"生成已有的字符串token"<<token_singleword.first<<endl;
				}
				else {

					token_struct token_tmp;
					token_tmp.type = tokenType(6);
					token_tmp.id = numst;
					token_tmp.row = line_num;
					token_singleword.first = word;
					token_singleword.second = token_tmp;
					tokenList.push_back(token_singleword);
					numst++;
					//		cout<<"生成新增的字符串token"<<token_singleword.first<<endl;
				}
				return token_singleword;
				break;
			case 4:// 字符
				pre_charnum = _charnum;
				_charnum++;
				// 从单引号后面开始识别,如果是单字符
				if ((int)text[_charnum] != 39 && _charnum < text.length() - 1)
					_charnum++;
				// 单引号匹配不成功,则报错
				if (text[_charnum] != '\'') {
					cout << text[_charnum] << "单引号不成对错误" << '\'' << endl;
					exit(1);
				}
				// 保存单引号和单字符作为一个整体
				for (int j = pre_charnum; j < _charnum + 1; j++) {
					word = word + text[j];
				}
				// 单引号匹配不成功,则报错
			   // if ((int)text[word.length()-1 ]!= 39)
				 //   cout<<"单引号不成对错误"<<endl;
				// 如果C单字符表中含有这个单词
				if (IsChar(word) >= 0) {// 如果i标识符表不为空,且表中有这个单词
					token_struct token_tmp;
					token_tmp.type = tokenType(5);
					token_tmp.id = IsChar(word);
					token_tmp.row = line_num;
					token_singleword.first = word;
					token_singleword.second = token_tmp;
					tokenList.push_back(token_singleword);
				}
				else {

					token_struct token_tmp;
					token_tmp.type = tokenType(5);
					token_tmp.id = numzt;
					token_tmp.row = line_num;
					token_singleword.first = word;
					token_singleword.second = token_tmp;
					tokenList.push_back(token_singleword);
					numzt++;
				}
				_charnum++;
				return token_singleword;
				break;
			case 5:// 符号
				   // 如果是符号,则直接将符号取出

				word = text[_charnum];

				///	cout<<_charnum<<">"<<word[0]<<"<"<<"查找返回值"<<IsDelimiter(word)<<endl; 
				if (_charnum + 1 < text.length()) {
					t = text[_charnum + 1];
					if (((int)t >= 33 && (int)t <= 47 && (int)t != 34 && (int)t != 39 && (int)t != 44) || ((int)t >= 58 && (int)t <= 64) || ((int)t >= 91 && (int)t <= 96)
						|| ((int)t == 124)) {
						char f_point1 = text[_charnum];
						char f_point2 = text[_charnum + 1];
						// cout<< f_point1 <<"  "<<f_point2 <<endl;
						if ((f_point1 == '&' && f_point2 == '&') || (f_point1 == '+' && f_point2 == '+')
							|| (f_point1 == '-' && f_point2 == '-') || (f_point1 == '|' && f_point2 == '|')
							|| (f_point1 == '>' && f_point2 == '=') || (f_point1 == '<' && f_point2 == '=')
							|| (f_point1 == '=' && f_point2 == '=') || (f_point1 == '!' && f_point2 == '=')
							|| (f_point1 == '<' && f_point2 == '<') || (f_point1 == '>' && f_point2 == '>')
							|| (f_point1 == '+' && f_point2 == '=') || (f_point1 == '-' && f_point2 == '=')
							|| (f_point1 == '+' && f_point2 == '+') || (f_point1 == '*' && f_point2 == '=')
							|| (f_point1 == '/' && f_point2 == '=') || (f_point1 == '%' && f_point2 == '=')
							|| (f_point1 == '&' && f_point2 == '=') || (f_point1 == '|' && f_point2 == '=')
							|| (f_point1 == '^' && f_point2 == '=')) {
							word += f_point2;
							//  cout<<word<<endl;
							_charnum++;
						}
					}
				}

				// token_struct token_tmp;
			   //  cout<<"2>"<<word<<"<"<<"查找返回值"<<IsDelimiter(word)<<endl; 
				if (IsDelimiter(word) >= 0) {// 如果表不为空,且表中有这个单词
					token_struct token_tmp;
					token_tmp.type = tokenType(3);
					token_tmp.id = IsDelimiter(word);
					token_tmp.row = line_num;
					token_singleword.first = word;
					token_singleword.second = token_tmp;
					tokenList.push_back(token_singleword);

					//	cout<<"生成已有的符号token"<<token_singleword.first<<endl;
				}
				else {
					//cout<<word<<endl;
					cout << "非法字符!!!" << endl;
					exit(1);
				}
				_charnum++;
				return token_singleword;
				break;
			default:
				cout << "\n字符识别异常:无法识别的状态" << endl;
				exit(1);
				break;
			}
		}

		if (_charnum == text.length()) {
			token_struct token_tmp;
			token_tmp.type = tokenType(3);
			token_tmp.id = 40;
			token_tmp.row = line_num;
			pair<string, token_struct> token_singleword("#", token_tmp);
			tokenList.push_back(token_singleword);
			return token_singleword;
		}

	}


	// char_select_word 是从文本信息中读取字符


   //判断是否为关键词和界符
	int IsDelimiter(string ch)		//判断是否为界符
	{
		//	cout<<"要查找的单词为>"<<ch<<"<"<<endl; 
		for (int i = 0; i < 45; i++) {

			if (ch == pt[i]) {
				//	cout<<"界符:"<<ch<<endl; 
				return i;
			}
		}
		return -1;
	}
	int IsKeywords(string ch)		//判断是否为
	{

		for (int i = 0; i < 33; i++) {
			if (ch == kt[i]) {
				return i;
			}
		}
		return -1;
	}
	//

	int IsIdentifier(string ch) {
		for (int i = 0; i < 100; i++) {
			if (ch == it[i]) {
				return i;
			}
		}
		return -1;
	}
	int IsConstnum(string ch) {
		for (int i = 0; i < 100; i++) {
			if (ch == ct[i]) {
				return i;
			}
		}
		return -1;
	}
	int IsString(string ch) {
		for (int i = 0; i < 100; i++) {
			if (ch == st[i]) {
				return i;
			}
		}
		return -1;
	}
	int IsChar(string ch) {
		for (int i = 0; i < 100; i++) {
			if (ch == zt[i]) {
				return i;
			}
		}
		return -1;
	}
};
/*
class lex {
public:
	char ch;
	string path;//文件地址 
	string text;//实际文本信息(不是真实文本)
	string getit(string a);//对输入的串信息进行处理
	string find_include(string pre_string);//对引入文件名查找操作,合并引入文件到现在的文件中
	string get_include(string _defind, string _trueString);//
	string openfile(string path);//打开现在的文件将内容读取到缓存中
	pair<string, token_struct> char_select_word(string path);//识别字符并转换成一个token
	int IsDelimiter(string ch);//对生成的token进行类型判断
	int IsKeywords(string ch);
	int IsIdentifier(string ch);
	int IsConstnum(string ch);
	int IsString(string ch);
	int IsChar(string ch);
};
*/