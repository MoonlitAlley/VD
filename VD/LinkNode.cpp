#include "LinkNode.h"

vector<char> LinkNode::GetTargetNodePath()
{
	return content;
}

void LinkNode::SetTargetNodePath(string TargetPath)
{
	content.assign(TargetPath.begin(), TargetPath.end());
}

string LinkNode::GetNodeTypeStr()
{
	return "<LINK>";
}
