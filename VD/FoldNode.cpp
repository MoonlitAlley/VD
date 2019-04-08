#include "FoldNode.h"

bool FoldNode::AddSubNode(CellNode * node)
{
	//如果是符号链接，，不允许有子项，只能有target;
	if (nodeType == LINK)
	{
		return false;
	}

	if (&node != NULL)
	{
		node->SetParent(this);
		//node->GetParent() = this;
		subCellNodeList.push_back(node);
		//更新当前节点的修改时间
		UpdateLastModifyTime();
		return true;
	}
	return false;
}

bool FoldNode::RemoveSubNode(CellNode * node, bool release)
{
	auto new_end = remove(subCellNodeList.begin(), subCellNodeList.end(), node);

	if (new_end == subCellNodeList.end())
	{
		return true;
	}
	//node->preLink = NULL;
	node->SetParent(NULL);
	subCellNodeList.erase(new_end, subCellNodeList.end());
	if (release)
	{
		delete node;
		node = NULL;
	}
	UpdateLastModifyTime();
	return true;
}

list<CellNode*> FoldNode::FilterSubNode(const string& whildcard)
{
	list<CellNode*> res;
	copy_if(subCellNodeList.begin(), subCellNodeList.end(), back_inserter(res), [&](CellNode* node) {return Tools::IsWildcardMatch(whildcard, node->GetCellName()); });
	return res;
}

string FoldNode::GetNodeTypeStr()
{
	return "<FOLD>";
}
