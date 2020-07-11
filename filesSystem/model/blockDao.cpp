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
	fcbMapFile = config + "/fcbMap";
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

void blockD::initBlockMap(char *blockMap) {
	ofstream fout(blockFile, ios::out);

	blockMap[0] = '1';
	blockMap[1] = '1';
	blockMap[2] = '1';
	blockMap[3] = '1';

	fout.seekp(0, ios::beg);
	fout.write(blockMap, blockMap_Len);
	fout.close();
}

void blockD::readBlockMap(char *blockMap) {
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

string blockD::getFcbMapFile()
{
	return fcbMapFile;
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
		
		fin.getline(str, 20);
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
		fout.put('\n');
		
		
	}
	fout.close();
}

void blockD::writeBlockMap(char *blockMap) {
	fstream fout(blockFile, ios::out|ios::in);
	//将文件指针置在文件开头
	fout.seekp(0, ios::beg);  

	//写入位示图内容
	fout.write(blockMap, blockMap_Len - 1);
	
	fout.close();
}

int blockD::writeReal(int num, char* str)
{
	fstream fout(blockFile,ios::out| ios::in);

	int size = strlen(str);
	if (size > 511) {
		cout << "More than 511B !\n";
		return -1;
	}

	//fout.seekp(0, ios::beg);
	int site = (num - 1) * 512;
	fout.seekp(site, ios::beg);
	fout.write(str, size);
	fout.put(0);
	fout.close();

	return 0;
}

void blockD::writeBlock(char *blockMap,fcb* file, char* contents)
{
	int i,j, k;
	int end;
	int count = 0;
	int content_size = strlen(contents);
	char ch, temp[300] = { 0 };
	file->fileSize += strlen(contents);
	//文件原来就没内容
	if (file->blockVec[0] == 0) {                  
		for (i = 0; i < blockMap_Len; i++) {
			if (blockMap[i] == '0') {
				blockMap[i] = '1';
				writeReal(i + 1, contents);
				file->blockVec[0] = i + 1;
				break;
			}
		}
	}
	else {
		for (i = 1; i < getsize(file->blockVec); i++) {
			if (file->blockVec[i] == 0) {
				end = file->blockVec[i - 1];
				FILE* f = fopen(blockFile.c_str(), "r+");
				//将文件指针置在文件开头
				fseek(f, 0, SEEK_SET); 
				fseek(f, (end - 1) * 512, SEEK_CUR);
				ch = fgetc(f);
				while (ch) {
					//count为这个物理块已经写了的长度,总长511B
					count++;   
					ch = fgetc(f);
				}

				if (count == 511) {         //这个块已经写满了
					fclose(f);
					for (j = 0; j < File_Len; j++) {
						if (blockMap[j] == '0') {
							blockMap[j] = '1';
							writeReal(j + 1, contents);
							file->blockVec[i] = j + 1;
							break;
						}
					}
					break;
				}
				//这个物理块剩下的位置够写
				if ((511 - count) >= content_size) {         
					fseek(f, -1, SEEK_CUR);
					//写入文件
					fwrite(contents, content_size, 1, f);  
					//写入结束符
					fputc(0, f); 
					fclose(f);
				}
				else {
					fseek(f, -1, SEEK_CUR);
					//写入文件
					fwrite(contents, 511 - count, 1, f);  
					//写入结束符
					fputc(0, f); 
					fclose(f);
					for (k = 0; k < content_size - (511 - count); k++) {
						temp[k] = contents[k + (511 - count)];
					}
					//temp为剩下的字符串
					temp[k] = 0;  
					for (j = 0; j < File_Len; j++) {
						if (blockMap[j] == '0') {
							blockMap[j] = '1';
							writeReal(j + 1, temp);
							file->blockVec[i] = j + 1;
							break;
						}
					}
				}
				writeBlockMap(blockMap);
				break;
			}
		}
	}
}

void blockD::writeFcbMap(string fcbMapPath, map<int, vector<int>> dirMap)
{
	ofstream fout(fcbMapPath, ios::out);
	char str[30] = { 0 };

	for (auto fcbSave = dirMap.begin(); fcbSave != dirMap.end(); fcbSave++) {

		//结点
		memset(str, 0, 30);
		sprintf(str, "%d", fcbSave->first);
		fout.write(str, strlen(str));
		fout.put(':');

		vector <int> dirVec = fcbSave->second;

		//长度
		int dirSzie = dirVec.size();
		memset(str, 0, 30);
		sprintf(str, "%d", dirSzie);
		fout.write(str, strlen(str));
		fout.put(':');

		
		for (int i = 0; i < dirSzie; i++) {
			memset(str, 0, 30);
			sprintf(str, "%d", dirVec[i]);
			fout.write(str, strlen(str));
			fout.put(':');
		}
	}
	fout.close();
}

map<int, vector<int>> blockD::readFcbMap(string fcbMapPath)
{
	ifstream fin(fcbMapPath, ios::in);

	if (!fin.is_open()) {
		cout << "Can not open the fcbMap! \n";
		exit(1);
	}

	char ch;
	char str[30] = { 0 };
	ch = fin.get();

	map<int, vector<int>> dirMap;

	while (ch != -1 && ch != 10) {
		vector<int> dirVec;
		int inode;

		memset(str, 0, sizeof(str));
		fin.seekg(-1, ios::cur);
		fin.getline(str, 20, ':');
		inode = atoi(str);

		int size;
		memset(str, 0, sizeof(str));
		fin.getline(str, 20, ':');
		size = atoi(str);

		for (int i = 0; i < size; i++) {
			memset(str, 0, sizeof(str));
			fin.getline(str, 20, ':');
			int num = atoi(str);
			dirVec.push_back(num);
		}

		ch = fin.get();
		dirMap[inode] = dirVec;
	}

	fin.close();
	return dirMap;
}

void blockD::readReal(int num)
{
	
	FILE* f = fopen(blockFile.c_str(), "r");
	char ch, * str;
	str = (char*)malloc(512);
	memset(str, 0, 512);
	int i = 0;
	
	//将文件指针置在文件开头
	fseek(f, 0, SEEK_SET); 

	//将文件指针偏转到对应物理块开头
	fseek(f, (num - 1) * 512, SEEK_CUR);
	ch = fgetc(f);
	while (ch != 0) {
		str[i] = ch;
		ch = fgetc(f);
		i++;
	}
	printf("%s", str);
	
}

void blockD::readBlock(fcb* file)
{
	int i;
	for (i = 0; i < File_Len; i++) {
		if (file->blockVec[i]) {
			readReal(file->blockVec[i]);
		}
		else {
			break;
		}
	}
	printf("\n");
}
