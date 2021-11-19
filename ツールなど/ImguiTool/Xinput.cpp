#include "Xinput.h"


void UpdateXpad(void)
{
	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	// Simply get the state of the controller from XInput.
	DWORD dwResult = XInputGetState(0, &state);

	if (dwResult == ERROR_SUCCESS)
	{
		// Controller is connected
	}
	else
	{
		// Controller is not connected
	}

	// Zero value if thumbsticks are within the dead zone

	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_A)
	{
		// A ƒ{ƒ^ƒ“‚ª‰Ÿ‚³‚ê‚½

	}
}


