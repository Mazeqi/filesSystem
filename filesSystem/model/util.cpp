#include"../Head/define.h"
#include"../Head/util.h"
#include<fstream>
#include<string>
#include<iostream>
#include<io.h>
#include<direct.h>
#include<time.h>
#include<string.h>
#include<cstdio>
#include<algorithm>
using namespace std;

//
//int util::read_file(ifstream &fin) {
//	char ch;
//	int count = 0;
//
//	while (!fin.eof()) {
//		//读取一个字符
//		ch = fin.get();
//
//		//10是换行 32是空格 44是逗号
//		if (ch == 10 || ch == 32 || ch == 44  || ch == ':') {
//			break;
//		}
//		count++;
//	}
//
//	fin.seekg(-count-1, ios::cur);
//	return count;
//}



bool util::createDir(string dirPath) {
	if (_access(dirPath.c_str(), 0) == -1) {
		if (_mkdir(dirPath.c_str()) == -1) {
			//cout << "Create dir " << dirPath << "error ! \n";
			return false;
		}
		
	}
	return true;
}

bool util::is_emptyFile(string filePath) {
	char ch;
	ifstream fin(filePath, ios::in);


	if (!fin.is_open()) {
		return true;
	}

	ch = fin.get();

	if (ch == EOF) {
		return true;
	}
	else {
		return false;
	}
}

struct tm util:: get_cur_time() {
	time_t t = time(NULL);
	struct tm* tm;
	tm = localtime(&t);
	tm->tm_year += 1900;
	tm->tm_mon++;
	return *tm;
}

vector<string> util:: splitString(const string& str, const string& pattern)
{
	vector <string> sVec;

	if (str == "") {
		return sVec;
	}
	string strTo = str + pattern;

	char strTochar[100];
	strcpy(strTochar, strTo.c_str());

	char pat[100];
	strcpy(pat, pattern.c_str());

	char* res;
	res = strtok(strTochar, pat);

	while (res)
	{
		
		sVec.push_back(string(res));
		res = strtok(NULL, pat);
		
	}
	return sVec;
	//return 0;
}

CMD util::commandParse(vector<string>& strVec, string command)
{

	
	strVec = splitString(command, " ");

	int vecSize = strVec.size();
	if (vecSize == 0) {
		return ERR;
	}

	transform(strVec[0].begin(), strVec[0].end(), strVec[0].begin(), ::tolower);

	

	if (vecSize == 1) {
		if (strVec[0] == "exit") {
			return EXIT;
		}
		if (strVec[0] == "read") {
			return READ;
		}
		if (strVec[0] == "close") {
			return CLOSE;
		}
		if (strVec[0] == "dir") {
			return DIR;
		}
		if (strVec[0] == "logout") {
			return LOGOUT;
		}
		if (strVec[0] == "pwd") {
			return PWD;
		}

		if (strVec[0] == "showmap") {
			return SHOWMAP;
		}

		if (strVec[0] == "showuser") {
			return SHOWUSER;
		}
	
		return ERR;
	}

	if (vecSize == 2) {
		if (strVec[0] == "open") {
			return OPEN;
		}

		if (strVec[0] == "write") {
			return WRITE;
		}

		if (strVec[0] == "delete") {
			return DELETE;
		}

		if (strVec[0] == "cd") {
			return CD;
		}
		return ERR;
	}

	if (vecSize == 3) {
		if (strVec[0] == "login") {
			return LOGIN;
		}
		if (strVec[0] == "create") {
			return CREATE;
		}

		if (strVec[0] == "mkdir") {
			return MKDIR;
		}
		return ERR;
	}



	return ERR;
}

void util::auth_disp(char* access) {
	int i;
	for (i = 0; i < strlen(access); i++) {
		switch (*(access + i)) {
		case '1': printf("--x"); break;
		case '2': printf("-w-"); break;
		case '3': printf("-wx"); break;
		case '4': printf("r--"); break;
		case '5': printf("r-x"); break;
		case '6': printf("rw-"); break;
		case '7': printf("rwx"); break;
		}
	}
	printf("\t");
}

bool util::fileParse(string fileName, vector<string> &strVec)
{
	
	if (fileName == "") {
		return false;
	}
	//fileName += ".";
	strVec = splitString(fileName, ".");
	int vecSize = strVec.size();
	if (vecSize != 2) {
		return false;
	}

	
	return true;
}
