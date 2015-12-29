#ifndef mdiStatusBar_hpp
#define mdiStatusBar_hpp

#include <axLib/axLib.h>

namespace mdi {
class StatusBar : public ax::Window::Backbone {
public:
	StatusBar(const ax::Rect& rect);
	
private:
	void OnPaint(ax::GC gc);
};
}

#endif /* mdiStatusBar_hpp */
