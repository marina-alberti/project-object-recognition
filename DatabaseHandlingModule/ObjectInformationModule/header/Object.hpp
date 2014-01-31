/*
 * Object.hpp
 *
 *  Created on: Nov 17, 2013
 *      Author: marina
 */

#ifndef OBJECT_HPP_
#define OBJECT_HPP_

#include<iostream>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <vector>
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include "utils.hpp"


using namespace std;

class Object{

private:

	/*
	The name of an object instance as it loaded from the
	manual annotation tool.
	*/
	string objectName;

	/*
	The name of an object instance as it loaded from the
	simulated dataset.
	*/
	string instanceName;

	/* A string identifier for the object categories */
	string categoryName;

	/* An int identifier for the object categories */
	int actualObjectID;

	/* An int identifier representing the instance id (mainly for the test phase)*/
	int instanceID;

	/*
	This variable stores a value for object category ID (int) that
	may be different from the "actual" ID
	*/
	int predictedObjectID;

	pcl::PointCloud<pcl::PointXYZ> boundingBox;
	pcl::PointXYZ centroid;

	/*
	This function computes the centroid of the bouding box given the 8 vertices
	of the bounging box and sets the private member: "centroid".
	Computes the 3D centroid coordinates: x, y, z, by computing the mean of the corresponding
	coordinate values of all the 8 vertices of the bounding box cuboid.
	*/

public:

	Object();
	void setObjectName(string);
	void setPredictedObjectID(int i) { predictedObjectID = i; }
	void setActualObjectID(int i) { actualObjectID = i; }
	void setInstanceName(string);
	void setCategoryName(string);
	void setInstanceID(int i) { instanceID = i; }

	void setObjectParameters(vector<pcl::PointXYZ>, string = "", string = "");

	/*
	This function sets the "boundingBox" data member given in input a
	vector of PCL points corresponding to the 8 vertices.
	*/
	void setBoundingBox(vector<pcl::PointXYZ>);

	void setCentroidPoint(pcl::PointXYZ);

	void setCentroid();


	int getActualObjectID() { return actualObjectID; }
	int getPredictedObjectID() { return predictedObjectID; }
	string getObjectName() { return objectName; }
	pcl::PointCloud<pcl::PointXYZ> getBoundingBox();
	pcl::PointXYZ getCentroid();
	string getInstanceName() { return instanceName; }
	string getCategoryName() { return categoryName; }
	int getInstanceID() { return instanceID; }

};

#endif /* OBJECT_HPP_ */
