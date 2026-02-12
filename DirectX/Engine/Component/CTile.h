#pragma once

#include "../EngineInfo.h"

class CTileMapComponent;

class CTile
{
	friend CTileMapComponent;

private:
	CTile();
	CTile(const CTile& other) = default;
	CTile(CTile&& other) noexcept = default;

public:
	~CTile();

private:
	ETileType Type = ETileType::Normal;
	FVector2 Pos;
	FVector2 Size;
	FVector2 Center;
	int TextureFrame{ -1 };
	FVector2 FrameStart;
	FVector2 FrameEnd;
	bool bRender{ false };
	bool bOutLineRender{ false };
	FColor OutlineColor{ FColor::White };

public:
	void Save(FILE* File);
	void Load(FILE* File);

	[[nodiscard]] bool GetOutLineRender() const
	{
		return bOutLineRender;
	}

	void SetOutLineRender(const bool bOutLineRender)
	{
		this->bOutLineRender = bOutLineRender;
	}

	[[nodiscard]] FColor GetOutlineColor() const
	{
		return OutlineColor;
	}

	void SetOutlineColor(const FColor& OutlineColor)
	{
		this->OutlineColor = OutlineColor;
	}

	void SetOutlineColor(float r, float g, float b, float a)
	{
		SetOutlineColor(FColor(r, g, b, a));
	}

	[[nodiscard]] ETileType GetType() const
	{
		return Type;
	}

	void SetType(ETileType type)
	{
		Type = type;
	}

	[[nodiscard]] FVector2 GetPos() const
	{
		return Pos;
	}

	void SetPos(const FVector2& pos)
	{
		Pos = pos;
	}

	void SetPos(float x, float y)
	{
		SetPos(FVector2(x, y));
	}

	[[nodiscard]] FVector2 GetSize() const
	{
		return Size;
	}

	void SetSize(const FVector2& size)
	{
		Size = size;
	}

	void SetSize(float x, float y)
	{
		SetSize(FVector2(x, y));
	}

	[[nodiscard]] FVector2 GetCenter() const
	{
		return Center;
	}

	void SetCenter(const FVector2& center)
	{
		Center = center;
	}

	void SetCenter(float x, float y)
	{
		SetCenter(FVector2(x, y));
	}

	[[nodiscard]] int GetTextureFrame() const
	{
		return TextureFrame;
	}

	void SetTextureFrame(int texture_frame)
	{
		TextureFrame = texture_frame;
	}

	[[nodiscard]] FVector2 GetFrameStart() const
	{
		return FrameStart;
	}

	void SetFrameStart(const FVector2& frame_start)
	{
		FrameStart = frame_start;
	}

	[[nodiscard]] FVector2 GetFrameEnd() const
	{
		return FrameEnd;
	}

	void SetFrameEnd(const FVector2& frame_end)
	{
		FrameEnd = frame_end;
	}

	void SetFrame(const FVector2& Start, const FVector2& End)
	{
		SetFrameStart(Start);
		SetFrameEnd(End);

		SetRender(true);
	}

	void SetFrame(float x, float y, float w, float h)
	{
		SetFrame(FVector2(x, y), FVector2(w, h));
	}

	[[nodiscard]] bool GetRender() const
	{
		return bRender;
	}

	void SetRender(bool b_render)
	{
		bRender = b_render;
	}
};

