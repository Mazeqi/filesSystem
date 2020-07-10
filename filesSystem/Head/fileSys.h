#pragma once
#ifndef __FILESYS_HEAD__
#define __FILESYS_HEAD__
#include"define.h"
#include"util.h"
#include"blockDao.h"
#include<string>
#include<map>
#include<vector>
#include<time.h>
using namespace std;

class fileSys {
public:
	fileSys();

	void running();

	//��ʼ�������ļ�
	void init();

private:

	util utSys;

	blockD blUt;
	
	//�洢�û�������
	vector<user> userVec;

	//λ��ͼ��Ϊ'0'ʱ��ʾ����ã�һ��涨Ϊ512B
	char blockMap[blockMap_Len] = {0};

	//��ǰ���û�
	struct user curUser;

	//�洢fcb
	map<int, fcb> fcbMap;

	//�洢Ϊ�ļ��е�fcb��Ŀ¼���ж����ļ�����vecotrΪ0ʱ���洢���Ǹ��ļ���
	map<int, vector<int>> dirMap;


	//����
	map<string, int> indexMap;


	//���
	vector<group> gVec;


};

#endif