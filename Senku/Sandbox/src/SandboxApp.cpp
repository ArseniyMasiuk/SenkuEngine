#include "PrecompiledHeader.h"
#include "SenkuEngine.h"
#include "SenkuEngine\Core\EntryPoint.h"

#include "SandboxLayer.h"

class SandboxApp : public Senku::Application
{
public:
	SandboxApp(Senku::WindowProps& props)
		: Senku::Application(props)
	{
		PushLayer(new SandBoxLayer());
	}

	~SandboxApp()
	{

	}
};


Senku::Application* Senku::CreateApplication()
{
	Senku::WindowProps props("Sandbox Application", 1600, 800);
	Senku::Application* app = new SandboxApp(props);

	return app;

}