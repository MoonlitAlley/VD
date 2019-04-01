
#include "MdCmd.h"
#include "VirtualDiskInside.h"
#include "Common.h"
//该位置判断可能出错
bool MdCmd::Execute(VirtualDiskInside* virtualdisk)
{
	vector<string> pathItems = cmdParaCollection.m_pathItems;
	for (size_t i = 0; i < pathItems.size(); i++)
	{
		//不处理绝对路径
		//对参数集合中的路径信息进行分解，首先将\替换为/， 然后按照/进行分解。分解为单个节点名称
		Path tempPath(pathItems[i]);

		if (tempPath.IsReal())
		{
			return false;
		}
		//得到路径的节点名称矢量组
		vector<string> items = tempPath.split();

		//判断是否是相对路径来确定当前节点
		CellNode * curNode = virtualdisk->GetNodeByPath(tempPath.StartNode());

		//开始寻路或者创建相应的文件夹
		for (size_t i = 0; i < items.size(); i++)
		{
			if (curNode == NULL)
			{
				return false;
			}
			//判断当前路径节点名称是否合法
			if (!Tools::IsLegalPathName(items[i]))
			{
				return false;
			}
			if (items[i] == "." || items[i] == "..")
			{
				curNode = curNode->GetNode(items[i]);
				continue;
			}

			//根据当前节点名称去寻找相应的节点
			CellNode* child = curNode->GetNode(items[i]);
			if (!child)
			{
				child = new CellNode();
				child->SetCellName(items[i]);
				child->SetNodeType(FOLD);
				curNode->AddSubNode(child);
				//将创建的节点置为当前节点，继续下一个用户输入节点的创建
				curNode = child;
			}
			else if (child->GetNodeType() & FOLD)
			{
				//如果child的类型为文件夹，则curNode = child ; 若类型为link，则curNode 等于 child的target
				curNode = virtualdisk->LookingForTaget(child);
				continue;
			}
			else
			{
				virtualdisk->LogMsgToConsole("已存在相同名称的文件");
				return false;
			}
		}
	}
	return true;
}
