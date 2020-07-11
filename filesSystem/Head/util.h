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

	//�����ļ��ӵ�ǰλ�õ������ض��ַ��Ļ��߽������ַ�����
	//int read_file(ifstream &fin);

	//�����ļ���
	bool createDir(string dirPath);

	//�ж��ļ��Ƿ�Ϊ��
	bool is_emptyFile(string filePath);

	struct tm get_cur_time();

	vector<string> splitString( const string& str, const string &pattern);

	CMD commandParse(vector<string> &strVec, string command);

	void auth_disp(char* access);

	//void fcb_disp(struct fcb *file, int access);

	bool fileParse(string fileName, vector<string> &strVec);

};

#endif