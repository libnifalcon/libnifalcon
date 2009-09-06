/***
 * @file falcon_mouse_win.cpp
 * @brief Windows mouse event generation functions
 * @author Kyle Machulis (kyle@nonpolynomial.com)
 * @version $Id$
 * @copyright (c) 2007-2008 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * $HeadURL$
 * 
 * Project info at http://libnifalcon.nonpolynomial.com/ 
 *
 */

#include "falcon_mouse.h"
#include <windows.h>

bool initDisplay()
{
	return true;
}

void setMousePosition(int x, int y)
{
	mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, x, y, 0, 0);
}

void closeDisplay()
{
}

int getDisplayWidth()
{
	return GetSystemMetrics(SM_CXBORDER);
}

int getDisplayHeight()
{
	return GetSystemMetrics(SM_CYBORDER);
}

//To press a button
//XTestFakeButtonEvent(display, button, release, 0);

