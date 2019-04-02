#include "VirtualDiskInside.h"
#include <iostream>

//����-����
VirtualDiskInside::VirtualDiskInside()
{
	rootNode = NULL;
	workingNode = NULL;
}

VirtualDiskInside::~VirtualDiskInside()
{
	delete rootNode;
	rootNode = NULL;
	workingNode = NULL;
}


//��ʽ��-���ڵ㴴��
bool VirtualDiskInside::InitFileSystem()
{
	rootNode = new CellNode();
	rootNode->SetCellName("/");
	rootNode->SetNodeType(FOLD);
	workingNode = rootNode;
	return true;
}

//����ִ��
bool VirtualDiskInside::Execute(Command * cmd)
{
	if (cmd != NULL)
	{
		return cmd->Execute(this);
	}
	else
	{
		return false;
	}
	
}

//Ѱ·
CellNode * VirtualDiskInside::GetNodeByPath(const Path & path)
{
	if (path.IsAbsolute())
	{
		return rootNode->GetNode(path);
	}
	else
	{
		return workingNode->GetNode(path);
	}
}

CellNode * VirtualDiskInside::LookingTarget(CellNode * node)
{
	if (!node)
	{
		return NULL;
	}
	if(node->GetNodeType() & LINK)
	{
		vector<char> temp = node->Content();
		string tempStrPath = string(temp.begin(), temp.end());
		Path targetPath(tempStrPath);
		CellNode* target = GetNodeByPath(targetPath);
		if (!target)
		{
			return NULL;
		}
		else if ((target->GetNodeType() & FILE_CUSTOM) && (!(node->GetNodeType() & FILE_CUSTOM)))
		{
			return NULL;
		}
		else if ((target->GetNodeType() & FOLD) && (!(node->GetNodeType() & FOLD)))
		{
			return NULL;
		}
		else
		{
			return target;
		}
	}
	else
	{
		return node;
	}
}
//��λ��Ѱ·���ܳ���,�жϷ�ʽ��֪�Ƿ���ȷ��
CellNode * VirtualDiskInside::LookingForTaget(CellNode * node)
{
	CellNode* target = LookingTarget(node);
	while (target && (target->GetNodeType() & LINK))
	{
		target = LookingTarget(target);
	}
	return target;
}


//�û�����
void VirtualDiskInside::LogMsgToConsole(string msg)
{
	cout << msg << endl;
}
string VirtualDiskInside::AskForUserInput(const string & output)
{
	cout << output << ":";
	string answer;
	cin >> answer;
	return answer;
}


//GET-SET
CellNode * VirtualDiskInside::GetRootNode()
{
	return rootNode;
}
void VirtualDiskInside::SetRootNode(CellNode* rootNodeSet)
{
	rootNode = rootNodeSet;
}

CellNode * VirtualDiskInside::GetWorkingNode()
{
	return workingNode;
}
void VirtualDiskInside::SetWorkingNode(CellNode * node)
{
	if (node == rootNode)
	{
		workingPath_CD = Path("c:/");
	}
	workingNode = node;
}


Path VirtualDiskInside::GetWorkingPath()
{
	workingPath.clear();
	CellNode* cur = workingNode;
	while (cur)
	{
		workingPath.push_front(cur);
		cur = cur->GetParent();
	}
	Path ret;
	for (auto it = workingPath.begin(); it != workingPath.end(); it++)
	{
		ret.append((*it)->GetCellName());
	}
	return ret;
}
string VirtualDiskInside::GetWorkingPathString()
{
	workingPath.clear();
	CellNode* cur = workingNode;
	while (cur)
	{
		workingPath.push_front(cur);
		cur = cur->GetParent();
	}
	Path ret;
	for (auto it = workingPath.begin(); it != workingPath.end(); it++)
	{
		ret.append((*it)->GetCellName());
	}
	return ret.str();
}

bool VirtualDiskInside::IfNodeBeUsing(CellNode * node)
{
	CellNode* cur = workingNode;
	while (cur)
	{
		if (cur == node)
		{
			return true;
		}
		cur = cur->GetParent();
	}
	return false;
}
