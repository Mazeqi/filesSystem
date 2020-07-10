#include"../Head/define.h"
#include"../Head/util.h"
#include<fstream>
#include<string>
#include<iostream>
#include<io.h>
#include<direct.h>
#include<time.h>
using namespace std;

//
//int util::read_file(ifstream &fin) {
//	char ch;
//	int count = 0;
//
//	while (!fin.eof()) {
//		//读取一个字符
//		ch = fin.get();
//
//		//10是换行 32是空格 44是逗号
//		if (ch == 10 || ch == 32 || ch == 44  || ch == ':') {
//			break;
//		}
//		count++;
//	}
//
//	fin.seekg(-count-1, ios::cur);
//	return count;
//}



bool util::createDir(string dirPath) {
	if (_access(dirPath.c_str(), 0) == -1) {
		if (_mkdir(dirPath.c_str()) == -1) {
			//cout << "Create dir " << dirPath << "error ! \n";
			return false;
		}
		
	}
	return true;
}

bool util::is_emptyFile(string filePath) {
	char ch;
	ifstream fin(filePath, ios::in);


	if (!fin.is_open()) {
		return true;
	}

	ch = fin.get();

	if (ch == EOF) {
		return true;
	}
	else {
		return false;
	}
}

struct tm util:: get_cur_time() {
	time_t t = time(NULL);
	struct tm* tm;
	tm = localtime(&t);
	tm->tm_year += 1900;
	tm->tm_mon++;
	return *tm;
}