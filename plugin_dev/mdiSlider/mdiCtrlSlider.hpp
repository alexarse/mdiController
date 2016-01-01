#ifndef __MDI_CTRL_SLIDER_H__
#define __MDI_CTRL_SLIDER_H__

#include <axLib/axSlider.h>

#include "mdiCtrl.hpp"

namespace mdi {
class Slider : public Ctrl {
public:
	Slider(const ax::Point& pos, const ax::StringPairVector& attributes);
	
	virtual std::shared_ptr<ax::Window::Backbone> GetBackbone();
	
private:
	std::shared_ptr<ax::Slider> _slider;
};
}

#endif // __MDI_CTRL_SLIDER_H__
