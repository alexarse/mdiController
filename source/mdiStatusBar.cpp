#include "mdiApplicationManager.hpp"
#include "mdiStatusBar.hpp"
#include <axLib/axToggle.h>

namespace mdi {
StatusBar::StatusBar(const ax::Rect& rect)
{
	// Create window.
	win = ax::Window::Create(rect);
	win->event.OnPaint = ax::WBind<ax::GC>(this, &StatusBar::OnPaint);

	// Transparent toggle with image.
	ax::Toggle::Info tog_info;
	tog_info.normal = ax::Color(0.0, 0.0);
	tog_info.hover = ax::Color(0.0, 0.0);
	tog_info.clicking = ax::Color(0.0, 0.0);
	tog_info.selected = ax::Color(0.0, 0.0);
	tog_info.font_color = ax::Color(0.0, 0.0);

	tog_info.selected = ax::Color(0.0, 0.0);
	tog_info.selected_hover = ax::Color(0.0, 0.0);
	tog_info.selected_clicking = ax::Color(0.0, 0.0);
	tog_info.selected = ax::Color(0.0, 0.0);
	tog_info.selected_font_color = ax::Color(0.0, 0.0);

	tog_info.contour = ax::Color(0.0, 0.0);
	tog_info.font_size = 12;
	tog_info.img = "resources/top_menu_toggle_left.png";
	tog_info.single_img = false;

	ax::Point pos(rect.size.x - 35 * 3 - 20, 2);
	const ax::Size tog_size(35, 35);
	
	ax::Toggle::Events tog_left_evts(ax::Event::Function([](ax::Event::Msg* msg) {
		ax::Toggle::Msg* t_msg(static_cast<ax::Toggle::Msg*>(msg));
		ApplicationManager::GetInstance()->GetMainEvtObj()->PushEvent(9000, new ax::Toggle::Msg(*t_msg));
	}));
	
	auto tog_left = ax::shared<ax::Toggle>(ax::Rect(pos, tog_size), tog_left_evts, tog_info);

	pos = tog_left->GetWindow()->dimension.GetRect().GetNextPosRight(5);
	tog_info.img = "resources/top_menu_toggle_bottom.png";

	ax::Toggle::Events tog_middle_evts(ax::Event::Function([](ax::Event::Msg* msg) {
		ax::Toggle::Msg* t_msg(static_cast<ax::Toggle::Msg*>(msg));
		ApplicationManager::GetInstance()->GetMainEvtObj()->PushEvent(9001, new ax::Toggle::Msg(*t_msg));
	}));
	auto tog_middle = ax::shared<ax::Toggle>(ax::Rect(pos, tog_size), tog_middle_evts, tog_info);

	pos = tog_middle->GetWindow()->dimension.GetRect().GetNextPosRight(5);
	tog_info.img = "resources/top_menu_toggle_right.png";

	ax::Toggle::Events tog_right_evts(ax::Event::Function([](ax::Event::Msg* msg) {
		ax::Toggle::Msg* t_msg(static_cast<ax::Toggle::Msg*>(msg));
		ApplicationManager::GetInstance()->GetMainEvtObj()->PushEvent(9002, new ax::Toggle::Msg(*t_msg));
	}));
	
	auto tog_right = ax::shared<ax::Toggle>(ax::Rect(pos, tog_size), tog_right_evts, tog_info);

	tog_left->SetSelected(true);
	tog_middle->SetSelected(true);
	tog_right->SetSelected(true);

	win->node.Add(tog_left);
	win->node.Add(tog_middle);
	win->node.Add(tog_right);

	pos = ax::Point(5, 2);

	tog_info.img = "resources/widget_menu.png";
	auto widget_menu
		= ax::shared<ax::Toggle>(ax::Rect(pos, tog_size), ax::Toggle::Events(), tog_info, "", "");

	win->node.Add(widget_menu);
	pos = widget_menu->GetWindow()->dimension.GetRect().GetNextPosRight(5);

	auto tree_menu = ax::shared<ax::Toggle>(ax::Rect(pos, tog_size), ax::Toggle::Events(), tog_info, "", "");

	win->node.Add(tree_menu);
}

void StatusBar::OnPaint(ax::GC gc)
{
	ax::Rect rect(win->dimension.GetDrawingRect());

	gc.SetColor(ax::Color(0.15));
	//		gc.DrawRectangle(rect);
	gc.DrawRectangleColorFade(rect, ax::Color(0.15), ax::Color(0.18));
	gc.DrawRectangleContour(rect);
}
}