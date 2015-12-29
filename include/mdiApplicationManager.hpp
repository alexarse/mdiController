//
//  mdiEditorManager.hpp
//  eosMidiController
//
//  Created by Alexandre Arsenault on 2015-12-29.
//  Copyright © 2015 ax. All rights reserved.
//

#ifndef mdiApplicationManager_hpp
#define mdiApplicationManager_hpp

#include <axLib/axLib.h>
#include "mdiMainWindow.hpp"

namespace mdi {
class ApplicationManager {
public:
	static ApplicationManager* Create();

	static inline ApplicationManager* GetInstance()
	{
		return _instance.get();
	}
	
	static ax::Event::Object* GetMainEvtObj();
	
	int MainLoop();
	
private:
	static std::unique_ptr<ApplicationManager> _instance;
	std::shared_ptr<MainWindow> _mainWindow;

	ApplicationManager();

	void SetupApplication();
};
}

#endif /* mdiApplicationManager_hpp */
