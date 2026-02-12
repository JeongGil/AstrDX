#include "CEditorPlayer.h"

#include <CDevice.h>
#include <Component/CCameraComponent.h>
#include <Component/CObjectMovementComponent.h>
#include <Component/CTile.h>
#include <Object/CTileMapObject.h>
#include <World/CWorld.h>

#include "../UI/CEditorWidget.h"

CEditorPlayer::CEditorPlayer()
{
}

CEditorPlayer::~CEditorPlayer()
{
}

void CEditorPlayer::Begin()
{
	CGameObject::Begin();

	auto World = this->World.lock();
	if (!World)
	{
		return;
	}

	TileMap = World->FindObject<CTileMapObject>("TileMap");

	if (auto UIManager = World->GetUIManager().lock())
	{
		EditorWidget = UIManager->FindWidget<CEditorWidget>("EditorWidget");
		if (auto EditorWidget = this->EditorWidget.lock())
		{

		}
	}
}

bool CEditorPlayer::Init()
{
	if (!CGameObject::Init())
	{
		return false;
	}

	CameraComponent = CreateComponent<CCameraComponent>("PlayerCamera");
	if (auto Camera = CameraComponent.lock())
	{
		const auto& RS = CDevice::GetInst()->GetResolution();
		Camera->SetProjection(ECameraProjectionType::Orthogonal, 90.f,
			static_cast<float>(RS.Width), static_cast<float>(RS.Height), 1000.f);

		Camera->SetInheritRotation(false);
	}

	Movement = CreateComponent<CObjectMovementComponent>("Movement");
	if (auto Move = Movement.lock())
	{
		Move->SetUpdateComponent(CameraComponent);
		Move->SetSpeed(400.f);
	}

	if (auto World = this->World.lock())
	{
		if (auto Input = World->GetInput().lock())
		{
			Input->AddBindKey("MoveUp", 'W');
			Input->SetBindFunction<CEditorPlayer>("MoveUp", EInputType::Hold, this, &CEditorPlayer::MoveUp);

			Input->AddBindKey("MoveDown", 'S');
			Input->SetBindFunction<CEditorPlayer>("MoveDown", EInputType::Hold, this, &CEditorPlayer::MoveDown);

			Input->AddBindKey("MoveLeft", 'A');
			Input->SetBindFunction<CEditorPlayer>("MoveLeft", EInputType::Hold, this, &CEditorPlayer::MoveLeft);

			Input->AddBindKey("MoveRight", 'D');
			Input->SetBindFunction<CEditorPlayer>("MoveRight", EInputType::Hold, this, &CEditorPlayer::MoveRight);

			Input->AddBindKey("LeftClick", VK_LBUTTON);
			Input->SetBindFunction<CEditorPlayer>("LeftClick", EInputType::Hold, this, &CEditorPlayer::LeftClick);

			Input->AddBindKey("Save", 'S');
			Input->SetBindFunction<CEditorPlayer>("Save", EInputType::Press, this, &CEditorPlayer::Save);
			Input->SetKeyCtrl("Save", true);

			Input->AddBindKey("Load", 'O');
			Input->SetBindFunction<CEditorPlayer>("Load", EInputType::Press, this, &CEditorPlayer::Load);
			Input->SetKeyCtrl("Load", true);

			Input->AddBindKey("ChangeMode", '1');
			Input->SetBindFunction<CEditorPlayer>("ChangeMode", EInputType::Press, this, &CEditorPlayer::ChangeMode);

			Input->AddBindKey("ChangeTileType", '2');
			Input->SetBindFunction<CEditorPlayer>("ChangeTileType", EInputType::Press, this, &CEditorPlayer::ChangeTileType);

			Input->AddBindKey("ChangeFrame", '3');
			Input->SetBindFunction<CEditorPlayer>("ChangeFrame", EInputType::Press, this, &CEditorPlayer::ChangeFrame);
		}
	}

	return true;
}

void CEditorPlayer::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
	
	if (auto Tile = GetTile().lock())
	{
		if (HoveredTile && HoveredTile != Tile)
		{
			HoveredTile->SetOutlineColor(FColor::White);
		}

		HoveredTile = Tile;
		Tile->SetOutlineColor(FColor::Yellow);
	}
	else
	{
		if (HoveredTile)
		{
			HoveredTile->SetOutlineColor(FColor::White);
		}

		HoveredTile.reset();
	}
}

void CEditorPlayer::MoveUp()
{
	if (auto Move = Movement.lock())
	{
		Move->AddMove(GetAxis(EAxis::Y));
	}
}

void CEditorPlayer::MoveDown()
{
	if (auto Move = Movement.lock())
	{
		Move->AddMove(-GetAxis(EAxis::Y));
	}
}

void CEditorPlayer::MoveLeft()
{
	if (auto Move = Movement.lock())
	{
		Move->AddMove(-GetAxis(EAxis::X));
	}
}

void CEditorPlayer::MoveRight()
{
	if (auto Move = Movement.lock())
	{
		Move->AddMove(GetAxis(EAxis::X));
	}
}

void CEditorPlayer::LeftClick()
{
	if (auto TileMapObj = TileMap.lock())
	{
		if (auto TileMap = TileMapObj->GetTileMap().lock())
		{

		}
	}
}

void CEditorPlayer::Save()
{
}

void CEditorPlayer::Load()
{
}

void CEditorPlayer::ChangeMode()
{
	EditorMode = EditorMode == EEditorMode::TileType ? EEditorMode::TileFrame : EEditorMode::TileType;

	if (auto Widget = EditorWidget.lock())
	{
		Widget->SetEditorMode(EditorMode);
	}
}

void CEditorPlayer::ChangeTileType()
{
	int	Type = static_cast<int>(TileType) + 1;
	if (Type >= static_cast<int>(ETileType::End))
	{
		Type = 0;
	}

	TileType = static_cast<ETileType>(Type);

	if (auto Widget = EditorWidget.lock())
	{
		Widget->SetTileType(TileType);
	}
}

void CEditorPlayer::ChangeFrame()
{
	++EditorFrame;

	if (auto TileMapObj = TileMap.lock())
	{
		if (auto TileMap = TileMapObj->GetTileMap().lock())
		{
			if (EditorFrame >= TileMap->GetTileFrameCount())
			{
				EditorFrame = 0;
			}
		}
	}

	if (auto Widget = EditorWidget.lock())
	{
		Widget->SetTileFrame(EditorFrame);
	}
}

std::weak_ptr<CTile> CEditorPlayer::GetTile()
{
	if (auto World = this->World.lock())
	{
		if (auto Input = World->GetInput().lock())
		{
			if (auto TileMapObj = TileMap.lock())
			{
				if (auto TileMap = TileMapObj->GetTileMap().lock())
				{
					auto MousePos = Input->GetMouseWorldPos();
					return TileMap->GetTile(MousePos);
				}
			}
		}
	}
}
