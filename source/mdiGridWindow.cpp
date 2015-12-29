#include "mdiGridWindow.hpp"

namespace mdi {
GridWindow::GridWindow(const ax::Rect& rect)
	: _grid_space(10)
	, _selection(false, ax::Rect(0, 0, 0, 0))
{
	// Create window.
	win = ax::Window::Create(rect);
	win->event.OnPaint = ax::WBind<ax::GC>(this, &GridWindow::OnPaint);
	win->event.OnMouseLeftDown = ax::WBind<ax::Point>(this, &GridWindow::OnMouseLeftDown);
	win->event.OnMouseLeftDragging = ax::WBind<ax::Point>(this, &GridWindow::OnMouseLeftDragging);
	win->event.OnMouseLeftUp = ax::WBind<ax::Point>(this, &GridWindow::OnMouseLeftUp);
}

void GridWindow::SetGridSpace(const int& space)
{
	if (space > 0 && _grid_space != space) {
		_grid_space = space;
		win->Update();
	}
}

void GridWindow::OnMouseLeftDown(const ax::Point& pos)
{
	ax::Point m_pos(pos - win->dimension.GetAbsoluteRect().position);

	_selection.first = true;
	_selection.second.position = m_pos;
	_selection.second.size = ax::Size(1, 1);

	win->event.GrabMouse();
	win->Update();
}

void GridWindow::OnMouseLeftDragging(const ax::Point& pos)
{
	ax::Point m_pos(pos - win->dimension.GetAbsoluteRect().position);
	_selection.second.size = m_pos - _selection.second.position;
	win->Update();
}

void GridWindow::OnMouseLeftUp(const ax::Point& pos)
{
	if (win->event.IsGrabbed()) {
		win->event.UnGrabMouse();
		_selection.first = false;
		win->Update();
	}
}

void GridWindow::OnPaint(ax::GC gc)
{
	ax::Rect rect(win->dimension.GetDrawingRect());

	// Background.
	gc.SetColor(ax::Color(0.9));
	gc.DrawRectangle(rect);

	gc.SetColor(ax::Color(0.8));

	// Vertical lines.
	for (int x = _grid_space; x < rect.size.x; x += _grid_space) {
		gc.DrawLineStripple(ax::Point(x, 0), ax::Point(x, rect.size.y));
	}

	// Horizontal lines.
	for (int y = _grid_space; y < rect.size.y; y += _grid_space) {
		gc.DrawLineStripple(ax::Point(0, y), ax::Point(rect.size.x, y));
	}

	// Selection rectangle.
	if (_selection.first) {
		gc.SetColor(ax::Color(0.7, 0.5));
		gc.DrawRectangle(_selection.second);
		gc.SetColor(ax::Color(0.7, 0.8));
		gc.DrawRectangleContour(_selection.second);
	}

	// Grid contour.
	gc.SetColor(ax::Color(0.7));
	gc.DrawRectangleContour(rect);
}
}