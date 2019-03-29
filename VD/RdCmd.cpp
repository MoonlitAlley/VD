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

		if (node->GetNodeType()&FILE_CUSTOM && node->GetNodeType()&LINK)
		{
			//Ŀ���ļ������ļ���
			m_VirtualDisk->LogMsgToConsole("Ŀ���ļ������ļ���");
		}
		else if (node->GetNodeType()&FOLD && node->GetNodeType()&LINK)
		{
			//�Ӹ����ڵ���ɾ��
			m_VirtualDisk->LogMsgToConsole("�˴����������Ŀ���ļ����Ƿ�Ϊ�գ�ֱ��ɾ��");
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