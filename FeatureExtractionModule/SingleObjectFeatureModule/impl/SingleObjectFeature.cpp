/*
 * SingleObjectFeature.cpp
 *
 *  Created on: Nov 18, 2013
 *      Author: marina
 */

#include "SingleObjectFeature.hpp"


void SingleObjectFeature::setObjectID(int in) {
	objectID = in;
}

void SingleObjectFeature::setInstanceID(int i) {

	instanceID = i;

}

void SingleObjectFeature::setInstanceName(string i) {
	instanceName = i;
}

void SingleObjectFeature::setPose(float x, float y, float z) {
	poseX = x;
	poseY = y;
	poseZ = z;
	allFeatures.push_back(x);
	allFeatures.push_back(y);
	allFeatures.push_back(z);

}

void SingleObjectFeature::setAngle2dCentroid(float in) {
	angle2dCentroid = in;
	allFeatures.push_back(in);

}

void SingleObjectFeature::setAngle2d(float in) {
	angle2d = in;
	allFeatures.push_back(in);

}

void SingleObjectFeature::setVolume(float in) {

	volumeSize = in;
	allFeatures.push_back(in);

}

void SingleObjectFeature::setSizeProjectedX(float in) {
	sizeProjectedX = in;
	allFeatures.push_back(in);

}

void SingleObjectFeature::setSizeProjectedY(float in) {
	sizeProjectedY  = in;
	allFeatures.push_back(in);

}

void SingleObjectFeature::setSizeProjectedZ(float in) {
	sizeProjectedZ = in;
	allFeatures.push_back(in);

}


