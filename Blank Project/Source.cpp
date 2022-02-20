#include "../NCLGL/window.h"
#include "Renderer.h"
#include<string>
#include <sstream>
int main()	{
	Window w("Island", 1920, 1080, true);

	if(!w.HasInitialised()) {
		return -1;
	}
	
	Renderer renderer(w);
	if(!renderer.HasInitialised()) {
		return -1;
	}
	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);
	while(w.UpdateWindow()  && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)){
		renderer.UpdateScene(w.GetTimer()->GetTimeDeltaSeconds());
		renderer.RenderScene();
		renderer.SwapBuffers();
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_F5)) {
			Shader::ReloadAllShaders();
		}
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_T))
		{
			renderer.postProcessing = !renderer.postProcessing;
		}
		//std::string title;
		//std::ostringstream os;
		//Vector3 position = renderer.camera->GetPosition();
		//os << position.x << " " << position.y << " " << position.z;
		//title = os.str();
		//w.SetTitle(title);
	}
	return 0;
}