/***
 * @file falcon_mouse.h
 * @brief Prototype stubs for platform specific mouse execution functions
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

#ifndef FALCONMOUSE_H
#define FALCONMOUSE_H

bool initDisplay();
void setMousePosition(int x, int y);
void closeDisplay();
int getDisplayWidth();
int getDisplayHeight();

#endif
