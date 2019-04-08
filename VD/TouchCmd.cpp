#include "TouchCmd.h"
#include "VirtualDiskInside.h"
#include "FileNode.h"
bool TouchCmd::Execute(VirtualDiskInside * virtualdisk)
{
	m_VirtualDisk = virtualdisk;
	vector<string> pathItems = cmdParaCollection.m_pathItems;

	for (size_t i = 0; i < pathItems.size(); i++)
	{
		Path path = pathItems[i];
		//�鿴��ǰ�ļ��Ƿ��Ѿ�����

		//�����ļ������� �����������ӣ����ж��ļ����Ƿ���ͬ
		//������ڣ�������ʾ��
		//��������ڣ��������ļ�

		CellNode* node;
		node = GetDstNodeByPath(path);
		if (node)
		{
			virtualdisk->LogMsgToConsole("����ͬ���ļ�");
		}
		else
		{
			CellNode* prelink = GetDstNodeByPath(path.Parent());


			//�����ڵ㣬�޸�ʱ���ڹ��캯����ʵ��
			//���ø��ڵ�����Ӻ�����ʵ��

			//virtualdisk->LogMsgToConsole("��λ�ÿ��ܳ������ļ���Ϊ��ʱҲ�ᴴ����bool TouchCmd::Execute(VirtualDisk * virtualdisk) ");
			if (path.FileName().empty())
			{
				virtualdisk->LogMsgToConsole("��λ�ÿ��ܳ������ļ���Ϊ��ʱҲ�ᴴ��,������Ӵ��ж� ��bool TouchCmd::Execute(VirtualDisk * virtualdisk) ");
				return false;
			}

			FileNode* fileNode = new FileNode();
			//��λ�ÿ��ܳ���
			fileNode->SetCellName(path.FileName());
			//fileNode->SetNodeType(FileNodeType::FILE_CUSTOM);
			prelink->AddSubNode(fileNode);

			//�����ļ����ݣ�**********************************���ܲ��������
			//Ĭ��Ϊ��
		}
	}
	return true;
}
CellNode* TouchCmd::GetDstNodeByPath(const Path &to)
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