/*
 * runTraining.cpp
 *
 *  Created on: Jan 15, 2014
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

#include "ros/ros.h"
#include "strands_qsr_msgs/GetGroupClassification.h"
#include "strands_qsr_msgs/BBox.h"
#include "strands_qsr_msgs/ObjectClassification.h"


using namespace std;

int main() {

		string dir = "/home/marina/workspace_eclipse_scene_object_classification/data/data_more_objects/";

		//vector<string> listXMLfiles =  storeFileNames(dir);
		DatabaseInformation db;
		db.loadAnnotations_Simulation(dir);

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


		ArrangeFeatureTraining::printFeatureMatrixSingleObject(FMSingleObject);
		ArrangeFeatureTraining::printFeatureMatrixObjectPair(FMObjectPair);

		// Learning

		int nclusters = 2;
		int normalizationOption = 0;

		Training doTraining;
		doTraining.learnGMMSingleObjectFeature(FMSingleObject, nclusters, normalizationOption);
		doTraining.learnGMMObjectPairFeature(FMObjectPair, nclusters, normalizationOption);

		// compute object frequencies and co-occurrence frequency on training database

		vector<double> frequenciesSingleObject = ApiStatisticsDatabase::computeFrequenciesSingleObject(db);
		vector<vector<double> > frequenciesObjectPair = ApiStatisticsDatabase::computeFrequenciesObjectPair(db);

		// storing to file

		string storingFolder = "params";
		ModelTrainedIO::storeTrainingToFile(doTraining, storingFolder);
		ModelTrainedIO::storefrequencies(frequenciesSingleObject, frequenciesObjectPair, storingFolder);

		return 0;
}

