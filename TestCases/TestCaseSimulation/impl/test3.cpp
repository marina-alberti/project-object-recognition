/*
 * test3.cpp
 *
 *  Created on: Dec 9, 2013
 *      Author: marina
 */


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

using namespace std;

int main() {

	// convert annotation in XML files into IDS

	//string filename = "/home/marina/workspace_eclipse_scene_object_classification/data/data_simulation/simulation/bham_office_desk_500_modifiedroot.json";
	string filename = "/home/marina/workspace_eclipse_scene_object_classification/data/data_simulation/simulation/after2m.json";

	DatabaseInformation db;
	db.loadAnnotations_Simulation(filename);

	cout << "the size of the database is: " << db.getNumberOfScenes() << endl;
	db.printSceneInformation();


	// feature extraction

	DatabaseSingleObjectFeature dbSof;
	cout << "EXTRACT SOF" <<endl;

	ApiFeatureExtractionDatabaseSingleObject::extract(db, dbSof);
	DatabaseObjectPairFeature dbOpf;

	cout << "extract opf " << endl;
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
	// ArrangeFeatureTraining::printFeatureMatrixObjectPair(FMObjectPair);

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


	// // testing


	// ApiConvertKTHDB testSceneConverter;
	SceneInformation testScene = (db.getSceneList()).at(0);

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


	// // loading from the files: options
	bool loadfromfile = true;

	if (loadfromfile) {
		ModelTrainedIO::loadTrainedGMMsFile(paramsfolder, testingScene);
		ModelTrainedIO::loadfrequencies(paramsfolder, testingScene);
	}
	else {
		// // loading directly from the saved models into the training class - no use of the files
		testingScene.loadTrainedGMMs(doTraining);
		testingScene.loadLearnedObjectCategoryFrequency(frequenciesSingleObject, frequenciesObjectPair);
	}

	cout << "Before voting " << endl;

	testingScene.predictObjectClassesOnlySOF(arrageFeaturesTest, normalizationOption);

	vector<vector<double> > votingTable;
	testingScene.voting(arrageFeaturesTest, normalizationOption, votingTable);

	return 0;
}




