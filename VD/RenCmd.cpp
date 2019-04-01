#include "RenCmd.h"
#include "VirtualDiskInside.h"
bool RenCmd::Execute(VirtualDiskInside * virtualdisk)
{
	m_VirtualDisk = virtualdisk;
	//类似于Mklink的路径存取方法

	//第一个路径存放着待修改节点的路径信息
	//第二个路径存放着要修改成的名字

	//查找检测第一个路径所指向节点的状态
		//不存在 -》 给出提示并退出
		//存在 -》 是否正在被使用
					//是	-》 退出
					//否	-》 目标名是否合法
							//合法 -》修改节点名




	//两个问题，
		//1、通配符
		//2、符号链接下的地址


	vector<string> pathItems = cmdParaCollection.m_pathItems;
	Path srcNodePath(pathItems[0]);
	string applyName = pathItems[1];
	CellNode* tempNode = NULL;
	if (Tools::isWildcard(srcNodePath.split().back()))
	{
		string wildcard = srcNodePath.split().back();

		vector<string> wildPath = srcNodePath.Self().append("..").split();

		CellNode* curNode = virtualdisk->GetNodeByPath(srcNodePath.StartNode());

		curNode = virtualdisk->LookingForTaget(curNode);


		CellNode* tempNode = NULL;
		for (size_t i = 0; i < wildPath.size(); i++)
		{
			tempNode = curNode->GetNode(wildPath[i]);
			if (++i < wildPath.size())
			{
				--i;
				tempNode = virtualdisk->LookingForTaget(tempNode);
			}
			if (!tempNode)
			{
				virtualdisk->LogMsgToConsole("路径不存在");
				return false;

			}
			else if (tempNode->GetNodeType()& FOLD)
			{
				curNode = tempNode;
			}
			else if (tempNode->GetNodeType()& FILE_CUSTOM)
			{
				//是一个文件,判断是否是最终节点
				if (++i < wildPath.size())
				{
					--i;
					virtualdisk->LogMsgToConsole("路径中不应该出现文件");
					return false;
				}

			}
		}

		//假设curNode存在
		list<CellNode*> wildNodeList = tempNode->FilterSubNode(wildcard);

		if (wildNodeList.empty())
		{
			//目标不存在
			return false;
		}

		CellNode* targetNode = wildNodeList.front();
		if (!targetNode)
		{
			m_VirtualDisk->LogMsgToConsole("目标节点不存在");
			return false;
		}
		else if (m_VirtualDisk->IfNodeBeUsing(targetNode))
		{
			m_VirtualDisk->LogMsgToConsole("目标路径是工作路径");
			return false;
		}
		else if (!Tools::IsLegalFileName(applyName))
		{
			m_VirtualDisk->LogMsgToConsole("名称不合法");
			return false;
		}
		else if (targetNode->GetParent()->GetNode(applyName))
		{
			m_VirtualDisk->LogMsgToConsole("存在同名文件");
			return false;
		}
		else
		{
			targetNode->SetCellName(applyName);
		}
		return true;

	}
	else
	{
		CellNode* curNode = virtualdisk->GetNodeByPath(srcNodePath.StartNode());

		curNode = virtualdisk->LookingForTaget(curNode);

		vector<string> path = srcNodePath.split();


		CellNode* targetNode =NULL;
		for (size_t i = 0; i < path.size(); i++)
		{
			targetNode = curNode->GetNode(path[i]);
			if (++i < path.size())
			{
				--i;
				targetNode = virtualdisk->LookingForTaget(targetNode);
			}
			if (!targetNode)
			{
				virtualdisk->LogMsgToConsole("路径不存在");
				return false;

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
					virtualdisk->LogMsgToConsole("路径中不应该出现文件");
					return false;
				}

			}
		}

		if (!targetNode)
		{
			m_VirtualDisk->LogMsgToConsole("目标节点不存在");
			return false;
		}
		else if (m_VirtualDisk->IfNodeBeUsing(targetNode))
		{
			m_VirtualDisk->LogMsgToConsole("目标路径是工作路径");
			return false;
		}
		else if (!Tools::IsLegalFileName(applyName))
		{
			m_VirtualDisk->LogMsgToConsole("名称不合法");
			return false;
		}
		else if (targetNode->GetParent()->GetNode(applyName))
		{
			m_VirtualDisk->LogMsgToConsole("存在同名文件");
			return false;
		}
		else
		{
			targetNode->SetCellName(applyName);
		}
		return true;
	}

	//vector<string> pathItems = cmdParaCollection.m_pathItems;
	//Path srcNodePath(pathItems[0]);
	//string applyName = pathItems[1];

	//CellNode* srcNode = m_VirtualDisk->GetNodeByPath(srcNodePath);

	//if (!srcNode)
	//{
	//	m_VirtualDisk->LogMsgToConsole("目标节点不存在");
	//	return false;
	//}
	//else if (m_VirtualDisk->IfNodeBeUsing(srcNode))
	//{
	//	m_VirtualDisk->LogMsgToConsole("目标路径是工作路径");
	//	return false;
	//}
	//else if (!Tools::IsLegalFileName(applyName))
	//{
	//	m_VirtualDisk->LogMsgToConsole("名称不合法");
	//	return false;
	//}
	//else if (srcNode->GetParent()->GetNode(applyName))
	//{
	//	m_VirtualDisk->LogMsgToConsole("存在同名文件");
	//	return false;
	//}
	//else
	//{
	//	srcNode->SetCellName(applyName);
	//}
	//return true;
}
