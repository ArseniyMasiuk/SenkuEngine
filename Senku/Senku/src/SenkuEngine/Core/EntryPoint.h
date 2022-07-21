#pragma once

#include "Application.h"

extern 	Senku::Application* Senku::CreateApplication();


int main(int argc, char** argv)
{
	Senku::Log::Init();

	// todo: creating app like in hazel engine

	Senku::Application* app = Senku::CreateApplication();

	// run app
	app->Run();
	delete app;
	//app.reset();


	//system("pause");
	return 0;

}
