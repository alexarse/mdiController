#include "mdiApplicationManager.hpp"

namespace mdi {

std::unique_ptr<ApplicationManager> ApplicationManager::_instance = nullptr;

ApplicationManager* ApplicationManager::Create()
{
	if (_instance == nullptr) {
		_instance = std::unique_ptr<ApplicationManager>(new ApplicationManager());
	}

	return _instance.get();
}

ax::Event::Object* ApplicationManager::GetMainEvtObj()
{
	auto& app(ApplicationManager::_instance);
	return std::static_pointer_cast<ax::Event::Object>(app->_mainWindow->GetWindow()).get();
}

ApplicationManager::ApplicationManager()
{
	SetupApplication();
}

void ApplicationManager::SetupApplication()
{
	ax::App& app(ax::App::GetInstance());

	app.AddMainEntry([&]() {
#ifdef __linux__
		const ax::Size size(app.GetScreenSize());
#else
		const ax::Size size(1024, 600);
		app.SetFrameSize(size);		
#endif
		_mainWindow = ax::shared<MainWindow>(ax::Rect(0, 0, size));
		app.AddTopLevel(_mainWindow);
	});
#ifndef __linux__
	app.AddAfterGUILoadFunction([&app]() { app.SetFrameSize(ax::Size(1024, 600)); });
#endif
}

int ApplicationManager::MainLoop()
{
	ax::App::GetInstance().MainLoop();
	return 0;
}
}
