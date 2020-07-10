#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<string>
#include<vector>
#include<time.h>
using namespace std;
//һ���ļ����ռ�õĿ�����
#define File_Len 10

//���λ��ͼ�Ĵ�С��2048B�������λ��ͼ��2049�ǻ���
#define blockMap_Len 2049


//����map�ĳ���
#define indexLen 50


typedef struct fcb {

	//�����ڵ��
	int inode;

	//�ļ���
	string fileName;

	//������
	string exName;

	//�ļ����ԣ�Ŀ¼/�ļ���d��Ŀ¼��f���ļ�
	char attribute;

	//�ļ���id
	int uid;

	//�ļ�������
	int gid;

	//�ļ��޸�ʱ��
	struct tm time;

	//�ļ�����
	unsigned long fileSize;

	//�ļ�״̬,����ʵ��αɾ��,0��ɾ��
	int lifeFlag;

	//ռ������Щ����
	int blockVec[File_Len];

	//Ȩ������, ���ƶ���дȨ��
	char access[4];
}fcb;


typedef struct user {

	//�û��������
	int gid;

	//�û�id
	int uid;

	string username;

	string passwd;
}user;


typedef struct group {

	//�û����
	string gName;

	//�û����id
	int gid;
};
