/*
 * SingleObjectFeature.hpp
 *
 *  Created on: Nov 18, 2013
 *      Author: marina
 */

#ifndef SINGLEOBJECTFEATURE_HPP_
#define SINGLEOBJECTFEATURE_HPP_

#include "Object.hpp"
#include <string>
#include <vector>
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <pcl/common/transforms.h>
#include <pcl/common/eigen.h>
#include <pcl/common/angles.h>
#include "math.h"
//#include "ApiFeatureExtractionSingleObject.hpp"

using namespace std;

class SingleObjectFeature{


private:

	int objectID;

	int instanceID;
	string instanceName;

	float poseX;
	float poseY;
	float poseZ;
	float angle2dCentroid;
	float angle2d;
    float volumeSize;
	float sizeProjectedX;
	float sizeProjectedY;
	float sizeProjectedZ;

	vector<float> allFeatures;

public:

	void setObjectID(int);
	void setInstanceID(int);
	void setInstanceName(string);

	void setPose(float, float, float);
	void setAngle2dCentroid(float);
	void setAngle2d(float);
	void setVolume(float);
	void setSizeProjectedX(float);
	void setSizeProjectedY(float);
	void setSizeProjectedZ(float);

    // get functions
	int getObjectID() { return objectID; }
	int getInstanceID() { return instanceID; }
	string getInstanceName() { return instanceName; }

	float getPoseX() {return poseX; }
	float getPoseY() {return poseY; }
	float getPoseZ() {return poseZ; }
	float getAngle2dCentroid() {return angle2dCentroid;}
	float getAngle2d() { return angle2d; }
	float getVolume() {return volumeSize; }
	float getSizeProjectedX() {return sizeProjectedX; }
	float getSizeProjectedY() {return sizeProjectedY; }
	float getSizeProjectedZ() {return sizeProjectedZ; }
	vector<float> getAllFeatures() {return allFeatures; }

};




#endif /* SINGLEOBJECTFEATURE_HPP_ */
