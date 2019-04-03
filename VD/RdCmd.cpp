#include "RdCmd.h"
#include "VirtualDiskInside.h"

bool RdCmd::Execute(VirtualDiskInside * virtualdisk)
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

		//���ȹ��˲����ڵ���Ŀ
		CellNode* tempNode = m_VirtualDisk->GetNodeByPath(pathItems[i]);

		//NEW
		//CellNode* tempNode = GetDstNodeByPath(pathItems[i]);

		if (!tempNode)
		{
			//û����Ӧ���ļ�
			continue;
		}
		else if (m_VirtualDisk->IfNodeBeUsing(tempNode))
		{
			//�ļ�����ʹ��
			m_VirtualDisk->LogMsgToConsole("Ŀ���ļ���������·��");
			continue;
		}
		else
		{
			node = tempNode;
		}

		//��ʼ����
		if (wildcard.empty())
		{
			wildcard = "*";
		}
		//node������Ϊ��
		if (node->GetNodeType()&FILE_CUSTOM && node->GetNodeType()&LINK)
		{
			//Ŀ���ļ������ļ���
			m_VirtualDisk->LogMsgToConsole("Ŀ���ļ������ļ���");
		}
		else if (node->GetNodeType()&FOLD && node->GetNodeType()&LINK)
		{
			//�Ӹ����ڵ���ɾ��
			//m_VirtualDisk->LogMsgToConsole("�˴����������Ŀ���ļ����Ƿ�Ϊ�գ�ֱ��ɾ��");
			node->RemoveFromPreSubList(true);
		}
		else if (node->GetNodeType()&FILE_CUSTOM)
		{
			m_VirtualDisk->LogMsgToConsole("Ŀ���ļ������ļ���");
			//Ŀ���ļ������ļ���
		}
		else if (node->GetNodeType()&FOLD)
		{
			//���ָ����Ŀ¼�ǿգ����û�û������ݹ�ɾ����ָ������ִ�и�·��
			if (!(cmdParaCollection.m_parsedOptions.find("/s") != cmdParaCollection.m_parsedOptions.end()) && !node->FilterSubNode("*").empty())
			{
				//Ŀ¼���ǿյ�
				m_VirtualDisk->LogMsgToConsole("Ŀ¼���ǿյ�");
				continue;
			}
			node->RemoveFromPreSubList(true);
		}
		else
		{
			m_VirtualDisk->LogMsgToConsole("���������Ϣ����ӡ��˵��������������������ϸ�ж������bool RdCmd::Execute");
		}
	}
	return true;
}

//�˷�����ʱ���� - rd��֧�ַ������ӻ��߲���ɾ��Ŀ¼��Ϊ�յ��ļ���
CellNode* RdCmd::GetDstNodeByPath(const Path &to)
{

	CellNode* targetNode = NULL;
	CellNode* curNode = m_VirtualDisk->GetNodeByPath(to.StartNode());
	curNode = m_VirtualDisk->LookingForTaget(curNode);
	vector<string> path = to.split();

	for (size_t i = 0; i < path.size(); i++)
	{
		targetNode = curNode->GetNode(path[i]);
		targetNode = m_VirtualDisk->LookingForTaget(targetNode);

		/*if (++i < path.size())
		{
			--i;
			targetNode = m_VirtualDisk->LookingForTaget(targetNode);
		}*/

		if (!targetNode)
		{
			//m_VirtualDisk->LogMsgToConsole("·��������");
			return NULL;
		}
		else if (targetNode->GetNodeType()& FOLD)
		{
			curNode = targetNode;
		}
		else if (targetNode->GetNodeType()& FILE_CUSTOM)
		{
			//��һ���ļ�,�ж��Ƿ������սڵ�
			if (++i < path.size())
			{
				--i;
				m_VirtualDisk->LogMsgToConsole("·���в�Ӧ�ó����ļ�");
				return NULL;
			}
		}
	}
	return targetNode;
}