#include "MoveCmd.h"
#include "Path.h"
#include "VirtualDisk.h"
#include "MyString.h"
bool MoveCmd::Execute(VirtualDisk * virtualdisk)
{
	m_VirtualDisk = virtualdisk;
	
	vector<string> pathItems = cmdParaCollection.m_pathItems;

	//copy��move����֧�ֵ���·����Ĭ�ϵڶ���·��Ϊ��ǰ����·��
	if (pathItems.size() == 1)
	{
		pathItems.push_back(".");
	}
	

	Path from(pathItems[0]);
	Path to(pathItems[1]);

	if (from.IsReal() || to.IsReal())
	{
		//��֧����ʵ·��
		return false;
	}
	if (!Tools::IsLegalPathName(to.split().back()))
	{
		//Ŀ¼������ȷ
		return false;
	}


	list<CellNode*> srcNodeList;
	if (Tools::isWildcard(from.split().back()))
	{
		//����ͨ���
		CellNode* prelink = m_VirtualDisk->GetNodeByPath(from.Parent());
		srcNodeList = prelink->FilterSubNode(from.FileName());

	}
	else
	{
		CellNode* node = m_VirtualDisk->GetNodeByPath(from);
		if (!node)
		{
			//�ļ�������
			return false;
		}
		if (m_VirtualDisk->IfNodeBeUsing(node))
		{
			//·���ǹ���·��
			return false;
		}
		srcNodeList.push_back(node);
	}


	if (srcNodeList.empty())
	{
		//û����Ҫ�������ļ�
		return false;
	}
	if (!m_VirtualDisk->GetNodeByPath(to) || m_VirtualDisk->GetNodeByPath(to)->GetNodeType()&FileNodeType::FILE_CUSTOM)
	{
		//Ŀ��ڵ㲻�����ļ��ڵ� ���߲�����
		return false;
	}

	//��Ŀ���ļ����´��ڳ�ͻ�ļ�
	string filename = from.FileName();
	if (!m_VirtualDisk->GetNodeByPath(to)->FilterSubNode(filename).empty())
	{
		conflict = true;
	}

	for (auto itera = srcNodeList.begin(); itera != srcNodeList.end(); itera++)
	{
		MoveNode(*itera, to);
	}

	m_VirtualDisk->LogMsgToConsole(StrProcess::sstr("�ƶ���%9d ���ļ�", srcNodeList.size()));

	return false;
}


void MoveCmd::MoveNode(CellNode* from, Path& to)
{
	CellNode* dstNode = m_VirtualDisk->GetNodeByPath(to);

	//Ŀ�����ļ��л��߷�������
	if (dstNode->GetNodeType()&FileNodeType::FOLD)
	{
		
		CellNode* node = m_VirtualDisk->LookingForTaget(dstNode);
		if (!node)
		{
			//�Ҳ���·��
			return;
		}
		else if (from == node)
		{
			//��ͬ����ִ��
			return;
		}
		CellNode* cur = node->GetParent();
		while (cur)
		{
			if (cur == from)
			{
				//�ܾ����ʣ�����Ŀ¼���ڱ�ʹ��
				return;
			}
			cur = cur->GetParent();
		}
		//�ƶ��ýڵ�

		if (conflict && needAsk)
		{
			while (true)
			{
				//�ƶ��ļ�
			}
		}
		else
		{
			//�ƶ��ļ�
			from->RemoveFromPreSubList(false);
			dstNode->AddSubNode(from);
		}




	}
}