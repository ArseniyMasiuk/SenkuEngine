#include "PrecompiledHeader.h"
#include "SenkuEngine.h"
#include "SenkuEngine\Core\EntryPoint.h"

#include "SenkuEditorLayer.h"

namespace Senku
{
	class SenkuEditorApp : public Senku::Application
	{
	public:
		SenkuEditorApp(Senku::WindowProps& props)
			: Senku::Application(props)
		{
			PushLayer(new SenkuEditorLayer());
		}

		~SenkuEditorApp()
		{

		}
	};


	Senku::Application* Senku::CreateApplication()
	{
		Senku::WindowProps props("Senku Engine Editor", 1600, 800);
		Senku::Application* app = new SenkuEditorApp(props);

		return app;

	}
}