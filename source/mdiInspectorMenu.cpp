#include "mdiInspectorMenu.hpp"

namespace mdi {
InspectorMenu::InspectorMenu(const ax::Rect& rect)
{
	// Create window.
	win = ax::Window::Create(rect);
	win->event.OnPaint = ax::WBind<ax::GC>(this, &InspectorMenu::OnPaint);

//	ax::Point pos(0, 0);
//	ax::Size size(rect.size.x, 50);
//	ax::Size separator_size(rect.size.x, 20);
//
//	pos = win->node.Add(ax::shared<WidgetMenuSeparator>(ax::Rect(pos, separator_size), "ax::Window"))
//			  ->dimension.GetRect()
//			  .GetNextPosDown(0);
}

void InspectorMenu::OnPaint(ax::GC gc)
{
	ax::Rect rect(win->dimension.GetDrawingRect());

	gc.SetColor(ax::Color(0.85));
	gc.DrawRectangle(rect);

	gc.SetColor(ax::Color(0.7));
	gc.DrawRectangleContour(rect);
}
}