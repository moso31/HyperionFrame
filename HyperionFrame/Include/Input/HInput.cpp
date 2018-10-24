#include "HInput.h"

void HInput::OnMouseDown(int x, int y)
{
	for each (auto listener in m_inputList)
	{
		listener->OnMouseDown(x, y);
	}
}

void HInput::OnKeyDown(WPARAM wParam)
{
	for each (auto listener in m_inputList)
	{
		listener->OnKeyDown(wParam);
	}
}
