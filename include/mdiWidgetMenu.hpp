#ifndef mdiWidgetMenu_hpp
#define mdiWidgetMenu_hpp

#include <axLib/axLib.h>
#include <axLib/axScrollBar.h>

namespace mdi {
	class WidgetMenuSeparator : public ax::Window::Backbone {
	public:
		WidgetMenuSeparator(const ax::Rect& rect, const std::string& name);
		
	private:
		ax::Font _font;
		std::string _name;
		
		void OnMouseLeftDown(const ax::Point& pos);
		void OnMouseLeftDragging(const ax::Point& pos);
		void OnMouseLeftUp(const ax::Point& pos);
		void OnPaint(ax::GC gc);
	};
	
	class WidgetMenuObj : public ax::Window::Backbone {
	public:
		WidgetMenuObj(const ax::Rect& rect, const std::string& name,
					  const std::string& title, const std::string& info,
					  const std::string& size, const std::string& img_path);
		
	private:
		ax::Font _font;
		ax::Font _font_normal;
		std::string _name, _title, _info, _size_str;
		std::shared_ptr<ax::Image> _img;
		
		void OnMouseLeftDown(const ax::Point& pos);
		void OnMouseLeftDragging(const ax::Point& pos);
		void OnMouseLeftUp(const ax::Point& pos);
		void OnPaint(ax::GC gc);
	};
	
	class WidgetMenu : public ax::Window::Backbone {
	public:
		WidgetMenu(const ax::Rect& rect);
	private:
		std::shared_ptr<WidgetMenuObj> _objs; // Fuckup for now.
		ax::Window::Ptr _panel;
		ax::ScrollBar::Ptr _scrollBar;
		
		void OnPaint(ax::GC gc);
	};
}

#endif /* mdiWidgetMenu_hpp */
