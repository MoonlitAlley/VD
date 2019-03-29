#include "MoveCmd.h"
#include "Path.h"
#include "VirtualDiskInside.h"
#include "MyString.h"

//得到 源路径	支持通配符
//得到目标路径	不支持通配符

//从源路径中得到待操作的文件列表（只包含文件）
	//若源路径指向文件夹
		
			//若目标路径为文件夹
			//若目标路径不存在
	//若源路径使用通配符
		//若匹配文件项目 < 1
			//移动或者覆盖
		//若匹配文件项目 > 1
			//若目标路径为文件夹
				//移动或者覆盖
			//若目标路径不存在
				//错误
	//若源路径指向普通文件
			//若目标路径为文件夹
			//若目标路径不存在

bool MoveCmd::Execute(VirtualDiskInside * virtualdisk)
{
	m_VirtualDisk = virtualdisk;
	needAsk = !(cmdParaCollection.m_parsedOptions.find("/y") != cmdParaCollection.m_parsedOptions.end());
	vector<string> pathItems = cmdParaCollection.m_pathItems;
	//copy和move命令支持单个路径，默认第二条路径为当前工作路径
	if (pathItems.size() == 1)
	{
		pathItems.push_back(".");
	}
	Path from(pathItems[0]);
	Path to(pathItems[1]);

	//其中有指向不同目录的符号链接文件，要找到这些符号链接文件的目标文件，加入待操作链表
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
			//情况不对，退出
			m_VirtualDisk->LogMsgToConsole("情况不对，撤退");
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
			m_VirtualDisk->LogMsgToConsole("文件不存在");
			return false;
		}
		if (m_VirtualDisk->IfNodeBeUsing(node))
		{
			m_VirtualDisk->LogMsgToConsole("拒绝访问，父级目录正在被使用");
			return false;
		}
		srcAllPath.push_back(from);
	}

#pragma region pick截取

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
		//没有待操作文件
		return true;
	}
	if (  srcAllPath.size()>1 && (!m_VirtualDisk->GetNodeByPath(to) || m_VirtualDisk->GetNodeByPath(to)->GetNodeType()&FileNodeType::FILE_CUSTOM))
	{
		m_VirtualDisk->LogMsgToConsole("目标节点不能是文件节点 或者不存在");
		return false;
	}

	for (auto itera = srcAllPath.begin(); itera != srcAllPath.end(); itera++)
	{
		MoveNode(m_VirtualDisk->GetNodeByPath(*itera), to);
	}	
	m_VirtualDisk->LogMsgToConsole(StrProcess::sstr("移动了%9d 个文件", srcAllPath.size()));	
	return false;
}


void MoveCmd::MoveNode(CellNode* from, Path& to)
{

	CellNode* dst = m_VirtualDisk->GetNodeByPath(to);
	if (!dst)
	{
		//如果目标路径不存在，且其有父节点
		CellNode* parent = m_VirtualDisk->GetNodeByPath(to.Parent());
		if (!parent)
		{
			//路径不存在
			return;
		}
		ConfirmThenMove(from, to);
	}
	else if (dst->GetNodeType()&FileNodeType::FOLD)
	{
		//如果目标节点存在，切问文件夹节点或者符号链接至文件夹的节点

		//得到目标文件夹，
		CellNode* node = m_VirtualDisk->LookingForTaget(dst);
		if (!node) {

			//路径不存在
			return;
		}
		else if (from == node)
		{
			//如果目标文件夹正在被访问
			m_VirtualDisk->LogMsgToConsole("拒绝访问，正在被使用");
			return;
		}
		else
		{
			//如果目标文件夹存在，而且没有被使用，则在此目标文件夹路径的末端，加上要移动的文件的名字。在移动时直接创建
			CellNode* cur = node->GetParent();
			while (cur)
			{
				if (cur == from)
				{
					m_VirtualDisk->LogMsgToConsole("拒绝访问，父级目录正在被使用");
					return;
				}
				cur = cur->GetParent();
			}
		}
		ConfirmThenMove(from, node->GetNodePath().append(from->GetCellName()));
	}
	else
	{
		//如果目标节点存在，为文件节点或者符号链接至文件的节点 ， 覆盖
		Path tempPath = dst->GetNodePath();
		ConfirmThenMove(from, tempPath);
	}
}


void MoveCmd::ConfirmThenMove(CellNode * from,Path & to)
{
	bool conflict = false;
	CellNode* dst = m_VirtualDisk->GetNodeByPath(to);
	conflict = (dst != nullptr);

	//首相输出在谁的目录下进行的本次操作
	m_VirtualDisk->LogMsgToConsole(from->GetNodePath().str());
	//目标路径已经被检测，应当为空或者文件。不可能为文件夹
	if (dst && dst->GetNodeType()&FileNodeType::FOLD)
	{
		m_VirtualDisk->LogMsgToConsole("sadfasdfasdfasdfasdfasdfasd");
		//checkProc(LanguageDenyAccess);
		return;
	}
	//移动操作
	CellNode* parent = m_VirtualDisk->GetNodeByPath(to.Parent());


	//目标节点与源节点相同，指向同一节点
	if (dst == from)
	{
		from->RemoveFromPreSubList(false);
		from->SetCellName(to.split().back());
		parent->AddSubNode(from);
		++countFile;
	}
	else if (conflict && needAsk)
	{
		//目标节点存在且需要询问
		while (true)
		{
			string answer = m_VirtualDisk->AskForUserInput(StrProcess::sstr("覆盖 %s 吗? (Yes/No/All):", to.str().c_str()));
			transform(answer.begin(), answer.end(), answer.begin(), tolower);
			if (answer == "y" || answer == "yes")
			{
				//移动
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
				//移动文件
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
		//目标节点不存在或者用户已经输入all选项
		if (dst && dst != from) {
			dst->RemoveFromPreSubList(true);
		}
		from->RemoveFromPreSubList(false);
		from->SetCellName(to.split().back());
		parent->AddSubNode(from);
		++countFile;
	}
}




//############################# 重写，这种方法只能移动到特定的存在目录 ，不能实现修改名称等功能

//bool MoveCmd::Execute(VirtualDiskInside * virtualdisk)
//{
//	m_VirtualDisk = virtualdisk;
//	
//	vector<string> pathItems = cmdParaCollection.m_pathItems;
//
//	//copy和move命令支持单个路径，默认第二条路径为当前工作路径
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
//		m_VirtualDisk->LogMsgToConsole("不支持真实路径");
//		return false;
//	}
//	if (!Tools::IsLegalPathName(to.split().back()))
//	{
//		m_VirtualDisk->LogMsgToConsole("目录名不正确");
//		return false;
//	}
//
//
//	list<CellNode*> srcNodeList;
//	if (Tools::isWildcard(from.split().back()))
//	{
//		//含有通配符
//		CellNode* prelink = m_VirtualDisk->GetNodeByPath(from.Parent());
//		srcNodeList = prelink->FilterSubNode(from.split().back());
//
//	}
//	else
//	{
//		CellNode* node = m_VirtualDisk->GetNodeByPath(from);
//		if (!node)
//		{
//			m_VirtualDisk->LogMsgToConsole("文件不存在");
//			return false;
//		}
//		if (m_VirtualDisk->IfNodeBeUsing(node))
//		{
//			m_VirtualDisk->LogMsgToConsole("拒绝访问，父级目录正在被使用");
//			return false;
//		}
//		srcNodeList.push_back(node);
//	}
//
//
//	if (srcNodeList.empty())
//	{
//		
//		//没有需要操作的文件
//		return false;
//	}
//	if (!m_VirtualDisk->GetNodeByPath(to) || m_VirtualDisk->GetNodeByPath(to)->GetNodeType()&FileNodeType::FILE_CUSTOM)
//	{
//		m_VirtualDisk->LogMsgToConsole("目标节点不能是文件节点 或者不存在");
//		return false;
//	}
//
//	//在目标文件夹下存在冲突文件
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
//	m_VirtualDisk->LogMsgToConsole(StrProcess::sstr("移动了%9d 个文件", srcNodeList.size()));
//
//	return false;
//}
//
//
//void MoveCmd::MoveNode(CellNode* from, Path& to)
//{
//	CellNode* dstNode = m_VirtualDisk->GetNodeByPath(to);
//
//	//目标是文件夹或者符号链接
//	if (dstNode->GetNodeType()&FileNodeType::FOLD)
//	{
//		
//		CellNode* node = m_VirtualDisk->LookingForTaget(dstNode);
//		if (!node)
//		{
//			m_VirtualDisk->LogMsgToConsole("找不到路径");
//			//找不到路径
//			return;
//		}
//		else if (from == node)
//		{
//			//相同，不执行
//			return;
//		}
//		CellNode* cur = node->GetParent();
//		while (cur)
//		{
//			if (cur == from)
//			{
//				m_VirtualDisk->LogMsgToConsole("拒绝访问，父级目录正在被使用");
//				return;
//			}
//			cur = cur->GetParent();
//		}
//		//移动该节点
//		if (conflict && needAsk)
//		{
//			while (true)
//			{
//				//询问是否移动
//				string answer = m_VirtualDisk->AskForUserInput(StrProcess::sstr("覆盖 %s 吗? (Yes/No/All):", to.str().c_str()));
//				string result;
//
//				transform(answer.begin(), answer.end(), answer.begin(), tolower);
//				if (answer == "y" || answer == "yes")
//				{
//					//移动
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
//					//移动文件
//					from->RemoveFromPreSubList(false);
//					dstNode->AddSubNode(from);
//					break;
//				}
//			}
//		}
//		else
//		{
//			//移动文件
//			from->RemoveFromPreSubList(false);
//			dstNode->AddSubNode(from);
//		}
//	}
//}


//void MoveCmd::MoveImpl(CellNode* from, const Path& to)
//{
//	//首相输出在谁的目录下进行的本次操作
//	m_VirtualDisk->LogMsgToConsole(from->GetNodePath().str());
//
//	CellNode* dst = m_VirtualDisk->GetNodeByPath(to);
//	//目标路径已经被检测，应当为空或者文件。不可能为文件夹
//	if (dst && dst->GetNodeType()&FileNodeType::FOLD)
//	{
//		m_VirtualDisk->LogMsgToConsole("sadfasdfasdfasdfasdfasdfasd");
//		//checkProc(LanguageDenyAccess);
//		return;
//	}
//	//移动操作
//	CellNode* parent = m_VirtualDisk->GetNodeByPath(to.Parent());
//	if (dst && dst != from) {
//		dst->RemoveFromPreSubList(true);
//	}
//	from->RemoveFromPreSubList(false);
//	from->SetCellName(to.split().back());
//	parent->AddSubNode(from);
//	++countFile;
//}
