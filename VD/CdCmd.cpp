#include "CdCmd.h"
#include "VirtualDiskInside.h"



bool CdCmd::Execute(VirtualDiskInside * virtualdiskInside)
{
	//�����������ȡ��·��
	vector<string> pathItem = cmdParaCollection.m_pathItems;

	//���������֧��һ��·��
	if (pathItem.size() > 1)
	{
		return false;
	}
	//������֧�ֿ�·�������·��Ϊ�գ�����ʾ��ǰ·��
	if (pathItem.empty())
	{
		//��ӡ��ǰ·��****************************
		virtualdiskInside->LogMsgToConsole(virtualdiskInside->GetWorkingPath().str());
		return false;
		//������
	}
	else
	{
		Path path(pathItem[0]);
		//����·��ȥѰ����Ӧ�Ľڵ�
		CellNode* node = virtualdiskInside->GetNodeByPath(path);
		node = virtualdiskInside->LookingForTaget(node);
		if (!node)
		{
			virtualdiskInside->LogMsgToConsole("·�������ڻ�Ϊ�ļ���");
			return false;
		}
		else if (node->GetNodeType()& FOLD)
		{
			virtualdiskInside->SetWorkingNode(node);
			return true;
		}
		else
		{
			//·�������ڻ�Ϊ�ļ���
			virtualdiskInside->LogMsgToConsole("·�������ڻ�Ϊ�ļ���");
			return false;
		}
	}
}

#pragma region ��������
//
//bool CdCmd::Execute(VirtualDisk * virtualdisk)
//{
//	//�����������ȡ��·��
//	vector<string> pathItem = cmdParaCollection.m_pathItems;
//
//	//���������֧��һ��·��
//	if (pathItem.size() > 1)
//	{
//		return false;
//	}
//	//������֧�ֿ�·�������·��Ϊ�գ�����ʾ��ǰ·��
//	if (pathItem.empty())
//	{
//		//��ӡ��ǰ·��****************************
//		virtualdisk->LogMsgToConsole(virtualdisk->GetWorkingPath().str());
//		return false;
//		//������
//	}
//	else
//	{
//		Path path(pathItem[0]);
//		//����·��ȥѰ����Ӧ�Ľڵ�
//		//CellNode* node = virtualdisk->GetNodeByPath(path);
//		vector<string> items = path.split();
//		CellNode* curNode = virtualdisk->GetNodeByPath(path.StartNode());
//		for (size_t i = 0; i < items.size(); i++)
//		{
//			if (curNode == NULL)
//			{
//
//				return false;
//			}
//
//			//���ݵ�ǰ�ڵ�����ȥѰ����Ӧ�Ľڵ�
//			CellNode* child = curNode->GetNode(items[i]);
//
//			if (!child)
//			{
//				virtualdisk->LogMsgToConsole("·�������ڻ�Ϊ�ļ���");
//				return false;
//			}
//
//			if (child->GetNodeType() == FOLD || child->GetNodeType() == LINK || child->GetNodeType() == DISK)
//			{
//				//���child������Ϊ�ļ��У���curNode = child ; ������Ϊlink����curNode ���� child��target
//				curNode = virtualdisk->LookingForTaget(child);
//				if (curNode->GetNodeType() == FILE_CUSTOM)
//				{
//					virtualdisk->LogMsgToConsole("·�������ڻ�Ϊ�ļ���");
//					return false;
//				}
//				continue;
//			}
//			else
//			{
//				//·�������ڻ�Ϊ�ļ���
//				virtualdisk->LogMsgToConsole("·�������ڻ�Ϊ�ļ���");
//				return false;
//			}
//		}
//
//		if (curNode == NULL)
//		{
//			virtualdisk->LogMsgToConsole("�Ҳ���·�������ļ�");
//			return false;
//		}
//		else if (curNode->GetNodeType() == FOLD || curNode->GetNodeType() == DISK)
//		{
//			virtualdisk->SetWorkingNode(curNode);
//			return true;
//		}
//		else
//		{
//			virtualdisk->LogMsgToConsole("·��ֻ֧���ļ�����");
//			return false;
//		}
//	}
//
//}

#pragma endregion