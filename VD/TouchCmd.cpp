#include "TouchCmd.h"
#include "VirtualDisk.h"
bool TouchCmd::Execute(VirtualDisk * virtualdisk)
{
	vector<string> pathItems = cmdParaCollection.m_pathItems;

	for (size_t i = 0; i < pathItems.size(); i++)
	{
		Path path = pathItems[i];
		//查看当前文件是否已经存在

		//创建文件的命令 不检查符号链接，仅判断文件名是否相同
		//如果存在，给出提示，
		//如果不存在，创建该文件

		CellNode* node;
		node = virtualdisk->GetNodeByPath(path);
		if (node)
		{
			virtualdisk->LogMsgToConsole("存在同名文件");
		}
		else
		{
			CellNode* prelink = virtualdisk->GetNodeByPath(path.Parent());

			//创建节点，修改时间在构造函数中实现
			//设置父节点在添加函数中实现
			CellNode* fileNode = new CellNode();
			//该位置可能出错
			virtualdisk->LogMsgToConsole("该位置可能出错，当文件名为空时也会创建：bool TouchCmd::Execute(VirtualDisk * virtualdisk) ");

			fileNode->SetCellName(path.FileName());
			fileNode->SetNodeType(FileNodeType::FILE_CUSTOM);
			prelink->AddSubNode(fileNode);

			//设置文件内容；**********************************可能不方便测试
			//默认为空
		}
	}
	return true;
}
