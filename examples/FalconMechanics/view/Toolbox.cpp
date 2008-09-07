#include "Toolbox.h"

namespace view
{

Toolbox::Toolbox(int windowId, void (*idle)(void), Parameter parameter)
{
	GLUI_Master.set_glutIdleFunc(idle);
	
	glui = GLUI_Master.create_glui("Toolbox"); 
	angle1 = glui->add_edittext("Angle 1",GLUI_EDITTEXT_FLOAT, parameter.angle1);
	angle2 = glui->add_edittext("Angle 2",GLUI_EDITTEXT_FLOAT, parameter.angle2);
	angle3 = glui->add_edittext("Angle 3",GLUI_EDITTEXT_FLOAT, parameter.angle3);
	angle1->disable();
	angle2->disable();
	angle3->disable();
	
	torque1 = glui->add_spinner("Torque 1", GLUI_SPINNER_INT, parameter.torque1);
	torque2 = glui->add_spinner("Torque 2", GLUI_SPINNER_INT, parameter.torque2);
	torque3 = glui->add_spinner("Torque 3", GLUI_SPINNER_INT, parameter.torque3);
	torque1->set_float_limits(-2000, 2000, GLUI_LIMIT_CLAMP);
	torque2->set_float_limits(-2000, 2000, GLUI_LIMIT_CLAMP);
	torque3->set_float_limits(-2000, 2000, GLUI_LIMIT_CLAMP);
	
	forceDirection = glui->add_rotation("Force direction", parameter.rotationMatrix);
	forceDirection->reset();
	
	glui->set_main_gfx_window(windowId);
}

Toolbox::~Toolbox()
{
}

void Toolbox::sync()
{
	glui->sync_live();
}

}
