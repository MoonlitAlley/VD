#define _CRT_SECURE_NO_WARNINGS
#include "CopyCmd.h"
#include "MyString.h"
#include "VirtualDiskInside.h"
#include <io.h>

//本命令要支持从物理磁盘拷贝内容

//四种拷贝方式
//V-R
//R-V
//V-V
//R-R

//第一个路径存放着源路径信息	支持通配符
//第二个路径存放着目标路径信息		支持通配符


//得到所有待操作路径
//用户输入的是真实路径	
	//输入的路径中不包括通配符
		//是一个正常的文件
		//是一个文件夹,将文件夹中的所有文件节点放入待操作列表
	//含有通配符
		//得到其父级路径，在父级路径中进行筛选

//用户输入的不是真实路径	
	//输入的路径中不包括通配符
		//是一个正常的文件
		//是一个文件夹,将文件夹中的所有文件节点放入待操作列表
	//含有通配符
		//得到其父级路径，在父级路径中进行筛选



//得到待操作列表

bool CopyCmd::Execute(VirtualDiskInside * virtualdisk)
{
	m_VirtualDisk = virtualdisk;
	needAsk = !(cmdParaCollection.m_parsedOptions.find("/y") != cmdParaCollection.m_parsedOptions.end());
	vector<string> pathItems = cmdParaCollection.m_pathItems;
	//命令参数解析中已经判断过路径数量
	Path srcPath(pathItems[0]);
	Path dstPath(pathItems[1]);


	//首先找到所有的待操作文件，放到路径链表中
	vector<Path> allsrcPath;
	//用户输入的是真实路径
	if (srcPath.IsReal())
	{
		//输入的路径中不包括通配符
		if (!Tools::isWildcard(srcPath.split().back()))
		{
			_finddata_t findData;
			intptr_t fileHandle = _findfirst(srcPath.str().c_str(), &findData);
			if (fileHandle == -1)
			{
				//查找失败，可能是路径出错
				m_VirtualDisk->LogMsgToConsole("路径不存在");
				return false;
			}
			if (findData.attrib &_A_ARCH)
			{
				//是一个正常的文件
				allsrcPath.push_back(srcPath);
			}
			else if (findData.attrib & _A_SUBDIR)
			{
				//是一个文件夹,将文件夹中的所有文件节点放入待操作列表
				fileHandle = _findfirst(srcPath.Self().append("*").str().c_str(), &findData);
				do
				{
					if (string(findData.name) == "." || string(findData.name) == "..")
					{
						continue;
					}
					if (findData.attrib & _A_SUBDIR)
					{
						continue;
					}
					allsrcPath.push_back(srcPath.Self().append(findData.name));
				} while (_findnext(fileHandle, &findData) == 0);
			}
		}
		else
		{
			//含有通配符
			_finddata_t findData;
			intptr_t fileHandle = _findfirst(srcPath.str().c_str(), &findData);
			if (fileHandle == -1)
			{
				//查找失败，可能是路径出错
				m_VirtualDisk->LogMsgToConsole("路径不存在");
				return false;
			}
			do
			{
				if (string(findData.name) == "." || string(findData.name) == "..")
				{
					continue;
				}
				if (findData.attrib & _A_SUBDIR)
				{
					continue;
				}
				if (!Tools::IsWildcardMatch(srcPath.split().back(), findData.name))
				{
					//再判断一次得到文件列表中当前文件是否匹配用户输入的通配符
					continue;
				}
				allsrcPath.push_back(srcPath.Self().append("..").append(findData.name));

			} while (_findnext(fileHandle, &findData) == 0);
		}
	}
	else
	{
		//不是真实路径
		if (!Tools::isWildcard(srcPath.split().back()))
		{
			//输入的路径中不包括通配符
			CellNode* node = m_VirtualDisk->GetNodeByPath(srcPath);
			if (node)
			{
				node = m_VirtualDisk->LookingForTaget(node);
				//路径指向节点是一个文件，将此文件的路径放入待复制列表
				if (node->GetNodeType()&FileNodeType::FILE_CUSTOM)
				{
					allsrcPath.push_back(node->GetNodePath());
				}
				else
				{
					//路径指向的节点是一个文件夹，将文件夹中的所有文件节点放入待操作列表
					list<CellNode*> children = node->FilterSubNode("*");
					for (auto itera = children.begin(); itera != children.end(); itera++)
					{
						if ((*itera)->GetNodeType()&FileNodeType::FILE_CUSTOM)
						{
							allsrcPath.push_back(m_VirtualDisk->LookingForTaget(*itera)->GetNodePath());
						}
					}
				}
			}
			else
			{
				m_VirtualDisk->LogMsgToConsole("路径不存在");
				return false;
			}
		}
		else
		{
			//含有通配符
			//得到其父级路径，在父级路径中进行筛选
			CellNode* dir = m_VirtualDisk->GetNodeByPath(srcPath.Self().append(".."));
			if (dir)
			{
				dir = m_VirtualDisk->LookingForTaget(dir);
				//在父级路径下筛选文件节点，放入到待操作列表
				if (dir)
				{
					list<CellNode*> children = dir->FilterSubNode(srcPath.split().back());
					for (auto itera = children.begin(); itera != children.end(); itera++)
					{
						if ((*itera)->GetNodeType()&FileNodeType::FILE_CUSTOM)
						{
							allsrcPath.push_back(m_VirtualDisk->LookingForTaget(*itera)->GetNodePath());
						}
					}
				}
			}
			else
			{
				m_VirtualDisk->LogMsgToConsole("路径不存在");
				return false;
			}
		}
	}

	if (allsrcPath.empty())
	{
		//没有要处理的文件
		return true;
	}
	for (size_t i = 0; i < allsrcPath.size(); i++)
	{
		Path& src(allsrcPath[i]);
		if (Tools::isWildcard(dstPath.split().back()))
		{
			//目标路径中有通配符
			string name;
			if (dstPath.split().back() == "*" || dstPath.split().back() == "*.*")
			{
				name = src.FileName();
			}
			else
			{
				string ret = dstPath.split().back();
				name = src.FileName();
			}

			//Path tempdstpath = dstPath.Parent().append(name);

			//NEW
			Path tempdstpath;
			if (!dstPath.IsReal())
			{
				CellNode* targetNode = GetDstNodeByPath(dstPath.Parent());
				if (targetNode == NULL)
				{
					m_VirtualDisk->LogMsgToConsole("路径不存在");
					return false;
				}
				tempdstpath = targetNode->GetNodePath().append(name);
			}
			else
			{
				tempdstpath = dstPath.Parent().append(name);
			}
			//END_NEW

			Copy(src, tempdstpath, true);
		}
		else
		{
			//目标路径中没有通配符
			Copy(src, dstPath, i == 0);
		}
	}
	return true;
}

void CopyCmd::Copy(Path & src, Path & dst, bool first)
{
	if (dst.IsReal())
	{
		//R-R
		if (src.IsReal())
		{
			string cmd = StrProcess::sstr("copy \"%s\" \"%s\"", src.str().c_str(), dst.str().c_str());	//交由系统执行
			system(cmd.c_str());
		}
		else
		{		
			//V-R
			_finddata_t findData;
			intptr_t handle = _findfirst(dst.str().c_str(), &findData);
			if (handle == -1)
			{
				//没有相同的文件
				CopyNodeToReal(src, dst);
			}
			else if (findData.attrib & _A_RDONLY)
			{
				m_VirtualDisk->LogMsgToConsole("文件只读，只能作为复制源");
				return;
			}
			else if (findData.attrib & _A_SUBDIR)
			{
				//将该文件拷贝到dst指向的文件夹中，文件名成与原名称相同
				Copy(src, dst.Self().append(src.FileName()), true);
			}
			else
			{
				if (findData.name == src.FileName())
				{
					if (needAsk)
					{
						//用户输入命令；
						//目标节点存在且需要询问
						while (true)
						{
							string answer = m_VirtualDisk->AskForUserInput(StrProcess::sstr("覆盖 %s 吗? (Yes/No/All):", dst.str().c_str()));
							transform(answer.begin(), answer.end(), answer.begin(), tolower);
							if (answer == "y" || answer == "yes")
							{
								break;
							}
							else if (answer == "n" || answer == "no")
							{
								return;
							}
							else if (answer == "all")
							{
								needAsk = false;
								break;
							}
						}
					}					
				}
				if (first)
				{
					//如果文件存在则 清空其内容；   ？？？？？为什么无效？
					FILE* fout = fopen(dst.str().c_str(), "wb");
					fclose(fout);
				}
				CopyNodeToReal(src, dst);
			}
		}
	}
	else   //dst is virtualdiskNode
	{
		//CellNode* dstNode = m_VirtualDisk->GetNodeByPath(dst);

		//NEW
		CellNode* dstNode = GetDstNodeByPath(dst);
		if (!dstNode)
		{
			//创建一个目标节点
			string filenametemp = dst.FileName();
			if (filenametemp.empty())
			{
				return;
			}
			//CellNode* nodeprelink = m_VirtualDisk->GetNodeByPath(dst.Self().append(".."));

			//NEW
			CellNode* nodeprelink = GetDstNodeByPath(dst.Self().append(".."));

			nodeprelink = m_VirtualDisk->LookingForTaget(nodeprelink);
			if (!nodeprelink)
			{
				m_VirtualDisk->LogMsgToConsole("路径不存在");
				return;
			}
			FileNode* node = new FileNode();
			node->SetCellName(dst.FileName());
			//node->SetNodeType(FileNodeType::FILE_CUSTOM);
			nodeprelink->AddSubNode(node);
			if (!node)
			{
				//创建失败，返回
				return;
			}
			else if(src.IsReal())
			{
				//创建成功，返回
				CopyRealToNode(src, node);
			}
			else
			{
				//src已经经过判断，可以直接使用
				//node也经过判断，可以直接使用
				CellNode* srcNode = m_VirtualDisk->GetNodeByPath(src);
				if (!srcNode) { return; }
				node->Content().insert(node->Content().end(), srcNode->Content().begin(), srcNode->Content().end());
				node->SetLastModifyTime(srcNode->GetlastModifyTime());
			}
		}
		else if (dstNode->GetNodeType()&FileNodeType::FOLD)
		{
			CellNode* node = m_VirtualDisk->LookingForTaget(dstNode);
			if (!node)
			{
				m_VirtualDisk->LogMsgToConsole("路径不存在");
				return;
			}
			//在该文件夹末尾添加要复制的文件名，再次进入Copy命令，执行创建节点的语句
			node->GetNodePath();
			Copy(src, node->GetNodePath().Self().append(src.FileName()), true);
			//Copy(src, dst.Self().append(src.FileName()), true);
		}
		else if (dstNode->GetNodeType()&FileNodeType::FILE_CUSTOM)
		{

			//当前节点是文件，或者指向文件的符号链接，，，找到最终节点进行操作
			CellNode* prelink = dstNode;
			CellNode* tempNode = m_VirtualDisk->LookingTarget(prelink);
			while (tempNode&& tempNode != prelink)
			{
				prelink = tempNode;
				tempNode = m_VirtualDisk->LookingTarget(tempNode);
			}
			//得到最终节点 ，最终节点若不存在，则说明其父节点是文件节点，执行拷贝
			if (!tempNode)
			{
				Path tempdstpath = prelink->GetTargetNodePath();
				Copy(src, tempdstpath, first);
			}
			else if (m_VirtualDisk->LookingForTaget(m_VirtualDisk->GetNodeByPath(src)) == tempNode)
			{
				m_VirtualDisk->LogMsgToConsole("源节点与目标节点相同");
			}
			else
			{
				if (dst.FileName() == src.FileName())
				{
					if (needAsk)
					{
						//用户输入命令；
						//目标节点存在且需要询问
						while (true)
						{
							string answer = m_VirtualDisk->AskForUserInput(StrProcess::sstr("覆盖 %s 吗? (Yes/No/All):", dst.str().c_str()));
							transform(answer.begin(), answer.end(), answer.begin(), tolower);
							if (answer == "y" || answer == "yes")
							{
								break;
							}
							else if (answer == "n" || answer == "no")
							{
								return;
							}
							else if (answer == "all")
							{
								needAsk = false;
								break;
							}
						}
					}
				}
				if (first)
				{
					tempNode->Content().clear();
				}
				if (src.IsReal())
				{
					CopyRealToNode(src, tempNode);
				}
				else
				{
					//源节点和目标节点都已经过判断
					CellNode* srcNode = m_VirtualDisk->GetNodeByPath(src);
					if (!srcNode) { return; }
					tempNode->Content().insert(tempNode->Content().end(), srcNode->Content().begin(), srcNode->Content().end());
					tempNode->SetLastModifyTime(srcNode->GetlastModifyTime());
				}
			}
		}
		else
		{
			m_VirtualDisk->LogMsgToConsole("情况不对，撤退");
			return;
		}
	}
}



void CopyCmd::CopyNodeToReal(Path& src, Path& dst)
{
	//已经对srcnode经过判断，此处可直接使用
	CellNode* nodesrc = m_VirtualDisk->GetNodeByPath(src);
	if (!nodesrc){return;}
	
	//如果文件不存在则创建该文件， 并以二进制只写方式打开该文件
	FILE* fout = fopen(dst.str().c_str(), "ab");
	if (!fout)
	{
		m_VirtualDisk->LogMsgToConsole("本地文件打开失败");
		return;
	}
	//将源节点中的数据内容以字符为单位写入到真实文件中去
	if (nodesrc->Content().size() > 0)
	{
		fwrite(&nodesrc->Content().front(), 1, nodesrc->Content().size(), fout);
	}
	fclose(fout);
}

void CopyCmd::CopyRealToNode(Path& src, CellNode* dstNode)
{
	if (!dstNode) { return; }
	//读取文件流写入到虚拟节点的content中
	//以二进制只读方式打开该文件
	FILE* fin = fopen(src.str().c_str(), "rb");
	if (!fin)
	{
		m_VirtualDisk->LogMsgToConsole("本地文件打开失败");
		return;
	}
	char buff[1024];
	//不到文件结尾的话就一直继续读取
	while (!feof(fin))
	{
		size_t count = fread(buff, 1, sizeof(buff), fin);
		dstNode->Content().insert(dstNode->Content().end(), buff, buff + count);
	}
	fclose(fin);
}


CellNode* CopyCmd::GetDstNodeByPath(const Path &to)
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
			return NULL;
		}
		else if (targetNode->GetNodeType()& FOLD)
		{
			curNode = targetNode;
		}
		else if (targetNode->GetNodeType()& FILE_CUSTOM)
		{
			//是一个文件,判断是否是最终节点
			if (++i < path.size())
			{
				--i;
				m_VirtualDisk->LogMsgToConsole("路径中不应该出现文件");
				return NULL;
			}
		}
	}
	return targetNode;
}
