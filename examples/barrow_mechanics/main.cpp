//////////////////////////////////////////////////////////
// Novint Falcon Kinematics/Dynamics based on R.E.Stamper's PhD(1997)
// with some modifications
//
// Using LibniFalcon Beta 4
//
// Alastair Barrow 26/08/09


#include <iostream>
#include <string>
#include <cmath>

#include "falcon/core/FalconDevice.h"
#include "falcon/firmware/FalconFirmwareNovintSDK.h"
#include "falcon/util/FalconCLIBase.h"
#include "falcon/util/FalconFirmwareBinaryNvent.h"
#include "falcon/kinematic/stamper/StamperUtils.h"
#include "falcon/core/FalconGeometry.h"
#include "falcon/gmtl/gmtl.h"

using namespace libnifalcon;
using namespace std;
using namespace StamperKinematicImpl;

FalconDevice falcon;

//////////////////////////////////////////////////////////
/// Ask libnifalcon to get the Falcon ready for action
/// nothing clever here, straight from the examples
bool initialise()
{
	falcon.setFalconFirmware<FalconFirmwareNovintSDK>();

	cout << "Setting up comm interface for Falcon comms" << endl;

	unsigned int count;
	falcon.getDeviceCount(count);
	cout << "Connected Device Count: " << count << endl;

	//Open the device number:
	int deviceNum = 0;
	cout << "Attempting to open Falcon device:  " << deviceNum << endl;
	if(!falcon.open(deviceNum))
	{
		cout << "Cannot open falcon device index " << deviceNum << " - Lib Error Code: " << falcon.getErrorCode() << " Device Error Code: " << falcon.getFalconComm()->getDeviceErrorCode() << endl;
		return false;
	}
	else
	{
		cout << "Connected to Falcon device " << deviceNum << endl ;
	}

	//Load the device firmware:
	//There's only one kind of firmware right now, so automatically set that.
	falcon.setFalconFirmware<FalconFirmwareNovintSDK>();
	//Next load the firmware to the device
	
	bool skip_checksum = false;
	//See if we have firmware
	bool firmware_loaded = false;
	firmware_loaded = falcon.isFirmwareLoaded();
	if(!firmware_loaded)
	{
		std::cout << "Loading firmware" << std::endl;
		uint8_t* firmware_block;
		long firmware_size;
		{

			firmware_block = const_cast<uint8_t*>(NOVINT_FALCON_NVENT_FIRMWARE);
			firmware_size = NOVINT_FALCON_NVENT_FIRMWARE_SIZE;


			for(int i = 0; i < 10; ++i)
			{
				if(!falcon.getFalconFirmware()->loadFirmware(skip_checksum, NOVINT_FALCON_NVENT_FIRMWARE_SIZE, const_cast<uint8_t*>(NOVINT_FALCON_NVENT_FIRMWARE)))

				{
					cout << "Firmware loading try failed";
					//Completely close and reopen
					//falcon.close();
					//if(!falcon.open(m_varMap["device_index"].as<int>()))
					//{
					//	std::cout << "Cannot open falcon device index " << m_varMap["device_index"].as<int>() << " - Lib Error Code: " << m_falconDevice->getErrorCode() << " Device Error Code: " << m_falconDevice->getFalconComm()->getDeviceErrorCode() << std::endl;
					//	return false;
					//}
				}
				else
				{
					firmware_loaded = true;
					break;
				}
			}
		}
	}
	else if(!firmware_loaded)
	{
		std::cout << "No firmware loaded to device, and no firmware specified to load (--nvent_firmware, --test_firmware, etc...). Cannot continue" << std::endl;
		//return false;
	}
	else
	{
		//return true;
	}
	if(!firmware_loaded || !falcon.isFirmwareLoaded())
	{
		std::cout << "No firmware loaded to device, cannot continue" << std::endl;
		//return false;
	}
	std::cout << "Firmware loaded" << std::endl;

	//Seems to be important to run the io loop once to be sure of sensible values next time:
	falcon.runIOLoop();

	falcon.getFalconFirmware()->setHomingMode(true);
	//falcon.setFalconKinematic<libnifalcon::FalconKinematicStamper>();

	return true;
}

//////////////////////////////////////////////////////////
//Inverse kinematics. All as in Stamper's PhD except for
//the addition of a second offset direction 's' per arm
void IK(Angle& angles, const gmtl::Vec3d& worldPosition)
{

	//First we need the offset vector from the origin of the XYZ coordinate frame to the
	//UVW coordinate frame:
	gmtl::Vec3d offset(-libnifalcon::r,-libnifalcon::s,0);
	

	//Next lets convert the current end effector position into the UVW coordinates
	//of each leg:
	gmtl::Matrix33d R;
	R(0,0)=cos(libnifalcon::phy[0]);	R(0,1)=sin(libnifalcon::phy[0]);	R(0,2)=0;
	R(1,0)=-sin(libnifalcon::phy[0]);	R(1,1)=cos(libnifalcon::phy[0]);	R(1,2)=0;
	R(2,0)=0;							R(2,1)=0;							R(2,2)=1;
	gmtl::Vec3d P1 = R*worldPosition + offset;

	R(0,0)=cos(libnifalcon::phy[1]);	R(0,1)=sin(libnifalcon::phy[1]);	R(0,2)=0;
	R(1,0)=-sin(libnifalcon::phy[1]);	R(1,1)=cos(libnifalcon::phy[1]);	R(1,2)=0;
	R(2,0)=0;							R(2,1)=0;							R(2,2)=1;
	gmtl::Vec3d P2 = R*worldPosition + offset;

	R(0,0)=cos(libnifalcon::phy[2]);	R(0,1)=sin(libnifalcon::phy[2]);	R(0,2)=0;
	R(1,0)=-sin(libnifalcon::phy[2]);	R(1,1)=cos(libnifalcon::phy[2]);	R(1,2)=0;
	R(2,0)=0;							R(2,1)=0;							R(2,2)=1;
	gmtl::Vec3d P3 = R*worldPosition + offset;


	//Do the theta3's first. This is +/- but fortunately in the Falcon's case
	//only the + result is correct
	angles.theta3[0] = acos( (P1[1]+libnifalcon::f)/libnifalcon::b);
	angles.theta3[1] = acos( (P2[1]+libnifalcon::f)/libnifalcon::b);
	angles.theta3[2] = acos( (P3[1]+libnifalcon::f)/libnifalcon::b);


	//Next find the theta1's
	//In certain cases could query the theta1 values directly and save a bit of processing
	//Again we have a +/- situation but only + is relevent
	double l01 = P1[2]*P1[2] + P1[0]*P1[0] + 2*libnifalcon::c*P1[0] - 2*libnifalcon::a*P1[0] + libnifalcon::a*libnifalcon::a + libnifalcon::c*libnifalcon::c - libnifalcon::d*libnifalcon::d - libnifalcon::e*libnifalcon::e - libnifalcon::b*libnifalcon::b*sin(angles.theta3[0])*sin(angles.theta3[0]) - 2*libnifalcon::b*libnifalcon::e*sin(angles.theta3[0]) - 2*libnifalcon::b*libnifalcon::d*sin(angles.theta3[0]) - 2*libnifalcon::d*libnifalcon::e - 2*libnifalcon::a*libnifalcon::c;
	double l11 = -4*libnifalcon::a*P1[2];
	double l21 = P1[2]*P1[2] + P1[0]*P1[0] + 2*libnifalcon::c*P1[0] + 2*libnifalcon::a*P1[0] + libnifalcon::a*libnifalcon::a + libnifalcon::c*libnifalcon::c - libnifalcon::d*libnifalcon::d - libnifalcon::e*libnifalcon::e - libnifalcon::b*libnifalcon::b*sin(angles.theta3[0])*sin(angles.theta3[0]) - 2*libnifalcon::b*libnifalcon::e*sin(angles.theta3[0]) - 2*libnifalcon::b*libnifalcon::d*sin(angles.theta3[0]) - 2*libnifalcon::d*libnifalcon::e + 2*libnifalcon::a*libnifalcon::c;

	double l02 = P2[2]*P2[2] + P2[0]*P2[0] + 2*libnifalcon::c*P2[0] - 2*libnifalcon::a*P2[0] + libnifalcon::a*libnifalcon::a + libnifalcon::c*libnifalcon::c - libnifalcon::d*libnifalcon::d - libnifalcon::e*libnifalcon::e - libnifalcon::b*libnifalcon::b*sin(angles.theta3[1])*sin(angles.theta3[1]) - 2*libnifalcon::b*libnifalcon::e*sin(angles.theta3[1]) - 2*libnifalcon::b*libnifalcon::d*sin(angles.theta3[1]) - 2*libnifalcon::d*libnifalcon::e - 2*libnifalcon::a*libnifalcon::c;
	double l12 = -4*libnifalcon::a*P2[2];
	double l22 = P2[2]*P2[2] + P2[0]*P2[0] + 2*libnifalcon::c*P2[0] + 2*libnifalcon::a*P2[0] + libnifalcon::a*libnifalcon::a + libnifalcon::c*libnifalcon::c - libnifalcon::d*libnifalcon::d - libnifalcon::e*libnifalcon::e - libnifalcon::b*libnifalcon::b*sin(angles.theta3[1])*sin(angles.theta3[1]) - 2*libnifalcon::b*libnifalcon::e*sin(angles.theta3[1]) - 2*libnifalcon::b*libnifalcon::d*sin(angles.theta3[1]) - 2*libnifalcon::d*libnifalcon::e + 2*libnifalcon::a*libnifalcon::c;
	
	double l03 = P3[2]*P3[2] + P3[0]*P3[0] + 2*libnifalcon::c*P3[0] - 2*libnifalcon::a*P3[0] + libnifalcon::a*libnifalcon::a + libnifalcon::c*libnifalcon::c - libnifalcon::d*libnifalcon::d - libnifalcon::e*libnifalcon::e - libnifalcon::b*libnifalcon::b*sin(angles.theta3[2])*sin(angles.theta3[2]) - 2*libnifalcon::b*libnifalcon::e*sin(angles.theta3[2]) - 2*libnifalcon::b*libnifalcon::d*sin(angles.theta3[2]) - 2*libnifalcon::d*libnifalcon::e - 2*libnifalcon::a*libnifalcon::c;
	double l13 = -4*libnifalcon::a*P3[2];
	double l23 = P3[2]*P3[2] + P3[0]*P3[0] + 2*libnifalcon::c*P3[0] + 2*libnifalcon::a*P3[0] + libnifalcon::a*libnifalcon::a + libnifalcon::c*libnifalcon::c - libnifalcon::d*libnifalcon::d - libnifalcon::e*libnifalcon::e - libnifalcon::b*libnifalcon::b*sin(angles.theta3[2])*sin(angles.theta3[2]) - 2*libnifalcon::b*libnifalcon::e*sin(angles.theta3[2]) - 2*libnifalcon::b*libnifalcon::d*sin(angles.theta3[2]) - 2*libnifalcon::d*libnifalcon::e + 2*libnifalcon::a*libnifalcon::c;
	

	double T1a = (-l11 + sqrt( l11*l11 - 4* l01* l21) ) / (2*l21);
	double T2a = (-l12 + sqrt( l12*l12 - 4* l02* l22) ) / (2*l22);
	double T3a = (-l13 + sqrt( l13*l13 - 4* l03* l23) ) / (2*l23);

	double T1b = (-l11 - sqrt( l11*l11 - 4* l01* l21) ) / (2*l21);
	double T2b = (-l12 - sqrt( l12*l12 - 4* l02* l22) ) / (2*l22);
	double T3b = (-l13 - sqrt( l13*l13 - 4* l03* l23) ) / (2*l23);

	angles.theta1[0] = atan(T1b)*2;
	angles.theta1[1] = atan(T2b)*2;
	angles.theta1[2] = atan(T3b)*2;
	

	//And finally calculate the theta2 values:
	angles.theta2[0] = acos( (-P1[0] + libnifalcon::a*cos(angles.theta1[0]) - libnifalcon::c)/(-libnifalcon::d - libnifalcon::e - libnifalcon::b*sin(angles.theta3[0]) )  );
	angles.theta2[1] = acos( (-P2[0] + libnifalcon::a*cos(angles.theta1[1]) - libnifalcon::c)/(-libnifalcon::d - libnifalcon::e - libnifalcon::b*sin(angles.theta3[1]) )  );
	angles.theta2[2] = acos( (-P3[0] + libnifalcon::a*cos(angles.theta1[2]) - libnifalcon::c)/(-libnifalcon::d - libnifalcon::e - libnifalcon::b*sin(angles.theta3[2]) )  );
}

////////////////////////////////////////////////////
/// The velocity Jacobian where Vel=J*theta and Torque=J'*Force
/// Derivation in a slightly different style to Stamper
/// and may result in a couple of sign changes due to the configuration
/// of the Falcon
gmtl::Matrix33d jacobian(const Angle& angles)
{

	//Naming scheme:
	//Jx1 = rotational velocity of joint 1 due to linear velocity in x

	gmtl::Matrix33d J;
	
	//Arm1:
	double den = -libnifalcon::a*sin(angles.theta3[0])*(sin(angles.theta1[0])*cos(angles.theta2[0])-sin(angles.theta2[0])*cos(angles.theta1[0]));

	double Jx0 = cos(phy[0])*cos(angles.theta2[0])*sin(angles.theta3[0])/den-sin(phy[0])*cos(angles.theta3[0])/den;
	double Jy0 = sin(phy[0])*cos(angles.theta2[0])*sin(angles.theta3[0])/den+cos(phy[0])*cos(angles.theta3[0])/den;
	double Jz0 = (sin(angles.theta2[0])* sin(angles.theta2[0]))/(den);

	//Arm2:
	den = -libnifalcon::a*sin(angles.theta3[1])*(sin(angles.theta1[1])*cos(angles.theta2[1])-sin(angles.theta2[1])*cos(angles.theta1[1]));

	double Jx1 = cos(phy[1])*cos(angles.theta2[1])*sin(angles.theta3[1])/den-sin(phy[1])*cos(angles.theta3[1])/den;
	double Jy1 = sin(phy[1])*cos(angles.theta2[1])*sin(angles.theta3[1])/den+cos(phy[1])*cos(angles.theta3[1])/den;
	double Jz1 = (sin(angles.theta2[1])* sin(angles.theta2[1]))/(den);

	//Arm3:
	den = -libnifalcon::a*sin(angles.theta3[2])*(sin(angles.theta1[2])*cos(angles.theta2[2])-sin(angles.theta2[2])*cos(angles.theta1[2]));

	double Jx2 = cos(phy[2])*cos(angles.theta2[2])*sin(angles.theta3[2])/den-sin(phy[2])*cos(angles.theta3[2])/den;
	double Jy2 = sin(phy[2])*cos(angles.theta2[2])*sin(angles.theta3[2])/den+cos(phy[2])*cos(angles.theta3[2])/den;
	double Jz2 = (sin(angles.theta2[2])* sin(angles.theta2[2]))/(den);

	
	J(0,0) = Jx0; J(0,1) = Jy0; J(0,2) = Jz0;
	J(1,0) = Jx1; J(1,1) = Jy1; J(1,2) = Jz1;
	J(2,0) = Jx2; J(2,1) = Jy2; J(2,2) = Jz2;

	J.setState(J.FULL);
	invert(J);

	//ToDo: Check to see if Jacobian inverted properly.
	//If not we need to take action.

	return J;

}

//////////////////////////////////////////////////////////
/// Forward kinematics. Standard Newton-Raphson for linear
/// systems using Jacobian to estimate slope. A small amount 
/// of adjustment in the step size is all that is requried 
/// to guarentee convergence
void FK(const gmtl::Vec3d& theta0, gmtl::Vec3d& pos)
{

	Angle angles;
	gmtl::Vec3d previousPos(pos);
	gmtl::Vec3d currentPos(pos);
	gmtl::Matrix33d J;
	gmtl::Vec3d delta;

	double targetError = 0.01;
	double previousError = 10000.0;
	double gradientAdjustment = 0.5;
	int maxTries = 15;

	bool done = 0;
	for(int i=0; i<maxTries; i++)
	{

		//All we have initially are the three values for Theta0 and a guess of position

		//We can use the position guess to generate the angles at this position:
		IK(angles, previousPos);
		//And these angles to find the Jacobian at the current position:
		J = jacobian(angles);
		//Then we can use the Jacobian to tell us which direction we need to move
		//in to rotate each theta0 to towards our desired values

		//Then we can see the difference between the actual and guess theta0:
		delta[0] = theta0[0]-angles.theta1[0];
		delta[1] = theta0[1]-angles.theta1[1];
		delta[2] = theta0[2]-angles.theta1[2];

		//Now use the Jacobian to tell us the direction:
		delta = J*delta;
		
		//And now we move along the adjustment vector
		//Nb: A good gradient descent algorithm would use more 
		//intelligent step size adjustment. Here it only seems 
		//to take a couple of steps to converge normally so we 
		//simply start with a sensible step size and reduce it 
		//if necessary to avoid oscillation about the target error.

		//Take the step size into account:
		delta*=gradientAdjustment;
		//And move the position guess:
		currentPos = previousPos + delta;
			
		//Let's see if we have got close enough to the target:
		//double error = sqrt(gmtl::dot(delta,delta));
		delta[0] = theta0[0]-angles.theta1[0];
		delta[1] = theta0[1]-angles.theta1[1];
		delta[2] = theta0[2]-angles.theta1[2];
		double error = dot(delta,delta);
		error = sqrt( error );
		previousPos = currentPos;

		if(error<targetError)
		{
			//Error is low enough so return the current position estimate
			pos = previousPos;
			//cout << i << endl;
			return;
		}
		//Error isn't small enough yet, see if we have over shot 
		if( (error>previousError) )
		{
			//Whoops, over shot, reduce the stepsize next time:
			gradientAdjustment /= 2.0;
		}
		
		previousError = error;

	}

	//Failed to converge, leave last position as it was
	cout << "Failed to find the tool position in the max tries" << endl;

}


int main(int argc, char* argv[])
{

	if(!initialise())
		return 0;

	gmtl::Vec3d pos(0.0,0.0,0.11); //Lets assume the device starts off roughly in the centre of the workspace


	while(1)
	{
		//Ask libnifalcon to update the encoder positions and apply any forces waiting:
		falcon.runIOLoop();

		
		//////////////////////////////////////////////
		//Request the current encoder positions:
		std::array<int, 3> encoderPos;
		encoderPos = falcon.getFalconFirmware()->getEncoderValues();
		gmtl::Vec3d encoderAngles;
		encoderAngles[0] = falcon.getFalconKinematic()->getTheta(encoderPos[0]);
		encoderAngles[1] = falcon.getFalconKinematic()->getTheta(encoderPos[1]);
		encoderAngles[2] = falcon.getFalconKinematic()->getTheta(encoderPos[2]);
		encoderAngles *= 0.0174532925;	//Convert to radians

		

		////////////////////////////////////
		//Forward Kinematics
		FK(encoderAngles, pos);

		//cout << pos << endl;	

		/////////////////////////////////////////
		//Inverse kinematics:
		Angle angles;

		IK(angles, pos);

		////////////////////////////////////////
		//Jacobian:
		gmtl::Matrix33d J;
		J = jacobian(angles);

		//////////////////////////////////////////////
		//


		//Offset Z so position origin is roughly in the centre of the workspace:
		gmtl::Vec3d offsetPos(pos);
		offsetPos[2] -= 0.11;

		//Reset the forces:
		gmtl::Vec3d force(0.0,0.0,0.0);


		//Test shapes:
		
		//X-Wall
		/*
		if(offsetPos[0]>0.0)
		{
			force[0] = -offsetPos[0]*800;
		}
		*/

		//Y-Wall
		/*
		if(offsetPos[1]<0.0)
		{
			force[1] = -offsetPos[1]*800;
		}
		*/

		//Z-Wall
		/*
		if(offsetPos[2]<0.0)
		{
			force[2] = -offsetPos[2]*800;
		}
		*/

		//Room
		/*
		if( offsetPos[0]<-0.03 )
			force[0] = -(offsetPos[0]+0.03);
		if( offsetPos[0]>0.03 )
			force[0] = -(offsetPos[0]-0.03);
		if( offsetPos[1]<-0.03 )
			force[1] = -(offsetPos[1]+0.03);
		if( offsetPos[1]>0.03 )
			force[1] = -(offsetPos[1]-0.03);
		if( offsetPos[2]<-0.02 )
			force[2] = -(offsetPos[2]+0.02);
		if( offsetPos[2]>0.02 )
			force[2] = -(offsetPos[2]-0.02);
			force *= 800.0;
		*/

		//Sphere

		double distance = sqrt(gmtl::dot(offsetPos,offsetPos));
		if( distance<0.028  )
		{
			gmtl::Vec3d direction = offsetPos;
			direction /= distance;
			direction *= 0.028-distance;
			force = direction*800.0;
		}


		//Slope
/*		
		gmtl::Vec3d normal(0.70711,0.70711,0.0);
		double D = dot(normal,offsetPos);
		
		if(D<0.0)
		{
			force = -D*normal*800.0;
		}
*/		
		
		
		
		////////////////////////////////////////////
		//Dynamics

		//Convert force to motor torque values:
		J.setTranspose(J.getData());
		gmtl::Vec3d torque = J * force;


		//Now, we must scale the torques to avoid saturation of a motor
		//changing the ratio of torques and thus the force direction

		
		//Find highest torque:
		double maxTorque=30.0;	//Rather random choice here, could be higher
		double largestTorqueValue=0.0;
		int largestTorqueAxis=-1;
		for(int i=0; i<3; i++)
		{
			if(abs(torque[i])>largestTorqueValue)
			{
				largestTorqueValue=abs(torque[i]);
				largestTorqueAxis=i;
			}
		}
		//If axis with the largest torque is over the limit, scale them all to
		//bring it back to the limit:
		if(largestTorqueValue>maxTorque)
		{
			double scale = largestTorqueValue/maxTorque;
			torque /= scale;
		}
		

		//Convert torque to motor voltages:
		torque *= 10000.0;
		std::array<int, 3> enc_vec;
		enc_vec[0] = -torque[0];
		enc_vec[1] = -torque[1];
		enc_vec[2] = -torque[2];

		//And send them off to libnifalcon
		falcon.getFalconFirmware()->setForces(enc_vec);
		
	}


	return 0;
}

