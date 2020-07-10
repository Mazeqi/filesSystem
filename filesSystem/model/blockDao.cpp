#define _CRT_SECURE_NO_WARNINGS
#include"../Head/define.h"
#include "../Head/blockDao.h"
#include<fstream>
#include<vector>
#include<string>
using namespace std;

blockD::blockD() {
	config = "config";
	blockFile = config + "/block";
	userFile = config + "/user.ini";
	indexFile = config + "/index";
	groupFile = config + "/group.ini";
	fcbFile = config + "/fcb";
}

string blockD::getFcbFile() {
	return fcbFile;
}
string blockD::getBlockFile() {
	return blockFile;
}

string blockD::getUserFile() {
	return userFile;
}

string blockD::getIndexFile() {
	return indexFile;
}

string blockD::getGroupFile() {
	return groupFile;
}
string blockD::getConfig() {
	return config;
}

void blockD::initBlock(char *blockMap) {
	ofstream fout(blockFile, ios::out);

	blockMap[0] = '1';
	blockMap[1] = '1';
	blockMap[2] = '1';
	blockMap[3] = '1';

	fout.seekp(0, ios::beg);
	fout.write(blockMap, blockMap_Len);
	fout.close();
}

void blockD::readBlock(char *blockMap) {
	fstream fin(blockFile, ios::in);

	fin.seekg(0, ios::beg);
	fin.read(blockMap, blockMap_Len - 1);
	fin.close();
}


vector<user> blockD::read_user(string userPath) {
	ifstream fin("config/user.ini", ios::in);

	if (!fin.is_open()) {
		cout << "Open the user.ini error\n";
		exit(0);
	}
	char ch;
	char str[30] = { 0 };

	vector<user> userVec;
	ch = fin.get();

	while (ch != -1 && ch != 10) {
		struct user userRe;
		fin.seekg(-1, ios::cur);
		//读取一个分号前的
		fin.getline(str, 20, ':');
		userRe.uid = atoi(str);


		fin.getline(str, 20, ':');
		userRe.gid = atoi(str);

		fin.getline(str, 20, ':');
		userRe.username = string(str);

		fin.getline(str, 20);
		userRe.passwd = string(str);

		ch = fin.get();
		userVec.push_back(userRe);
	}
	fin.close();
	return userVec;
}


vector<group> blockD::readGroup(string groupPath) {
	ifstream fin("config/group.ini", ios::in);

	if (!fin.is_open()) {
		cout << "Open the group.ini error\n";
		exit(0);
	}
	char ch;
	char str[30] = { 0 };

	vector<group> gVec;
	ch = fin.get();

	while (ch != -1 && ch != 10) {
		struct group gRe;
		fin.seekg(-1, ios::cur);
		//读取一个分号前的
		fin.getline(str, 20, ':');
		gRe.gid = atoi(str);


		fin.getline(str, 20);
		gRe.gName = string(str);

		ch = fin.get();
		gVec.push_back(gRe);
	}
	fin.close();
	return gVec;
}


map<int, fcb> blockD::readFcb(string fcbPath) {
	ifstream fin(fcbPath, ios::in);

	if (!fin.is_open()) {
		cout << "Open the fcb error ! \n";
		exit(1);
	}

	char ch;
	char str[30] = { 0 };

	ch = fin.get();
	map<int, fcb> fcbMap;

	while (ch != -1 && ch != 10) {
		struct fcb fcbRe;
		fin.seekg(-1, ios::cur);
		//读取一个分号前的
		fin.getline(str, 20, ':');
		fcbRe.inode = atoi(str);


		fin.getline(str, 20, ':');
		fcbRe.fileName = string(str);

		fin.getline(str, 20, ':');
		fcbRe.exName = string(str);

		fin.getline(str, 20, ':');
		fcbRe.attribute = *str;
		//ch = fin.get();
		//fcbRe.push_back(gRe);


		fin.getline(str, 20, ':');
		fcbRe.uid = atoi(str);

		fin.getline(str, 20, ':');
		fcbRe.gid = atoi(str);


		//读取时间
		fin.getline(str, 20, ':');
		fcbRe.time.tm_year = atoi(str);
		fin.getline(str, 20, ':');
		fcbRe.time.tm_mon = atoi(str);
		fin.getline(str, 20, ':');
		fcbRe.time.tm_mday = atoi(str);
		fin.getline(str, 20, ':');
		fcbRe.time.tm_hour = atoi(str);
		fin.getline(str, 20, ':');
		fcbRe.time.tm_min = atoi(str);
		fin.getline(str, 20, ':');
		fcbRe.time.tm_sec = atoi(str);

		fin.getline(str, 20, ':');
		fcbRe.fileSize = atoi(str);

		for (int i = 0; i < File_Len; i++) {
			fin.getline(str, 20, ':');
			fcbRe.blockVec[i] = atoi(str);
		}
		
		fin.getline(str, 20,';');
		strcpy(fcbRe.access, str);

		fcbRe.lifeFlag = 1;
		fcbMap[fcbRe.inode] = fcbRe;

		ch = fin.get();
	}
	fin.close();
	return fcbMap;
}

void blockD::writeFcb(string fcbPath,map<int, fcb> fcbMap) {
	ofstream fout(fcbPath, ios::out);

	char str[30] = { 0 };
	
	for (auto fcbSave = fcbMap.begin(); fcbSave != fcbMap.end(); fcbSave++) {
		
		//结点
		memset(str, 0, 30);
		sprintf(str, "%d", fcbSave->second.inode);
		fout.write(str, strlen(str));
		fout.put(':');
	
		//文件名字
		memset(str, 0, 30);
		strcpy(str, fcbSave->second.fileName.c_str());
		fout.write(str, strlen(str));
		fout.put(':');

		//文件拓展名字
		memset(str, 0, 30);
		strcpy(str, fcbSave->second.exName.c_str());
		fout.write(str, strlen(str));
		fout.put(':');

		//文件属性
		fout.put(fcbSave->second.attribute);
		fout.put(':');


		//文件主id
		memset(str, 0, 30);
		sprintf(str, "%d", fcbSave->second.uid);
		fout.write(str, strlen(str));
		fout.put(':');

		//文件所属组别
		memset(str, 0, 30);
		sprintf(str, "%d", fcbSave->second.gid);
		fout.write(str, strlen(str));
		fout.put(':');


		//读取时间
		memset(str, 0, 30);
		sprintf(str, "%d", fcbSave->second.time.tm_year);
		fout.write(str, strlen(str));
		fout.put(':');

		memset(str, 0, 30);
		sprintf(str, "%d", fcbSave->second.time.tm_mon);
		fout.write(str, strlen(str));
		fout.put(':');

		memset(str, 0, 30);
		sprintf(str, "%d", fcbSave->second.time.tm_mday);
		fout.write(str, strlen(str));
		fout.put(':');

		memset(str, 0, 30);
		sprintf(str, "%d", fcbSave->second.time.tm_hour);
		fout.write(str, strlen(str));
		fout.put(':');

		memset(str, 0, 30);
		sprintf(str, "%d", fcbSave->second.time.tm_min);
		fout.write(str, strlen(str));
		fout.put(':');

		memset(str, 0, 30);
		sprintf(str, "%d", fcbSave->second.time.tm_sec);
		fout.write(str, strlen(str));
		fout.put(':');

		//文件长度
		memset(str, 0, 30);
		sprintf(str, "%d", fcbSave->second.fileSize);
		fout.write(str, strlen(str));
		fout.put(':');

		//文件占用盘块
		for (int i = 0; i < File_Len; i++) {
			memset(str, 0, 30);
			sprintf(str, "%d", fcbSave->second.blockVec[i]);
			fout.write(str, strlen(str));
			fout.put(':');
		}

		//文件access
		memset(str, 0, 30);
		strcpy(str, fcbSave->second.access);
		fout.write(str, strlen(str));
		fout.put(';');
	}
	fout.close();
}