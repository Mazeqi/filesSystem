#include"../Head/fileSys.h"
#include<iostream>
#include<fstream>
#include<vector>
#include<string.h>
#include<cstdio>
using namespace std;

int main() {

	/*ifstream fin;
	vector<user> vec;
	fin.open("config/user.ini", ios::in);
	vec = test.read_user();
	for (int i = 0; i < vec.size(); i++) {
		struct user u = vec[i];
		cout << u.gid << endl;
		cout << u.uid << endl;
		cout << u.username << endl;
		cout << u.passwd << endl;
	}*/

	//test.createDir("config/config");
	/*blockD bt;
	char map[2049] = { '\0'};
	memset(map, '0', 2048);
	
	bt.initBlock(map);
	cout << map<< endl;*/
	/*ofstream fout("config/fcb", ios::out);
	char str[2048] = {0};
	for (int i = 0; i < 2046; i++) {
	
		str[i] = '1';
	};
	fout.write(str, strlen(str));
	fout.put('\n');*/

	/*string s = "as // d s /  g ;";
	string p = "/";
	util ut;
	vector<string> sVec;
	sVec = ut.splitString(s, p);
	for (int i = 0; i < sVec.size(); i++) {
		cout << sVec[i] << endl;
	}*/

	fileSys s;
	s.running();
	
	
	
	return 0;
}