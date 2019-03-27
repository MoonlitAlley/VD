#include "CellNode.h"

CellNode::CellNode()
{
	nodeType = FOLD;
	preLink = NULL;
	UpdateLastModifyTime();
}

//�ڵ�ǰ�ڵ�ɾ��ʱ��ɾ���������е�ÿ���ڵ�
CellNode::~CellNode()
{
	list<CellNode*>::iterator itera = subCellNodeList.begin();
	for (itera; itera != subCellNodeList.end(); itera++)
	{
		delete *itera;
	}
	subCellNodeList.clear();
}

//��ǰ�ڵ���ӽڵ������в���Ԫ��
bool CellNode::AddSubNode(CellNode* node)
{
	//����Ƿ������ӣ��������������ֻ����target;
	if (nodeType == LINK)
	{
		return false;
	}

	if (&node != NULL)
	{
		node->preLink = this;
		subCellNodeList.push_back(node);
		//���µ�ǰ�ڵ���޸�ʱ��
		UpdateLastModifyTime();
		return true;
	}
	return false;
}

//�ӵ�ǰ�����ӽڵ����Ƴ��ض��ڵ㣬����ѡ���Ƿ��ͷŴ˽ڵ�
bool CellNode::RemoveSubNode(CellNode * node, bool release)
{
	auto new_end = remove(subCellNodeList.begin(), subCellNodeList.end(), node);

	if (new_end == subCellNodeList.end())
	{
		return true;
	}
	node->preLink = NULL;
	subCellNodeList.erase(new_end, subCellNodeList.end());
	if (release)
	{
		delete node;
	}
	UpdateLastModifyTime();
	return false;
}

void CellNode::RemoveFromPreSubList(bool release)
{
	if (preLink == NULL)
	{
		return;
	}
	preLink->RemoveSubNode(this, release);
}

list<CellNode*> CellNode::FilterSubNode(string whildcard)
{
	list<CellNode*> res;
	copy_if(subCellNodeList.begin(), subCellNodeList.end(), back_inserter(res), [&](CellNode* node) {return Tools::IsWildcardMatch(whildcard, node->GetCellName()); });
	return res;
}

CellNode * CellNode::GetParent()
{
	return preLink;
}

vector<char> CellNode::Content()
{
	return content;
}

//�ҵ�ָ���ڵ�
CellNode * CellNode::GetNode(const Path & path)
{
	vector<string> items = path.split();

	CellNode* curNode = this;

	for (size_t i = 0; i < items.size(); i++)
	{
		if (items[i] == ".")
			continue;
		if (items[i] == "..")
		{
			curNode = curNode->preLink;
			continue;
		}

		auto target = find_if(curNode->subCellNodeList.begin(), curNode->subCellNodeList.end(), [&](CellNode* child) {return child->GetCellName() == items[i]; });
		//���صĵ������Ƿ�Ϊ��βԪ��
		curNode = target == curNode->subCellNodeList.end() ? NULL : *target;
	}
	return curNode;
}

void CellNode::SetNodeType(FileNodeType type)
{
	nodeType = type;
}

FileNodeType CellNode::GetNodeType()
{
	return nodeType;
}

string CellNode::GetNodeTypeStr()
{
	if (nodeType == LINK)
	{
		return "<LINK>";
	}
	else if (nodeType == FOLD)
	{
		return "<FOLD>";
	}
	else
	{
		return "";
	}
}

void CellNode::SetCellName(string name)
{
	cellName = name;
}

void CellNode::UpdateLastModifyTime()
{
	time(&lastModifyTime);
}

time_t CellNode::GetlastModifyTime()
{
	return lastModifyTime;
}

Path CellNode::GetNodePath()
{
	list<CellNode*> nodePath;
	CellNode* cur = this;
	while (cur)
	{
		nodePath.push_front(cur);
		cur = cur->GetParent();
	}
	Path ret;
	for (auto it = nodePath.begin(); it != nodePath.end(); it++)
	{
		ret.append((*it)->GetCellName());
	}
	return ret;
}

bool CellNode::IsRootNode()
{
	return preLink == NULL && cellName == "/";
}

string CellNode::Target()
{
	if (nodeType&LINK)
	{
		return string(content.begin(), content.end());
	}
	return "";
}

string CellNode::GetCellName()
{
	return cellName;
}
