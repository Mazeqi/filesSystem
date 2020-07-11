#pragma once
#ifndef __BLOCKDAO_HEAD__
#define __BLOCKDAO_HEAD__
#include"define.h"
#include<string>
#include<iostream>
#include<vector>
#include<map>
using namespace std;
class blockD{
private:

	//�洢��λ��
    string config;
	string blockFile;
	string userFile;
	string indexFile;
	string groupFile;
	string fcbFile;
	string fcbMapFile;

public:

	blockD();
	//��ʼ������Ϣ
	void initBlockMap(char *blockMap);

	//��ȡ����Ϣ
	void readBlockMap(char *blockMap);

	//�����û�����Ϣ
	vector<user> read_user(string userPath);

	string getFcbMapFile();

	string getBlockFile();

	string getUserFile();

	string getIndexFile();

	string getGroupFile();

	string getConfig();

	string getFcbFile();

	//��ȡ�û�����Ϣ
	vector<group> readGroup(string groupPath);

	map<int, fcb> readFcb(string fcbPath);

	void writeFcb(string fcbPath, map<int, fcb> fcbMap);

	void writeBlockMap(char* blcokMap);

	int writeReal(int num, char* str);

	void writeBlock(char* blockMap, struct fcb *file, char *contents);

	void writeFcbMap(string fcbMapPath, map<int, vector<int>> fcbMap);

	map<int, vector<int>> readFcbMap(string fcbMapPath);
};

#endif