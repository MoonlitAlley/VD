#define _CRT_SECURE_NO_WARNINGS

#include "DirCmd.h"
#include "MyString.h"
#include "VirtualDiskInside.h"
bool DirCmd::Execute(VirtualDiskInside * virtualdisk)
{
	m_VirtualDisk = virtualdisk;


	vector<string> pathItems = cmdParaCollection.m_pathItems;
	//����û�û������·��,�ǵݹ��ӡ��ǰ·��
	if (pathItems.empty())
	{
		pathItems.push_back(".");
	}

	for (size_t i = 0; i < pathItems.size(); i++)
	{
		if (cmdParaCollection.m_parsedOptions.find("/s") != cmdParaCollection.m_parsedOptions.end())
		{
			//�ݹ����
			RecursionPrintItem(pathItems[i]);
		}
		else
		{
			PrintItem(pathItems[i]);
			//������ڵ���ӽڵ�
		}
	}


	//���㱾�ε��������Ϣ
	size_t filecount = 0;
	size_t foldcount = 0;
	size_t totalsize = 0;
	CountSummary(totalNode, filecount, foldcount, totalsize);

	//���ϵͳû��Ҫ��ݹ���� ********************************************************************************���޸�
	if (!(cmdParaCollection.m_parsedOptions.find("/s") != cmdParaCollection.m_parsedOptions.end()))
	{
		m_VirtualDisk->LogMsgToConsole(StrProcess::sstr("%16d ��Ŀ¼", (int)foldcount));
	}
	else
	{
		if (filecount == 0 && foldcount == 0)
		{
			m_VirtualDisk->LogMsgToConsole("�Ҳ�����Ӧ���ļ�");
		}
		else
		{
			m_VirtualDisk->LogMsgToConsole(StrProcess::sstr("\n     �����ļ�����:\n%16d ���ļ�%s �ֽ�\n%16d��Ŀ¼", (int)filecount, NumberToString(totalsize, 15).c_str(), foldcount));
		}
	}

	return true;
}


void DirCmd::RecursionPrintItem(const Path & path)
{
	//�Ƿ���ʹ��ͨ�����·����Ϣ
	if (Tools::isWildcard(path.split().back()))
	{
		CellNode* prelink = m_VirtualDisk->GetNodeByPath(path.Parent());
		CheckDirWithFiles(prelink, path.split().back());
	}
	else
	{
		CellNode* node = m_VirtualDisk->GetNodeByPath(path);
		//���·����ָ��Ľڵ㲻���ڣ��򲻽��������ֱ�ӷ���
		if (!node)
		{
			m_VirtualDisk->LogMsgToConsole("��λ�ÿ��ܳ���void DirCmd::RecursionPrintItem(const Path & path)  FormatCheck.noexist ");
			return;
		}
		else if (node->GetNodeType()& LINK && node->GetNodeType()& FOLD)
		{
			//·����Ϣ��ָ���ļ��еķ������� ���Ŀ���ļ����µ������ļ����ļ��У���Ϣ�����û�������ز�������ֻ��ʾ��Ӧ��Ϣ
			//û���ҵ��������ӵ�Ŀ���ļ�---------��ʱ�ݲ�����ʧЧ�ķ�������
			if (!m_VirtualDisk->LookingForTaget(node))
			{
				return;
			}
			CheckDirWithFiles(node, "*");
		}
		else if (node->GetNodeType() & LINK && node->GetNodeType() & FILE_CUSTOM)
		{
			//·����Ϣ��ָ���ļ��ķ�������  --����ʾ��ǰָ����ļ�����һ����Ϣ
			if (!m_VirtualDisk->LookingForTaget(node))
			{
				return;
			}
			CheckDirWithFiles(node->GetParent(), node->GetCellName());
		}
		else if (node->GetNodeType()&FOLD)
		{
			//·����Ϣ��ָ���ļ��еģ����Ŀ���ļ����µ�������Ϣ
			CheckDirWithFiles(node, "*");
		}
		else if (node->GetNodeType()&FILE_CUSTOM)
		{
			//·����Ϣ��ָ���ļ���   �ҵ��丸�ڵ㣬�������Ϣ --����ʾ��ǰָ����ļ�����һ����Ϣ
			CheckDirWithFiles(node->GetParent(), node->GetCellName());
		}
	}
}

void DirCmd::PrintItem(const Path & path)
{
	if (Tools::isWildcard(path.split().back()))
	{
		CellNode* parent = m_VirtualDisk->GetNodeByPath(path.Parent());
		if (parent)
		{
			CheckDirWithFiles(parent, path.split().back());
		}
		else
		{
			m_VirtualDisk->LogMsgToConsole("ϵͳ�Ҳ���ָ����·��");
		}
	}
	else
	{
		CellNode* node = m_VirtualDisk->GetNodeByPath(path);
		if (!node)
		{
			CellNode* parent = m_VirtualDisk->GetNodeByPath(path.Parent());
			if (parent)
			{
				CheckDirWithFiles(parent, path.split().back());
			}
			else
			{
				m_VirtualDisk->LogMsgToConsole("ϵͳ�Ҳ���ָ����·��");
			}
		}
		else if (node->GetNodeType()& LINK && node->GetNodeType()& FOLD)
		{
			if (!m_VirtualDisk->LookingForTaget(node))
			{
				m_VirtualDisk->LogMsgToConsole("ϵͳ�Ҳ���ָ����·��");
				return;
			}
			CheckDirWithFiles(node, "*");
		}
		else if (node->GetNodeType() & LINK && node->GetNodeType() & FILE_CUSTOM)
		{
			//·����Ϣ��ָ���ļ��ķ�������  --����ʾ��ǰָ����ļ�����һ����Ϣ
			if (!m_VirtualDisk->LookingForTaget(node))
			{
				m_VirtualDisk->LogMsgToConsole("ϵͳ�Ҳ���ָ����·��");
				return;
			}
			CheckDirWithFiles(node->GetParent(), node->GetCellName());
		}
		else if (node->GetNodeType()&FOLD)
		{
			//·����Ϣ��ָ���ļ��еģ����Ŀ���ļ����µ�������Ϣ
			CheckDirWithFiles(node, "*");
		}
		else if (node->GetNodeType()&FILE_CUSTOM)
		{
			//·����Ϣ��ָ���ļ���   �ҵ��丸�ڵ㣬�������Ϣ --����ʾ��ǰָ����ļ�����һ����Ϣ
			CheckDirWithFiles(node->GetParent(), node->GetCellName());
		}
	}
}

void DirCmd::CheckDirWithFiles(CellNode * node, const string & filesname)
{
	if (!node)
	{
		return;
	}

	//��취��¼һ���Ѿ���������ļ������ļ��У�������ͬ�������ظ����У���ֹѭ�����ӵ��޾����
	if (m_proCessed.find(ProcessedInfo{ node , filesname }) != m_proCessed.end())
	{
		return;
	}
	m_proCessed.insert(ProcessedInfo{ node , filesname });

	//�����ǰĿ¼�ı�ͷ����
	m_VirtualDisk->LogMsgToConsole(StrProcess::sstr("%s ��Ŀ¼\n", node->GetNodePath().str().c_str()));

	//����Ƿ������ӣ����ҵ���Ŀ��ڵ㣬Ȼ��ʼ����
	//����Ƿ������ӣ��򷵻��䱾����������
	node = m_VirtualDisk->LookingForTaget(node);
	if (!node) { return; }

	//�����ļ�������ɸѡ��ǰ�ڵ���ӽڵ�Ȼ������������
	list<CellNode*> children = node->FilterSubNode(filesname);
	if (Tools::IsWildcardMatch(filesname, "."))
	{
		//���һ����Ϣ
		m_VirtualDisk->LogMsgToConsole(MakeOneOutput(node, "."));
	}
	if (Tools::IsWildcardMatch(filesname, "..") && !node->IsRootNode())
	{
		//MakeOneOutput(node->GetParent(), "..");
		m_VirtualDisk->LogMsgToConsole(MakeOneOutput(node->GetParent(), ".."));
	}


	for (auto it = children.begin(); it != children.end(); it++)
	{
		//���ϵͳֻҪ������ļ��У���������ǰ�ļ����Ͳ���fold��ѡ��
		if (cmdParaCollection.m_parsedOptions.find("/ad") != cmdParaCollection.m_parsedOptions.end() && !((*it)->GetNodeType() & FOLD))
		{
			continue;
		}
		m_VirtualDisk->LogMsgToConsole(MakeOneOutput(*it, (*it)->GetCellName()));
	}

	//������ǰ���Ѿ�������һ���ļ���*********************************************************************
	oneFoldSummary();
	totalNode.insert(totalNode.end(), oneFoldNode.begin(), oneFoldNode.end());
	oneFoldNode.clear();

	//��취��¼�����������
	//ͳ����Ϣ

	if (cmdParaCollection.m_parsedOptions.find("/s") != cmdParaCollection.m_parsedOptions.end())
	{
		list<CellNode*> children = node->FilterSubNode("*");

		for (auto it = children.begin(); it != children.end(); it++)
		{
			if ((*it)->GetNodeType()& FOLD)
			{
				RecursionPrintItem((*it)->GetNodePath().append(filesname));
			}
		}
	}
}

string DirCmd::MakeOneOutput(CellNode * node, const string & name)
{
	time_t rawtime = node->GetlastModifyTime();
	struct tm * timeinfo = localtime(&rawtime);
	char buff[128] = { 0 };

	//������ʱ�� �����ո�   ʱ�������ͣ��ĸ��ո�
	strftime(buff, sizeof(buff), "%G/%m/%d  %H:%M    ", timeinfo);
	string res = buff;
	if ((node->GetNodeType()& FILE_CUSTOM) && !(node->GetNodeType()& LINK))
	{
		//�ŵ�һ��д�����
		vector<char> a = node->Content();
		res += NumberToString(a.size(), 14);
	}
	else
	{
		res += StrProcess::sstr("%-14s", node->GetNodeTypeStr().c_str());
	}
	res += " ";
	res += name;
	oneFoldNode.push_back(node);
	//���������������Ϣ
	return res;
}

//������ת����ָ����ʽ���ַ���
string DirCmd::NumberToString(size_t num, size_t length)
{
	list<char> retTmp;
	const string temp = StrProcess::sstr("%u", num);

	for (auto it = temp.rbegin(); it != temp.rend(); it++)
	{
		int diff = (int)distance(temp.rbegin(), it);
		if (diff > 0 && diff % 3 == 0)
		{
			retTmp.push_front(',');
		}
		retTmp.push_front(*it);
	}
	while (retTmp.size() < length)
	{
		retTmp.push_front(' ');
	}
	return string(retTmp.begin(), retTmp.end());
}

void DirCmd::oneFoldSummary()
{
	size_t fileCount = 0;
	size_t foldCount = 0;
	size_t totalSize = 0;

	CountSummary(oneFoldNode, fileCount, foldCount, totalSize);

	if (fileCount == 0 && foldCount == 0)
	{
		if (!(cmdParaCollection.m_parsedOptions.find("/s") != cmdParaCollection.m_parsedOptions.end()))
		{
			//��λ�ÿ��ܻ�����ѭ��
			m_VirtualDisk->LogMsgToConsole("�Ҳ���ָ���ļ�");
			m_VirtualDisk->LogMsgToConsole("��λ�ÿ��ܻ�����ѭ����void DirCmd::oneFoldSummary()");
		}
	}
	else
	{
		//�����һ���ļ��е�ͳ����Ϣ��Ȼ�����ѭ��    ������ļ��е�����*********************************************************************************
		m_VirtualDisk->LogMsgToConsole(StrProcess::sstr("%s ���ļ�%s �ֽ�", NumberToString(fileCount, 16).c_str(), NumberToString(totalSize, 15).c_str()));
	}
}

void DirCmd::CountSummary(const list<CellNode*>& nodes, size_t & files, size_t & dirs, size_t & sizes)
{
	//ʹ��ν�ʺ���������������
	//�˴��д��޸�����
	for (auto itera = nodes.begin(); itera != nodes.end(); itera++)
	{
		CellNode* tempNode = *itera;
		Path tempPath = tempNode->GetNodePath();

		if (Tools::isWildcard(tempPath.split().back()))
		{

		}
		else
		{
			if (!tempNode)
			{

			}
			else if (tempNode->GetNodeType()& LINK && tempNode->GetNodeType()& FOLD)
			{
				dirs++;
			}
			else if (tempNode->GetNodeType() & LINK && tempNode->GetNodeType() & FILE_CUSTOM)
			{
				files++;
			}
			else if (tempNode->GetNodeType()&FOLD)
			{
				dirs++;
			}
			else if (tempNode->GetNodeType()&FILE_CUSTOM)
			{
				files++;
				sizes += (int)tempNode->Content().size();
			}
		}
	}
}



