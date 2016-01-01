#ifndef __MDI_CTRL_BUTTON_H__
#define __MDI_CTRL_BUTTON_H__

#include <axLib/axButton.h>
#include "mdiCtrl.hpp"

namespace mdi {
class Button : public Ctrl {
public:
	Button(const ax::Point& pos, const ax::StringPairVector& attributes);
	
	virtual std::shared_ptr<ax::Window::Backbone> GetBackbone();
	
private:
	std::shared_ptr<ax::Window::Backbone> _button;

};
}

#endif // __MDI_CTRL_BUTTON_H__
