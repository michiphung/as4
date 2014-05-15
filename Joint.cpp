
#include "Joint.h"
#pragma once

Joint::Joint(vec3 my_base, vec3 my_tip): base(my_base), tip(my_tip) {
	length = (float)(base - tip).length();
	prev = NULL;
	next = NULL;
};

Joint::Joint(Joint* my_prev, vec3 my_tip): prev(my_prev), tip(my_tip) {
	base = prev->tip;
	length = (float)(base - tip).length();
	prev->next = this;
	next = NULL;
};

// Rotate the arm joint about the given axis, through the given
// angle represented in degrees.
void Joint::rotate(vec3 &axis, const double angleDeg) {
	
	// Local variables.
	vec4 currBase4D;
	vec4 currTip4D;
	Joint* currJoint;

	// Create the proper transformation matrices:
	// Translate the arm joint and subsequent joints so that
	// the base of the first arm joint is at the origin.
	mat4 translateMatrix = translation3D(vec4((-1 * base), 1.0));
	// Rotate about the axis provided.
	mat4 rotateMatrix = rotation3D(axis, angleDeg);
	// Move the arm joint back to its original position.
	mat4 revTranslateMatrix = translation3D(vec4(base, 1.0));

	currJoint = this;
	while (currJoint != NULL) {
		// Perform the transformations *up* the chain.
		currBase4D = vec4(currJoint->base, 1.0);
		currTip4D = vec4(currJoint->tip, 1.0);

		currBase4D = rotateMatrix * (translateMatrix * currBase4D);
		currTip4D = rotateMatrix * (translateMatrix * currTip4D);

		currBase4D = revTranslateMatrix * currBase4D;
		currTip4D = revTranslateMatrix * currTip4D;

		currJoint->base = vec3(currBase4D, VW);
		currJoint->tip = vec3(currTip4D, VW);

		currJoint = currJoint->next;
	};
};