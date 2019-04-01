#include "RenCmd.h"
#include "VirtualDiskInside.h"
bool RenCmd::Execute(VirtualDiskInside * virtualdisk)
{
	m_VirtualDisk = virtualdisk;
	//������Mklink��·����ȡ����

	//��һ��·������Ŵ��޸Ľڵ��·����Ϣ
	//�ڶ���·�������Ҫ�޸ĳɵ�����

	//���Ҽ���һ��·����ָ��ڵ��״̬
		//������ -�� ������ʾ���˳�
		//���� -�� �Ƿ����ڱ�ʹ��
					//��	-�� �˳�
					//��	-�� Ŀ�����Ƿ�Ϸ�
							//�Ϸ� -���޸Ľڵ���




	//�������⣬
		//1��ͨ���
		//2�����������µĵ�ַ


	vector<string> pathItems = cmdParaCollection.m_pathItems;
	Path srcNodePath(pathItems[0]);
	string applyName = pathItems[1];
	CellNode* tempNode = NULL;
	if (Tools::isWildcard(srcNodePath.split().back()))
	{
		string wildcard = srcNodePath.split().back();

		vector<string> wildPath = srcNodePath.Self().append("..").split();

		CellNode* curNode = virtualdisk->GetNodeByPath(srcNodePath.StartNode());

		curNode = virtualdisk->LookingForTaget(curNode);


		CellNode* tempNode = NULL;
		for (size_t i = 0; i < wildPath.size(); i++)
		{
			tempNode = curNode->GetNode(wildPath[i]);
			if (++i < wildPath.size())
			{
				--i;
				tempNode = virtualdisk->LookingForTaget(tempNode);
			}
			if (!tempNode)
			{
				virtualdisk->LogMsgToConsole("·��������");
				return false;

			}
			else if (tempNode->GetNodeType()& FOLD)
			{
				curNode = tempNode;
			}
			else if (tempNode->GetNodeType()& FILE_CUSTOM)
			{
				//��һ���ļ�,�ж��Ƿ������սڵ�
				if (++i < wildPath.size())
				{
					--i;
					virtualdisk->LogMsgToConsole("·���в�Ӧ�ó����ļ�");
					return false;
				}

			}
		}

		//����curNode����
		list<CellNode*> wildNodeList = tempNode->FilterSubNode(wildcard);

		if (wildNodeList.empty())
		{
			//Ŀ�겻����
			return false;
		}

		CellNode* targetNode = wildNodeList.front();
		if (!targetNode)
		{
			m_VirtualDisk->LogMsgToConsole("Ŀ��ڵ㲻����");
			return false;
		}
		else if (m_VirtualDisk->IfNodeBeUsing(targetNode))
		{
			m_VirtualDisk->LogMsgToConsole("Ŀ��·���ǹ���·��");
			return false;
		}
		else if (!Tools::IsLegalFileName(applyName))
		{
			m_VirtualDisk->LogMsgToConsole("���Ʋ��Ϸ�");
			return false;
		}
		else if (targetNode->GetParent()->GetNode(applyName))
		{
			m_VirtualDisk->LogMsgToConsole("����ͬ���ļ�");
			return false;
		}
		else
		{
			targetNode->SetCellName(applyName);
		}
		return true;

	}
	else
	{
		CellNode* curNode = virtualdisk->GetNodeByPath(srcNodePath.StartNode());

		curNode = virtualdisk->LookingForTaget(curNode);

		vector<string> path = srcNodePath.split();


		CellNode* targetNode =NULL;
		for (size_t i = 0; i < path.size(); i++)
		{
			targetNode = curNode->GetNode(path[i]);
			if (++i < path.size())
			{
				--i;
				targetNode = virtualdisk->LookingForTaget(targetNode);
			}
			if (!targetNode)
			{
				virtualdisk->LogMsgToConsole("·��������");
				return false;

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
					virtualdisk->LogMsgToConsole("·���в�Ӧ�ó����ļ�");
					return false;
				}

			}
		}

		if (!targetNode)
		{
			m_VirtualDisk->LogMsgToConsole("Ŀ��ڵ㲻����");
			return false;
		}
		else if (m_VirtualDisk->IfNodeBeUsing(targetNode))
		{
			m_VirtualDisk->LogMsgToConsole("Ŀ��·���ǹ���·��");
			return false;
		}
		else if (!Tools::IsLegalFileName(applyName))
		{
			m_VirtualDisk->LogMsgToConsole("���Ʋ��Ϸ�");
			return false;
		}
		else if (targetNode->GetParent()->GetNode(applyName))
		{
			m_VirtualDisk->LogMsgToConsole("����ͬ���ļ�");
			return false;
		}
		else
		{
			targetNode->SetCellName(applyName);
		}
		return true;
	}

	//vector<string> pathItems = cmdParaCollection.m_pathItems;
	//Path srcNodePath(pathItems[0]);
	//string applyName = pathItems[1];

	//CellNode* srcNode = m_VirtualDisk->GetNodeByPath(srcNodePath);

	//if (!srcNode)
	//{
	//	m_VirtualDisk->LogMsgToConsole("Ŀ��ڵ㲻����");
	//	return false;
	//}
	//else if (m_VirtualDisk->IfNodeBeUsing(srcNode))
	//{
	//	m_VirtualDisk->LogMsgToConsole("Ŀ��·���ǹ���·��");
	//	return false;
	//}
	//else if (!Tools::IsLegalFileName(applyName))
	//{
	//	m_VirtualDisk->LogMsgToConsole("���Ʋ��Ϸ�");
	//	return false;
	//}
	//else if (srcNode->GetParent()->GetNode(applyName))
	//{
	//	m_VirtualDisk->LogMsgToConsole("����ͬ���ļ�");
	//	return false;
	//}
	//else
	//{
	//	srcNode->SetCellName(applyName);
	//}
	//return true;
}
