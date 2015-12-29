#ifndef mdiInspectorMenu_hpp
#define mdiInspectorMenu_hpp

#include <axLib/axLib.h>

namespace mdi {
	class InspectorMenu : public ax::Window::Backbone {
	public:
		InspectorMenu(const ax::Rect& rect);
		
		void OnPaint(ax::GC gc);
	};
}

#endif /* mdiInspectorMenu_hpp */
