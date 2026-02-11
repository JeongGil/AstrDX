#include "CRenderInstancing.h"

#include "CRenderManager.h"
#include "CRenderState.h"
#include "../CDevice.h"
#include "../Utils.h"
#include "../Asset/CAssetManager.h"
#include "../Asset/Mesh/CMesh.h"
#include "../Asset/Shader/CShader.h"
#include "../Asset/Shader/CShaderManager.h"
#include "../Component/CAnimation2DComponent.h"
#include "../Component/CSceneComponent.h"
#include "../World/CWorld.h"

constexpr int INSTANCING_START_COUNT{ 5 };

bool SortYRenderList(const std::weak_ptr<CSceneComponent>& A, const std::weak_ptr<CSceneComponent>& B)
{
	auto Src = A.lock();
	auto Dest = B.lock();

	if (!Src)
	{
		return true;
	}

	if (!Dest)
	{
		return false;
	}

	return Src->GetWorldPosition().y > Dest->GetWorldPosition().y;
}
CRenderInstancing::CRenderInstancing()
{
}

CRenderInstancing::~CRenderInstancing()
{
}

bool CRenderInstancing::CompareAsset(const std::weak_ptr<CMesh>& Mesh, const std::weak_ptr<CTexture>& Texture)
{
	return IsSameTarget(this->Mesh, Mesh) && IsSameTarget(this->Texture, Texture);
}

bool CRenderInstancing::CheckMesh(const std::weak_ptr<CMesh>& Mesh) const
{
	return IsSameTarget(this->Mesh, Mesh);
}

bool CRenderInstancing::CheckTexture(const std::weak_ptr<CTexture>& Texture) const
{
	return IsSameTarget(this->Texture, Texture);
}

void CRenderInstancing::SetMesh(const std::weak_ptr<CMesh>& Mesh)
{
	this->Mesh = Mesh;
}

void CRenderInstancing::SetTexture(const std::weak_ptr<CTexture>& Texture)
{
	this->Texture = Texture;
}

void CRenderInstancing::AddRenderComponent(const std::weak_ptr<CSceneComponent>& Component)
{
	auto Comp = Component.lock();
	if (!Comp || !Comp->GetAlive() || !Comp->GetEnable())
	{
		return;
	}

	if (RenderComponents.empty())
	{
		World = Comp->GetWorld();
		BlendState = Comp->GetBlendState();

		if (auto Shader = Comp->GetShader().lock())
		{
			std::string Key = Shader->GetKey() + "Instancing";
			if (auto ShaderMgr = CAssetManager::GetInst()->GetShaderManager().lock())
			{
				this->Shader = ShaderMgr->FindShader(Key);
			}
		}
	}

	RenderComponents.push_back(Component);

	std::erase_if(RenderComponents, [](const std::weak_ptr<CSceneComponent>& Weak)
		{
			auto Comp = Weak.lock();
			if (!Comp || !Comp->GetAlive())
			{
				return true;
			}

			return false;
		});

	if (!bRender)
	{
		if (RenderComponents.size() >= INSTANCING_START_COUNT)
		{
			bRender = true;

			if (!InstancingBuffer.Buffer)
			{
				CreateInstancingBuffer(sizeof(FInstancingData), INSTANCING_START_COUNT);
			}

			auto RenderCompsView = RenderComponents
				| std::views::transform([](auto& Weak) {return Weak.lock(); })
				| std::views::filter([](const auto& Comp) {return Comp != nullptr; });

			for (const auto& Comp : RenderCompsView)
			{
				Comp->SetRenderOption(EComponentRenderOption::Instancing);
			}
		}
	}
	else if (RenderComponents.size() < INSTANCING_START_COUNT)
	{
		bRender = false;

		auto RenderCompsView = RenderComponents
			| std::views::transform([](auto& Weak) {return Weak.lock(); })
			| std::views::filter([](const auto& Comp) {return Comp != nullptr; });

		for (const auto& Comp : RenderCompsView)
		{
			Comp->SetRenderOption(EComponentRenderOption::Normal);
		}
	}
	else
	{
		Comp->SetRenderOption(EComponentRenderOption::Instancing);
	}
}

void CRenderInstancing::Update(const float DeltaTime)
{
	if (!bRender || RenderComponents.empty())
	{
		return;
	}

	if (RenderComponents.size() > 1)
	{
		RenderComponents.sort(CRenderManager::SortYRenderList);
	}

	InstancingData.resize(RenderComponents.size());
	InstancingCount = 0;

	auto World = this->World.lock();
	if (!World)
	{
		return;
	}

	auto CamMgr = World->GetCameraManager().lock();
	if (!CamMgr)
	{
		return;
	}

	auto Mesh = this->Mesh.lock();
	if (!Mesh)
	{
		return;
	}

	auto Texture = this->Texture.lock();

	auto RenderCompsView = RenderComponents
		| std::views::transform([](const auto& Weak) { return Weak.lock(); })
		| std::views::filter([](const auto& Comp) {return Comp != nullptr; });
	for (const auto& Comp : RenderCompsView)

	{
		// 인스턴싱용 버퍼를 채워준다.
		FMatrix	ScaleMat, RotMat, TranslateMat, WorldMat;

		ScaleMat.Scaling(Comp->GetWorldScale());
		RotMat.Rotation(Comp->GetWorldRotation());
		TranslateMat.Translation(Comp->GetWorldPosition());

		WorldMat = ScaleMat * RotMat * TranslateMat;

		FMatrix	ViewMat = CamMgr->GetViewMatrix();
		FMatrix	ProjMat = CamMgr->GetProjMatrix();

		FMatrix	WVPMat = WorldMat * ViewMat * ProjMat;

		auto& InstancingDatum = InstancingData[InstancingCount];

		InstancingDatum.WVP0 = WVPMat[0];
		InstancingDatum.WVP1 = WVPMat[1];
		InstancingDatum.WVP2 = WVPMat[2];
		InstancingDatum.WVP3 = WVPMat[3];

		InstancingDatum.ArrayTextureEnable = 0;
		InstancingDatum.AnimFrame = 0;

		if (auto AnimCom = Comp->GetAnimComponent().lock())
		{
			InstancingDatum.LTUV = AnimCom->GetAnimLTUV();
			InstancingDatum.RBUV = AnimCom->GetAnimRBUV();

			InstancingDatum.AnimFrame = AnimCom->GetCurrentFrame();

			if (AnimCom->GetTextureType() == EAnimation2DTextureType::Array)
			{
				InstancingDatum.ArrayTextureEnable = 1;
			}
		}
		else
		{
			if (Texture && Texture->GetTextureType() == ETextureType::Array)
			{
				InstancingDatum.ArrayTextureEnable = 1;
			}

			InstancingDatum.LTUV = FVector2(0.f, 0.f);
			InstancingDatum.RBUV = FVector2(1.f, 1.f);
		}

		FVector Pivot = Comp->GetPivot();
		FVector PivotSize = Pivot * Mesh->GetMeshSize();

		InstancingDatum.PivotSize = PivotSize;
		InstancingDatum.BaseColor = Comp->GetBaseColor();

		++InstancingCount;
	}

	SetInstancingData(&InstancingData[0], InstancingCount);
}

bool CRenderInstancing::CreateInstancingBuffer(int Size, int Count)
{
	SAFE_RELEASE(InstancingBuffer.Buffer);

	InstancingBuffer.Size = Size;
	InstancingBuffer.Count = Count;

	D3D11_BUFFER_DESC BufferDesc{};
	BufferDesc.ByteWidth = Size * Count;
	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&BufferDesc, nullptr, &InstancingBuffer.Buffer)))
	{
		return false;
	}

	return true;
}

bool CRenderInstancing::SetInstancingData(void* Data, int Count)
{
	if (!InstancingBuffer.Buffer)
	{
		return false;
	}

	if (InstancingBuffer.Count < Count)
	{
		if (!CreateInstancingBuffer(InstancingBuffer.Size, Count * 2))
		{
			return false;
		}
	}

	ID3D11DeviceContext* Context = CDevice::GetInst()->GetContext();

	D3D11_MAPPED_SUBRESOURCE MS{};

	Context->Map(InstancingBuffer.Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MS);

	memcpy(MS.pData, Data, InstancingBuffer.Size * Count);

	Context->Unmap(InstancingBuffer.Buffer, 0);

	return true;
}

void CRenderInstancing::Render()
{
	if (!bRender || RenderComponents.empty())
	{
		return;
	}

	auto BlendState = this->BlendState.lock();
	if (BlendState)
	{
		BlendState->SetState();
	}

	auto Mesh = this->Mesh.lock();
	auto Shader = this->Shader.lock();
	auto Texture = this->Texture.lock();

	auto RenderCompView = RenderComponents
		| std::views::transform([](const auto& Weak) {return Weak.lock(); })
		| std::views::filter([](const auto& Comp) {return Comp != nullptr; })
		| std::views::filter([](const auto& Comp) {return Comp->GetAlive(); })
		| std::views::filter([](const auto& Comp) {return Comp->GetEnable(); });

	for (const auto& Comp : RenderCompView)
	{
		Comp->SetRenderOption(EComponentRenderOption::Normal);
	}

	if (Texture)
	{
		int Register = 0;
		if (Texture->GetTextureType() == ETextureType::Array)
		{
			Register = 1;
		}

		Texture->SetShader(Register, EShaderBufferType::Pixel, 0);
	}

	Shader->SetShader();
	Mesh->RenderInstancing(InstancingBuffer, InstancingCount);

	if (BlendState)
	{
		BlendState->ResetState();
	}
}

void CRenderInstancing::RenderClear()
{
	RenderComponents.clear();
}
