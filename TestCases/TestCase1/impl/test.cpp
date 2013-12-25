/*
 * test.cpp
 *
 *  Created on: Nov 28, 2013
 *      Author: marina
 */


#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <iostream>
#include <fstream>
#include "utils.hpp"
#include "SceneInformation.hpp"
#include "ApiConvertKTHDB.hpp"
#include "DatabaseInformation.hpp"
#include "ApiFeatureExtractionDatabaseSingleObject.hpp"
#include "ApiFeatureExtractionDatabaseObjectPair.hpp"
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include "ArrangeFeatureTraining.hpp"
#include "DatabaseSingleObjectFeature.hpp"
#include "DatabaseObjectPairFeature.hpp"
#include "Training.hpp"
#include "ModelTrainedIO.hpp"
#include "ArrangeFeatureTestScene.hpp"
#include "Test.hpp"
#include "ApiStatisticsDatabase.hpp"


#define DEBUG 1

/*
http://stackoverflow.com/questions/3722704/c-read-numbers-from-file-and-store-in-vectors
*/

using namespace std;

int main() {

	// convert annotation in XML files into IDS

	string dir = "/home/marina/workspace_eclipse_scene_object_classification/data/data_more_objects/";
	vector<string> listXMLfiles =  storeFileNames(dir);
	DatabaseInformation db;
	db.loadAnnotations_KTH(listXMLfiles);

	vector<SceneInformation> allScenes = db.getSceneList();
	vector<SceneInformation> trainingScenes;
	for (int i = 0 ; i < 35; i++) {
		trainingScenes.push_back(allScenes.at(i));
	}
	DatabaseInformation trainingDB(trainingScenes);

	vector<SceneInformation> testScenes;
	for (int i = 36 ; i < 42; i++) {
		testScenes.push_back(allScenes.at(i));
	}
	DatabaseInformation testDB(testScenes);


	cout << "the size of the database is: " << db.getNumberOfScenes() << endl;
	db.printSceneInformation();


	// feature extraction

	DatabaseSingleObjectFeature dbSof;
	ApiFeatureExtractionDatabaseSingleObject::extract(db, dbSof);
	DatabaseObjectPairFeature dbOpf;
	ApiFeatureExtractionDatabaseObjectPair::extract(db, dbOpf);


	// arrange the features

	vector<vector<vector<float> > > FMSingleObject;
	ArrangeFeatureTraining::setFeatureMatrixSingleObject(dbSof, FMSingleObject);
	vector<vector<vector<vector<float> > > > FMObjectPair;
	ArrangeFeatureTraining::setFeatureMatrixObjectPair(dbOpf, FMObjectPair);

	// print
	cout << "size of feature matrix is: " <<  FMSingleObject.size() << endl;
	cout << "size of feature matrix dim 2 is: " <<  FMSingleObject.at(0).size() << endl;
	cout << "size of feature matrix dim 3 is: " << FMSingleObject.at(0).at(0).size() << endl;

	ArrangeFeatureTraining::printFeatureMatrixSingleObject(FMSingleObject);
	ArrangeFeatureTraining::printFeatureMatrixObjectPair(FMObjectPair);

	// Learning

	int nclusters = 2;
	int normalizationOption = 0;
	Training doTraining;
	doTraining.learnGMMSingleObjectFeature(FMSingleObject, nclusters, normalizationOption);
	doTraining.learnGMMObjectPairFeature(FMObjectPair, nclusters, normalizationOption);

	string folder = "params";
	ModelTrainedIO::storeTrainingToFile(doTraining, folder);

	// compute object frequencies and co-occurrence

	vector<double> frequenciesSingleObject = ApiStatisticsDatabase::computeFrequenciesSingleObject(db);
	vector<vector<double> > frequenciesObjectPair = ApiStatisticsDatabase::computeFrequenciesObjectPair(db);

	ModelTrainedIO::storefrequencies(frequenciesSingleObject, frequenciesObjectPair, folder);


	// ******************************************************************************************************
	// Test

	string testFile = listXMLfiles.at(0);
	ApiConvertKTHDB testSceneConverter;
	SceneInformation testScene;

	testSceneConverter.parseFileXML(testFile, testScene);

	// // feature extraction
	SceneSingleObjectFeature sceneSof;
	SceneObjectPairFeature sceneOpf;
	ApiFeatureExtractionSceneSingleObject::extract(testScene, sceneSof);
	ApiFeatureExtractionSceneObjectPair::extract(testScene, sceneOpf);

	// // Arrange features of test scene

	ArrangeFeatureTestScene arrageFeaturesTest;
	arrageFeaturesTest.arrangeTestFeatures(sceneSof, sceneOpf);

	// // testing

	Test testingScene;
	string paramsfolder = "params";

	// // loading from the files
	bool loadfromfile = false;

	if (loadfromfile) {

		ModelTrainedIO::loadTrainedGMMsFile(paramsfolder, testingScene);
		ModelTrainedIO::loadfrequencies(paramsfolder, testingScene);
	}
	else {

		// // loading directly from the saved models into the training class - no use of the files
		testingScene.loadTrainedGMMs(doTraining);
		testingScene.loadLearnedObjectCategoryFrequency(frequenciesSingleObject, frequenciesObjectPair);
	}


	testingScene.predictObjectClassesOnlySOF(arrageFeaturesTest, normalizationOption);

	vector<vector<double> > votingTable;
	testingScene.voting(arrageFeaturesTest, normalizationOption, votingTable);




  return 0;
}
