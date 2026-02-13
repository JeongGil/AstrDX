#pragma once
#include "CSceneComponent.h"

class CTileMapComponent;
class CCBufferTransform;
class CRenderState;
class CTexture;
class CMesh;
class CShader;
class CGameObject;

class CTileMapRender :
	public CSceneComponent
{
	friend CGameObject;
	friend CObject;

protected:
	CTileMapRender();
	CTileMapRender(const CTileMapRender& other);
	CTileMapRender(CTileMapRender&& other) noexcept;

public:	
	~CTileMapRender() override;

protected:
	std::weak_ptr<CShader> BackShader;
	std::weak_ptr<CMesh> BackMesh;
	std::weak_ptr<CTexture> Textures[ETileTextureType::End];
	std::weak_ptr<CRenderState> AlphaBlend;
	std::weak_ptr<CRenderState> TileAlphaBlend;
	std::shared_ptr<CCBufferTransform> TransformCBuffer;
	std::weak_ptr<CTileMapComponent> TileMap;
	std::vector<FTileFrame> TileFrames;

public:
	bool Init() override;
	void Render() override;

protected:
	CTileMapRender* Clone() const override;

public:
	void SetTileMapComponent(const std::weak_ptr<CTileMapComponent>& TileMap);
	void EnableAlphaBlend();
	void EnableTileAlphaBlend();
	bool SetTexture(ETileTextureType::Type Type, const std::weak_ptr<CTexture>& Texture);
	bool SetTexture(ETileTextureType::Type Type, const std::string& Key);
	bool SetTexture(ETileTextureType::Type Type, const std::string& Key, const TCHAR* FileName, const std::string& PathName = "Texture");
	bool SetTextureFullPath(ETileTextureType::Type Type, const std::string& Key, const TCHAR* FullPath);
	bool SetTextureFullPath(ETileTextureType::Type Type, const std::string& Key, const std::vector<const TCHAR*>& FullPaths);
	void SetBackMesh(const std::weak_ptr<CMesh>& Mesh);
	void SetBackMesh(const std::string& Key);
	void SetBackShader(const std::weak_ptr<CShader>& Shader);
	void SetBackShader(const std::string& Key);
	void SetTileFrameSize(float x, float y);
	void AddTileFrame(const FVector2& Start, const FVector2& End);
	void AddTileFrame(float StartX, float StartY, float EndX, float EndY);

	void Save(FILE* File);
	void Load(FILE* File);

	std::weak_ptr<CTexture> GetTexture(ETileTextureType::Type Type) const
	{
		return Textures[Type];
	}
};

