#include "mdiCtrlKnob.hpp"

namespace mdi {
Knob::Knob(const ax::Point& pos, const ax::StringPairVector& attributes)
{
	int n_img = 0;
	std::string img_path;
	ax::Size size;

	for (auto& n : attributes) {
		ax::Print(n.first, n.second);
		if (n.first == "n_img") {
			n_img = std::stoi(n.second);
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

	ax::Knob::Info k_info;
	k_info.bgColorNormal = ax::Color(0.0, 0.0);
	k_info.bgColorHover = ax::Color(0.0, 0.0);
	k_info.bgColorClicked = ax::Color(0.0, 0.0);

	k_info.img_path = img_path;
	k_info.selected_img_path = img_path;
	k_info.knob_size = size;
	k_info.n_knob = n_img;

	_knob = ax::shared<ax::Knob>(ax::Rect(pos, k_info.knob_size), ax::Knob::Events(), k_info);
}

std::shared_ptr<ax::Window::Backbone> Knob::GetBackbone()
{
	return _knob;
}

}

extern "C"
{
	mdi::Ctrl* Create(const ax::Point& pos, const ax::StringPairVector& attributes)
	{
		return new mdi::Knob(pos, attributes);
	}
}