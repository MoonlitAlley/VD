#include "FileNode.h"

FileNode::FileNode():CellNode()
{
	nodeType = FileNodeType::FILE_CUSTOM;
}

vector<char>& FileNode::Content()
{
	return content;
}

string FileNode::GetNodeTypeStr()
{
	return "";
}
