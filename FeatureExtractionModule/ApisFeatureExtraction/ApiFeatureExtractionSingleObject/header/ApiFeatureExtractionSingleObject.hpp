/*
 * ApiSingleObjectFeature.hpp
 *
 *  Created on: Nov 18, 2013
 *      Author: marina
 */

#ifndef APISINGLEOBJECTFEATURE_HPP_
#define APISINGLEOBJECTFEATURE_HPP_

#include "Object.hpp"
#include <string>
#include <vector>
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <pcl/common/transforms.h>
#include <pcl/common/eigen.h>
#include <pcl/common/angles.h>
#include <algorithm>
#include <cmath>
#include "math.h"
#include "SingleObjectFeature.hpp"


class ApiFeatureExtractionSingleObject{

private:

	float poseX;
	float poseY;
	float poseZ;
	float angle2dCentroid;
	float angle2d;
	float angle3d;
	float volumeSize;
	float sizeProjectedX;
	float sizeProjectedY;
	float sizeProjectedZ;

	void computePose(Object & , pcl::PointXYZ );
	void computeAngle2dCentroid(Object & , pcl::PointXYZ );
	void computeAngle2d(Object & );

	/* to do: check vector4f - check : which angle do i want to compute?
	See kasper2011, they dont compute the 3d orientations but only 2d */
	void computeAngle3d(Object & , pcl::PointXYZ);
	void computeVolumeSize(Object &);
	void computeSizeProjectedX(Object &);
	void computeSizeProjectedY(Object &);
	void computeSizeProjectedZ(Object &);

public:

	void extractFeatures(Object & , pcl::PointXYZ , SingleObjectFeature &);

};

#endif /* APISINGLEOBJECTFEATURE_HPP_ */
