/*
 * SceneSingleObjectfeature.hpp
 *
 *  Created on: Nov 18, 2013
 *      Author: marina
 */

#ifndef SCENESINGLEOBJECTFEATURE_HPP_
#define SCENESINGLEOBJECTFEATURE_HPP_

#include "Object.hpp"
#include <string>
#include <vector>
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <pcl/common/transforms.h>
#include <pcl/common/eigen.h>
#include <pcl/common/angles.h>
#include "math.h"
#include "SingleObjectFeature.hpp"


using namespace std;

class SceneSingleObjectFeature{

private:

	vector<SingleObjectFeature> listSingleObjectFeature;

public:

    void addSingleObjectFeature(SingleObjectFeature);
    vector<SingleObjectFeature> getListSingleObjectFeature() { return listSingleObjectFeature; }

};

#endif /* SCENESINGLEOBJECTFEATURE_HPP_ */
