#ifndef mdiGridWindow_hpp
#define mdiGridWindow_hpp

#include <axLib/axLib.h>

namespace mdi {
class GridWindow : public ax::Window::Backbone {
public:
	GridWindow(const ax::Rect& rect);

	/// Set number of pixels between each grid lines.
	void SetGridSpace(const int& space);

private:
	int _grid_space;
	std::pair<bool, ax::Rect> _selection;

	void OnMouseLeftDown(const ax::Point& pos);
	void OnMouseLeftDragging(const ax::Point& pos);
	void OnMouseLeftUp(const ax::Point& pos);
	void OnPaint(ax::GC gc);
};
}

#endif /* mdiGridWindow_hpp */
