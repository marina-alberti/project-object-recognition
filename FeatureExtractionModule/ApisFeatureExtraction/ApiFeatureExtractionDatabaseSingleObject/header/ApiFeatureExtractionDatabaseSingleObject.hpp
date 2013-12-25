/*
 * ApiFeatureExtractionDatabaseSingleObject.hpp
 *
 *  Created on: Nov 23, 2013
 *      Author: marina
 */

#ifndef APIFEATUREEXTRACTIONDATABASESINGLEOBJECT_HPP_
#define APIFEATUREEXTRACTIONDATABASESINGLEOBJECT_HPP_

#include "SceneSingleObjectFeature.hpp"
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
#include "ApiFeatureExtractionSceneSingleObject.hpp"
#include "DatabaseInformation.hpp"
#include "DatabaseSingleObjectFeature.hpp"


using namespace std;

class ApiFeatureExtractionDatabaseSingleObject {

private:



public:

static void extract(DatabaseInformation &, DatabaseSingleObjectFeature & );

};



#endif /* APIFEATUREEXTRACTIONDATABASESINGLEOBJECT_HPP_ */
