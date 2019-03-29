#include "CopyCmd.h"
#include <io.h>
bool CopyCmd::Execute(VirtualDiskInside * virtualdisk)
{

	m_VirtualDisk = virtualdisk;
	//本命令要支持从物理磁盘拷贝内容

	//四种拷贝方式
	//V-R
	//R-V
	//V-V
	//R-R

	//第一个路径存放着源路径信息
	//第二个路径存放着目标路径信息


	//按照源路径信息找到所有待操作的点，放到操作列表中，
	//将操作列表中的点挨个移动到目标路径，
		//冲突时询问用户操作
		

	vector<string> pathItems = cmdParaCollection.m_pathItems;

	Path dstPath(pathItems[1]);

	vector<Path> allsrcPath;
	//a= pickupAllfile(pathItems[0], *m_VirtualDisk);


	//首先找到所有的待操作文件，放到路径链表中
	Path srcPath(pathItems[0]);


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
			}
			if (findData.attrib &_A_ARCH)
			{
				//是一个正常的文件
				allsrcPath.push_back(srcPath);
			}
			else if (findData.attrib & _A_SUBDIR)
			{
				//是一个文件夹
				fileHandle = _findfirst(srcPath.Self().append("*").str().c_str(), &findData);
				do
				{
					if (string(findData.name) == "." || string(findData.name) == "..");
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


		}
	}
	else
	{
		//不是真实路径

		if (!Tools::isWildcard(srcPath.split().back()))
		{
			//输入的路径中不包括通配符

		}
		else
		{
			//含有通配符

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
			string name;
		}
	}






	return false;
}
