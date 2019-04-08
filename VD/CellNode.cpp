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

//在当前节点删除时，删除其链表中的每个节点
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

//向当前节点的子节点链表中插入元素
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
//从当前结点的子节点中移除特定节点，可以选择是否释放此节点
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
	// TODO: 在此处插入 return 语句
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
	//在每一条有效数据前设置关卡
	//节点类型信息
	int guard = 0;
	output.write((char*)&guard, sizeof(guard));
	output.write((char*)&self.nodeType, sizeof(FileNodeType));

	//修改时间信息
	guard = 0;
	output.write((char*)&guard, sizeof(guard));
	output.write((char*)&self.lastModifyTime, sizeof(time_t));


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
	//节点类型信息 调整读取顺序，为了能根据类型创建相应的节点
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
		
		//节点类型信息
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
			//将读取到的子节点添加到当前结点的孩子列表中
			self.AddSubNode(node1);
		}
		
	}
	//循环读取
	return input;
}
