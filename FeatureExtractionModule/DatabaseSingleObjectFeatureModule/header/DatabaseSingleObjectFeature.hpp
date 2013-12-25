/*
 * DatabaseSingleObjectFeature.hpp
 *
 *  Created on: Nov 18, 2013
 *      Author: marina
 */

#ifndef DATABASESINGLEOBJECTFEATURE_HPP_
#define DATABASESINGLEOBJECTFEATURE_HPP_


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
#include "SceneSingleObjectFeature.hpp"


using namespace std;

class DatabaseSingleObjectFeature{

private:

	vector<SceneSingleObjectFeature> listSceneSingleObjectFeature;

	int numberOfCategories;

public:

    void addSceneSingleObjectFeature(SceneSingleObjectFeature);
    vector<SceneSingleObjectFeature> getListSceneSingleObjectFeature() { return listSceneSingleObjectFeature; }

    void setNumberOfCategories(int n) {numberOfCategories = n; }

    int getNumberOfCategories() { return numberOfCategories; }
};


#endif /* DATABASESINGLEOBJECTFEATURE_HPP_ */
