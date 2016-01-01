#ifndef __MDI_CTRL_KNOB_H__
#define __MDI_CTRL_KNOB_H__

#include <axLib/axKnob.h>

#include "mdiCtrl.hpp"

namespace mdi {
class Knob : public Ctrl {
public:
	Knob(const ax::Point& pos, const ax::StringPairVector& attributes);
	
	virtual std::shared_ptr<ax::Window::Backbone> GetBackbone();
	
private:
	std::shared_ptr<ax::Knob> _knob;

};
}

#endif // __MDI_CTRL_KNOB_H__
