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

public:

	blockD();
	//��ʼ������Ϣ
	void initBlock(char *blockMap);

	//��ȡ����Ϣ
	void readBlock(char *blockMap);

	//�����û�����Ϣ
	vector<user> read_user(string userPath);

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
};

#endif