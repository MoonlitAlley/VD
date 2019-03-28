#include "RenCmd.h"
#include "VirtualDisk.h"
bool RenCmd::Execute(VirtualDisk * virtualdisk)
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

	vector<string> pathItems = cmdParaCollection.m_pathItems;
	Path srcNodePath(pathItems[0]);
	string applyName = pathItems[1];

	CellNode* srcNode = m_VirtualDisk->GetNodeByPath(srcNodePath);

	if (!srcNode)
	{
		m_VirtualDisk->LogMsgToConsole("Ŀ��ڵ㲻����");
		return false;
	}
	else if (m_VirtualDisk->IfNodeBeUsing(srcNode))
	{
		m_VirtualDisk->LogMsgToConsole("Ŀ��·���ǹ���·��");
		return false;
	}
	else if (!Tools::IsLegalFileName(applyName))
	{
		m_VirtualDisk->LogMsgToConsole("���Ʋ��Ϸ�");
		return false;
	}
	else if (srcNode->GetParent()->GetNode(applyName))
	{
		m_VirtualDisk->LogMsgToConsole("����ͬ���ļ�");
		return false;
	}
	else
	{
		srcNode->SetCellName(applyName);
	}
	return true;
}
