#include "CdCmd.h"
#include "VirtualDiskInside.h"



bool CdCmd::Execute(VirtualDiskInside * virtualdiskInside)
{

	//ADD
	//Path workingPath = virtualdiskInside->workingPath_CD;

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
		//virtualdiskInside->LogMsgToConsole(virtualdiskInside->GetWorkingPath().str());

		//new
		virtualdiskInside->LogMsgToConsole(virtualdiskInside->workingPath_CD.str());
		return false;
		//������
	}
	else
	{
		Path path(pathItem[0]);

		//����ͨ���
		if (Tools::isWildcard(path.split().back()))
		{
			string wildcard = path.split().back();

			vector<string> wildPath = path.Self().append("..").split();

			CellNode* curNode = virtualdiskInside->GetNodeByPath(path.StartNode());

			curNode = virtualdiskInside->LookingForTaget(curNode);

			for (size_t i = 0; i < wildPath.size(); i++)
			{
				CellNode* tempNode = curNode->GetNode(wildPath[i]);
				tempNode = virtualdiskInside->LookingForTaget(tempNode);

				if (!tempNode)
				{
					virtualdiskInside->LogMsgToConsole("·�������ڻ�Ϊ�ļ���");
					return false;
				}
				else if (tempNode->GetNodeType()& FOLD)
				{
					curNode = tempNode;
				}
				else
				{
					//·�������ڻ�Ϊ�ļ���
					virtualdiskInside->LogMsgToConsole("·�������ڻ�Ϊ�ļ���");
					return false;
				}
			}
			//����curNode����
			list<CellNode*> wildNodeList = curNode->FilterSubNode(wildcard);

			if (wildNodeList.empty())
			{
				//Ŀ�겻����
				return false;
			}

			CellNode* targetNode = wildNodeList.front();

			if (!targetNode)
			{
				//Ŀ�겻����
				return false;
			}

			path.append("..").append(targetNode->GetCellName());
			//NEW
			if (path.IsAbsolute())
			{
				virtualdiskInside->workingPath_CD = path;
			}
			virtualdiskInside->workingPath_CD.append(path.str());
			virtualdiskInside->SetWorkingNode(targetNode);
			return true;
		}

		
		vector<string> item = path.split();

		CellNode* curNode = virtualdiskInside->GetNodeByPath(path.StartNode());

		curNode = virtualdiskInside->LookingForTaget(curNode);

		for (size_t i = 0; i < item.size(); i++)
		{
			CellNode* tempNode = curNode->GetNode(item[i]);
			tempNode = virtualdiskInside->LookingForTaget(tempNode);

			if (!tempNode)
			{
				virtualdiskInside->LogMsgToConsole("·�������ڻ�Ϊ�ļ���");
				return false;
			}
			else if (tempNode->GetNodeType()& FOLD)
			{
				curNode = tempNode;	
			}
			else
			{
				//·�������ڻ�Ϊ�ļ���
				virtualdiskInside->LogMsgToConsole("·�������ڻ�Ϊ�ļ���");
				return false;
			}
		}


		//NEW
		if (path.IsAbsolute())
		{
			virtualdiskInside->workingPath_CD = path;
		}
		virtualdiskInside->workingPath_CD.append(path.str());
		virtualdiskInside->SetWorkingNode(curNode);
		return true;



		////����·��ȥѰ����Ӧ�Ľڵ�
		//CellNode* node = virtualdiskInside->GetNodeByPath(path);
		//node = virtualdiskInside->LookingForTaget(node);
		//if (!node)
		//{
		//	virtualdiskInside->LogMsgToConsole("·�������ڻ�Ϊ�ļ���");
		//	return false;
		//}
		//else if (node->GetNodeType()& FOLD)
		//{
		//	virtualdiskInside->SetWorkingNode(node);
		//	return true;
		//}
		//else
		//{
		//	//·�������ڻ�Ϊ�ļ���
		//	virtualdiskInside->LogMsgToConsole("·�������ڻ�Ϊ�ļ���");
		//	return false;
		//}
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