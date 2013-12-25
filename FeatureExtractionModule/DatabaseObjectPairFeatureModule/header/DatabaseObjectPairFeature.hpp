/*
 * DatabaseObjectPairFeature.hpp
 *
 *  Created on: Nov 18, 2013
 *      Author: marina
 */

#ifndef DATABASEOBJECTPAIRFEATURE_HPP_
#define DATABASEOBJECTPAIRFEATURE_HPP_


#include "Object.hpp"
#include <string>
#include <vector>
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <pcl/common/transforms.h>
#include <pcl/common/eigen.h>
#include <pcl/common/angles.h>
#include "math.h"
#include "SceneObjectPairFeature.hpp"


using namespace std;

class DatabaseObjectPairFeature{

private:

	vector<SceneObjectPairFeature> listSceneObjectPairFeature;

	int numberOfCategories;

public:

    void addSceneObjectPairFeature(SceneObjectPairFeature);

    vector<SceneObjectPairFeature> getListSceneObjectPairFeature() { return listSceneObjectPairFeature; }

    void setNumberOfCategories(int n) {numberOfCategories = n; }

    int getNumberOfCategories() { return numberOfCategories; }

};




#endif /* DATABASEOBJECTPAIRFEATURE_HPP_ */
