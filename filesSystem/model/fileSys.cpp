#include"../Head/fileSys.h"
#include"../Head/util.h"
#include"../Head/define.h"
#include"../Head/blockDao.h"
#include<string>
#include<iostream>
#include<fstream>
#include<vector>
using namespace std;

fileSys::fileSys() {
	curUser.username = "";
	curUser.passwd = "";
	memset(blockMap, '0', blockMap_Len - 1);
	
}

void fileSys::init() {
	//创建存放文件夹
	utSys.createDir(blUt.getConfig());

	if (utSys.is_emptyFile(blUt.getBlockFile())) {
		blUt.initBlock(blockMap);
	}
	else {
		blUt.readBlock(blockMap);
	}
	
	//读取user.ini
	if (!utSys.is_emptyFile(blUt.getUserFile())) {
		userVec = blUt.read_user(blUt.getUserFile());
	}
	else {
		cout << "Can not open user.ini! \n";
		exit(1);
	}

	//读取组别,不同分组有不同权限
	if (!utSys.is_emptyFile(blUt.getGroupFile())) {
		gVec = blUt.readGroup(blUt.getGroupFile());
	}
	else {
		printf("group.ini can not open!\n");
		exit(1);
	}

	////读取索引信息
	//if (!utSys.is_emptyFile(blUt.getIndexFile())) {
	//
	//}

	
}


//login之后才初始化
void fileSys::initFcb() {
	//读取该用户的fcb表
	if (!utSys.is_emptyFile(blUt.getFcbFile() + to_string(curUser.uid))) {
		fcbMap = blUt.readFcb(blUt.getFcbFile() + to_string(curUser.uid));
		curDir = &fcbMap[0];
	}
	else {
		fcbMap[0].fileName = "root";
		fcbMap[0].inode = 0;
		fcbMap[0].attribute = 'd';
		fcbMap[0].uid = curUser.uid;
		fcbMap[0].gid = curUser.gid;
		strcpy(fcbMap[0].access, "777");
		fcbMap[0].time = utSys.get_cur_time();

		curDir = &fcbMap[0];

		//把本文件夹push进来，因为这里是root，所以父文件夹只能是自己
		vector<int> dirVec;
		dirVec.push_back(0);

		dirMap[0] = dirVec;
	}
}

void fileSys::start(){
	cout << "Welcome to MZQ fileSystem ! \n";
	while (1) {
		char buf[1024];
		memset(buf, 0, 1024);
		fflush(stdin);
		cout << "Please input your command ! \n";
		pwd();
		cout << ">> ";
		cin.getline(buf, 1024);

		string command = string(buf);

		vector<string> strVec;
		CMD Swit = utSys.commandParse(strVec, command);

		switch (Swit) {

			case DIR: {
				dir();
				break;
			}

			case EXIT: {
				cout << "Bye ! \n";
				return;
			}

			default: {
				cout << "unkown command !\n";
			}

		}
		cout << endl;
	}
}

bool fileSys::is_file_in_curDir(int inode)
{
	vector<int> dirVec;
	dirVec = dirMap[curDir->inode];
	for (int i = 1; i < dirVec.size(); i++) {
		if (dirVec[i] == inode) {
			return true;
		}
	}
	return false;
}

void fileSys::running() {
	init();
	/*if (cmd.login(userVec, "abc", "1234", curUser)) {
		cout << curUser.username << endl;
		cout << curUser.passwd << endl;
	}*/

	while (1) {
		char buf[1024];
		memset(buf, 0, 1024);
		fflush(stdin);
		cout << "Please input \"login username password \" to login ! \n";
		cout << ">> ";
		cin.getline(buf, 1024);

		string command = string(buf);

		vector<string> strVec;
		CMD Swit = utSys.commandParse(strVec, command);

		switch (Swit) {

			case LOGIN: {
				string username = strVec[1];
				string passwd = strVec[2];

				if (login(username, passwd)) {
					system("cls");
					initFcb();
					start();
				}
				else {
					cout << "login filed ! \n";
				}

				break;
			}
			
			case EXIT: {
				exit(0);
			}

			default: {
				cout << "unkown command !\n";
			}

		}
		

	}
}

//cmd
bool fileSys::login(string username, string password)
{
	for (int i = 0; i < userVec.size(); i++) {
		if (username == userVec[i].username) {
			if (password == userVec[i].passwd) {
				curUser.uid = userVec[i].uid;
				curUser.gid = userVec[i].gid;
				curUser.username = username;
				curUser.passwd = password;
				return true;
			}
		}
	}
	return false;
}

void fileSys::dir(){
	fcb_disp();
}
void fileSys::fcb_disp(){
	vector<int> nodeVec = dirMap[curDir->inode];
	//cout << "Current Dir:" << curDir->fileName  << endl;
	for (int i = 0; i < nodeVec.size(); i++) {
		cout << fcbMap[i].time.tm_year << "/" << fcbMap[i].time.tm_mon << "/" << fcbMap[i].time.tm_mday << "\t";
		cout << fcbMap[i].time.tm_hour << ":" << fcbMap[i].time.tm_min << ":" << fcbMap[i].time.tm_sec << "\t";
		if (fcbMap[i].attribute == 'd') {
			cout << "<DIR> \t";
		}
		else {
			cout << "<FILE>\t";
		}
		cout << fcbMap[i].fileName << endl ;
	}
	

}

void fileSys::pwd(){

	string pth = "user.";
	pth += curUser.username;
	pth += "/";
	pth += "root";

	//if (curDir->inode == 0) {
	//	cout << pth;
	//}
	//else {
		
	vector<int> nodeVec = dirMap[curDir->inode];
	int father = nodeVec[0];
	while (father != 0) {
		pth += "/";
		pth += fcbMap[father].fileName;

		vector<int> nodeVec = dirMap[curDir->inode];
		int father = nodeVec[0];
	}
	cout << pth;
	//}
}


bool fileSys::auth(char op, struct fcb* file) {
	if (curUser.uid == 1) {
		return true;
	}

	int acc;
	if (file->uid == curUser.uid) {
		acc = 0;
	}
	else if (file->gid == curUser.gid) {
		acc = 1;
	}
	else {
		acc = 2;
	}
	switch (op) {
		case 'r':
			if (file->access[acc] != '1' && file->access[acc] != '2' && file->access[acc] != '3') {
				return true;
			}
			else {
				return false;
			}
		case 'w':
			if (file->access[acc] != '1' && file->access[acc] != '4' && file->access[acc] != '5') {
				return true;
			}
			else return false;
		case 'x':
			if (file->access[acc] != '2' && file->access[acc] != '4' && file->access[acc] != '6') {
				return true;
			}
			else {
				return false;
			}
		default: {
			return false;
			break;
		} 
	}

}

