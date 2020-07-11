#pragma once
#ifndef __DEFINE_HEAD__
#define __DEFINE_HEAD__
#define _CRT_SECURE_NO_WARNINGS

#include<string>
#include<vector>
#include<time.h>
using namespace std;
//一个文件最多占用的块数量
#define File_Len 10

//存放位视图的大小，2048B用来存放位视图，2049是换行
#define blockMap_Len 2049


//索引map的长度
#define indexLen 50

enum CMD {
	ERR = -2,
	EXIT = -1,
	LOGIN = 0,
	CREATE = 1,
	OPEN = 2,
	READ = 3,
	WRITE = 4,
	CLOSE = 5,
	DELETE = 6,
	MKDIR = 7,
	CD = 8,
	DIR = 9,
	LOGOUT = 10,
	PWD = 11
};

typedef struct fcb {

	//索引节点号
	int inode;

	//文件名
	string fileName;

	//扩张名
	string exName;

	//文件属性，目录/文件，d是目录，f是文件
	char attribute;

	//文件主id
	int uid;

	//文件所属组
	int gid;

	//文件修改时间
	struct tm time;

	//文件长度
	unsigned long fileSize;

	//文件状态,用于实现伪删除,0是删除
	int lifeFlag;

	//占用了哪些分区
	int blockVec[File_Len];

	//权限数组, 控制读、写权限
	char access[4];
}fcb;


typedef struct user {

	//用户所属组别
	int gid;

	//用户id
	int uid;

	string username;

	string passwd;
}user;


typedef struct group {

	//用户组别
	string gName;

	//用户组的id
	int gid;
};
#endif