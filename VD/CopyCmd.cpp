#define _CRT_SECURE_NO_WARNINGS
#include "CopyCmd.h"
#include "MyString.h"
#include "VirtualDiskInside.h"
#include <io.h>

//������Ҫ֧�ִ�������̿�������

//���ֿ�����ʽ
//V-R
//R-V
//V-V
//R-R

//��һ��·�������Դ·����Ϣ	֧��ͨ���
//�ڶ���·�������Ŀ��·����Ϣ		֧��ͨ���


//�õ����д�����·��
//�û����������ʵ·��	
	//�����·���в�����ͨ���
		//��һ���������ļ�
		//��һ���ļ���,���ļ����е������ļ��ڵ����������б�
	//����ͨ���
		//�õ��丸��·�����ڸ���·���н���ɸѡ

//�û�����Ĳ�����ʵ·��	
	//�����·���в�����ͨ���
		//��һ���������ļ�
		//��һ���ļ���,���ļ����е������ļ��ڵ����������б�
	//����ͨ���
		//�õ��丸��·�����ڸ���·���н���ɸѡ



//�õ��������б�

bool CopyCmd::Execute(VirtualDiskInside * virtualdisk)
{
	m_VirtualDisk = virtualdisk;
	needAsk = !(cmdParaCollection.m_parsedOptions.find("/y") != cmdParaCollection.m_parsedOptions.end());
	vector<string> pathItems = cmdParaCollection.m_pathItems;
	//��������������Ѿ��жϹ�·������
	Path srcPath(pathItems[0]);
	Path dstPath(pathItems[1]);


	//�����ҵ����еĴ������ļ����ŵ�·��������
	vector<Path> allsrcPath;
	//�û����������ʵ·��
	if (srcPath.IsReal())
	{
		//�����·���в�����ͨ���
		if (!Tools::isWildcard(srcPath.split().back()))
		{
			_finddata_t findData;
			intptr_t fileHandle = _findfirst(srcPath.str().c_str(), &findData);
			if (fileHandle == -1)
			{
				//����ʧ�ܣ�������·������
				m_VirtualDisk->LogMsgToConsole("·��������");
				return false;
			}
			if (findData.attrib &_A_ARCH)
			{
				//��һ���������ļ�
				allsrcPath.push_back(srcPath);
			}
			else if (findData.attrib & _A_SUBDIR)
			{
				//��һ���ļ���,���ļ����е������ļ��ڵ����������б�
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
			//����ͨ���
			_finddata_t findData;
			intptr_t fileHandle = _findfirst(srcPath.str().c_str(), &findData);
			if (fileHandle == -1)
			{
				//����ʧ�ܣ�������·������
				m_VirtualDisk->LogMsgToConsole("·��������");
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
					//���ж�һ�εõ��ļ��б��е�ǰ�ļ��Ƿ�ƥ���û������ͨ���
					continue;
				}
				allsrcPath.push_back(srcPath.Self().append("..").append(findData.name));

			} while (_findnext(fileHandle, &findData) == 0);
		}
	}
	else
	{
		//������ʵ·��
		if (!Tools::isWildcard(srcPath.split().back()))
		{
			//�����·���в�����ͨ���
			CellNode* node = m_VirtualDisk->GetNodeByPath(srcPath);
			if (node)
			{
				node = m_VirtualDisk->LookingForTaget(node);
				//·��ָ��ڵ���һ���ļ��������ļ���·������������б�
				if (node->GetNodeType()&FileNodeType::FILE_CUSTOM)
				{
					allsrcPath.push_back(node->GetNodePath());
				}
				else
				{
					//·��ָ��Ľڵ���һ���ļ��У����ļ����е������ļ��ڵ����������б�
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
				m_VirtualDisk->LogMsgToConsole("·��������");
				return false;
			}
		}
		else
		{
			//����ͨ���
			//�õ��丸��·�����ڸ���·���н���ɸѡ
			CellNode* dir = m_VirtualDisk->GetNodeByPath(srcPath.Self().append(".."));
			if (dir)
			{
				dir = m_VirtualDisk->LookingForTaget(dir);
				//�ڸ���·����ɸѡ�ļ��ڵ㣬���뵽�������б�
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
				m_VirtualDisk->LogMsgToConsole("·��������");
				return false;
			}
		}
	}

	if (allsrcPath.empty())
	{
		//û��Ҫ������ļ�
		return true;
	}
	for (size_t i = 0; i < allsrcPath.size(); i++)
	{
		Path& src(allsrcPath[i]);
		if (Tools::isWildcard(dstPath.split().back()))
		{
			//Ŀ��·������ͨ���
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
					m_VirtualDisk->LogMsgToConsole("·��������");
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
			//Ŀ��·����û��ͨ���
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
			string cmd = StrProcess::sstr("copy \"%s\" \"%s\"", src.str().c_str(), dst.str().c_str());	//����ϵͳִ��
			system(cmd.c_str());
		}
		else
		{		
			//V-R
			_finddata_t findData;
			intptr_t handle = _findfirst(dst.str().c_str(), &findData);
			if (handle == -1)
			{
				//û����ͬ���ļ�
				CopyNodeToReal(src, dst);
			}
			else if (findData.attrib & _A_RDONLY)
			{
				m_VirtualDisk->LogMsgToConsole("�ļ�ֻ����ֻ����Ϊ����Դ");
				return;
			}
			else if (findData.attrib & _A_SUBDIR)
			{
				//�����ļ�������dstָ����ļ����У��ļ�������ԭ������ͬ
				Copy(src, dst.Self().append(src.FileName()), true);
			}
			else
			{
				if (findData.name == src.FileName())
				{
					if (needAsk)
					{
						//�û��������
						//Ŀ��ڵ��������Ҫѯ��
						while (true)
						{
							string answer = m_VirtualDisk->AskForUserInput(StrProcess::sstr("���� %s ��? (Yes/No/All):", dst.str().c_str()));
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
					//����ļ������� ��������ݣ�   ����������Ϊʲô��Ч��
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
			//����һ��Ŀ��ڵ�
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
				m_VirtualDisk->LogMsgToConsole("·��������");
				return;
			}
			FileNode* node = new FileNode();
			node->SetCellName(dst.FileName());
			//node->SetNodeType(FileNodeType::FILE_CUSTOM);
			nodeprelink->AddSubNode(node);
			if (!node)
			{
				//����ʧ�ܣ�����
				return;
			}
			else if(src.IsReal())
			{
				//�����ɹ�������
				CopyRealToNode(src, node);
			}
			else
			{
				//src�Ѿ������жϣ�����ֱ��ʹ��
				//nodeҲ�����жϣ�����ֱ��ʹ��
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
				m_VirtualDisk->LogMsgToConsole("·��������");
				return;
			}
			//�ڸ��ļ���ĩβ���Ҫ���Ƶ��ļ������ٴν���Copy���ִ�д����ڵ�����
			node->GetNodePath();
			Copy(src, node->GetNodePath().Self().append(src.FileName()), true);
			//Copy(src, dst.Self().append(src.FileName()), true);
		}
		else if (dstNode->GetNodeType()&FileNodeType::FILE_CUSTOM)
		{

			//��ǰ�ڵ����ļ�������ָ���ļ��ķ������ӣ������ҵ����սڵ���в���
			CellNode* prelink = dstNode;
			CellNode* tempNode = m_VirtualDisk->LookingTarget(prelink);
			while (tempNode&& tempNode != prelink)
			{
				prelink = tempNode;
				tempNode = m_VirtualDisk->LookingTarget(tempNode);
			}
			//�õ����սڵ� �����սڵ��������ڣ���˵���丸�ڵ����ļ��ڵ㣬ִ�п���
			if (!tempNode)
			{
				Path tempdstpath = prelink->GetTargetNodePath();
				Copy(src, tempdstpath, first);
			}
			else if (m_VirtualDisk->LookingForTaget(m_VirtualDisk->GetNodeByPath(src)) == tempNode)
			{
				m_VirtualDisk->LogMsgToConsole("Դ�ڵ���Ŀ��ڵ���ͬ");
			}
			else
			{
				if (dst.FileName() == src.FileName())
				{
					if (needAsk)
					{
						//�û��������
						//Ŀ��ڵ��������Ҫѯ��
						while (true)
						{
							string answer = m_VirtualDisk->AskForUserInput(StrProcess::sstr("���� %s ��? (Yes/No/All):", dst.str().c_str()));
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
					//Դ�ڵ��Ŀ��ڵ㶼�Ѿ����ж�
					CellNode* srcNode = m_VirtualDisk->GetNodeByPath(src);
					if (!srcNode) { return; }
					tempNode->Content().insert(tempNode->Content().end(), srcNode->Content().begin(), srcNode->Content().end());
					tempNode->SetLastModifyTime(srcNode->GetlastModifyTime());
				}
			}
		}
		else
		{
			m_VirtualDisk->LogMsgToConsole("������ԣ�����");
			return;
		}
	}
}



void CopyCmd::CopyNodeToReal(Path& src, Path& dst)
{
	//�Ѿ���srcnode�����жϣ��˴���ֱ��ʹ��
	CellNode* nodesrc = m_VirtualDisk->GetNodeByPath(src);
	if (!nodesrc){return;}
	
	//����ļ��������򴴽����ļ��� ���Զ�����ֻд��ʽ�򿪸��ļ�
	FILE* fout = fopen(dst.str().c_str(), "ab");
	if (!fout)
	{
		m_VirtualDisk->LogMsgToConsole("�����ļ���ʧ��");
		return;
	}
	//��Դ�ڵ��е������������ַ�Ϊ��λд�뵽��ʵ�ļ���ȥ
	if (nodesrc->Content().size() > 0)
	{
		fwrite(&nodesrc->Content().front(), 1, nodesrc->Content().size(), fout);
	}
	fclose(fout);
}

void CopyCmd::CopyRealToNode(Path& src, CellNode* dstNode)
{
	if (!dstNode) { return; }
	//��ȡ�ļ���д�뵽����ڵ��content��
	//�Զ�����ֻ����ʽ�򿪸��ļ�
	FILE* fin = fopen(src.str().c_str(), "rb");
	if (!fin)
	{
		m_VirtualDisk->LogMsgToConsole("�����ļ���ʧ��");
		return;
	}
	char buff[1024];
	//�����ļ���β�Ļ���һֱ������ȡ
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
