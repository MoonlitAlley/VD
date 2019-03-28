#include "MoveCmd.h"
#include "Path.h"
#include "VirtualDisk.h"
#include "MyString.h"
bool MoveCmd::Execute(VirtualDisk * virtualdisk)
{
	m_VirtualDisk = virtualdisk;
	
	vector<string> pathItems = cmdParaCollection.m_pathItems;

	//copy和move命令支持单个路径，默认第二条路径为当前工作路径
	if (pathItems.size() == 1)
	{
		pathItems.push_back(".");
	}
	

	Path from(pathItems[0]);
	Path to(pathItems[1]);

	if (from.IsReal() || to.IsReal())
	{
		//不支持真实路径
		return false;
	}
	if (!Tools::IsLegalPathName(to.split().back()))
	{
		//目录名不正确
		return false;
	}


	list<CellNode*> srcNodeList;
	if (Tools::isWildcard(from.split().back()))
	{
		//含有通配符
		CellNode* prelink = m_VirtualDisk->GetNodeByPath(from.Parent());
		srcNodeList = prelink->FilterSubNode(from.FileName());

	}
	else
	{
		CellNode* node = m_VirtualDisk->GetNodeByPath(from);
		if (!node)
		{
			//文件不存在
			return false;
		}
		if (m_VirtualDisk->IfNodeBeUsing(node))
		{
			//路径是工作路径
			return false;
		}
		srcNodeList.push_back(node);
	}


	if (srcNodeList.empty())
	{
		//没有需要操作的文件
		return false;
	}
	if (!m_VirtualDisk->GetNodeByPath(to) || m_VirtualDisk->GetNodeByPath(to)->GetNodeType()&FileNodeType::FILE_CUSTOM)
	{
		//目标节点不能是文件节点 或者不存在
		return false;
	}

	//在目标文件夹下存在冲突文件
	string filename = from.FileName();
	if (!m_VirtualDisk->GetNodeByPath(to)->FilterSubNode(filename).empty())
	{
		conflict = true;
	}

	for (auto itera = srcNodeList.begin(); itera != srcNodeList.end(); itera++)
	{
		MoveNode(*itera, to);
	}

	m_VirtualDisk->LogMsgToConsole(StrProcess::sstr("移动了%9d 个文件", srcNodeList.size()));

	return false;
}


void MoveCmd::MoveNode(CellNode* from, Path& to)
{
	CellNode* dstNode = m_VirtualDisk->GetNodeByPath(to);

	//目标是文件夹或者符号链接
	if (dstNode->GetNodeType()&FileNodeType::FOLD)
	{
		
		CellNode* node = m_VirtualDisk->LookingForTaget(dstNode);
		if (!node)
		{
			//找不到路径
			return;
		}
		else if (from == node)
		{
			//相同，不执行
			return;
		}
		CellNode* cur = node->GetParent();
		while (cur)
		{
			if (cur == from)
			{
				//拒绝访问，父级目录正在被使用
				return;
			}
			cur = cur->GetParent();
		}
		//移动该节点

		if (conflict && needAsk)
		{
			while (true)
			{
				//移动文件
			}
		}
		else
		{
			//移动文件
			from->RemoveFromPreSubList(false);
			dstNode->AddSubNode(from);
		}




	}
}