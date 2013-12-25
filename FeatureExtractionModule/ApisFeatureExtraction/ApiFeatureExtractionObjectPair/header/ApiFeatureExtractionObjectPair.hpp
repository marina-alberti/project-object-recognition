/*
 * ApiObjectPairFeature.hpp
 *
 *  Created on: Nov 18, 2013
 *      Author: marina
 */

#ifndef APIOBJECTPAIRFEATURE_HPP_
#define APIOBJECTPAIRFEATURE_HPP_

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
#include "ObjectPairFeature.hpp"


class ApiFeatureExtractionObjectPair {

private:

	float EuclideanDistance;
	float EuclideanDistance2d;
	float orientation2d;
	float sizeDifference;
	float verticalHeightDifference;

	void computeEuclideanDistance(Object & , Object &);
	void computeEuclideanDistance2d(Object & , Object &);
	void computeOrientation2d(Object & , Object &);
	void computeMinimumDistanceBoudaries(Object & , Object &);
	void computeSizeDifference(Object & , Object &);
	void computeVerticalHeightDifference(Object & , Object &);

public:

	void extractFeatures(Object & , Object & , ObjectPairFeature &);

};



#endif /* APIOBJECTPAIRFEATURE_HPP_ */
