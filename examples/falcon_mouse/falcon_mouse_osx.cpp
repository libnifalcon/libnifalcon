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


