#include <iostream>
#include <sstream>
#include <cstring>
#include <cstdio>
#include <vector>
#include <set>
#include <Windows.h>
using namespace std;

bool isInclude(const string& str) {
	return str.find("#include") != string::npos;
}

set<string> included;

void printFile(const char* fileName) {
	string codeInput;
	freopen(fileName, "r", stdin);
	cin.clear();
	//读入文件
	vector<string> file;
	while (codeInput != "END" && !cin.eof()) {
		getline(cin, codeInput);
		file.push_back(codeInput);
	}
	//输出文件
	for (int i = 0;i < file.size();i++) {
		codeInput = file[i];
		if (!isInclude(codeInput)) cout << codeInput << endl;
		else {
			string name;
			int idx = 0;
			while (codeInput[idx] != '\"') idx++;
			idx++;
			while (codeInput[idx] != '\"') {
				name.push_back(codeInput[idx]);
				idx++;
			}
			if (included.find(name) == included.end()) {
				printFile(name.c_str());
				included.insert(name);
			}
		}
	}
}

int main() {
	freopen("unfoldedcode.c", "w", stdout);
	//把头文件展开打印
	included.insert("head.h");
	printFile("source.c");
	//输入代码转换器进行转换
	system("代码转换器.exe < unfoldedcode.c > transformedcode.c");
	system("C语言编译器.exe < transformedcode.c > fsccode.txt");
}