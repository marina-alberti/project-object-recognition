/*
 * SceneInformation.cpp
 *
 *  Created on: Nov 18, 2013
 *      Author: marina
 */

#include "SceneInformation.hpp"
#include "Object.hpp"
#include <fstream>
#include <iostream>
#include <ios>
#include <sstream>
#include <boost/property_tree/xml_parser.hpp>

#define DEBUG 0


using namespace boost::property_tree;

SceneInformation::SceneInformation () {
  sceneType = "";
  numberOfObjects = 0;
  referenceLength = 0;
  referenceWidth = 0;
  referenceCentroid.x = 0;
  referenceCentroid.y = 0;
  referenceCentroid.z = 0;
}

void SceneInformation::setReferenceLength(float inputLength) {
  referenceLength = inputLength;
}

float SceneInformation::getReferenceLength() {
  return referenceLength;
}

void SceneInformation::setReferenceWidth(float inputWidth) {
  referenceWidth = inputWidth;
}

float SceneInformation::getReferenceWidth() {
  return referenceWidth;
}

void SceneInformation::setType(string inputString) {
  sceneType = inputString;
}

string SceneInformation::getType() {
  return sceneType;
}

void SceneInformation::setReferenceCentroid() {
  referenceCentroid.x = referenceLength / 2;
  referenceCentroid.y = referenceWidth / 2;
  referenceCentroid.z = 0;
}

pcl::PointXYZ SceneInformation::getReferenceCentroid() {
  return referenceCentroid;
}

void SceneInformation::addObject(Object& currentObject) {
  objectList.push_back(currentObject);
  numberOfObjects++;
}

vector<Object> SceneInformation::getObjectList(){
  return objectList;
}

void SceneInformation::showSceneInformation() {
  cout << "This scene is of type: " <<  getType() << endl
       << "Internal data structure (IDS): " << endl;
  for (int i = 0; i < objectList.size(); i++) {
    Object currentObject = objectList[i];
    if (DEBUG) {
      cout << endl << currentObject.getObjectName() << endl;
    }
    currentObject.getBoundingBox();
    currentObject.getCentroid();
  }
}

vector<int> SceneInformation::getObjectIds() {

	vector<int> out;
	for(int i = 0; i < objectList.size(); i++) {
		out.push_back(objectList.at(i).getInstanceID());
	}
	return out;

}

