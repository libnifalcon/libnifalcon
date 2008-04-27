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


	/*
	float e = 8, f = 16, re = 10.3094, rf = 8, t1=55.19, t2=9.5, t3=37.8;
	float w1,x1,y1,z1,w2,x2,y2,z2,ar, b1, b2, d;
	float x,y,z,a,b,c,p01,p02,p03,p23,p31,p12,p2,tr1,tr2,tr3,tr,dtr,r3,ef,e1x,e1y,e1z,e2x,e2y,e2z,e3x,e3y,e3z;

	// Leaving the original version of the code from the paper here.
	dtr = 3.1415926/180.0;
	r3 = sqrt(3.0);

	tr1 = t1*dtr;
	tr2 = t2*dtr;
	tr3 = t3*dtr;
	tr = 2.0*r3;

	ef = (e-f)/tr;
	e1y=ef-rf*cos(tr1);
	e1z=-rf*sin(tr1);
	printf("e1 %f %f\n", e1y, e1z);
	
	e2y=(rf*cos(tr2)-ef)/2.0;
	e2x=e2y*r3;
	e2z=-rf*sin(tr2);
	printf("e2 %f %f %f\n", e2x, e2y, e2z);

	e3y=(rf*cos(tr3)-ef)/2.0;
	e3x=-e3y*r3;
	e3z=-rf*sin(tr3);
	printf("e3 %f %f %f\n", e3x, e3y, e3z);

	w1=((e1y*e1y)-(e2x*e2x)-(e2y*e2y)+(e1z*e1z)-(e2z*e2z))/2.0;
	x1=e2x;
	y1=e2y-e1y;
	z1=e2z-e1z;

	w2=((e2x*e2x)-(e3x*e3x)+(e2y*e2y)-(e3y*e3y)+(e2z*e2z)-(e3z*e3z))/2.0;
	x2=e3x-e2x;
	y2=e3y-e2y;
	z2=e3z-e2z;
	
	p01=y1*z2-y2*z1;
	if(!p01)
	{
		printf("p01 == 0 : %f %f %f %f %f\n", y1, y2, z1, z2, sin(tr1));
		return 1;
	}
	p02=z1*x2-z2*x1;
	p03=x1*y2-x2*y1;

	p23=w1*x2-w2*x1;
	p31=w1*y2-w2*y1;
	p12=w1*z2-w2*z1;
	p2=p01*p01;

	a=p2+(p02*p02)+(p03*p03);
	b1=p12+p01*e1y;
	b2=p31-p01*e1z;
	b=p02*b1-p03*b2;

	c=(b1*b1)+(b2*b2)-(p2*(re*re));
	ar=(b*b)-(a*c);
	if(ar < 0)
	{
		printf("ar < 0\n");
		return 1;
	}

	d=sqrt(ar);
	x=(b+d)/a;
	y=(p02*x-p12)/p01;
	z=(p03*x+p31)/p01;

	printf("%f %f %f %f\n%f %f %f\n%f %f %f\n", e, f, re, rf, t1, t2, t3, x, y, z);
		
*/	


#include "nifalcon_test_fw.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
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

typedef struct point3f {
	float x;
	float y;
	float z;
} point3f;

typedef struct point4f {
	float w;
	float x;
	float y;
	float z;
} point4f;

typedef struct falcon_kinematics {
	
	float f, e, re, rf;

	//thigh angles 
	float thigh_angle[3];
	//knees e1, e2, e3
	point3f knee[3];
	//collisions midpoints e12, e23, e31
	point4f col[3];
	//intersection solutions
	point3f solution[2];
} falcon_kinematics;

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

GLvoid InitSimulationValues()
{
	/******** Parallel bot unit input section ********/
	/******** Units are assumed to be inches ********/

	/*
    //The original coordinates. Left here just in case you want to check things.
	//fixed foot length
	float f = 16.0;
	//End Effector length
	float e = 8.0;
	//shin length
	float re = 10.3094;
	//thigh length
	float rf = 8.0;
	*/

	//Units for the novint falcon

	//fixed frame length
	//I don't have a damn /clue/ what this really is
	//That's part of why I wrote this simulator, so I can just throw numbers until something seems right.
	//It's probably derivable from the measuring the closest distance between the fixed frame origin and
	//the innermost workspace point, but really, do I care?
	
	dk.f = 3.0;

	//End Effector length
	//Measured with a ruler!
	dk.e = 3.0;
	
	//shin length
	//Measued with a ruler! Includes the thigh/shin and shin/effector jointy thingy length
	dk.re = 5.0;
	
	//thigh length
	//It's bent, so it's funky. Let's just say 4.
	dk.rf = 4.0;

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

GLint ComputeDirectKinematics(GLvoid)
{
	float tr, tr1, tr2, tr3, p01, p02, p03, p12, p23, p31, p2, a, b, c, d, b1, b2, ar;
	float ef;
	float r3 = sqrt(3.0);

	++lps;
	//Default thigh angles
	dk.thigh_angle[0] = 55.19;
	dk.thigh_angle[1] = 30.5;
	dk.thigh_angle[2] = 37.8;

	// Lots of assumptions being made here.
	// First off, I have no idea what the actual encoder min/max for the falcon is.
	// However, in practice it seems to be around -/+1750. This puts us close to but
	// not exactly at 90 degrees from the center of the fixed frame on either side.
	// Therefore, the encoder values are buffered around 250 ticks in to account for
	// this.
	
	dk.thigh_angle[0] = (((float)output_packet.motor[0] + 2000.0f)/4000.0f) * 90.0f;
	dk.thigh_angle[1] = (((float)output_packet.motor[1] + 2000.0f)/4000.0f) * 90.0f;
	dk.thigh_angle[2] = (((float)output_packet.motor[2] + 2000.0f)/4000.0f) * 90.0f;

	//Convert all input angles to radians since that's what the c math funcs eat
	tr1 = dk.thigh_angle[0] * DEGREES_TO_RADIANS;
	tr2 = dk.thigh_angle[1] * DEGREES_TO_RADIANS;
	tr3 = dk.thigh_angle[2] * DEGREES_TO_RADIANS;
	tr = 2.0*r3;
	
	ef = (dk.e-dk.f)/tr;

	/******** Hip/Hip Restraint/Knee Calculation and Rendering ********/

	// All of our calculations here use rf (thigh length, also the radius of the straint sphere
	// of the thigh) as a scalar.
	// Pseudocode: 
	// for N[1:3]	
	// eNy = (rf*cos(trN)-ef)/2.0)
	// eNx = (eNy * sqrt(3))
	// eNz = -rf*(sin(tr1));

	//No e1x because it's always 0. Yay physical constraints!
	dk.knee[0].x= 0;
	dk.knee[0].y= ef-dk.rf*cos(tr1);
	dk.knee[0].z= -dk.rf*sin(tr1);

	dk.knee[1].y=(dk.rf*cos(tr2)-ef)/2.0;
	dk.knee[1].x=dk.knee[1].y*r3;
	dk.knee[1].z=-dk.rf*sin(tr2);

	dk.knee[2].y=(dk.rf*cos(tr3)-ef)/2.0;
	dk.knee[2].x=-dk.knee[2].y*r3;
	dk.knee[2].z=-dk.rf*sin(tr3);
	
	/******** Shin/Effector Position Calculation and Rendering ********/

	//Basically, we're finding the two intersection points of the 3 constraint spheres
	//with centers at the knee positions and radii of the knee constraint length (re).
	//This is done by finding the intersection of two of the constraint spheres, then
	//testing the third against the resulting circle to find the 2 intersection points.
	//We then need to figure out which point is valid. This choice is made by the location
	//of knees 2 and 3, as can be seen in the code below.
	
	//Compute the homogenenous coordinates of the intersection plane
	//of the constraint spheres of knee e1 and e2, and e2 and e3
	//The original equation for this looks like
	// {w:x:y:z} =
	// { (rf^2 - re^2 + eNx^2 - e(N+1)x^2 + eNy^2 - e(N+1)y^2 + eNz^2 - e(N+1)z^2) / 2 : e(N+1)x - eNx : e(N+1)y - eNy : e(N+1)z - eNz }
	
	//Coords for e1/e2
	//w1 looks funky because e1x is 0, so a few things get shifted for readability
	dk.col[0].w=((dk.knee[0].y*dk.knee[0].y)-(dk.knee[1].x*dk.knee[1].x)-(dk.knee[1].y*dk.knee[1].y)+(dk.knee[0].z*dk.knee[0].z)-(dk.knee[1].z*dk.knee[1].z))/2.0;
	dk.col[0].x=dk.knee[1].x;
	dk.col[0].y=dk.knee[1].y-dk.knee[0].y;
	dk.col[0].z=dk.knee[1].z-dk.knee[0].z;
	
	dk.col[1].w=((dk.knee[1].x*dk.knee[1].x)-(dk.knee[2].x*dk.knee[2].x)+(dk.knee[1].y*dk.knee[1].y)-(dk.knee[2].y*dk.knee[2].y)+(dk.knee[1].z*dk.knee[1].z)-(dk.knee[2].z*dk.knee[2].z))/2.0;
	dk.col[1].x=dk.knee[2].x-dk.knee[1].x;
	dk.col[1].y=dk.knee[2].y-dk.knee[1].y;
	dk.col[1].z=dk.knee[2].z-dk.knee[1].z;
	
	dk.col[2].w=((dk.knee[0].y*dk.knee[0].y)-(dk.knee[2].x*dk.knee[2].x)-(dk.knee[2].y*dk.knee[2].y)+(dk.knee[0].z*dk.knee[0].z)-(dk.knee[2].z*dk.knee[2].z))/2.0;
	dk.col[2].x=dk.knee[2].x;
	dk.col[2].y=dk.knee[2].y-dk.knee[0].y;
	dk.col[2].z=dk.knee[2].z-dk.knee[0].z;

	//Plucker coordinate derivation for intersection of the collision circles
	//Explanation at http://en.wikipedia.org/wiki/Line_geometry - Check the Dual Coordinates section
	
	p01 = dk.col[0].y * dk.col[1].z - dk.col[1].y * dk.col[0].z;
	if(!p01)
	{
//		printf("p01 == 0\n");
		current_error = 1;
		return 0;
	}
	p02 = dk.col[0].z * dk.col[1].x - dk.col[1].z * dk.col[0].x;
	p03 = dk.col[0].x * dk.col[1].y - dk.col[1].x * dk.col[0].y;	
	p23 = dk.col[0].w * dk.col[1].x - dk.col[1].w * dk.col[0].x;
	p31 = dk.col[0].w * dk.col[1].y - dk.col[1].w * dk.col[0].y;
	p12 = dk.col[0].w * dk.col[1].z - dk.col[1].w * dk.col[0].z;
	
//		printf("p %f %f %f %f\n", p01, p02, p03, p23, p31, p12);

	//Now that we've got the Plucker coords, start building the quadratic equation as
	//laid out in the paper		
	p2 = p01 * p01;
	
	a = p2 + (p02 * p02) + (p03 * p03);
	b1 = p12 + p01 * dk.knee[0].y;
	b2 = p31- p01 * dk.knee[0].z;
	b = p02 * b1 - p03 * b2;
	
	c = (b1 * b1) + (b2 * b2) - (p2 * (dk.re * dk.re));
	ar = (b * b) - (a * c);
	if(ar < 0) 
	{
//		printf("ar < 0\n");
		current_error = 1;
		return 0;
	}		
	d = sqrt(ar);
	
//		printf("quad %f %f %f %f\n", a, b, c, d);

	//Solve for X, then use the univariate X to find y and z
	dk.solution[0].x = (b + d) / a;
	dk.solution[0].y = (p02 * dk.solution[0].x - p12) / p01;
	dk.solution[0].z = (p03 * dk.solution[0].x + p31) / p01;

	//Remember, a quadratic gives you back two solutions.
	dk.solution[1].x = (-b + d) / a;
	dk.solution[1].y = -(( p02 * dk.solution[1].x - p12) / p01);
	dk.solution[1].z = -(( p03 * dk.solution[1].x + p31)  /  p01);
	dk.solution[1].x = -dk.solution[1].x;

}

GLvoid IdleLoop()
{
	current_error = 0;
	if(!RunFalconIO()) return;
	if(!current_error) ComputeDirectKinematics();
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

	InitFalcon();
	InitSimulationValues();

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
