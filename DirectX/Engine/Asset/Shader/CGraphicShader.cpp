#include "CGraphicShader.h"

#include "../CPathManager.h"
#include "../../CDevice.h"

void CGraphicShader::SetShader()
{
	CDevice::GetInst()->GetContext()->VSSetShader(VS, nullptr, 0);
	CDevice::GetInst()->GetContext()->PSSetShader(PS, nullptr, 0);

	CDevice::GetInst()->GetContext()->IASetInputLayout(InputLayout);
}

void CGraphicShader::AddInputDesc(const char* Semantic, UINT SemanticIndex, DXGI_FORMAT Format, UINT InputSlot,
                                  UINT Size, D3D11_INPUT_CLASSIFICATION InputSlotClass, UINT InstanceDataStepRate)
{
	InputDescs.emplace_back(Semantic, SemanticIndex, Format, InputSlot, Offset, InputSlotClass, InstanceDataStepRate);
	Offset += Size;
}

bool CGraphicShader::CreateInputLayout()
{
	// An Input Layout represents the structure of a vertex in terms of its composition.
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateInputLayout(
		&InputDescs[0],
		static_cast<UINT>(InputDescs.size()),
		VSBlob->GetBufferPointer(),
		VSBlob->GetBufferSize(),
		&InputLayout)))
	{
		return false;
	}

	return true;
}

bool CGraphicShader::LoadVertexShader(const char* EntryName, const TCHAR* FileName)
{
	const TCHAR* BasePath = CPathManager::FindPath("Shader");
	TCHAR FullPath[MAX_PATH] = {};

	lstrcpy(FullPath, BasePath);
	lstrcat(FullPath, FileName);

	UINT Flag = 0;
#ifdef _DEBUG
	Flag = D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* ErrorBlob = nullptr;

	// Compiles the Shader file in this path.
	// 1. The path where the Shader file is located.
	// 2. Macro.
	// 3. Specifies how to handle includes.
	// 4. The name of the function to compile.
	// 5. Specifies the Shader version.
	// 6, 7. Additional options.
	// 8. The bytecode of the compiled Shader.
	// 9. A buffer to retrieve error messages in case of errors.
	if (FAILED(D3DCompileFromFile(FullPath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, EntryName,
		"vs_5_0", Flag, 0, &VSBlob, &ErrorBlob)))
	{
#ifdef _DEBUG
		char ErrorText[1024] = {};
		strcpy_s(ErrorText, static_cast<const char*>(ErrorBlob->GetBufferPointer()));
		strcpy_s(ErrorText, "\n");

		OutputDebugStringA(ErrorText);
#endif

		return false;
	}

	// Creates a VertexShader object.
	// 1. Compiled Shader bytecode.
	// 2. Size of the bytecode.
	// 3. Link.
	// 4. The Vertex Shader object to be created.
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateVertexShader(
		VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), nullptr, &VS)))
	{
		return false;
	}

	return true;
}

bool CGraphicShader::LoadPixelShader(const char* EntryName, const TCHAR* FileName)
{
	const TCHAR* BasePath = CPathManager::FindPath("Shader");
	TCHAR FullPath[MAX_PATH] = {};

	lstrcpy(FullPath, BasePath);
	lstrcat(FullPath, FileName);

	UINT Flag = 0;
#ifdef _DEBUG
	Flag = D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* ErrorBlob = nullptr;

	// Compiles the Shader file in this path.
	// 1. The path where the Shader file is located.
	// 2. Macro.
	// 3. Specifies how to handle includes.
	// 4. The name of the function to compile.
	// 5. Specifies the Shader version.
	// 6, 7. Additional options.
	// 8. The bytecode of the compiled Shader.
	// 9. A buffer to retrieve error messages in case of errors.
	if (FAILED(D3DCompileFromFile(FullPath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, EntryName,
		"ps_5_0", Flag, 0, &PSBlob, &ErrorBlob)))
	{
#ifdef _DEBUG
		char ErrorText[1024] = {};
		strcpy_s(ErrorText, static_cast<const char*>(ErrorBlob->GetBufferPointer()));
		strcpy_s(ErrorText, "\n");

		OutputDebugStringA(ErrorText);
#endif

		return false;
	}

	// Creates a VertexShader object.
	// 1. Compiled Shader bytecode.
	// 2. Size of the bytecode.
	// 3. Link.
	// 4. The Vertex Shader object to be created.
	if (FAILED(CDevice::GetInst()->GetDevice()->CreatePixelShader(
		PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), nullptr, &PS)))
	{
		return false;
	}

	return true;
}

CGraphicShader::CGraphicShader()
{
}

CGraphicShader::~CGraphicShader()
{
	SAFE_RELEASE(InputLayout);
	SAFE_RELEASE(VS);
	SAFE_RELEASE(VSBlob);
	SAFE_RELEASE(PS);
	SAFE_RELEASE(PSBlob);
}
