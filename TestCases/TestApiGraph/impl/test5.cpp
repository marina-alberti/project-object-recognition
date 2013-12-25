/*
 * test5.cpp
 *
 *  Created on: Dec 13, 2013
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
#include "ApiGraph.hpp"
#include "ConfusionMatrix.hpp"
#include "ApiConvertionResultsTestConfusionMatrix.hpp"
#include "Evaluation.hpp"


#define DEBUG 1

/*
http://stackoverflow.com/questions/3722704/c-read-numbers-from-file-and-store-in-vectors
*/

using namespace std;

int main() {


	/*
	vector<int> objectids ; //= {1, 2, 3, 4};
	objectids.push_back(1);
	objectids.push_back(2);
	objectids.push_back(3);

	vector<int> categories ; //= {30, 40};
	categories.push_back(1);
	categories.push_back(2);

	ApiGraph graphs(objectids, categories);

	graphs.findAllPaths();
	graphs.printAllPaths();
	*/


		string dir = "/home/marina/workspace_eclipse_scene_object_classification/data/data_more_objects/";

		vector<string> listXMLfiles =  storeFileNames(dir);
		DatabaseInformation db;
		db.loadAnnotations_KTH(listXMLfiles);

		vector<SceneInformation> allScenes = db.getSceneList();
		vector<SceneInformation> trainingScenes;
		vector<SceneInformation> testScenes;

		for (int i = 0 ; i < 35; i++) {
			trainingScenes.push_back(allScenes.at(i));
		}
		DatabaseInformation trainingDB(trainingScenes);

		for (int i = 35 ; i < 42; i++) {
			testScenes.push_back(allScenes.at(i));
		}

		cout << "the size of the database is: " << db.getNumberOfScenes() << endl;
		db.printSceneInformation();


		// feature extraction

		DatabaseSingleObjectFeature dbSof;
		ApiFeatureExtractionDatabaseSingleObject::extract(trainingDB, dbSof);
		DatabaseObjectPairFeature dbOpf;
		ApiFeatureExtractionDatabaseObjectPair::extract(trainingDB, dbOpf);

		// arrange the features

		vector<vector<vector<float> > > FMSingleObject;
		ArrangeFeatureTraining::setFeatureMatrixSingleObject(dbSof, FMSingleObject);
		vector<vector<vector<vector<float> > > > FMObjectPair;
		ArrangeFeatureTraining::setFeatureMatrixObjectPair(dbOpf, FMObjectPair);

		// print
		//cout << "size of feature matrix is: " <<  FMSingleObject.size() << endl;
		//cout << "size of feature matrix dim 2 is: " <<  FMSingleObject.at(0).size() << endl;
		//cout << "size of feature matrix dim 3 is: " << FMSingleObject.at(0).at(0).size() << endl;

		ArrangeFeatureTraining::printFeatureMatrixSingleObject(FMSingleObject);
		ArrangeFeatureTraining::printFeatureMatrixObjectPair(FMObjectPair);

		// Learning

		int nclusters = 2;
		int normalizationOption = 0;
		Training doTraining;
		doTraining.learnGMMSingleObjectFeature(FMSingleObject, nclusters, normalizationOption);
		doTraining.learnGMMObjectPairFeature(FMObjectPair, nclusters, normalizationOption);

		// compute object frequencies and co-occurrence frequency on training database

		vector<double> frequenciesSingleObject = ApiStatisticsDatabase::computeFrequenciesSingleObject(trainingDB);
		vector<vector<double> > frequenciesObjectPair = ApiStatisticsDatabase::computeFrequenciesObjectPair(trainingDB);

		// storing to file

		string storingFolder = "params";
		ModelTrainedIO::storeTrainingToFile(doTraining, storingFolder);
		ModelTrainedIO::storefrequencies(frequenciesSingleObject, frequenciesObjectPair, storingFolder);


		// ******************************************************************************************************
		// Test

		//string testFile = listXMLfiles.at(0);
		//ApiConvertKTHDB testSceneConverter;

		ConfusionMatrix totalCMatrix;

		// for each test scene among the test scenes
		for (int i = 0; i < testScenes.size(); i++) {

			SceneInformation testScene = testScenes.at(i);

			vector<int> objectids = testScene.getObjectIds();
			vector<int> categoryList;

			for (int k = 0; k < NOBJECTCLASSES; k++) {
				categoryList.push_back(k);
			}

			ApiGraph mygraph(objectids, categoryList);
			mygraph.findAllPaths();
			// mygraph.printAllPaths();
			vector<path> allPaths = mygraph.getAllPaths();


			// // feature extraction

			SceneSingleObjectFeature sceneSof;
			SceneObjectPairFeature sceneOpf;
			ApiFeatureExtractionSceneSingleObject::extract(testScene, sceneSof);
			ApiFeatureExtractionSceneObjectPair::extract(testScene, sceneOpf);

			// // Arrange features of test scene

			ArrangeFeatureTestScene arrangeFeaturesTest;
			arrangeFeaturesTest.arrangeTestFeatures(sceneSof, sceneOpf);

			// // testing

			Test testingScene;

			// // option tag for loading from the files

			bool loadfromfile = false;
			if (loadfromfile) {
				ModelTrainedIO::loadTrainedGMMsFile(storingFolder, testingScene);
				ModelTrainedIO::loadfrequencies(storingFolder, testingScene);
			}
			else {
				// // loading directly from the saved models into the training class - no use of the files
				testingScene.loadTrainedGMMs(doTraining);
				testingScene.loadLearnedObjectCategoryFrequency(frequenciesSingleObject, frequenciesObjectPair);
			}

			int optionTestFunction;
			path resultsPath;

			if (optionTestFunction == 0) {

				resultsPath = testingScene.predictObjectClassesOnlySOF(arrangeFeaturesTest, normalizationOption);
			}

			if (optionTestFunction == 1) {

				// prepare the input for the voting strategy
				vector<vector<double> > votingTable;

				resultsPath = testingScene.voting(arrangeFeaturesTest, normalizationOption, votingTable);
			}

			if (optionTestFunction == 2) {

				resultsPath = testingScene.exhaustiveSearch(arrangeFeaturesTest, normalizationOption, allPaths);
			}

			if (optionTestFunction == 3) {

				// prepare the input for the voting strategy
				vector<vector<double> > votingTable;

				path resultVoting = testingScene.voting(arrangeFeaturesTest, normalizationOption, votingTable);

				vector<vector<pairScore> > votingTableComplete = Test::createVotingTableComplete(votingTable, arrangeFeaturesTest);

				resultsPath = testingScene.optimizationGreedy(arrangeFeaturesTest, votingTableComplete, normalizationOption);

			}

		    ConfusionMatrix cMatrix;
		    ApiConvertionResultsTestConfusionMatrix::convertResultsToMatrix(resultsPath, testScene, cMatrix, categoryList);
		    cMatrix.printConfusionMatrix();
		    totalCMatrix.sumConfusionMatrix(cMatrix);

		}

		totalCMatrix.printConfusionMatrix();
		Evaluation * evaluate;
		evaluate = new Evaluation(totalCMatrix);
		evaluate->evaluatePerformance();

	return 0;
}
