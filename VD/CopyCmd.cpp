#include "CopyCmd.h"
#include <io.h>
bool CopyCmd::Execute(VirtualDiskInside * virtualdisk)
{

	m_VirtualDisk = virtualdisk;
	//������Ҫ֧�ִ�������̿�������

	//���ֿ�����ʽ
	//V-R
	//R-V
	//V-V
	//R-R

	//��һ��·�������Դ·����Ϣ
	//�ڶ���·�������Ŀ��·����Ϣ


	//����Դ·����Ϣ�ҵ����д������ĵ㣬�ŵ������б��У�
	//�������б��еĵ㰤���ƶ���Ŀ��·����
		//��ͻʱѯ���û�����
		

	vector<string> pathItems = cmdParaCollection.m_pathItems;

	Path dstPath(pathItems[1]);

	vector<Path> allsrcPath;
	//a= pickupAllfile(pathItems[0], *m_VirtualDisk);


	//�����ҵ����еĴ������ļ����ŵ�·��������
	Path srcPath(pathItems[0]);


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
			}
			if (findData.attrib &_A_ARCH)
			{
				//��һ���������ļ�
				allsrcPath.push_back(srcPath);
			}
			else if (findData.attrib & _A_SUBDIR)
			{
				//��һ���ļ���
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
			//����ͨ���


		}
	}
	else
	{
		//������ʵ·��

		if (!Tools::isWildcard(srcPath.split().back()))
		{
			//�����·���в�����ͨ���

		}
		else
		{
			//����ͨ���

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
			string name;
		}
	}






	return false;
}
