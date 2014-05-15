#ifndef IKSolver_h
#define IKSolver_h
 #pragma once

#include "Algebra3.h"

class IKSolver{
public:
	void performCCD(Joint* lastJoint, vec3 &goalTip); 
};

#endif