#include "MoveCmd.h"
#include "Path.h"
#include "VirtualDiskInside.h"
#include "MyString.h"
bool MoveCmd::Execute(VirtualDiskInside * virtualdisk)
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
		m_VirtualDisk->LogMsgToConsole("��֧����ʵ·��");
		return false;
	}
	if (!Tools::IsLegalPathName(to.split().back()))
	{
		m_VirtualDisk->LogMsgToConsole("Ŀ¼������ȷ");
		return false;
	}


	list<CellNode*> srcNodeList;
	if (Tools::isWildcard(from.split().back()))
	{
		//����ͨ���
		CellNode* prelink = m_VirtualDisk->GetNodeByPath(from.Parent());
		srcNodeList = prelink->FilterSubNode(from.split().back());

	}
	else
	{
		CellNode* node = m_VirtualDisk->GetNodeByPath(from);
		if (!node)
		{
			m_VirtualDisk->LogMsgToConsole("�ļ�������");
			return false;
		}
		if (m_VirtualDisk->IfNodeBeUsing(node))
		{
			m_VirtualDisk->LogMsgToConsole("�ܾ����ʣ�����Ŀ¼���ڱ�ʹ��");
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
		m_VirtualDisk->LogMsgToConsole("Ŀ��ڵ㲻�����ļ��ڵ� ���߲�����");
		return false;
	}

	//��Ŀ���ļ����´��ڳ�ͻ�ļ�
	string filename = from.FileName();
	if (!m_VirtualDisk->GetNodeByPath(to)->FilterSubNode(filename).empty())
	{
		conflict = true;
	}

	needAsk = !(cmdParaCollection.m_parsedOptions.find("/y") != cmdParaCollection.m_parsedOptions.end());


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
			m_VirtualDisk->LogMsgToConsole("�Ҳ���·��");
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
				m_VirtualDisk->LogMsgToConsole("�ܾ����ʣ�����Ŀ¼���ڱ�ʹ��");
				return;
			}
			cur = cur->GetParent();
		}
		//�ƶ��ýڵ�
		if (conflict && needAsk)
		{
			while (true)
			{
				//ѯ���Ƿ��ƶ�
				string answer = m_VirtualDisk->AskForUserInput(StrProcess::sstr("���� %s ��? (Yes/No/All):", to.str().c_str()));
				string result;

				transform(answer.begin(), answer.end(), answer.begin(), tolower);
				if (answer == "y" || answer == "yes")
				{
					//�ƶ�
					from->RemoveFromPreSubList(false);
					dstNode->AddSubNode(from);
					break;
				}
				else if (answer == "n" || answer == "no")
				{
					return;
				}
				else if (answer == "all")
				{
					needAsk = false;
					//�ƶ��ļ�
					from->RemoveFromPreSubList(false);
					dstNode->AddSubNode(from);
					break;
				}
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