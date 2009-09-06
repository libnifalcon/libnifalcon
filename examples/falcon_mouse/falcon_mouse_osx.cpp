/***
 * @file falcon_mouse_osx.cpp
 * @brief Mouse event generation functions for OS X
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
#include <ApplicationServices/ApplicationServices.h>

bool initDisplay()
{
	return true;
}

void setMousePosition(int x, int y)
{
	CGPoint pt;
	pt.x = x;
	pt.y = y;
	CGPostMouseEvent(pt, 1, 1, 0);
}

void closeDisplay()
{
}

int getDisplayWidth()
{
	return CGDisplayPixelsWide(CGMainDisplayID());
}

int getDisplayHeight()
{
	return CGDisplayPixelsHigh(CGMainDisplayID());
}
