/*
 * Implementation file for NovInt Falcon FindFalcon Example Program
 *
 * Copyright (c) 2008 Kyle Machulis <kyle@nonpolynomial.com> http://www.nonpolynomial.com
 *
 * Sourceforge project @ http://www.sourceforge.net/projects/libnifalcon
 *
 * This library is covered by the MIT License, read LICENSE for details.
 *
 * Various piece of code ripped from NeHe, 'cause it's been a while since I've played with OpenGL
 * http://nehe.gamedev.net/
 */

#include "nifalcon_test_fw.h"
#include "nifalcon_kinematics.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#define DEGREES_TO_RADIANS 3.1415926/180.0
#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 700
#define PACKET_TIMEOUT 1000

int fps, lps, iops, errps, current_error;

falcon_device dev;
falcon_kinematics dk;
float angle = 0.0f;
falcon_packet input_packet, output_packet;
float view_z = 20.0f;

void sigproc()
{
	printf("closing falcon and quitting\n");
	nifalcon_close(&dev);
	exit(0);
}

int InitFalcon()
{
	int num_falcons = 0, status = 0, i = 0;
	nifalcon_test_fw_init_packet(&input_packet);
	nifalcon_test_fw_init_packet(&output_packet);

	nifalcon_init(&dev);
	
	num_falcons = nifalcon_get_count(&dev);
	
	printf("Falcons found: %d\n", num_falcons);

	printf("Opening falcon\n");
	if((status = nifalcon_open(&dev, 0)) < 0)
	{
		printf ("Cannot open falcon! Error: %s\n", nifalcon_get_error_string(&dev));
		return status;
	}
	printf("Opened falcon\n");
	printf("Loading firmware\n");
	for(i = 0; i < 10; ++i)
	{
		if((status = nifalcon_load_firmware(&dev, "test_firmware.bin")) == 0)
		{
			break;
		}
		printf("Firmware not loaded! Error: %d %s\n", dev.falcon_status_code, nifalcon_get_error_string(&dev));
	}
	if(i==10)
	{
		printf("Cannot load firmware, bailing...");
		return status;
	}
	printf("Firmware loaded\n");
	return 0;
}

GLvoid InitGL(GLvoid)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_BLEND);
	glEnable ( GL_COLOR_MATERIAL );
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void keyboard ( unsigned char key, int x, int y )  // Create Keyboard Function
{
	switch ( key )
	{
    case 27:        // When Escape Is Pressed...
		nifalcon_close(&dev);
		exit ( 0 );   // Exit The Program
		break;        // Ready For Next Case
    default:        // Now Wrap It Up
		break;
	}
}

GLint RunFalconIO(GLvoid)
{
	int status;
	++iops;

	/******** Falcon I/O section ********/

	input_packet.info |= NOVINT_TEST_FW_HOMING_MODE;
	if(!(output_packet.info & (NOVINT_TEST_FW_HOMED_AXIS1 | NOVINT_TEST_FW_HOMED_AXIS2 | NOVINT_TEST_FW_HOMED_AXIS3)))
	{
		input_packet.info &= ~NOVINT_TEST_FW_LED_GREEN;
		input_packet.info |= NOVINT_TEST_FW_LED_RED;
	}
	else
	{
		input_packet.info &= ~NOVINT_TEST_FW_LED_RED;
		input_packet.info |= NOVINT_TEST_FW_LED_GREEN;
	}

	if((status = nifalcon_test_fw_send_struct(&dev, &input_packet)) < 0)
	{
		++errps;
		current_error = 1;
		return 0;
	}
	if((status = nifalcon_test_fw_receive_struct(&dev, &output_packet, 0)) < 0)
	{
		++errps;
		current_error = 1;
		return 0;
	}
	return 1;
}

GLvoid IdleLoop()
{
	current_error = 0;
	if(!RunFalconIO()) return;
	if(!current_error) nifalcon_direct_kinematics(&dk, output_packet.motor[0], output_packet.motor[1], output_packet.motor[2]);
//	if(dk.solution[0].z < 0) printf("0: %f %f %f\n", dk.solution[0].x, dk.solution[0].y, dk.solution[0].z);
//	else printf("1: %f %f %f\n", dk.solution[1].x, dk.solution[1].y, dk.solution[1].z);
}

GLvoid RenderTimer(GLint arg)
{
	glutPostRedisplay();
	glutTimerFunc(40, RenderTimer, 0);
}

GLvoid DrawGLScene(GLvoid)
{	

	/******** Rendery goodness initialization ********/
	
	//Circumradius and inradius of FF equilateral triangle (Needed for rendering fixed frame)
	float ff_ccr = (1.0/3.0)*(sqrt(3.0))*dk.f;
	float ff_ir = (1.0/6.0)*(sqrt(3.0))*dk.f;

	GLUquadric* gluq = gluNewQuadric();
	
	/******** Silly extra variable initialization section ********/
	
	float dtr, tr, tr1, tr2, tr3, r3, ef;
	int i;
	int color_shift, render_angle;

	/******** Camera/Viewpoint Setup ********/
	++fps;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	//Camera
	if(output_packet.info & NOVINT_TEST_FW_BUTTON4)
	{
		angle += 1.0;
	}
	if(output_packet.info & NOVINT_TEST_FW_BUTTON1)
	{
		angle -= 1.0;
	}
	if(output_packet.info & NOVINT_TEST_FW_BUTTON3)
	{
		view_z += 0.75;
	}
	if(output_packet.info & NOVINT_TEST_FW_BUTTON2)
	{
		view_z -= 0.75;
	}   		

	gluLookAt(cos(angle*DEGREES_TO_RADIANS)*view_z, 0, sin(angle*(DEGREES_TO_RADIANS)) * view_z, 0, 0, 0, 0, 1, 0);

	//This was originally written to look like the drawings in the paper
	//To make it look like the falcon, just rotate everything 180 on the z.

	glRotatef(180.0f, 0,0,1);

	/******** Fixed Frame Rendering ********/

	glPushMatrix();
	//Fixed Frame Marker
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
	glBegin(GL_TRIANGLES);
		glVertex3f( -dk.f * .5,  -ff_ir, 0.0f);
		glVertex3f(  dk.f * .5,  -ff_ir, 0.0f);
		glVertex3f(       0.0f,  ff_ccr, 0.0f);
	glEnd();

	//Fixed Frame Endpoint Check
	glPushMatrix();
	glTranslatef(0.0, ff_ccr, 0.0);
	gluSphere(gluq,.25, 100, 100);
	glPopMatrix();
	glPushMatrix();
	glRotatef(120.0f, 0,0,1);
	glTranslatef(0.0, ff_ccr, 0.0);
	gluSphere(gluq,.25, 100, 100);
	glPopMatrix();
	glPushMatrix();
	glRotatef(240.0f, 0,0,1);
	glTranslatef(0.0, ff_ccr, 0.0);
	gluSphere(gluq,.25, 100, 100);
	glPopMatrix();

	
	//Origin Marker
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	gluSphere(gluq,.25, 100, 100);

	for(i = 0, render_angle=60; i < 3; render_angle+=120, ++i)
	{
		//Hip Rendering
		glPushMatrix();	
		glRotatef(render_angle, 0.0f, 0.0f, 1.0f);
		glColor4f(1.0f, 1.0f, 0.0f, 0.2f);
		//Hip Placement Sphere
		glTranslatef(0.0f, ff_ir/2, 0.0f);
		gluSphere(gluq, 1, 100, 100);		
		//Hip Restraint Sphere
		glColor4f(1.0f, 1.0f, 0.0f, 0.1f);
		gluSphere(gluq, dk.rf, 100, 100);
		glPopMatrix();

		//Knee Rendering
		glPushMatrix();
		glTranslatef(dk.knee[i].x, dk.knee[i].y, dk.knee[i].z);
		//Knee Placement Sphere
		glColor3f(1.0, 1.0, 1.0);
		gluSphere(gluq, 1, 100, 100);
		//Knee Restraint Sphere
		glColor4f(0.0, 0.0, 1.0, 0.1);
		gluSphere(gluq, dk.re, 100, 100);
		glPopMatrix();
	}

	//Plot the solutions.
	for(i = 0; i < 2; ++i)
	{
		glPushMatrix();
		glTranslatef(dk.solution[i].x, dk.solution[i].y, dk.solution[i].z);
		glColor3f(1.0, 1.0, 1.0);
		gluSphere(gluq, 1, 100, 100);
		glPopMatrix();
	}

	glutSwapBuffers();
}

GLvoid ReSizeGLScene(int width, int height)
{
	if (height==0) height=1;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void StatTimer(int arg)
{
	printf("fps %d lps %d iops %d errors %d\n", fps, lps, iops, errps);
	fps = lps = iops = errps = 0;
	glutTimerFunc(1000, StatTimer, 0);
}

int main(int argc, char** argv)
{

	int num_falcons, status, i;
	unsigned int count;
	fps = lps = iops = errps = 0;
	nifalcon_test_fw_init_packet(&input_packet);
	nifalcon_test_fw_init_packet(&output_packet);

	if(InitFalcon() != 0)
	{
		printf("Cannot initialize falcon, returning...\n");
		return 1;
	}

	nifalcon_init_kinematics(&dk);

	glutInit(&argc, argv);	
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB );
	glutInitWindowSize (WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	InitGL();
	
	glutKeyboardFunc    ( keyboard );
	glutDisplayFunc(DrawGLScene);
	glutReshapeFunc(ReSizeGLScene);
	glutIdleFunc(IdleLoop);
	glutTimerFunc(0, RenderTimer, 0);
	glutTimerFunc(1000, StatTimer, 0);
	glutMainLoop();
	return 0;	
}
