#pragma once

#include "../../CObject.h"

class CShader
{
	friend class CShaderManager;

public:
	virtual bool Init() = 0;
	virtual void SetShader() = 0;

protected:
	CShader();

public:
	virtual ~CShader();
};

