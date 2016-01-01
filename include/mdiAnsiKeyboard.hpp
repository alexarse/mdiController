#ifndef __MDI_ANSI_KEYBOARD_HPP__
#define __MDI_ANSI_KEYBOARD_HPP__

#include <axLib/axLib.h>

namespace mdi {
class AnsiKeyboard : public ax::Window::Backbone {
public:
	AnsiKeyboard(const ax::Rect& rect);
	
private:
	void OnPaint(ax::GC gc);
};
}

#endif // __MDI_ANSI_KEYBOARD_HPP__
