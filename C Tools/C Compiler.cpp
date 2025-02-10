#define _CRT_SECURE_NO_WARNINGS 1
#define $0 0
#define $ax 99999
#define $bx 100000
#define $cx 100001
#define $dx 100002
#define $ex 100003
#define $st 100004
#define $PC 199999
#include <iostream>
#include <cstdio>
#include <iomanip>
#include <algorithm>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <stack>
#include <utility>
#include <map>
#include <Windows.h>
#include <strsafe.h>
#include <unordered_map>
#include <queue>
using namespace std;

//void error(int pos, LPCWSTR s) {
//	MessageBox(NULL, (L"列 " + to_wstring(pos) + L" " + s).c_str(), L"错误", MB_OK);
//	exit(0);
//}
//
//void warning(int pos, LPCWSTR s) {
//	MessageBox(NULL, (L"列 " + to_wstring(pos) + L" " + s).c_str(), L"警告", MB_OK);
//}

void error(int pos, string s) {
	cerr << "错误：" << "列 " << pos << " " << s << endl;
	system("pause");
	exit(0);
}

void warning(int pos, string s) {
	cerr << "警告：" << "列 " << pos << " " << s << endl;
}

const int N = 2e5 + 10;
vector<string> keywords{ "var", "struct", "void", "if", "else", "switch", "case", "default", "break", "for", "while", "do", "continue", "const", "return", "sizeof", "__asm" };
vector<string> operators{ "->", "++", "--", "<=", ">=", "==", "!=", "&&", "||", "+=", "-=", "*=", "/=", "%=", "(", ")", "[", "]", ".", "!", "+", "-", "*", "&", "/", "%", "<", ">", "^", "?", ":", "=", "," };
vector<string> separators{ " ", "{", "}", ";" };
vector<string> basictype{ "KEYWORD", "IDENTIFIER", "CONSTANT", "OPERATOR", "SEPARATOR" };
vector<string> internal_implemention{ "abs","ceil","floor","sqrt","sin","cos","tan","asin","acos","atan","ln","log10","exp" ,"rand","display","exit","scf" };
vector<string> one_arg_cmd{ "abs","cil","flr","sqr","sin","cos","tan","asin","acos","atan","ln","log","exp","not","jmp","jz","jnz","rand","disp","exit" };

bool iskeyword(string s) {
	return find(keywords.begin(), keywords.end(), s) != keywords.end();
}

struct Word {
	int pos;
	string type, content;
	Word() {}
	Word(int _pos, string _type, string _content) {
		pos = _pos, type = _type, content = _content;
	}
};

struct GrammarAutomat {
	struct node {
		int type;//0：普通点；1：起点；2：终点。
		string errtext;//报错信息。但是终点不会报错。
		string endtype;//作为终点，在此处停止的话，是什么类型的单词
		int to[128];
	}graph[N];
	int cnt;//默认1号点起点

	int createend(int type, string endtype) {
		cnt++;
		graph[cnt].type = type;
		graph[cnt].endtype = endtype;
		return cnt;
	}

	void add(int from, char c, int to) {
		graph[from].to[c] = to;
	}

	void adddigit(int from, int l, int r, int to) {
		for (int i = l; i <= r; i++) add(from, i + '0', to);
	}

	void adddigit(int from, int to) {
		for (int i = 0; i <= 9; i++) add(from, i + '0', to);
	}

	void addalpha(int from, char l, char r, int to) {
		for (char i = l; i <= r; i++) add(from, i, to);
	}

	void addalpha(int from, int to) {
		for (char i = 'a'; i <= 'z'; i++) add(from, i, to);
		for (char i = 'A'; i <= 'Z'; i++) add(from, i, to);
	}

	void addoperator(int from, int to) {
		for (auto it = operators.begin(); it != operators.end(); it++) {
			if (it->size() == 2) {
				if (graph[from].to[(*it)[0]] == 0) {
					int tmp = createend(2, "OPERATOR");
					add(from, (*it)[0], tmp);
					add(tmp, (*it)[1], to);
				}
				else add(graph[from].to[(*it)[0]], (*it)[1], to);
			}
			else if (graph[from].to[(*it)[0]] == 0) add(from, (*it)[0], to);
		}
	}

	void addseparator(int from, int to) {
		for (auto it = separators.begin(); it != separators.end(); it++) add(from, (*it)[0], to);
	}

	void addall(int from, int to) {
		for (int i = 0; i < 128; i++) add(from, i, to);
	}

	void build() {
		cnt = 22;
		graph[1].type = 1;
		graph[2].type = 2;
		graph[3].type = 2;
		graph[4].type = 2;
		graph[5].type = 2;
		graph[6].type = 2;
		graph[7].type = 2;
		graph[8].type = 0;
		graph[9].type = 2;
		graph[10].type = 2;
		//graph[11].type = 2;
		graph[12].type = 0;
		graph[13].type = 2;
		graph[14].type = 0;
		graph[15].type = 2;
		graph[16].type = 0;
		graph[17].type = 0;
		graph[18].type = 2;
		graph[19].type = 0;
		graph[20].type = 0;
		graph[21].type = 0;
		graph[22].type = 2;
		graph[1].errtext = "存在非法字符";
		graph[2].endtype = "WORD";
		graph[3].endtype = "SEPARATOR";
		graph[4].endtype = "OPERATOR";
		graph[5].endtype = "CONSTANT";
		graph[6].endtype = "CONSTANT";
		graph[7].endtype = "CONSTANT";
		graph[8].errtext = "不完整的科学计数法";
		graph[9].endtype = "CONSTANT";
		graph[10].endtype = "CONSTANT";
		//graph[11].endtype = "CONSTANT";
		graph[12].errtext = "不完整的十六进制数";
		graph[13].endtype = "CONSTANT";
		graph[14].errtext = "不完整的二进制数";
		graph[15].endtype = "CONSTANT";
		graph[16].errtext = "不完整的字符串";
		graph[17].errtext = "不完整的字符串";
		graph[18].endtype = "CONSTANT";
		graph[19].errtext = "不完整的字符";
		graph[20].errtext = "不完整的字符";
		graph[21].errtext = "不完整的字符";
		graph[22].endtype = "CONSTANT";
		addalpha(1, 2);
		add(1, '_', 2);
		addalpha(2, 2);
		adddigit(2, 2);
		add(2, '_', 2);
		addseparator(1, 3);
		addoperator(1, 4);
		adddigit(1, 1, 9, 5);
		adddigit(5, 5);
		add(5, '.', 6);
		adddigit(6, 6);
		add(5, 'e', 7);
		add(6, 'e', 7);
		add(7, '+', 8);
		add(7, '-', 8);
		adddigit(7, 9);
		adddigit(8, 9);
		adddigit(9, 9);
		adddigit(1, 0, 0, 10);
		//adddigit(10, 0, 7, 11);
		//adddigit(11, 0, 7, 11);
		adddigit(10, 5);
		add(10, '.', 6);
		add(10, 'x', 12);
		adddigit(12, 13);
		addalpha(12, 'a', 'f', 13);
		addalpha(12, 'A', 'F', 13);
		adddigit(13, 13);
		addalpha(13, 'a', 'f', 13);
		addalpha(13, 'A', 'F', 13);
		add(10, 'b', 14);
		adddigit(14, 0, 1, 15);
		adddigit(15, 0, 1, 15);
		add(1, '\"', 16);
		addall(16, 16);
		add(16, '\\', 17);
		addall(17, 16);
		add(16, '\"', 18);
		add(1, '\'', 19);
		addall(19, 21);
		add(19, '\\', 20);
		addall(20, 21);
		add(21, '\'', 22);
	}

	vector<Word> run(string s) {
		vector<Word> res;
		int now = 1;
		string tmp;
		for (int i = 0; i <= s.size();) {
			if (i == s.size()) {
				if (graph[now].type == 2) {
					if (tmp != " ") res.push_back(Word(i, graph[now].endtype, tmp));
					tmp.clear();
					now = 1;
				}
				else error(i, graph[now].errtext.c_str());
				break;
			}
			else if (graph[now].to[s[i]] == 0) {
				if (graph[now].type == 2) {
					if (tmp != " ") res.push_back(Word(i, graph[now].endtype, tmp));
					tmp.clear();
					now = 1;
				}
				else error(i, graph[now].errtext.c_str());
			}
			else {
				tmp += s[i];
				now = graph[now].to[s[i]];
				i++;
			}
		}
		return res;
	}
}GA;

template<class Ty> vector<Ty> subvector(const vector<Ty>& ori, int l, int r) {
	vector<Ty> res;
	res.assign(ori.begin() + l, ori.begin() + r);
	return res;
}

struct GrammerChecker {
	vector<Word> s;

	//以下为语法树
	struct node {
		string type;//词法分析结果类型
		string content;//字符串内容
		int fa;//父节点
		int pos;//列数
	}p[N];
	int tot;//节点数量

	//以下为建边反悔处理器
	void kill_son(vector<int>& kill_list) {
		for (auto it = kill_list.begin(); it != kill_list.end(); it++)
			p[*it].fa = 0;
	}

	//以下为语法树的链式前向星图
	struct edge {
		int to, next;
		edge() {}
		edge(int _to, int _next) {
			to = _to, next = _next;
		}
	}ed[N];
	int pre[N];

	void add(int x, int y) {
		ed[++pre[0]] = edge(y, pre[x]);
		pre[x] = pre[0];
	}

	void fix() {
		for (int i = tot; i > 0; i--)
			if (p[i].fa) add(p[i].fa, i);
	}

	void dfs_calc_pos(int now) {
		if (p[now].pos != 0) return;
		for (int i = pre[now]; i != 0; i = ed[i].next)
			dfs_calc_pos(ed[i].to);
		p[now].pos = p[ed[pre[now]].to].pos;
	}

	//以下为词法分析识别的类别
	int Type(int pos, string x, int now, int fa) {
		if (pos == s.size()) return 0;
		if (s[pos].type == x) {
			p[now].content = s[pos].content;
			p[now].type = s[pos].type;
			p[now].fa = fa;
			p[now].pos = s[pos].pos;
			return pos + 1;
		}
		return 0;
	}

	//以下为固定字符串识别
	int String(int pos, string x, int now, int fa) {
		if (pos == s.size()) return 0;
		if (s[pos].content == x) {
			p[now].content = s[pos].content;
			p[now].type = s[pos].type;
			p[now].fa = fa;
			p[now].pos = s[pos].pos;
			return pos + 1;
		}
		return 0;
	}

	//以下为语法分析器识别程序，函数输入起始位置，返回终止位置，[起始位置，终止位置)
	int program(int pos, int num, int now, int fa);

	int declaration(int pos, int num, int now, int fa);

	int variable_definition(int pos, int num, int now, int fa);

	int array_definition(int pos, int num, int now, int fa);

	int function_declaration(int pos, int num, int now, int fa);

	int function_definition(int pos, int num, int now, int fa);

	int type_specifier(int pos, int num, int now, int fa);

	int full_type_specifier(int pos, int num, int now, int fa);

	int direct_variable_declaration(int pos, int num, int now, int fa);

	int direct_array_declaration(int pos, int num, int now, int fa);

	int direct_function_declaration(int pos, int num, int now, int fa);

	int scope(int pos, int num, int now, int fa);

	int type_keyword(int pos, int num, int now, int fa);

	int pointer_specifier(int pos, int num, int now, int fa);

	int expression(int pos, int num, int now, int fa);

	int assignment_expression(int pos, int num, int now, int fa);

	int assignment_operator(int pos, int num, int now, int fa);

	int conditional_expression(int pos, int num, int now, int fa);

	int logical_or_expression(int pos, int num, int now, int fa);

	int logical_and_expression(int pos, int num, int now, int fa);

	int equality_expression(int pos, int num, int now, int fa);

	int equality_operator(int pos, int num, int now, int fa);

	int relational_expression(int pos, int num, int now, int fa);

	int relational_operator(int pos, int num, int now, int fa);

	int additive_expression(int pos, int num, int now, int fa);

	int additive_operator(int pos, int num, int now, int fa);

	int multiplicative_expression(int pos, int num, int now, int fa);

	int multiplicative_operator(int pos, int num, int now, int fa);

	int power_expression(int pos, int num, int now, int fa);

	int unary_expression(int pos, int num, int now, int fa);

	int unary_operation(int pos, int num, int now, int fa);

	int postfix_expression(int pos, int num, int now, int fa);

	int postfix_operation(int pos, int num, int now, int fa);

	int primary_expression(int pos, int num, int now, int fa);

	int initial_list(int pos, int num, int now, int fa);

	int struct_member(int pos, int num, int now, int fa);

	int initial_element(int pos, int num, int now, int fa);

	int argument_list(int pos, int num, int now, int fa);

	int argument_declaration(int pos, int num, int now, int fa);

	int struct_declaration(int pos, int num, int now, int fa);

	int struct_definition(int pos, int num, int now, int fa);

	int struct_member_declaration(int pos, int num, int now, int fa);

	int struct_member_declarator(int pos, int num, int now, int fa);

	int statement_list(int pos, int num, int now, int fa);

	int statement(int pos, int num, int now, int fa);

	int selection_statement(int pos, int num, int now, int fa);

	int switch_structure(int pos, int num, int now, int fa);

	int switch_statement(int pos, int num, int now, int fa);

	int iteration_statement(int pos, int num, int now, int fa);

	int jump_statement(int pos, int num, int now, int fa);

	int asm_statement(int pos, int num, int now, int fa);

	int asm_argument(int pos, int num, int now, int fa);
}grmchk;

//每一个num代表一个分支，num=0表示全部分支的或
int GrammerChecker::program(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 1; i++) {
			int tmp = program(pos, i, now, fa);
			if (tmp) {
				p[now].content = "program";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		while (true) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = declaration(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		return nowpos0;
	}
}

int GrammerChecker::declaration(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 6; i++) {
			int tmp = declaration(pos, i, now, fa);
			if (tmp) {
				p[now].content = "declaration";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = struct_definition(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, ";", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 2) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = struct_declaration(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, ";", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 3) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = array_definition(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, ";", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 4) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = function_definition(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 5) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = function_declaration(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, ";", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 6) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = variable_definition(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, ";", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
}

int GrammerChecker::variable_definition(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 1; i++) {
			int tmp = variable_definition(pos, i, now, fa);
			if (tmp) {
				p[now].content = "variable_definition";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = type_specifier(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = direct_variable_declaration(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		while (true) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = String(nowpos1, ",", tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			kill_list1.push_back(++tot);
			nowpos1 = direct_variable_declaration(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		return nowpos0;
	}
}

int GrammerChecker::array_definition(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 1; i++) {
			int tmp = array_definition(pos, i, now, fa);
			if (tmp) {
				p[now].content = "array_definition";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = type_specifier(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = direct_array_declaration(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		while (true) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = String(nowpos1, ",", tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			kill_list1.push_back(++tot);
			nowpos1 = direct_array_declaration(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		return nowpos0;
	}
}

int GrammerChecker::function_declaration(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 1; i++) {
			int tmp = function_declaration(pos, i, now, fa);
			if (tmp) {
				p[now].content = "function_declaration";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = type_specifier(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = direct_function_declaration(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		while (true) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = String(nowpos1, ",", tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			kill_list1.push_back(++tot);
			nowpos1 = direct_function_declaration(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		return nowpos0;
	}
}

int GrammerChecker::function_definition(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 1; i++) {
			int tmp = function_definition(pos, i, now, fa);
			if (tmp) {
				p[now].content = "function_definition";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = type_specifier(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = direct_function_declaration(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = scope(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
}

int GrammerChecker::type_specifier(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 2; i++) {
			int tmp = type_specifier(pos, i, now, fa);
			if (tmp) {
				p[now].content = "type_specifier";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		while (true) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = String(nowpos1, "const", tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		kill_list0.push_back(++tot);
		nowpos0 = type_keyword(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		while (true) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = String(nowpos1, "const", tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		return nowpos0;
	}
	if (num == 2) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		while (true) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = String(nowpos1, "const", tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "struct", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = Type(nowpos0, "IDENTIFIER", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		while (true) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = String(nowpos1, "const", tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		return nowpos0;
	}
}

int GrammerChecker::full_type_specifier(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 1; i++) {
			int tmp = full_type_specifier(pos, i, now, fa);
			if (tmp) {
				p[now].content = "full_type_specifier";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = type_specifier(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		while (true) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = pointer_specifier(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		return nowpos0;
	}
}

int GrammerChecker::direct_variable_declaration(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 1; i++) {
			int tmp = direct_variable_declaration(pos, i, now, fa);
			if (tmp) {
				p[now].content = "direct_variable_declaration";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		while (true) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = pointer_specifier(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		kill_list0.push_back(++tot);
		nowpos0 = Type(nowpos0, "IDENTIFIER", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		for (int i = 0; i < 1; i++) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = String(nowpos1, "=", tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			kill_list1.push_back(++tot);
			nowpos1 = initial_element(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		return nowpos0;
	}
}

int GrammerChecker::direct_array_declaration(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 2; i++) {
			int tmp = direct_array_declaration(pos, i, now, fa);
			if (tmp) {
				p[now].content = "direct_array_declaration";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		while (true) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = pointer_specifier(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		kill_list0.push_back(++tot);
		nowpos0 = Type(nowpos0, "IDENTIFIER", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "[", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = expression(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "]", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		while (true) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = String(nowpos1, "[", tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			kill_list1.push_back(++tot);
			nowpos1 = expression(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			kill_list1.push_back(++tot);
			nowpos1 = String(nowpos1, "]", tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		for (int i = 0; i < 1; i++) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			for (int i = 0; i < 1; i++) {
				int nowpos2 = nowpos1;
				vector<int> kill_list2;
				kill_list2.push_back(++tot);
				nowpos2 = String(nowpos2, "=", tot, now);
				if (!nowpos2) {
					kill_son(kill_list2);
					break;
				}
				nowpos1 = nowpos2;
			}
			kill_list1.push_back(++tot);
			nowpos1 = initial_list(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		return nowpos0;
	}
	if (num == 2) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		while (true) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = pointer_specifier(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		kill_list0.push_back(++tot);
		nowpos0 = Type(nowpos0, "IDENTIFIER", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "[", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "]", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		for (int i = 0; i < 1; i++) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = String(nowpos1, "=", tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		kill_list0.push_back(++tot);
		nowpos0 = initial_list(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
}

int GrammerChecker::direct_function_declaration(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 1; i++) {
			int tmp = direct_function_declaration(pos, i, now, fa);
			if (tmp) {
				p[now].content = "direct_function_declaration";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		while (true) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = pointer_specifier(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		kill_list0.push_back(++tot);
		nowpos0 = Type(nowpos0, "IDENTIFIER", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "(", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		for (int i = 0; i < 1; i++) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = argument_list(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, ")", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
}

int GrammerChecker::scope(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 1; i++) {
			int tmp = scope(pos, i, now, fa);
			if (tmp) {
				p[now].content = "scope";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "{", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = statement_list(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "}", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
}

int GrammerChecker::type_keyword(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 2; i++) {
			int tmp = type_keyword(pos, i, now, fa);
			if (tmp) {
				p[now].content = "type_keyword";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "void", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 2) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "var", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
}

int GrammerChecker::pointer_specifier(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 1; i++) {
			int tmp = pointer_specifier(pos, i, now, fa);
			if (tmp) {
				p[now].content = "pointer_specifier";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "*", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		while (true) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = String(nowpos1, "const", tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		return nowpos0;
	}
}

int GrammerChecker::expression(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 1; i++) {
			int tmp = expression(pos, i, now, fa);
			if (tmp) {
				p[now].content = "expression";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = assignment_expression(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		while (true) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = String(nowpos1, ",", tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			kill_list1.push_back(++tot);
			nowpos1 = assignment_expression(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		return nowpos0;
	}
}

int GrammerChecker::assignment_expression(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 1; i++) {
			int tmp = assignment_expression(pos, i, now, fa);
			if (tmp) {
				p[now].content = "assignment_expression";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		while (true) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = unary_expression(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			kill_list1.push_back(++tot);
			nowpos1 = assignment_operator(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		kill_list0.push_back(++tot);
		nowpos0 = conditional_expression(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
}

int GrammerChecker::assignment_operator(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 6; i++) {
			int tmp = assignment_operator(pos, i, now, fa);
			if (tmp) {
				p[now].content = "assignment_operator";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "=", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 2) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "*=", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 3) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "/=", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 4) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "%=", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 5) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "+=", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 6) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "-=", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
}

int GrammerChecker::conditional_expression(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 1; i++) {
			int tmp = conditional_expression(pos, i, now, fa);
			if (tmp) {
				p[now].content = "conditional_expression";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		while (true) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = logical_or_expression(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			kill_list1.push_back(++tot);
			nowpos1 = String(nowpos1, "?", tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			kill_list1.push_back(++tot);
			nowpos1 = expression(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			kill_list1.push_back(++tot);
			nowpos1 = String(nowpos1, ":", tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		kill_list0.push_back(++tot);
		nowpos0 = logical_or_expression(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
}

int GrammerChecker::logical_or_expression(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 1; i++) {
			int tmp = logical_or_expression(pos, i, now, fa);
			if (tmp) {
				p[now].content = "logical_or_expression";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = logical_and_expression(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		while (true) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = String(nowpos1, "||", tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			kill_list1.push_back(++tot);
			nowpos1 = logical_and_expression(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		return nowpos0;
	}
}

int GrammerChecker::logical_and_expression(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 1; i++) {
			int tmp = logical_and_expression(pos, i, now, fa);
			if (tmp) {
				p[now].content = "logical_and_expression";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = equality_expression(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		while (true) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = String(nowpos1, "&&", tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			kill_list1.push_back(++tot);
			nowpos1 = equality_expression(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		return nowpos0;
	}
}

int GrammerChecker::equality_expression(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 1; i++) {
			int tmp = equality_expression(pos, i, now, fa);
			if (tmp) {
				p[now].content = "equality_expression";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = relational_expression(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		while (true) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = equality_operator(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			kill_list1.push_back(++tot);
			nowpos1 = relational_expression(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		return nowpos0;
	}
}

int GrammerChecker::equality_operator(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 2; i++) {
			int tmp = equality_operator(pos, i, now, fa);
			if (tmp) {
				p[now].content = "equality_operator";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "==", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 2) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "!=", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
}

int GrammerChecker::relational_expression(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 1; i++) {
			int tmp = relational_expression(pos, i, now, fa);
			if (tmp) {
				p[now].content = "relational_expression";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = additive_expression(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		while (true) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = relational_operator(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			kill_list1.push_back(++tot);
			nowpos1 = additive_expression(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		return nowpos0;
	}
}

int GrammerChecker::relational_operator(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 4; i++) {
			int tmp = relational_operator(pos, i, now, fa);
			if (tmp) {
				p[now].content = "relational_operator";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "<", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 2) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, ">", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 3) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "<=", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 4) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, ">=", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
}

int GrammerChecker::additive_expression(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 1; i++) {
			int tmp = additive_expression(pos, i, now, fa);
			if (tmp) {
				p[now].content = "additive_expression";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = multiplicative_expression(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		while (true) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = additive_operator(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			kill_list1.push_back(++tot);
			nowpos1 = multiplicative_expression(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		return nowpos0;
	}
}

int GrammerChecker::additive_operator(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 2; i++) {
			int tmp = additive_operator(pos, i, now, fa);
			if (tmp) {
				p[now].content = "additive_operator";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "+", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 2) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "-", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
}

int GrammerChecker::multiplicative_expression(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 1; i++) {
			int tmp = multiplicative_expression(pos, i, now, fa);
			if (tmp) {
				p[now].content = "multiplicative_expression";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = power_expression(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		while (true) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = multiplicative_operator(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			kill_list1.push_back(++tot);
			nowpos1 = power_expression(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		return nowpos0;
	}
}

int GrammerChecker::multiplicative_operator(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 3; i++) {
			int tmp = multiplicative_operator(pos, i, now, fa);
			if (tmp) {
				p[now].content = "multiplicative_operator";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "*", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 2) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "/", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 3) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "%", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
}

int GrammerChecker::power_expression(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 1; i++) {
			int tmp = power_expression(pos, i, now, fa);
			if (tmp) {
				p[now].content = "power_expression";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = unary_expression(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		while (true) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = String(nowpos1, "^", tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			kill_list1.push_back(++tot);
			nowpos1 = unary_expression(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		return nowpos0;
	}
}

int GrammerChecker::unary_expression(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 2; i++) {
			int tmp = unary_expression(pos, i, now, fa);
			if (tmp) {
				p[now].content = "unary_expression";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		while (true) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = unary_operation(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		kill_list0.push_back(++tot);
		nowpos0 = postfix_expression(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 2) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "sizeof", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "(", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = full_type_specifier(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, ")", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
}

int GrammerChecker::unary_operation(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 9; i++) {
			int tmp = unary_operation(pos, i, now, fa);
			if (tmp) {
				p[now].content = "unary_operation";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "&", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 2) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "*", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 3) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "+", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 4) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "-", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 5) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "!", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 6) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "++", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 7) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "--", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 8) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "sizeof", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 9) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "(", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = full_type_specifier(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, ")", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
}

int GrammerChecker::postfix_expression(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 1; i++) {
			int tmp = postfix_expression(pos, i, now, fa);
			if (tmp) {
				p[now].content = "postfix_expression";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = primary_expression(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		while (true) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = postfix_operation(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		return nowpos0;
	}
}

int GrammerChecker::postfix_operation(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 5; i++) {
			int tmp = postfix_operation(pos, i, now, fa);
			if (tmp) {
				p[now].content = "postfix_operation";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "[", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = expression(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "]", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 2) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, ".", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = Type(nowpos0, "IDENTIFIER", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 3) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "->", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = Type(nowpos0, "IDENTIFIER", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 4) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "++", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 5) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "--", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
}

int GrammerChecker::primary_expression(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 3; i++) {
			int tmp = primary_expression(pos, i, now, fa);
			if (tmp) {
				p[now].content = "primary_expression";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = Type(nowpos0, "IDENTIFIER", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		for (int i = 0; i < 1; i++) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = String(nowpos1, "(", tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			for (int i = 0; i < 1; i++) {
				int nowpos2 = nowpos1;
				vector<int> kill_list2;
				kill_list2.push_back(++tot);
				nowpos2 = assignment_expression(nowpos2, 0, tot, now);
				if (!nowpos2) {
					kill_son(kill_list2);
					break;
				}
				while (true) {
					int nowpos3 = nowpos2;
					vector<int> kill_list3;
					kill_list3.push_back(++tot);
					nowpos3 = String(nowpos3, ",", tot, now);
					if (!nowpos3) {
						kill_son(kill_list3);
						break;
					}
					kill_list3.push_back(++tot);
					nowpos3 = assignment_expression(nowpos3, 0, tot, now);
					if (!nowpos3) {
						kill_son(kill_list3);
						break;
					}
					nowpos2 = nowpos3;
				}
				nowpos1 = nowpos2;
			}
			kill_list1.push_back(++tot);
			nowpos1 = String(nowpos1, ")", tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		return nowpos0;
	}
	if (num == 2) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = Type(nowpos0, "CONSTANT", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 3) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "(", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = expression(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, ")", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
}

int GrammerChecker::initial_list(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 2; i++) {
			int tmp = initial_list(pos, i, now, fa);
			if (tmp) {
				p[now].content = "initial_list";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "{", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = initial_element(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		while (true) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = String(nowpos1, ",", tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			kill_list1.push_back(++tot);
			nowpos1 = initial_element(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		for (int i = 0; i < 1; i++) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = String(nowpos1, ",", tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "}", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 2) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "{", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = struct_member(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "=", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = initial_element(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		while (true) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = String(nowpos1, ",", tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			kill_list1.push_back(++tot);
			nowpos1 = struct_member(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			kill_list1.push_back(++tot);
			nowpos1 = String(nowpos1, "=", tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			kill_list1.push_back(++tot);
			nowpos1 = initial_element(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		for (int i = 0; i < 1; i++) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = String(nowpos1, ",", tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "}", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
}

int GrammerChecker::struct_member(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 1; i++) {
			int tmp = struct_member(pos, i, now, fa);
			if (tmp) {
				p[now].content = "struct_member";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		while (true) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = String(nowpos1, ".", tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			kill_list1.push_back(++tot);
			nowpos1 = Type(nowpos1, "IDENTIFIER", tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		return nowpos0;
	}
}

int GrammerChecker::initial_element(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 2; i++) {
			int tmp = initial_element(pos, i, now, fa);
			if (tmp) {
				p[now].content = "initial_element";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = initial_list(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 2) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = assignment_expression(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
}

int GrammerChecker::argument_list(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 1; i++) {
			int tmp = argument_list(pos, i, now, fa);
			if (tmp) {
				p[now].content = "argument_list";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = argument_declaration(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		while (true) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = String(nowpos1, ",", tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			kill_list1.push_back(++tot);
			nowpos1 = argument_declaration(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		return nowpos0;
	}
}

int GrammerChecker::argument_declaration(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 1; i++) {
			int tmp = argument_declaration(pos, i, now, fa);
			if (tmp) {
				p[now].content = "argument_declaration";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = full_type_specifier(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = Type(nowpos0, "IDENTIFIER", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
}

int GrammerChecker::struct_declaration(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 1; i++) {
			int tmp = struct_declaration(pos, i, now, fa);
			if (tmp) {
				p[now].content = "struct_declaration";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "struct", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = Type(nowpos0, "IDENTIFIER", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
}

int GrammerChecker::struct_definition(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 1; i++) {
			int tmp = struct_definition(pos, i, now, fa);
			if (tmp) {
				p[now].content = "struct_definition";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = struct_declaration(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "{", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		while (true) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = struct_member_declaration(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			kill_list1.push_back(++tot);
			nowpos1 = String(nowpos1, ";", tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "}", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
}

int GrammerChecker::struct_member_declaration(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 2; i++) {
			int tmp = struct_member_declaration(pos, i, now, fa);
			if (tmp) {
				p[now].content = "struct_member_declaration";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = type_specifier(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = struct_member_declarator(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		while (true) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = String(nowpos1, ",", tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			kill_list1.push_back(++tot);
			nowpos1 = struct_member_declarator(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		return nowpos0;
	}
	if (num == 2) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = struct_definition(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
}

int GrammerChecker::struct_member_declarator(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 2; i++) {
			int tmp = struct_member_declarator(pos, i, now, fa);
			if (tmp) {
				p[now].content = "struct_member_declarator";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		while (true) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = pointer_specifier(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		kill_list0.push_back(++tot);
		nowpos0 = Type(nowpos0, "IDENTIFIER", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "[", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = expression(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "]", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		while (true) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = String(nowpos1, "[", tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			kill_list1.push_back(++tot);
			nowpos1 = expression(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			kill_list1.push_back(++tot);
			nowpos1 = String(nowpos1, "]", tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		return nowpos0;
	}
	if (num == 2) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		while (true) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = pointer_specifier(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		kill_list0.push_back(++tot);
		nowpos0 = Type(nowpos0, "IDENTIFIER", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
}

int GrammerChecker::statement_list(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 1; i++) {
			int tmp = statement_list(pos, i, now, fa);
			if (tmp) {
				p[now].content = "statement_list";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		while (true) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = statement(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		return nowpos0;
	}
}

int GrammerChecker::statement(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 10; i++) {
			int tmp = statement(pos, i, now, fa);
			if (tmp) {
				p[now].content = "statement";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, ";", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 2) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = scope(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 3) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = asm_statement(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 4) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = array_definition(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, ";", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 5) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = variable_definition(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, ";", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 6) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = expression(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, ";", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 7) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = selection_statement(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 8) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = switch_structure(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 9) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = iteration_statement(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 10) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = jump_statement(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
}

int GrammerChecker::selection_statement(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 1; i++) {
			int tmp = selection_statement(pos, i, now, fa);
			if (tmp) {
				p[now].content = "selection_statement";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "if", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "(", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = expression(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, ")", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = statement(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		for (int i = 0; i < 1; i++) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = String(nowpos1, "else", tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			kill_list1.push_back(++tot);
			nowpos1 = statement(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		return nowpos0;
	}
}

int GrammerChecker::switch_structure(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 1; i++) {
			int tmp = switch_structure(pos, i, now, fa);
			if (tmp) {
				p[now].content = "switch_structure";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "switch", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "(", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = expression(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, ")", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "{", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		while (true) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = switch_statement(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "}", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
}

int GrammerChecker::switch_statement(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 3; i++) {
			int tmp = switch_statement(pos, i, now, fa);
			if (tmp) {
				p[now].content = "switch_statement";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "case", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = expression(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, ":", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 2) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "default", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, ":", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 3) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = statement(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
}

int GrammerChecker::iteration_statement(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 5; i++) {
			int tmp = iteration_statement(pos, i, now, fa);
			if (tmp) {
				p[now].content = "iteration_statement";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "while", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "(", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = expression(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, ")", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = statement(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 2) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "do", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = statement(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "while", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "(", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = expression(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, ")", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 3) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "for", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "(", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		for (int i = 0; i < 1; i++) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = expression(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, ";", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		for (int i = 0; i < 1; i++) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = expression(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, ";", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		for (int i = 0; i < 1; i++) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = expression(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, ")", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = statement(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 4) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "for", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "(", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		for (int i = 0; i < 1; i++) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = array_definition(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, ";", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		for (int i = 0; i < 1; i++) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = expression(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, ";", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		for (int i = 0; i < 1; i++) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = expression(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, ")", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = statement(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 5) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "for", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "(", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		for (int i = 0; i < 1; i++) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = variable_definition(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, ";", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		for (int i = 0; i < 1; i++) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = expression(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, ";", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		for (int i = 0; i < 1; i++) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = expression(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, ")", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = statement(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
}

int GrammerChecker::jump_statement(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 3; i++) {
			int tmp = jump_statement(pos, i, now, fa);
			if (tmp) {
				p[now].content = "jump_statement";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "continue", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, ";", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 2) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "break", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, ";", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 3) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "return", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		for (int i = 0; i < 1; i++) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = expression(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, ";", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
}

int GrammerChecker::asm_statement(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 1; i++) {
			int tmp = asm_statement(pos, i, now, fa);
			if (tmp) {
				p[now].content = "asm_statement";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "__asm", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, "(", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = Type(nowpos0, "CONSTANT", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, ",", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = asm_argument(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		for (int i = 0; i < 1; i++) {
			int nowpos1 = nowpos0;
			vector<int> kill_list1;
			kill_list1.push_back(++tot);
			nowpos1 = String(nowpos1, ",", tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			kill_list1.push_back(++tot);
			nowpos1 = asm_argument(nowpos1, 0, tot, now);
			if (!nowpos1) {
				kill_son(kill_list1);
				break;
			}
			nowpos0 = nowpos1;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, ")", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		kill_list0.push_back(++tot);
		nowpos0 = String(nowpos0, ";", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
}

int GrammerChecker::asm_argument(int pos, int num, int now, int fa) {
	if (num == 0) {
		for (int i = 1; i <= 2; i++) {
			int tmp = asm_argument(pos, i, now, fa);
			if (tmp) {
				p[now].content = "asm_argument";
				p[now].type = 2;
				p[now].fa = fa;
				return tmp;
			}
		}
		p[now].fa = 0;
		return 0;
	}
	if (num == 1) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = Type(nowpos0, "CONSTANT", tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
	if (num == 2) {
		tot = now;
		int nowpos0 = pos;
		vector<int> kill_list0;
		kill_list0.push_back(++tot);
		nowpos0 = assignment_expression(nowpos0, 0, tot, now);
		if (!nowpos0) {
			kill_son(kill_list0);
			return 0;
		}
		return nowpos0;
	}
}
/*
寄存器说明
$ax:表达式结果存放地址，jmp读取地址
$bx:运算赋值地址存放，指针偏移量存放，初始化地址存放
$cx:相对地址到绝对地址偏移量存放
$dx:内联汇编第一个参数暂存
*/

struct fsccode {//fsc plus代码
	//<op> <arg> <arg>
	string op, arg1, arg2;
	fsccode() {}
	fsccode(string _op, string _arg1 = "0", string _arg2 = "0") {
		op = _op, arg1 = _arg1, arg2 = _arg2;
		//if (_op == "mov" && _arg1 == "$st(0)" && _arg2 == "$ax(0)") {
		//	int test = 1;
		//	test++;
		//}
	}
};

double mod(double x, double y) {
	return x - floor(x / y) * y;
}

double to_double(string x) {
	stringstream ss;
	ss << x;
	double res;
	ss >> res;
	return res;
}

char to_char(string s) {
	if (s.size() == 1) return s[0];
	else if (s.size() == 2) {
		switch (s[1]) {
		case 'a':return 7;
		case 'b':return 8;
		case 'f':return 12;
		case 'n':return 10;
		case 'r':return 13;
		case 't':return 9;
		case 'v':return 11;
		case '\\':return 92;
		case '\'':return 39;
		case '\"':return 34;
		case '\?':return 63;
		case '0':return 0;
		default:error(0, "不支持的转义字符\'" + s + "\'");
		}
	}
	else error(0, "未知错误");
}

struct SemanticAnalysis {
	SemanticAnalysis() {}

	GrammerChecker g;

	struct full_datatype {
		int datatype;//0表示void，-1表示var，正数表示符号表中的结构体类型编号
		bool isconst;//是否是常量标识符
		vector<bool> isconstp;//指针每一层是否是常量指针
		deque<int> dimension;//数组每维长度
		int size;//数据类型大小（指针是1，数组是1，var是1）

		bool operator==(full_datatype& x) {
			return datatype == x.datatype && isconst == x.isconst && isconstp == x.isconstp && dimension == x.dimension && size == x.size;
		}

		bool operator!=(full_datatype& x) {
			return !(*this == x);
		}

		int steplenth() {
			int res = 1;
			for (auto i = dimension.begin();i != dimension.end();i++)
				res *= *i;
			return res;
		}
	};

	struct full_data {
		full_datatype type;
		string name;

		bool operator==(full_data& x) {
			return type == x.type && name == x.name;
		}

		bool operator!=(full_data& x) {
			return type != x.type || name != x.name;
		}
	};

	struct symbol {
		string name;//标识符名称
		int kind;//标识符代表类型。0：函数；1：变量、数组、指针；2：结构体。
		full_datatype type;//函数返回值或变量数组常量类型
		int size;//如果是结构体标识符，则表示该结构体类型大小；如果是函数标识符，则表示函数所需最大栈空间大小
		int argsiz;//如果是函数标识符，则表示参数所需总空间
		int ptr;//标识符内存指向位置（相对当前top_sp或相对0）或函数代码位置
		bool defined;//是否定义了
		//如果函数只声明未定义，则调用函数全部跳转到声明行，定义时只需回填声明行
		//如果结构体只声明未定义，则不能使用，但可以使用其指针
		vector<full_data> list;//函数参数列表或结构体成员列表
		vector<int> ret_fillback;//函数返回跳转回填项
	}table[N];
	int num[N];//该作用域符号表符号数量
	int idx = 1;//作用域栈尾指针
	int stack_offset = 0;//栈上的下一个可用地址
	int static_offset = 1;//静态存储区下一个可用地址

	friend bool operator==(vector<SemanticAnalysis::full_data>& x, vector<SemanticAnalysis::full_data>& y) {
		if (x.size() != y.size()) return false;
		for (int i = 0; i < x.size(); i++) if (x[i] != y[i]) return false;
		return true;
	}

	friend bool operator!=(vector<SemanticAnalysis::full_data>& x, vector<SemanticAnalysis::full_data>& y) {
		if (x.size() != y.size()) return true;
		for (int i = 0; i < x.size(); i++) if (x[i] != y[i]) return true;
		return false;
	}

	int find(string x, int kind) {
		int res = kind == 1 ? num[idx] : num[1];
		while (res > 0) {
			if (table[res].name == x && table[res].kind == kind) return res;
			res--;
		}
		return res;
	}

	vector<fsccode> code;

	//便捷代码
	void addmov(string reg1, int off1, string reg2, int off2, int len) {
		for (int i = 0; i < len; i++)
			code.push_back(fsccode("mov", reg(reg1, off1 + i), reg(reg2, off2 + i)));
	}

	string reg(string reg, int off) {
		return reg + "(" + to_string(off) + ")";
	}

	//语义属性L-SDD
	//非终结符
	struct program {
		//void
	};

	struct declaration {

	};

	struct variable_definition {
		full_datatype type;
		int totsize;
	};

	struct array_definition {
		full_datatype type;
		int totsize;
	};

	struct function_declaration {
		full_datatype type;
	};

	struct function_definition {
		full_datatype type;
		int idx;
	};

	struct type_specifier {
		full_datatype type;
	};

	struct full_type_specifier {
		full_datatype type;//该type包含指针信息
	};

	struct direct_variable_declaration {
		full_datatype type;//该type包含指针信息
		int idx;
	};

	struct direct_array_declaration {
		full_datatype type;//该type包含指针信息
		int idx;
	};

	struct direct_function_declaration {
		full_datatype type;//该type包含指针信息
		int idx;//函数在符号表中的编号
	};

	struct scope {
		int size;//作用域内局部变量总大小
		int funcidx;//当前所在函数的编号
		int continuepos;
		int breakpos;
	};

	struct type_keyword {
		string keyword;
	};

	struct pointer_specifier {
		bool isconst;
	};

	struct expression {
		bool isconstexpr;//左值表示是否是已知地址，右值表示是否是已知量
		full_datatype type;//表达式结果类型
		double val;//用double模拟sc变量
		bool isleftval;//是否为左值
		int argsiz;//表达式参数总大小
	};

	struct assignment_operator {
		int op;
	};

	struct equality_operator {
		int op;
	};

	struct relational_operator {
		int op;
	};

	struct shift_operator {
		int op;
	};

	struct additive_operator {
		int op;
	};

	struct multiplicative_operator {
		int op;
	};

	struct unary_operation {
		int op;
		full_datatype type;//该type包含指针信息
	};

	struct postfix_operation {
		int op;
		expression expr;
		string name;
	};

	struct initial_list {
		int ptr;
		full_datatype type;//要初始化的内容的类型
		int size;//不定长数组的长度
	};

	struct struct_member {
		int typenum;//符号表编号
		full_datatype type;//该结构体该成员的类型
		int ptr;//该成员相对地址
	};

	struct initial_element {
		int ptr;
		full_datatype type;//要初始化的内容的类型
	};

	struct argument_list {
		//int idx;//参数列表对应函数的符号表编号
		vector<full_data> list;
	};

	struct argument_declaration {
		full_datatype type;
		string name;
	};

	struct struct_declaration {
		string name;
	};

	struct struct_definition {
		//void
	};

	struct struct_member_declaration {
		int num;//结构体符号表编号
	};

	struct struct_member_declarator {
		full_datatype type;
		int num;
	};

	struct statement_list {
		int size;
		int funcidx;
		int continuepos;
		int breakpos;
	};

	struct statement {
		int size;
		int funcidx;
		int continuepos;
		int breakpos;
	};

	struct selection_statement : statement {

	};

	struct switch_structure : statement {

	};

	struct switch_statement : statement {

	};

	struct iteration_statement : statement {

	};

	struct jump_statement : statement {

	};

	struct asm_statement {

	};

	struct asm_argument{
		int type;//1:常量 2:表达式
		string arg;
		expression expr;
	};

	struct attribute {
		attribute() {}
		~attribute() {}
		program program;
		declaration declaration;
		variable_definition variable_definition;
		array_definition array_definition;
		function_declaration function_declaration;
		function_definition function_definition;
		type_specifier type_specifier;
		full_type_specifier full_type_specifier;
		direct_variable_declaration direct_variable_declaration;
		direct_array_declaration direct_array_declaration;
		direct_function_declaration direct_function_declaration;
		scope scope;
		type_keyword type_keyword;
		pointer_specifier pointer_specifier;
		expression expression;
		assignment_operator assignment_operator;
		equality_operator equality_operator;
		relational_operator relational_operator;
		shift_operator shift_operator;
		additive_operator additive_operator;
		multiplicative_operator multiplicative_operator;
		unary_operation unary_operation;
		postfix_operation postfix_operation;
		initial_list initial_list;
		struct_member struct_member;
		initial_element initial_element;
		argument_list argument_list;
		argument_declaration argument_declaration;
		struct_declaration struct_declaration;
		struct_definition struct_definition;
		struct_member_declaration struct_member_declaration;
		struct_member_declarator struct_member_declarator;
		statement_list statement_list;
		statement statement;
		selection_statement selection_statement;
		switch_structure switch_structure;
		switch_statement switch_statement;
		iteration_statement iteration_statement;
		jump_statement jump_statement;
		asm_statement asm_statement;
		asm_argument asm_argument;
	}attr[N];

	//语义分析器核心
	void analyse(int now) {
		if (g.p[now].content == "program") {
			int i = g.pre[now];
			while (i != 0) {
				analyse(g.ed[i].to);
				i = g.ed[i].next;
			}
		}
		else if (g.p[now].content == "declaration") {
			int i = g.pre[now];
			while (i != 0) {
				analyse(g.ed[i].to);
				i = g.ed[i].next;
			}
		}
		else if (g.p[now].content == "variable_definition") {
			int i = g.pre[now];
			analyse(g.ed[i].to);
			attr[now].variable_definition.type = attr[g.ed[i].to].type_specifier.type;
			i = g.ed[i].next;
			analyse(g.ed[i].to);
			attr[now].variable_definition.totsize += attr[g.ed[i].to].direct_variable_declaration.type.size;
			i = g.ed[i].next;
			while (i != 0) {
				i = g.ed[i].next;
				analyse(g.ed[i].to);
				attr[now].variable_definition.totsize += attr[g.ed[i].to].direct_variable_declaration.type.size;
				i = g.ed[i].next;
			}
		}
		else if (g.p[now].content == "array_definition") {
			int i = g.pre[now];
			analyse(g.ed[i].to);
			attr[now].array_definition.type = attr[g.ed[i].to].type_specifier.type;
			i = g.ed[i].next;
			analyse(g.ed[i].to);
			attr[now].array_definition.totsize += attr[g.ed[i].to].direct_array_declaration.type.steplenth();
			i = g.ed[i].next;
			while (i != 0) {
				i = g.ed[i].next;
				analyse(g.ed[i].to);
				attr[now].array_definition.totsize += attr[g.ed[i].to].direct_array_declaration.type.steplenth();
				i = g.ed[i].next;
			}
		}
		else if (g.p[now].content == "function_declaration") {
			int i = g.pre[now];
			analyse(g.ed[i].to);
			attr[now].function_declaration.type = attr[g.ed[i].to].type_specifier.type;
			i = g.ed[i].next;
			analyse(g.ed[i].to);
			i = g.ed[i].next;
			while (i != 0) {
				i = g.ed[i].next;
				analyse(g.ed[i].to);
				i = g.ed[i].next;
			}
		}
		else if (g.p[now].content == "function_definition") {
			int i = g.pre[now];
			analyse(g.ed[i].to);
			attr[now].function_definition.type = attr[g.ed[i].to].type_specifier.type;
			i = g.ed[i].next;
			int tmp2 = code.size();
			code.push_back(fsccode("jmp", "0"));//函数带套保护，等待回填
			analyse(g.ed[i].to);
			int tmp = attr[g.ed[i].to].direct_function_declaration.idx;
			attr[now].function_definition.idx = tmp;
			code.push_back(fsccode("mov", "$cx", "$st"));
			code.push_back(fsccode("sub", "$cx", to_string(table[tmp].argsiz + table[tmp].type.size + 1 + 1)));
			int tmp1 = code.size();
			code.push_back(fsccode("add", "$st", "0"));//给函数开栈，仅包含局部数据，大小等待回填
			i = g.ed[i].next;
			attr[g.ed[i].to].scope.funcidx = tmp;
			analyse(g.ed[i].to);
			table[tmp].size = table[tmp].argsiz + table[tmp].type.size + 1 + 1 + attr[g.ed[i].to].scope.size;
			code[tmp1].arg2 = to_string(attr[g.ed[i].to].scope.size);
			for (auto it = table[tmp].ret_fillback.begin(); it != table[tmp].ret_fillback.end(); it++)
				code[*it].arg1 = to_string(code.size());
			code.push_back(fsccode("sub", "$st", to_string(table[tmp].size)));//退栈
			code.push_back(fsccode("mov", "$cx", reg("$st", table[tmp].argsiz + table[tmp].type.size)));
			code.push_back(fsccode("mov", "$PC", reg("$st", table[tmp].argsiz + table[tmp].type.size + 1)));
			code[tmp2].arg1 = to_string(code.size());
		}
		else if (g.p[now].content == "type_specifier") {
			int i = g.pre[now];
			while (g.p[g.ed[i].to].content == "const") {
				attr[now].type_specifier.type.isconst = true;
				i = g.ed[i].next;
			}
			if (g.p[g.ed[i].to].content == "struct") {
				i = g.ed[i].next;
				int tmp = find(g.p[g.ed[i].to].content, 2);
				if (tmp == 0) error(g.p[now].pos, "未定义的结构体标识符\"" + g.p[g.ed[i].to].content + "\"");
				attr[now].type_specifier.type.datatype = tmp;
				attr[now].type_specifier.type.size = table[tmp].size;
			}
			else {
				analyse(g.ed[i].to);
				if (attr[g.ed[i].to].type_keyword.keyword == "void") {
					attr[now].type_specifier.type.datatype = 0;
					attr[now].type_specifier.type.size = 0;
				}
				else if (attr[g.ed[i].to].type_keyword.keyword == "var") {
					attr[now].type_specifier.type.datatype = -1;
					attr[now].type_specifier.type.size = 1;
				}
			}
			while (g.p[g.ed[i].to].content == "const") {
				attr[now].type_specifier.type.isconst = true;
				i = g.ed[i].next;
			}
		}
		else if (g.p[now].content == "full_type_specifier") {
			int i = g.pre[now];
			analyse(g.ed[i].to);
			attr[now].full_type_specifier.type = attr[g.ed[i].to].type_specifier.type;
			attr[now].full_type_specifier.type.dimension.push_back(attr[now].full_type_specifier.type.size);
			i = g.ed[i].next;
			while (i != 0) {
				analyse(g.ed[i].to);
				attr[now].full_type_specifier.type.isconstp.push_back(attr[g.ed[i].to].pointer_specifier.isconst);
				i = g.ed[i].next;
			}
			if (!attr[now].full_type_specifier.type.isconstp.empty()) attr[now].full_type_specifier.type.size = 1;
			else if (attr[now].full_type_specifier.type.size == 0) error(g.p[now].pos, "不允许使用不完整的结构体\"" + table[attr[now].full_type_specifier.type.datatype].name + "\"");

		}
		else if (g.p[now].content == "direct_variable_declaration") {
			attr[now].direct_variable_declaration.type = attr[g.p[now].fa].variable_definition.type;
			int i = g.pre[now];
			while (g.p[g.ed[i].to].content == "pointer_specifier") {
				analyse(g.ed[i].to);
				attr[now].direct_variable_declaration.type.isconstp.push_back(attr[g.ed[i].to].pointer_specifier.isconst);
				//attr[now].direct_variable_declaration.type.dimension.push_back(1);
				//区分开指针和数组，指针不影响dimension数组
				i = g.ed[i].next;
			}
			if (!attr[now].direct_variable_declaration.type.isconstp.empty()) attr[now].direct_variable_declaration.type.size = 1;
			attr[now].direct_variable_declaration.type.dimension.push_front(attr[now].direct_variable_declaration.type.size);
			int tmp = find(g.p[g.ed[i].to].content, 1);
			if (tmp > num[idx - 1]) error(g.p[now].pos, "重定义变量\"" + g.p[g.ed[i].to].content + "\"");
			num[idx]++;
			attr[now].direct_variable_declaration.idx = num[idx];
			table[num[idx]].name = g.p[g.ed[i].to].content;
			table[num[idx]].kind = 1;
			table[num[idx]].type = attr[now].direct_variable_declaration.type;
			if (idx == 1) {
				table[num[idx]].ptr = static_offset;
				if (static_offset + attr[now].direct_variable_declaration.type.size > 39999) error(g.p[now].pos, "静态存储区总大小不能超过39999");
				static_offset += attr[now].direct_variable_declaration.type.size;
			}
			else {
				table[num[idx]].ptr = stack_offset;
				stack_offset += attr[now].direct_variable_declaration.type.size;
			}
			string tmp1 = g.p[g.ed[i].to].content;
			i = g.ed[i].next;
			if (i != 0) {
				i = g.ed[i].next;
				attr[g.ed[i].to].initial_element.type = table[num[idx]].type;
				attr[g.ed[i].to].initial_element.ptr = table[num[idx]].ptr;
				analyse(g.ed[i].to);//初始化
			}
		}
		else if (g.p[now].content == "direct_array_declaration") {
			attr[now].direct_array_declaration.type = attr[g.p[now].fa].array_definition.type;
			int i = g.pre[now];
			while (g.p[g.ed[i].to].content == "pointer_specifier") {
				analyse(g.ed[i].to);
				attr[now].direct_array_declaration.type.isconstp.push_back(attr[g.ed[i].to].pointer_specifier.isconst);
				i = g.ed[i].next;
			}
			if (!attr[now].direct_array_declaration.type.isconstp.empty()) attr[now].direct_array_declaration.type.size = 1;
			int tmp = find(g.p[g.ed[i].to].content, 1);
			if (tmp > num[idx - 1]) error(g.p[now].pos, "重定义数组\"" + g.p[g.ed[i].to].content + "\"");
			num[idx]++;
			attr[now].direct_array_declaration.idx = num[idx];
			table[num[idx]].name = g.p[g.ed[i].to].content;
			table[num[idx]].kind = 1;
			table[num[idx]].type = attr[now].direct_array_declaration.type;
			table[num[idx]].size = 1;
			if (idx == 1) table[num[idx]].ptr = static_offset;
			else table[num[idx]].ptr = stack_offset;
			i = g.ed[i].next;
			i = g.ed[i].next;
			if (g.p[g.ed[i].to].content == "expression") {
				analyse(g.ed[i].to);
				if (!attr[g.ed[i].to].expression.isconstexpr || attr[g.ed[i].to].expression.isleftval) error(g.p[now].pos, "数组大小必须是常量表达式");
				if (attr[g.ed[i].to].expression.val > INT_MAX) error(g.p[now].pos, "数组太大");
				if (attr[g.ed[i].to].expression.val < 1) error(g.p[now].pos, "数组长度至少为1");
				table[num[idx]].type.dimension.push_front(attr[g.ed[i].to].expression.val);
				//sizeof阉割，数组第一维舍弃，但是是在表达式那块舍弃，这里还要
				table[num[idx]].type.isconstp.push_back(true);
				long long totsize = attr[g.ed[i].to].expression.val;
				i = g.ed[i].next;
				i = g.ed[i].next;
				while (g.p[g.ed[i].to].content == "[") {
					i = g.ed[i].next;
					analyse(g.ed[i].to);
					if (!attr[g.ed[i].to].expression.isconstexpr || attr[g.ed[i].to].expression.isleftval) error(g.p[now].pos, "数组大小必须是常量表达式");
					if (attr[g.ed[i].to].expression.val > INT_MAX) error(g.p[now].pos, "数组太大");
					if (attr[g.ed[i].to].expression.val < 1) error(g.p[now].pos, "数组长度至少为1");
					table[num[idx]].type.dimension.push_front(attr[g.ed[i].to].expression.val);
					table[num[idx]].type.isconstp.push_back(true);
					totsize *= attr[g.ed[i].to].expression.val;
					if (idx == 1 && static_offset + totsize > 39999) error(g.p[now].pos, "静态存储区总大小不能超过39999");
					i = g.ed[i].next;
					i = g.ed[i].next;
				}
				if (idx == 1 && static_offset + totsize > 39999) error(g.p[now].pos, "静态存储区总大小不能超过39999");
				table[num[idx]].type.dimension.push_front(attr[now].direct_array_declaration.type.size);
				//for (int j = 1; j < table[num[idx]].type.dimension.size(); j++)
				//	table[num[idx]].type.dimension[j] *= table[num[idx]].type.dimension[j - 1];
				table[num[idx]].type.size = 1;//数组类视为指针，大小视为1
				if (idx == 1) static_offset += totsize;
				else stack_offset += totsize;
				i = g.ed[i].next;
				if (i != 0) {
					if (g.p[g.ed[i].to].content == "=") i = g.ed[i].next;
					attr[g.ed[i].to].initial_list.type = table[num[idx]].type;
					attr[g.ed[i].to].initial_list.ptr = table[num[idx]].ptr;
					analyse(g.ed[i].to);
				}
			}
			else {//不定长数组
				i = g.ed[i].next;
				if (g.p[g.ed[i].to].content == "=") i = g.ed[i].next;
				attr[g.ed[i].to].initial_list.type = table[num[idx]].type;
				attr[g.ed[i].to].initial_list.ptr = table[num[idx]].ptr;
				analyse(g.ed[i].to);
				if (attr[g.ed[i].to].initial_list.size < 1) error(g.p[now].pos, "数组长度至少为1");
				table[num[idx]].type.dimension.push_front(attr[g.ed[i].to].initial_list.size);
				table[num[idx]].type.isconstp.push_back(true);
				table[num[idx]].type.dimension.push_front(attr[now].direct_array_declaration.type.size);
				long long totsize = attr[g.ed[i].to].initial_list.size;
				i = g.ed[i].next;
				if (idx == 1) static_offset += totsize;
				else stack_offset += totsize;
			}
			attr[now].direct_array_declaration.type = table[num[idx]].type;
		}
		else if (g.p[now].content == "direct_function_declaration") {
			if (g.p[g.p[now].fa].content == "function_declaration")
				attr[now].direct_function_declaration.type = attr[g.p[now].fa].function_declaration.type;
			else attr[now].direct_function_declaration.type = attr[g.p[now].fa].function_definition.type;
			int i = g.pre[now];
			while (g.p[g.ed[i].to].content == "pointer_specifier") {
				analyse(g.ed[i].to);
				attr[now].direct_function_declaration.type.isconstp.push_back(attr[g.ed[i].to].pointer_specifier.isconst);
				i = g.ed[i].next;
			}
			if (!attr[now].direct_function_declaration.type.isconstp.empty()) attr[now].direct_function_declaration.type.size = 1;
			attr[now].direct_function_declaration.type.dimension.push_front(attr[now].direct_function_declaration.type.size);
			if (std::find(internal_implemention.begin(), internal_implemention.end(), g.p[g.ed[i].to].content) != internal_implemention.end())
				error(g.p[now].pos, "不能声明已内部实施的函数");
			int tmp = find(g.p[g.ed[i].to].content, 0);
			if (tmp != 0) {//符号表中有
				if (table[tmp].defined) error(g.p[now].pos, "已定义的函数不能再次定义或声明\"" + g.p[g.ed[i].to].content + "\"");
				if (g.p[g.p[now].fa].content == "function_declaration") error(g.p[now].pos, "已声明的函数不能再次声明\"" + g.p[g.ed[i].to].content + "\"");
				if (table[tmp].type != attr[now].direct_function_declaration.type) error(g.p[now].pos, "函数声明和定义返回值类型不一致\"" + g.p[g.ed[i].to].content + "\"");
				attr[now].direct_function_declaration.idx = tmp;
				i = g.ed[i].next;//左括号
				i = g.ed[i].next;//参数表
				if (g.p[g.ed[i].to].content != ")") analyse(g.ed[i].to);
				if (attr[g.ed[i].to].argument_list.list != table[tmp].list) error(g.p[now].pos, "函数声明和定义参数不一致\"" + g.p[g.ed[i].to].content + "\"");
				table[tmp].defined = true;
				code[table[tmp].ptr].arg1 = to_string(code.size());//声明行跳转地址回填
			}
			else {//符号表中没有
				num[idx]++;
				attr[now].direct_function_declaration.idx = num[idx];
				table[num[idx]].name = g.p[g.ed[i].to].content;
				table[num[idx]].kind = 0;
				table[num[idx]].type = attr[now].direct_function_declaration.type;
				code.push_back(fsccode("jmp", to_string(code.size() + 2)));//声明也要带套保护！！！
				table[num[idx]].ptr = code.size();//注意scratch列表从1开始，是size+1
				table[num[idx]].defined = g.p[g.p[now].fa].content == "function_definition";
				if (g.p[g.p[now].fa].content == "function_declaration") code.push_back(fsccode("jmp", "0"));//函数声明后跳转位置等待回填
				i = g.ed[i].next;//左括号
				i = g.ed[i].next;//参数表
				if (g.p[g.ed[i].to].content != ")") analyse(g.ed[i].to);
				table[num[idx]].list = attr[g.ed[i].to].argument_list.list;
				for (auto it = table[num[idx]].list.begin(); it != table[num[idx]].list.end(); it++)
					table[num[idx]].argsiz += it->type.size;
			}
		}
		else if (g.p[now].content == "scope") {
			idx++;//新作用域
			num[idx] = num[idx - 1];//符号数量继承
			if (idx == 2) {//处理进入函数
				int tmp = attr[g.p[now].fa].function_definition.idx;
				stack_offset = 0;
				for (auto it = table[tmp].list.begin(); it != table[tmp].list.end(); it++) {
					num[idx]++;
					table[num[idx]].kind = 1;
					table[num[idx]].name = it->name;
					table[num[idx]].ptr = stack_offset;
					table[num[idx]].type = it->type;
					stack_offset += it->type.size;
				}
				stack_offset += table[tmp].type.size + 1 + 1;
			}
			int i = g.pre[now];
			i = g.ed[i].next;
			attr[g.ed[i].to].statement_list.funcidx = attr[now].scope.funcidx;
			attr[g.ed[i].to].statement_list.continuepos = attr[now].scope.continuepos;
			attr[g.ed[i].to].statement_list.breakpos = attr[now].scope.breakpos;
			analyse(g.ed[i].to);//分析statement_list
			attr[now].scope.size = attr[g.ed[i].to].statement_list.size;
			idx--;//作用域结束
		}
		else if (g.p[now].content == "type_keyword") {
			int i = g.pre[now];
			attr[now].type_keyword.keyword = g.p[g.ed[i].to].content;
		}
		else if (g.p[now].content == "pointer_specifier") {
			int i = g.pre[now];
			i = g.ed[i].next;
			if (i != 0) attr[now].pointer_specifier.isconst = true;
		}
		else if (g.p[now].content == "expression") {
			int i = g.pre[now];
			analyse(g.ed[i].to);
			if (g.ed[i].next == 0) attr[now].expression = attr[g.ed[i].to].expression;
			else {
				attr[now].expression.type.isconst = true;
				attr[now].expression.isconstexpr = false;
				attr[now].expression.isleftval = false;
				i = g.ed[i].next;
				while (i != 0) {
					i = g.ed[i].next;
					analyse(g.ed[i].to);
					attr[now].expression.argsiz = attr[g.ed[i].to].expression.argsiz;
					attr[now].expression.type = attr[g.ed[i].to].expression.type;
					i = g.ed[i].next;
				}
			}
		}
		else if (g.p[now].content == "assignment_expression") {
			int i = g.pre[now];
			if (g.p[g.ed[i].to].content == "conditional_expression") {
				analyse(g.ed[i].to);
				attr[now].expression = attr[g.ed[i].to].expression;
			}
			else {
				analyse(g.ed[i].to);
				if (attr[g.ed[i].to].expression.type.isconstp.empty()) {
					if (!attr[g.ed[i].to].expression.isleftval || attr[g.ed[i].to].expression.type.isconst) error(g.p[g.ed[i].to].pos, "表达式必须是可修改的左值");
				}
				else if (!attr[g.ed[i].to].expression.isleftval || attr[g.ed[i].to].expression.type.isconstp.back()) error(g.p[g.ed[i].to].pos, "表达式必须是可修改的左值");
				if (attr[g.ed[i].to].expression.isconstexpr)
					code.push_back(fsccode("mov", "$st(0)", to_string(attr[g.ed[i].to].expression.val)));
				else code.push_back(fsccode("mov", "$st(0)", reg("$st", attr[g.ed[i].to].expression.argsiz)));//把地址作为参数拷贝到0号位
				code.push_back(fsccode("add", "$st", "1"));//重要！！！参数拷贝过程一定要随之调整栈顶！不然会覆盖！！！！
				full_datatype type = attr[g.ed[i].to].expression.type;//左值数据类型
				int tmp = i;
				i = g.ed[i].next;
				analyse(g.ed[i].to);//计算第二个参数
				int op = attr[g.ed[i].to].assignment_operator.op;
				g.pre[now] = g.ed[i].next;
				analyse(now);
				g.pre[now] = tmp;
				if (op == 1) {//赋值
					if (attr[now].expression.type.size != type.size) error(g.p[now].pos, "赋值表达式类型不兼容");
					if (attr[now].expression.isconstexpr && !attr[now].expression.isleftval) code.push_back(fsccode("mov", "$st(0)", to_string(attr[now].expression.val)));//将结果拷贝到参数区，得到第二个参数
					else if (!attr[now].expression.isleftval) addmov("$st", 0, "$st", attr[now].expression.argsiz, type.size);
					else {//左值先拷贝地址到$ax，再利用$ax中存储的地址信息获取值的实际位置
						if (attr[now].expression.isconstexpr) code.push_back(fsccode("mov", "$ax", to_string(attr[now].expression.val)));
						else code.push_back(fsccode("mov", "$ax", reg("$st", attr[now].expression.argsiz)));
						addmov("$st", 0, "$ax", 0, type.size);
					}
					attr[now].expression.argsiz = attr[now].expression.type.size + 1;//一参，二参
					code.push_back(fsccode("add", "$st", to_string(type.size)));//调整栈顶防止覆盖
					//到这里函数参数计算完毕了，top在参数末端（开）
					int framesize = attr[now].expression.argsiz + type.size;//栈帧总大小
					code.push_back(fsccode("add", "$st", to_string(type.size)));//开栈帧空间，(参数)+返回值
					//执行赋值代码（注意，这个时候栈顶已经跑了！！！）
					code.push_back(fsccode("mov", "$ax", reg("$st", -framesize)));//先把要被赋值的地址记录在$ax中
					addmov("$ax", 0, "$st", -framesize + 1, type.size);//进行内存拷贝
					addmov("$st", -framesize + attr[now].expression.argsiz, "$ax", 0, type.size);//处理返回值
					//这个时候操作已经完成了
					code.push_back(fsccode("sub", "$st", to_string(framesize)));//栈顶指针回退
				}
				else {//运算赋值
					if (type.datatype != -1 && type.isconstp.empty() || attr[now].expression.type.datatype != -1 && attr[now].expression.type.isconstp.empty())
						error(g.p[now].pos, "表达式必须具有数值或指针类型");
					if (attr[now].expression.isconstexpr) code.push_back(fsccode("mov", "$st(0)", to_string(attr[now].expression.val)));//拷贝返回值到参数
					else if (!attr[now].expression.isleftval) code.push_back(fsccode("mov", "$st(0)", reg("$st", attr[now].expression.argsiz)));
					else {
						if (attr[now].expression.isconstexpr) code.push_back(fsccode("mov", "$ax", to_string(attr[now].expression.val)));
						else code.push_back(fsccode("mov", "$ax", reg("$st", attr[now].expression.argsiz)));
						code.push_back(fsccode("mov", "$st(0)", "$ax(0)"));
					}
					attr[now].expression.argsiz = 2;//一参，二参
					code.push_back(fsccode("add", "$st", "1"));
					//到这里函数参数计算完毕了，top在参数末端（开）
					code.push_back(fsccode("add", "$st", "1"));
					//code.push_back(fsccode("mov", "$st(1)", "$PC"));//拷贝机器状态
					code.push_back(fsccode("mov", "$bx", "$st(-3)"));
					if (op == 2) code.push_back(fsccode("mul", "$bx(0)", "$st(-2)"));
					else if (op == 3) code.push_back(fsccode("div", "$bx(0)", "$st(-2)"));
					else if (op == 4) {
						code.push_back(fsccode("mod", "$bx(0)", "$st(-2)"));
						code.push_back(fsccode("mov", "$st(-1)", "$ax"));
					}
					else if (op == 5) code.push_back(fsccode("add", "$bx(0)", "$st(-2)"));
					else code.push_back(fsccode("sub", "$bx(0)", "$st(-2)"));
					code.push_back(fsccode("mov", "$st(-1)", "$bx(0)"));
					//这个时候操作已经完成了
					code.push_back(fsccode("sub", "$st", "3"));//栈顶指针回退
					//code.push_back(fsccode("mov", "$PC", "$st(3)"));//机器状态还原
				}
				attr[now].expression.type.isconst = true;
				attr[now].expression.isconstexpr = false;
				attr[now].expression.isleftval = false;
			}
		}
		else if (g.p[now].content == "assignment_operator") {
			int i = g.pre[now];
			if (g.p[g.ed[i].to].content == "=") attr[now].assignment_operator.op = 1;
			else if (g.p[g.ed[i].to].content == "*=") attr[now].assignment_operator.op = 2;
			else if (g.p[g.ed[i].to].content == "/=") attr[now].assignment_operator.op = 3;
			else if (g.p[g.ed[i].to].content == "%=") attr[now].assignment_operator.op = 4;
			else if (g.p[g.ed[i].to].content == "+=") attr[now].assignment_operator.op = 5;
			else if (g.p[g.ed[i].to].content == "-=") attr[now].assignment_operator.op = 6;
			else error(g.p[g.ed[i].to].pos, "未知错误");
		}
		else if (g.p[now].content == "conditional_expression") {
			int i = g.pre[now];
			if (g.p[g.ed[i].to].content == "logical_or_expression") {
				analyse(g.ed[i].to);
				attr[now].expression = attr[g.ed[i].to].expression;
			}
			else {
				attr[now].expression.type.isconst = true;
				attr[now].expression.isconstexpr = false;
				attr[now].expression.isleftval = false;
				analyse(g.ed[i].to);
				if (attr[g.ed[i].to].expression.type.datatype != -1 && attr[g.ed[i].to].expression.type.isconstp.empty())
					error(g.p[now].pos, "表达式必须具有数值或指针类型");
				if (attr[g.ed[i].to].expression.isconstexpr && !attr[g.ed[i].to].expression.isleftval) {//第一个参数是常量表达式的话优化
					warning(g.p[now].pos, "你这三目运算符第一个参数是常量表达式。\n"
						"如果你在正常使用，那没啥事，这个已经给优化了。这条警告你忽略就好了\n"
						"如果你就是想找茬，你故意写这一大堆无意义代码等着我给你擦屁股，那我日你妈。\n"
						"但是我并不知道你具体是不是故意的所以我没法攻击你，所以我只能人身攻击你，你tm就是个大傻逼，\n"
						"因为你的存在，我他妈要多写这么多代码优化！你看看你干的屌事！你别用sCr了你不配，滚蛋");
					if (attr[g.ed[i].to].expression.val > 0) {
						i = g.ed[i].next;
						i = g.ed[i].next;
						analyse(g.ed[i].to);
						attr[now].expression.argsiz = attr[g.ed[i].to].expression.argsiz;//注意，这里三目运算符被优化掉了，所以不再会有返回值转移，所以直接记录argsiz
					}
					else {
						i = g.ed[i].next;
						i = g.ed[i].next;
						i = g.ed[i].next;
						g.pre[now] = g.ed[i].next;//直接改变树的结构优化，不需要再改回来
						analyse(now);//argsiz跟随了，不用修改
					}
					return;//优化结束，直接跳出
				}
				int tmp = code.size();//记录当前代码行数
				code.push_back(fsccode("add", "$st", "0"));//栈顶上移给返回值预留空间，但是返回值大小不确定，所以等待回填，行数为tmp
				if (!attr[g.ed[i].to].expression.isleftval) code.push_back(fsccode("grt", reg("$st", attr[g.ed[i].to].expression.argsiz), "0"));//判断第一个参数是否大于0
				else {
					if (attr[g.ed[i].to].expression.isconstexpr) code.push_back(fsccode("mov", "$ax", to_string(attr[g.ed[i].to].expression.val)));
					else code.push_back(fsccode("mov", "$ax", reg("$st", attr[g.ed[i].to].expression.argsiz)));
					code.push_back(fsccode("grt", "$ax(0)", "0"));
				}
				int tmp1 = code.size();
				code.push_back(fsccode("jnz", "0"));//如果非0，也就是不大于0，就跳转到否分支，具体位置不确定，等待回填，行数为tmp1
				i = g.ed[i].next;
				i = g.ed[i].next;
				analyse(g.ed[i].to);//分析expression项
				full_datatype type = attr[g.ed[i].to].expression.type;
				//第一个分支直接决定三目运算符结果类型，如果第二个分支类型不符直接报错。但是不同类型的指针可以算作相同类型
				code[tmp].arg2 = to_string(type.size);//回填预留空间大小
				if (attr[g.ed[i].to].expression.isconstexpr && !attr[g.ed[i].to].expression.isleftval) code.push_back(fsccode("mov", reg("$st", -type.size), reg("$st", attr[g.ed[i].to].expression.argsiz)));//拷贝返回值
				else if (!attr[g.ed[i].to].expression.isleftval) addmov("$st", -type.size, "$st", attr[g.ed[i].to].expression.argsiz, type.size);
				else {
					if (attr[g.ed[i].to].expression.isconstexpr) code.push_back(fsccode("mov", "$ax", to_string(attr[now].expression.val)));
					else code.push_back(fsccode("mov", "$ax", reg("$st", attr[g.ed[i].to].expression.argsiz)));
					addmov("$st", -type.size, "$ax", 0, type.size);
				}
				//到这里对第一个分支的处理就结束了
				int tmp2 = code.size();//记录另一个当前代码行数
				code.push_back(fsccode("jmp", "0"));//第一个分支结束之后跳过第二个分支，具体位置不确定，等待回填，行数为tmp2;
				code[tmp1].arg1 = to_string(code.size());//第二个分支跳转位置回填
				i = g.ed[i].next;
				int tmp3 = g.pre[now];
				g.pre[now] = g.ed[i].next;
				analyse(now);
				g.pre[now] = tmp3;
				if (attr[g.ed[i].to].expression.isconstexpr && !attr[g.ed[i].to].expression.isleftval) code.push_back(fsccode("mov", reg("$st", -type.size), reg("$st", attr[g.ed[i].to].expression.argsiz)));//拷贝返回值
				else if (!attr[g.ed[i].to].expression.isleftval) addmov("$st", -type.size, "$st", attr[g.ed[i].to].expression.argsiz, type.size);
				else {
					if (attr[g.ed[i].to].expression.isconstexpr) code.push_back(fsccode("mov", "$ax", to_string(attr[now].expression.val)));
					else code.push_back(fsccode("mov", "$ax", reg("$st", attr[g.ed[i].to].expression.argsiz)));
					addmov("$st", -type.size, "$ax", 0, type.size);
				}
				//到这里对第二个分支的处理就结束了
				if (type != attr[now].expression.type && (type.isconstp.empty() || attr[now].expression.type.isconstp.empty()))
					error(g.p[now].pos, "三目运算符两个分支的运算结果类型不兼容");
				code[tmp2].arg1 = to_string(code.size());//第二个分支结束跳转位置回填
				attr[now].expression.argsiz = 0;//没有额外参数
			}
		}
		else if (g.p[now].content == "logical_or_expression") {
			int i = g.pre[now];
			if (g.ed[i].next == 0) {
				analyse(g.ed[i].to);
				attr[now].expression = attr[g.ed[i].to].expression;
			}
			else {
				attr[now].expression.type.isconst = true;//不需要自递归，所以把属性放前面
				attr[now].expression.isconstexpr = true;//假设是常量表达式
				attr[now].expression.isleftval = false;
				attr[now].expression.type.datatype = -1;
				attr[now].expression.type.size = 1;
				attr[now].expression.argsiz = 0;
				code.push_back(fsccode("add", "$st", "1"));//预留空间
				code.push_back(fsccode("mov", "$st(-1)", "0"));//假设为假
				analyse(g.ed[i].to);
				if (attr[g.ed[i].to].expression.type.datatype != -1 && attr[g.ed[i].to].expression.type.isconstp.empty())
					error(g.p[now].pos, "表达式必须具有数值或指针类型");
				if (attr[g.ed[i].to].expression.isconstexpr && !attr[g.ed[i].to].expression.isleftval) {
					if (attr[g.ed[i].to].expression.val > 0) {
						code.push_back(fsccode("mov", "$st(-1)", "1"));//一次性短路
						return;
					}
				}
				else if (!attr[g.ed[i].to].expression.isleftval) {
					attr[now].expression.isconstexpr = false;
					code.push_back(fsccode("grt", reg("$st", attr[g.ed[i].to].expression.argsiz), "0"));
					code.push_back(fsccode("mov", "$st(-1)", "$ax"));
				}
				else {
					attr[now].expression.isconstexpr = false;
					if (attr[g.ed[i].to].expression.isconstexpr) code.push_back(fsccode("mov", "$ax", to_string(attr[g.ed[i].to].expression.val)));
					else code.push_back(fsccode("mov", "$ax", reg("$st", attr[g.ed[i].to].expression.argsiz)));
					code.push_back(fsccode("grt", "$ax(0)", "0"));
					code.push_back(fsccode("mov", "$st(-1)", "$ax"));
				}
				vector<int> tmp;//短路跳转回填列表
				tmp.push_back(code.size());
				code.push_back(fsccode("jz", "0"));
				i = g.ed[i].next;
				while (i != 0) {
					i = g.ed[i].next;
					analyse(g.ed[i].to);
					if (attr[g.ed[i].to].expression.type.datatype != -1 && attr[g.ed[i].to].expression.type.isconstp.empty())
						error(g.p[now].pos, "表达式必须具有数值或指针类型");
					if (attr[g.ed[i].to].expression.isconstexpr && !attr[g.ed[i].to].expression.isleftval) {
						if (attr[g.ed[i].to].expression.val > 0) {
							attr[now].expression.isconstexpr = true;//一次性短路
							attr[now].expression.val = 1;
							return;
						}
					}
					else if (!attr[g.ed[i].to].expression.isleftval) {
						attr[now].expression.isconstexpr = false;
						code.push_back(fsccode("grt", reg("$st", attr[g.ed[i].to].expression.argsiz), "0"));
						code.push_back(fsccode("mov", "$st(-1)", "$ax"));
					}
					else {
						attr[now].expression.isconstexpr = false;
						if (attr[g.ed[i].to].expression.isconstexpr) code.push_back(fsccode("mov", "$ax", to_string(attr[g.ed[i].to].expression.val)));
						else code.push_back(fsccode("mov", "$ax", reg("$st", attr[g.ed[i].to].expression.argsiz)));
						code.push_back(fsccode("grt", "$ax(0)", "0"));
						code.push_back(fsccode("mov", "$st(-1)", "$ax"));
					}
					tmp.push_back(code.size());
					code.push_back(fsccode("jz", "0"));
					i = g.ed[i].next;
				}
				for (auto it = tmp.begin(); it != tmp.end(); it++) code[*it].arg1 = to_string(code.size());//短路跳转
				code.push_back(fsccode("sub", "$st", "1"));//退栈
			}
		}
		else if (g.p[now].content == "logical_and_expression") {
			int i = g.pre[now];
			if (g.ed[i].next == 0) {
				analyse(g.ed[i].to);
				attr[now].expression = attr[g.ed[i].to].expression;
			}
			else {
				attr[now].expression.type.isconst = true;//不需要自递归，所以把属性放前面
				attr[now].expression.isconstexpr = true;//假设是常量表达式
				attr[now].expression.isleftval = false;
				attr[now].expression.type.datatype = -1;
				attr[now].expression.type.size = 1;
				attr[now].expression.argsiz = 0;
				code.push_back(fsccode("add", "$st", "1"));//预留空间
				code.push_back(fsccode("mov", "$st(-1)", "1"));//假设为真
				analyse(g.ed[i].to);
				if (attr[g.ed[i].to].expression.type.datatype != -1 && attr[g.ed[i].to].expression.type.isconstp.empty())
					error(g.p[now].pos, "表达式必须具有数值或指针类型");
				if (attr[g.ed[i].to].expression.isconstexpr && !attr[g.ed[i].to].expression.isleftval) {
					if (attr[g.ed[i].to].expression.val <= 0) {
						attr[now].expression.isconstexpr = false;//一次性短路
						attr[now].expression.val = 1;
						return;
					}
				}
				else if (!attr[g.ed[i].to].expression.isleftval) {
					attr[now].expression.isconstexpr = false;
					code.push_back(fsccode("grt", reg("$st", attr[g.ed[i].to].expression.argsiz), "0"));
					code.push_back(fsccode("mov", "$st(-1)", "$ax"));
				}
				else {
					attr[now].expression.isconstexpr = false;
					if (attr[g.ed[i].to].expression.isconstexpr) code.push_back(fsccode("mov", "$ax", to_string(attr[g.ed[i].to].expression.val)));
					else code.push_back(fsccode("mov", "$ax", reg("$st", attr[g.ed[i].to].expression.argsiz)));
					code.push_back(fsccode("grt", "$ax(0)", "0"));
					code.push_back(fsccode("mov", "$st(-1)", "$ax"));
				}
				vector<int> tmp;//短路跳转回填列表
				tmp.push_back(code.size());
				code.push_back(fsccode("jnz", "0"));
				i = g.ed[i].next;
				while (i != 0) {
					i = g.ed[i].next;
					analyse(g.ed[i].to);
					if (attr[g.ed[i].to].expression.type.datatype != -1 && attr[g.ed[i].to].expression.type.isconstp.empty())
						error(g.p[now].pos, "表达式必须具有数值或指针类型");
					if (attr[g.ed[i].to].expression.isconstexpr && !attr[g.ed[i].to].expression.isleftval) {
						if (attr[g.ed[i].to].expression.val > 0) {
							code.push_back(fsccode("mov", "$st(-1)", "0"));//一次性短路
							return;
						}
					}
					else if (!attr[g.ed[i].to].expression.isleftval) {
						attr[now].expression.isconstexpr = false;
						code.push_back(fsccode("grt", reg("$st", attr[g.ed[i].to].expression.argsiz), "0"));
						code.push_back(fsccode("mov", "$st(-1)", "$ax"));
					}
					else {
						attr[now].expression.isconstexpr = false;
						if (attr[g.ed[i].to].expression.isconstexpr) code.push_back(fsccode("mov", "$ax", to_string(attr[g.ed[i].to].expression.val)));
						else code.push_back(fsccode("mov", "$ax", reg("$st", attr[g.ed[i].to].expression.argsiz)));
						code.push_back(fsccode("grt", "$ax(0)", "0"));
						code.push_back(fsccode("mov", "$st(-1)", "$ax"));
					}
					tmp.push_back(code.size());
					code.push_back(fsccode("jnz", "0"));
					i = g.ed[i].next;
				}
				for (auto it = tmp.begin(); it != tmp.end(); it++) code[*it].arg1 = to_string(code.size());//短路跳转
				code.push_back(fsccode("sub", "$st", "1"));//退栈
			}
		}
		else if (g.p[now].content == "equality_expression") {
			int i = g.pre[now];
			if (g.ed[i].next == 0) {
				analyse(g.ed[i].to);
				attr[now].expression = attr[g.ed[i].to].expression;
			}
			else {
				attr[now].expression.type.isconst = true;//不需要自递归，所以把属性放前面
				attr[now].expression.isconstexpr = true;//假设是常量表达式
				attr[now].expression.isleftval = false;
				attr[now].expression.type.datatype = -1;
				attr[now].expression.type.size = 1;
				attr[now].expression.argsiz = 0;
				code.push_back(fsccode("add", "$st", "1"));
				analyse(g.ed[i].to);
				if (attr[g.ed[i].to].expression.type.datatype != -1 && attr[g.ed[i].to].expression.type.isconstp.empty()) error(g.p[now].pos, "表达式必须具有数值或指针类型");
				//这种结构简单，不用严格按照函数开栈的模式，直接简单分配一下就行
				if (attr[g.ed[i].to].expression.isconstexpr && !attr[g.ed[i].to].expression.isleftval) attr[now].expression.val = attr[g.ed[i].to].expression.val;
				else if (!attr[g.ed[i].to].expression.isleftval) {
					attr[now].expression.isconstexpr = false;
					code.push_back(fsccode("mov", "$st(-1)", reg("$st", attr[g.ed[i].to].expression.argsiz)));
				}
				else {
					attr[now].expression.isconstexpr = false;
					if (attr[g.ed[i].to].expression.isconstexpr) code.push_back(fsccode("mov", "$ax", to_string(attr[g.ed[i].to].expression.val)));
					else code.push_back(fsccode("mov", "$ax", reg("$st", attr[g.ed[i].to].expression.argsiz)));
					code.push_back(fsccode("mov", "$st(-1)", "$ax(0)"));//!!!!!!!!
				}
				i = g.ed[i].next;
				while (i != 0) {
					analyse(g.ed[i].to);
					int op = attr[g.ed[i].to].equality_operator.op;
					i = g.ed[i].next;
					analyse(g.ed[i].to);
					if (attr[g.ed[i].to].expression.type.datatype != -1 && attr[g.ed[i].to].expression.type.isconstp.empty()) error(g.p[now].pos, "表达式必须具有数值或指针类型");
					if (attr[g.ed[i].to].expression.isconstexpr && !attr[g.ed[i].to].expression.isleftval) {
						if (attr[now].expression.isconstexpr) {
							if (op == 1) attr[now].expression.val = attr[now].expression.val == attr[g.ed[i].to].expression.val;
							else attr[now].expression.val = attr[now].expression.val != attr[g.ed[i].to].expression.val;
						}
						else {
							code.push_back(fsccode("eq", "$st(-1)", to_string(attr[g.ed[i].to].expression.val)));
							code.push_back(fsccode("mov", "$st(-1)", "$ax"));
							if (op == 2) code.push_back(fsccode("not", "$st(-1)"));
						}
					}
					else if (!attr[g.ed[i].to].expression.isleftval) {
						if (attr[now].expression.isconstexpr) {
							code.push_back(fsccode("eq", to_string(attr[now].expression.val), reg("$st", attr[g.ed[i].to].expression.argsiz)));
							code.push_back(fsccode("mov", "$st(-1)", "$ax"));
							if (op == 2) code.push_back(fsccode("not", "$st(-1)"));
						}
						else {
							code.push_back(fsccode("eq", "$st(-1)", reg("$st", attr[g.ed[i].to].expression.argsiz)));
							code.push_back(fsccode("mov", "$st(-1)", "$ax"));
							if (op == 2) code.push_back(fsccode("not", "$st(-1)"));
						}
						attr[now].expression.isconstexpr = false;
					}
					else {

						if (attr[g.ed[i].to].expression.isconstexpr) code.push_back(fsccode("mov", "$ax", to_string(attr[g.ed[i].to].expression.val)));
						else code.push_back(fsccode("mov", "$ax", reg("$st", attr[g.ed[i].to].expression.argsiz)));
						if (attr[now].expression.isconstexpr) code.push_back(fsccode("eq", to_string(attr[now].expression.val), "$ax(0)"));
						else code.push_back(fsccode("eq", "$st(-1)", "$ax(0)"));
						code.push_back(fsccode("mov", "$st(-1)", "$ax"));
						if (op == 2) code.push_back(fsccode("not", "$st(-1)"));
						attr[now].expression.isconstexpr = false;
					}
					i = g.ed[i].next;
				}
				code.push_back(fsccode("sub", "$st", "1"));//退栈
			}
		}
		else if (g.p[now].content == "equality_operator") {
			int i = g.pre[now];
			if (g.p[g.ed[i].to].content == "==") attr[now].equality_operator.op = 1;
			else if (g.p[g.ed[i].to].content == "!=") attr[now].equality_operator.op = 2;
			else error(g.p[g.ed[i].to].pos, "未知错误");
		}
		else if (g.p[now].content == "relational_expression") {
			int i = g.pre[now];
			if (g.ed[i].next == 0) {
				analyse(g.ed[i].to);
				attr[now].expression = attr[g.ed[i].to].expression;
			}
			else {
				attr[now].expression.type.isconst = true;//不需要自递归，所以把属性放前面
				attr[now].expression.isconstexpr = true;//假设是常量表达式
				attr[now].expression.isleftval = false;
				attr[now].expression.type.datatype = -1;
				attr[now].expression.type.size = 1;
				attr[now].expression.argsiz = 0;
				code.push_back(fsccode("add", "$st", "1"));
				analyse(g.ed[i].to);
				if (attr[g.ed[i].to].expression.type.datatype != -1 && attr[g.ed[i].to].expression.type.isconstp.empty()) error(g.p[now].pos, "表达式必须具有数值或指针类型");
				//这种结构简单，不用严格按照函数开栈的模式，直接简单分配一下就行
				if (attr[g.ed[i].to].expression.isconstexpr && !attr[g.ed[i].to].expression.isleftval) attr[now].expression.val = attr[g.ed[i].to].expression.val;
				else if (!attr[g.ed[i].to].expression.isleftval) {
					attr[now].expression.isconstexpr = false;
					code.push_back(fsccode("mov", "$st(-1)", reg("$st", attr[g.ed[i].to].expression.argsiz)));
				}
				else {
					attr[now].expression.isconstexpr = false;
					if (attr[g.ed[i].to].expression.isconstexpr) code.push_back(fsccode("mov", "$ax", to_string(attr[g.ed[i].to].expression.val)));
					else code.push_back(fsccode("mov", "$ax", reg("$st", attr[g.ed[i].to].expression.argsiz)));
					code.push_back(fsccode("mov", "$st(-1)", "$ax(0)"));//!!!!!!!!
				}
				i = g.ed[i].next;
				while (i != 0) {
					analyse(g.ed[i].to);
					int op = attr[g.ed[i].to].equality_operator.op;
					i = g.ed[i].next;
					analyse(g.ed[i].to);
					if (attr[g.ed[i].to].expression.type.datatype != -1 && attr[g.ed[i].to].expression.type.isconstp.empty()) error(g.p[now].pos, "表达式必须具有数值或指针类型");
					if (attr[g.ed[i].to].expression.isconstexpr && !attr[g.ed[i].to].expression.isleftval) {
						if (attr[now].expression.isconstexpr) {
							if (op == 1) attr[now].expression.val = attr[now].expression.val < attr[g.ed[i].to].expression.val;
							else if (op == 2) attr[now].expression.val = attr[now].expression.val > attr[g.ed[i].to].expression.val;
							else if (op == 3) attr[now].expression.val = attr[now].expression.val <= attr[g.ed[i].to].expression.val;
							else attr[now].expression.val = attr[now].expression.val >= attr[g.ed[i].to].expression.val;
						}
						else {
							if (op == 1 || op == 4) {
								code.push_back(fsccode("les", "$st(-1)", to_string(attr[g.ed[i].to].expression.val)));
								if (op == 4) code.push_back(fsccode("not", "$ax"));
							}
							else {
								code.push_back(fsccode("grt", "$st(-1)", to_string(attr[g.ed[i].to].expression.val)));
								if (op == 3) code.push_back(fsccode("not", "$ax"));
							}
							code.push_back(fsccode("mov", "$st(-1)", "$ax"));
						}
					}
					else if (!attr[g.ed[i].to].expression.isleftval) {
						if (attr[now].expression.isconstexpr) {
							if (op == 1 || op == 4) {
								code.push_back(fsccode("les", to_string(attr[now].expression.val), reg("$st", attr[g.ed[i].to].expression.argsiz)));
								if (op == 4) code.push_back(fsccode("not", "$ax"));
							}
							else {
								code.push_back(fsccode("grt", to_string(attr[now].expression.val), reg("$st", attr[g.ed[i].to].expression.argsiz)));
								if (op == 3) code.push_back(fsccode("not", "$ax"));
							}
							code.push_back(fsccode("mov", "$st(-1)", "$ax"));
						}
						else {
							if (op == 1 || op == 4) {
								code.push_back(fsccode("les", "$st(-1)", reg("$st", attr[g.ed[i].to].expression.argsiz)));
								if (op == 4) code.push_back(fsccode("not", "$ax"));
							}
							else {
								code.push_back(fsccode("grt", "$st(-1)", reg("$st", attr[g.ed[i].to].expression.argsiz)));
								if (op == 3) code.push_back(fsccode("not", "$ax"));
							}
							code.push_back(fsccode("mov", "$st(-1)", "$ax"));
						}
						attr[now].expression.isconstexpr = false;
					}
					else {
						if (attr[g.ed[i].to].expression.isconstexpr) code.push_back(fsccode("mov", "$ax", to_string(attr[g.ed[i].to].expression.val)));
						else code.push_back(fsccode("mov", "$ax", reg("$st", attr[g.ed[i].to].expression.argsiz)));
						if (op == 1 || op == 4) {
							if (attr[now].expression.isconstexpr) code.push_back(fsccode("les", to_string(attr[now].expression.val), "$ax(0)"));
							else code.push_back(fsccode("les", "$st(-1)", "$ax(0)"));
							if (op == 4) code.push_back(fsccode("not", "$ax"));
						}
						else {
							if (attr[now].expression.isconstexpr) code.push_back(fsccode("grt", to_string(attr[now].expression.val), "$ax(0)"));
							else code.push_back(fsccode("grt", "$st(-1)", "$ax(0)"));
							if (op == 3) code.push_back(fsccode("not", "$ax"));
						}
						code.push_back(fsccode("mov", "$st(-1)", "$ax"));
						attr[now].expression.isconstexpr = false;
					}
					i = g.ed[i].next;
				}
				code.push_back(fsccode("sub", "$st", "1"));//退栈
			}
		}
		else if (g.p[now].content == "relational_operator") {
			int i = g.pre[now];
			if (g.p[g.ed[i].to].content == "<") attr[now].equality_operator.op = 1;
			else if (g.p[g.ed[i].to].content == ">") attr[now].equality_operator.op = 2;
			else if (g.p[g.ed[i].to].content == "<=") attr[now].equality_operator.op = 3;
			else if (g.p[g.ed[i].to].content == ">=") attr[now].equality_operator.op = 4;
			else error(g.p[g.ed[i].to].pos, "未知错误");
		}
		else if (g.p[now].content == "additive_expression") {
			int i = g.pre[now];
			if (g.ed[i].next == 0) {
				analyse(g.ed[i].to);
				attr[now].expression = attr[g.ed[i].to].expression;
			}
			else {
				attr[now].expression.type.isconst = true;//不需要自递归，所以把属性放前面
				attr[now].expression.isconstexpr = true;//假设是常量表达式
				attr[now].expression.isleftval = false;
				attr[now].expression.type.datatype = -1;
				attr[now].expression.type.size = 1;
				attr[now].expression.argsiz = 0;
				code.push_back(fsccode("add", "$st", "1"));//!!!!!!!!
				analyse(g.ed[i].to);
				if (attr[g.ed[i].to].expression.type.datatype != -1 && attr[g.ed[i].to].expression.type.isconstp.empty()) error(g.p[now].pos, "表达式必须具有数值或指针类型");
				//这种结构简单，不用严格按照函数开栈的模式，直接简单分配一下就行
				if (attr[g.ed[i].to].expression.isconstexpr && !attr[g.ed[i].to].expression.isleftval) attr[now].expression.val = attr[g.ed[i].to].expression.val;
				else if (!attr[g.ed[i].to].expression.isleftval) {
					attr[now].expression.isconstexpr = false;
					code.push_back(fsccode("mov", "$st(-1)", reg("$st", attr[g.ed[i].to].expression.argsiz)));
				}
				else {
					attr[now].expression.isconstexpr = false;
					if (attr[g.ed[i].to].expression.isconstexpr) code.push_back(fsccode("mov", "$ax", to_string(attr[g.ed[i].to].expression.val)));
					else code.push_back(fsccode("mov", "$ax", reg("$st", attr[g.ed[i].to].expression.argsiz)));
					code.push_back(fsccode("mov", "$st(-1)", "$ax(0)"));//!!!!!!!!
				}
				i = g.ed[i].next;
				while (i != 0) {
					analyse(g.ed[i].to);
					int op = attr[g.ed[i].to].equality_operator.op;
					i = g.ed[i].next;
					analyse(g.ed[i].to);
					if (attr[g.ed[i].to].expression.type.datatype != -1 && attr[g.ed[i].to].expression.type.isconstp.empty()) error(g.p[now].pos, "表达式必须具有数值或指针类型");
					if (attr[g.ed[i].to].expression.isconstexpr && !attr[g.ed[i].to].expression.isleftval) {
						if (attr[now].expression.isconstexpr) {
							if (op == 1) attr[now].expression.val = attr[now].expression.val + attr[g.ed[i].to].expression.val;
							else attr[now].expression.val = attr[now].expression.val - attr[g.ed[i].to].expression.val;
						}
						else {
							if (op == 1) code.push_back(fsccode("add", "$st(-1)", to_string(attr[g.ed[i].to].expression.val)));//!!!!!!!!!
							else code.push_back(fsccode("sub", "$st(-1)", to_string(attr[g.ed[i].to].expression.val)));
						}
					}
					else if (!attr[g.ed[i].to].expression.isleftval) {
						if (attr[now].expression.isconstexpr) code.push_back(fsccode("mov", "$st(-1)", to_string(attr[now].expression.val)));
						if (op == 1) code.push_back(fsccode("add", "$st(-1)", reg("$st", attr[g.ed[i].to].expression.argsiz)));
						else code.push_back(fsccode("sub", "$st(-1)", reg("$st", attr[g.ed[i].to].expression.argsiz)));
						attr[now].expression.isconstexpr = false;
					}
					else {
						if (attr[g.ed[i].to].expression.isconstexpr) code.push_back(fsccode("mov", "$ax", to_string(attr[g.ed[i].to].expression.val)));
						else code.push_back(fsccode("mov", "$ax", reg("$st", attr[g.ed[i].to].expression.argsiz)));
						if (op == 1) {
							if (attr[now].expression.isconstexpr) code.push_back(fsccode("add", to_string(attr[now].expression.val), "$ax(0)"));
							else code.push_back(fsccode("add", "$st(-1)", "$ax(0)"));
						}
						else {
							if (attr[now].expression.isconstexpr) code.push_back(fsccode("sub", to_string(attr[now].expression.val), "$ax(0)"));
							else code.push_back(fsccode("sub", "$st(-1)", "$ax(0)"));
						}
						attr[now].expression.isconstexpr = false;
					}
					i = g.ed[i].next;
				}
				code.push_back(fsccode("sub", "$st", "1"));//退栈
			}
		}
		else if (g.p[now].content == "additive_operator") {
			int i = g.pre[now];
			if (g.p[g.ed[i].to].content == "+") attr[now].equality_operator.op = 1;
			else if (g.p[g.ed[i].to].content == "-") attr[now].equality_operator.op = 2;
			else error(g.p[g.ed[i].to].pos, "未知错误");
		}
		else if (g.p[now].content == "multiplicative_expression") {
			int i = g.pre[now];
			if (g.ed[i].next == 0) {
				analyse(g.ed[i].to);
				attr[now].expression = attr[g.ed[i].to].expression;
			}
			else {
				attr[now].expression.type.isconst = true;//不需要自递归，所以把属性放前面
				attr[now].expression.isconstexpr = true;//假设是常量表达式
				attr[now].expression.isleftval = false;
				attr[now].expression.type.datatype = -1;
				attr[now].expression.type.size = 1;
				attr[now].expression.argsiz = 0;
				code.push_back(fsccode("add", "$st", "1"));
				analyse(g.ed[i].to);
				if (attr[g.ed[i].to].expression.type.datatype != -1 && attr[g.ed[i].to].expression.type.isconstp.empty()) error(g.p[now].pos, "表达式必须具有数值或指针类型");
				//这种结构简单，不用严格按照函数开栈的模式，直接简单分配一下就行
				if (attr[g.ed[i].to].expression.isconstexpr && !attr[g.ed[i].to].expression.isleftval) attr[now].expression.val = attr[g.ed[i].to].expression.val;
				else if (!attr[g.ed[i].to].expression.isleftval) {
					attr[now].expression.isconstexpr = false;
					code.push_back(fsccode("mov", "$st(-1)", reg("$st", attr[g.ed[i].to].expression.argsiz)));
				}
				else {
					attr[now].expression.isconstexpr = false;
					if (attr[g.ed[i].to].expression.isconstexpr) code.push_back(fsccode("mov", "$ax", to_string(attr[g.ed[i].to].expression.val)));
					else code.push_back(fsccode("mov", "$ax", reg("$st", attr[g.ed[i].to].expression.argsiz)));
					code.push_back(fsccode("mov", "$st(-1)", "$ax(0)"));
				}
				i = g.ed[i].next;
				while (i != 0) {
					analyse(g.ed[i].to);
					int op = attr[g.ed[i].to].equality_operator.op;
					i = g.ed[i].next;
					analyse(g.ed[i].to);
					if (attr[g.ed[i].to].expression.type.datatype != -1 && attr[g.ed[i].to].expression.type.isconstp.empty()) error(g.p[now].pos, "表达式必须具有数值或指针类型");
					if (attr[g.ed[i].to].expression.isconstexpr && !attr[g.ed[i].to].expression.isleftval) {
						if (attr[now].expression.isconstexpr) {
							if (op == 1) attr[now].expression.val = attr[now].expression.val * attr[g.ed[i].to].expression.val;
							else if (op == 2) attr[now].expression.val = attr[now].expression.val / attr[g.ed[i].to].expression.val;
							else attr[now].expression.val = mod(attr[now].expression.val, attr[g.ed[i].to].expression.val);
						}
						else {
							if (op == 1) code.push_back(fsccode("mul", "$st(-1)", to_string(attr[g.ed[i].to].expression.val)));
							else if (op == 2) code.push_back(fsccode("div", "$st(-1)", to_string(attr[g.ed[i].to].expression.val)));
							else {
								code.push_back(fsccode("mod", "$st(-1)", to_string(attr[g.ed[i].to].expression.val)));
								code.push_back(fsccode("mov", "$st(-1)", "$ax"));
							}
						}
					}
					else if (!attr[g.ed[i].to].expression.isleftval) {
						if (attr[now].expression.isconstexpr) code.push_back(fsccode("mov", "$st(-1)", to_string(attr[now].expression.val)));
						if (op == 1) code.push_back(fsccode("mul", "$st(-1)", reg("$st", attr[g.ed[i].to].expression.argsiz)));
						else if (op == 2) code.push_back(fsccode("div", "$st(-1)", reg("$st", attr[g.ed[i].to].expression.argsiz)));
						else {
							code.push_back(fsccode("mod", "$st(-1)", reg("$st", attr[g.ed[i].to].expression.argsiz)));
							code.push_back(fsccode("mov", "$st(-1)", "$ax"));
						}
						attr[now].expression.isconstexpr = false;
					}
					else {
						if (attr[g.ed[i].to].expression.isconstexpr) code.push_back(fsccode("mov", "$ax", to_string(attr[g.ed[i].to].expression.val)));
						else code.push_back(fsccode("mov", "$ax", reg("$st", attr[g.ed[i].to].expression.argsiz)));
						if (op == 1) {
							if (attr[now].expression.isconstexpr) code.push_back(fsccode("mul", to_string(attr[now].expression.val), "$ax(0)"));
							else code.push_back(fsccode("mul", "$st(-1)", "$ax(0)"));
						}
						else if (op == 2) {
							if (attr[now].expression.isconstexpr) code.push_back(fsccode("div", to_string(attr[now].expression.val), "$ax(0)"));
							else code.push_back(fsccode("div", "$st(-1)", "$ax(0)"));
						}
						else {
							if (attr[now].expression.isconstexpr) code.push_back(fsccode("mod", to_string(attr[now].expression.val), "$ax(0)"));
							else code.push_back(fsccode("mod", "$st(-1)", "$ax(0)"));
							code.push_back(fsccode("mov", "$st(-1)", "$ax"));
						}
						attr[now].expression.isconstexpr = false;
					}
					i = g.ed[i].next;
				}
				code.push_back(fsccode("sub", "$st", "1"));//退栈
			}
		}
		else if (g.p[now].content == "multiplicative_operator") {
			int i = g.pre[now];
			if (g.p[g.ed[i].to].content == "*") attr[now].equality_operator.op = 1;
			else if (g.p[g.ed[i].to].content == "/") attr[now].equality_operator.op = 2;
			else if (g.p[g.ed[i].to].content == "%") attr[now].equality_operator.op = 3;
			else error(g.p[g.ed[i].to].pos, "未知错误");
		}
		else if (g.p[now].content == "power_expression") {
			int i = g.pre[now];
			if (g.ed[i].next == 0) {
				analyse(g.ed[i].to);
				attr[now].expression = attr[g.ed[i].to].expression;
			}
			else {
				attr[now].expression.type.isconst = true;//不需要自递归，所以把属性放前面
				attr[now].expression.isconstexpr = true;//假设是常量表达式
				attr[now].expression.isleftval = false;
				attr[now].expression.type.datatype = -1;
				attr[now].expression.type.size = 1;
				attr[now].expression.argsiz = 0;
				code.push_back(fsccode("add", "$st", "1"));
				analyse(g.ed[i].to);
				if (attr[g.ed[i].to].expression.type.datatype != -1 && attr[g.ed[i].to].expression.type.isconstp.empty()) error(g.p[now].pos, "表达式必须具有数值或指针类型");
				//这种结构简单，不用严格按照函数开栈的模式，直接简单分配一下就行
				if (attr[g.ed[i].to].expression.isconstexpr && !attr[g.ed[i].to].expression.isleftval) attr[now].expression.val = attr[g.ed[i].to].expression.val;
				else if (!attr[g.ed[i].to].expression.isleftval) {
					attr[now].expression.isconstexpr = false;
					code.push_back(fsccode("mov", "$st(-1)", reg("$st", attr[g.ed[i].to].expression.argsiz)));
				}
				else {
					attr[now].expression.isconstexpr = false;
					if (attr[g.ed[i].to].expression.isconstexpr) code.push_back(fsccode("mov", "$ax", to_string(attr[g.ed[i].to].expression.val)));
					else code.push_back(fsccode("mov", "$ax", reg("$st", attr[g.ed[i].to].expression.argsiz)));
					code.push_back(fsccode("mov", "$st(-1)", "$ax(0)"));
				}
				i = g.ed[i].next;
				while (i != 0) {
					i = g.ed[i].next;
					analyse(g.ed[i].to);
					if (attr[g.ed[i].to].expression.type.datatype != -1 && attr[g.ed[i].to].expression.type.isconstp.empty()) error(g.p[now].pos, "表达式必须具有数值或指针类型");
					if (attr[g.ed[i].to].expression.isconstexpr && !attr[g.ed[i].to].expression.isleftval) {
						if (attr[now].expression.isconstexpr)
							attr[now].expression.val = pow(attr[now].expression.val, attr[g.ed[i].to].expression.val);
						else {
							code.push_back(fsccode("pow", "$st(-1)", to_string(attr[now].expression.val)));
							code.push_back(fsccode("mov", "$st(-1)", "$ax"));
						}
					}
					else if (!attr[g.ed[i].to].expression.isleftval) {
						if (attr[now].expression.isconstexpr) {
							code.push_back(fsccode("pow", to_string(attr[now].expression.val), reg("$st", attr[g.ed[i].to].expression.argsiz)));
							code.push_back(fsccode("mov", "$st(-1)", "$ax"));
						}
						else {
							code.push_back(fsccode("pow", "$st(-1)", reg("$st", attr[g.ed[i].to].expression.argsiz)));
							code.push_back(fsccode("mov", "$st(-1)", "$ax"));
						}
						attr[now].expression.isconstexpr = false;
					}
					else {
						if (attr[g.ed[i].to].expression.isconstexpr) code.push_back(fsccode("mov", "$ax", to_string(attr[g.ed[i].to].expression.val)));
						else code.push_back(fsccode("mov", "$ax", reg("$st", attr[g.ed[i].to].expression.argsiz)));
						if (attr[now].expression.isconstexpr) code.push_back(fsccode("pow", to_string(attr[now].expression.val), "$ax(0)"));
						else code.push_back(fsccode("pow", "$st(-1)", "$ax(0)"));
						code.push_back(fsccode("mov", "$st(-1)", "$ax"));
						attr[now].expression.isconstexpr = false;
					}
					i = g.ed[i].next;
				}
				code.push_back(fsccode("sub", "$st", "1"));//退栈
			}
		}
		else if (g.p[now].content == "unary_expression") {
			int i = g.pre[now];
			if (g.ed[i].next == 0) {
				analyse(g.ed[i].to);
				attr[now].expression = attr[g.ed[i].to].expression;
				//必须要转移一下，不然有问题
				code.push_back(fsccode("mov", "$st(0)", reg("$st", attr[now].expression.argsiz)));
				attr[now].expression.argsiz = 0;
			}
			else if (g.p[g.ed[i].to].content == "sizeof") {
				i = g.ed[i].next;
				i = g.ed[i].next;
				attr[now].expression.isconstexpr = true;
				attr[now].expression.isleftval = false;
				analyse(g.ed[i].to);
				attr[now].expression.val = attr[g.ed[i].to].full_type_specifier.type.size;
			}
			else {
				//不一定是常量
				//返回值是左是右不确定
				//数据类型不确定，可能是任何类型，大小也不确定，可能得到结构体
				attr[now].expression.argsiz = 0;//不需要参数，简单结构，直接返回值操作

				int tmp = g.pre[now];
				g.pre[now] = g.ed[i].next;
				analyse(now);
				g.pre[now] = tmp;
				code.push_back(fsccode("add", "$st", "1"));//仍然需要推一格栈，防止后面解析后覆盖
				analyse(g.ed[i].to);
				int op = attr[g.ed[i].to].unary_operation.op;
				if (op == 1) {//&，取地址，必须是左值
					if (!attr[now].expression.isleftval) error(g.p[now].pos, "表达式必须是可修改的左值");
					attr[now].expression.type.isconstp.push_back(true);
					attr[now].expression.type.size = 1;
					attr[now].expression.type.isconst = true;
					attr[now].expression.isleftval = false;//取完之后就不是左值了
					//if (!attr[now].expression.isconstexpr) {
					//	code.push_back(fsccode("mov", "$st(-1)", reg("$st", attr[g.ed[i].to].expression.argsiz)));
					//}
					if (attr[now].expression.isconstexpr) {
						code.push_back(fsccode("mov", "$st(-1)", to_string(attr[now].expression.val)));
					}
				}
				else if (op == 2) {//*，访问地址
					if (attr[now].expression.type.isconstp.empty()) error(g.p[now].pos, "表达式必须具有指针类型");
					attr[now].expression.type.isconst = attr[now].expression.type.isconstp.back();
					attr[now].expression.type.isconstp.pop_back();
					attr[now].expression.isleftval = false;
					if (attr[now].expression.type.dimension.size() > 1) attr[now].expression.type.dimension.pop_back();
					else attr[now].expression.isleftval = true;
					if (attr[now].expression.type.isconstp.empty()) {//访问结果直接是一个非指针类型数据
						if (attr[now].expression.type.datatype == -1) attr[now].expression.type.size = 1;
						else if (attr[now].expression.type.datatype == 0) error(g.p[now].pos, "表达式不允许出现void类型的值");
						//else if (!table[attr[now].expression.type.datatype].defined) error(g.p[now].pos, "不允许使用不完整的类型:\"" + table[attr[now].expression.type.datatype].name + "\"");
						else attr[now].expression.type.size = table[attr[now].expression.type.datatype].size;
						attr[now].expression.type.dimension.front() = attr[now].expression.type.size;
					}
					//如果访问结果仍然是指针，则不用特殊处理
					if (attr[now].expression.isconstexpr && !attr[now].expression.isleftval) code.push_back(fsccode("mov", "$st(-1)", to_string(attr[now].expression.val)));
					else if (!attr[now].expression.isleftval);//啥也不用写，值就在那呆着呢，还恰好可以处理数组指针问题
					else {
						if (attr[now].expression.isconstexpr) code.push_back(fsccode("mov", "$ax", to_string(attr[now].expression.val)));
						else code.push_back(fsccode("mov", "$ax", "$st(-1)"));
						code.push_back(fsccode("mov", "$st(-1)", "$ax(0)"));
					}
					attr[now].expression.isconstexpr = false;//一定不是常量表达式，因为不可能知道当前地址中存储的数据是什么
				}
				else if (op == 3) {//+，正号
					if (attr[now].expression.type.datatype != -1 || !attr[now].expression.type.isconstp.empty()) error(g.p[now].pos, "表达式必须是基本var类型");
					if (attr[now].expression.isleftval) {
						if (attr[now].expression.isconstexpr) code.push_back(fsccode("mov", "$ax", to_string(attr[now].expression.val)));
						else code.push_back(fsccode("mov", "$ax", "$st(-1)"));
						code.push_back(fsccode("mov", "$st(-1)", "$ax(0)"));
						attr[now].expression.isconstexpr = false;
						attr[now].expression.type.isconst = true;
					}
					attr[now].expression.isleftval = false;
				}
				else if (op == 4) {//-，负号
					if (attr[now].expression.type.datatype != -1 || !attr[now].expression.type.isconstp.empty()) error(g.p[now].pos, "表达式必须是基本var类型");
					if (attr[now].expression.isconstexpr && !attr[now].expression.isleftval) attr[now].expression.val = -attr[now].expression.val;
					else if (!attr[now].expression.isleftval) code.push_back(fsccode("mul", "$st(-1)", "-1"));
					else {
						if (attr[now].expression.isconstexpr) code.push_back(fsccode("mov", "$ax", to_string(attr[now].expression.val)));
						else code.push_back(fsccode("mov", "$ax", "$st(-1)"));
						code.push_back(fsccode("mov", "$st(-1)", "$ax(0)"));
						code.push_back(fsccode("mul", "$st(-1)", "-1"));
						attr[now].expression.isconstexpr = false;
						attr[now].expression.type.isconst = true;
					}
					attr[now].expression.isleftval = false;
				}
				else if (op == 5) {//!，逻辑非
					if (attr[now].expression.type.datatype != -1 || !attr[now].expression.type.isconstp.empty()) error(g.p[now].pos, "表达式必须是基本var类型");
					if (attr[now].expression.isconstexpr && !attr[now].expression.isleftval) attr[now].expression.val = !attr[now].expression.val;
					else if (!attr[now].expression.isleftval) code.push_back(fsccode("not", "$st(-1)"));
					else {
						if (attr[now].expression.isconstexpr) code.push_back(fsccode("mov", "$ax", to_string(attr[now].expression.val)));
						else code.push_back(fsccode("mov", "$ax", "$st(-1)"));
						code.push_back(fsccode("mov", "$st(-1)", "$ax(0)"));
						code.push_back(fsccode("not", "$st(-1)"));
						attr[now].expression.isconstexpr = false;
						attr[now].expression.type.isconst = true;
					}
					attr[now].expression.isleftval = false;
				}
				else if (op == 6) {//++，前置自增
					if (attr[now].expression.type.isconstp.empty()) {
						if (!attr[now].expression.isleftval || attr[now].expression.type.isconst) error(g.p[now].pos, "表达式必须是可修改的左值");
					}
					else if (!attr[now].expression.isleftval || attr[now].expression.type.isconstp.back()) error(g.p[now].pos, "表达式必须是可修改的左值");
					if (attr[now].expression.type.datatype != -1 && attr[now].expression.type.isconstp.empty()) error(g.p[now].pos, "表达式必须具有数值或指针类型");
					attr[now].expression.isleftval = false;
					attr[now].expression.type.isconst = true;
					if (attr[now].expression.isconstexpr) {
						code.push_back(fsccode("add", reg("$0", attr[now].expression.val), to_string(attr[now].expression.type.steplenth())));
						code.push_back(fsccode("mov", "$st(-1)", reg("$0", attr[now].expression.val)));
					}
					else {
						code.push_back(fsccode("mov", "$ax", "$st(-1)"));//$ax搭桥处理左值
						code.push_back(fsccode("add", "$ax(0)", to_string(attr[now].expression.type.steplenth())));
						code.push_back(fsccode("mov", "$st(-1)", "$ax(0)"));
					}
					attr[now].expression.isconstexpr = false;//左值变右值了，isconstexpr一定是false
				}
				else if (op == 7) {//--，前置自减
					if (attr[now].expression.type.isconstp.empty()) {
						if (!attr[now].expression.isleftval || attr[now].expression.type.isconst) error(g.p[now].pos, "表达式必须是可修改的左值");
					}
					else if (!attr[now].expression.isleftval || attr[now].expression.type.isconstp.back()) error(g.p[now].pos, "表达式必须是可修改的左值");
					if (attr[now].expression.type.datatype != -1 && attr[now].expression.type.isconstp.empty()) error(g.p[now].pos, "表达式必须具有数值或指针类型");
					attr[now].expression.isleftval = false;
					attr[now].expression.type.isconst = true;
					if (attr[now].expression.isconstexpr) {
						code.push_back(fsccode("sub", reg("$0", attr[now].expression.val), to_string(attr[now].expression.type.steplenth())));
						code.push_back(fsccode("mov", "$st(-1)", reg("$0", attr[now].expression.val)));
					}
					else {
						code.push_back(fsccode("mov", "$ax", "$st(-1)"));//$ax搭桥处理左值
						code.push_back(fsccode("sub", "$ax(0)", to_string(attr[now].expression.type.steplenth())));
						code.push_back(fsccode("mov", "$st(-1)", "$ax(0)"));
					}
					attr[now].expression.isconstexpr = false;//左值变右值了，isconstexpr一定是false
				}
				else if (op == 8) {//sizeof
					attr[now].expression.val = attr[now].expression.type.steplenth();
					attr[now].expression.isconstexpr = true;
					attr[now].expression.isleftval = false;
				}
				else if (op == 9) {//强制类型转换
					if (attr[now].expression.type.size != attr[g.ed[i].to].unary_operation.type.size) error(g.p[now].pos, "强制类型转换的类型不兼容");
					if (attr[now].expression.isleftval) {
						if (attr[now].expression.isconstexpr) code.push_back(fsccode("mov", "$ax", to_string(attr[now].expression.val)));
						else code.push_back(fsccode("mov", "$ax", "$st(-1)"));
						code.push_back(fsccode("mov", "$st(-1)", "$ax(0)"));
					}
					attr[now].expression.type = attr[g.ed[i].to].unary_operation.type;
					attr[now].expression.isleftval = false;
				}
				code.push_back(fsccode("sub", "$st", "1"));
			}
		}
		else if (g.p[now].content == "unary_operation") {
			int i = g.pre[now];
			if (g.p[g.ed[i].to].content == "&") attr[now].unary_operation.op = 1;
			else if (g.p[g.ed[i].to].content == "*") attr[now].unary_operation.op = 2;
			else if (g.p[g.ed[i].to].content == "+") attr[now].unary_operation.op = 3;
			else if (g.p[g.ed[i].to].content == "-") attr[now].unary_operation.op = 4;
			else if (g.p[g.ed[i].to].content == "!") attr[now].unary_operation.op = 5;
			else if (g.p[g.ed[i].to].content == "++") attr[now].unary_operation.op = 6;
			else if (g.p[g.ed[i].to].content == "--") attr[now].unary_operation.op = 7;
			else if (g.p[g.ed[i].to].content == "sizeof") attr[now].unary_operation.op = 8;
			else if (g.p[g.ed[i].to].content == "(") {
				attr[now].unary_operation.op = 9;
				i = g.ed[i].next;
				analyse(g.ed[i].to);
				attr[now].unary_operation.type = attr[g.ed[i].to].full_type_specifier.type;
			}
			else error(g.p[now].pos, "未知错误");
		}
		else if (g.p[now].content == "postfix_expression") {
			int i = g.pre[now];
			if (g.ed[i].next == 0) {
				analyse(g.ed[i].to);
				attr[now].expression = attr[g.ed[i].to].expression;
			}
			else {
				analyse(g.ed[i].to);
				attr[now].expression = attr[g.ed[i].to].expression;
				attr[now].expression.argsiz = 0;
				int framesize = attr[g.ed[i].to].expression.type.size;
				if (attr[g.ed[i].to].expression.isleftval) framesize = 1;
				addmov("$st", 0, "$st", attr[g.ed[i].to].expression.argsiz, framesize);
				code.push_back(fsccode("add", "$st", to_string(framesize)));//先把初始参数的栈开出来
				i = g.ed[i].next;
				while (i != 0) {
					attr[now].expression.argsiz = 0;
					analyse(g.ed[i].to);
					int op = attr[g.ed[i].to].postfix_operation.op;
					const expression& expr = attr[g.ed[i].to].postfix_operation.expr;
					if (op == 1) {//[expr]，访问指针或数组
						if (attr[now].expression.type.isconstp.empty()) error(g.p[now].pos, "表达式必须具有指针类型");
						if (expr.type.datatype != -1) error(g.p[now].pos, "表达式必须是基本var类型");
						if (attr[now].expression.isconstexpr && !attr[now].expression.isleftval)//第一步：先把指针的值读到位置
							code.push_back(fsccode("mov", "$st(-1)", to_string(attr[now].expression.val)));
						else if (!attr[now].expression.isleftval);//啥也不用干，现成的地址放着
						else {
							if (attr[now].expression.isconstexpr) code.push_back(fsccode("mov", "$ax", to_string(attr[now].expression.val)));
							else code.push_back(fsccode("mov", "$ax", "$st(-1)"));//⚠⚠乱改的，可能会炸
							code.push_back(fsccode("mov", "$st(-1)", "$ax(0)"));
						}
						//第二步：指针偏移，偏移量存$bx
						if (expr.isconstexpr && !expr.isleftval) {
							code.push_back(fsccode("mov", "$bx", to_string(expr.val)));
							//attr[now].expression.val += expr.val;
						}
						else if (!expr.isleftval) code.push_back(fsccode("mov", "$bx", "$st(0)"));//⚠⚠乱改的，可能会炸
						else {
							if (expr.isconstexpr) code.push_back(fsccode("mov", "$ax", to_string(expr.val)));
							else code.push_back(fsccode("mov", "$ax", "$st(0)"));//⚠⚠乱改的，可能会炸
							code.push_back(fsccode("mov", "$bx", "$ax(0)"));
						}
						code.push_back(fsccode("mul", "$bx", to_string(attr[now].expression.type.steplenth())));
						attr[now].expression.type.isconstp.pop_back();
						attr[now].expression.isleftval = false;
						if (attr[now].expression.type.dimension.size() == 1) attr[now].expression.isleftval = true;
						else attr[now].expression.type.dimension.pop_back();
						if (attr[now].expression.type.isconstp.empty()) {
							if (attr[now].expression.type.datatype == -1) attr[now].expression.type.size = 1;
							else if (attr[now].expression.type.datatype == 0) error(g.p[now].pos, "表达式不允许出现void类型的值");
							else attr[now].expression.type.size = table[attr[now].expression.type.datatype].size;
						}
						code.push_back(fsccode("add", "$st(-1)", "$bx"));
						attr[now].expression.isconstexpr = false;
					}
					else if (op == 2) {//.，访问成员
						if (attr[now].expression.type.datatype <= 0) error(g.p[now].pos, "表达式必须是结构体类型的变量");
						int addr = 0;//成员相对整个结构体的地址，枚举获取
						full_datatype type = full_datatype();//该成员的类型
						const string& name = attr[g.ed[i].to].postfix_operation.name;
						for (auto it = table[attr[now].expression.type.datatype].list.begin(); it != table[attr[now].expression.type.datatype].list.end(); it++) {
							if (it->name == name) {
								type = it->type;
								break;
							}
							addr += it->type.steplenth();
						}
						if (type.datatype == 0) error(g.p[now].pos, "结构体\"" + table[attr[now].expression.type.datatype].name + "\"没有成员\"" + name + "\"");
						//因为是结构体，所以不可能是常量表达式
						if (!attr[now].expression.isleftval) {
							addmov("$st", -framesize + addr, "$st", -framesize, type.size);
							code.push_back(fsccode("add", "$st", to_string(-framesize + type.size)));
							framesize = type.size;
						}
						else {
							if (attr[now].expression.isconstexpr) attr[now].expression.val += addr;
							else code.push_back(fsccode("add", "$st(-1)", to_string(addr)));
						}
						if (type.dimension.size() > 1) {
							type.dimension.pop_back();//数组降维退化
							attr[now].expression.isleftval = false;
						}
						attr[now].expression.type = type;
					}
					else if (op == 3) {//->，通过地址访问成员
						if (attr[now].expression.type.isconstp.size() != 1 || attr[now].expression.type.datatype <= 0) error(g.p[now].pos, "表达式必须具有结构体指针类型");
						if (attr[now].expression.isconstexpr && !attr[now].expression.isleftval)//第一步：先把指针的值读到位置
							code.push_back(fsccode("mov", "$st(-1)", to_string(attr[now].expression.val)));
						else if (!attr[now].expression.isleftval);//啥也不用干，现成的地址放着
						else {
							if (attr[now].expression.isconstexpr) code.push_back(fsccode("mov", "$ax", to_string(attr[now].expression.val)));
							else code.push_back(fsccode("mov", "$ax", "$st(-1)"));
							code.push_back(fsccode("mov", "$st(-1)", "$ax(0)"));
						}
						//上述所有分支保证$st(-1)存储左参数的具体值，这个值就是要访问的地址
						int addr = 0;//成员相对整个结构体的地址，枚举获取
						full_datatype type = full_datatype();//该成员的类型
						const string& name = attr[g.ed[i].to].postfix_operation.name;
						for (auto it = table[attr[now].expression.type.datatype].list.begin(); it != table[attr[now].expression.type.datatype].list.end(); it++) {
							if (it->name == name) {
								type = it->type;
								break;
							}
							addr += it->type.steplenth();
						}
						if (type.datatype == 0) error(g.p[now].pos, "结构体\"" + table[attr[now].expression.type.datatype].name + "\"没有成员\"" + name + "\"");
						if (type.dimension.size() > 1) {
							type.dimension.pop_back();//数组降维退化
							attr[now].expression.isleftval = false;
						}
						code.push_back(fsccode("add", "$st(-1)", to_string(addr)));
						attr[now].expression.type = type;
						attr[now].expression.isconstexpr = false;//从变量读取的地址，一定不是常量表达式
						attr[now].expression.isleftval = true;//从地址读取的值，一定是左值
					}
					else if (op == 4) {//++，后置自增
						if (attr[now].expression.type.isconstp.empty()) {
							if (!attr[now].expression.isleftval || attr[now].expression.type.isconst) error(g.p[now].pos, "表达式必须是可修改的左值");
						}
						else if (!attr[now].expression.isleftval || attr[now].expression.type.isconstp.back()) error(g.p[now].pos, "表达式必须是可修改的左值");
						if (attr[now].expression.type.datatype != -1 && attr[now].expression.type.isconstp.empty()) error(g.p[now].pos, "表达式必须具有数值或指针类型");
						attr[now].expression.isleftval = false;
						attr[now].expression.type.isconst = true;
						if (attr[now].expression.isconstexpr) {
							code.push_back(fsccode("mov", "$st(-1)", reg("$0", attr[now].expression.val)));
							code.push_back(fsccode("add", reg("$0", attr[now].expression.val), to_string(attr[now].expression.type.steplenth())));
						}
						else {
							code.push_back(fsccode("mov", "$ax", "$st(-1)"));//$ax搭桥处理左值
							code.push_back(fsccode("mov", "$st(-1)", "$ax(0)"));
							code.push_back(fsccode("add", "$ax(0)", to_string(attr[now].expression.type.steplenth())));
						}
						attr[now].expression.isconstexpr = false;//左值变右值了，isconstexpr一定是false
					}
					else if (op == 5) {//--，后置自减
						if (attr[now].expression.type.isconstp.empty()) {
							if (!attr[now].expression.isleftval || attr[now].expression.type.isconst) error(g.p[now].pos, "表达式必须是可修改的左值");
						}
						else if (!attr[now].expression.isleftval || attr[now].expression.type.isconstp.back()) error(g.p[now].pos, "表达式必须是可修改的左值");
						if (attr[now].expression.type.datatype != -1 && attr[now].expression.type.isconstp.empty()) error(g.p[now].pos, "表达式必须具有数值或指针类型");
						attr[now].expression.isleftval = false;
						attr[now].expression.type.isconst = true;
						if (attr[now].expression.isconstexpr) {
							code.push_back(fsccode("mov", "$st(-1)", reg("$0", attr[now].expression.val)));
							code.push_back(fsccode("sub", reg("$0", attr[now].expression.val), to_string(attr[now].expression.type.steplenth())));
						}
						else {
							code.push_back(fsccode("mov", "$ax", "$st(-1)"));//$ax搭桥处理左值
							code.push_back(fsccode("mov", "$st(-1)", "$ax(0)"));
							code.push_back(fsccode("sub", "$ax(0)", to_string(attr[now].expression.type.steplenth())));
						}
						attr[now].expression.isconstexpr = false;//左值变右值了，isconstexpr一定是false
					}
					i = g.ed[i].next;
				}
				code.push_back(fsccode("sub", "$st", to_string(framesize)));
			}
		}
		else if (g.p[now].content == "postfix_operation") {
			int i = g.pre[now];
			if (g.p[g.ed[i].to].content == "[") {
				attr[now].postfix_operation.op = 1;
				i = g.ed[i].next;
				analyse(g.ed[i].to);
				attr[now].postfix_operation.expr = attr[g.ed[i].to].expression;
			}
			else if (g.p[g.ed[i].to].content == ".") {
				attr[now].postfix_operation.op = 2;
				i = g.ed[i].next;
				attr[now].postfix_operation.name = g.p[g.ed[i].to].content;
			}
			else if (g.p[g.ed[i].to].content == "->") {
				attr[now].postfix_operation.op = 3;
				i = g.ed[i].next;
				attr[now].postfix_operation.name = g.p[g.ed[i].to].content;
			}
			else if (g.p[g.ed[i].to].content == "++") attr[now].postfix_operation.op = 4;
			else if (g.p[g.ed[i].to].content == "--") attr[now].postfix_operation.op = 5;
			else error(g.p[now].pos, "未知错误");
		}
		else if (g.p[now].content == "primary_expression") {
			attr[now].expression.argsiz = 0;
			int i = g.pre[now];
			if (g.p[g.ed[i].to].type == "IDENTIFIER") {
				const string& name = g.p[g.ed[i].to].content;
				if (g.ed[i].next == 0) {//变量或数组
					int tmp = find(name, 1);
					if (tmp == 0) error(g.p[now].pos, "未定义变量或数组\"" + name + "\"");
					attr[now].expression.type = table[tmp].type;
					if (attr[now].expression.type.dimension.size() == 1) {//变量
						attr[now].expression.isleftval = true;
						//如果在栈上，地址就是不知道的
						if (tmp > num[1]) {
							attr[now].expression.isconstexpr = false;
							code.push_back(fsccode("mov", "$st(0)", to_string(table[tmp].ptr)));
							code.push_back(fsccode("add", "$st(0)", "$cx"));
						}
						else {
							attr[now].expression.isconstexpr = true;
							attr[now].expression.val = table[tmp].ptr;
						}
					}
					else {//数组
						attr[now].expression.isleftval = false;//是一个临时生成的指针，不是左值
						attr[now].expression.type.dimension.pop_back();//数组进表达式要退化
						if (tmp > num[1]) {
							attr[now].expression.isconstexpr = false;
							code.push_back(fsccode("mov", "$st(0)", to_string(table[tmp].ptr)));
							code.push_back(fsccode("add", "$st(0)", "$cx"));
						}
						else {
							attr[now].expression.isconstexpr = true;
							attr[now].expression.val = table[tmp].ptr;
						}
					}
				}
				else {//函数
					int tmp = find(name, 0);
					if (tmp == 0) error(g.p[now].pos, "未声明的函数\"" + name + "\"");
					if (!table[tmp].defined) warning(g.p[now].pos, "已声明但未定义的函数\"" + name + "\"");
					attr[now].expression.argsiz = table[tmp].argsiz;
					attr[now].expression.isconstexpr = false;
					attr[now].expression.isleftval = false;
					attr[now].expression.type = table[tmp].type;
					//警告⚠⚠⚠这是函数调用语句⚠⚠⚠要仔细分配栈内存！！！！
					//参数，返回值，访问链，机器状态，局部变量。函数所需总空间为符号表中的size，参数大小为符号表中的argsiz。
					i = g.ed[i].next;//这里是左括号
					int argnum = 0;
					while (g.p[g.ed[i].to].content != ")" && g.p[g.ed[g.ed[i].next].to].content != ")") {
						i = g.ed[i].next;//这里是下一个参数（如果有）
						argnum++;
						if (argnum > table[tmp].list.size()) error(g.p[now].pos, "调用函数\"" + name + "\"参数过多");
						analyse(g.ed[i].to);
						if (attr[g.ed[i].to].expression.isconstexpr && !attr[g.ed[i].to].expression.isleftval)
							code.push_back(fsccode("mov", "$st(0)", to_string(attr[g.ed[i].to].expression.val)));
						else if (!attr[g.ed[i].to].expression.isleftval)
							addmov("$st", 0, "$st", attr[g.ed[i].to].expression.argsiz, table[tmp].list[argnum - 1].type.size);
						else {
							if (attr[g.ed[i].to].expression.isconstexpr) code.push_back(fsccode("mov", "$ax", to_string(attr[g.ed[i].to].expression.val)));
							else code.push_back(fsccode("mov", "$ax", reg("$st", attr[g.ed[i].to].expression.argsiz)));
							addmov("$st", 0, "$ax", 0, table[tmp].list[argnum - 1].type.size);
						}
						code.push_back(fsccode("add", "$st", to_string(table[tmp].list[argnum - 1].type.size)));//栈顶上移
						i = g.ed[i].next;//这里是逗号
					}
					if (argnum < table[tmp].list.size()) error(g.p[now].pos, "调用函数\"" + name + "\"参数过少");
					code.push_back(fsccode("add", "$st", to_string(table[tmp].type.size + 1 + 1)));//这里栈顶在机器状态后面
					code.push_back(fsccode("mov", "$st(-2)", "$cx"));//$cx存入访问链备份
					code.push_back(fsccode("mov", "$st(-1)", "$PC"));//保存机器状态，
					code.push_back(fsccode("add", "$st(-1)", "2"));//保存的行数是jmp的行数
					code.push_back(fsccode("jmp", to_string(table[tmp].ptr)));//跳转到函数，之后的操作由函数来处理
					/*
					code.push_back(fsccode("add", "$st", to_string(table[tmp].size - table[tmp].argsiz)));
					//到这里函数已经运行完毕了，返回值就在那放着不用加工就可以直接用
					code.push_back(fsccode("sub", "$st", to_string(table[tmp].size)));
					*/
					//⚠⚠⚠⚠上面那堆涉及函数局部数据大小，如果函数没有定义则未知，所以局部栈调整由函数自己处理！！！调用者不用管！！！！
				}
			}
			else if (g.p[g.ed[i].to].type == "CONSTANT") {//常量
				string x = g.p[g.ed[i].to].content;
				attr[now].expression.argsiz = 0;
				attr[now].expression.isconstexpr = true;
				attr[now].expression.isleftval = false;
				attr[now].expression.type.datatype = -1;
				attr[now].expression.type.isconst = true;
				attr[now].expression.type.size = 1;
				if (x.front() == '\"') {
					attr[now].expression.val = static_offset;
					attr[now].expression.type.isconstp.push_back(true);
					for (int i = 1; i < x.size() - 1; i++) {
						if (x[i] == '\\') code.push_back(fsccode("mov", reg("$0", static_offset++), to_string(to_char(x.substr(i++, 2)))));
						else code.push_back(fsccode("mov", reg("$0", static_offset++), to_string(x[i])));
					}
					code.push_back(fsccode("mov", reg("$0", static_offset++), "0"));
				}
				else if (x.front() == '\'') {
					if (x.size() > 4 || x.size() < 3) error(g.p[now].pos, "格式错误的字符\'" + x + "\'");
					attr[now].expression.val = to_char(x.substr(1, x.size() - 2));
				}
				else attr[now].expression.val = to_double(x);
			}
			else {//括号里面表达式
				i = g.ed[i].next;
				analyse(g.ed[i].to);
				attr[now].expression = attr[g.ed[i].to].expression;
			}
		}
		else if (g.p[now].content == "initial_list") {
			int i = g.pre[now];
			full_datatype type = attr[now].initial_list.type;
			i = g.ed[i].next;
			if (g.p[g.ed[i].to].content == "initial_element") {//列表初始化
				int ptr = attr[now].initial_list.ptr;
				if (type.dimension.size() == 1) {//结构体
					int tmp = type.datatype;
					for (auto it = table[tmp].list.begin();g.p[g.ed[i].to].content != "}";it++) {
						if (it == table[tmp].list.end()) error(g.p[now].pos, "结构体变量初始值设定项目过多");
						attr[g.ed[i].to].initial_element.type = it->type;
						attr[g.ed[i].to].initial_element.ptr = ptr;
						analyse(g.ed[i].to);
						ptr += it->type.steplenth();
						i = g.ed[i].next;
						if (g.p[g.ed[i].to].content == ",") i = g.ed[i].next;
					}
				}
				else {//数组
					int remain = type.dimension.back();
					type.dimension.pop_back();
					int steplenth = type.steplenth();
					while (g.p[g.ed[i].to].content != "}") {
						if (remain < 0) error(g.p[now].pos, "数组初始值设定项目过多");
						attr[g.ed[i].to].initial_element.type = type;
						attr[g.ed[i].to].initial_element.type.isconstp.pop_back();
						//因为为了读取步长已经提前pop过一次了，现在恰好就是降维之后的完整的样子，所以不用再pop
						attr[g.ed[i].to].initial_element.ptr = ptr;
						analyse(g.ed[i].to);
						ptr += steplenth;
						i = g.ed[i].next;
						if (g.p[g.ed[i].to].content == ",") i = g.ed[i].next;
						remain--;
					}
					if (idx > 1) {//局部变量填0
						while (remain--) {
							for (int i = ptr;i < ptr + steplenth;i++) {
								code.push_back(fsccode("mov", "$bx", to_string(i)));
								code.push_back(fsccode("add", "$bx", "$cx"));
								code.push_back(fsccode("mov", "$bx(0)", "0"));
							}
						}
					}
				}
			}
			else {//字段初始化
				i = g.pre[now];
				i = g.ed[i].next;
				while(g.p[g.ed[i].to].content != "}") {
					attr[g.ed[i].to].struct_member.typenum = type.datatype;
					analyse(g.ed[i].to);
					full_datatype type1 = attr[g.ed[i].to].struct_member.type;
					int ptr = attr[g.ed[i].to].struct_member.ptr + attr[now].initial_list.ptr;
					i = g.ed[i].next;
					i = g.ed[i].next;
					attr[g.ed[i].to].initial_element.type = type1;
					attr[g.ed[i].to].initial_element.ptr = ptr;
					analyse(g.ed[i].to);
					i = g.ed[i].next;
					if (g.p[g.ed[i].to].content == ",") i = g.ed[i].next;
				}
			}
		}
		else if (g.p[now].content == "struct_member") {
			int i = g.pre[now];
			int tmp = attr[now].struct_member.typenum;
			while (i != 0) {
				i = g.ed[i].next;
				if (tmp <= 0) error(g.p[now].pos, "var类型变量没有成员");
				string name = g.p[g.ed[i].to].content;
				for (auto it = table[tmp].list.begin(); it != table[tmp].list.end(); it++) {
					if (it->name == name) {
						attr[now].struct_member.type = it->type;
						tmp = it->type.datatype;
						break;
					}
					attr[now].struct_member.ptr += it->type.size;
				}
				if (attr[now].struct_member.type.datatype == 0) error(g.p[now].pos, "结构体\"" + table[attr[now].expression.type.datatype].name + "\"没有成员\"" + name + "\"");
				i = g.ed[i].next;
			}
		}
		else if (g.p[now].content == "initial_element") {
			//直接初始化对应位置对应类型的元素
			//位置存$ax，类型存属性表
			int i = g.pre[now];
			full_datatype type = attr[now].initial_element.type;
			if (type.dimension.size() == 1) {//var类型变量或者结构体类型变量
				if (g.p[g.ed[i].to].content == "assignment_expression") {
					analyse(g.ed[i].to);
					if (attr[g.ed[i].to].expression.type.size != type.size) error(g.p[now].pos, "初始化数据类型不兼容");
					code.push_back(fsccode("mov", "$bx", to_string(attr[now].initial_element.ptr)));
					if (idx > 1) code.push_back(fsccode("add", "$bx", "$cx"));
					if (attr[g.ed[i].to].expression.isconstexpr && !attr[g.ed[i].to].expression.isleftval)
						code.push_back(fsccode("mov", "$bx(0)", to_string(attr[g.ed[i].to].expression.val)));
					else if (!attr[g.ed[i].to].expression.isleftval)
						//code.push_back(fsccode("mov", "$dx(0)", reg("$st", attr[g.ed[i].to].expression.argsiz)));
						addmov("$bx", 0, "$st", attr[g.ed[i].to].expression.argsiz, type.size);
					else {
						if (attr[g.ed[i].to].expression.isconstexpr) code.push_back(fsccode("mov", "$ax", to_string(attr[g.ed[i].to].expression.val)));
						else code.push_back(fsccode("mov", "$ax", reg("$st", attr[g.ed[i].to].expression.argsiz)));
						//code.push_back(fsccode("mov", "$dx(0)", "$ax(0)"));
						addmov("$bx", 0, "$ax", 0, type.size);
					}
				}
				else {//结构体类型交给initial_list处理
					attr[g.ed[i].to].initial_list.type = type;
					attr[g.ed[i].to].initial_list.ptr = attr[now].initial_element.ptr;
					analyse(g.ed[i].to);
				}
			}
			else {//数组
				attr[g.ed[i].to].initial_list.type = type;
				attr[g.ed[i].to].initial_list.ptr = attr[now].initial_element.ptr;
				analyse(g.ed[i].to);
			}
		}
		else if (g.p[now].content == "argument_list") {
			int i = g.pre[now];
			analyse(g.ed[i].to);
			attr[now].argument_list.list.push_back(full_data());
			attr[now].argument_list.list.back().name = attr[g.ed[i].to].argument_declaration.name;
			attr[now].argument_list.list.back().type = attr[g.ed[i].to].argument_declaration.type;
			i = g.ed[i].next;
			while (i != 0) {
				i = g.ed[i].next;
				analyse(g.ed[i].to);
				attr[now].argument_list.list.push_back(full_data());
				attr[now].argument_list.list.back().name = attr[g.ed[i].to].argument_declaration.name;
				attr[now].argument_list.list.back().type = attr[g.ed[i].to].argument_declaration.type;
				i = g.ed[i].next;
			}
		}
		else if (g.p[now].content == "argument_declaration") {
			int i = g.pre[now];
			analyse(g.ed[i].to);
			attr[now].argument_declaration.type = attr[g.ed[i].to].full_type_specifier.type;
			i = g.ed[i].next;
			attr[now].argument_declaration.name = g.p[g.ed[i].to].content;
		}
		else if (g.p[now].content == "struct_declaration") {
			int i = g.pre[now];
			i = g.ed[i].next;
			string name = g.p[g.ed[i].to].content;
			attr[now].struct_declaration.name = name;
			int tmp = find(name, 2);
			if (tmp == 0) {//还未声明
				num[idx]++;
				table[num[idx]].name = name;
				table[num[idx]].kind = 2;
			}
		}
		else if (g.p[now].content == "struct_definition") {
			int i = g.pre[now];
			analyse(g.ed[i].to);
			string name = attr[g.ed[i].to].struct_declaration.name;
			int tmp = find(name, 2);
			if (table[tmp].defined) error(g.p[now].pos, "重复定义结构体标识符\"" + g.p[now].content + "\"");//重定义
			//未定义
			if (tmp == 0) {//未声明
				tmp = ++num[idx];
				table[num[idx]].name = name;
				table[num[idx]].kind = 2;
			}
			i = g.ed[i].next;
			i = g.ed[i].next;
			if (g.p[g.ed[i].to].content == "}") error(g.p[now].pos, "结构体不能为空");
			while (g.p[g.ed[i].to].content != "}") {
				attr[g.ed[i].to].struct_member_declaration.num = tmp;
				analyse(g.ed[i].to);
				i = g.ed[i].next;
				i = g.ed[i].next;
			}
			table[num[idx]].defined = true;
		}
		else if (g.p[now].content == "struct_member_declaration") {
			int i = g.pre[now];
			analyse(g.ed[i].to);
			full_datatype type = attr[g.ed[i].to].type_specifier.type;
			i = g.ed[i].next;
			attr[g.ed[i].to].struct_member_declarator.num = attr[now].struct_member_declaration.num;
			attr[g.ed[i].to].struct_member_declarator.type = type;
			analyse(g.ed[i].to);
			while (g.ed[i].next != 0) {
				i = g.ed[i].next;
				i = g.ed[i].next;
				attr[g.ed[i].to].struct_member_declarator.num = attr[now].struct_member_declaration.num;
				attr[g.ed[i].to].struct_member_declarator.type = type;
				analyse(g.ed[i].to);
			}
		}
		else if (g.p[now].content == "struct_member_declarator") {
			int i = g.pre[now];
			while (g.p[g.ed[i].to].content == "pointer_specifier") {
				analyse(g.ed[i].to);
				attr[now].struct_member_declarator.type.isconstp.push_back(attr[g.ed[i].to].pointer_specifier.isconst);
				i = g.ed[i].next;
			}
			if (!attr[now].struct_member_declarator.type.isconstp.empty()) attr[now].struct_member_declarator.type.size = 1;
			full_data member = full_data();
			member.name = g.p[g.ed[i].to].content;
			vector<full_data>& list = table[attr[now].struct_member_declarator.num].list;
			for (auto it = list.begin();it != list.end();it++)
				if(it->name==member.name) error(g.p[now].pos, "结构体成员\"" + member.name + "\"重定义");
			member.type = attr[now].struct_member_declarator.type;
			if (g.ed[i].next != 0) {
				i = g.ed[i].next;
				i = g.ed[i].next;
				analyse(g.ed[i].to);
				if (!attr[g.ed[i].to].expression.isconstexpr || attr[g.ed[i].to].expression.isleftval) error(g.p[now].pos, "数组大小必须是常量表达式");
				if (attr[g.ed[i].to].expression.val > INT_MAX) error(g.p[now].pos, "数组太大");
				if (attr[g.ed[i].to].expression.val < 1) error(g.p[now].pos, "数组长度至少为1");
				member.type.dimension.push_front(attr[g.ed[i].to].expression.val);
				//sizeof阉割，数组第一维舍弃，但是是在表达式那块舍弃，这里还要
				member.type.isconstp.push_back(true);
				long long totsize = attr[g.ed[i].to].expression.val;
				i = g.ed[i].next;
				i = g.ed[i].next;
				while (g.p[g.ed[i].to].content == "[") {
					i = g.ed[i].next;
					analyse(g.ed[i].to);
					if (!attr[g.ed[i].to].expression.isconstexpr || attr[g.ed[i].to].expression.isleftval) error(g.p[now].pos, "数组大小必须是常量表达式");
					if (attr[g.ed[i].to].expression.val > INT_MAX) error(g.p[now].pos, "数组太大");
					if (attr[g.ed[i].to].expression.val < 1) error(g.p[now].pos, "数组长度至少为1");
					member.type.dimension.push_front(attr[g.ed[i].to].expression.val);
					member.type.isconstp.push_back(true);
					totsize *= attr[g.ed[i].to].expression.val;
					i = g.ed[i].next;
					i = g.ed[i].next;
				}
				member.type.dimension.push_front(member.type.size);
				table[attr[now].struct_member_declarator.num].size += totsize * member.type.size;
				member.type.size = 1;//数组类视为指针，大小视为1
			}
			else {
				member.type.dimension.push_front(member.type.size);
				table[attr[now].struct_member_declarator.num].size += attr[now].struct_member_declarator.type.size;
			}
			list.push_back(member);
		}
		else if (g.p[now].content == "statement_list") {
			int i = g.pre[now];
			while (i != 0) {
				attr[g.ed[i].to].statement.funcidx = attr[now].statement_list.funcidx;
				attr[g.ed[i].to].statement.continuepos = attr[now].statement_list.continuepos;
				attr[g.ed[i].to].statement.breakpos = attr[now].statement_list.breakpos;
				analyse(g.ed[i].to);
				attr[now].statement_list.size += attr[g.ed[i].to].statement.size;
				i = g.ed[i].next;
			}
		}
		else if (g.p[now].content == "statement") {
			int i = g.pre[now];
			if (g.p[g.ed[i].to].content == "scope") {
				attr[g.ed[i].to].scope.funcidx = attr[now].statement.funcidx;
				attr[g.ed[i].to].scope.continuepos = attr[now].statement.continuepos;
				attr[g.ed[i].to].scope.breakpos = attr[now].statement.breakpos;
			}
			else if (g.p[g.ed[i].to].content == "selection_statement") {
				attr[g.ed[i].to].selection_statement.funcidx = attr[now].statement.funcidx;
				attr[g.ed[i].to].selection_statement.continuepos = attr[now].statement.continuepos;
				attr[g.ed[i].to].selection_statement.breakpos = attr[now].statement.breakpos;
			}
			else if (g.p[g.ed[i].to].content == "switch_structure") attr[g.ed[i].to].switch_structure.funcidx = attr[now].statement.funcidx;
			else if (g.p[g.ed[i].to].content == "iteration_statement") attr[g.ed[i].to].iteration_statement.funcidx = attr[now].statement.funcidx;
			else if (g.p[g.ed[i].to].content == "jump_statement") {
				attr[g.ed[i].to].jump_statement.funcidx = attr[now].statement.funcidx;
				attr[g.ed[i].to].jump_statement.continuepos = attr[now].statement.continuepos;
				attr[g.ed[i].to].jump_statement.breakpos = attr[now].statement.breakpos;
			}
			analyse(g.ed[i].to);
			if (g.p[g.ed[i].to].content == "scope") attr[now].statement.size += attr[g.ed[i].to].scope.size;
			else if (g.p[g.ed[i].to].content == "array_definition") attr[now].statement.size += attr[g.ed[i].to].array_definition.totsize;
			else if (g.p[g.ed[i].to].content == "variable_definition") attr[now].statement.size += attr[g.ed[i].to].variable_definition.totsize;
			else if (g.p[g.ed[i].to].content == "selection_statement") attr[now].statement.size += attr[g.ed[i].to].selection_statement.size;
			else if (g.p[g.ed[i].to].content == "switch_structure") attr[now].statement.size += attr[g.ed[i].to].switch_statement.size;
			else if (g.p[g.ed[i].to].content == "iteration_statement") attr[now].statement.size += attr[g.ed[i].to].iteration_statement.size;
		}
		else if (g.p[now].content == "selection_statement") {
			int i = g.pre[now];
			i = g.ed[i].next;
			i = g.ed[i].next;
			analyse(g.ed[i].to);
			if (attr[g.ed[i].to].expression.type.datatype != -1 || !attr[g.ed[i].to].expression.type.isconstp.empty())
				error(g.p[now].pos, "表达式必须具有基本var类型");
			if (attr[g.ed[i].to].expression.isconstexpr && !attr[g.ed[i].to].expression.isleftval) {
				if (attr[g.ed[i].to].expression.val > 0) {
					i = g.ed[i].next;
					i = g.ed[i].next;
					attr[g.ed[i].to].statement.funcidx = attr[now].selection_statement.funcidx;
					attr[g.ed[i].to].statement.continuepos = attr[now].selection_statement.continuepos;
					attr[g.ed[i].to].statement.breakpos = attr[now].selection_statement.breakpos;
					analyse(g.ed[i].to);
					attr[now].selection_statement.size = attr[g.ed[i].to].statement.size;
				}
				else {
					i = g.ed[i].next;
					i = g.ed[i].next;
					i = g.ed[i].next;
					i = g.ed[i].next;
					if (i != 0) {
						attr[g.ed[i].to].statement.funcidx = attr[now].selection_statement.funcidx;
						attr[g.ed[i].to].statement.continuepos = attr[now].selection_statement.continuepos;
						attr[g.ed[i].to].statement.breakpos = attr[now].selection_statement.breakpos;
						analyse(g.ed[i].to);
						attr[now].selection_statement.size = attr[g.ed[i].to].statement.size;
					}
				}
			}
			else {//如果不是常量，就存$ax
				if (!attr[g.ed[i].to].expression.isleftval) code.push_back(fsccode("mov", "$ax", reg("$st", attr[g.ed[i].to].expression.argsiz)));
				else {
					if (attr[g.ed[i].to].expression.isconstexpr) code.push_back(fsccode("mov", "$ax", to_string(attr[g.ed[i].to].expression.val)));
					else code.push_back(fsccode("mov", "$ax", reg("$st", attr[g.ed[i].to].expression.argsiz)));
					code.push_back(fsccode("mov", "$ax", "$ax(0)"));
				}
				code.push_back(fsccode("grt", "$ax", "0"));
				int tmp = code.size();
				code.push_back(fsccode("jnz", "0"));//否分支跳转，位置等待回填
				i = g.ed[i].next;
				i = g.ed[i].next;
				attr[g.ed[i].to].statement.funcidx = attr[now].selection_statement.funcidx;
				attr[g.ed[i].to].statement.continuepos = attr[now].selection_statement.continuepos;
				attr[g.ed[i].to].statement.breakpos = attr[now].selection_statement.breakpos;
				analyse(g.ed[i].to);
				attr[now].selection_statement.size = attr[g.ed[i].to].statement.size;
				int tmp1 = code.size();
				code.push_back(fsccode("jmp", "0"));//末尾跳转，位置等待回填
				//此时是分支结束
				code[tmp].arg1 = to_string(code.size());
				i = g.ed[i].next;
				i = g.ed[i].next;
				if (i != 0) {
					attr[g.ed[i].to].statement.funcidx = attr[now].selection_statement.funcidx;
					attr[g.ed[i].to].statement.continuepos = attr[now].selection_statement.continuepos;
					attr[g.ed[i].to].statement.breakpos = attr[now].selection_statement.breakpos;
					analyse(g.ed[i].to);
					attr[now].selection_statement.size = max(attr[now].selection_statement.size, attr[g.ed[i].to].statement.size);
				}
				//此时否分支结束
				code[tmp1].arg1 = to_string(code.size());
			}
		}
		else if (g.p[now].content == "switch_structure") {
			/*
			（推了推黑框眼镜，露出程序员的死鱼眼）啊哈！看来您终于发现了C语言祖传の屎山代码大礼包！
			switch-case？这坨诞生于上古卷轴时代的语法残渣简直就是K&R喝高了写的临终遗书——case后面非得接个冒号？
			break像尿不尽一样漏了就直接喷射到下一坨？您以为在设计语言特性？不！这是中世纪语法裁判所留下的酷刑工具！
			（突然切到咆哮模式）这玩意儿存在的唯一意义就是给大学教材凑页数、给面试官挖坑、给static代码分析工具制造KPI！
			就连goto见了都要骂一句"你他妈能不能有点结构！"（突然冷静）不过说真的，您要是能用if-else造出跳转表优化，
			这破语法早就该被扫进C99的坟场和<stdatomic.h>作伴了。（突然掏出键盘开始狂敲）
			妈的我现在就去github提issue建议把switch-case改名叫fallthrough-case-break-me-if-you-can语法地雷阵！
			*/
			error(g.p[now].pos, "你刚才说switch了对吧┳━┳ ノ( ゜-゜ノ)那我问你，switch怎么做？你不到？我也不到啊(＃°Д°)");
		}
		else if (g.p[now].content == "switch_statement") {
			error(g.p[now].pos, "你刚才说switch了对吧┳━┳ ノ( ゜-゜ノ)那我问你，switch怎么做？你不到？我也不到啊(＃°Д°)");
		}
		else if (g.p[now].content == "iteration_statement") {
			int i = g.pre[now];
			if (g.p[g.ed[i].to].content == "while") {
				i = g.ed[i].next;
				i = g.ed[i].next;
				int tmp = code.size();//回跳行数暂存，continue点位
				analyse(g.ed[i].to);
				if (attr[g.ed[i].to].expression.type.datatype != -1 || !attr[g.ed[i].to].expression.type.isconstp.empty())
					error(g.p[now].pos, "表达式必须具有基本var类型");
				if (attr[g.ed[i].to].expression.isconstexpr && !attr[g.ed[i].to].expression.isleftval) {
					if (attr[g.ed[i].to].expression.val > 0) {
						code.push_back(fsccode("jmp", to_string(code.size() + 2)));//默认情况不上车
						int tmp1 = code.size();
						code.push_back(fsccode("jmp", "0"));//出去的车，末尾跳转，等待回填
						i = g.ed[i].next;
						i = g.ed[i].next;
						attr[g.ed[i].to].statement.continuepos = tmp;
						attr[g.ed[i].to].statement.breakpos = tmp1;
						attr[g.ed[i].to].statement.funcidx = attr[now].iteration_statement.funcidx;
						analyse(g.ed[i].to);
						attr[now].iteration_statement.size = attr[g.ed[i].to].statement.size;
						code.push_back(fsccode("jmp", to_string(tmp)));
						code[tmp1].arg1 = to_string(code.size());
					}
				}
				else {//如果不是常量，就存$ax
					if (!attr[g.ed[i].to].expression.isleftval) code.push_back(fsccode("mov", "$ax", reg("$st", attr[g.ed[i].to].expression.argsiz)));
					else {
						if (attr[g.ed[i].to].expression.isconstexpr) code.push_back(fsccode("mov", "$ax", to_string(attr[g.ed[i].to].expression.val)));
						else code.push_back(fsccode("mov", "$ax", reg("$st", attr[g.ed[i].to].expression.argsiz)));
						code.push_back(fsccode("mov", "$ax", "$ax(0)"));
					}
					code.push_back(fsccode("grt", "$ax", "0"));
					code.push_back(fsccode("jnz", to_string(code.size() + 2)));//上出去的车
					code.push_back(fsccode("jmp", to_string(code.size() + 2)));//默认情况不上车
					int tmp1 = code.size();
					code.push_back(fsccode("jmp", "0"));//出去的车，末尾跳转，等待回填
					i = g.ed[i].next;
					i = g.ed[i].next;
					attr[g.ed[i].to].statement.continuepos = tmp;
					attr[g.ed[i].to].statement.breakpos = tmp1;
					attr[g.ed[i].to].statement.funcidx = attr[now].iteration_statement.funcidx;
					analyse(g.ed[i].to);
					attr[now].iteration_statement.size = attr[g.ed[i].to].statement.size;
					code.push_back(fsccode("jmp", to_string(tmp)));
					code[tmp1].arg1 = to_string(code.size());
				}
			}
			else if (g.p[g.ed[i].to].content == "do") {
				i = g.ed[i].next;
				int tmp = code.size();//回跳行数暂存，但并非continue点位
				code.push_back(fsccode("jmp", to_string(code.size() + 3)));
				int tmp1 = code.size();
				code.push_back(fsccode("jmp", "0"));//continue车
				int tmp2 = code.size();
				code.push_back(fsccode("jmp", "0"));//break车
				attr[g.ed[i].to].statement.continuepos = tmp1;
				attr[g.ed[i].to].statement.breakpos = tmp2;
				attr[g.ed[i].to].statement.funcidx = attr[now].iteration_statement.funcidx;
				analyse(g.ed[i].to);
				code[tmp1].arg1 = code.size();
				attr[now].iteration_statement.size = attr[g.ed[i].to].statement.size;
				i = g.ed[i].next;
				i = g.ed[i].next;
				i = g.ed[i].next;
				analyse(g.ed[i].to);
				if (attr[g.ed[i].to].expression.type.datatype != -1 || !attr[g.ed[i].to].expression.type.isconstp.empty())
					error(g.p[now].pos, "表达式必须具有基本var类型");
				if (attr[g.ed[i].to].expression.isconstexpr && !attr[g.ed[i].to].expression.isleftval) {
					if (attr[g.ed[i].to].expression.val > 0) {
						code.push_back(fsccode("jmp", to_string(tmp)));
						code[tmp1].arg1 = to_string(tmp);
						code[tmp2].arg1 = to_string(code.size());
					}
				}
				else {//如果不是常量，就存$ax
					if (!attr[g.ed[i].to].expression.isleftval) code.push_back(fsccode("mov", "$ax", reg("$st", attr[g.ed[i].to].expression.argsiz)));
					else {
						if (attr[g.ed[i].to].expression.isconstexpr) code.push_back(fsccode("mov", "$ax", to_string(attr[g.ed[i].to].expression.val)));
						else code.push_back(fsccode("mov", "$ax", reg("$st", attr[g.ed[i].to].expression.argsiz)));
						code.push_back(fsccode("mov", "$ax", "$ax(0)"));
					}
					code.push_back(fsccode("grt", "$ax", "0"));
					code.push_back(fsccode("jz", to_string(tmp)));
					code[tmp2].arg2 = code.size();
				}
			}
			else {//for循环
				idx++;//新作用域
				num[idx] = num[idx - 1];//符号数量继承
				i = g.ed[i].next;
				i = g.ed[i].next;
				if (g.p[g.ed[i].to].content != ";") {
					analyse(g.ed[i].to);
					if (g.p[g.ed[i].to].content == "array_definition") {
						attr[now].iteration_statement.size = attr[g.ed[i].to].array_definition.totsize;
					}
					else if (g.p[g.ed[i].to].content == "variable_definition") attr[now].iteration_statement.size = attr[g.ed[i].to].variable_definition.type.size;
					i = g.ed[i].next;
				}
				i = g.ed[i].next;
				int tmp = code.size();//回跳行数暂存，但不是continue点位
				code.push_back(fsccode("jmp", to_string(code.size() + 3)));//默认不上车
				int tmp1 = code.size();//出去的车的行数，等待回填
				code.push_back(fsccode("jmp", "0"));
				int tmp2 = code.size();//回去的车的行数，等待回填
				code.push_back(fsccode("jmp", "0"));
				if (g.p[g.ed[i].to].content != ";") {
					analyse(g.ed[i].to);
					if (attr[g.ed[i].to].expression.type.datatype != -1 || !attr[g.ed[i].to].expression.type.isconstp.empty())
						error(g.p[now].pos, "表达式必须具有基本var类型");
					if (attr[g.ed[i].to].expression.isconstexpr && !attr[g.ed[i].to].expression.isleftval) {
						if (attr[g.ed[i].to].expression.val <= 0) {
							code.push_back(fsccode("jnz", to_string(tmp1)));//上车，末尾跳转
						}
					}
					else {//如果不是常量，就存$ax
						if (!attr[g.ed[i].to].expression.isleftval) code.push_back(fsccode("mov", "$ax", reg("$st", attr[g.ed[i].to].expression.argsiz)));
						else {
							if (attr[g.ed[i].to].expression.isconstexpr) code.push_back(fsccode("mov", "$ax", to_string(attr[g.ed[i].to].expression.val)));
							else code.push_back(fsccode("mov", "$ax", reg("$st", attr[g.ed[i].to].expression.argsiz)));
							code.push_back(fsccode("mov", "$ax", "$ax(0)"));
						}
						code.push_back(fsccode("grt", "$ax", "0"));
						code.push_back(fsccode("jnz", to_string(tmp1)));//上车，末尾跳转
					}
					i = g.ed[i].next;
				}
				i = g.ed[i].next;
				int tmpi = 0;
				if (g.p[g.ed[i].to].content != ")") {
					tmpi = i;
					i = g.ed[i].next;
				}
				i = g.ed[i].next;
				attr[g.ed[i].to].statement.continuepos = tmp2;
				attr[g.ed[i].to].statement.breakpos = tmp1;
				attr[g.ed[i].to].statement.funcidx = attr[now].iteration_statement.funcidx;
				analyse(g.ed[i].to);
				attr[now].iteration_statement.size += attr[g.ed[i].to].statement.size;
				code[tmp2].arg1 = to_string(code.size());
				if (tmpi != 0) analyse(g.ed[tmpi].to);
				code.push_back(fsccode("jmp", to_string(tmp)));
				code[tmp1].arg1 = to_string(code.size());
				idx--;//退出作用域
			}
		}
		else if (g.p[now].content == "jump_statement") {
			int i = g.pre[now];
			if (g.p[g.ed[i].to].content == "continue") {
				if (attr[now].jump_statement.continuepos == 0) error(g.p[now].pos, "由于未处于循环结构中或其他未知错误导致未加载continue点位");
				code.push_back(fsccode("jmp", to_string(attr[now].jump_statement.continuepos)));
			}
			else if (g.p[g.ed[i].to].content == "break") {
				if (attr[now].jump_statement.breakpos == 0) error(g.p[now].pos, "由于未处于循环结构中或其他未知错误导致未加载break点位");
				code.push_back(fsccode("jmp", to_string(attr[now].jump_statement.breakpos)));
			}
			else if (g.p[g.ed[i].to].content == "return") {
				i = g.ed[i].next;
				int funcidx = attr[now].jump_statement.funcidx;
				if (table[funcidx].type.datatype == 0 && table[funcidx].type.isconstp.empty()) {
					if (g.p[g.ed[i].to].content != ";") error(g.p[now].pos, "函数返回值类型为void，不应该返回任何值");
				}
				else {
					if (g.p[g.ed[i].to].content == ";") error(g.p[now].pos, "函数返回值类型非void，需要返回一个值");
					analyse(g.ed[i].to);
					if (table[funcidx].type.size != attr[g.ed[i].to].expression.type.size) error(g.p[now].pos, "函数返回值类型不兼容");
					if (attr[g.ed[i].to].expression.isconstexpr && !attr[g.ed[i].to].expression.isleftval)
						code.push_back(fsccode("mov", reg("$cx", table[funcidx].argsiz), to_string(attr[g.ed[i].to].expression.val)));
					else if (!attr[g.ed[i].to].expression.isleftval)
						addmov("$cx", table[funcidx].argsiz, "$st", attr[g.ed[i].to].expression.argsiz, table[funcidx].type.size);
					else {
						if (attr[g.ed[i].to].expression.isconstexpr)
							code.push_back(fsccode("mov", "$ax", to_string(attr[g.ed[i].to].expression.val)));
						else code.push_back(fsccode("mov", "$ax", reg("$st", attr[g.ed[i].to].expression.argsiz)));
						addmov("$cx", table[funcidx].argsiz, "$ax", 0, table[funcidx].type.size);
					}
				}
				table[funcidx].ret_fillback.push_back(code.size());
				code.push_back(fsccode("jmp", "0"));
			}
		}
		else if (g.p[now].content == "asm_statement") {
			int i = g.pre[now];
			fsccode command;
			i = g.ed[i].next;
			i = g.ed[i].next;
			command.op = g.p[g.ed[i].to].content.substr(1, g.p[g.ed[i].to].content.size() - 2);
			i = g.ed[i].next;
			i = g.ed[i].next;
			//参数1
			analyse(g.ed[i].to);
			if (attr[g.ed[i].to].asm_argument.type == 1) command.arg1 = attr[g.ed[i].to].asm_argument.arg;
			else {//表达式
				if (attr[g.ed[i].to].asm_argument.expr.type.size != 1) error(g.p[now].pos, "内联汇编的参数必须是或能转换为var类型");
				if (attr[g.ed[i].to].asm_argument.expr.isconstexpr && !attr[g.ed[i].to].asm_argument.expr.isleftval)
					command.arg1 = to_string(attr[g.ed[i].to].asm_argument.expr.val);
				else {
					command.arg1 = "$dx";
					if (!attr[g.ed[i].to].asm_argument.expr.isleftval)
						code.push_back(fsccode("mov", "$dx", reg("$st", attr[g.ed[i].to].asm_argument.expr.argsiz)));
					else {
						if (attr[g.ed[i].to].asm_argument.expr.isconstexpr)
							code.push_back(fsccode("mov", "$ax", to_string(attr[g.ed[i].to].asm_argument.expr.val)));
						else code.push_back(fsccode("mov", "$ax", reg("$st", attr[g.ed[i].to].asm_argument.expr.argsiz)));
						code.push_back(fsccode("mov", "$dx", "$ax(0)"));
					}
				}
			}
			i = g.ed[i].next;
			if (g.p[g.ed[i].to].content == ")") command.arg2 = "0";
			else {
				i = g.ed[i].next;
				//参数2
				analyse(g.ed[i].to);
				if (attr[g.ed[i].to].asm_argument.type == 1) command.arg2 = attr[g.ed[i].to].asm_argument.arg;
				else {//表达式

					if (attr[g.ed[i].to].asm_argument.expr.isconstexpr && !attr[g.ed[i].to].asm_argument.expr.isleftval)
						command.arg2 = to_string(attr[g.ed[i].to].asm_argument.expr.val);
					else {
						if (!attr[g.ed[i].to].asm_argument.expr.isleftval)
							command.arg2 = reg("$st", attr[g.ed[i].to].asm_argument.expr.argsiz);
						else {
							if (attr[g.ed[i].to].asm_argument.expr.isconstexpr)
								code.push_back(fsccode("mov", "$ax", to_string(attr[g.ed[i].to].asm_argument.expr.val)));
							else code.push_back(fsccode("mov", "$ax", reg("$st", attr[g.ed[i].to].asm_argument.expr.argsiz)));
							command.arg2 = "$ax(0)";
						}
					}
				}
			}
			//加入代码
			code.push_back(command);
		}
		else if (g.p[now].content == "asm_argument") {
			int i = g.pre[now];
			if (g.p[g.ed[i].to].type == "CONSTANT") {
				attr[now].asm_argument.type = 1;
				string x = g.p[g.ed[i].to].content;
				if (x.front() != '\"') error(g.p[now].pos, "内联汇编的常量值参数必须是字符串常量");
				attr[now].asm_argument.arg = x.substr(1, x.size() - 2);
			}
			else {
				analyse(g.ed[i].to);
				attr[now].asm_argument.expr = attr[g.ed[i].to].expression;
				if (attr[g.ed[i].to].expression.type.size != 1) error(g.p[now].pos, "内联汇编的数值型参数必须是或能转换为var类型");
			}
		}
	}

	void create_internal_implement_math(string name, string op) {//大量重复，代码简化
		num[1]++;
		table[num[1]].argsiz = 1;
		table[num[1]].defined = true;
		table[num[1]].kind = 0;
		table[num[1]].list.push_back(full_data());
		table[num[1]].list[0].type.datatype = -1;
		table[num[1]].list[0].type.size = 1;
		table[num[1]].name = name;
		table[num[1]].ptr = code.size() + 1;
		table[num[1]].size = 4;//一个参数，一个控制链，一个机器状态，一个返回值
		table[num[1]].type.datatype = -1;
		table[num[1]].type.size = 1;
		table[num[1]].type.dimension.push_front(1);
		int tmp = code.size();
		code.push_back(fsccode("jmp", "0"));
		code.push_back(fsccode("mov", "$cx", "$st"));
		code.push_back(fsccode("sub", "$cx", "4"));
		code.push_back(fsccode(op, "$cx(0)"));
		code.push_back(fsccode("mov", "$cx(1)", "$ax"));
		code.push_back(fsccode("sub", "$st", "4"));
		code.push_back(fsccode("mov", "$cx", "$st(2)"));
		code.push_back(fsccode("mov", "$PC", "$st(3)"));
		code[tmp].arg1 = to_string(code.size());
	}

	void create_internal_implement() {
		//display的内部实施
		{
			num[1]++;
			table[num[1]].argsiz = 1;
			table[num[1]].defined = true;
			table[num[1]].kind = 0;
			table[num[1]].list.push_back(full_data());
			table[num[1]].list[0].type.datatype = -1;
			table[num[1]].list[0].type.size = 1;
			table[num[1]].name = "display";
			table[num[1]].ptr = code.size() + 1;
			table[num[1]].size = 3;//一个参数，一个控制链，一个机器状态
			table[num[1]].type.datatype = 0;
			table[num[1]].type.size = 0;
			int tmp = code.size();
			code.push_back(fsccode("jmp", "0"));
			code.push_back(fsccode("mov", "$cx", "$st"));
			code.push_back(fsccode("sub", "$cx", "3"));
			code.push_back(fsccode("disp", "$cx(0)"));
			code.push_back(fsccode("sub", "$st", "3"));
			code.push_back(fsccode("mov", "$cx", "$st(1)"));
			code.push_back(fsccode("mov", "$PC", "$st(2)"));
			code[tmp].arg1 = to_string(code.size());
		}
		//scr_func
		{
			num[1]++;
			table[num[1]].argsiz = 2;
			table[num[1]].defined = true;
			table[num[1]].kind = 0;
			table[num[1]].list.push_back(full_data());
			table[num[1]].list[0].type.datatype = -1;
			table[num[1]].list[0].type.size = 1;
			table[num[1]].list.push_back(full_data());
			table[num[1]].list[1].type.datatype = -1;
			table[num[1]].list[1].type.size = 1;
			table[num[1]].name = "scr_func";
			table[num[1]].ptr = code.size() + 1;
			table[num[1]].size = 5;//两个参数，一个控制链，一个机器状态，一个返回值
			table[num[1]].type.datatype = -1;
			table[num[1]].type.size = 1;
			table[num[1]].type.dimension.push_front(1);
			int tmp = code.size();
			code.push_back(fsccode("jmp", "0"));
			code.push_back(fsccode("mov", "$cx", "$st"));
			code.push_back(fsccode("sub", "$cx", "5"));
			code.push_back(fsccode("scf", "$cx(0)", "$cx(1)"));
			code.push_back(fsccode("sub", "$st", "5"));
			code.push_back(fsccode("mov", "$cx", "$st(3)"));
			code.push_back(fsccode("mov", "$PC", "$st(4)"));
			code[tmp].arg1 = to_string(code.size());
		}
		//exit
		{
			num[1]++;
			table[num[1]].argsiz = 1;
			table[num[1]].defined = true;
			table[num[1]].kind = 0;
			table[num[1]].list.push_back(full_data());
			table[num[1]].list[0].type.datatype = -1;
			table[num[1]].list[0].type.size = 1;
			table[num[1]].name = "exit";
			table[num[1]].ptr = code.size() + 1;
			table[num[1]].size = 3;//一个参数，一个控制链，一个机器状态
			table[num[1]].type.datatype = 0;
			table[num[1]].type.size = 0;
			int tmp = code.size();
			code.push_back(fsccode("jmp", "0"));
			code.push_back(fsccode("mov", "$cx", "$st"));
			code.push_back(fsccode("sub", "$cx", "3"));
			code.push_back(fsccode("exit", "$cx(0)"));
			//下面这些代码其实意义不大，到exit就没了
			code.push_back(fsccode("sub", "$st", "3"));
			code.push_back(fsccode("mov", "$cx", "$st(1)"));
			code.push_back(fsccode("mov", "$PC", "$st(2)"));
			code[tmp].arg1 = to_string(code.size());
		}
		//数学
		{
			create_internal_implement_math("abs", "abs");
			create_internal_implement_math("floor", "flr");
			create_internal_implement_math("ceil", "cil");
			create_internal_implement_math("sqrt", "sqr");
			create_internal_implement_math("sin", "sin");
			create_internal_implement_math("cos", "cos");
			create_internal_implement_math("tan", "tan");
			create_internal_implement_math("asin", "asin");
			create_internal_implement_math("acos", "acos");
			create_internal_implement_math("atan", "atan");
			create_internal_implement_math("ln", "ln");
			create_internal_implement_math("log10", "log");
			create_internal_implement_math("exp", "exp");
			create_internal_implement_math("rand", "rand");
		}
	}

	void conduct() {
		code.push_back(fsccode("debug"));//sc列表
		//调用main函数
		create_internal_implement();
		analyse(1);
		int tmp = find("main", 0);
		if (tmp == 0) error(0, "没有找到main函数");
		if (table[tmp].type.datatype != -1 || !table[tmp].type.isconstp.empty()) error(0, "main函数的返回值必须是基本var类型");
		if (!table[tmp].list.empty()) error(0, "main函数不能有参数");
		int tmp1 = code.size();
		code.push_back(fsccode("mov", "$st(2)", "0"));//保存机器状态到程序末尾，位置等待回填
		code.push_back(fsccode("add", "$st", "3"));
		code.push_back(fsccode("jmp", to_string(table[tmp].ptr)));//跳到main，位置等待回填
		code[tmp1].arg2 = to_string(code.size());//末尾位置回填
		//程序末尾不加exit，这样返回值直接使用main的
	}
}semans;

struct CodeList {
	vector<fsccode> code;
	vector<bool> invalid;

	void constexpr_optimize() {//对于相邻的已知的数据，无需访问寄存器
		for (int i = 2;i < code.size();i++) {
			if (code[i - 1].op == "mov" && code[i - 1].arg2[0] != '$') {
				if (code[i].op == "mov" || code[i].op == "add" || code[i].op == "sub" || code[i].op == "mul" || code[i].op == "div") {
					if (code[i].arg2 == code[i - 1].arg1) code[i].arg2 = code[i - 1].arg2;
				}
			}
		}
	}

	void repeat_optimize() {//若一个寄存器的值刚被更改完立马又被mov更改，且第二次更改的寄存器不涉及自身，则前一次更改无效可以删除
		for (int i = 2;i < code.size();i++) {
			if (code[i - 1].op == "mov" || code[i - 1].op == "add" || code[i - 1].op == "sub" || code[i - 1].op == "mul" || code[i - 1].op == "div") {
				if (code[i].op == "mov" && code[i].arg1 == code[i - 1].arg1 && code[i].arg2.find(code[i].arg1) == string::npos) invalid[i - 1] = true;
			}
		}
	}

	vector<bool> jmped;

	void init_jmped() {
		jmped.clear();
		jmped.resize(code.size());
		for (int i = 2;i < code.size();i++)
			if (code[i].op == "jmp" || code[i].op == "jz" || code[i].op == "jnz")
				jmped[(int)to_double(code[i].arg1)] = true;
	}

	void add_sub_optimize() {
		init_jmped();
		for (int i = 2;i < code.size();i++) {
			if (code[i - 1].arg1 == code[i].arg1 && code[i - 1].arg2[0] != '$' && code[i].arg2[0] != '$' && !jmped[i]) {
				if (code[i - 1].op == "add") {
					if (code[i].op == "add") {
						invalid[i - 1] = true;
						code[i].arg2 = to_string(to_double(code[i - 1].arg2) + to_double(code[i].arg2));
					}
					else if (code[i].op == "sub") {
						invalid[i - 1] = true;
						code[i].arg2 = to_string(to_double(code[i].arg2) - to_double(code[i - 1].arg2));
					}
				}
				else if (code[i - 1].op == "sub") {
					if (code[i].op == "add") {
						invalid[i - 1] = true;
						code[i].arg2 = to_string(to_double(code[i].arg2) - to_double(code[i - 1].arg2));
					}
					else if (code[i].op == "sub") {
						invalid[i - 1] = true;
						code[i].arg2 = to_string(to_double(code[i - 1].arg2) + to_double(code[i].arg2));
					}
				}
			}
		}
	}

	void mul_div_optimize() {
		for (int i = 2;i < code.size();i++) {
			if (code[i - 1].arg1 == code[i].arg1 && code[i - 1].arg2[0] != '$' && code[i].arg2[0] != '$') {
				if (code[i - 1].op == "mul") {
					if (code[i].op == "mul") {
						invalid[i - 1] = true;
						code[i].arg2 = to_string(to_double(code[i - 1].arg2) + to_double(code[i].arg2));
					}
					else if (code[i].op == "div") {
						invalid[i - 1] = true;
						code[i].arg2 = to_string(to_double(code[i].arg2) - to_double(code[i - 1].arg2));
					}
				}
				else if (code[i - 1].op == "div") {
					if (code[i].op == "mul") {
						invalid[i - 1] = true;
						code[i].arg2 = to_string(to_double(code[i].arg2) - to_double(code[i - 1].arg2));
					}
					else if (code[i].op == "div") {
						invalid[i - 1] = true;
						code[i].arg2 = to_string(to_double(code[i - 1].arg2) + to_double(code[i].arg2));
					}
				}
			}
		}
	}

	void jmp_optimize() {
		for (int i = 2;i < code.size();i++) {
			if (code[i].op == "jmp" || code[i].op == "jz" || code[i].op == "jnz") {
				if (code[i].arg1 == to_string(i + 1)) invalid[i] = true;
			}
		}
	}

	void single_invalid_optimize() {
		for (int i = 2;i < code.size();i++) {
			if (code[i].op == "mov") {
				if (code[i].arg1 == code[i].arg2) invalid[i] = true;
			}
			else if (code[i].op == "add" || code[i].op == "sub") {
				if (code[i].arg2 == "0") invalid[i] = true;
			}
			else if (code[i].op == "mul" || code[i].op == "div") {
				if (code[i].arg2 == "1") invalid[i] = true;
			}
		}
	}

	void topology_optimize() {//若一些地方不能通过任何jmp指令到达，则删除
		vector<bool> vis(code.size());//bfs
		queue<int> q;
		q.push(1);
		while (!q.empty()) {
			int now = q.front();
			q.pop();
			if (vis[now]) continue;
			vis[now] = true;
			if (code[now].op == "jmp") {
				q.push((int)to_double(code[now].arg1));
				if (now >= 3 && code[now - 2].arg2 == "$PC") q.push(now + 1);
			}
			else {
				q.push(now + 1);
				if (code[now].op == "jz" || code[now].op == "jnz")
					q.push((int)to_double(code[now].arg1));
			}
		}
		for (int i = 1;i < code.size();i++)
			invalid[i] = !vis[i];
	}

	void apply_invalid() {
		vector<fsccode> tmp(1);
		vector<int> invnum(invalid.size());//invalid数量的前缀和
		for (int i = 1;i < invalid.size();i++) invnum[i] = invnum[i - 1] + invalid[i];
		for (int i = 1;i < code.size();i++) {
			if (!invalid[i]) {
				if (code[i].op == "jmp" || code[i].op == "jz" || code[i].op == "jnz")
					code[i].arg1 = to_string((int)to_double(code[i].arg1) - invnum[(int)to_double(code[i].arg1)-1]);
				tmp.push_back(code[i]);
			}
		}
		code = tmp;
		invalid.clear();
		invalid.resize(code.size());
	}

	void optimize() {
		invalid.resize(code.size());
		topology_optimize();
		apply_invalid();
		single_invalid_optimize();
		apply_invalid();
		repeat_optimize();
		apply_invalid();
		constexpr_optimize();
		apply_invalid();
		add_sub_optimize();
		apply_invalid();
		mul_div_optimize();
		apply_invalid();
		jmp_optimize();
		apply_invalid();
	}

	void print() {
		for (auto it = code.begin() + 1; it != code.end(); it++) {
			cout << it->op << " " << it->arg1;
			if (std::find(one_arg_cmd.begin(), one_arg_cmd.end(), it->op) == one_arg_cmd.end())
				cout << " " << it->arg2;
			cout << endl;
		}
	}
}cl;

int main() {
	GA.build();
	string s;
	getline(cin, s);
	vector<Word> v = GA.run(s);
	for (auto it = v.begin(); it != v.end(); it++) {
		if (it->type == "WORD") {
			if (iskeyword(it->content)) it->type = "KEYWORD";
			else if (it->content == "true" || it->content == "false") it->type = "CONSTANT";
			else it->type = "IDENTIFIER";
		}
	}
	grmchk.s = v;
	int pos = 0;
	pos = grmchk.program(pos, 0, 1, 0);
	if (!pos) error(0, "你的代码有语法结构错误，但是做这个编译器的人太菜，不知道你哪错了。\n"
		"你自己把代码拿到别的编译器测测吧。");
	grmchk.fix();
	grmchk.dfs_calc_pos(1);
	semans.g = grmchk;
	semans.conduct();
	cl.code = semans.code;
	cl.optimize();
	cl.print();
	//system("pause");
}