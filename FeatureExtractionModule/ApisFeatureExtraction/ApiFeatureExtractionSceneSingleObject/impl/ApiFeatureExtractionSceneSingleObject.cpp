/*
 * ApiFeatureExtractionSceneSingleObject.cpp
 *
 *  Created on: Nov 18, 2013
 *      Author: marina
 */


#include "ApiFeatureExtractionSceneSingleObject.hpp"


void ApiFeatureExtractionSceneSingleObject::extract(SceneInformation & scene, SceneSingleObjectFeature & out) {

	vector<Object> objectList = scene.getObjectList();
	pcl::PointXYZ centroid = scene.getReferenceCentroid();


	// for each object in the scene
	for (vector<Object>::iterator it = objectList.begin(); it != objectList.end(); it++ ) {

		SingleObjectFeature currentObjectFeature;
		ApiFeatureExtractionSingleObject fe;

		fe.extractFeatures( *it, centroid, currentObjectFeature);

		out.addSingleObjectFeature(currentObjectFeature);

    }

}


void ApiFeatureExtractionSceneSingleObject::extractNoReference(SceneInformation & scene, SceneSingleObjectFeature & out) {

	vector<Object> objectList = scene.getObjectList();
	//pcl::PointXYZ centroid = scene.getReferenceCentroid();


	// for each object in the scene
	for (vector<Object>::iterator it = objectList.begin(); it != objectList.end(); it++ ) {

		SingleObjectFeature currentObjectFeature;
		ApiFeatureExtractionSingleObject fe;

		fe.extractFeaturesNoReference( *it,  currentObjectFeature);

		out.addSingleObjectFeature(currentObjectFeature);

    }

}
