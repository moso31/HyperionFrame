#include "HInput.h"

void HInput::OnMouseDown(int x, int y)
{
	for (int i = 0; i < m_inputList.size(); i++)
	{
		m_inputList[i]->OnMouseDown(x, y);
	}
}

void HInput::OnKeyDown(WPARAM wParam)
{
	for (int i = 0; i < m_inputList.size(); i++)
	{
		m_inputList[i]->OnKeyDown(wParam);
	}
}
