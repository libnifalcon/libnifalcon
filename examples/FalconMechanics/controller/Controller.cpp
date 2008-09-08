#include "Controller.h"

#include "boost/thread.hpp"
#include "boost/bind.hpp"
#include "boost/progress.hpp"



#include "kinematic/stamper/InverseKinematic.h"
#include "kinematic/stamper/JacobianMatrix.h"

#include <iostream>

namespace controller
{

	Controller::Controller()
	{
		falconKinematic = new libnifalcon::FalconKinematicStamper(false);
		std::cout << "Falcon initialization" << std::endl;
		//Don't set a kinematics model, since we're handing that ourselves
		falconModel = new libnifalcon::FalconDevice();
		int8_t i;
		falconModel->setFalconComm(&falconComm);
		falconModel->setFalconFirmware(&falconFirmware);
		falconModel->setFalconGrip(&falconGrip);

		if(!falconModel->open(0))
		{
			std::cout << "Can't open!" << std::endl;
		}
		
		falconFirmware.setHomingMode(true);
		int8_t count;
		if(!falconModel->isFirmwareLoaded())
		{
			std::cout << "Loading firmware..." << std::endl;
			std::cout << falconModel->setFirmwareFile("test_firmware.bin") << std::endl;		
			for(int i = 0; i < 10; ++i)
			{			
				if(falconModel->loadFirmware()) break;
			}
		}
		std::cout << "Kinematics initialization" << std::endl;
		if(falconModel->isFirmwareLoaded())
		{
			falconKinematic->initialize();
			std::cout << "Initialization finished" << std::endl;
			boost::thread thread(boost::bind( &libnifalcon::FalconDevice::runThreadLoop, falconModel));
		}
	}

	Controller::~Controller()
	{
		delete toolbox;
		delete viewer;
		delete falconModel;
		delete falconView;
	}

	void Controller::display()
	{
		if (viewer)
			viewer->display();
	}

	void Controller::reshape(int w, int h)
	{
		if (viewer)
			viewer->reshape(w, h);
	}

	void Controller::mouseButton(int button, int state, int x, int y)
	{
		if (viewer)
			viewer->mouseButton(button, state, x, y);
	}

	void Controller::mouseMove(int x, int y)
	{
		if (viewer)
			viewer->mouseMove(x, y);
	}

	void Controller::keyboard(unsigned char key, int x, int y)
	{
		if (viewer)
			viewer->keyboard(key, x, y);
	}

	void Controller::idle()
	{
		int16_t* encoders = new int16_t[3];
		encoders = falconModel->getFalconFirmware()->getEncoderValues();
		angle1 = falconKinematic->getTheta(encoders[0]);
		angle2 = falconKinematic->getTheta(encoders[1]);
		angle3 = falconKinematic->getTheta(encoders[2]);
	
		if (toolbox)
			toolbox->sync();
	
		gmtl::Point3f position = falconKinematic->getDirectKinematic()->calculate(gmtl::Vec3f(gmtl::Math::deg2Rad(angle1), gmtl::Math::deg2Rad(angle2), gmtl::Math::deg2Rad(angle3)));
	
		falconView->getEffector()->setPosition(position);
	
		libnifalcon::StamperKinematicImpl::Angle angle = libnifalcon::StamperKinematicImpl::InverseKinematic::calculate(position);
	
		falconView->getArm(view::Falcon::ARM_1)->setAngle(angle.theta1[view::Falcon::ARM_1]);
		falconView->getArm(view::Falcon::ARM_2)->setAngle(angle.theta1[view::Falcon::ARM_2]);
		falconView->getArm(view::Falcon::ARM_3)->setAngle(angle.theta1[view::Falcon::ARM_3]);
	
		falconView->getParallel(view::Falcon::ARM_1)->update(angle.theta1[view::Falcon::ARM_1],
															 angle.theta2[view::Falcon::ARM_1],
															 angle.theta3[view::Falcon::ARM_1]);
	
		falconView->getParallel(view::Falcon::ARM_2)->update(angle.theta1[view::Falcon::ARM_2],
															 angle.theta2[view::Falcon::ARM_2],
															 angle.theta3[view::Falcon::ARM_2]);
	
		falconView->getParallel(view::Falcon::ARM_3)->update(angle.theta1[view::Falcon::ARM_3],
															 angle.theta2[view::Falcon::ARM_3],
															 angle.theta3[view::Falcon::ARM_3]);
//		delete[] encoders;
/*
		if(falconModel->isHomed())
		{
			gmtl::Vec3f forceVector(0,0,torque1);
			gmtl::Matrix44f rotation;
			rotation.set(rotationMatrix[0], rotationMatrix[1], rotationMatrix[2], rotationMatrix[3],
						 rotationMatrix[4], rotationMatrix[5], rotationMatrix[6], rotationMatrix[7],
						 rotationMatrix[8], rotationMatrix[9], rotationMatrix[10], rotationMatrix[11],
						 rotationMatrix[12], rotationMatrix[13], rotationMatrix[14], rotationMatrix[15]);
			forceVector = rotation*forceVector;
	
			gmtl::Vec3f angularVelocity = libnifalcon::StamperKinematicImpl::JacobianMatrix::calculate(angle, forceVector);
	
			falconView->force->update(position, forceVector, rotation);
	
			float new_torque1 = -angularVelocity[libnifalcon::StamperKinematicImpl::ARM_1];
			float new_torque2 = -angularVelocity[libnifalcon::StamperKinematicImpl::ARM_2];
			float new_torque3 = -angularVelocity[libnifalcon::StamperKinematicImpl::ARM_3];
	
			falconModel->setTorque(libnifalcon::StamperKinematicImpl::ARM_1, new_torque1);
			falconModel->setTorque(libnifalcon::StamperKinematicImpl::ARM_2, new_torque2);
			falconModel->setTorque(libnifalcon::StamperKinematicImpl::ARM_3, new_torque3);
		}
*/

	}

	void Controller::createViewer()
	{
		viewer = new view::Viewer();
		falconView = new view::Falcon();
		viewer->setRootModel(falconView->getModel());
	}

	void Controller::createToolbox(int windowId, void (*idle)(void))
	{
		angle1 = 0;
		angle2 = 0;
		angle3 = 0;
		torque1 = 0;
		torque2 = 0;
		torque3 = 0;
	
		view::Toolbox::Parameter parameter;
		parameter.angle1 = &angle1;
		parameter.angle2 = &angle2;
		parameter.angle3 = &angle3;
		parameter.torque1 = &torque1;
		parameter.torque2 = &torque2;
		parameter.torque3 = &torque3;
		parameter.rotationMatrix = rotationMatrix;
		toolbox = new view::Toolbox(windowId, idle, parameter);	
	}

	void Controller::hapticLoop()
	{
		
		while(1)
		{
			
			falconModel->runIOLoop();
			/*
			angle1 = falconModel->getTheta(model::Falcon::ARM_1);
			angle2 = falconModel->getTheta(model::Falcon::ARM_2);
			angle3 = falconModel->getTheta(model::Falcon::ARM_3);

			gmtl::Point3f position = directKinematic->calculate(gmtl::Vec3f(gmtl::Math::deg2Rad(angle1), gmtl::Math::deg2Rad(angle2), gmtl::Math::deg2Rad(angle3)));
			
			model::falcon::Angle angle = model::InverseKinematic::calculate(position);
		
			gmtl::Vec3f center(0,0,120);


			gmtl::Vec3f vecSphere(position-center);
			float radius = 20;
			float norme = 0;
			if (length(vecSphere)<radius)
			{
				norme = ((radius-length(vecSphere))/3)+20;
				//norme = 50;

			}
			normalize(vecSphere);
			gmtl::Vec3f forceVector = vecSphere;
		
			gmtl::Vec3f angularVelocity = model::JacobianMatrix::calculate(angle, forceVector);
			
			float torque1 = -angularVelocity[model::Falcon::ARM_1]*norme;
			float torque2 = -angularVelocity[model::Falcon::ARM_2]*norme;
			float torque3 = -angularVelocity[model::Falcon::ARM_3]*norme;
		
			falconModel->setTorque(model::Falcon::ARM_1, torque1);
			falconModel->setTorque(model::Falcon::ARM_2, torque2);
			falconModel->setTorque(model::Falcon::ARM_3, torque3);

*/
		}

	}

}
