
#ifndef JOINT_H
#define JOINT_H

#include "Algebra3.h"

class Joint {
public:
	Joint(vec3 my_base, vec3 my_tip);
	Joint(Joint* my_prev, vec3 my_tip);

	void rotate(vec3 &axis, const double angleDeg);

	vec3 base;
	vec3 tip;
	Joint* prev; // prev Joint's "tip" is attached to this "base"
	Joint* next; // next Joint's "base" is attached to this "tip"

protected:
	float length;
};

#endif  // _ARM_H