/*
 * ApiFeatureExtractionSceneObjectPair.hpp
 *
 *  Created on: Nov 22, 2013
 *      Author: marina
 */

#ifndef APIFEATUREEXTRACTIONSCENEOBJECTPAIR_HPP_
#define APIFEATUREEXTRACTIONSCENEOBJECTPAIR_HPP_


#include "ApiFeatureExtractionObjectPair.hpp"
#include "SceneObjectPairFeature.hpp"
#include "SceneInformation.hpp"
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


using namespace std;

class ApiFeatureExtractionSceneObjectPair {

private:



public:

static void extract(SceneInformation &, SceneObjectPairFeature & );

};





#endif /* APIFEATUREEXTRACTIONSCENEOBJECTPAIR_HPP_ */
