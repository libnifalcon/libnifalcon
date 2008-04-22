#include "nifalcon_test_fw.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <GLUT/glut.h>

#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 700
#define PACKET_TIMEOUT 1000

falcon_device dev;

void sigproc()
{
	printf("closing falcon and quitting\n");
	nifalcon_close(&dev);
	exit(0);
}

GLvoid InitGL(GLvoid)
{
//	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_BLEND);
	glEnable ( GL_COLOR_MATERIAL );
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);	
//	glDepthFunc(GL_LEQUAL);
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

float angle = 0.0;
falcon_packet input_packet, output_packet;
float view_z = 20.0f;

GLvoid DrawGLScene(GLvoid)
{	



	/******** Parallel bot unit input section ********/
	/******** Units are assumed to be inches ********/
/*
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
	float f = 3.0;

	//End Effector length
	//Measured with a ruler!
	float e = 3.0;
	
	//shin length
	//Measued with a ruler! Includes the thigh/shin and shin/effector jointy thingy length
	float re = 5.0;
	
	//thigh length
	//It's bent, so it's funky. Let's just say 4.
	float rf = 4.0;

	//Default thigh angles
	float t1 = 55.19;
	float t2 = 30.5;
	float t3 = 37.8;

	/******** Rendery goodness initialization ********/
	
	//Circumradius and inradius of FF equilateral triangle (Needed for rendering fixed frame)
	float ff_ccr = (1.0/3.0)*(sqrt(3.0))*f;
	float ff_ir = (1.0/6.0)*(sqrt(3.0))*f;

	GLUquadric* gluq = gluNewQuadric();
	
	/******** Silly extra variable initialization section ********/
	
	float dtr, tr, tr1, tr2, tr3, r3, ef, e1x, e1y, e1z, e2x, e2y, e2z, e3x, e3y, e3z;
	int i;

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

		// Lots of assumptions being made here.
		// First off, I have no idea what the actual encoder min/max for the falcon is.
		// However, in practice it seems to be around -/+1750. This puts us close to but
		// not exactly at 90 degrees from the center of the fixed frame on either side.
		// Therefore, the encoder values are buffered around 250 ticks in to account for
		// this.

		t1 = (((float)output_packet.motor[0] + 2000.0f)/4000.0f) * 90.0f;
		t2 = (((float)output_packet.motor[1] + 2000.0f)/4000.0f) * 90.0f;
		t3 = (((float)output_packet.motor[2] + 2000.0f)/4000.0f) * 90.0f;
	}

	if(nifalcon_test_fw_send_struct(&dev, &input_packet) < 0)
	{
		printf("Write error: %s\n", nifalcon_get_error_string(&dev));
		return;
	}
	if(nifalcon_test_fw_receive_struct(&dev, &output_packet, 0) < 0)
	{
		printf("Read error: %s\n", nifalcon_get_error_string(&dev));
		return;
	}

	/******** Camera/Viewpoint Setup ********/

	//Convert all input angles to radians since that's what the c math funcs eat

	dtr = 3.1415926/180.0;
	r3 = sqrt(3.0);
	
	tr1 = t1*dtr;
	tr2 = t2*dtr;
	tr3 = t3*dtr;
	tr = 2.0*r3;
	
	ef = (e-f)/tr;

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

	gluLookAt(cos(angle*(3.1415926/180.0))*view_z, 0, sin(angle*(3.1415926/180.0)) * view_z, 0, 0, 0, 0, 1, 0);

	//This was originally written to look like the drawings in the paper
	//To make it look like the falcon, just rotate everything 180 on the z.

	glRotatef(180.0f, 0,0,1);

	/******** Fixed Frame Rendering ********/

	glPushMatrix();
	//Fixed Frame Marker
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
	glBegin(GL_TRIANGLES);
		glVertex3f( -f*.5,  -ff_ir, 0.0f);
		glVertex3f(  f*.5,  -ff_ir, 0.0f);
		glVertex3f(   0.0f, ff_ccr, 0.0f);
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

	/******** Hip/Hip Restraint/Knee Calculation and Rendering ********/

	// All of our calculations here use rf (thigh length, also the radius of the straint sphere
	// of the thigh) as a scalar.
	// Pseudocode: 
	// for N[1:3]	
	// eNy = (rf*cos(trN)-ef)/2.0)
	// eNx = (eNy * sqrt(3))
	// eNz = -rf*(sin(tr1));
	
	//D1
	{
		glPushMatrix();	
		glColor4f(1.0f, 1.0f, 0.0f, 0.2f);
		//Hip Placement Sphere
		glTranslatef(0.0f, -ff_ir/2, 0.0f);
		gluSphere(gluq, 1, 100, 100);
		
		//Hip Restraint Sphere
		glColor4f(1.0f, 1.0f, 0.0f, 0.1f);
		gluSphere(gluq, rf, 100, 100);

		glPopMatrix();

		//No e1x because it's always 0. Yay physical constraints!
		e1y=ef-rf*cos(tr1);
		e1z=-rf*sin(tr1);
		
		glPushMatrix();
		glTranslatef(0, e1y, e1z);
		//Knee Placement Sphere
		glColor3f(1.0, 1.0, 1.0);
		gluSphere(gluq, 1, 100, 100);
		//Knee Restraint Sphere
		glColor4f(0.0, 0.0, 1.0, 0.1);
		gluSphere(gluq, re, 100, 100);
		glPopMatrix();
	}

	//D2 - See comments of D1
	if(1)
	{
		glPushMatrix();
		
		glRotatef(300.0f, 0.0f, 0.0f, 1.0f);
		glTranslatef(0.0f, ff_ir/2, 0.0f);
		glColor4f(1.0f, 0.0f, 1.0f, 0.6f);
		gluSphere(gluq, 1, 100, 100);
		glColor4f(1.0f, 0.0f, 1.0f, 0.1f);
		gluSphere(gluq, rf, 100, 100);

		glPopMatrix();
		e2y=(rf*cos(tr2)-ef)/2.0;
		e2x=e2y*r3;
		e2z=-rf*sin(tr2);

		glPushMatrix();
		glTranslatef(e2x, e2y, e2z);
		glColor3f(1.0, 1.0, 1.0);
		gluSphere(gluq, 1, 100, 100);
		glColor4f(1.0, 0.0, 0.0, 0.1);
		gluSphere(gluq, re, 100, 100);
		glPopMatrix();

	}

	//D3 - See comments of D1
	if(1)
	{
		glPushMatrix();
		
		glRotatef(60.0f, 0.0f, 0.0f, 1.0f);
		glTranslatef(0.0f, ff_ir/2, 0.0f);
		glColor4f(0.0f, 1.0f, 1.0f, 0.6f);
		gluSphere(gluq, 1, 100, 100);
		glColor4f(1.0f, 1.0f, 1.0f, 0.1f);
		gluSphere(gluq, rf, 100, 100);

		glPopMatrix();

		e3y=(rf*cos(tr3)-ef)/2.0;
		e3x=-e3y*r3;
		e3z=-rf*sin(tr3);
		glPushMatrix();
		glTranslatef(e3x, e3y, e3z);
		glColor3f(1.0, 1.0, 1.0);
		gluSphere(gluq, 1, 100, 100);
		glColor4f(0.0, 1.0, 0.0, 0.1);
		gluSphere(gluq, re, 100, 100);
		glPopMatrix();

	}

	//Sphere collision circle midpoints
	glPushMatrix();
	glTranslatef((e1x+e2x)/2,(e1y+e2y)/2,(e1z+e2z)/2);
	gluSphere(gluq, 0.5, 100, 100);
	glPopMatrix();
	//Sphere collision circle midpoints
	glPushMatrix();
	glTranslatef((e2x+e3x)/2,(e2y+e3y)/2,(e2z+e3z)/2);
	gluSphere(gluq, 0.5, 100, 100);
	glPopMatrix();
	//Sphere collision circle midpoints
	glPushMatrix();
	glTranslatef((e1x+e3x)/2,(e1y+e3y)/2,(e1z+e3z)/2);
	gluSphere(gluq, 0.5, 100, 100);
	glPopMatrix();


	//printf("e1 %f %f %f\ne2 %f %f %f\ne3 %f %f %f\n", e1x, e1y, e1z, e2x, e2y, e2z, e3x, e3y, e3z);

	/******** Shin/Effector Position Calculation and Rendering ********/

	//Basically, we're finding the two intersection points of the 3 constraint spheres
	//with centers at the knee positions and radii of the knee constraint length (re).
	//This is done by finding the intersection of two of the constraint spheres, then
	//testing the third against the resulting circle to find the 2 intersection points.
	//We then need to figure out which point is valid. This choice is made by the location
	//of knees 2 and 3, as can be seen in the code below.
	
	//How not to do flow control... :)
	while(1)
	{		
		float w1,x1,y1,z1,w2,x2,y2,z2,w3,x3,y3,z3, ar, b1, b2, d;
		float x,y,z,a,b,c,p01,p02,p03,p23,p31,p12,p2,xa,ya,za,xb,yb,zb;

		//Compute the homogenenous coordinates of the intersection plane
		//of the constraint spheres of knee e1 and e2, and e2 and e3
		//The original equation for this looks like
		// {w:x:y:z} =
		// { (rf^2 - re^2 + eNx^2 - e(N+1)x^2 + eNy^2 - e(N+1)y^2 + eNz^2 - e(N+1)z^2) / 2 : e(N+1)x - eNx : e(N+1)y - eNy : e(N+1)z - eNz }
		
		//Coords for e1/e2
		//w1 looks funky because e1x is 0, so a few things get shifted for readability
		w1=((e1y*e1y)-(e2x*e2x)-(e2y*e2y)+(e1z*e1z)-(e2z*e2z))/2.0;
		x1=e2x;
		y1=e2y-e1y;
		z1=e2z-e1z;

		
		w2=((e2x*e2x)-(e3x*e3x)+(e2y*e2y)-(e3y*e3y)+(e2z*e2z)-(e3z*e3z))/2.0;
		x2=e3x-e2x;
		y2=e3y-e2y;
		z2=e3z-e2z;


		glLineWidth(4.0f);
		glColor4f(1,1,1,1);
		glPushMatrix();
		glTranslatef((e1x+e2x)/2,(e1y+e2y)/2,(e1z+e2z)/2);
		glBegin(GL_LINES);
		glVertex3d(x1*-25, y1*-25, z1*-25);
		glVertex3d(x1*25, y1*25, z1*25);		
		glEnd();
		glPopMatrix();

		glLineWidth(4.0f);
		glColor4f(1,1,1,1);
		glPushMatrix();
		glTranslatef((e2x+e3x)/2,(e2y+e3y)/2,(e2z+e3z)/2);
		glBegin(GL_LINES);
		glVertex3d(x2*-25, y2*-25, z2*-25);
		glVertex3d(x2*25, y2*25, z2*25);		
		glEnd();
		glPopMatrix();

		w3=((e1y*e1y)-(e3x*e3x)-(e3y*e3y)+(e1z*e1z)-(e3z*e3z))/2.0;
		x3=e3x;
		y3=e3y-e1y;
		z3=e3z-e1z;

		glLineWidth(4.0f);
		glColor4f(1,1,1,1);
		glPushMatrix();
		glTranslatef((e1x+e3x)/2,(e1y+e3y)/2,(e1z+e3z)/2);
		glBegin(GL_LINES);
		glVertex3d(x3*-25, y3*-25, z3*-25);
		glVertex3d(x3*25, y3*25, z3*25);		
		glEnd();
		glPopMatrix();


//		printf("1 %f %f %f %f\n", w1, x1, y1, z1);
//		printf("2 %f %f %f %f\n", w2, x2, y2, z2);

		//Plucker coordinate derivation for intersection of the collision circles
		//Explanation at http://en.wikipedia.org/wiki/Line_geometry - Check the Dual Coordinates section

		p01=y1*z2-y2*z1;
		if(!p01)
		{
			printf("p01 == 0 : %f %f %f %f %f\n", y1, y2, z1, z2, sin(tr1));
			break;
		}
		p02=z1*x2-z2*x1;
		p03=x1*y2-x2*y1;
		
		p23=w1*x2-w2*x1;
		p31=w1*y2-w2*y1;
		p12=w1*z2-w2*z1;

//		printf("p %f %f %f %f\n", p01, p02, p03, p23, p31, p12);

		//Now that we've got the Plucker coords, start building the quadratic equation as
		//laid out in the paper		
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
			break;
		}		
		d=sqrt(ar);
		
//		printf("quad %f %f %f %f\n", a, b, c, d);

		//Solve for X, then use the univariate X to find y and z
		x=(b+d)/a;
		y=(p02*x-p12)/p01;
		z=(p03*x+p31)/p01;

		//Remember, a quadratic gives you back two solutions.
		xa=((-b+d)/a);
		ya=-((p02*xa-p12)/p01);
		za=-((p03*xa+p31)/p01);
		xa=-xa;

		//Plot the solutions.		
//		printf("SOLUTION 1: %f %f %f\n", x, y, z);
		glPushMatrix();
		glTranslatef(x, y, z);
		glColor3f(1.0, 1.0, 1.0);
		gluSphere(gluq, 1, 100, 100);
		glPopMatrix();

//		printf("SOLUTION 2: %f %f %f\n", x, y, z);
		glPushMatrix();
		glTranslatef(xa, ya, za);
		glColor3f(1.0, 1.0, 1.0);
		gluSphere(gluq, 1, 100, 100);
		glPopMatrix();

		break;
	}
	

	glutSwapBuffers();
	glutPostRedisplay();
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

int main(int argc, char** argv)
{

	int num_falcons, status, i;
	unsigned int count;
	unsigned char input[17] = "<AAAAAAAAAAAAAA>";
	unsigned char output[17];	


	float e = 8, f = 16, re = 10.3094, rf = 8, t1=55.19, t2=9.5, t3=37.8;
	float w1,x1,y1,z1,w2,x2,y2,z2,ar, b1, b2, d;
	float x,y,z,a,b,c,p01,p02,p03,p23,p31,p12,p2,tr1,tr2,tr3,tr,dtr,r3,ef,e1x,e1y,e1z,e2x,e2y,e2z,e3x,e3y,e3z;

	nifalcon_test_fw_init_packet(&input_packet);
	nifalcon_test_fw_init_packet(&output_packet);

	nifalcon_init(&dev);
	
	num_falcons = nifalcon_get_count(&dev);
	
	count = 0;
	input[16] = output[16] = 0;

	printf("Falcons found: %d\n", num_falcons);

	printf("Opening falcon\n");
	if((status = nifalcon_open(&dev, 0)) < 0)
	{
		printf ("Cannot open falcon! Error: %s\n", nifalcon_get_error_string(&dev));
		return 1;
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
		return 1;
	}
	printf("Firmware loaded\n");
	
	glutInit(&argc, argv);	
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB );
	glutInitWindowSize (WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	InitGL();
	glutKeyboardFunc    ( keyboard );
	glutDisplayFunc(DrawGLScene);
	glutReshapeFunc(ReSizeGLScene);
	glutMainLoop();

	/*
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
	return 0;	
}
