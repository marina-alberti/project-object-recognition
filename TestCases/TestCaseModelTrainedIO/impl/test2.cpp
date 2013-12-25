/*
 * test2.cpp
 *
 *  Created on: Dec 6, 2013
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

	string dir = "./data/data_more_objects/";
	vector<string> listXMLfiles =  storeFileNames(dir);
	DatabaseInformation db;
	db.loadAnnotations_KTH(listXMLfiles);

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
	int normalizationOption = 1;
	Training doTraining;
	doTraining.learnGMMSingleObjectFeature(FMSingleObject, nclusters, normalizationOption);
	doTraining.learnGMMObjectPairFeature(FMObjectPair, nclusters, normalizationOption);

	string folder = "params";
	ModelTrainedIO::storeTrainingToFile(doTraining, folder);

	/*

	ModelTrainedIO::storeMeanNormalizationSingleObjectFile(doTraining, folder);
	ModelTrainedIO::storeStdNormalizationSingleObjectFile(doTraining, folder);
	ModelTrainedIO::storeMinFeatSingleObjectFile(doTraining, folder);
	ModelTrainedIO::storeMaxFeatSingleObjectFile(doTraining, folder);
	ModelTrainedIO::storeMeanNormalizationObjectPairFile(doTraining, folder);
	ModelTrainedIO::storeStdNormalizationObjectPairFile(doTraining, folder);
	ModelTrainedIO::storeMinFeatObjectPairFile(doTraining, folder);
	ModelTrainedIO::storeMaxFeatObjectPairFile(doTraining, folder);
	ModelTrainedIO::storeMeansSingleObjectFile(doTraining, folder);
	ModelTrainedIO::storeWeightsSingleObjectFile(doTraining, folder);
	ModelTrainedIO::storeCovsSingleObjectFile(doTraining, folder);
	ModelTrainedIO::storeMeansObjectPairFile(doTraining, folder);
	ModelTrainedIO::storeWeightsObjectPairFile(doTraining, folder);
	ModelTrainedIO::storeCovsObjectPairFile(doTraining, folder);
*/

	// compute object frequencies and co-occurrence

	vector<double> frequenciesSingleObject = ApiStatisticsDatabase::computeFrequenciesSingleObject(db);
	vector<vector<double> > frequenciesObjectPair = ApiStatisticsDatabase::computeFrequenciesObjectPair(db);


	// ******************************************************************************************************
	// Test


	Test testingScene;
	string paramsfolder = "params";

	cout << "going to load the files " << endl;

	/*

	ModelTrainedIO::loadMeanNormalizationSingleObjectFile(paramsfolder, testingScene);
	ModelTrainedIO::loadStdNormalizationSingleObjectFile(paramsfolder, testingScene);
	ModelTrainedIO::loadMinFeatSingleObjectFile(paramsfolder, testingScene);
	ModelTrainedIO::loadMaxFeatSingleObjectFile(paramsfolder, testingScene);
	ModelTrainedIO::loadMeanNormalizationObjectPairFile(paramsfolder, testingScene);
	ModelTrainedIO::loadStdNormalizationObjectPairFile(paramsfolder, testingScene);
	ModelTrainedIO::loadMinFeatObjectPairFile(paramsfolder, testingScene);
	ModelTrainedIO::loadMaxFeatObjectPairFile(paramsfolder, testingScene);

	ModelTrainedIO::loadMeansSingleObjectFile(paramsfolder, testingScene);
	ModelTrainedIO::loadWeightsSingleObjectFile(paramsfolder, testingScene);
	ModelTrainedIO::loadCovsSingleObjectFile(paramsfolder, testingScene);
	ModelTrainedIO::loadMeansObjectPairFile(paramsfolder, testingScene);
	ModelTrainedIO::loadWeightsObjectPairFile(paramsfolder, testingScene);
	ModelTrainedIO::loadCovsObjectPairFile(paramsfolder, testingScene);
	*/

	ModelTrainedIO::loadTrainedGMMsFile(paramsfolder, testingScene);

	// testingScene.printmeanNormalizationSingleObject();
	// testingScene.printmeanNormalizationObjectPair();



	return 0;

}


