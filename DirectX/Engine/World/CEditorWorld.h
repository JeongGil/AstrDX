#pragma once
#include "CWorld.h"

class CEditorWorld :
    public CWorld
{
public:
	CEditorWorld();
	~CEditorWorld() override;

	bool Init() override;

private:
	void LoadAnimation2D();
	void LoadSound();
	void CreateUI();
};

