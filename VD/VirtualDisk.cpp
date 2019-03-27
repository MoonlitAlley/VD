#include "VirtualDisk.h"
#include <iostream>
VirtualDisk::VirtualDisk()
{
	
}

VirtualDisk::~VirtualDisk()
{
	delete rootNode;
}

bool VirtualDisk::InitFileSystem()
{
	rootNode = new CellNode();
	rootNode->SetCellName("/");
	rootNode->SetNodeType(FOLD);
	workingNode = rootNode;
	return true;
}

void VirtualDisk::Execute(Command * cmd)
{
	cmd->Execute(this);
}

CellNode * VirtualDisk::GetNodeByPath(const Path & path)
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



CellNode * VirtualDisk::LookingTarget(CellNode * node)
{
	if (!node)
	{
		return NULL;
	}
	if(node->GetNodeType() & LINK)
	{
		Path targetPath = string(node->Content().begin(), node->Content().end());
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

//该位置寻路可能出错,判断方式不知是否正确，
CellNode * VirtualDisk::LookingForTaget(CellNode * node)
{
	cout << "使用一次可能出错的寻路 ：CellNode * VirtualDisk::LookingForTaget(CellNode * node)" << endl;

	CellNode* target = LookingTarget(node);
	while (target && (target->GetNodeType() & LINK))
	{
		target = LookingTarget(target);
	}
	return target;
}

void VirtualDisk::LogMsgToConsole(string msg)
{
	cout << msg << endl;
}



CellNode * VirtualDisk::GetRootNode()
{
	return rootNode;
}
void VirtualDisk::SetRootNode(CellNode* rootNodeSet)
{
	rootNode = rootNodeSet;
}

CellNode * VirtualDisk::GetWorkingNode()
{
	return workingNode;
}

void VirtualDisk::SetWorkingNode(CellNode * node)
{
	workingNode = node;
}


Path VirtualDisk::GetWorkingPath()
{
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


string VirtualDisk::GetWorkingPathString()
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

bool VirtualDisk::IfNodeBeUsing(CellNode * node)
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
