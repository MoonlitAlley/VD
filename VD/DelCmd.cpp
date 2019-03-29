#include "DelCmd.h"
#include "Path.h"
#include "Common.h"
#include "VirtualDiskInside.h"
#include "MyString.h"


bool DelCmd::Execute(VirtualDiskInside * virtualdisk)
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
		Path tempPath(pathItems[i]);

		//首先过滤不存在的项目

		if (Tools::isWildcard(tempPath.split().back()))
		{
			wildcard = tempPath.split().back();

			CellNode* tempNode = m_VirtualDisk->GetNodeByPath(tempPath.Self().append(".."));
			if (!tempNode)
			{
				//没有相应的父级文件
				continue;
			}
			else if (!(tempNode->GetNodeType()&FOLD))
			{
				//没有找到对应的文件
				continue;
			}
			node = tempNode;
		}
		else
		{
			node = m_VirtualDisk->GetNodeByPath(tempPath);
			if (!node)
			{
				//没有找到对应的文件
				continue;
			}
		}

		//开始处理
		if (wildcard.empty())
		{
			wildcard = "*";
		}
		DisPatchPathItem(node, wildcard);
	}

	return true;
}


void DelCmd::DisPatchPathItem(CellNode * node, const string & wildcard)
{
	if (node->GetNodeType()&FILE_CUSTOM && node->GetNodeType()&LINK)
	{
		RemoveFoldLinkFile(node->GetParent(), m_VirtualDisk->LookingForTaget(node->GetParent()), node->GetCellName());
	}
	else if (node->GetNodeType()&FOLD && node->GetNodeType()&LINK)
	{
		CellNode* target = m_VirtualDisk->LookingForTaget(node);
		if (!target)
		{
			m_VirtualDisk->LogMsgToConsole("找不到文件");
		}
		else
		{
			RemoveFoldLinkFile(target, target, wildcard);
		}
	}
	else if (node->GetNodeType()&FILE_CUSTOM)
	{
		RemoveFoldLinkFile(node->GetParent(), node->GetParent(), node->GetCellName());
	}
	else if (node->GetNodeType()&FOLD)
	{
		RemoveFoldLinkFile(node, node, wildcard);
	}
	else
	{
		m_VirtualDisk->LogMsgToConsole("如果此条信息被打印，说明程序存在意外情况，详细判断请见：bool DelCmd::Execute");
		return;
	}
}

void DelCmd::RemoveFoldLinkFile(CellNode * fold, CellNode * deep, const string & filesName)
{
	//是否是匹配所有文件
	if (filesName == "*" || filesName == "*.*")
	{
		while (true)
		{
			//询问用户是否删除当前路径下的本项内容
			//向Virtual Disk请求操作
			string answer = m_VirtualDisk->AskForUserInput(StrProcess::sstr("%s, 是否确认(Y/N)?", fold->GetNodePath().str().c_str()));

			string result;

			transform(answer.begin(), answer.end(), answer.begin(), tolower);
			if (answer == "y" || answer == "yes")
			{
				break;
			}
			else if (answer == "n" || answer == "no")
			{
				return;
			}
		}
	}

	list<CellNode*> children = deep->FilterSubNode(filesName);
	for (auto itera = children.begin(); itera != children.end(); itera++)
	{
		if ((*itera)->GetNodeType()&FILE_CUSTOM)
		{
			//如果用户有循环删除的参数，则删除时提醒当前操作对象
			if (cmdParaCollection.m_parsedOptions.find("/s") != cmdParaCollection.m_parsedOptions.end())
			{
				m_VirtualDisk->LogMsgToConsole(StrProcess::sstr("删除文件 - %s", (*itera)->GetNodePath().str().c_str()));
				//删除这个节点
			}
			//无法确定删除改点之后是否会影响整个list
			(*itera)->RemoveFromPreSubList(true);
			//删除掉这个节点
		}
	}

	//判断是否循环处理子节点
	if (cmdParaCollection.m_parsedOptions.find("/s") != cmdParaCollection.m_parsedOptions.end())
	{
		list<CellNode*> children = deep->FilterSubNode("*");
		for (auto itera = children.begin(); itera != children.end(); itera++)
		{
			if ((*itera)->GetNodeType()&FOLD)
			{
				DisPatchPathItem(*itera, filesName);
			}
		}
	}
}
