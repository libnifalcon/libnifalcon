#include <iostream>
#ifdef WIN32
#include <windows.h>
#endif
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include "controller/Controller.h"

#include "boost/thread.hpp"
#include "boost/bind.hpp"

controller::Controller *control = 0;
int windowId;

void display()
{
	if (control)
		control->display();
	glutSwapBuffers();
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	if (control)
		control->reshape(w, h);
}

void mouseButton(int button, int state, int x, int y)
{
	if (control)
		control->mouseButton(button, state, x, y);
}

void mouseMove(int x, int y)
{
	if (control)
		control->mouseMove(x, y);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 27:	// Esc will quit
			exit(1);
		break;
	}
	if (control)
		control->keyboard(key, x, y);
}

void idle()
{
	if (control)
		control->idle();
	glutSetWindow(windowId); 
	glutPostRedisplay();
}

int main(int argc, char **argv)
{	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_ALPHA);
	glutInitWindowPosition( 100, 100);
	glutInitWindowSize( 800, 600);
	windowId = glutCreateWindow(argv[0]);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);
	glutKeyboardFunc(keyboard);	

	control = new controller::Controller();
	control->createViewer();
	control->createToolbox(windowId, idle);
	
	glutMainLoop();

	delete control;

	return 0;
}
