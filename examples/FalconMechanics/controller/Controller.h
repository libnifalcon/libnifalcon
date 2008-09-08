#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "../view/Viewer.h"
#include "../view/Toolbox.h"
#include "../view/falcon/Falcon.h"
#include "core/FalconDevice.h"
#include "comm/FalconCommLibFTDI.h"
#include "grip/FalconGripFourButton.h"
#include "firmware/FalconFirmwareNovintSDK.h"
#include "kinematic/FalconKinematicStamper.h"

namespace controller
{

	class Controller
	{
	public:
		Controller();
		virtual ~Controller();
	
		// begin: glut dispached functions
		void display();

		void reshape(int w, int h);

		void mouseButton(int button, int state, int x, int y);

		void mouseMove(int x, int y);

		void keyboard(unsigned char key, int x, int y);

		void idle();
		// end: glut dispached functions
	
		void createViewer();
	
		void createToolbox(int windowId, void (*idle)(void));
	
	private:
	
		void hapticLoop();

		float angle1;
		float angle2;
		float angle3;

		int torque1;
		int torque2;
		int torque3;
	
		float rotationMatrix[16];
	
		view::Viewer *viewer;
		view::Toolbox *toolbox;
		view::Falcon *falconView;

		libnifalcon::FalconDevice* falconModel;
		libnifalcon::FalconKinematicStamper* falconKinematic;
		libnifalcon::FalconGripFourButton falconGrip;
		libnifalcon::FalconCommLibFTDI falconComm;
		libnifalcon::FalconFirmwareNovintSDK falconFirmware;
	};

}

#endif /*CONTROLLER_H_*/
