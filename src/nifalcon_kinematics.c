/*
 * Implementation file for NovInt Falcon Kinematics Algorithms
 *
 * Copyright (c) 2008 Kyle Machulis <kyle@nonpolynomial.com> http://www.nonpolynomial.com
 *
 * Sourceforge project @ http://www.sourceforge.net/projects/libnifalcon
 *
 * This library is covered by the MIT License, read LICENSE for details.
 *
 */

//Kinematics algorithms from "Descriptive Geometric Kinematic Analysis of Clavel's Delta Robot", P.J. Zsombor-Murray, McGill University
//Paper available at http://www.nonpolynomial.com/pdf/claveldelta.pdf
//BASIC Code available at http://libnifalcon.wiki.sourceforge.net/Kinematics

#include "nifalcon_kinematics.h"
#include "math.h"

#define DEGREES_TO_RADIANS 3.141592654/180.0
#define RADIANS_TO_DEGREES 180.0/3.141592654

void nifalcon_init_kinematics(falcon_kinematics* dk)
{
	/******** Parallel bot unit input section ********/
	/******** Units are assumed to be inches ********/

	//Units for the novint falcon

	//fixed frame edge length
	//I don't have a damn /clue/ what this really is
	//That's part of why I wrote this simulator, so I can just throw numbers until something seems right.
	//It's probably derivable from the measuring the closest distance between the fixed frame origin and
	//the innermost workspace point, but really, do I care?	
	dk->f = 3.0;

	//End Effector edge length
	//Measured with a ruler!
	dk->e = 3.0;
	
	//shin length
	//Measued with a ruler! Includes the thigh/shin and shin/effector jointy thingy length
	dk->re = 5.0;
	
	//thigh length
	//It's bent, so it's funky. Let's just say 4. It looks right in the OpenGL sim.
	dk->rf = 4.0;
}

int nifalcon_direct_kinematics_encoder(falcon_kinematics* dk, int encoder1, int encoder2, int encoder3)
{
	// Lots of assumptions being made here.
	// First off, I have no idea what the actual encoder min/max for the falcon is.
	// However, in practice it seems to be around -/+1750. This puts us close to but
	// not exactly at 90 degrees from the center of the fixed frame on either side.
	// Therefore, the encoder values are buffered around 250 ticks in to account for
	// this.
	
	dk->thigh_angle[0] = (((float)encoder1 + 2000.0f)/4000.0f) * 90.0f;
	dk->thigh_angle[1] = (((float)encoder2 + 2000.0f)/4000.0f) * 90.0f;
	dk->thigh_angle[2] = (((float)encoder3 + 2000.0f)/4000.0f) * 90.0f;

	return nifalcon_direct_kinematics_angle(dk, dk->thigh_angle[0], dk->thigh_angle[1], dk->thigh_angle[2]);
}

int nifalcon_direct_kinematics_angle(falcon_kinematics* dk, float angle1, float angle2, float angle3)
{
	float tr, tr1, tr2, tr3, p01, p02, p03, p12, p23, p31, p2, a, b, c, d, b1, b2, ar;
	float ef;
	float r3 = sqrt(3.0);
	int i;

	//Convert all input angles to radians since that's what the c math funcs eat
	tr1 = angle1 * DEGREES_TO_RADIANS;
	tr2 = angle2 * DEGREES_TO_RADIANS;
	tr3 = angle3 * DEGREES_TO_RADIANS;
	tr = 2.0*r3;
	
	ef = (dk->e-dk->f)/tr;

	/******** Hip/Hip Restraint/Knee Calculation and Rendering ********/

	// All of our calculations here use rf (thigh length, also the radius of the straint sphere
	// of the thigh) as a scalar.
	// Pseudocode: 
	// for N[1:3]	
	// eNy = (rf*cos(trN)-ef)/2.0)
	// eNx = (eNy * sqrt(3))
	// eNz = -rf*(sin(tr1));

	//No e1x because it's always 0. Yay physical constraints!
	dk->knee[0].x= 0;
	dk->knee[0].y= ef-dk->rf*cos(tr1);
	dk->knee[0].z= -dk->rf*sin(tr1);

	dk->knee[1].y=(dk->rf*cos(tr2)-ef)/2.0;
	dk->knee[1].x=dk->knee[1].y*r3;
	dk->knee[1].z=-dk->rf*sin(tr2);

	dk->knee[2].y=(dk->rf*cos(tr3)-ef)/2.0;
	dk->knee[2].x=-dk->knee[2].y*r3;
	dk->knee[2].z=-dk->rf*sin(tr3);
/*
	for(i =0; i < 3; ++i)
	{
		printf("knee%d %f %f %f\n", i, dk->knee[i].x, dk->knee[i].y, dk->knee[i].z);
	}
*/
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
	dk->col[0].w=((dk->knee[0].y*dk->knee[0].y)-(dk->knee[1].x*dk->knee[1].x)-(dk->knee[1].y*dk->knee[1].y)+(dk->knee[0].z*dk->knee[0].z)-(dk->knee[1].z*dk->knee[1].z))/2.0;
	dk->col[0].x=dk->knee[1].x;
	dk->col[0].y=dk->knee[1].y-dk->knee[0].y;
	dk->col[0].z=dk->knee[1].z-dk->knee[0].z;
	
	dk->col[1].w=((dk->knee[1].x*dk->knee[1].x)-(dk->knee[2].x*dk->knee[2].x)+(dk->knee[1].y*dk->knee[1].y)-(dk->knee[2].y*dk->knee[2].y)+(dk->knee[1].z*dk->knee[1].z)-(dk->knee[2].z*dk->knee[2].z))/2.0;
	dk->col[1].x=dk->knee[2].x-dk->knee[1].x;
	dk->col[1].y=dk->knee[2].y-dk->knee[1].y;
	dk->col[1].z=dk->knee[2].z-dk->knee[1].z;
/*
	for(i =0; i < 2; ++i)
	{
		printf("col%d %f %f %f %f\n", i, dk->col[i].w, dk->col[i].x, dk->col[i].y, dk->col[i].z);
	}
*/
	//We don't actually need to do this calculation, it was just here for the OpenGL simulation visualization
	//It should probably be optimized out somehow
	dk->col[2].w=((dk->knee[0].y*dk->knee[0].y)-(dk->knee[2].x*dk->knee[2].x)-(dk->knee[2].y*dk->knee[2].y)+(dk->knee[0].z*dk->knee[0].z)-(dk->knee[2].z*dk->knee[2].z))/2.0;
	dk->col[2].x=dk->knee[2].x;
	dk->col[2].y=dk->knee[2].y-dk->knee[0].y;
	dk->col[2].z=dk->knee[2].z-dk->knee[0].z;

	//Plucker coordinate derivation for intersection of the collision circles
	//Explanation at http://en.wikipedia.org/wiki/Line_geometry - Check the Dual Coordinates section
	
	p01 = dk->col[0].y * dk->col[1].z - dk->col[1].y * dk->col[0].z;
	if(!p01)
	{
//		printf("p01 == 0\n");
		return -1;
	}
	p02 = dk->col[0].z * dk->col[1].x - dk->col[1].z * dk->col[0].x;
	p03 = dk->col[0].x * dk->col[1].y - dk->col[1].x * dk->col[0].y;	
	p23 = dk->col[0].w * dk->col[1].x - dk->col[1].w * dk->col[0].x;
	p31 = dk->col[0].w * dk->col[1].y - dk->col[1].w * dk->col[0].y;
	p12 = dk->col[0].w * dk->col[1].z - dk->col[1].w * dk->col[0].z;
	
//		printf("p %f %f %f %f\n", p01, p02, p03, p23, p31, p12);

	//Now that we've got the Plucker coords, start building the quadratic equation as
	//laid out in the paper		
	p2 = p01 * p01;
	
	a = p2 + (p02 * p02) + (p03 * p03);
	b1 = p12 + p01 * dk->knee[0].y;
	b2 = p31- p01 * dk->knee[0].z;
	b = p02 * b1 - p03 * b2;
	
	c = (b1 * b1) + (b2 * b2) - (p2 * (dk->re * dk->re));
	ar = (b * b) - (a * c);
	if(ar < 0) 
	{
//		printf("ar < 0\n");
		return -1;
	}		
	d = sqrt(ar);
	
//		printf("quad %f %f %f %f\n", a, b, c, d);

	//Solve for X, then use the univariate X to find y and z
	dk->solution[0].x = (b + d) / a;
	dk->solution[0].y = (p02 * dk->solution[0].x - p12) / p01;
	dk->solution[0].z = (p03 * dk->solution[0].x + p31) / p01;

	//Remember, a quadratic gives you back two solutions.
	//And really, once again, this doesn't need to be done if our initial z
	//is in a valid range (<= -3.5). 
	dk->solution[1].x = (-b + d) / a;
	dk->solution[1].y = -(( p02 * dk->solution[1].x - p12) / p01);
	dk->solution[1].z = -(( p03 * dk->solution[1].x + p31)  /  p01);
	dk->solution[1].x = -dk->solution[1].x;
	return 0;
}

int nifalcon_inverse_kinematics_encoder(falcon_kinematics* dk, float effector_x, float effector_y, float effector_z)
{
	nifalcon_inverse_kinematics_angle(dk, effector_x, effector_y, effector_z);

	dk->thigh_encoder[0] = ((dk->thigh_angle[0] / 90.0f) * 4000.0f) - 2000.0f;
	dk->thigh_encoder[0] = ((dk->thigh_angle[1] / 90.0f) * 4000.0f) - 2000.0f;
	dk->thigh_encoder[0] = ((dk->thigh_angle[2] / 90.0f) * 4000.0f) - 2000.0f;	
}

int nifalcon_inverse_kinematics_angle(falcon_kinematics* dk, float effector_x, float effector_y, float effector_z)
{
	float e2 = dk->e / 2.0, f2 = dk->f / 2.0, e4 = dk->e / 4.0, f4 = dk->f / 4.0;
	float r3 = sqrt(3), rf2 = dk->rf * dk->rf;
	float ef = dk->re*dk->re - rf2;
	point3f c1, c2, c3, d1, d2, d3;
	float p01, p02, p03, p23, q01, q02, q03, q23, q31, r01, r02, r03, r23, r31;
	float t, u, a, b, c, d, s, v, w;
	int i;
	
	c1.x = effector_x;
	c1.y = effector_y - (e2/r3);
	c1.z = effector_z;

//	printf("c1 %f %f %f\n",  c1.x, c1.y, c1.z);
	
	c2.x = effector_x + e4;
	c2.y = effector_y + (e4/r3);
	c2.z = effector_z;

//	printf("c2 %f %f %f\n",  c2.x, c2.y, c2.z);

	c3.x = effector_x - e4;
	c3.y = c2.y;
	c3.z = effector_z;

//	printf("c3 %f %f %f\n",  c3.x, c3.y, c3.z);

	d1.y = -f2 / r3;

	d2.x = f4;
	d2.y = f4 / r3;

	d3.x = -f4;
	d3.y = d2.y;

	/*
	for(i = 0; i < 3; ++i)
	{
		printf("d %f %f %f %f %f\n", d1.y, d2.x, d2.y, d3.x, d3.y);
	}
	*/
	
	dk->col[0].x = c1.x;
	dk->col[0].y = c1.y - d1.y;
	dk->col[0].z = c1.z;
	dk->col[0].w = (ef - (c1.x * c1.x) + (d1.y * d1.y) - (c1.y * c1.y) - (c1.z * c1.z))/2.0;
	
	dk->col[1].x = c2.x - d2.x;
	dk->col[1].y = c2.y - d2.y;
	dk->col[1].z = c2.z;
	dk->col[1].w = (ef + (d2.x * d2.x) - (c2.x * c2.x) + (d2.y * d2.y) - (c2.y * c2.y) - (c2.z * c2.z))/2.0;

	dk->col[2].x = c3.x - d3.x;
	dk->col[2].y = c3.y - d3.y;
	dk->col[2].z = c3.z;
	dk->col[2].w = (ef + (d3.x * d3.x) - (c3.x * c3.x) + (d3.y * d3.y) - (c3.y * c3.y) - (c3.z * c3.z))/2.0;

	/*
	for(i = 0; i < 3; ++i)
	{
		printf("collision%d %f %f %f %f\n", i, dk->col[i].w, dk->col[i].x, dk->col[i].y, dk->col[i].z);
	}
	*/
	
	p02 = dk->col[0].z;
	p03 = -dk->col[0].y;
	p23 = dk->col[0].w;

//	printf("p %f %f %f\n", p02, p03, p23);
	
	q01 = -r3 * dk->col[1].z;
	q02 = -dk->col[1].z;
	q03 =  dk->col[1].y + r3 * dk->col[1].x;
	q23 = -dk->col[1].w;
	q31  = r3*dk->col[1].w;

//	printf("q %f %f %f %f %f\n", q01, q02, q03, q23, q31);
	
	r01 = r3 * dk->col[2].z;
	r02 = -dk->col[2].z;
	r03 = dk->col[2].y - r3 * dk->col[2].x;
	r23 = -dk->col[2].w;
	r31 = -r3 * dk->col[2].w;

//	printf("r %f %f %f %f %f\n", r01, r02, r03, r23, r31);
	
	t = p02 / p03;
	u = p23 / p03;
	a = (t * t) + 1;
	b = t * (d1.y - u);
	c = u * (2 * d1.y - u) - (d1.y * d1.y) + rf2;
	d = (b * b) + (a * c);
	if(d < 0)
	{
//		printf("d1 < 0!\n");
		return -1;
	}
	else
	{
		d = sqrt(d);
		v = (b-d)/a;
		s = v/dk->rf;
		dk->thigh_angle[0] = RADIANS_TO_DEGREES * atan(s/sqrt(1-(s*s)));
//		printf("v1  %f %f %f\n",  v, s, dk->thigh_angle[0]);
	}
	

	t = q02 / q03;
	u = q01 / q03;
	v = q31 / q03;
	w = q23 / q03;
	a = (t * t) + (u * u) + 1;
	b = u * (d2.x + v) + t * (d2.y - w);
	c = rf2 - d2.x * (d2.x + 2 * v) - d2.y * (d2.y - 2 * w) - (v * v) - (w * w);	
	d = b*b+a*c;
	if(d < 0)
	{
//		printf("d2 < 0!\n");
		return -1;
	}
	else
	{
		d = sqrt(d);
		v = (b+d)/a;
		s = v/dk->rf;
		dk->thigh_angle[1] = RADIANS_TO_DEGREES * atan(s/sqrt(1-(s*s)));
	}

	t = r02 / r03;
	u = r01 / r03;
	v = r31 / r03;
	w = r23 / r03;
	a = (t * t) + (u * u) + 1;
	b = u * (d3.x + v) + t * (d3.y - w);
	c = rf2 - d3.x * (d3.x + 2 * v) - d3.y * (d3.y - 2 * w) - (v * v) - (w * w);	
	d = b*b+a*c;
	if(d < 0)
	{
//		printf("d3 < 0!\n");
		return -1;
	}
	else
	{
		d = sqrt(d);
		v = (b-d)/a;
		s = v/dk->rf;
		dk->thigh_angle[2] = RADIANS_TO_DEGREES * atan(s/sqrt(1-(s*s)));
	}
	
	return 0;
}
