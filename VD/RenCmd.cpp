#include "RenCmd.h"
#include "VirtualDisk.h"
bool RenCmd::Execute(VirtualDisk * virtualdisk)
{
	m_VirtualDisk = virtualdisk;
	//类似于Mklink的路径存取方法

	//第一个路径存放着待修改节点的路径信息
	//第二个路径存放着要修改成的名字

	//查找检测第一个路径所指向节点的状态
		//不存在 -》 给出提示并退出
		//存在 -》 是否正在被使用
					//是	-》 退出
					//否	-》 目标名是否合法
							//合法 -》修改节点名

	vector<string> pathItems = cmdParaCollection.m_pathItems;
	Path srcNodePath(pathItems[0]);
	string applyName = pathItems[1];

	CellNode* srcNode = m_VirtualDisk->GetNodeByPath(srcNodePath);

	if (!srcNode)
	{
		m_VirtualDisk->LogMsgToConsole("目标节点不存在");
		return false;
	}
	else if (m_VirtualDisk->IfNodeBeUsing(srcNode))
	{
		m_VirtualDisk->LogMsgToConsole("目标路径是工作路径");
		return false;
	}
	else if (!Tools::IsLegalFileName(applyName))
	{
		m_VirtualDisk->LogMsgToConsole("名称不合法");
		return false;
	}
	else if (srcNode->GetParent()->GetNode(applyName))
	{
		m_VirtualDisk->LogMsgToConsole("存在同名文件");
		return false;
	}
	else
	{
		srcNode->SetCellName(applyName);
	}
	return true;
}
