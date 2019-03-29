#include "MklinkCmd.h"
#include "VirtualDiskInside.h"
#include "Path.h"
bool MklinkCmd::Execute(VirtualDiskInside * virtualdisk)
{
	m_VirtualDisk = virtualdisk;
	//��pathItems�б���������items���ֱ����һ��·��
	//�õ�Ŀ��ڵ�� ·�������͡�������Ϣ �� ��ŵ�Ҫ�����ķ������ӽڵ��content�С�

	
	vector<string> pathItems = cmdParaCollection.m_pathItems;
	//����Ҫ��֤��vector��������·����Ϣ��
	Path linkNodePath(pathItems[0]);
	Path targetNodePath(pathItems[1]);

	if (linkNodePath.FileName().empty())
	{
		m_VirtualDisk->LogMsgToConsole("�ļ������Ϸ����˳�");
		return false;
	}

	if (virtualdisk->GetNodeByPath(linkNodePath))
	{
		m_VirtualDisk->LogMsgToConsole("�����Ľڵ����ͬ���ļ����˳�");
		return false;
	}

	CellNode* targetNode = virtualdisk->GetNodeByPath(targetNodePath);
	if (!targetNode)
	{
		m_VirtualDisk->LogMsgToConsole("Ŀ��ڵ㲻���ڣ��˳�");
		return false;
	}
	if (cmdParaCollection.m_parsedOptions.find("/d") != cmdParaCollection.m_parsedOptions.end() && !(targetNode->GetNodeType()&FOLD))
	{
		m_VirtualDisk->LogMsgToConsole("Ŀ���ļ�������Ŀ¼���˳�");
		return false;
	}
	if (!(cmdParaCollection.m_parsedOptions.find("/d") != cmdParaCollection.m_parsedOptions.end()) && !(targetNode->GetNodeType()&FILE_CUSTOM))
	{
		m_VirtualDisk->LogMsgToConsole("Ŀ���ļ��������ļ����˳�");
		return false;
	}


	//�жϽ�������ʼ�������ӡ�

	//Ŀ���ļ��Ƿ�����Ƿ������ӣ�

	string filename = linkNodePath.FileName();
	//�ҵ�������·�����ϼ��ڵ�
	linkNodePath.append("..");

	CellNode* prelink = virtualdisk->GetNodeByPath(linkNodePath);

	if (!prelink)
	{
		return false;
	}

	//�����ýڵ�
	CellNode* node = new CellNode();
	node->SetCellName(filename);
	prelink->AddSubNode(node);

	node->SetNodeType(FileNodeType(LINK |targetNode->GetNodeType()));

	//��Ŀ��ڵ����Ϣ�ŵ��ýڵ��content��
	const string tempPathString = targetNode->GetNodePath().str();
	node->Content().assign(tempPathString.begin(), tempPathString.end());

	return true;
}
