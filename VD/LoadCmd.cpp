#include "LoadCmd.h"
#include <fstream>
#include "VirtualDiskInside.h"
bool LoadCmd::Execute(VirtualDiskInside * virtualdisk)
{
	m_VirtualDisk = virtualdisk;

	//该命令直接收一条路径，必须为真实路径

	//创建一个节点用于接受从文件中读取的数据
	//将此节点设置为root节点，
		//如何处理源根节点？



	vector<string> pathItems = cmdParaCollection.m_pathItems;

	Path srcPath(pathItems[0]);

	if (!srcPath.IsReal())
	{
		m_VirtualDisk->LogMsgToConsole("不是真实路径");
		return false;
	}

	fstream fin;
	fin.open(srcPath.str(), ios::binary | ios::in);

	if (!fin)
	{
		m_VirtualDisk->LogMsgToConsole("文件打开失败");
		return false;
	}

	CellNode* rootNode = new CellNode();

	fin >> *rootNode;

	fin.close();


	if (rootNode->GetCellName().empty() || rootNode->GetCellName()!= "/")
	{
		m_VirtualDisk->LogMsgToConsole("读取失败");
		return false;
	}

	m_VirtualDisk->SetRootNode(rootNode);
	m_VirtualDisk->SetWorkingNode(rootNode);

	return true;
}
