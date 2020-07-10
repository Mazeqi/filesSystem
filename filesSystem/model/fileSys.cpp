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
	
	memset(blockMap, '0', blockMap_Len - 1);
	
}

void fileSys::init() {
	//��������ļ���
	utSys.createDir(blUt.getConfig());

	if (utSys.is_emptyFile(blUt.getBlockFile())) {
		blUt.initBlock(blockMap);
	}
	else {
		blUt.readBlock(blockMap);
	}
	
	//��ȡuser.ini
	if (!utSys.is_emptyFile(blUt.getUserFile())) {
		userVec = blUt.read_user(blUt.getUserFile());
	}
	else {
		cout << "Can not open user.ini! \n";
		exit(1);
	}

	//��ȡ���,��ͬ�����в�ͬȨ��
	if (!utSys.is_emptyFile(blUt.getGroupFile())) {
		gVec = blUt.readGroup(blUt.getGroupFile());
	}
	else {
		printf("group.ini can not open!\n");
		exit(1);
	}

	////��ȡ������Ϣ
	//if (!utSys.is_emptyFile(blUt.getIndexFile())) {
	//
	//}

	//��ȡ���û���fcb��
	if (!utSys.is_emptyFile(blUt.getFcbFile() + to_string(curUser.uid))) {
		 fcbMap = blUt.readFcb(blUt.getFcbFile() + to_string(curUser.uid));
	}
	else {
		struct fcb fcbI;
		fcbI.fileName = "root";
		fcbI.inode = 0;
		fcbI.attribute = 'd';
		fcbI.uid = 1;
		fcbI.gid = 1;
		strcpy(fcbI.access, "777");
		fcbI.time = utSys.get_cur_time();

		fcbMap[fcbI.inode] = fcbI;
	}



}

