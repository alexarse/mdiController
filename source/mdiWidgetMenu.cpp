#include "mdiApplicationManager.hpp"
#include "mdiWidgetMenu.hpp"

#include <axLib/axOSFileSystem.h>
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

WidgetMenuObj::WidgetMenuObj(const ax::Rect& rect, const std::string& name, const std::string& title,
	const std::string& info, const std::string& size, const std::string& img_path,
	const ax::StringPairVector& attributes)
	: _font("resources/FreeSansBold.ttf")
	, _font_normal(0)
	, _name(name)
	, _title(title)
	, _info(info)
	, _size_str(size)
	, _attributes(attributes)
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
	using MsgType = std::tuple<ax::Point, std::string, ax::StringPairVector>;
	auto msg_data = MsgType(pos, _name, _attributes);
	ApplicationManager::GetMainEvtObj()->PushEvent(8000, new ax::Event::SimpleMsg<MsgType>(msg_data));
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

	ax::Point pos(ParseControlFolder());

	ax::ScrollBar::Info sInfo;
	sInfo.normal = ax::Color(0.22);
	sInfo.hover = ax::Color(0.23);
	sInfo.clicking = ax::Color(0.21);
	sInfo.slider_contour = ax::Color(0.0, 0.2);
	sInfo.contour = ax::Color(0.0, 0.0);
	sInfo.bg_top = ax::Color(0.8, 0.2);
	sInfo.bg_bottom = ax::Color(0.82, 0.2);

	const int scroll_bar_width(10);
	ax::Rect sRect(rect.size.x - scroll_bar_width + 1, 0, scroll_bar_width, rect.size.y - 1);
	_scrollBar = ax::shared<ax::ScrollBar>(sRect, ax::ScrollBar::Events(), sInfo);

	win->node.Add(_scrollBar);

	_panel->property.AddProperty("BlockDrawing");
	_panel->dimension.SetSizeNoShowRect(ax::Size(rect.size.x, pos.y));

	_scrollBar->SetWindowHandle(_panel.get());
	_scrollBar->UpdateWindowSize(ax::Size(rect.size.x, pos.y));
}

ax::Point WidgetMenu::ParseControlFolder()
{
	ax::os::Directory dir;
	dir.Goto("plugin");

	ax::StringVector ctrl_type;

	for (auto& n : dir.GetContent()) {
		if (n.type == ax::os::File::FOLDER) {
			ax::Print(n.name);
			ctrl_type.push_back(n.name);
		}
	}

	const ax::Rect rect(win->dimension.GetRect());
	const ax::Size size(rect.size.x, 50);
	const ax::Size separator_size(rect.size.x, 20);

	ax::Point pos(0, 0);

	for (auto n : ctrl_type) {
		const ax::Rect sep_rect(pos, separator_size);
		ax::Window::Ptr last = _panel->node.Add(ax::shared<WidgetMenuSeparator>(sep_rect, n));
		pos = last->dimension.GetRect().GetNextPosDown(0);

		ax::os::Directory ctrl_dir;
		ctrl_dir.Goto("plugin/" + n);

		for (auto& file : ctrl_dir.GetContent()) {
			if (file.ext == "xml") {
				// Read the xml file into a vector.
				std::ifstream f_path("plugin/" + n + "/" + file.name);
				std::vector<char> buffer(
					(std::istreambuf_iterator<char>(f_path)), std::istreambuf_iterator<char>());
				buffer.push_back('\0');

				try {
					rapidxml::xml_document<> doc;
					doc.parse<0>(&buffer[0]);

					rapidxml::xml_node<>* node = doc.first_node("widget");
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

					ax::StringPairVector attributes;
					rapidxml::xml_node<>* child = node->first_node();

					if (child) {
						rapidxml::xml_attribute<>* child_att = child->first_attribute();

						if (child_att) {
							do {
								attributes.push_back(ax::StringPair(child_att->name(), child_att->value()));
								child_att = child_att->next_attribute();
							} while (child_att);
						}
					}

					auto obj(ax::shared<WidgetMenuObj>(ax::Rect(pos, size), widget_id, widget_label,
						widget_desc, widget_size, widget_img, attributes));

					pos = _panel->node.Add(obj)->dimension.GetRect().GetNextPosDown(0);
				}
				catch (rapidxml::parse_error err) {
					ax::Error("Widget menu xml", err.what());
				}
			}
		}
	}
	
	return pos;
}

void WidgetMenu::OnPaint(ax::GC gc)
{
	ax::Rect rect(win->dimension.GetDrawingRect());

	gc.SetColor(ax::Color(0.45));
	gc.DrawRectangle(rect);
	gc.DrawRectangleContour(rect);
}
}