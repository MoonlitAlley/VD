#pragma once

#include "Command.h"

class VirtualDiskInside
{
public:
	//����-����
	VirtualDiskInside();
	~VirtualDiskInside();

	//��ʽ��-���ڵ㴴��
	bool InitFileSystem();

	bool Execute(Command* cmd);

	CellNode* GetNodeByPath(const Path &path);

	CellNode* LookingTarget(CellNode* node);
	CellNode* LookingForTaget(CellNode * node);

	//�û�����
	void LogMsgToConsole(string msg);
	string AskForUserInput(const string& output);


	//GET-SET
	//�õ����ڵ�
	CellNode* GetRootNode();
	void SetRootNode(CellNode*);

	CellNode* GetWorkingNode();
	void SetWorkingNode(CellNode*);

	//�õ���ǰ�����ڵ�
	Path GetWorkingPath();
	//�õ���ǰ����·�����ַ���ʾ
	string GetWorkingPathString();

	bool IfNodeBeUsing(CellNode* node);
	

public:
	//������ʾ
	Path workingPath_CD = Path("c:/");
private:

	CellNode* rootNode;
	CellNode* workingNode;
	list<CellNode*> workingPath;
	string workingPathString;
};