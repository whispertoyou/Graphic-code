#pragma comment(lib, "nclgl.lib")

#include "../../nclgl/window.h"
#include "Renderer.h"

int main() {
	Window w("My Coursework!", 1024, 768, false);
	if (!w.HasInitialised()) {
		return -1;
	}

	Renderer renderer(w);
	if (!renderer.HasInitialised()) {
		return -1;
	}

	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);

	//ini the position of the light
	/*Vector3 position ((RAW_HEIGHT * HEIGHTMAP_X / 2.0f), 5000.0f,
		(RAW_HEIGHT * HEIGHTMAP_Z / 2.0f));
*/
	while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_1)) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_2)) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		//renderer.SetLightPosition(position);
		renderer.UpdateScene(w.GetTimer()->GetTimedMS());
		renderer.RenderScene();

	}

	return 0;
}