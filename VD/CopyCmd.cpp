#include "CopyCmd.h"

bool CopyCmd::Execute(VirtualDisk * virtualdisk)
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















	return false;
}
