#pragma once

#include <string>
#include <algorithm>
#include<regex>

using namespace std;

namespace Tools
{
	bool isWildcard(const string& pattern);

	bool IsWildcardMatch(const string& wildcard, const string &str);

	bool IsLegalPathName(const string & pathName);

	bool IsLegalFileName(const string& filename);
}


class OutputMsg
{
public:
	string Command_Illegal = "�����ʽ�Ƿ�";
	string Path_IsNotExist = "·��������";
	string Path_IsAlreadyExist = "�ļ����ļ����Ѿ�����";
	string QuotationMarks_NotMatch = "���Ų�ƥ��";
	string PathName_Illegal = "·�����Ƿ�";
	string Dir_BeingUse = "��һ����������ʹ�ô��ļ��������޷�����";

	//string CommandCollection[13] = { "cls" ,"cd","save","load", "dir","touch","rd","del","md","ren","copy" ,"move" ,"mklink" };
};


//�ļ��ڵ������
enum FileNodeType
{
	FILE_CUSTOM = 1,	//�ļ�		Ϊ��ֹ��ϵͳ�ؼ��ֳ�ͻ����Ҫʹ�õĵط����������������� ::

	FOLD = 2,	//�ļ���
	
	LINK = 4,	//��������
};

//����Ԫ������
enum CmdElementType
{
	CMD_NAME = 0,	//��������
	CMD_PARA = 1,	//�������
	CMD_PATH = 2,	//����·��
};
//��������/����ID
enum CommandID
{
	clsCmd = 0,	//��֧��ͨ���	��֧�ֲ���

	cdCmd,		//��֧��ͨ���	��֧�ֲ���
	saveCmd,	//��֧��ͨ���	��֧�ֲ���
	loadCmd,	//��֧��ͨ���	��֧�ֲ���

	dirCmd,			//			/s�ݹ����Ŀ¼����������Ŀ¼�µ��ļ� 		/d ֻ�������Ŀ¼
	touchCmd,	//��֧��ͨ���	��֧�ֲ���
	rdCmd,		//��֧��ͨ���	/s�ݹ�ɾ��Ŀ¼����������Ŀ¼�µ��ļ� 
	delCmd,			//			/s�ݹ�ɾ��Ŀ¼����������Ŀ¼�µ��ļ� 
	mdCmd,		//��֧��ͨ���	��֧�ֲ���

	renCmd,		//��֧��ͨ���	��֧�ֲ���
	copyCmd,		//�ڶ���·����֧��ͨ���	/y����ͬ�����ļ����ļ���ʱִ�и���
	moveCmd,		//�ڶ���·����֧��ͨ���	/y����ͬ�����ļ����ļ���ʱִ�и���
	mklinkCmd,	//��֧��ͨ���	/d ����Ŀ¼�ķ������ӣ�������ʹ�ò���ʱ���ɶ�Ŀ¼������������
};