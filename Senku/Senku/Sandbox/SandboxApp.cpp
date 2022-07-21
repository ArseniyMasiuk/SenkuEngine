#include "SenkuEngine.h"
#include "SenkuEngine\Core\EntryPoint.h"


Senku::Application* Senku::CreateApplication()
{
	Senku::Application* app = new Senku::Application(Senku::WindowProps("Sandbox Application", 800, 600));

	return app;

}