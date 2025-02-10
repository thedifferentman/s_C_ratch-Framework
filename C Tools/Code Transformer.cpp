#include <iostream>
#include <cstring>
#include <sstream>
#include <Windows.h>
using namespace std;

string codeInput;
string code;

bool isMacro(const string& str) {
	return str[0] == '#';
}

string deleteComment1(const string& str) {
	/*
	* 0.����״̬
	* 1.�����ַ�����
	* 2.�����ַ������ҵȴ�ת��
	* 3.�����ַ���
	* 4.�����ַ����ҵȴ�ת��
	* 5.б�ܣ�����ע��
	*/
	string res;
	int state = 0;
	for (int i = 0;i < str.size();i++) {
		switch (state) {
		case 0:
			if (str[i] == '\"') state = 1;
			if (str[i] == '\'') state = 3;
			if (str[i] == '/') state = 5;
			res.push_back(str[i]);
			break;
		case 1:
			if (str[i] == '\\') state = 2;
			if (str[i] == '\"') state = 0;
			res.push_back(str[i]);
			break;
		case 2:
			state = 1;
			res.push_back(str[i]);
			break;
		case 3:
			if (str[i] == '\\') state = 4;
			if (str[i] == '\'') state = 0;
			res.push_back(str[i]);
			break;
		case 4:
			state = 3;
			res.push_back(str[i]);
			break;
		case 5:
			if (str[i] == '/') {
				res.pop_back();
				return res;
			}
			state = 0;
			if (str[i] == '\"') state = 1;
			if (str[i] == '\'') state = 3;
			res.push_back(str[i]);
			break;
		}
	}
	return res;
}

string deleteSpace(const string& str) {
	/*
	* 0.����״̬
	* 1.�����ַ�����
	* 2.�����ַ������ҵȴ�ת��
	* 3.�����ַ���
	* 4.�����ַ����ҵȴ�ת��
	* 5.��һ���ո񣬼��������ո�
	*/
	string res;
	int state = 0;
	for (int i = 0;i < str.size();i++) {
		if (str[i] == '\t') continue;
		switch (state) {
		case 0:
			if (str[i] == '\"') state = 1;
			if (str[i] == '\'') state = 3;
			if (str[i] == ' ') state = 5;
			res.push_back(str[i]);
			break;
		case 1:
			if (str[i] == '\\') state = 2;
			if (str[i] == '\"') state = 0;
			res.push_back(str[i]);
			break;
		case 2:
			state = 1;
			res.push_back(str[i]);
			break;
		case 3:
			if (str[i] == '\\') state = 4;
			if (str[i] == '\'') state = 0;
			res.push_back(str[i]);
			break;
		case 4:
			state = 3;
			res.push_back(str[i]);
			break;
		case 5:
			if (str[i] == ' ') break;
			state = 0;
			res.push_back(str[i]);
		}
	}
	return res;
}

string deleteComment2(const string& str) {
	/*
	* 0.����״̬
	* 1.�����ַ�����
	* 2.�����ַ������ҵȴ�ת��
	* 3.�����ַ���
	* 4.�����ַ����ҵȴ�ת��
	* 5.б�ܣ�����ע��
	* 6.б�ܺ������Ǻţ�����ע��
	* 7.�Ǻţ������˳�ע��
	*/
	string res;
	int state = 0;
	for (int i = 0;i < str.size();i++) {
		switch (state) {
		case 0:
			if (str[i] == '\"') state = 1;
			if (str[i] == '\'') state = 3;
			if (str[i] == '/') state = 5;
			res.push_back(str[i]);
			break;
		case 1:
			if (str[i] == '\\') state = 2;
			if (str[i] == '\"') state = 0;
			res.push_back(str[i]);
			break;
		case 2:
			state = 1;
			res.push_back(str[i]);
			break;
		case 3:
			if (str[i] == '\\') state = 4;
			if (str[i] == '\'') state = 0;
			res.push_back(str[i]);
			break;
		case 4:
			state = 3;
			res.push_back(str[i]);
			break;
		case 5:
			if (str[i] == '*') {
				res.pop_back();
				state = 6;
			}
			else {
				state = 0;
				res.push_back(str[i]);
			}
		case 6:
			if (str[i] == '*') state = 7;
		case 7:
			if (str[i] == '/') state = 0;
			else state = 6;
		}
	}
	return res;
}

int main() {
	while (codeInput != "END" && !cin.eof()) {
		getline(cin, codeInput);
		if (isMacro(codeInput)) continue;
		codeInput = deleteComment1(codeInput);
		codeInput = deleteSpace(codeInput);
		code += codeInput;
	}
	code = deleteComment2(code);
	cout << code;
}