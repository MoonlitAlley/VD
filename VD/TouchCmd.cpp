#include "TouchCmd.h"
#include "VirtualDiskInside.h"
bool TouchCmd::Execute(VirtualDiskInside * virtualdisk)
{
	vector<string> pathItems = cmdParaCollection.m_pathItems;

	for (size_t i = 0; i < pathItems.size(); i++)
	{
		Path path = pathItems[i];
		//�鿴��ǰ�ļ��Ƿ��Ѿ�����

		//�����ļ������� �����������ӣ����ж��ļ����Ƿ���ͬ
		//������ڣ�������ʾ��
		//��������ڣ��������ļ�

		CellNode* node;
		node = virtualdisk->GetNodeByPath(path);
		if (node)
		{
			virtualdisk->LogMsgToConsole("����ͬ���ļ�");
		}
		else
		{
			CellNode* prelink = virtualdisk->GetNodeByPath(path.Parent());

			//�����ڵ㣬�޸�ʱ���ڹ��캯����ʵ��
			//���ø��ڵ�����Ӻ�����ʵ��

			//virtualdisk->LogMsgToConsole("��λ�ÿ��ܳ������ļ���Ϊ��ʱҲ�ᴴ����bool TouchCmd::Execute(VirtualDisk * virtualdisk) ");
			if (path.FileName().empty())
			{
				virtualdisk->LogMsgToConsole("��λ�ÿ��ܳ������ļ���Ϊ��ʱҲ�ᴴ��,������Ӵ��ж� ��bool TouchCmd::Execute(VirtualDisk * virtualdisk) ");
				return false;
			}

			CellNode* fileNode = new CellNode();
			//��λ�ÿ��ܳ���
			fileNode->SetCellName(path.FileName());
			fileNode->SetNodeType(FileNodeType::FILE_CUSTOM);
			prelink->AddSubNode(fileNode);

			//�����ļ����ݣ�**********************************���ܲ��������
			//Ĭ��Ϊ��
		}
	}
	return true;
}
