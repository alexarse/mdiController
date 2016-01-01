#include "Script.h"
#include "axLib/axButton.h"
#include "axLib/axKnob.h"
#include "axLib/axScrollBar.h"
#include "axLib/axToggle.h"
#include "axLib/axWindowManager.h"
#include "axLib/axWindowTree.h"
#include "mdiMainWindow.hpp"

#include "ctrlLineFollower.hpp"

#include "mdiCtrl.hpp"

namespace mdi {
MainWindow::MainWindow(const ax::Rect& rect)
{
	// Create window.
	win = ax::Window::Create(rect);
	win->event.OnPaint = ax::WBind<ax::GC>(this, &MainWindow::OnPaint);
	win->event.OnResize = ax::WBind<ax::Size>(this, &MainWindow::OnResize);

	const int top_menu_height = 40;

	// Create top menu.
	ax::Rect top_menu_rect(0, 0, rect.size.x, top_menu_height);
	win->node.Add(_statusBar = ax::shared<StatusBar>(top_menu_rect));

	// Create widget menu.
	ax::Rect widget_menu_rect(0, top_menu_height, 250, rect.size.y - top_menu_height - 18);
	win->node.Add(_widgetMenu = ax::shared<WidgetMenu>(widget_menu_rect));

	// Create info menu.
	ax::Rect info_rect(rect.size.x - 200, top_menu_height, 200, rect.size.y - top_menu_height - 18);
	win->node.Add(_inspectorMenu = ax::shared<InspectorMenu>(info_rect));

	// Create grid window.
	ax::Rect grid_rect(250, top_menu_height, rect.size.x - 250 - 200, rect.size.y - top_menu_height - 200 - 18);
	win->node.Add(_gridWindow = ax::shared<GridWindow>(grid_rect));

	// Create code editor.
	TextEditor::Info txt_info;
	txt_info.bg_color = ax::Color(0.9);
	txt_info.cursor_color = ax::Color(0.0);
	txt_info.line_number_bg_color = ax::Color(0.2);
	txt_info.line_number_color = ax::Color(0.5);
	txt_info.text_color = ax::Color(0.0);

	ax::Rect editor_rect(251, rect.size.y - 200 - 18, rect.size.x - 250 - 200, 200);
	win->node.Add(_codeEditor = ax::shared<TextEditor>(editor_rect, txt_info));

	win->AddConnection(8000, GetOnCreateDraggingWidget());
	win->AddConnection(8001, GetOnDraggingWidget());
	win->AddConnection(8002, GetOnReleaseObjWidget());
	
	ConnectStatusBarViewToggles();

	_codeEditor->GetWindow()->AddConnection(10020, ax::Event::Function([&](ax::Event::Msg* msg) {
		ax::Print("FUCK");
		std::vector<ax::Window::Ptr>& children
			= _gridWindow->GetWindow()->node.GetChildren();
			for (auto& n : children) {
				n->Update();
			}
		}));
}

void MainWindow::ConnectStatusBarViewToggles()
{
	// Widgets list toggle.
	win->AddConnection(9000, ax::Event::Function([&](ax::Event::Msg* msg) {
		ax::Toggle::Msg* t_msg(static_cast<ax::Toggle::Msg*>(msg));
		
		if(t_msg->GetSelected()) {
			_widgetMenu->GetWindow()->Show();
		} else {
			_widgetMenu->GetWindow()->Hide();
		}
		
		ResizeGridWindow();
	}));
	
	// Code editor toggle.
	win->AddConnection(9001, ax::Event::Function([&](ax::Event::Msg* msg) {
		ax::Toggle::Msg* t_msg(static_cast<ax::Toggle::Msg*>(msg));
		
		if(t_msg->GetSelected()) {
			_codeEditor->GetWindow()->Show();
		} else {
			_codeEditor->GetWindow()->Hide();
		}
		
		ResizeGridWindow();
	}));
	
	// Inspector menu toggle.
	win->AddConnection(9002, ax::Event::Function([&](ax::Event::Msg* msg) {
		ax::Toggle::Msg* t_msg(static_cast<ax::Toggle::Msg*>(msg));
		
		if(t_msg->GetSelected()) {
			_inspectorMenu->GetWindow()->Show();
		} else {
			_inspectorMenu->GetWindow()->Hide();
		}
		
		ResizeGridWindow();
	}));
}

void MainWindow::ResizeGridWindow()
{
	ax::Size size(win->dimension.GetRect().size);
	size.y -= (40 + 18);
	
	ax::Point position;

	if(_widgetMenu->GetWindow()->IsShown()) {
		position = ax::Point(_widgetMenu->GetWindow()->dimension.GetRect().GetNextPosRight(0));
		size.x -= _widgetMenu->GetWindow()->dimension.GetRect().size.x;
	} else {
		const ax::Point pos(_widgetMenu->GetWindow()->dimension.GetRect().GetNextPosRight(0));
		position = ax::Point(0, pos.y);
	}
	
	if(_inspectorMenu->GetWindow()->IsShown()) {
		size.x -= _inspectorMenu->GetWindow()->dimension.GetRect().size.x;
	}
	
	if(_codeEditor->GetWindow()->IsShown()) {
		size.y -= _codeEditor->GetWindow()->dimension.GetRect().size.y;
	}
	
	_gridWindow->GetWindow()->dimension.SetRect(ax::Rect(position, size));
}

void MainWindow::OnCreateDraggingWidget(const ax::Event::SimpleMsg<ObjMsg>& msg)
{
	const auto& msg_tuple(msg.GetMsg());
	ax::Point pos(std::get<0>(msg_tuple));
	std::string obj_type(std::get<1>(msg_tuple));
	ax::StringPairVector attributes(std::get<2>(msg_tuple));

	CtrlFactory* factory = CtrlFactory::GetInstance();
	Ctrl::Ptr ctrl(factory->Create(pos, obj_type, attributes));
	
	if(ctrl != nullptr) {
		ax::App::GetInstance().GetPopupManager()->GetWindowTree()->AddTopLevel(ctrl->GetBackbone()->GetWindow());
		ctrl->GetBackbone()->GetWindow()->backbone = ctrl->GetBackbone();
	}
	
	//	ax::App::GetInstance().GetPopupManager()->GetWindowTree()->AddTopLevel(ctrl->GetBackbone());

	//	ax::Point pos(msg.GetMsg().second);

	//	ax::StringPairVector attributes;
	//	attributes.push_back(ax::StringPair("size", "100 100"));
	//	attributes.push_back(ax::StringPair("img_path", "resources/JP8000.png"));
	//	attributes.push_back(ax::StringPair("n_img", "20"));
	//
	//	CtrlFactory factory;
	//	factory.Create(pos, obj_type, attributes);
	//
	//
	//	if (obj_type == "Window") {
	//		ax::Window::Ptr window = ax::Window::Create(ax::Rect(pos, ax::Size(200, 200)));
	//
	//		window->property.AddProperty("AcceptWidget");
	//
	//		window->event.OnPaint = ax::WFunc<ax::GC>([&, window](ax::GC gc) {
	//			ax::Rect rect(window->dimension.GetDrawingRect());
	//			gc.SetColor(0.4f, 0.4f, 0.4f);
	//			gc.DrawRectangle(rect);
	//
	////			Script script;
	////			script.AddWindowHandle(window);
	////			script.ProcessFile(_codeEditor->GetFilePath());
	//		});
	//
	//		window->event.OnMouseLeftDown = ax::WFunc<ax::Point>([window](const ax::Point& pos) {
	//			bool cmd_down = ax::App::GetInstance().GetWindowManager()->IsCmdDown();
	//
	//			if (cmd_down) {
	//				ax::Point c_delta(pos - window->dimension.GetAbsoluteRect().position);
	//				window->resource.Add("click_delta", c_delta);
	//
	//				if (c_delta.x > window->dimension.GetShownRect().size.x - 4) {
	//					ax::Print("Right resize");
	//					window->property.AddProperty("ResizeRight");
	//				}
	//				else if (c_delta.y > window->dimension.GetShownRect().size.y - 4) {
	//					ax::Print("Bottom resize");
	//					window->property.AddProperty("ResizeBottom");
	//				}
	//				else if (c_delta.x < 4) {
	//					ax::Print("Left resize");
	//					window->property.AddProperty("ResizeLeft");
	//				}
	//
	//				window->event.GrabMouse();
	//			}
	//		});
	//
	//		window->event.OnMouseLeftDragging = ax::WFunc<ax::Point>([window](const ax::Point& pos) {
	//			if (window->event.IsGrabbed()) {
	//				ax::Point c_delta = window->resource.GetResource("click_delta");
	//
	//				if (window->property.HasProperty("ResizeRight")) {
	//					int size_y = window->dimension.GetSize().y;
	//					int size_x = pos.x - window->dimension.GetAbsoluteRect().position.x;
	//					window->dimension.SetSize(ax::Size(size_x, size_y));
	//				}
	//				else if (window->property.HasProperty("ResizeBottom")) {
	//					int size_x = window->dimension.GetSize().x;
	//					int size_y = pos.y - window->dimension.GetAbsoluteRect().position.y;
	//					window->dimension.SetSize(ax::Size(size_x, size_y));
	//				}
	//				else if (window->property.HasProperty("ResizeLeft")) {
	//					ax::Rect abs_rect(window->dimension.GetAbsoluteRect());
	//					int size_x = abs_rect.position.x + abs_rect.size.x - pos.x;
	//					int size_y = abs_rect.size.y;
	//					int pos_y = window->dimension.GetRect().position.y;
	//					int pos_x = pos.x - window->node.GetParent()->dimension.GetAbsoluteRect().position.x;
	//					window->dimension.SetSize(ax::Size(size_x, size_y));
	//					window->dimension.SetPosition(ax::Point(pos_x, pos_y));
	//				}
	//				else {
	//					window->dimension.SetPosition(
	//						pos - window->node.GetParent()->dimension.GetAbsoluteRect().position - c_delta);
	//				}
	//			}
	//		});
	//
	//		window->event.OnMouseLeftUp = ax::WFunc<ax::Point>([window](const ax::Point& pos) {
	//			if (window->event.IsGrabbed()) {
	//				window->event.UnGrabMouse();
	//				ax::Print("Release.");
	//
	//				if (window->property.HasProperty("ResizeRight")) {
	//					window->property.RemoveProperty("ResizeRight");
	//				}
	//
	//				if (window->property.HasProperty("ResizeBottom")) {
	//					window->property.RemoveProperty("ResizeBottom");
	//				}
	//
	//				if (window->property.HasProperty("ResizeLeft")) {
	//					window->property.RemoveProperty("ResizeLeft");
	//				}
	//			}
	//		});
	//
	//		ax::App::GetInstance().GetPopupManager()->GetWindowTree()->AddTopLevel(window);
	//	}
	//
	//	if (obj_type == "Button") {
	//		std::shared_ptr<ax::Window::Backbone> btn
	//			= ax::Button::Create(ax::Rect(pos, ax::Size(60, 25)), ax::Button::Events(), "Button");
	//
	//		auto fct_ldown = btn->GetWindow()->event.OnMouseLeftDown.GetFunction();
	//
	//		if (fct_ldown) {
	//			btn->GetWindow()->event.OnMouseLeftDown
	//				= ax::WFunc<ax::Point>([btn, fct_ldown](const ax::Point& pos) {
	//					  ax::Print("Button left down.");
	//					  bool cmd_down = ax::App::GetInstance().GetWindowManager()->IsCmdDown();
	//
	//					  fct_ldown(pos);
	//				  });
	//		}
	//
	//		ax::App::GetInstance().GetPopupManager()->GetWindowTree()->AddTopLevel(btn->GetWindow());
	//		btn->GetWindow()->backbone = btn;
	//	}
	//
	//
	//
	//	if (obj_type == "ButtonBgImg1") {
	//
	//		ax::Button::Info btn_info;
	//		btn_info.normal = ax::Color(0.0, 0.0);
	//		btn_info.hover = ax::Color(0.0, 0.0);
	//		btn_info.clicking = ax::Color(0.0, 0.0);
	//		btn_info.selected = ax::Color(0.0, 0.0);
	//		btn_info.contour = ax::Color(0.0, 0.0);
	//
	//		std::shared_ptr<ax::Window::Backbone> btn = ax::Button::Create(ax::Rect(pos, ax::Size(60, 25)),
	//			ax::Button::Events(), "Button", "resources/axButtonImg.png", btn_info);
	//
	//		ax::App::GetInstance().GetPopupManager()->GetWindowTree()->AddTopLevel(btn->GetWindow());
	//		btn->GetWindow()->backbone = btn;
	//	}
	//
	//	if (obj_type == "RoundedButton01") {
	//
	//		ax::Button::Info btn_info;
	//		btn_info.normal = ax::Color(0.4);
	//		btn_info.hover = ax::Color(0.45);
	//		btn_info.clicking = ax::Color(0.35);
	//		btn_info.selected = ax::Color(0.4);
	//		btn_info.contour = ax::Color(0.4);
	//		btn_info.font_color = ax::Color(0.8);
	//		btn_info.round_corner_radius = 5.0;
	//
	//		std::shared_ptr<ax::Window::Backbone> btn = ax::Button::Create(
	//			ax::Rect(pos, ax::Size(60, 25)), ax::Button::Events(), "Button", "", btn_info);
	//
	//		ax::App::GetInstance().GetPopupManager()->GetWindowTree()->AddTopLevel(btn->GetWindow());
	//		btn->GetWindow()->backbone = btn;
	//	}
	//
	//	if (obj_type == "Toggle") {
	//
	//		ax::Toggle::Info tog_info;
	//		tog_info.normal = ax::Color(0.7);
	//		tog_info.hover = ax::Color(0.75);
	//		tog_info.clicking = ax::Color(0.65);
	//		tog_info.selected = ax::Color(0.7);
	//		tog_info.font_color = ax::Color(0.3);
	//
	//		tog_info.selected = ax::Color(0.4);
	//		tog_info.selected_hover = ax::Color(0.45);
	//		tog_info.selected_clicking = ax::Color(0.35);
	//		tog_info.selected = ax::Color(0.4);
	//		tog_info.selected_font_color = ax::Color(0.8);
	//
	//		tog_info.contour = ax::Color(0.7);
	//		tog_info.font_size = 12;
	//
	//		std::shared_ptr<ax::Window::Backbone> tog = ax::shared<ax::Toggle>(
	//			ax::Rect(pos, ax::Size(60, 25)), ax::Toggle::Events(), tog_info, "", "Toggle");
	//
	//		ax::App::GetInstance().GetPopupManager()->GetWindowTree()->AddTopLevel(tog->GetWindow());
	//		tog->GetWindow()->backbone = tog;
	//	}
	//
	//	if (obj_type == "Toggle2") {
	//
	//		ax::Toggle::Info tog_info;
	//		tog_info.normal = ax::Color(0.0, 0.0);
	//		tog_info.hover = ax::Color(0.0, 0.0);
	//		tog_info.clicking = ax::Color(0.0, 0.0);
	//		tog_info.selected = ax::Color(0.0, 0.0);
	//		tog_info.font_color = ax::Color(0.3);
	//
	//		tog_info.selected = ax::Color(0.0, 0.0);
	//		tog_info.selected_hover = ax::Color(0.0, 0.0);
	//		tog_info.selected_clicking = ax::Color(0.0, 0.0);
	//		tog_info.selected = ax::Color(0.0, 0.0);
	//		tog_info.selected_font_color = ax::Color(0.8);
	//
	//		tog_info.contour = ax::Color(0.0, 0.0);
	//		tog_info.font_size = 12;
	//		tog_info.img = "resources/seq_toggle.png";
	//
	//		std::shared_ptr<ax::Window::Backbone> tog = ax::shared<ax::Toggle>(
	//			ax::Rect(pos, ax::Size(55, 55)), ax::Toggle::Events(), tog_info, "", "1");
	//
	//		ax::App::GetInstance().GetPopupManager()->GetWindowTree()->AddTopLevel(tog->GetWindow());
	//		tog->GetWindow()->backbone = tog;
	//	}
	//
	//	if (obj_type == "Knob") {
	//
	//		ax::Knob::Info k_info;
	//		k_info.bgColorNormal = ax::Color(0.0, 0.0);
	//		k_info.bgColorHover = ax::Color(0.0, 0.0);
	//		k_info.bgColorClicked = ax::Color(0.0, 0.0);
	//
	//		k_info.img_path = "resources/LittlePhatty.png";
	//		k_info.selected_img_path = "resources/LittlePhatty.png";
	//		k_info.knob_size = ax::Size(64, 64);
	//		k_info.n_knob = 101;
	//
	//		std::shared_ptr<ax::Window::Backbone> tog
	//			= ax::shared<ax::Knob>(ax::Rect(pos, k_info.knob_size), ax::Knob::Events(), k_info);
	//
	//		ax::App::GetInstance().GetPopupManager()->GetWindowTree()->AddTopLevel(tog->GetWindow());
	//		tog->GetWindow()->backbone = tog;
	//	}
	//
	//	if (obj_type == "Knob2") {
	//
	//		ax::Knob::Info k_info;
	//		k_info.bgColorNormal = ax::Color(0.0, 0.0);
	//		k_info.bgColorHover = ax::Color(0.0, 0.0);
	//		k_info.bgColorClicked = ax::Color(0.0, 0.0);
	//
	//		k_info.img_path = "resources/JP8000.png";
	//		k_info.selected_img_path = "resources/JP8000.png";
	//		k_info.knob_size = ax::Size(64, 64);
	//		k_info.n_knob = 101;
	//
	//		std::shared_ptr<ax::Window::Backbone> tog
	//			= ax::shared<ax::Knob>(ax::Rect(pos, k_info.knob_size), ax::Knob::Events(), k_info);
	//
	//		ax::App::GetInstance().GetPopupManager()->GetWindowTree()->AddTopLevel(tog->GetWindow());
	//		tog->GetWindow()->backbone = tog;
	//	}
	//
	//	if (obj_type == "Knob3") {
	//
	//		ax::Knob::Info k_info;
	//		k_info.bgColorNormal = ax::Color(0.0, 0.0);
	//		k_info.bgColorHover = ax::Color(0.0, 0.0);
	//		k_info.bgColorClicked = ax::Color(0.0, 0.0);
	//
	//		k_info.img_path = "resources/BlackAngle.png";
	//		k_info.selected_img_path = "resources/BlackAngle.png";
	//		k_info.knob_size = ax::Size(100, 100);
	//		k_info.n_knob = 20;
	//
	//		std::shared_ptr<ax::Window::Backbone> tog
	//			= ax::shared<ax::Knob>(ax::Rect(pos, k_info.knob_size), ax::Knob::Events(), k_info);
	//
	//		ax::App::GetInstance().GetPopupManager()->GetWindowTree()->AddTopLevel(tog->GetWindow());
	//		tog->GetWindow()->backbone = tog;
	//	}
	//
	//	if (obj_type == "Knob4") {
	//
	//		ax::Knob::Info k_info;
	//		k_info.bgColorNormal = ax::Color(0.0, 0.0);
	//		k_info.bgColorHover = ax::Color(0.0, 0.0);
	//		k_info.bgColorClicked = ax::Color(0.0, 0.0);
	//
	//		k_info.img_path = "resources/knob_dark.png";
	//		k_info.selected_img_path = "resources/knob_dark.png";
	//		k_info.knob_size = ax::Size(46, 46);
	//		k_info.n_knob = 128;
	//
	//		std::shared_ptr<ax::Window::Backbone> tog
	//			= ax::shared<ax::Knob>(ax::Rect(pos, k_info.knob_size), ax::Knob::Events(), k_info);
	//
	//		ax::App::GetInstance().GetPopupManager()->GetWindowTree()->AddTopLevel(tog->GetWindow());
	//		tog->GetWindow()->backbone = tog;
	//	}
	//
	//	if (obj_type == "Knob5") {
	//
	//		ax::Knob::Info k_info;
	//		k_info.bgColorNormal = ax::Color(0.0, 0.0);
	//		k_info.bgColorHover = ax::Color(0.0, 0.0);
	//		k_info.bgColorClicked = ax::Color(0.0, 0.0);
	//
	//		k_info.img_path = "resources/knob_blue_36x36.png";
	//		k_info.selected_img_path = "resources/knob_blue_36x36.png";
	//		k_info.knob_size = ax::Size(36, 36);
	//		k_info.n_knob = 128;
	//
	//		std::shared_ptr<ax::Window::Backbone> tog
	//			= ax::shared<ax::Knob>(ax::Rect(pos, k_info.knob_size), ax::Knob::Events(), k_info);
	//
	//		ax::App::GetInstance().GetPopupManager()->GetWindowTree()->AddTopLevel(tog->GetWindow());
	//		tog->GetWindow()->backbone = tog;
	//	}
	//
	//	if (obj_type == "Knob6") {
	//
	//		ax::Knob::Info k_info;
	//		k_info.bgColorNormal = ax::Color(0.0, 0.0);
	//		k_info.bgColorHover = ax::Color(0.0, 0.0);
	//		k_info.bgColorClicked = ax::Color(0.0, 0.0);
	//
	//		k_info.img_path = "resources/axKnobTB303_64x64.png";
	//		k_info.selected_img_path = "resources/axKnobTB303_64x64.png";
	//		k_info.knob_size = ax::Size(64, 64);
	//		k_info.n_knob = 101;
	//
	//		std::shared_ptr<ax::Window::Backbone> tog
	//			= ax::shared<ax::Knob>(ax::Rect(pos, k_info.knob_size), ax::Knob::Events(), k_info);
	//
	//		ax::App::GetInstance().GetPopupManager()->GetWindowTree()->AddTopLevel(tog->GetWindow());
	//		tog->GetWindow()->backbone = tog;
	//	}
	//	if (obj_type == "Knob7") {
	//
	//		ax::Knob::Info k_info;
	//		k_info.bgColorNormal = ax::Color(0.0, 0.0);
	//		k_info.bgColorHover = ax::Color(0.0, 0.0);
	//		k_info.bgColorClicked = ax::Color(0.0, 0.0);
	//
	//		k_info.img_path = "resources/axKnobReaktor.png";
	//		k_info.selected_img_path = "resources/axKnobReaktor.png";
	//		k_info.knob_size = ax::Size(46, 46);
	//		k_info.n_knob = 128;
	//
	//		std::shared_ptr<ax::Window::Backbone> tog
	//			= ax::shared<ax::Knob>(ax::Rect(pos, k_info.knob_size), ax::Knob::Events(), k_info);
	//
	//		ax::App::GetInstance().GetPopupManager()->GetWindowTree()->AddTopLevel(tog->GetWindow());
	//		tog->GetWindow()->backbone = tog;
	//	}
	//
	//	if (obj_type == "LineFollower") {
	//		std::shared_ptr<ax::Window::Backbone> ctrl
	//			= ax::shared<ctrl::LineFollower>(ax::Rect(pos, ax::Size(300, 300)));
	//
	//		ax::App::GetInstance().GetPopupManager()->GetWindowTree()->AddTopLevel(ctrl->GetWindow());
	//		ctrl->GetWindow()->backbone = ctrl;
	//	}
}

void MainWindow::OnDraggingWidget(const ax::Event::SimpleMsg<ax::Point>& msg)
{
	ax::Point pos(msg.GetMsg());
	ax::Window::Ptr wobj = ax::App::GetInstance().GetPopupManager()->GetWindowTree()->GetTopLevel();

	if (wobj) {
		wobj->dimension.SetPosition(pos);
	}
}

void MainWindow::OnReleaseObjWidget(const ax::Event::SimpleMsg<ax::Point>& msg)
{
	std::vector<ax::Window::Ptr>& nodes
		= ax::App::GetInstance().GetPopupManager()->GetWindowTree()->GetNodeVector();
	ax::Window::Ptr win = nodes[0];

	nodes.clear();

	ax::Point pos(msg.GetMsg());
	bool inside_grid(_gridWindow->GetWindow()->dimension.GetAbsoluteRect().IsPointInside(pos));

	if (inside_grid) {
		std::vector<ax::Window::Ptr>& children(_gridWindow->GetWindow()->node.GetChildren());
		ax::Window::Ptr child_win = nullptr;

		for (auto& n : children) {
			if (n->property.HasProperty("AcceptWidget")) {
				ax::Rect abs_rect(n->dimension.GetAbsoluteRect());

				if (abs_rect.IsPointInside(pos)) {
					child_win = n;
				}
			}
		}

		if (child_win) {
			ax::Print("FOUND WINDOW");
			// Reparent.
			win->node.SetParent(child_win.get());
			child_win->node.GetChildren().push_back(win);
			win->dimension.SetPosition(pos - child_win->dimension.GetAbsoluteRect().position);
		}
		else {

			// Reparent.
			win->node.SetParent(_gridWindow->GetWindow().get());
			_gridWindow->GetWindow()->node.GetChildren().push_back(win);
			win->dimension.SetPosition(pos - _gridWindow->GetWindow()->dimension.GetAbsoluteRect().position);
		}
	}
}

void MainWindow::OnResize(const ax::Size& size)
{
	// Resize main window.
	win->dimension.SetSize(size);

	// Resize top menu.
	ax::Size top_menu_size(_statusBar->GetWindow()->dimension.GetSize());
	top_menu_size.x = size.x;
	_statusBar->GetWindow()->dimension.SetSize(top_menu_size);

	// Resize widget menu.
	ax::Size widget_menu_size(_widgetMenu->GetWindow()->dimension.GetSize());
	widget_menu_size.y = size.y - 40 - 25;
	_widgetMenu->GetWindow()->dimension.SetSize(widget_menu_size);
}

void MainWindow::OnPaint(ax::GC gc)
{
	ax::Print("Main window draw");
	ax::Rect rect(ax::Point(0, 0), win->dimension.GetSize());

	gc.SetColor(ax::Color(0.3));
	gc.DrawRectangle(rect);
	gc.DrawRectangleContour(rect);

	gc.SetColor(ax::Color(0.15));
	gc.DrawRectangle(ax::Rect(0, rect.size.y - 18, rect.size.x, 18));
}
}
