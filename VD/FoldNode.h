#pragma once
#include "CellNode.h"

class FoldNode :public CellNode
{
public:
	virtual bool AddSubNode(CellNode* node) override;
	virtual bool RemoveSubNode(CellNode* node, bool release) override;
	virtual list<CellNode*> FilterSubNode(const string& whildcard) override;

	virtual string GetNodeTypeStr() override;
};