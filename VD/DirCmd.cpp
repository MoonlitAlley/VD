#define _CRT_SECURE_NO_WARNINGS

#include "DirCmd.h"
#include "MyString.h"
#include "VirtualDiskInside.h"
bool DirCmd::Execute(VirtualDiskInside * virtualdisk)
{
	m_VirtualDisk = virtualdisk;


	vector<string> pathItems = cmdParaCollection.m_pathItems;
	//如果用户没有输入路径,非递归打印当前路径
	if (pathItems.empty())
	{
		pathItems.push_back(".");
	}

	for (size_t i = 0; i < pathItems.size(); i++)
	{
		if (cmdParaCollection.m_parsedOptions.find("/s") != cmdParaCollection.m_parsedOptions.end())
		{
			//递归输出
			RecursionPrintItem(pathItems[i]);
		}
		else
		{
			PrintItem(pathItems[i]);
			//输出本节点的子节点
		}
	}


	//计算本次的总输出信息
	size_t filecount = 0;
	size_t foldcount = 0;
	size_t totalsize = 0;
	CountSummary(totalNode, filecount, foldcount, totalsize);

	//如果系统没有要求递归输出 ********************************************************************************待修改
	if (!(cmdParaCollection.m_parsedOptions.find("/s") != cmdParaCollection.m_parsedOptions.end()))
	{
		m_VirtualDisk->LogMsgToConsole(StrProcess::sstr("%16d 个目录", (int)foldcount));
	}
	else
	{
		if (filecount == 0 && foldcount == 0)
		{
			m_VirtualDisk->LogMsgToConsole("找不到相应的文件");
		}
		else
		{
			m_VirtualDisk->LogMsgToConsole(StrProcess::sstr("\n     所列文件总数:\n%16d 个文件%s 字节\n%16d个目录", (int)filecount, NumberToString(totalsize, 15).c_str(), foldcount));
		}
	}

	return true;
}


void DirCmd::RecursionPrintItem(const Path & path)
{
	//是否是使用通配符的路径信息
	if (Tools::isWildcard(path.split().back()))
	{
		CellNode* prelink = m_VirtualDisk->GetNodeByPath(path.Parent());
		CheckDirWithFiles(prelink, path.split().back());
	}
	else
	{
		CellNode* node = m_VirtualDisk->GetNodeByPath(path);
		//如果路径所指向的节点不存在，则不进行输出，直接返回
		if (!node)
		{
			m_VirtualDisk->LogMsgToConsole("该位置可能出错：void DirCmd::RecursionPrintItem(const Path & path)  FormatCheck.noexist ");
			return;
		}
		else if (node->GetNodeType()& LINK && node->GetNodeType()& FOLD)
		{
			//路径信息是指向文件夹的符号链接 输出目标文件夹下的所有文件（文件夹）信息，若用户输入相关参数，则只显示对应信息
			//没有找到符号链接的目标文件---------此时暂不处理失效的符号链接
			if (!m_VirtualDisk->LookingForTaget(node))
			{
				return;
			}
			CheckDirWithFiles(node, "*");
		}
		else if (node->GetNodeType() & LINK && node->GetNodeType() & FILE_CUSTOM)
		{
			//路径信息是指向文件的符号链接  --仅显示当前指向的文件的这一条信息
			if (!m_VirtualDisk->LookingForTaget(node))
			{
				return;
			}
			CheckDirWithFiles(node->GetParent(), node->GetCellName());
		}
		else if (node->GetNodeType()&FOLD)
		{
			//路径信息是指向文件夹的，输出目标文件夹下的所有信息
			CheckDirWithFiles(node, "*");
		}
		else if (node->GetNodeType()&FILE_CUSTOM)
		{
			//路径信息是指向文件的   找到其父节点，输出其信息 --仅显示当前指向的文件的这一条信息
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
			m_VirtualDisk->LogMsgToConsole("系统找不到指定的路径");
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
				m_VirtualDisk->LogMsgToConsole("系统找不到指定的路径");
			}
		}
		else if (node->GetNodeType()& LINK && node->GetNodeType()& FOLD)
		{
			if (!m_VirtualDisk->LookingForTaget(node))
			{
				m_VirtualDisk->LogMsgToConsole("系统找不到指定的路径");
				return;
			}
			CheckDirWithFiles(node, "*");
		}
		else if (node->GetNodeType() & LINK && node->GetNodeType() & FILE_CUSTOM)
		{
			//路径信息是指向文件的符号链接  --仅显示当前指向的文件的这一条信息
			if (!m_VirtualDisk->LookingForTaget(node))
			{
				m_VirtualDisk->LogMsgToConsole("系统找不到指定的路径");
				return;
			}
			CheckDirWithFiles(node->GetParent(), node->GetCellName());
		}
		else if (node->GetNodeType()&FOLD)
		{
			//路径信息是指向文件夹的，输出目标文件夹下的所有信息
			CheckDirWithFiles(node, "*");
		}
		else if (node->GetNodeType()&FILE_CUSTOM)
		{
			//路径信息是指向文件的   找到其父节点，输出其信息 --仅显示当前指向的文件的这一条信息
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

	//想办法记录一下已经处理过的文件（或文件夹），对相同的任务不重复进行，防止循环链接的无尽输出
	if (m_proCessed.find(ProcessedInfo{ node , filesname }) != m_proCessed.end())
	{
		return;
	}
	m_proCessed.insert(ProcessedInfo{ node , filesname });

	//输出当前目录的标头总括
	m_VirtualDisk->LogMsgToConsole(StrProcess::sstr("%s 的目录\n", node->GetNodePath().str().c_str()));

	//如果是符号链接，则找到其目标节点，然后开始处理
	//如果非符号链接，则返回其本身，继续处理
	node = m_VirtualDisk->LookingForTaget(node);
	if (!node) { return; }

	//根据文件名进行筛选当前节点的子节点然后进行输出处理
	list<CellNode*> children = node->FilterSubNode(filesname);
	if (Tools::IsWildcardMatch(filesname, "."))
	{
		//输出一个信息
		m_VirtualDisk->LogMsgToConsole(MakeOneOutput(node, "."));
	}
	if (Tools::IsWildcardMatch(filesname, "..") && !node->IsRootNode())
	{
		//MakeOneOutput(node->GetParent(), "..");
		m_VirtualDisk->LogMsgToConsole(MakeOneOutput(node->GetParent(), ".."));
	}


	for (auto it = children.begin(); it != children.end(); it++)
	{
		//如果系统只要求输出文件夹，则跳过当前文件类型不是fold的选项
		if (cmdParaCollection.m_parsedOptions.find("/ad") != cmdParaCollection.m_parsedOptions.end() && !((*it)->GetNodeType() & FOLD))
		{
			continue;
		}
		m_VirtualDisk->LogMsgToConsole(MakeOneOutput(*it, (*it)->GetCellName()));
	}

	//截至当前，已经处理完一个文件夹*********************************************************************
	oneFoldSummary();
	totalNode.insert(totalNode.end(), oneFoldNode.begin(), oneFoldNode.end());
	oneFoldNode.clear();

	//想办法记录处理过的数量
	//统计信息

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

	//日期与时间 两个空格，   时间与类型，四个空格
	strftime(buff, sizeof(buff), "%G/%m/%d  %H:%M    ", timeinfo);
	string res = buff;
	if ((node->GetNodeType()& FILE_CUSTOM) && !(node->GetNodeType()& LINK))
	{
		//放到一行写会出错
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
	//不在输出其链接信息
	return res;
}

//将数字转化成指定格式的字符串
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
			//该位置可能会无限循环
			m_VirtualDisk->LogMsgToConsole("找不到指定文件");
			m_VirtualDisk->LogMsgToConsole("该位置可能会无限循环：void DirCmd::oneFoldSummary()");
		}
	}
	else
	{
		//输出这一个文件夹的统计信息，然后继续循环    不输出文件夹的数量*********************************************************************************
		m_VirtualDisk->LogMsgToConsole(StrProcess::sstr("%s 个文件%s 字节", NumberToString(fileCount, 16).c_str(), NumberToString(totalSize, 15).c_str()));
	}
}

void DirCmd::CountSummary(const list<CellNode*>& nodes, size_t & files, size_t & dirs, size_t & sizes)
{
	//使用谓词函数进行数量计算
	//此处有待修改内容
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



