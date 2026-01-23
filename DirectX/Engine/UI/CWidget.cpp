#include "CWidget.h"

#include "../Asset/CAssetManager.h"
#include "../Asset/Mesh/CMeshManager.h"
#include "../Asset/Shader/CCBufferTransform.h"
#include "../Asset/Shader/CCBufferUIDefault.h"
#include "../Asset/Shader/CShaderManager.h"
#include "../World/CWorld.h"

CWidget::CWidget()
{
}

CWidget::~CWidget()
{
}

void CWidget::SetShader(const std::weak_ptr<CShader>& Shader)
{
	this->Shader = Shader;
}

void CWidget::SetShader(const std::string& Key)
{
	if (auto ShaderMgr = CAssetManager::GetInst()->GetShaderManager().lock())
	{
		Shader = ShaderMgr->FindShader(Key);
	}
}

bool CWidget::Init()
{
	if (auto World = this->World.lock())
	{
		auto AssetMgr = World->GetWorldAssetManager().lock();
		if (auto Mesh = AssetMgr->FindMesh("RectTex").lock())
		{
			this->Mesh = Mesh;
		}
	}
	else
	{
		auto MeshMgr = CAssetManager::GetInst()->GetMeshManager().lock();
		Mesh = MeshMgr->FindMesh("Mesh_RectTex");
	}

	SetShader("UIDefault");

	TransformCBuffer.reset(new CCBufferTransform);
	TransformCBuffer->Init();

	UIDefaultCBuffer.reset(new CCBufferUIDefault);
	UIDefaultCBuffer->Init();

	return true;
}

void CWidget::Update(const float DeltaTime)
{
}

void CWidget::Render()
{
}
