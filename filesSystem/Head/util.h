#pragma once
#ifndef __UTIL_HEAD__
#define __UTIL_HEAD__
#include"define.h"
#include<string>
#include<fstream>
#include<vector>
#include<time.h>
using namespace std;
class util {

public:
	//util();

	//返回文件从当前位置到遇到特定字符的或者结束的字符数量
	//int read_file(ifstream &fin);

	//创建文件夹
	bool createDir(string dirPath);

	//判断文件是否为空
	bool is_emptyFile(string filePath);

	struct tm get_cur_time();

	vector<string> splitString( const string& str, const string &pattern);

	CMD commandParse(vector<string> &strVec, string command);

	void auth_disp(char* access);

	//void fcb_disp(struct fcb *file, int access);

	bool fileParse(string fileName, vector<string> &strVec);

};

#endif