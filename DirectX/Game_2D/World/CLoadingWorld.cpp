#include "CLoadingWorld.h"

#include <CThreadManager.h>
#include <Render/CRenderManager.h>
#include <World/CWorld.h>
#include <World/CWorldManager.h>

#include "../Thread/CLoadingThread.h"
#include "../UI/CLoadingWidget.h"

CLoadingWorld::CLoadingWorld()
{
}

CLoadingWorld::~CLoadingWorld()
{
	CThreadManager::GetInst().DeleteThread("LoadingThread");
}

bool CLoadingWorld::Init()
{
	if (!CWorld::Init())
	{
		return false;
	}

	LoadAnimation2D();

	LoadSound();

	CreateUI();

	return true;
}

void CLoadingWorld::Update(const float DeltaTime)
{
	CWorld::Update(DeltaTime);

	if (Thread && Thread->GetComplete())
	{
		CWorldManager::GetInst()->CompleteAsyncWorld();

		if (LoadType == EWorldType::Editor)
		{
			CRenderManager::GetInst()->SetDebugTarget(false);
		}
	}
}

void CLoadingWorld::Load(EWorldType WorldType)
{
	LoadType = WorldType;

	Thread = CThreadManager::GetInst().Create<CLoadingThread>("LoadingThread");
	if (Thread)
	{
		Thread->SetWorldType(WorldType);

		Thread->Resume();
	}
}

void CLoadingWorld::LoadAnimation2D()
{
}

void CLoadingWorld::LoadSound()
{
	WorldAssetManager->LoadSound("BlobfishMove", "Effect", false, "Brotato\\ui\\splash\\blobfish_move.wav", "Asset");

	WorldAssetManager->SoundPlay("BlobfishMove");
}

void CLoadingWorld::CreateUI()
{
	auto Widget = UIManager->CreateWidget<CLoadingWidget>("LoadingWidget");
}
