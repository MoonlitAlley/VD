#include "DelCmd.h"
#include "Path.h"
#include "Common.h"
#include "VirtualDiskInside.h"
#include "MyString.h"


bool DelCmd::Execute(VirtualDiskInside * virtualdisk)
{
	m_VirtualDisk = virtualdisk;
	vector<string> pathItems = cmdParaCollection.m_pathItems;
	if (pathItems.empty())
	{
		return false;
		//·���ǿյ�
		//�����﷨����ȷ������һ��·����path���Ѿ������ж�
	}
	//��ÿ��·���ֱ���
	for (size_t i = 0; i < pathItems.size(); i++)
	{
		CellNode* node = NULL;
		string wildcard;
		Path tempPath(pathItems[i]);

		//���ȹ��˲����ڵ���Ŀ

		if (Tools::isWildcard(tempPath.split().back()))
		{
			wildcard = tempPath.split().back();

			CellNode* tempNode = m_VirtualDisk->GetNodeByPath(tempPath.Self().append(".."));
			if (!tempNode)
			{
				//û����Ӧ�ĸ����ļ�
				continue;
			}
			else if (!(tempNode->GetNodeType()&FOLD))
			{
				//û���ҵ���Ӧ���ļ�
				continue;
			}
			node = tempNode;
		}
		else
		{
			node = m_VirtualDisk->GetNodeByPath(tempPath);
			if (!node)
			{
				//û���ҵ���Ӧ���ļ�
				continue;
			}
		}

		//��ʼ����
		if (wildcard.empty())
		{
			wildcard = "*";
		}
		DisPatchPathItem(node, wildcard);
	}

	return true;
}


void DelCmd::DisPatchPathItem(CellNode * node, const string & wildcard)
{
	if (node->GetNodeType()&FILE_CUSTOM && node->GetNodeType()&LINK)
	{
		RemoveFoldLinkFile(node->GetParent(), m_VirtualDisk->LookingForTaget(node->GetParent()), node->GetCellName());
	}
	else if (node->GetNodeType()&FOLD && node->GetNodeType()&LINK)
	{
		CellNode* target = m_VirtualDisk->LookingForTaget(node);
		if (!target)
		{
			m_VirtualDisk->LogMsgToConsole("�Ҳ����ļ�");
		}
		else
		{
			RemoveFoldLinkFile(target, target, wildcard);
		}
	}
	else if (node->GetNodeType()&FILE_CUSTOM)
	{
		RemoveFoldLinkFile(node->GetParent(), node->GetParent(), node->GetCellName());
	}
	else if (node->GetNodeType()&FOLD)
	{
		RemoveFoldLinkFile(node, node, wildcard);
	}
	else
	{
		m_VirtualDisk->LogMsgToConsole("���������Ϣ����ӡ��˵��������������������ϸ�ж������bool DelCmd::Execute");
		return;
	}
}

void DelCmd::RemoveFoldLinkFile(CellNode * fold, CellNode * deep, const string & filesName)
{
	//�Ƿ���ƥ�������ļ�
	if (filesName == "*" || filesName == "*.*")
	{
		while (true)
		{
			//ѯ���û��Ƿ�ɾ����ǰ·���µı�������
			//��Virtual Disk�������
			string answer = m_VirtualDisk->AskForUserInput(StrProcess::sstr("%s, �Ƿ�ȷ��(Y/N)?", fold->GetNodePath().str().c_str()));

			string result;

			transform(answer.begin(), answer.end(), answer.begin(), tolower);
			if (answer == "y" || answer == "yes")
			{
				break;
			}
			else if (answer == "n" || answer == "no")
			{
				return;
			}
		}
	}

	list<CellNode*> children = deep->FilterSubNode(filesName);
	for (auto itera = children.begin(); itera != children.end(); itera++)
	{
		if ((*itera)->GetNodeType()&FILE_CUSTOM)
		{
			//����û���ѭ��ɾ���Ĳ�������ɾ��ʱ���ѵ�ǰ��������
			if (cmdParaCollection.m_parsedOptions.find("/s") != cmdParaCollection.m_parsedOptions.end())
			{
				m_VirtualDisk->LogMsgToConsole(StrProcess::sstr("ɾ���ļ� - %s", (*itera)->GetNodePath().str().c_str()));
				//ɾ������ڵ�
			}
			//�޷�ȷ��ɾ���ĵ�֮���Ƿ��Ӱ������list
			(*itera)->RemoveFromPreSubList(true);
			//ɾ��������ڵ�
		}
	}

	//�ж��Ƿ�ѭ�������ӽڵ�
	if (cmdParaCollection.m_parsedOptions.find("/s") != cmdParaCollection.m_parsedOptions.end())
	{
		list<CellNode*> children = deep->FilterSubNode("*");
		for (auto itera = children.begin(); itera != children.end(); itera++)
		{
			if ((*itera)->GetNodeType()&FOLD)
			{
				DisPatchPathItem(*itera, filesName);
			}
		}
	}
}
