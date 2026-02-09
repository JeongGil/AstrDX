#pragma once
#include "CShader.h"
class CGraphicShader :
    public CShader
{
	friend class CShaderManager;

public:
	bool Init(const std::string& PathName) override = 0;
	void SetShader() final;

	/*
	 LPCSTR SemanticName;							register name
	 UINT SemanticIndex;							register number
	 DXGI_FORMAT Format;							Specify the format for the number of bytes.
	 UINT InputSlot;								vertex buffer number
	 UINT AlignedByteOffset;						Specifies which byte within the structure the element is located.
	 D3D11_INPUT_CLASSIFICATION InputSlotClass;		Determine whether it is vertex data or instance data.
	 UINT InstanceDataStepRate;						Used only for instancing.
	 */
	void AddInputDesc(const char* Semantic, UINT SemanticIndex, DXGI_FORMAT Format, UINT InputSlot, UINT Size,
		D3D11_INPUT_CLASSIFICATION InputSlotClass, UINT InstanceDataStepRate);

	bool CreateInputLayout();

	bool LoadVertexShader(const char* EntryName, const TCHAR* FileName, const std::string& PathName = "Shader");
	bool LoadPixelShader(const char* EntryName, const TCHAR* FileName, const std::string& PathName = "Shader");

protected:
	// VertexShader, PixelShader, DomainShader, HullShader, GeometryShader
	ID3D11InputLayout* InputLayout = nullptr;

	// D3D11_INPUT_ELEMENT_DESC: Each element of the input register requires a single desc.
	std::vector<D3D11_INPUT_ELEMENT_DESC> InputDescs;

	UINT Offsets[2]{};

	ID3D11VertexShader* VS = nullptr;
	// Buffer for bytecode after compilation.
	ID3DBlob* VSBlob = nullptr;

	ID3D11PixelShader* PS = nullptr;
	ID3DBlob* PSBlob = nullptr;

protected:
	CGraphicShader();

public:
	~CGraphicShader() override = 0;
};

