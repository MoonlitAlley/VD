#include "SaveCmd.h"
#include "Path.h"
#include "VirtualDiskInside.h"
#include<fstream>

bool SaveCmd::Execute(VirtualDiskInside * virtualdisk)
{
	m_VirtualDisk = virtualdisk;

	//������ֱ����һ��·��������Ϊ��ʵ·��

	vector<string> pathItems = cmdParaCollection.m_pathItems;

	Path dstPath(pathItems[0]);

	if (!dstPath.IsReal())
	{
		m_VirtualDisk->LogMsgToConsole("������ʵ·��");
		return false;
	}


	//�Զ����Ƹ�ʽ���ļ�
	fstream fout;
	fout.open(dstPath.str(), ios::binary | ios::out);

	if (!fout)
	{
		m_VirtualDisk->LogMsgToConsole("�ļ���ʧ��");
		return false;
	}

	CellNode* rootNode = m_VirtualDisk->GetRootNode();

	//ʹ�ýڵ������<<������Խڵ�������
	fout << *rootNode;

	fout.close();

	return true;
}
