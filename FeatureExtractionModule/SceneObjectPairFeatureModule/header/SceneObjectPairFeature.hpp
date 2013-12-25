/*
 * SceneObjectPairFeature.hpp
 *
 *  Created on: Nov 18, 2013
 *      Author: marina
 */

#ifndef SCENEOBJECTPAIRFEATURE_HPP_
#define SCENEOBJECTPAIRFEATURE_HPP_

#include "Object.hpp"
#include <string>
#include <vector>
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <pcl/common/transforms.h>
#include <pcl/common/eigen.h>
#include <pcl/common/angles.h>
#include "math.h"
#include "ObjectPairFeature.hpp"


using namespace std;

class SceneObjectPairFeature{

private:

	vector<ObjectPairFeature> listObjectPairFeature;

public:

    void addObjectPairFeature(ObjectPairFeature);
    vector<ObjectPairFeature> getListObjectPairFeature() { return listObjectPairFeature; }

};



#endif /* SCENEOBJECTPAIRFEATURE_HPP_ */
