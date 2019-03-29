#include "SaveCmd.h"
#include "Path.h"
#include "VirtualDiskInside.h"
#include<fstream>

bool SaveCmd::Execute(VirtualDiskInside * virtualdisk)
{
	m_VirtualDisk = virtualdisk;

	//该命令直接收一条路径，必须为真实路径

	vector<string> pathItems = cmdParaCollection.m_pathItems;

	Path dstPath(pathItems[0]);

	if (!dstPath.IsReal())
	{
		m_VirtualDisk->LogMsgToConsole("不是真实路径");
		return false;
	}


	//以二进制格式打开文件
	fstream fout;
	fout.open(dstPath.str(), ios::binary | ios::out);

	if (!fout)
	{
		m_VirtualDisk->LogMsgToConsole("文件打开失败");
		return false;
	}

	CellNode* rootNode = m_VirtualDisk->GetRootNode();

	//使用节点的重载<<运算符对节点进行输出
	fout << *rootNode;

	fout.close();

	return true;
}
