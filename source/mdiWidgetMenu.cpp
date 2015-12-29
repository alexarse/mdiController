#include "mdiApplicationManager.hpp"
#include "mdiWidgetMenu.hpp"

#include <axLib/rapidxml.hpp>

namespace mdi {
WidgetMenuSeparator::WidgetMenuSeparator(const ax::Rect& rect, const std::string& name)
	: _font("resources/FreeSansBold.ttf")
	, _name(name)
{
	// Create window.
	win = ax::Window::Create(rect);
	win->event.OnPaint = ax::WBind<ax::GC>(this, &WidgetMenuSeparator::OnPaint);
	win->event.OnMouseLeftDown = ax::WBind<ax::Point>(this, &WidgetMenuSeparator::OnMouseLeftDown);
	win->event.OnMouseLeftDragging = ax::WBind<ax::Point>(this, &WidgetMenuSeparator::OnMouseLeftDragging);
	win->event.OnMouseLeftUp = ax::WBind<ax::Point>(this, &WidgetMenuSeparator::OnMouseLeftUp);
}

void WidgetMenuSeparator::OnMouseLeftDown(const ax::Point& pos)
{
}

void WidgetMenuSeparator::OnMouseLeftDragging(const ax::Point& pos)
{
}

void WidgetMenuSeparator::OnMouseLeftUp(const ax::Point& pos)
{
}

void WidgetMenuSeparator::OnPaint(ax::GC gc)
{
	ax::Rect rect(win->dimension.GetDrawingRect());
	gc.SetColor(ax::Color(0.15));
	//		gc.DrawRectangle(rect);
	gc.DrawRectangleColorFade(rect, ax::Color(0.15), ax::Color(0.25));
	gc.DrawRectangleContour(rect);

	gc.SetColor(ax::Color(0.7));
	gc.DrawString(_font, _name, ax::Point(5, 2));
}

WidgetMenuObj::WidgetMenuObj(const ax::Rect& rect, const std::string& name, const std::string& title,
	const std::string& info, const std::string& size, const std::string& img_path)
	: _font("resources/FreeSansBold.ttf")
	, _font_normal(0)
	, _name(name)
	, _title(title)
	, _info(info)
	, _size_str(size)
{
	_font_normal.SetFontSize(11);

	_img = ax::shared<ax::Image>(img_path);

	// Create window.
	win = ax::Window::Create(rect);
	win->event.OnPaint = ax::WBind<ax::GC>(this, &WidgetMenuObj::OnPaint);
	win->event.OnMouseLeftDown = ax::WBind<ax::Point>(this, &WidgetMenuObj::OnMouseLeftDown);
	win->event.OnMouseLeftDragging = ax::WBind<ax::Point>(this, &WidgetMenuObj::OnMouseLeftDragging);
	win->event.OnMouseLeftUp = ax::WBind<ax::Point>(this, &WidgetMenuObj::OnMouseLeftUp);
}

void WidgetMenuObj::OnMouseLeftDown(const ax::Point& pos)
{
	win->event.GrabMouse();
	ApplicationManager::GetMainEvtObj()->PushEvent(
		8000, new ax::Event::SimpleMsg<std::pair<std::string, ax::Point>>(
				  std::pair<std::string, ax::Point>(_name, pos)));
	win->Update();
}

void WidgetMenuObj::OnMouseLeftDragging(const ax::Point& pos)
{
	ApplicationManager::GetMainEvtObj()->PushEvent(8001, new ax::Event::SimpleMsg<ax::Point>(pos));
}

void WidgetMenuObj::OnMouseLeftUp(const ax::Point& pos)
{
	if (win->event.IsGrabbed()) {
		win->event.UnGrabMouse();
		ApplicationManager::GetMainEvtObj()->PushEvent(8002, new ax::Event::SimpleMsg<ax::Point>(pos));
		win->Update();
	}
}

void WidgetMenuObj::OnPaint(ax::GC gc)
{
	ax::Rect rect(win->dimension.GetDrawingRect());
	gc.SetColor(ax::Color(0.85));
	//		gc.DrawRectangle(rect);
	gc.DrawRectangleColorFade(rect, ax::Color(0.85), ax::Color(0.86));

	ax::Size img_size(_img->GetSize());
	ax::Point img_pos(5 + (65 - img_size.x) / 2, 5 + (rect.size.y - 8 - img_size.y) / 2);
	gc.DrawImage(_img.get(), img_pos);

	gc.SetColor(ax::Color(0.2));
	gc.DrawString(_font, _title, ax::Point(75, 6));

	gc.SetColor(ax::Color(0.2));
	gc.DrawString(_font_normal, _info, ax::Point(75, 20));
	gc.DrawString(_font_normal, _size_str, ax::Point(75, 32));

	gc.SetColor(ax::Color(0.5));
	gc.DrawRectangleContour(rect);
}

WidgetMenu::WidgetMenu(const ax::Rect& rect)
{
	// Create window.
	win = ax::Window::Create(rect);
	win->event.OnPaint = ax::WBind<ax::GC>(this, &WidgetMenu::OnPaint);

	_panel = ax::Window::Create(ax::Rect(ax::Point(1, 1), rect.size - ax::Size(2, 2)));
	win->node.Add(_panel);

	ax::Point pos(0, 0);
	ax::Size size(rect.size.x, 50);
	ax::Size separator_size(rect.size.x, 20);

	// Read the xml file into a vector.
	std::ifstream menu_file_path("resources/widget_menu.xml");
	std::vector<char> xml_buffer(
		(std::istreambuf_iterator<char>(menu_file_path)), std::istreambuf_iterator<char>());
	xml_buffer.push_back('\0');

	rapidxml::xml_document<> doc;

	try {
		// Parse the buffer using the xml file parsing library into doc.
		doc.parse<0>(&xml_buffer[0]);

		rapidxml::xml_node<>* root_node = doc.first_node("WidgetMenu");

		if (root_node) {
			rapidxml::xml_node<>* node = root_node->first_node();

			while (node) {
				std::string node_name(node->name(), node->name_size());

				if (node_name == "separator") {
					rapidxml::xml_attribute<>* att = node->first_attribute("name");
					if (att) {
						std::string separator_name(att->value(), att->value_size());
						ax::Rect sep_rect(pos, separator_size);
						pos = _panel->node.Add(ax::shared<WidgetMenuSeparator>(sep_rect, separator_name))
								  ->dimension.GetRect()
								  .GetNextPosDown(0);
					}
				}
				else if (node_name == "widget") {
					rapidxml::xml_attribute<>* att = node->first_attribute("id");
					std::string widget_id(att->value(), att->value_size());

					att = node->first_attribute("label");
					std::string widget_label(att->value(), att->value_size());

					att = node->first_attribute("description");
					std::string widget_desc(att->value(), att->value_size());

					att = node->first_attribute("size");
					std::string widget_size(att->value(), att->value_size());

					att = node->first_attribute("img");
					std::string widget_img(att->value(), att->value_size());

					pos = _panel->node.Add(ax::shared<WidgetMenuObj>(ax::Rect(pos, size), widget_id,
											   widget_label, widget_desc, widget_size, widget_img))
							  ->dimension.GetRect()
							  .GetNextPosDown(0);
				}

				node = node->next_sibling();
			}
		}
	}
	catch (rapidxml::parse_error err) {
		ax::Error("Widget menu xml", err.what());
	}

	ax::ScrollBar::Info sInfo;
	sInfo.normal = ax::Color(0.22);
	sInfo.hover = ax::Color(0.23);
	sInfo.clicking = ax::Color(0.21);
	sInfo.slider_contour = ax::Color(0.0, 0.2);
	sInfo.contour = ax::Color(0.0, 0.0);
	sInfo.bg_top = ax::Color(0.8, 0.2);
	sInfo.bg_bottom = ax::Color(0.82, 0.2);

	ax::Rect sRect(rect.size.x - 9, 0, 10, rect.size.y - 1);
	_scrollBar = ax::shared<ax::ScrollBar>(sRect, ax::ScrollBar::Events(), sInfo);

	win->node.Add(_scrollBar);

	_panel->property.AddProperty("BlockDrawing");
	_panel->dimension.SetSizeNoShowRect(ax::Size(rect.size.x, pos.y));

	_scrollBar->SetWindowHandle(_panel.get());
	_scrollBar->UpdateWindowSize(ax::Size(rect.size.x, pos.y));
}

void WidgetMenu::OnPaint(ax::GC gc)
{
	ax::Rect rect(win->dimension.GetDrawingRect());

	gc.SetColor(ax::Color(0.45));
	gc.DrawRectangle(rect);
	gc.DrawRectangleContour(rect);
}
}