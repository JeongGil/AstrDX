#include "CInput.h"

#include <ranges>

#include "../CDevice.h"
#include "../CEngine.h"

void CInput::AddBindKey(const std::string& Key, unsigned char KeyCode)
{
	if (BindKeys.contains(Key))
	{
		return;
	}

	auto NewKey = std::make_unique<FBindKey>();

	auto State = FindKeyState(KeyCode);
	if (!State)
	{
		auto [It, Result] = KeyStates.try_emplace(KeyCode, new FKeyState);
		if (Result)
		{
			State = It->second;
			State->Key = ConvertKey(KeyCode);
		}
	}

	NewKey->Key = State;

	BindKeys.emplace(Key, std::move(NewKey));
}

void CInput::SetKeyCtrl(const std::string& Key, bool Ctrl)
{
	auto BK = FindBindKey(Key);
	if (!BK)
	{
		return;
	}

	BK->Ctrl = Ctrl;
}

void CInput::SetKeyAlt(const std::string& Key, bool Alt)
{
	auto BK = FindBindKey(Key);
	if (!BK)
	{
		return;
	}

	BK->Alt = Alt;
}

void CInput::SetKeyShift(const std::string& Key, bool Shift)
{
	auto BK = FindBindKey(Key);
	if (!BK)
	{
		return;
	}

	BK->Shift = Shift;
}

FKeyState* CInput::FindKeyState(unsigned char KeyCode)
{
	auto It = KeyStates.find(KeyCode);
	return It == KeyStates.end() ? nullptr : It->second;
}

FBindKey* CInput::FindBindKey(const std::string& Key)
{
	auto It = BindKeys.find(Key);
	return It == BindKeys.end() ? nullptr : It->second.get();
}

unsigned char CInput::ConvertKey(unsigned char KeyCode)
{
	if (InputType == EInputSystemType::DInput)
	{
		switch (KeyCode)
		{
			case VK_LBUTTON:
				return DIK_MOUSELBUTTON;
			case VK_RBUTTON:
				return DIK_MOUSERBUTTON;
			case VK_MBUTTON:
				return DIK_MOUSEWHEELBUTTON;
			case VK_BACK:
				return DIK_BACK;
			case VK_TAB:
				return DIK_TAB;
			case VK_RETURN:
				return DIK_RETURN;
			case VK_LCONTROL:
				return DIK_LCONTROL;
			case VK_RCONTROL:
				return DIK_RCONTROL;
			case VK_LMENU:
				return DIK_LALT;
			case VK_RMENU:
				return DIK_RALT;
			case VK_LSHIFT:
				return DIK_LSHIFT;
			case VK_RSHIFT:
				return DIK_RSHIFT;
			case VK_PAUSE:
				return DIK_PAUSE;
			case VK_CAPITAL:
				return DIK_CAPSLOCK;
			case VK_ESCAPE:
				return DIK_ESCAPE;
			case VK_SPACE:
				return DIK_SPACE;
			case VK_NEXT:
				return DIK_PGDN;
			case VK_PRIOR:
				return DIK_PGUP;
			case VK_END:
				return DIK_END;
			case VK_HOME:
				return DIK_HOME;
			case VK_LEFT:
				return DIK_LEFT;
			case VK_UP:
				return DIK_UP;
			case VK_RIGHT:
				return DIK_RIGHT;
			case VK_DOWN:
				return DIK_DOWN;
			case VK_PRINT:
				return DIK_SYSRQ;
			case VK_INSERT:
				return DIK_INSERT;
			case VK_DELETE:
				return DIK_DELETE;
			case VK_HELP:
				return 0;
			case '0':
				return DIK_0;
			case '1':
				return DIK_1;
			case '2':
				return DIK_2;
			case '3':
				return DIK_3;
			case '4':
				return DIK_4;
			case '5':
				return DIK_5;
			case '6':
				return DIK_6;
			case '7':
				return DIK_7;
			case '8':
				return DIK_8;
			case '9':
				return DIK_9;
			case 'A':
				return DIK_A;
			case 'B':
				return DIK_B;
			case 'C':
				return DIK_C;
			case 'D':
				return DIK_D;
			case 'E':
				return DIK_E;
			case 'F':
				return DIK_F;
			case 'G':
				return DIK_G;
			case 'H':
				return DIK_H;
			case 'I':
				return DIK_I;
			case 'J':
				return DIK_J;
			case 'K':
				return DIK_K;
			case 'L':
				return DIK_L;
			case 'M':
				return DIK_M;
			case 'N':
				return DIK_N;
			case 'O':
				return DIK_O;
			case 'P':
				return DIK_P;
			case 'Q':
				return DIK_Q;
			case 'R':
				return DIK_R;
			case 'S':
				return DIK_S;
			case 'T':
				return DIK_T;
			case 'U':
				return DIK_U;
			case 'V':
				return DIK_V;
			case 'W':
				return DIK_W;
			case 'X':
				return DIK_X;
			case 'Y':
				return DIK_Y;
			case 'Z':
				return DIK_Z;
			case VK_OEM_3:
				return DIK_GRAVE;
			case VK_OEM_MINUS:
				return DIK_MINUS;
			case VK_OEM_NEC_EQUAL:
				return DIK_EQUALS;
			case VK_OEM_4:
				return DIK_LBRACKET;
			case VK_OEM_6:
				return DIK_RBRACKET;
			case VK_OEM_5:
				return DIK_BACKSLASH;
			case VK_OEM_1:
				return DIK_SEMICOLON;
			case VK_OEM_7:
				return DIK_APOSTROPHE;
			case VK_OEM_COMMA:
				return DIK_COMMA;
			case VK_OEM_PERIOD:
				return DIK_PERIOD;
			case VK_OEM_2:
				return DIK_SLASH;
			case VK_NUMPAD0:
				return DIK_NUMPAD0;
			case VK_NUMPAD1:
				return DIK_NUMPAD1;
			case VK_NUMPAD2:
				return DIK_NUMPAD2;
			case VK_NUMPAD3:
				return DIK_NUMPAD3;
			case VK_NUMPAD4:
				return DIK_NUMPAD4;
			case VK_NUMPAD5:
				return DIK_NUMPAD5;
			case VK_NUMPAD6:
				return DIK_NUMPAD6;
			case VK_NUMPAD7:
				return DIK_NUMPAD7;
			case VK_NUMPAD8:
				return DIK_NUMPAD8;
			case VK_NUMPAD9:
				return DIK_NUMPAD9;
			case VK_MULTIPLY:
				return DIK_MULTIPLY;
			case VK_ADD:
				return DIK_ADD;
			case VK_SEPARATOR:
				return DIK_NUMPADCOMMA;
			case VK_SUBTRACT:
				return DIK_SUBTRACT;
			case VK_DECIMAL:
				return DIK_DECIMAL;
			case VK_DIVIDE:
				return DIK_DIVIDE;
				//case VK_RETURN:		
				//return DIK_NUMPADENTER;
			case VK_F1:
				return DIK_F1;
			case VK_F2:
				return DIK_F2;
			case VK_F3:
				return DIK_F3;
			case VK_F4:
				return DIK_F4;
			case VK_F5:
				return DIK_F5;
			case VK_F6:
				return DIK_F6;
			case VK_F7:
				return DIK_F7;
			case VK_F8:
				return DIK_F8;
			case VK_F9:
				return DIK_F9;
			case VK_F10:
				return DIK_F10;
			case VK_F11:
				return DIK_F11;
			case VK_F12:
				return DIK_F12;
			case VK_F13:
				return DIK_F13;
			case VK_F14:
				return DIK_F14;
			case VK_F15:
				return DIK_F15;
			case VK_F16:
			case VK_F17:
			case VK_F18:
			case VK_F19:
			case VK_F20:
			case VK_F21:
			case VK_F22:
			case VK_F23:
			case VK_F24:
				return 0;
			case VK_NUMLOCK:
				return DIK_NUMLOCK;
			case VK_SCROLL:
				return DIK_SCROLL;
			case VK_LWIN:
				return DIK_LWIN;
			case VK_RWIN:
				return DIK_RWIN;
			case VK_APPS:
				return DIK_APPS;
			case VK_OEM_102:
				return DIK_OEM_102;
			default:
				break;
		}

		return 0xFF;
	}

	return KeyCode;
}

void CInput::UpdateInput(const float DeltaTime)
{
	switch (InputType)
	{
		case EInputSystemType::DInput:
			UpdateDInput(DeltaTime);
			break;
		case EInputSystemType::Window:
			UpdateWindowInput(DeltaTime);
			break;
	}
}

void CInput::UpdateDInput(const float DeltaTime)
{
	// Ctrl
	if (KeyState[DIK_LCONTROL] & 0x80)
	{
		CtrlState[EInputType::Release] = false;

		if (!CtrlState[EInputType::Press]
			&& !CtrlState[EInputType::Hold])
		{
			CtrlState[EInputType::Press] = true;
			CtrlState[EInputType::Hold] = true;
		}
		else
		{
			CtrlState[EInputType::Press] = false;
		}
	}
	else if (CtrlState[EInputType::Hold])
	{
		CtrlState[EInputType::Press] = false;
		CtrlState[EInputType::Hold] = false;
		CtrlState[EInputType::Release] = true;
	}
	else if (CtrlState[EInputType::Release])
	{
		CtrlState[EInputType::Release] = false;
	}
	// Alt
	if (KeyState[DIK_LALT] & 0x80)
	{
		AltState[EInputType::Release] = false;

		if (!AltState[EInputType::Press]
			&& !AltState[EInputType::Hold])
		{
			AltState[EInputType::Press] = true;
			AltState[EInputType::Hold] = true;
		}
		else
		{
			AltState[EInputType::Press] = false;
		}
	}
	else if (AltState[EInputType::Hold])
	{
		AltState[EInputType::Press] = false;
		AltState[EInputType::Hold] = false;
		AltState[EInputType::Release] = true;
	}
	else if (AltState[EInputType::Release])
	{
		AltState[EInputType::Release] = false;
	}
	// Shift
	if (KeyState[DIK_LSHIFT] & 0x80)
	{
		ShiftState[EInputType::Release] = false;

		if (!ShiftState[EInputType::Press]
			&& !ShiftState[EInputType::Hold])
		{
			ShiftState[EInputType::Press] = true;
			ShiftState[EInputType::Hold] = true;
		}
		else
		{
			ShiftState[EInputType::Press] = false;
		}
	}
	else if (ShiftState[EInputType::Hold])
	{
		ShiftState[EInputType::Press] = false;
		ShiftState[EInputType::Hold] = false;
		ShiftState[EInputType::Release] = true;
	}
	else if (ShiftState[EInputType::Release])
	{
		ShiftState[EInputType::Release] = false;
	}

	for (int i = 0; i < EMouseType::End; ++i)
	{
		auto& Button = MouseButton[i];
		if (MouseState.rgbButtons[i] & 0x80)
		{
			Button[EInputType::Release] = false;

			if (!Button[EInputType::Press]
				&& !Button[EInputType::Hold])
			{
				Button[EInputType::Press] = true;
				Button[EInputType::Hold] = true;
			}
			else
			{
				Button[EInputType::Press] = false;
			}
		}
		else if (Button[EInputType::Hold])
		{
			Button[EInputType::Press] = false;
			Button[EInputType::Hold] = false;
			Button[EInputType::Release] = true;
		}
		else if (Button[EInputType::Release])
		{
			Button[EInputType::Release] = false;
		}
	}

	for (auto& State : KeyStates | std::views::values)
	{
		switch (State->Key)
		{
			case DIK_MOUSELBUTTON:
				State->Press = MouseButton[EMouseType::LButton][EInputType::Press];
				State->Hold = MouseButton[EMouseType::LButton][EInputType::Hold];
				State->Release = MouseButton[EMouseType::LButton][EInputType::Release];
				break;
			case DIK_MOUSERBUTTON:
				State->Press = MouseButton[EMouseType::RButton][EInputType::Press];
				State->Hold = MouseButton[EMouseType::RButton][EInputType::Hold];
				State->Release = MouseButton[EMouseType::RButton][EInputType::Release];
				break;
			case DIK_MOUSEWHEELBUTTON:
				State->Press = MouseButton[EMouseType::MButton][EInputType::Press];
				State->Hold = MouseButton[EMouseType::MButton][EInputType::Hold];
				State->Release = MouseButton[EMouseType::MButton][EInputType::Release];
				break;
			default:
				if (KeyState[State->Key] & 0x80)
				{
					State->Release = false;

					if (!State->Press && !State->Hold)
					{
						State->Press = true;
						State->Hold = true;
					}
					else
					{
						State->Press = false;
					}
				}
				else if (State->Hold)
				{
					State->Press = false;
					State->Hold = false;
					State->Release = true;
				}
				else if (State->Release)
				{
					State->Release = false;
				}

				break;
		}
	}
}

void CInput::UpdateWindowInput(const float DeltaTime)
{
	// Ctrl
	if (GetAsyncKeyState(VK_LCONTROL) & 0x8000)
	{
		CtrlState[EInputType::Release] = false;

		if (!CtrlState[EInputType::Press] && !CtrlState[EInputType::Hold])
		{
			CtrlState[EInputType::Press] = true;
			CtrlState[EInputType::Hold] = true;
		}
		else
		{
			CtrlState[EInputType::Press] = false;
		}
	}
	else if (CtrlState[EInputType::Hold])
	{
		CtrlState[EInputType::Press] = false;
		CtrlState[EInputType::Hold] = false;
		CtrlState[EInputType::Release] = true;
	}
	else if (CtrlState[EInputType::Release])
	{
		CtrlState[EInputType::Release] = false;
	}

	// Alt
	if (GetAsyncKeyState(VK_LMENU) & 0x8000)
	{
		AltState[EInputType::Release] = false;

		if (!AltState[EInputType::Press] && !AltState[EInputType::Hold])
		{
			AltState[EInputType::Press] = true;
			AltState[EInputType::Hold] = true;
		}

		else
		{
			AltState[EInputType::Press] = false;
		}
	}
	else if (AltState[EInputType::Hold])
	{
		AltState[EInputType::Press] = false;
		AltState[EInputType::Hold] = false;
		AltState[EInputType::Release] = true;
	}
	else if (AltState[EInputType::Release])
	{
		AltState[EInputType::Release] = false;
	}

	// Shift
	if (GetAsyncKeyState(VK_LSHIFT) & 0x8000)
	{
		ShiftState[EInputType::Release] = false;

		if (!ShiftState[EInputType::Press] && !ShiftState[EInputType::Hold])
		{
			ShiftState[EInputType::Press] = true;
			ShiftState[EInputType::Hold] = true;
		}
		else
		{
			ShiftState[EInputType::Press] = false;
		}
	}
	else if (ShiftState[EInputType::Hold])
	{
		ShiftState[EInputType::Press] = false;
		ShiftState[EInputType::Hold] = false;
		ShiftState[EInputType::Release] = true;
	}
	else if (ShiftState[EInputType::Release])
	{
		ShiftState[EInputType::Release] = false;
	}

	for (int i = 0; i < EMouseType::End; ++i)
	{
		int MouseKey[EMouseType::End] =
		{
			VK_LBUTTON,
			VK_RBUTTON,
			VK_MBUTTON
		};

		auto& Button = MouseButton[i];
		if (GetAsyncKeyState(MouseKey[i]) & 0x8000)
		{
			Button[EInputType::Release] = false;

			if (!Button[EInputType::Press] && !Button[EInputType::Hold])
			{
				Button[EInputType::Press] = true;
				Button[EInputType::Hold] = true;
			}
			else
			{
				Button[EInputType::Press] = false;
			}
		}
		else if (Button[EInputType::Hold])
		{
			Button[EInputType::Press] = false;
			Button[EInputType::Hold] = false;
			Button[EInputType::Release] = true;
		}
		else if (Button[EInputType::Release])
		{
			Button[EInputType::Release] = false;
		}
	}

	for (auto& State : KeyStates | std::views::values)
	{
		if (GetAsyncKeyState(State->Key) & 0x8000)
		{
			State->Release = false;

			if (!State->Press && !State->Hold)
			{
				State->Press = true;
				State->Hold = true;
			}
			else
			{
				State->Press = false;
			}
		}
		else if (State->Hold)
		{
			State->Press = false;
			State->Hold = false;
			State->Release = true;
		}
		else if (State->Release)
		{
			State->Release = false;
		}
	}
}

void CInput::UpdateBindKey(const float DeltaTime)
{
	for (auto& BindKey : BindKeys | std::views::values)
	{
		if (BindKey->Key->Press
			&& BindKey->Ctrl == CtrlState[EInputType::Hold]
			&& BindKey->Shift == ShiftState[EInputType::Hold]
			&& BindKey->Alt == AltState[EInputType::Hold])
		{
			if (BindKey->Func[EInputType::Press])
			{
				BindKey->Func[EInputType::Press]();
			}
		}

		if (BindKey->Key->Hold
			&& BindKey->Ctrl == CtrlState[EInputType::Hold]
			&& BindKey->Shift == ShiftState[EInputType::Hold]
			&& BindKey->Alt == AltState[EInputType::Hold])
		{
			BindKey->KeyHold = true;

			if (BindKey->Func[EInputType::Hold])
			{
				BindKey->Func[EInputType::Hold]();
			}
		}

		bool Verification = false;

		if (BindKey->Ctrl)
		{
			if (CtrlState[EInputType::Release])
			{
				Verification = true;
			}
		}
		else
		{
			if (CtrlState[EInputType::Hold])
			{
				Verification = true;
			}
		}

		if (BindKey->Alt)
		{
			if (AltState[EInputType::Release])
			{
				Verification = true;
			}
		}
		else
		{
			if (AltState[EInputType::Hold])
			{
				Verification = true;
			}
		}

		if (BindKey->Shift)
		{
			if (ShiftState[EInputType::Release])
			{
				Verification = true;
			}
		}
		else
		{
			if (ShiftState[EInputType::Hold])
			{
				Verification = true;
			}
		}

		if ((BindKey->Key->Release || Verification)
			&& BindKey->KeyHold)
		{
			BindKey->KeyHold = false;

			if (BindKey->Func[EInputType::Release])
			{
				BindKey->Func[EInputType::Release]();
			}
		}
	}
}

CInput::~CInput()
{
	for (auto& State : KeyStates | std::views::values)
	{
		SAFE_DELETE(State)
	}
}

bool CInput::Init()
{
	hInst = CEngine::GetInst()->GetWindowInstance();
	hWnd = CEngine::GetInst()->GetWindowHandle();

	if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&Input, nullptr)))
	{
		InputType = EInputSystemType::Window;
	}
	else
	{
		if (!InitDevice())
		{
			return false;
		}
	}

	return true;
}

void CInput::Update(const float DeltaTime)
{
	if (InputType == EInputSystemType::DInput)
	{
		UpdateKeyboard();
		UpdateMouse();
	}

	UpdateInput(DeltaTime);

	UpdateBindKey(DeltaTime);
}

bool CInput::InitDevice()
{
	// Keyboard.
	if (FAILED(Input->CreateDevice(GUID_SysKeyboard, &Keyboard, nullptr)))
	{
		return false;
	}

	Keyboard->SetDataFormat(&c_dfDIKeyboard);

	if (CDevice::GetInst()->GetWindowMode())
	{
		Keyboard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	}
	else
	{
		Keyboard->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	}

	if (FAILED(Keyboard->Acquire()))
	{
		return false;
	}


	// Mouse.
	if (FAILED(Input->CreateDevice(GUID_SysMouse, &Mouse, nullptr)))
	{
		return false;
	}

	Mouse->SetDataFormat(&c_dfDIMouse);

	if (CDevice::GetInst()->GetWindowMode())
	{
		Mouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	}
	else
	{
		Mouse->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	}

	if (FAILED(Mouse->Acquire()))
	{
		return false;
	}

	return true;
}

void CInput::UpdateKeyboard()
{
	if (!Keyboard)
	{
		return;
	}

	HRESULT Result = Keyboard->GetDeviceState(256, (LPVOID)&KeyState);

	if (FAILED(Result))
	{
		if (Result == DIERR_INPUTLOST
			|| Result == DIERR_NOTACQUIRED)
		{
			if (SUCCEEDED(Keyboard->Acquire()))
			{
				Keyboard->GetDeviceState(256, (LPVOID)&KeyState);
			}
		}
	}
}

void CInput::UpdateMouse()
{
	if (!Mouse)
	{
		return;
	}

	HRESULT Result = Mouse->GetDeviceState(sizeof(MouseState), (LPVOID)&MouseState);

	if (FAILED(Result))
	{
		if (Result == DIERR_INPUTLOST
			|| Result == DIERR_NOTACQUIRED)
		{
			if (SUCCEEDED(Mouse->Acquire()))
			{
				Mouse->GetDeviceState(sizeof(MouseState), (LPVOID)&MouseState);
			}
		}
	}
}
