/*
 * DatabaseInformation.cpp
 *
 *  Created on: Nov 18, 2013
 *      Author: marina
 */

#include "DatabaseInformation.hpp"

#define DEBUG 0
#define TESTFLAG 1

DatabaseInformation::DatabaseInformation(int in) {
  numberOfScenes = 0;
  numberOfCategories = in;
}


DatabaseInformation::DatabaseInformation(vector<SceneInformation> in, int cat) {

	sceneList = in;
	numberOfScenes = in.size();
	numberOfCategories = cat;

}
//****************************************************************************

void DatabaseInformation::loadAnnotations_KTH(vector<string> fileList) {

  for (int i = 0; i < fileList.size(); i++ ) {

    string filenameXML = fileList.at(i);

    if (TESTFLAG)  { cout << "In loadAnnotationsInIDS, The XML file name is: " << filenameXML << endl;    }

    SceneInformation currentScene;
    ApiConvertKTHDB converter;
    converter.parseFileXML(filenameXML, currentScene);
    sceneList.push_back(currentScene);
    numberOfScenes++;

    if (DEBUG) {cout << "Added a new scene to the sceneList. " << endl; }

  }
}

//****************************************************************************

/*
   Loads into the "sceneList" data member of this "DatabaseInformation" class
   all the scenes present in the given file.
   TO DO: add first and last scenes to load, and add possibility
   to load all scenes but 1 for leave-one-out cross-validation
*/

void DatabaseInformation::loadAnnotations_Simulation(string fileAnnotations) {

  if (TESTFLAG)  { cout << "The JSON file name is: " << fileAnnotations << endl; }

  ApiConvertSimulationDB::parseFileJSON(fileAnnotations, sceneList);   // pass by reference the scene list

  numberOfScenes = sceneList.size();

  if (DEBUG)  { cout << "The number of scenes in the database is : " << numberOfScenes << endl; }

}



void DatabaseInformation::loadAnnotations_RealWorld(string fileAnnotations) {

  if (TESTFLAG)  { cout << "The JSON file name is: " << fileAnnotations << endl; }

  ApiConvertRealWorldDB::parseFileJSON(fileAnnotations, sceneList);   // pass by reference the scene list

  numberOfScenes = sceneList.size();

  if (DEBUG)  { cout << "The number of scenes in the database is : " << numberOfScenes << endl; }

}


void DatabaseInformation::printSceneInformation() {

	for (int i = 0; i < 1; i++) {
		SceneInformation scene = sceneList.at(i);
		int nObjects = scene.getNumberOfObjects();

		cout << endl << "Scene number " << i << endl;
		cout << "The scene contains " << nObjects << " objects." << endl;

		vector<Object> listObj = scene.getObjectList();

		for (int j = 0; j < listObj.size(); j++) {

			cout << endl << "New object" << endl;

			Object obj = listObj.at(j);
			int id = obj.getActualObjectID();
			cout << "the actual object ID is: " << id << endl;
			string category = obj.getCategoryName();
			cout << "The category name is: " << category << endl;
			string objectname = obj.getObjectName();
			cout << "The obj name is: " << objectname << endl;
			string instance = obj.getInstanceName();
			cout << "The instance name is: " << instance << endl;

			pcl::PointXYZ centroid = obj.getCentroid();
			cout << "The centroid is:   " << centroid.x << "   " << centroid.y << "  " << centroid.z  << endl;

			pcl::PointCloud<pcl::PointXYZ> bbox = obj.getBoundingBox();
			cout << "The size of the bounding box is " << bbox.size() << endl;
		}

		cout << "end of object list" << endl;
	}

	cout <<  "end of scene list " << endl;

}



