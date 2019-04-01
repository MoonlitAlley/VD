
#include "MdCmd.h"
#include "VirtualDiskInside.h"
#include "Common.h"
//��λ���жϿ��ܳ���
bool MdCmd::Execute(VirtualDiskInside* virtualdisk)
{
	vector<string> pathItems = cmdParaCollection.m_pathItems;
	for (size_t i = 0; i < pathItems.size(); i++)
	{
		//���������·��
		//�Բ��������е�·����Ϣ���зֽ⣬���Ƚ�\�滻Ϊ/�� Ȼ����/���зֽ⡣�ֽ�Ϊ�����ڵ�����
		Path tempPath(pathItems[i]);

		if (tempPath.IsReal())
		{
			return false;
		}
		//�õ�·���Ľڵ�����ʸ����
		vector<string> items = tempPath.split();

		//�ж��Ƿ������·����ȷ����ǰ�ڵ�
		CellNode * curNode = virtualdisk->GetNodeByPath(tempPath.StartNode());

		//��ʼѰ·���ߴ�����Ӧ���ļ���
		for (size_t i = 0; i < items.size(); i++)
		{
			if (curNode == NULL)
			{
				return false;
			}
			//�жϵ�ǰ·���ڵ������Ƿ�Ϸ�
			if (!Tools::IsLegalPathName(items[i]))
			{
				return false;
			}
			if (items[i] == "." || items[i] == "..")
			{
				curNode = curNode->GetNode(items[i]);
				continue;
			}

			//���ݵ�ǰ�ڵ�����ȥѰ����Ӧ�Ľڵ�
			CellNode* child = curNode->GetNode(items[i]);
			if (!child)
			{
				child = new CellNode();
				child->SetCellName(items[i]);
				child->SetNodeType(FOLD);
				curNode->AddSubNode(child);
				//�������Ľڵ���Ϊ��ǰ�ڵ㣬������һ���û�����ڵ�Ĵ���
				curNode = child;
			}
			else if (child->GetNodeType() & FOLD)
			{
				//���child������Ϊ�ļ��У���curNode = child ; ������Ϊlink����curNode ���� child��target
				curNode = virtualdisk->LookingForTaget(child);
				continue;
			}
			else
			{
				virtualdisk->LogMsgToConsole("�Ѵ�����ͬ���Ƶ��ļ�");
				return false;
			}
		}
	}
	return true;
}
