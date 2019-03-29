#include "MoveCmd.h"
#include "Path.h"
#include "VirtualDiskInside.h"
#include "MyString.h"

//�õ� Դ·��	֧��ͨ���
//�õ�Ŀ��·��	��֧��ͨ���

//��Դ·���еõ����������ļ��б�ֻ�����ļ���
	//��Դ·��ָ���ļ���
		
			//��Ŀ��·��Ϊ�ļ���
			//��Ŀ��·��������
	//��Դ·��ʹ��ͨ���
		//��ƥ���ļ���Ŀ < 1
			//�ƶ����߸���
		//��ƥ���ļ���Ŀ > 1
			//��Ŀ��·��Ϊ�ļ���
				//�ƶ����߸���
			//��Ŀ��·��������
				//����
	//��Դ·��ָ����ͨ�ļ�
			//��Ŀ��·��Ϊ�ļ���
			//��Ŀ��·��������

bool MoveCmd::Execute(VirtualDiskInside * virtualdisk)
{
	m_VirtualDisk = virtualdisk;
	needAsk = !(cmdParaCollection.m_parsedOptions.find("/y") != cmdParaCollection.m_parsedOptions.end());
	vector<string> pathItems = cmdParaCollection.m_pathItems;
	//copy��move����֧�ֵ���·����Ĭ�ϵڶ���·��Ϊ��ǰ����·��
	if (pathItems.size() == 1)
	{
		pathItems.push_back(".");
	}
	Path from(pathItems[0]);
	Path to(pathItems[1]);

	//������ָ��ͬĿ¼�ķ��������ļ���Ҫ�ҵ���Щ���������ļ���Ŀ���ļ����������������
	if (!Tools::IsLegalPathName(to.split().back()))
	{
		return false;
	}

	vector<Path> srcAllPath;
	if (Tools::isWildcard(from.split().back()))
	{
		CellNode* dir = m_VirtualDisk->GetNodeByPath(from.Self().append(".."));
		if (!dir)
		{
			//������ԣ��˳�
			m_VirtualDisk->LogMsgToConsole("������ԣ�����");
			return false;
		}
		else
		{
			dir = m_VirtualDisk->LookingForTaget(dir);
			if (dir)
			{
				list<CellNode*> children = dir->FilterSubNode(from.split().back());
				for (auto it = children.begin(); it != children.end(); ++it)
				{
					if ((*it)->GetNodeType()&FileNodeType::FILE_CUSTOM) {
						srcAllPath.push_back(m_VirtualDisk->LookingForTaget(*it)->GetNodePath());
					}
				}
			}
		}
	}
	else
	{
		CellNode* node = m_VirtualDisk->GetNodeByPath(from);
		if (!node) {
			m_VirtualDisk->LogMsgToConsole("�ļ�������");
			return false;
		}
		if (m_VirtualDisk->IfNodeBeUsing(node))
		{
			m_VirtualDisk->LogMsgToConsole("�ܾ����ʣ�����Ŀ¼���ڱ�ʹ��");
			return false;
		}
		srcAllPath.push_back(from);
	}

#pragma region pick��ȡ

	//if (!Tools::isWildcard(from.split().back()))
	//{
	//	CellNode* node = m_VirtualDisk->GetNodeByPath(from);
	//	if (!node)
	//	{

	//	}
	//	else
	//	{
	//		node = m_VirtualDisk->LookingForTaget(node);
	//		if (node->GetNodeType()&FileNodeType::FILE_CUSTOM)
	//		{
	//			srcAllPath.push_back(node->GetNodePath());
	//		}
	//		else
	//		{
	//			list<CellNode*> children = node->FilterSubNode("*");
	//			for (auto it = children.begin(); it != children.end(); ++it)
	//			{
	//				if ((*it)->GetNodeType()&FileNodeType::FILE_CUSTOM) {
	//					srcAllPath.push_back(m_VirtualDisk->LookingForTaget(*it)->GetNodePath());
	//				}
	//			}
	//		}
	//	}
	//	
	//}
	//else
	//{
	//	CellNode* dir = m_VirtualDisk->GetNodeByPath(from.Self().append(".."));
	//	if (!dir)
	//	{

	//	}
	//	else
	//	{
	//		dir = m_VirtualDisk->LookingForTaget(dir);
	//		if (dir)
	//		{
	//			list<CellNode*> children = dir->FilterSubNode(from.split().back());
	//			for (auto it = children.begin(); it != children.end(); ++it)
	//			{
	//				if ((*it)->GetNodeType()&FileNodeType::FILE_CUSTOM) {
	//					srcAllPath.push_back(m_VirtualDisk->LookingForTaget(*it)->GetNodePath());
	//				}
	//			}
	//		}
	//	}
	//	
	//}


#pragma endregion

	if (srcAllPath.empty())
	{
		//û�д������ļ�
		return true;
	}
	if (  srcAllPath.size()>1 && (!m_VirtualDisk->GetNodeByPath(to) || m_VirtualDisk->GetNodeByPath(to)->GetNodeType()&FileNodeType::FILE_CUSTOM))
	{
		m_VirtualDisk->LogMsgToConsole("Ŀ��ڵ㲻�����ļ��ڵ� ���߲�����");
		return false;
	}

	for (auto itera = srcAllPath.begin(); itera != srcAllPath.end(); itera++)
	{
		MoveNode(m_VirtualDisk->GetNodeByPath(*itera), to);
	}	
	m_VirtualDisk->LogMsgToConsole(StrProcess::sstr("�ƶ���%9d ���ļ�", srcAllPath.size()));	
	return false;
}


void MoveCmd::MoveNode(CellNode* from, Path& to)
{

	CellNode* dst = m_VirtualDisk->GetNodeByPath(to);
	if (!dst)
	{
		//���Ŀ��·�������ڣ������и��ڵ�
		CellNode* parent = m_VirtualDisk->GetNodeByPath(to.Parent());
		if (!parent)
		{
			//·��������
			return;
		}
		ConfirmThenMove(from, to);
	}
	else if (dst->GetNodeType()&FileNodeType::FOLD)
	{
		//���Ŀ��ڵ���ڣ������ļ��нڵ���߷����������ļ��еĽڵ�

		//�õ�Ŀ���ļ��У�
		CellNode* node = m_VirtualDisk->LookingForTaget(dst);
		if (!node) {

			//·��������
			return;
		}
		else if (from == node)
		{
			//���Ŀ���ļ������ڱ�����
			m_VirtualDisk->LogMsgToConsole("�ܾ����ʣ����ڱ�ʹ��");
			return;
		}
		else
		{
			//���Ŀ���ļ��д��ڣ�����û�б�ʹ�ã����ڴ�Ŀ���ļ���·����ĩ�ˣ�����Ҫ�ƶ����ļ������֡����ƶ�ʱֱ�Ӵ���
			CellNode* cur = node->GetParent();
			while (cur)
			{
				if (cur == from)
				{
					m_VirtualDisk->LogMsgToConsole("�ܾ����ʣ�����Ŀ¼���ڱ�ʹ��");
					return;
				}
				cur = cur->GetParent();
			}
		}
		ConfirmThenMove(from, node->GetNodePath().append(from->GetCellName()));
	}
	else
	{
		//���Ŀ��ڵ���ڣ�Ϊ�ļ��ڵ���߷����������ļ��Ľڵ� �� ����
		Path tempPath = dst->GetNodePath();
		ConfirmThenMove(from, tempPath);
	}
}


void MoveCmd::ConfirmThenMove(CellNode * from,Path & to)
{
	bool conflict = false;
	CellNode* dst = m_VirtualDisk->GetNodeByPath(to);
	conflict = (dst != nullptr);

	//���������˭��Ŀ¼�½��еı��β���
	m_VirtualDisk->LogMsgToConsole(from->GetNodePath().str());
	//Ŀ��·���Ѿ�����⣬Ӧ��Ϊ�ջ����ļ���������Ϊ�ļ���
	if (dst && dst->GetNodeType()&FileNodeType::FOLD)
	{
		m_VirtualDisk->LogMsgToConsole("sadfasdfasdfasdfasdfasdfasd");
		//checkProc(LanguageDenyAccess);
		return;
	}
	//�ƶ�����
	CellNode* parent = m_VirtualDisk->GetNodeByPath(to.Parent());


	//Ŀ��ڵ���Դ�ڵ���ͬ��ָ��ͬһ�ڵ�
	if (dst == from)
	{
		from->RemoveFromPreSubList(false);
		from->SetCellName(to.split().back());
		parent->AddSubNode(from);
		++countFile;
	}
	else if (conflict && needAsk)
	{
		//Ŀ��ڵ��������Ҫѯ��
		while (true)
		{
			string answer = m_VirtualDisk->AskForUserInput(StrProcess::sstr("���� %s ��? (Yes/No/All):", to.str().c_str()));
			transform(answer.begin(), answer.end(), answer.begin(), tolower);
			if (answer == "y" || answer == "yes")
			{
				//�ƶ�
				if (dst && dst != from) {
					dst->RemoveFromPreSubList(true);
				}
				from->RemoveFromPreSubList(false);
				from->SetCellName(to.split().back());
				parent->AddSubNode(from);
				++countFile;
				break;
			}
			else if (answer == "n" || answer == "no")
			{
				break;
			}
			else if (answer == "all")
			{
				needAsk = false;
				//�ƶ��ļ�
				if (dst && dst != from) {
					dst->RemoveFromPreSubList(true);
				}
				from->RemoveFromPreSubList(false);
				from->SetCellName(to.split().back());
				parent->AddSubNode(from);
				++countFile;
				break;
			}
		}
	}
	else
	{
		//Ŀ��ڵ㲻���ڻ����û��Ѿ�����allѡ��
		if (dst && dst != from) {
			dst->RemoveFromPreSubList(true);
		}
		from->RemoveFromPreSubList(false);
		from->SetCellName(to.split().back());
		parent->AddSubNode(from);
		++countFile;
	}
}




//############################# ��д�����ַ���ֻ���ƶ����ض��Ĵ���Ŀ¼ ������ʵ���޸����Ƶȹ���

//bool MoveCmd::Execute(VirtualDiskInside * virtualdisk)
//{
//	m_VirtualDisk = virtualdisk;
//	
//	vector<string> pathItems = cmdParaCollection.m_pathItems;
//
//	//copy��move����֧�ֵ���·����Ĭ�ϵڶ���·��Ϊ��ǰ����·��
//	if (pathItems.size() == 1)
//	{
//		pathItems.push_back(".");
//	}
//	
//
//	Path from(pathItems[0]);
//	Path to(pathItems[1]);
//
//	if (from.IsReal() || to.IsReal())
//	{
//		m_VirtualDisk->LogMsgToConsole("��֧����ʵ·��");
//		return false;
//	}
//	if (!Tools::IsLegalPathName(to.split().back()))
//	{
//		m_VirtualDisk->LogMsgToConsole("Ŀ¼������ȷ");
//		return false;
//	}
//
//
//	list<CellNode*> srcNodeList;
//	if (Tools::isWildcard(from.split().back()))
//	{
//		//����ͨ���
//		CellNode* prelink = m_VirtualDisk->GetNodeByPath(from.Parent());
//		srcNodeList = prelink->FilterSubNode(from.split().back());
//
//	}
//	else
//	{
//		CellNode* node = m_VirtualDisk->GetNodeByPath(from);
//		if (!node)
//		{
//			m_VirtualDisk->LogMsgToConsole("�ļ�������");
//			return false;
//		}
//		if (m_VirtualDisk->IfNodeBeUsing(node))
//		{
//			m_VirtualDisk->LogMsgToConsole("�ܾ����ʣ�����Ŀ¼���ڱ�ʹ��");
//			return false;
//		}
//		srcNodeList.push_back(node);
//	}
//
//
//	if (srcNodeList.empty())
//	{
//		
//		//û����Ҫ�������ļ�
//		return false;
//	}
//	if (!m_VirtualDisk->GetNodeByPath(to) || m_VirtualDisk->GetNodeByPath(to)->GetNodeType()&FileNodeType::FILE_CUSTOM)
//	{
//		m_VirtualDisk->LogMsgToConsole("Ŀ��ڵ㲻�����ļ��ڵ� ���߲�����");
//		return false;
//	}
//
//	//��Ŀ���ļ����´��ڳ�ͻ�ļ�
//	string filename = from.FileName();
//	if (!m_VirtualDisk->GetNodeByPath(to)->FilterSubNode(filename).empty())
//	{
//		conflict = true;
//	}
//
//	needAsk = !(cmdParaCollection.m_parsedOptions.find("/y") != cmdParaCollection.m_parsedOptions.end());
//
//
//	for (auto itera = srcNodeList.begin(); itera != srcNodeList.end(); itera++)
//	{
//		MoveNode(*itera, to);
//	}
//
//	m_VirtualDisk->LogMsgToConsole(StrProcess::sstr("�ƶ���%9d ���ļ�", srcNodeList.size()));
//
//	return false;
//}
//
//
//void MoveCmd::MoveNode(CellNode* from, Path& to)
//{
//	CellNode* dstNode = m_VirtualDisk->GetNodeByPath(to);
//
//	//Ŀ�����ļ��л��߷�������
//	if (dstNode->GetNodeType()&FileNodeType::FOLD)
//	{
//		
//		CellNode* node = m_VirtualDisk->LookingForTaget(dstNode);
//		if (!node)
//		{
//			m_VirtualDisk->LogMsgToConsole("�Ҳ���·��");
//			//�Ҳ���·��
//			return;
//		}
//		else if (from == node)
//		{
//			//��ͬ����ִ��
//			return;
//		}
//		CellNode* cur = node->GetParent();
//		while (cur)
//		{
//			if (cur == from)
//			{
//				m_VirtualDisk->LogMsgToConsole("�ܾ����ʣ�����Ŀ¼���ڱ�ʹ��");
//				return;
//			}
//			cur = cur->GetParent();
//		}
//		//�ƶ��ýڵ�
//		if (conflict && needAsk)
//		{
//			while (true)
//			{
//				//ѯ���Ƿ��ƶ�
//				string answer = m_VirtualDisk->AskForUserInput(StrProcess::sstr("���� %s ��? (Yes/No/All):", to.str().c_str()));
//				string result;
//
//				transform(answer.begin(), answer.end(), answer.begin(), tolower);
//				if (answer == "y" || answer == "yes")
//				{
//					//�ƶ�
//					from->RemoveFromPreSubList(false);
//					dstNode->AddSubNode(from);
//					break;
//				}
//				else if (answer == "n" || answer == "no")
//				{
//					return;
//				}
//				else if (answer == "all")
//				{
//					needAsk = false;
//					//�ƶ��ļ�
//					from->RemoveFromPreSubList(false);
//					dstNode->AddSubNode(from);
//					break;
//				}
//			}
//		}
//		else
//		{
//			//�ƶ��ļ�
//			from->RemoveFromPreSubList(false);
//			dstNode->AddSubNode(from);
//		}
//	}
//}


//void MoveCmd::MoveImpl(CellNode* from, const Path& to)
//{
//	//���������˭��Ŀ¼�½��еı��β���
//	m_VirtualDisk->LogMsgToConsole(from->GetNodePath().str());
//
//	CellNode* dst = m_VirtualDisk->GetNodeByPath(to);
//	//Ŀ��·���Ѿ�����⣬Ӧ��Ϊ�ջ����ļ���������Ϊ�ļ���
//	if (dst && dst->GetNodeType()&FileNodeType::FOLD)
//	{
//		m_VirtualDisk->LogMsgToConsole("sadfasdfasdfasdfasdfasdfasd");
//		//checkProc(LanguageDenyAccess);
//		return;
//	}
//	//�ƶ�����
//	CellNode* parent = m_VirtualDisk->GetNodeByPath(to.Parent());
//	if (dst && dst != from) {
//		dst->RemoveFromPreSubList(true);
//	}
//	from->RemoveFromPreSubList(false);
//	from->SetCellName(to.split().back());
//	parent->AddSubNode(from);
//	++countFile;
//}
