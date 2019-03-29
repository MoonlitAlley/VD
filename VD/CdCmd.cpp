#include "CdCmd.h"
#include "VirtualDiskInside.h"



bool CdCmd::Execute(VirtualDiskInside * virtualdiskInside)
{
	//从命令参数中取出路径
	vector<string> pathItem = cmdParaCollection.m_pathItems;

	//该命令最多支持一条路径
	if (pathItem.size() > 1)
	{
		return false;
	}
	//该命令支持空路径，如果路径为空，则显示当前路径
	if (pathItem.empty())
	{
		//打印当前路径****************************
		virtualdiskInside->LogMsgToConsole(virtualdiskInside->GetWorkingPath().str());
		return false;
		//不处理
	}
	else
	{
		Path path(pathItem[0]);
		//根据路径去寻找相应的节点
		CellNode* node = virtualdiskInside->GetNodeByPath(path);
		node = virtualdiskInside->LookingForTaget(node);
		if (!node)
		{
			virtualdiskInside->LogMsgToConsole("路径不存在或为文件名");
			return false;
		}
		else if (node->GetNodeType()& FOLD)
		{
			virtualdiskInside->SetWorkingNode(node);
			return true;
		}
		else
		{
			//路径不存在或为文件名
			virtualdiskInside->LogMsgToConsole("路径不存在或为文件名");
			return false;
		}
	}
}

#pragma region 丢弃方法
//
//bool CdCmd::Execute(VirtualDisk * virtualdisk)
//{
//	//从命令参数中取出路径
//	vector<string> pathItem = cmdParaCollection.m_pathItems;
//
//	//该命令最多支持一条路径
//	if (pathItem.size() > 1)
//	{
//		return false;
//	}
//	//该命令支持空路径，如果路径为空，则显示当前路径
//	if (pathItem.empty())
//	{
//		//打印当前路径****************************
//		virtualdisk->LogMsgToConsole(virtualdisk->GetWorkingPath().str());
//		return false;
//		//不处理
//	}
//	else
//	{
//		Path path(pathItem[0]);
//		//根据路径去寻找相应的节点
//		//CellNode* node = virtualdisk->GetNodeByPath(path);
//		vector<string> items = path.split();
//		CellNode* curNode = virtualdisk->GetNodeByPath(path.StartNode());
//		for (size_t i = 0; i < items.size(); i++)
//		{
//			if (curNode == NULL)
//			{
//
//				return false;
//			}
//
//			//根据当前节点名称去寻找相应的节点
//			CellNode* child = curNode->GetNode(items[i]);
//
//			if (!child)
//			{
//				virtualdisk->LogMsgToConsole("路径不存在或为文件名");
//				return false;
//			}
//
//			if (child->GetNodeType() == FOLD || child->GetNodeType() == LINK || child->GetNodeType() == DISK)
//			{
//				//如果child的类型为文件夹，则curNode = child ; 若类型为link，则curNode 等于 child的target
//				curNode = virtualdisk->LookingForTaget(child);
//				if (curNode->GetNodeType() == FILE_CUSTOM)
//				{
//					virtualdisk->LogMsgToConsole("路径不存在或为文件名");
//					return false;
//				}
//				continue;
//			}
//			else
//			{
//				//路径不存在或为文件名
//				virtualdisk->LogMsgToConsole("路径不存在或为文件名");
//				return false;
//			}
//		}
//
//		if (curNode == NULL)
//		{
//			virtualdisk->LogMsgToConsole("找不到路径或者文件");
//			return false;
//		}
//		else if (curNode->GetNodeType() == FOLD || curNode->GetNodeType() == DISK)
//		{
//			virtualdisk->SetWorkingNode(curNode);
//			return true;
//		}
//		else
//		{
//			virtualdisk->LogMsgToConsole("路径只支持文件夹名");
//			return false;
//		}
//	}
//
//}

#pragma endregion