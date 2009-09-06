/***
 * @file falcon_mouse_linux.cpp
 * @brief X11 mouse event generation functions
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
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

Display *display;
Window winRoot;

bool initDisplay()
{
	// Initialise X11 Low-level variables
	display = XOpenDisplay(0);
	if(display == NULL)
		return false;
	winRoot = XDefaultRootWindow(display);
	return true;
}

void setMousePosition(int x, int y)
{
	XTestFakeMotionEvent(display, -1, x, y, 0);
}

void closeDisplay()
{
	XCloseDisplay(display);
}

int getDisplayWidth()
{
	if(display == NULL)
	{
		return -1;
	}
	return XDisplayWidth(display, 0);
}

int getDisplayHeight()
{
	if(display == NULL)
	{
		return -1;
	}
	return XDisplayHeight(display, 0);
}

//To press a button
//XTestFakeButtonEvent(display, button, release, 0);

