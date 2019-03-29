#include "MoveCmd.h"
#include "Path.h"
#include "VirtualDiskInside.h"
#include "MyString.h"
bool MoveCmd::Execute(VirtualDiskInside * virtualdisk)
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
		m_VirtualDisk->LogMsgToConsole("不支持真实路径");
		return false;
	}
	if (!Tools::IsLegalPathName(to.split().back()))
	{
		m_VirtualDisk->LogMsgToConsole("目录名不正确");
		return false;
	}


	list<CellNode*> srcNodeList;
	if (Tools::isWildcard(from.split().back()))
	{
		//含有通配符
		CellNode* prelink = m_VirtualDisk->GetNodeByPath(from.Parent());
		srcNodeList = prelink->FilterSubNode(from.split().back());

	}
	else
	{
		CellNode* node = m_VirtualDisk->GetNodeByPath(from);
		if (!node)
		{
			m_VirtualDisk->LogMsgToConsole("文件不存在");
			return false;
		}
		if (m_VirtualDisk->IfNodeBeUsing(node))
		{
			m_VirtualDisk->LogMsgToConsole("拒绝访问，父级目录正在被使用");
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
		m_VirtualDisk->LogMsgToConsole("目标节点不能是文件节点 或者不存在");
		return false;
	}

	//在目标文件夹下存在冲突文件
	string filename = from.FileName();
	if (!m_VirtualDisk->GetNodeByPath(to)->FilterSubNode(filename).empty())
	{
		conflict = true;
	}

	needAsk = !(cmdParaCollection.m_parsedOptions.find("/y") != cmdParaCollection.m_parsedOptions.end());


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
			m_VirtualDisk->LogMsgToConsole("找不到路径");
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
				m_VirtualDisk->LogMsgToConsole("拒绝访问，父级目录正在被使用");
				return;
			}
			cur = cur->GetParent();
		}
		//移动该节点
		if (conflict && needAsk)
		{
			while (true)
			{
				//询问是否移动
				string answer = m_VirtualDisk->AskForUserInput(StrProcess::sstr("覆盖 %s 吗? (Yes/No/All):", to.str().c_str()));
				string result;

				transform(answer.begin(), answer.end(), answer.begin(), tolower);
				if (answer == "y" || answer == "yes")
				{
					//移动
					from->RemoveFromPreSubList(false);
					dstNode->AddSubNode(from);
					break;
				}
				else if (answer == "n" || answer == "no")
				{
					return;
				}
				else if (answer == "all")
				{
					needAsk = false;
					//移动文件
					from->RemoveFromPreSubList(false);
					dstNode->AddSubNode(from);
					break;
				}
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