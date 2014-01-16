/*
 * ApiConvertServiceFormat.cpp
 *
 *  Created on: Jan 9, 2014
 *      Author: marina
 */


#include "ApiConvertServiceFormat.hpp"



void ApiConvertServiceFormat::parseScene(vector<string> objectList, vector<vector<pcl::PointXYZ> > bboxList, vector<pcl::PointXYZ> poseList, SceneInformation & currentScene){

	int counter = 0;

	// for each object in the arrays of the service request, a new object is added to the scene in input
	for (vector<string>::iterator it = objectList.begin(); it != objectList.end(); ++it) {

        Object newObject;

        newObject.setInstanceName(*it);

        // // may need change.
        ////  the instance id is an int following the same order of appearance of the objects in the scene
        newObject.setInstanceID(counter);

        newObject.setCentroidPoint(poseList.at(counter));
        newObject.setBoundingBox(bboxList.at(counter));

        currentScene.addObject(newObject);

        counter++;
    }
}


