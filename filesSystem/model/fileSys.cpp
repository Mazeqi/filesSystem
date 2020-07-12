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
	curDir = NULL;
	
}

void fileSys::init() {
	//创建存放文件夹
	utSys.createDir(blUt.getConfig());

	if (utSys.is_emptyFile(blUt.getBlockFile())) {
		blUt.initBlockMap(blockMap);
	}
	else {
		blUt.readBlockMap(blockMap);
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
		dirMap = blUt.readFcbMap(blUt.getFcbMapFile() + to_string(curUser.uid));
		curDir = &fcbMap[0];
	}
	else {
		fcbMap[0].fileName = "..";
		fcbMap[0].inode = 0;
		fcbMap[0].attribute = 'd';
		fcbMap[0].uid = curUser.uid;
		fcbMap[0].gid = curUser.gid;
		strcpy(fcbMap[0].access, "777");
		fcbMap[0].time = utSys.get_cur_time();
		fcbMap[0].lifeFlag = 1;
		curDir = &fcbMap[0];

		//把本文件夹push进来，因为这里是root，所以父文件夹只能是自己
		vector<int> dirVec;
		dirVec.push_back(0);

		dirMap[0] = dirVec;
	}

	/*if (!utSys.is_emptyFile(blUt.getFcbMapFile() + to_string(curUser.uid))) {
		
	}*/
}

void fileSys::start(){
	cout << "Welcome to MZQ fileSystem ! \n";
	while (1) {
		char buf[1024];
		memset(buf, 0, 1024);
		fflush(stdin);
		//cout << "Please input your command ! \n";
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
			
			case CREATE: {
				mkDir(strVec[1], strVec[2], 'f');
				break;
			}
			case MKDIR: {
				mkDir(strVec[1], strVec[2], 'd');
				break;
			}

			case OPEN: {
				struct fcb* f = openFile(strVec[1]);
				if (f == NULL) {
					cout << "Can not open the file! \n";
				}
				else {
					fileStart(f);
				}
				break;
			}

			case CD: {
				cd(strVec[1]);
				break;
			}

			case SHOW: {
				show();
				break;
			}

			case EXIT: {
				cout << "Bye ! \n";
				saveConfig();
				return;
			}

			case DELETE: {
				del(strVec[1]);
				break;
			}

			default: {
				cout << "unknown command !\n";
			}

		}
		cout << endl;
	}
}

void fileSys::saveConfig() {
	blUt.writeFcb(blUt.getFcbFile() + to_string(curUser.uid), fcbMap);
	blUt.writeBlockMap(blockMap);
	blUt.writeFcbMap(blUt.getFcbMapFile() + to_string(curUser.uid), dirMap);
}

void fileSys::mkDir(string param1, string filename, char type){



	/*if (param1.size() != 3) {
		cout << "param1 error ! \n";
		return;
	}*/

	char acc[6];
	strcpy(acc, param1.c_str());
	for (int i = 0; i < 3; i++) {
		if (acc[i] < '1' || acc[i] > '7') {
			cout << "param1 error ! \n";
			return;
		}
	}

	vector <string> strVec;
	if (type == 'f') {
		if (!utSys.fileParse(filename, strVec)) {
			cout << "fileName error !\n";
			return;
		}
	}

	if (is_file_in_curDir(filename, type)) {
		cout << "The file has in this dir ! \n";
		return;
	}
	
	

	int index = 0;
	for (auto fcbI = fcbMap.begin(); fcbI != fcbMap.end(); fcbI++) {
		if (fcbI->second.lifeFlag == 0) {
			index = fcbI->second.inode;
			break;
		}
		else {
			index++;
		}
	}

	struct fcb nfcb;

	nfcb.inode = index;
	nfcb.uid = curUser.uid;
	nfcb.gid = curUser.gid;
	nfcb.lifeFlag = 1;
	nfcb.time = utSys.get_cur_time();
	nfcb.fileSize = 0;

	strcpy(nfcb.access, acc);
	
	if (type == 'd') {
		nfcb.attribute = 'd';
		nfcb.fileName = filename;
	}
	else {
		nfcb.attribute = 'f';
		nfcb.fileName = strVec[0];
		nfcb.exName = strVec[1];
	}
	for (int i = 0; i < File_Len; i++) {
		nfcb.blockVec[i] = 0;
	}
	fcbMap[index] = nfcb;


	vector<int> dirVec;
	dirVec.push_back(curDir->inode);
	//cout << curDir->inode << " test\n";

	dirMap[index] = dirVec;

	dirVec = dirMap[curDir->inode];
	dirVec.push_back(index);
	dirMap[curDir->inode] = dirVec;


}

fcb* fileSys::openFile(string fileName)
{
	if (!is_file_in_curDir(fileName, 'f')) {
		cout << "The file can not find in cur dir! \n";
		return NULL;
	}

	vector<string> strVec;
	utSys.fileParse(fileName, strVec);

	vector<int> iVec = dirMap[curDir->inode];
	for (int i = 1; i < iVec.size(); i++) {
		if (fcbMap[iVec[i]].fileName == strVec[0] && fcbMap[iVec[i]].exName == strVec[1]) {
			return &fcbMap[iVec[i]];
		}
	}

	return NULL;
}

void fileSys::fileStart(fcb* f)
{
	while (1) {

		char buf[1024];
		memset(buf, 0, 1024);
		fflush(stdin);
		//cout << "Please input your command ! \n";
		//pwd();
		cout << f->fileName << "." << f->exName << ">";
		cin.getline(buf, 1024);

		string command = string(buf);

		vector<string> strVec;
		CMD Swit = utSys.commandParse(strVec, command);
		switch (Swit) {

			case READ: {
				if (auth('r', f)) {
					blUt.readBlock(f);
				}
				else {
					cout << "No access to read the file ! \n";
				}
				break;
			}
			
			case WRITE: {
				if (auth('w', f)) {
					char buffer[512];
					strcpy(buffer, strVec[1].c_str());
					blUt.writeBlock(blockMap, f, buffer);
				}
				else {
					cout << "No access to write the file ! \n";
				}
				break;
			}
			case CLOSE: {
				f = NULL;
				return;
			}

			default: {
				cout << "unknown command! \n";
				break;
			}
		}
		
	}
	
}

void fileSys::cd(string fileName){
	if (fileName == ".") {
		return;
	}

	vector<int> iVec = dirMap[curDir->inode];
	//cout << curDir->inode;
	if (fileName == "..") {
		//cout << iVec[0] << "test\n"; 
		curDir = &fcbMap[iVec[0]];
		
		return;
	}

	 if (!is_file_in_curDir(fileName, 'd')) {
		cout << "can not find the dir !\n";
		return;
	 }

	
	for (int i = 1; i < iVec.size(); i++) {
		if (fcbMap[iVec[i]].fileName == fileName && fcbMap[iVec[i]].attribute == 'd') {
			curDir = &fcbMap[iVec[i]];
			break;
		}
	}
}

void fileSys::del(string fileName)
{
	if (!is_file_in_curDir(fileName, 'f')) {
		cout << "Can not find the file in this dir!\n";
		return;
	}
	vector<int> fileVec = dirMap[curDir->inode];

	vector<string> strVec;
	utSys.fileParse(fileName, strVec);

	int index = 0;
	for (int i = 1; i < fileVec.size(); i++) {
		if (fcbMap[fileVec[i]].fileName == strVec[0] && fcbMap[fileVec[i]].exName == strVec[1]) {
			index = i;
		}
	}

	if (index) {
		fcbMap[fileVec[index]].lifeFlag = 0;
		dirMap[curDir->inode].erase(dirMap[curDir->inode].begin() + index);
		//fcbMap.erase(fileVec[index]);
	}


}

//void fileSys::del(string fileName)
//{
//	//if(!)
//}

void fileSys::show()
{
	for (int i = 0; i < 32; i++) {
		for (int j = 0; j < 64; j++) {
			cout << blockMap[i * 32 + j] << " ";
		}
		cout << endl;
	}
}

bool fileSys::is_file_in_curDir(string fileName, char type)
{
	vector<int> dirVec;
	vector<string> strVec;

	utSys.fileParse(fileName, strVec);

	dirVec = dirMap[curDir->inode];
	if (strVec.size() == 1 && type == 'd') {
		//cout << ".hh" << endl;
		for (int i = 1; i < dirVec.size(); i++) {
			if (fcbMap[dirVec[i]].fileName == fileName && fcbMap[dirVec[i]].attribute == 'd' && fcbMap[dirVec[i]].lifeFlag == 1) {
				cout << fcbMap[dirVec[i]].fileName << endl;
				return true;
			}
		}
	}
	else if (strVec.size() == 2 && type == 'f') {
		for (int i = 1; i < dirVec.size(); i++) {
			if (fcbMap[dirVec[i]].fileName == strVec[0] && fcbMap[dirVec[i]].exName == strVec[1] && fcbMap[dirVec[i]].attribute == 'f' && fcbMap[dirVec[i]].lifeFlag == 1) {
				return true;
			}
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
	//cout << curDir->inode << endl;
	//cout << "Current Dir:" << curDir->fileName  << endl;
	for (int i = 0; i < nodeVec.size(); i++) {

		string sMon; 
		if (fcbMap[nodeVec[i]].time.tm_mon < 10) {
			sMon += "0";
		}
		sMon += to_string(fcbMap[nodeVec[i]].time.tm_mon);

		string sDay;
		if (fcbMap[nodeVec[i]].time.tm_mday < 10) {
			sDay += "0";
		}
		sDay += to_string(fcbMap[nodeVec[i]].time.tm_mday);

		string sHour;
		if (fcbMap[nodeVec[i]].time.tm_hour < 10) {
			sHour += "0";
		}
		sHour += to_string(fcbMap[nodeVec[i]].time.tm_hour);

		string sMin;
		if (fcbMap[nodeVec[i]].time.tm_min < 10) {
			sMin += "0";
		}
		sMin += to_string(fcbMap[nodeVec[i]].time.tm_min);

		string sSec;
		if (fcbMap[nodeVec[i]].time.tm_sec < 10) {
			sSec += "0";
		}
		sSec += to_string(fcbMap[nodeVec[i]].time.tm_sec);


		cout << fcbMap[nodeVec[i]].time.tm_year << "/" << sMon << "/" << sDay << "\t";
		cout <<sHour << ":" << sMin << ":" << sSec <<"\t";
		if (fcbMap[nodeVec[i]].attribute == 'd') {
			cout << "<DIR> \t";
		}
		else {
			cout << "<FILE> \t";
		}

		cout << fcbMap[nodeVec[i]].fileSize << "\t";

		if (i == 0) {
			cout << ".." << endl;
		}
		else {
			if (fcbMap[nodeVec[i]].attribute == 'd') {
				cout << fcbMap[nodeVec[i]].fileName << endl;
			}
			else {
				cout << fcbMap[nodeVec[i]].fileName << "." << fcbMap[nodeVec[i]].exName << endl;
			}
		}

		
		
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
		nodeVec = dirMap[father];
		father = nodeVec[0];
	}
	if (curDir->inode) {
		pth += "/";
		pth += fcbMap[curDir->inode].fileName;
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

