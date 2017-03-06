#include "Light.h"
#include "Window.h"
/*
Take control of the light point
*/

void Light::UpdateLightPos(float msec) {
	//msec *= 5.0f;
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_I))
		position.z -= 500.0f;
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_K))
		position.z += 500.0f;
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_J))
		position.x -= 500.0f;
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_L))
		position.x += 500.0f;
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_O))
		position.y += 500.0f;
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_P))
		position.y -= 500.0f;
}