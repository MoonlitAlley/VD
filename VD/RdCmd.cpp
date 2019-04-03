#include "RdCmd.h"
#include "VirtualDiskInside.h"

bool RdCmd::Execute(VirtualDiskInside * virtualdisk)
{
	m_VirtualDisk = virtualdisk;
	vector<string> pathItems = cmdParaCollection.m_pathItems;
	if (pathItems.empty())
	{
		return false;
		//路径是空的
		//命令语法不正确，最少一条路径，path中已经经过判断
	}
	//对每条路径分别处理
	for (size_t i = 0; i < pathItems.size(); i++)
	{
		CellNode* node = NULL;
		string wildcard;

		//首先过滤不存在的项目
		CellNode* tempNode = m_VirtualDisk->GetNodeByPath(pathItems[i]);

		//NEW
		//CellNode* tempNode = GetDstNodeByPath(pathItems[i]);

		if (!tempNode)
		{
			//没有相应的文件
			continue;
		}
		else if (m_VirtualDisk->IfNodeBeUsing(tempNode))
		{
			//文件正在使用
			m_VirtualDisk->LogMsgToConsole("目标文件包含工作路径");
			continue;
		}
		else
		{
			node = tempNode;
		}

		//开始处理
		if (wildcard.empty())
		{
			wildcard = "*";
		}
		//node不可能为空
		if (node->GetNodeType()&FILE_CUSTOM && node->GetNodeType()&LINK)
		{
			//目标文件不是文件夹
			m_VirtualDisk->LogMsgToConsole("目标文件不是文件夹");
		}
		else if (node->GetNodeType()&FOLD && node->GetNodeType()&LINK)
		{
			//从父级节点中删除
			//m_VirtualDisk->LogMsgToConsole("此处不检查链接目标文件夹是否为空，直接删除");
			node->RemoveFromPreSubList(true);
		}
		else if (node->GetNodeType()&FILE_CUSTOM)
		{
			m_VirtualDisk->LogMsgToConsole("目标文件不是文件夹");
			//目标文件不是文件夹
		}
		else if (node->GetNodeType()&FOLD)
		{
			//如果指定的目录非空，且用户没有输入递归删除的指定，则不执行该路径
			if (!(cmdParaCollection.m_parsedOptions.find("/s") != cmdParaCollection.m_parsedOptions.end()) && !node->FilterSubNode("*").empty())
			{
				//目录不是空的
				m_VirtualDisk->LogMsgToConsole("目录不是空的");
				continue;
			}
			node->RemoveFromPreSubList(true);
		}
		else
		{
			m_VirtualDisk->LogMsgToConsole("如果此条信息被打印，说明程序存在意外情况，详细判断请见：bool RdCmd::Execute");
		}
	}
	return true;
}

//此方法暂时无用 - rd不支持符号链接或者不能删除目录不为空的文件夹
CellNode* RdCmd::GetDstNodeByPath(const Path &to)
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