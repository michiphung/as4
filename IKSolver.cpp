
#include "Algebra3.h"
#include "IKSolver.h"
#pragma once

#define MAX_TRIES	1500	// Maximum number of tries through the CCD loop.
#define THRESHOLD	0.001f	// Threshold for success.

// Solve the given inverse kinematics problem using the CCD algorithm.
void IKSolver::performCCD(Joint* lastJoint, vec3 &goalTip) {

	if (lastJoint == NULL) return;

	// printf("hi! fill me in.\n");
	// lastJoint->tip += vec3(0.1, 0.1, 0.1); // debugging
	// lastJoint->translate(vec3(0.1, 0.1, 0.1));

	// Local variables
	Joint* currJoint = lastJoint;
	vec3 tipVec, currBase, currTip, currVec, targetVec, crossVec;

	int numTries = 0;
	double diff, dotProduct, angDeg, angRad;
	bool done = false;

	// Translations between our variable names and those in the example:
	// rootPos = currBase, curEnd = tipVec, desiredEnd/endPos = goalTip,
	// curVector = currVec, targetVector = targetVec

	do {
		// Obtain necessary positions
		currBase = vec3(currJoint->base);
		currTip = vec3(currJoint->tip);

		// Determine if the current position of end effector is within tolerance.
		tipVec = vec3(lastJoint->tip);
		// cout << "tipVec is " << tipVec << endl;
		diff = (vec3(goalTip - tipVec)).length2();
		// cout << "diff is " << diff << endl;

		if (diff <= THRESHOLD) {
			// Within tolerance: stop the loop!
			done = true;
		} else {
			// Create the vector from the root to the current effector position.
			currVec = vec3(tipVec - currBase);

			// Create the desired effector position vector.
			targetVec = vec3(goalTip - currBase);

			// cout << "currVec is " << currVec << endl;
			// cout << "targetVec is " << targetVec << endl;

			// Normalize the vectors.
			currVec.normalize();
			targetVec.normalize();

			// Obtain the dot product. This returns the cosine
			// of the angle between the two vectors.
			dotProduct = currVec * targetVec;

			// If the cosine is 1, do nothing, since it is zero degrees.
			if (dotProduct < 0.9999f) {
				// Get the cross product to figure out which axis to rotate about.
				crossVec = currVec ^ targetVec;
				// cout << "New cross vec is " << crossVec << endl;

				// Determine the angle of rotation.
				angRad = acos((double) dotProduct);
				angDeg = (angRad * 180.0) / M_PI;
				// cout << "angle in degree " << angDeg << endl;

				// Rotate about the axis determined.
				currJoint->rotate(crossVec, angDeg);
			}

			// Determine the next joint to work with.
			currJoint = currJoint->prev;
			if (currJoint == NULL) currJoint = lastJoint;
		}
	} while (numTries++ <= MAX_TRIES && !done);
};