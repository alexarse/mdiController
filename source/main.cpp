#include "mdiApplicationManager.hpp"

int main()
{
	mdi::ApplicationManager* app = mdi::ApplicationManager::Create();
	return app->MainLoop();
}