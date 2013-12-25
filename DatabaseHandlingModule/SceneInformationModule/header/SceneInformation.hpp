/*
 * SceneInformation.hpp
 *
 *  Created on: Nov 18, 2013
 *      Author: marina
 */

#ifndef SCENEINFORMATION_HPP_
#define SCENEINFORMATION_HPP_


#include <string.h>
#include <boost/property_tree/ptree.hpp>
#include "Object.hpp"
#include <vector>
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>


class SceneInformation{

private:

	int numberOfObjects;

	vector<Object> objectList;

	string sceneType;

	/*
	Information about the reference system (desk)
	in the scene representation.
	*/
	pcl::PointXYZ referenceCentroid;
	float referenceLength;
	float referenceWidth;

public:

	SceneInformation();

	void addObject(Object &);

	void setType(string);
	void setReferenceLength(float);
	void setReferenceWidth(float);
	void setReferenceCentroid();

	vector<Object> getObjectList();
	string getType();
	float getReferenceLength();
	float getReferenceWidth();
	pcl::PointXYZ getReferenceCentroid();
	int getNumberOfObjects() { return numberOfObjects; }

	void showSceneInformation();

	vector<int> getObjectIds();

};



#endif /* SCENEINFORMATION_HPP_ */
