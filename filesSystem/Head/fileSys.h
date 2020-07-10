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

	//初始化所有文件
	void init();

private:

	util utSys;

	blockD blUt;
	
	//存储用户的属性
	vector<user> userVec;

	//位视图，为'0'时表示块可用，一块规定为512B
	char blockMap[blockMap_Len] = {0};

	//当前的用户
	struct user curUser;

	//存储fcb
	map<int, fcb> fcbMap;

	//存储为文件夹的fcb的目录下有多少文件，当vecotr为0时，存储的是父文件夹
	map<int, vector<int>> dirMap;


	//索引
	map<string, int> indexMap;


	//组别
	vector<group> gVec;


};

#endif