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

vector<char>& CellNode::Content()
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
	//��ÿһ����Ч����ǰ���ùؿ�

	//�޸�ʱ����Ϣ
	int guard = 0;
	output.write((char*)&guard, sizeof(guard));
	output.write((char*)&self.lastModifyTime, sizeof(time_t));

	//�ڵ�������Ϣ
	guard = 0;
	output.write((char*)&guard, sizeof(guard));
	output.write((char*)&self.nodeType, sizeof(FileNodeType));

	//�ļ�������Ϣ
	guard = 0;
	size_t count = self.content.size();
	output.write((char*)&guard, sizeof(guard));
	output.write((char*)&count, sizeof(size_t));
	if (self.content.size() > 0)
	{
		//���ڵ�����д�뵽�ļ��У�д���СΪ�����ܴ�С
		output.write(&self.content.front(), self.content.size());
	}

	//�ڵ�������Ϣ
	guard = 0;
	count = self.GetCellName().size();
	output.write((char*)&guard, sizeof(guard));
	output.write((char*)&count, sizeof(size_t));
	output.write(&self.cellName.front(), count);


	//�ӽڵ���Ϣ
	guard = 0;
	count = self.subCellNodeList.size();
	output.write((char*)&guard, sizeof(guard));
	output.write((char*)&count, sizeof(size_t));

	for (auto itera = self.subCellNodeList.begin(); itera != self.subCellNodeList.end(); itera++)
	{
		CellNode* node = *itera;
		output << *node;
	}
	//ѭ�����
	return output;
}

istream & operator>>(istream & input, CellNode & self)
{
	// TODO: �ڴ˴����� return ���

	//�޸�ʱ����Ϣ
	int guard;
	input.read((char*)&guard, sizeof(guard));
	if (guard == 0)
	{
		input.read((char*)&self.lastModifyTime, sizeof(time_t));
	}


	//�ڵ�������Ϣ
	input.read((char*)&guard, sizeof(guard));
	if (guard == 0)
	{
		input.read((char*)&self.nodeType, sizeof(FileNodeType));
	}
	
	size_t count =0;
	//��ȡ�ڵ�������Ϣ
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


	//�ڵ�������Ϣ
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

	//�ӽڵ���Ϣ
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
	//ѭ����ȡ
	return input;
}
