#include "CellNode.h"
#include "FoldNode.h"
#include "LinkNode.h"
#include "FileNode.h"
#include <assert.h>
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
	preLink = NULL;
}

//��ǰ�ڵ���ӽڵ������в���Ԫ��
bool CellNode::AddSubNode(CellNode* node)
{
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
//�ӵ�ǰ�����ӽڵ����Ƴ��ض��ڵ㣬����ѡ���Ƿ��ͷŴ˽ڵ�
bool CellNode::RemoveSubNode(CellNode * node, bool release)
{
	return false;
}


list<CellNode*> CellNode::FilterSubNode(const string& whildcard)
{
	list<CellNode*> res;
	copy_if(subCellNodeList.begin(), subCellNodeList.end(), back_inserter(res), [&](CellNode* node) {return Tools::IsWildcardMatch(whildcard, node->GetCellName()); });
	return res;
}

CellNode * CellNode::GetParent()
{
	return preLink;
}

void CellNode::SetParent(CellNode* pre)
{
	preLink = pre;
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

void CellNode::SetNodeType(const FileNodeType type)
{
	nodeType = type;
}

FileNodeType CellNode::GetNodeType()
{
	return nodeType;
}

string CellNode::GetNodeTypeStr()
{
	return "";
}

void CellNode::SetCellName(const string& name)
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

void CellNode::SetLastModifyTime(const time_t& value)
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

vector<char>& CellNode::Content()
{
	assert(false);
	vector<char> a;
	return a;
	// TODO: �ڴ˴����� return ���
}

string CellNode::GetTargetNodePath()
{
	return string();
}

void CellNode::SetTargetNodePath(const string& TargetPath)
{
}

string CellNode::GetCellName()
{
	return cellName;
}


ostream & operator<<(ostream & output, CellNode & self)
{
	//��ÿһ����Ч����ǰ���ùؿ�
	//�ڵ�������Ϣ
	int guard = 0;
	output.write((char*)&guard, sizeof(guard));
	output.write((char*)&self.nodeType, sizeof(FileNodeType));

	//�޸�ʱ����Ϣ
	guard = 0;
	output.write((char*)&guard, sizeof(guard));
	output.write((char*)&self.lastModifyTime, sizeof(time_t));


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
	//�ڵ�������Ϣ ������ȡ˳��Ϊ���ܸ������ʹ�����Ӧ�Ľڵ�
	/*input.read((char*)&guard, sizeof(guard));
	if (guard == 0)
	{
		input.read((char*)&self.nodeType, sizeof(FileNodeType));
	}
*/

	input.read((char*)&guard, sizeof(guard));
	if (guard == 0)
	{
		input.read((char*)&self.lastModifyTime, sizeof(time_t));
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
		
		//�ڵ�������Ϣ
		int tempnodetype =0;
		CellNode* node1 = NULL;
		input.read((char*)&guard, sizeof(guard));

		if (guard == 0)
		{
			input.read((char*)&tempnodetype, sizeof(FileNodeType));
			if (tempnodetype != 0)
			{
				if (tempnodetype == FOLD)
				{
					node1 = new FoldNode();
				}
				else if (tempnodetype == FILE_CUSTOM)
				{
					node1 = new FileNode();
				}
				else if (tempnodetype& LINK)
				{
					node1 = new LinkNode();
					node1->SetNodeType((FileNodeType)tempnodetype);
				}
				else
				{
					node1 = new CellNode();
				}
			}
			input >> *node1;
			if (node1->cellName.empty())
			{
				return input;
			}
			//����ȡ�����ӽڵ���ӵ���ǰ���ĺ����б���
			self.AddSubNode(node1);
		}
		
	}
	//ѭ����ȡ
	return input;
}
