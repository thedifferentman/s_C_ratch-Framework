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
	* 0.正常状态
	* 1.处于字符串内
	* 2.处于字符串内且等待转义
	* 3.处于字符内
	* 4.处于字符内且等待转义
	* 5.斜杠，即将注释
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
	* 0.正常状态
	* 1.处于字符串内
	* 2.处于字符串内且等待转义
	* 3.处于字符内
	* 4.处于字符内且等待转义
	* 5.有一个空格，即将连续空格
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
	* 0.正常状态
	* 1.处于字符串内
	* 2.处于字符串内且等待转义
	* 3.处于字符内
	* 4.处于字符内且等待转义
	* 5.斜杠，即将注释
	* 6.斜杠后面有星号，进入注释
	* 7.星号，即将退出注释
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