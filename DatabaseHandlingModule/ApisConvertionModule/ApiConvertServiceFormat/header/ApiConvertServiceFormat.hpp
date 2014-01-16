/*
 * ApiConvertServiceFormat.hpp
 *
 *  Created on: Jan 9, 2014
 *      Author: marina
 */

#ifndef APICONVERTSERVICEFORMAT_HPP_
#define APICONVERTSERVICEFORMAT_HPP_

#include "Object.hpp"
#include "SceneInformation.hpp"
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <vector>
#include <pcl/common/transforms.h>
#include <pcl/common/eigen.h>
#include <pcl/common/angles.h>
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>


class ApiConvertServiceFormat {

private:

public:

	ApiConvertServiceFormat();
	static void parseScene(vector<string> objectList, vector<vector<pcl::PointXYZ> > bbox, vector<pcl::PointXYZ> pose, SceneInformation & currentScene);

    // TODO check with test and print old bb and new bb
    static vector<pcl::PointXYZ> addNoiseBoundingBox(vector<pcl::PointXYZ> boundingBoxVertices, int noiseAmount);

};


#endif /* APICONVERTSERVICEFORMAT_HPP_ */
