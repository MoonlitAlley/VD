#include "CellNode.h"

CellNode::CellNode()
{
	nodeType = FOLD;
	preLink = NULL;
	UpdateLastModifyTime();
}

//在当前节点删除时，删除其链表中的每个节点
CellNode::~CellNode()
{
	list<CellNode*>::iterator itera = subCellNodeList.begin();
	for (itera; itera != subCellNodeList.end(); itera++)
	{
		delete *itera;
	}
	subCellNodeList.clear();
}

//向当前节点的子节点链表中插入元素
bool CellNode::AddSubNode(CellNode* node)
{
	//如果是符号链接，，不允许有子项，只能有target;
	if (nodeType == LINK)
	{
		return false;
	}

	if (&node != NULL)
	{
		node->preLink = this;
		subCellNodeList.push_back(node);
		//更新当前节点的修改时间
		UpdateLastModifyTime();
		return true;
	}
	return false;
}

//从当前结点的子节点中移除特定节点，可以选择是否释放此节点
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

vector<char>& CellNode::Content()
{
	return content;
}

//找到指定节点
CellNode * CellNode::GetNode(const Path & path)
{
	vector<string> items = path.split();

	CellNode* curNode = this;

	for (size_t i = 0; i < items.size(); i++)
	{

		if (!curNode)
		{
			return NULL;
		}
		if (items[i] == ".")
			continue;
		if (items[i] == "..")
		{
			curNode = curNode->preLink;
			continue;
		}
		if (curNode->GetNodeType()&FileNodeType::LINK)
		{
			return curNode;
		}
		auto target = find_if(curNode->subCellNodeList.begin(), curNode->subCellNodeList.end(), [&](CellNode* child) {return child->GetCellName() == items[i]; });
		//返回的迭代器是否为超尾元素
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

void CellNode::SetLastModifyTime(time_t value)
{
	lastModifyTime = value;
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
	
	return preLink == NULL && (cellName == "/" || cellName =="C:" || cellName == "c:");
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


ostream & operator<<(ostream & output, CellNode & self)
{
	//在每一条有效数据前设置关卡

	//修改时间信息
	int guard = 0;
	output.write((char*)&guard, sizeof(guard));
	output.write((char*)&self.lastModifyTime, sizeof(time_t));

	//节点类型信息
	guard = 0;
	output.write((char*)&guard, sizeof(guard));
	output.write((char*)&self.nodeType, sizeof(FileNodeType));

	//文件内容信息
	guard = 0;
	size_t count = self.content.size();
	output.write((char*)&guard, sizeof(guard));
	output.write((char*)&count, sizeof(size_t));
	if (self.content.size() > 0)
	{
		//将节点内容写入到文件中，写入大小为内容总大小
		output.write(&self.content.front(), self.content.size());
	}

	//节点名称信息
	guard = 0;
	count = self.GetCellName().size();
	output.write((char*)&guard, sizeof(guard));
	output.write((char*)&count, sizeof(size_t));
	output.write(&self.cellName.front(), count);


	//子节点信息
	guard = 0;
	count = self.subCellNodeList.size();
	output.write((char*)&guard, sizeof(guard));
	output.write((char*)&count, sizeof(size_t));

	for (auto itera = self.subCellNodeList.begin(); itera != self.subCellNodeList.end(); itera++)
	{
		CellNode* node = *itera;
		output << *node;
	}
	//循环输出
	return output;
}

istream & operator>>(istream & input, CellNode & self)
{
	// TODO: 在此处插入 return 语句

	//修改时间信息
	int guard;
	input.read((char*)&guard, sizeof(guard));
	if (guard == 0)
	{
		input.read((char*)&self.lastModifyTime, sizeof(time_t));
	}


	//节点类型信息
	input.read((char*)&guard, sizeof(guard));
	if (guard == 0)
	{
		input.read((char*)&self.nodeType, sizeof(FileNodeType));
	}
	
	size_t count =0;
	//读取节点内容信息
	input.read((char*)&guard, sizeof(guard));
	if (guard == 0)
	{
		input.read((char*)&count, sizeof(size_t));
	}
	if (count > 0)
	{
		self.content.resize(count);
		input.read(&self.content.front(), count);
	}


	//节点名称信息
	count = 0;
	input.read((char*)&guard, sizeof(guard));
	if (guard == 0)
	{
		input.read((char*)&count, sizeof(size_t));
	}
	if (count > 0)
	{
		self.cellName.resize(count);
		input.read(&self.cellName.front(), count);
	}

	//子节点信息
	count = 0;
	input.read((char*)&guard, sizeof(guard));
	if (guard == 0)
	{
		input.read((char*)&count, sizeof(size_t));
	}
	for (size_t i = 0; i < count; i++)
	{
		CellNode* node = new CellNode();
		input >> *node;
		if (node->cellName.empty())
		{
			return input;
		}
		self.AddSubNode(node);
	}
	//循环读取
	return input;
}
