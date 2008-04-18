#include "nifalcon_test_fw.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <GLUT/glut.h>

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500
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
		exit ( 0 );   // Exit The Program
		break;        // Ready For Next Case
    default:        // Now Wrap It Up
		break;
	}
}

float angle = 0.0;
	falcon_packet input_packet, output_packet;

GLvoid DrawGLScene(GLvoid)
{	

	//fixed foot length
	float f = 16.0;
	//End Effector length
	float e = 8.0;
	//shin length
	float re = 10.3094;
	//thigh length
	float rf = 8.0;
/*
	//fixed foot length
	float f = 3.0;
	//End Effector length
	float e = 3.0;
	//shin length
	float re = 4.0;
	//thigh length
	float rf = 4.0;
*/
	//Thigh angles
	float t1 = 55.19;
	float t2 = 30.5;
	float t3 = 37.8;
	
	//Circumradius and inradius of FF equilateral triangle
	float ff_ccr = (1.0/3.0)*(sqrt(3.0))*f;
	float ff_ir = (1.0/6.0)*(sqrt(3.0))*f;
	
	float dtr, tr, tr1, tr2, tr3, r3, ef, e1x, e1y, e1z, e2x, e2y, e2z, e3x, e3y, e3z;
	
	GLUquadric* gluq = gluNewQuadric();


	input_packet.info = NOVINT_TEST_FW_HOMING_MODE;
	if(!(output_packet.info & (NOVINT_TEST_FW_HOMED_AXIS1 | NOVINT_TEST_FW_HOMED_AXIS2 | NOVINT_TEST_FW_HOMED_AXIS3)))
	{
		input_packet.info |= NOVINT_TEST_FW_LED_RED;
//		return;
	}
	else
	{
		input_packet.info |= NOVINT_TEST_FW_LED_GREEN;
		t1 = (((float)output_packet.motor[0] + 2000.0f)/4000.0f) * 90.0f;
		t2 = (((float)output_packet.motor[1] + 2000.0f)/4000.0f) * 90.0f;
		t3 = (((float)output_packet.motor[2] + 2000.0f)/4000.0f) * 90.0f;
		printf("ANGLES %f %f %f\n", t1, t2, t3);
	}

	if(nifalcon_test_fw_send_struct(&dev, &input_packet) < 0)
	{
		printf("Write error: %s\n", nifalcon_get_error_string(&dev));
		return 1;
	}
	if(nifalcon_test_fw_receive_struct(&dev, &output_packet, PACKET_TIMEOUT) < 0)
	{
		printf("Read error: %s\n", nifalcon_get_error_string(&dev));
		return 1;
	}

	printf("%x %x %x %x\n", output_packet.motor[0], output_packet.motor[1], output_packet.motor[2], output_packet.info);

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
	gluLookAt(cos(angle*(3.1415926/180.0))*40.0f, 0, sin(angle*(3.1415926/180.0)) * 40.0f, 0, 0, 0, 0, 1, 0);
//	gluLookAt(0, 0, 60.0f, 0, 0, 0, 0, 1, 0);
	angle += 0.3;

	glRotatef(180.0f, 0,0,1);
	
	glPushMatrix();
	//Fixed Frame Marker
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
	glBegin(GL_TRIANGLES);
		glVertex3f( -f*.5,  -ff_ir, 0.0f);
		glVertex3f(  f*.5,  -ff_ir, 0.0f);
		glVertex3f(    0.0f, ff_ccr, 0.0f);
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
   
	//D1
	{
		glPushMatrix();	
		glColor4f(1.0f, 1.0f, 0.0f, 0.2f);
		//Hip 
		glTranslatef(0.0f, -ff_ir/2, 0.0f);
		gluSphere(gluq, 1, 100, 100);
		
		//Hip Restraint Sphere
		glColor4f(1.0f, 1.0f, 0.0f, 0.1f);
		gluSphere(gluq, rf, 100, 100);
		glColor4f(1.0f, 1.0f, 0.0f, 0.6f);
		gluSphere(gluq, 1, 100, 100);

		glPopMatrix();

		e1y=ef-rf*cos(tr1);
		e1z=-rf*sin(tr1);

		
		glPushMatrix();
		glTranslatef(0, e1y, e1z);
		glColor3f(1.0, 1.0, 1.0);
		gluSphere(gluq, 1, 100, 100);
		glColor4f(0.0, 0.0, 1.0, 0.1);
		gluSphere(gluq, re, 100, 100);
		glPopMatrix();
	}

	//D2
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

	//D3
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

	while(1)
	{		
		float w1,x1,y1,z1,w2,x2,y2,z2,ar, b1, b2, d;
		float x,y,z,a,b,c,p01,p02,p03,p23,p31,p12,p2;

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
			break;
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
			break;
		}
		
		d=sqrt(ar);
		x=(b+d)/a;
		y=(p02*x-p12)/p01;
		z=(p03*x+p31)/p01;
		printf("SOLUTION: %f %f %f\n", x, y, z);
		glTranslatef(x, y, z);
		glColor3f(1.0, 1.0, 1.0);
		gluSphere(gluq, 1, 100, 100);
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

	int num_falcons, status;
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
	if((status = nifalcon_load_firmware(&dev, "test_firmware.bin")) < 0)
	{
		printf("Firmware not loaded! Error: %d %s\n", dev.falcon_error_code, nifalcon_get_error_string(&dev));
		return 1;
	}
	printf("Firmware loaded\n");
	
	if(1)
	{	
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
	}

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
		
/*	
	

*/	
	return 0;	
}
