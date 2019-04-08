#include "TouchCmd.h"
#include "VirtualDiskInside.h"
#include "FileNode.h"
bool TouchCmd::Execute(VirtualDiskInside * virtualdisk)
{
	m_VirtualDisk = virtualdisk;
	vector<string> pathItems = cmdParaCollection.m_pathItems;

	for (size_t i = 0; i < pathItems.size(); i++)
	{
		Path path = pathItems[i];
		//查看当前文件是否已经存在

		//创建文件的命令 不检查符号链接，仅判断文件名是否相同
		//如果存在，给出提示，
		//如果不存在，创建该文件

		CellNode* node;
		node = GetDstNodeByPath(path);
		if (node)
		{
			virtualdisk->LogMsgToConsole("存在同名文件");
		}
		else
		{
			CellNode* prelink = GetDstNodeByPath(path.Parent());


			//创建节点，修改时间在构造函数中实现
			//设置父节点在添加函数中实现

			//virtualdisk->LogMsgToConsole("该位置可能出错，当文件名为空时也会创建：bool TouchCmd::Execute(VirtualDisk * virtualdisk) ");
			if (path.FileName().empty())
			{
				virtualdisk->LogMsgToConsole("该位置可能出错，当文件名为空时也会创建,所以添加此判断 ：bool TouchCmd::Execute(VirtualDisk * virtualdisk) ");
				return false;
			}

			FileNode* fileNode = new FileNode();
			//该位置可能出错
			fileNode->SetCellName(path.FileName());
			//fileNode->SetNodeType(FileNodeType::FILE_CUSTOM);
			prelink->AddSubNode(fileNode);

			//设置文件内容；**********************************可能不方便测试
			//默认为空
		}
	}
	return true;
}
CellNode* TouchCmd::GetDstNodeByPath(const Path &to)
{

	CellNode* targetNode = NULL;
	CellNode* curNode = m_VirtualDisk->GetNodeByPath(to.StartNode());
	curNode = m_VirtualDisk->LookingForTaget(curNode);
	vector<string> path = to.split();

	for (size_t i = 0; i < path.size(); i++)
	{
		targetNode = curNode->GetNode(path[i]);
		targetNode = m_VirtualDisk->LookingForTaget(targetNode);

		/*if (++i < path.size())
		{
			--i;
			targetNode = m_VirtualDisk->LookingForTaget(targetNode);
		}*/

		if (!targetNode)
		{
			//m_VirtualDisk->LogMsgToConsole("路径不存在");
			return NULL;
		}
		else if (targetNode->GetNodeType()& FOLD)
		{
			curNode = targetNode;
		}
		else if (targetNode->GetNodeType()& FILE_CUSTOM)
		{
			//是一个文件,判断是否是最终节点
			if (++i < path.size())
			{
				--i;
				m_VirtualDisk->LogMsgToConsole("路径中不应该出现文件");
				return NULL;
			}
		}
	}
	return targetNode;
}