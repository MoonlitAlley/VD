#include "LoadCmd.h"
#include <fstream>
#include "VirtualDiskInside.h"
bool LoadCmd::Execute(VirtualDiskInside * virtualdisk)
{
	m_VirtualDisk = virtualdisk;

	//������ֱ����һ��·��������Ϊ��ʵ·��

	//����һ���ڵ����ڽ��ܴ��ļ��ж�ȡ������
	//���˽ڵ�����Ϊroot�ڵ㣬
		//��δ���Դ���ڵ㣿



	vector<string> pathItems = cmdParaCollection.m_pathItems;

	Path srcPath(pathItems[0]);

	if (!srcPath.IsReal())
	{
		m_VirtualDisk->LogMsgToConsole("������ʵ·��");
		return false;
	}

	fstream fin;
	fin.open(srcPath.str(), ios::binary | ios::in);

	if (!fin)
	{
		m_VirtualDisk->LogMsgToConsole("�ļ���ʧ��");
		return false;
	}

	CellNode* rootNode = new CellNode();

	fin >> *rootNode;

	fin.close();


	if (rootNode->GetCellName().empty() || rootNode->GetCellName()!= "/")
	{
		m_VirtualDisk->LogMsgToConsole("��ȡʧ��");
		return false;
	}

	m_VirtualDisk->SetRootNode(rootNode);
	m_VirtualDisk->SetWorkingNode(rootNode);

	return true;
}
