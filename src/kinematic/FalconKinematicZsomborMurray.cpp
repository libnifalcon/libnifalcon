/***
 * @file FalconKinematicZsomborMurray.cpp
 * @brief IK and DK implementation for the Novint Falcon based on paper by
 * @author Kevin Ouellet (kouellet@users.sourceforge.net) / Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.sourceforge.net/
 *
 */

#include "falcon/kinematic/FalconKinematicZsomborMurray.h"
#include "falcon/core/FalconGeometry.h"
#include <cmath>

namespace libnifalcon
{

	const static float DEGREES_TO_RADIANS = 3.141592654/180.0;
	const static float RADIANS_TO_DEGREES = 180.0/3.141592654;


	inline float clamp(float x, float a, float b)
	{
		return x < a ? a : (x > b ? b : x);
	}

	FalconKinematicZsomborMurray::FalconKinematicZsomborMurray(bool init_now)
	{
	}

	void FalconKinematicZsomborMurray::initialize()
	{
	}

	bool FalconKinematicZsomborMurray::getForces(const boost::array<double, 3> (&position), const boost::array<double, 3> (&cart_force), boost::array<int, 3> (&enc_force))
	{
		return true;
	}

	bool FalconKinematicZsomborMurray::getAngles(boost::array<double, 3> (&position), boost::array<double, 3> (&angles))
	{
		float e2 = FalconGeometry::c / 2.0, f2 = FalconGeometry::r / 2.0, e4 = FalconGeometry::c / 4.0, f4 = FalconGeometry::r / 4.0;
		float r3 = sqrt(3), rf2 = FalconGeometry::a * FalconGeometry::a;
		float ef = FalconGeometry::b*FalconGeometry::b - rf2;
		point3f c1, c2, c3, d1, d2, d3;
		float p01, p02, p03, p23, q01, q02, q03, q23, q31, r01, r02, r03, r23, r31;
		float t, u, a, b, c, d, s, v, w;
		int i;

		point3f knee[3]; /*!< Point in 3-space of the knee */
		point4f col[3]; /*!< Collision midpoints of the knee spheres */
		point3f solution[2]; /*!< Two possibly solutions for the 3 sphere collision problem */

		c1.x = position[0];
		c1.y = position[1] - (e2/r3);
		c1.z = position[2];

//	printf("c1 %f %f %f\n",  c1.x, c1.y, c1.z);

		c2.x = position[0] + e4;
		c2.y = position[1] + (e4/r3);
		c2.z = position[2];

//	printf("c2 %f %f %f\n",  c2.x, c2.y, c2.z);

		c3.x = position[0] - e4;
		c3.y = c2.y;
		c3.z = position[2];

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

		col[0].x = c1.x;
		col[0].y = c1.y - d1.y;
		col[0].z = c1.z;
		col[0].w = (ef - (c1.x * c1.x) + (d1.y * d1.y) - (c1.y * c1.y) - (c1.z * c1.z))/2.0;

		col[1].x = c2.x - d2.x;
		col[1].y = c2.y - d2.y;
		col[1].z = c2.z;
		col[1].w = (ef + (d2.x * d2.x) - (c2.x * c2.x) + (d2.y * d2.y) - (c2.y * c2.y) - (c2.z * c2.z))/2.0;

		col[2].x = c3.x - d3.x;
		col[2].y = c3.y - d3.y;
		col[2].z = c3.z;
		col[2].w = (ef + (d3.x * d3.x) - (c3.x * c3.x) + (d3.y * d3.y) - (c3.y * c3.y) - (c3.z * c3.z))/2.0;

		/*
		  for(i = 0; i < 3; ++i)
		  {
		  printf("collision%d %f %f %f %f\n", i, col[i].w, col[i].x, col[i].y, col[i].z);
		  }
		*/

		p02 = col[0].z;
		p03 = -col[0].y;
		p23 = col[0].w;

//	printf("p %f %f %f\n", p02, p03, p23);

		q01 = -r3 * col[1].z;
		q02 = -col[1].z;
		q03 =  col[1].y + r3 * col[1].x;
		q23 = -col[1].w;
		q31  = r3*col[1].w;

//	printf("q %f %f %f %f %f\n", q01, q02, q03, q23, q31);

		r01 = r3 * col[2].z;
		r02 = -col[2].z;
		r03 = col[2].y - r3 * col[2].x;
		r23 = -col[2].w;
		r31 = -r3 * col[2].w;

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
			return false;
		}
		else
		{
			d = sqrt(d);
			v = (b-d)/a;
			s = v/FalconGeometry::a;
			angles[0] = RADIANS_TO_DEGREES * atan(s/sqrt(1-(s*s)));
//		printf("v1  %f %f %f\n",  v, s, thigh_angle[0]);
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
			return false;
		}
		else
		{
			d = sqrt(d);
			v = (b+d)/a;
			s = v/FalconGeometry::a;
			angles[1] = RADIANS_TO_DEGREES * atan(s/sqrt(1-(s*s)));
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
			return false;
		}
		else
		{
			d = sqrt(d);
			v = (b-d)/a;
			s = v/FalconGeometry::a;
			angles[2] = RADIANS_TO_DEGREES * atan(s/sqrt(1-(s*s)));
		}

		return true;
	}

	bool FalconKinematicZsomborMurray::getPosition(boost::array<int, 3> (&encoders), boost::array<double,3> (&position))
	{
		float tr, tr1, tr2, tr3, p01, p02, p03, p12, p23, p31, p2, a, b, c, d, b1, b2, ar;
		float ef;
		float r3 = sqrt(3.0);
		int i;

		float angle1 = getTheta(encoders[0]);
		float angle2 = getTheta(encoders[1]);
		float angle3 = getTheta(encoders[2]);

		point3f knee[3]; /*!< Point in 3-space of the knee */
		point4f col[3]; /*!< Collision midpoints of the knee spheres */
		point3f solution[2]; /*!< Two possibly solutions for the 3 sphere collision problem */

		//Convert all input angles to radians since that's what the c math funcs eat
		tr1 = angle1 * DEGREES_TO_RADIANS;
		tr2 = angle2 * DEGREES_TO_RADIANS;
		tr3 = angle3 * DEGREES_TO_RADIANS;
		tr = 2.0*r3;

		ef = (FalconGeometry::c - FalconGeometry::r)/tr;

		/******** Hip/Hip Restraint/Knee Calculation and Rendering ********/

		// All of our calculations here use rf (thigh length, also the radius of the straint sphere
		// of the thigh) as a scalar.
		// Pseudocode:
		// for N[1:3]
		// eNy = (rf*cos(trN)-ef)/2.0)
		// eNx = (eNy * sqrt(3))
		// eNz = -rf*(sin(tr1));

		//No e1x because it's always 0. Yay physical constraints!
		knee[0].x= 0;
		knee[0].y= ef-FalconGeometry::a*cos(tr1);
		knee[0].z= -FalconGeometry::a*sin(tr1);

		knee[1].y=(FalconGeometry::a*cos(tr2)-ef)/2.0;
		knee[1].x=knee[1].y*r3;
		knee[1].z=-FalconGeometry::a*sin(tr2);

		knee[2].y=(FalconGeometry::a*cos(tr3)-ef)/2.0;
		knee[2].x=-knee[2].y*r3;
		knee[2].z=-FalconGeometry::a*sin(tr3);
		/*
		  for(i =0; i < 3; ++i)
		  {
		  printf("knee%d %f %f %f\n", i, knee[i].x, knee[i].y, knee[i].z);
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
		col[0].w=((knee[0].y*knee[0].y)-(knee[1].x*knee[1].x)-(knee[1].y*knee[1].y)+(knee[0].z*knee[0].z)-(knee[1].z*knee[1].z))/2.0;
		col[0].x=knee[1].x;
		col[0].y=knee[1].y-knee[0].y;
		col[0].z=knee[1].z-knee[0].z;

		col[1].w=((knee[1].x*knee[1].x)-(knee[2].x*knee[2].x)+(knee[1].y*knee[1].y)-(knee[2].y*knee[2].y)+(knee[1].z*knee[1].z)-(knee[2].z*knee[2].z))/2.0;
		col[1].x=knee[2].x-knee[1].x;
		col[1].y=knee[2].y-knee[1].y;
		col[1].z=knee[2].z-knee[1].z;
		/*
		  for(i =0; i < 2; ++i)
		  {
		  printf("col%d %f %f %f %f\n", i, col[i].w, col[i].x, col[i].y, col[i].z);
		  }
		*/
		//We don't actually need to do this calculation, it was just here for the OpenGL simulation visualization
		//It should probably be optimized out somehow
		col[2].w=((knee[0].y*knee[0].y)-(knee[2].x*knee[2].x)-(knee[2].y*knee[2].y)+(knee[0].z*knee[0].z)-(knee[2].z*knee[2].z))/2.0;
		col[2].x=knee[2].x;
		col[2].y=knee[2].y-knee[0].y;
		col[2].z=knee[2].z-knee[0].z;

		//Plucker coordinate derivation for intersection of the collision circles
		//Explanation at http://en.wikipedia.org/wiki/Line_geometry - Check the Dual Coordinates section

		p01 = col[0].y * col[1].z - col[1].y * col[0].z;
		if(!p01)
		{
			//		printf("p01 == 0\n");
			return false;
		}
		p02 = col[0].z * col[1].x - col[1].z * col[0].x;
		p03 = col[0].x * col[1].y - col[1].x * col[0].y;
		p23 = col[0].w * col[1].x - col[1].w * col[0].x;
		p31 = col[0].w * col[1].y - col[1].w * col[0].y;
		p12 = col[0].w * col[1].z - col[1].w * col[0].z;

		//		printf("p %f %f %f %f\n", p01, p02, p03, p23, p31, p12);

		//Now that we've got the Plucker coords, start building the quadratic equation as
		//laid out in the paper
		p2 = p01 * p01;

		a = p2 + (p02 * p02) + (p03 * p03);
		b1 = p12 + p01 * knee[0].y;
		b2 = p31- p01 * knee[0].z;
		b = p02 * b1 - p03 * b2;

		c = (b1 * b1) + (b2 * b2) - (p2 * (FalconGeometry::b * FalconGeometry::b));
		ar = (b * b) - (a * c);
		if(ar < 0)
		{
			//		printf("ar < 0\n");
			return false;
		}
		d = sqrt(ar);

		//		printf("quad %f %f %f %f\n", a, b, c, d);

		//Solve for X, then use the univariate X to find y and z
		solution[0].x = (b + d) / a;
		solution[0].y = (p02 * solution[0].x - p12) / p01;
		solution[0].z = (p03 * solution[0].x + p31) / p01;

		//Remember, a quadratic gives you back two solutions.
		//And really, once again, this doesn't need to be done if our initial z
		//is in a valid range (<= -3.5).
		solution[1].x = (-b + d) / a;
		solution[1].y = -(( p02 * solution[1].x - p12) / p01);
		solution[1].z = -(( p03 * solution[1].x + p31)  /  p01);
		solution[1].x = -solution[1].x;

		if(solution[0].z > 0)
		{
			position[0] = solution[0].x;
			position[1] = solution[0].y;
			position[2] = solution[0].z;
		}
		else
		{
			position[0] = solution[1].x;
			position[1] = solution[1].y;
			position[2] = solution[1].z;
		}
		return true;
	}
}
