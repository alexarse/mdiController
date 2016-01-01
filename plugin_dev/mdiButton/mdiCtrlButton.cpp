#include "mdiCtrlButton.hpp"

namespace mdi {
Button::Button(const ax::Point& pos, const ax::StringPairVector& attributes)
{
	std::string img_path;
	std::string label;
	ax::Size size;

	ax::Button::Info btn_info;
	btn_info.normal = ax::Color(0.0, 0.0);
	btn_info.hover = ax::Color(0.0, 0.0);
	btn_info.clicking = ax::Color(0.0, 0.0);
	btn_info.selected = ax::Color(0.0, 0.0);
	btn_info.contour = ax::Color(0.0, 0.0);

	for (auto& n : attributes) {
		ax::Print(n.first, n.second);

		if (n.first == "label") {
			label = n.second;
		}
		else if (n.first == "img_path") {
			img_path = n.second;
		}
		else if (n.first == "size") {
			std::istringstream ss(n.second);
			std::string n1, n2;
			ss >> n1 >> n2;
			size.x = std::stoi(n1);
			size.y = std::stoi(n2);
		}
	}

	_button = ax::Button::Create(ax::Rect(pos, size), ax::Button::Events(), label, img_path, btn_info);
}

std::shared_ptr<ax::Window::Backbone> Button::GetBackbone()
{
	return _button;
}
}

extern "C" {
mdi::Ctrl* Create(const ax::Point& pos, const ax::StringPairVector& attributes)
{
	return new mdi::Button(pos, attributes);
}
}