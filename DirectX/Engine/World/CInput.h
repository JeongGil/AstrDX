#pragma once

#define DIRECTINPUT_VERSION 0x0800

#define DIK_MOUSELBUTTON 0xFC
#define DIK_MOUSERBUTTON 0xFD
#define DIK_MOUSEWHEELBUTTON 0xFE

#include "../EngineInfo.h"
#include <dinput.h>

#pragma comment(lib, "dinput8.lib")

enum class EInputSystemType
{
	DInput,
	Window,
};

namespace EInputType
{
	enum Type
	{
		Press,
		Hold,
		Release,
		End
	};
}

namespace EMouseType
{
	enum Type
	{
		LButton,
		RButton,
		MButton,
		End
	};
}

struct FKeyState
{
	unsigned char Key = 0;
	bool Press = false;
	bool Hold = false;
	bool Release = false;
};

struct FBindKey
{
	FKeyState* Key = nullptr;
	bool Ctrl = false;
	bool Alt = false;
	bool Shift = false;
	std::function<void()> Func[EInputType::End];
	bool KeyHold = false;
};

class CWorld;

class CInput
{
	friend CWorld;

public:
	void AddBindKey(const std::string& Key, unsigned char KeyCode);
	void SetKeyCtrl(const std::string& Key, bool Ctrl);
	void SetKeyAlt(const std::string& Key, bool Alt);
	void SetKeyShift(const std::string& Key, bool Shift);

	template <typename T>
	void SetBindFunction(const std::string& Key, EInputType::Type Type, T* Object, void (T::* Func)())
	{
		auto BindKey = FindBindKey(Key);
		if (!BindKey)
		{
			return;
		}

		BindKey->Func[Type] = std::bind(Func, Object);
	}

	bool Init();
	void Update(const float DeltaTime);

private:
	FKeyState* FindKeyState(unsigned char KeyCode);
	FBindKey* FindBindKey(const std::string& Key);
	unsigned char ConvertKey(unsigned char KeyCode);

	bool InitDevice();
	void UpdateKeyboard();
	void UpdateMouse();
	void UpdateMousePos(const float DeltaTime);
	void UpdateInput(const float DeltaTime);
	void UpdateDInput(const float DeltaTime);
	void UpdateWindowInput(const float DeltaTime);
	void UpdateBindKey(const float DeltaTime);

private:
	std::weak_ptr<CWorld> World;

	EInputSystemType InputType = EInputSystemType::DInput;

	IDirectInput8* Input = nullptr;
	IDirectInputDevice8* Keyboard = nullptr;
	IDirectInputDevice8* Mouse = nullptr;

	unsigned char KeyState[256] = {};
	DIMOUSESTATE MouseState = {};

	std::unordered_map<unsigned char, FKeyState*> KeyStates;
	std::unordered_map<std::string, std::unique_ptr<FBindKey>> BindKeys;

	bool CtrlState[EInputType::End] = {};
	bool AltState[EInputType::End] = {};
	bool ShiftState[EInputType::End] = {};

	bool MouseButton[EInputType::End][EInputType::End] = {};

	FVector2 MousePos;
	FVector2 MouseWorldPos;
	FVector2 MouseMove;
	bool bMouseCheckStart = false;

	HINSTANCE hInst;
	HWND hWnd;

public:
	[[nodiscard]] FVector2 GetMousePos() const
	{
		return MousePos;
	}

	[[nodiscard]] FVector2 GetMouseWorldPos() const
	{
		return MouseWorldPos;
	}

	[[nodiscard]] FVector2 GetMouseMove() const
	{
		return MouseMove;
	}

	[[nodiscard]] bool GetMouseState(EMouseType::Type MouseType, EInputType::Type InputType) const
	{
		return MouseButton[MouseType][InputType];
	}

	[[nodiscard]] bool GetCtrl(EInputType::Type Type) const
	{
		return CtrlState[Type];
	}

	[[nodiscard]] bool GetAlt(EInputType::Type Type) const
	{
		return AltState[Type];
	}

	[[nodiscard]] bool GetShift(EInputType::Type Type) const
	{
		return ShiftState[Type];
	}

private:
	CInput() = default;

public:
	~CInput();
};

