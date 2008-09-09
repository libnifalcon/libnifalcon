#ifndef TOOLBOX_H_
#define TOOLBOX_H_

#include <glui.h>
#include <string>

namespace view
{

class Toolbox
{
public:
	struct Parameter
	{
		float *angle1;
		float *angle2;
		float *angle3;
		int *torque1;
		int *torque2;
		int *torque3;
		float *rotationMatrix;
	};
	
	Toolbox(int windowId, void (*idle)(void), Parameter parameter);
	virtual ~Toolbox();
	void sync();
	
private:
	GLUI *glui;
	GLUI_EditText *angle1;
	GLUI_EditText *angle2;
	GLUI_EditText *angle3;
	GLUI_Spinner *torque1;
	GLUI_Spinner *torque2;
	GLUI_Spinner *torque3;
	GLUI_Rotation *forceDirection;
};

}

#endif /*TOOLBOX_H_*/
