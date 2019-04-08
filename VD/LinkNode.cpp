#include "LinkNode.h"

string LinkNode::GetTargetNodePath()
{
	return string(content.begin(), content.end());
}

void LinkNode::SetTargetNodePath(const string& TargetPath)
{
	content.assign(TargetPath.begin(), TargetPath.end());
}

string LinkNode::GetNodeTypeStr()
{
	return "<LINK>";
}
