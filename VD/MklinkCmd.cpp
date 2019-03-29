#include "MklinkCmd.h"
#include "VirtualDiskInside.h"
#include "Path.h"
bool MklinkCmd::Execute(VirtualDiskInside * virtualdisk)
{
	m_VirtualDisk = virtualdisk;
	//在pathItems中保存着两个items，分别存有一个路径
	//得到目标节点的 路径、类型、名称信息 ， 存放到要建立的符号链接节点的content中。

	
	vector<string> pathItems = cmdParaCollection.m_pathItems;
	//首先要保证该vector中有两个路径信息。
	Path linkNodePath(pathItems[0]);
	Path targetNodePath(pathItems[1]);

	if (linkNodePath.FileName().empty())
	{
		m_VirtualDisk->LogMsgToConsole("文件名不合法，退出");
		return false;
	}

	if (virtualdisk->GetNodeByPath(linkNodePath))
	{
		m_VirtualDisk->LogMsgToConsole("创建的节点存在同名文件，退出");
		return false;
	}

	CellNode* targetNode = virtualdisk->GetNodeByPath(targetNodePath);
	if (!targetNode)
	{
		m_VirtualDisk->LogMsgToConsole("目标节点不存在，退出");
		return false;
	}
	if (cmdParaCollection.m_parsedOptions.find("/d") != cmdParaCollection.m_parsedOptions.end() && !(targetNode->GetNodeType()&FOLD))
	{
		m_VirtualDisk->LogMsgToConsole("目标文件必须是目录，退出");
		return false;
	}
	if (!(cmdParaCollection.m_parsedOptions.find("/d") != cmdParaCollection.m_parsedOptions.end()) && !(targetNode->GetNodeType()&FILE_CUSTOM))
	{
		m_VirtualDisk->LogMsgToConsole("目标文件必须是文件，退出");
		return false;
	}


	//判断结束，开始创建链接。

	//目标文件是否可以是符号链接？

	string filename = linkNodePath.FileName();
	//找到待创建路径的上级节点
	linkNodePath.append("..");

	CellNode* prelink = virtualdisk->GetNodeByPath(linkNodePath);

	if (!prelink)
	{
		return false;
	}

	//创建该节点
	CellNode* node = new CellNode();
	node->SetCellName(filename);
	prelink->AddSubNode(node);

	node->SetNodeType(FileNodeType(LINK |targetNode->GetNodeType()));

	//将目标节点的信息放到该节点的content中
	const string tempPathString = targetNode->GetNodePath().str();
	node->Content().assign(tempPathString.begin(), tempPathString.end());

	return true;
}
