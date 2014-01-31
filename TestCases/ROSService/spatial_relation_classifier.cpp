/*
 * object_classification_service.cpp
 *
 *  Created on: Jan 10, 2014
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
#include "ApiConvertServiceFormat.hpp"

#include "ros/ros.h"
#include "strands_qsr_msgs/GetGroupClassification.h"
#include "strands_qsr_msgs/BBox.h"
#include "strands_qsr_msgs/ObjectClassification.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/Quaternion.h"
#include "geometry_msgs/Pose.h"



#define DEBUG 1

using namespace std;

bool handle_group_estimate(strands_qsr_msgs::GetGroupClassification::Request  & req,
         strands_qsr_msgs::GetGroupClassification::Response & res) {

	SceneInformation testScene;

	// extract the fields from the ROS service request and stores them into c++ data structures
	vector<string> objectInstanceNameList = req.object_id;
	vector<strands_qsr_msgs::BBox> bboxListInput = req.bbox;
	vector<geometry_msgs::Pose> poseListInput = req.pose;

	// convert the Pose field of the request
	vector<pcl::PointXYZ> poseList;
	for(int i = 0; i < poseListInput.size(); i++) {
		pcl::PointXYZ point;
		point.x = poseListInput.at(i).position.x;
		point.y = poseListInput.at(i).position.y;
		point.z = poseListInput.at(i).position.z;
		poseList.push_back(point);
	}

	// convert the bbox field of the request
	vector<vector<pcl::PointXYZ> > bboxList;
	for (int i = 0; i < bboxList.size(); i++) {
		strands_qsr_msgs::BBox currentBbox = bboxListInput.at(i);

		vector<pcl::PointXYZ> currentBboxConverted;
		for (int j = 0; j < currentBbox.point.size(); j++) {
			pcl::PointXYZ point;
			point.x = currentBbox.point.at(j).x;
			point.y = currentBbox.point.at(j).y;
			point.z = currentBbox.point.at(j).z;

			currentBboxConverted.push_back(point);
		}
		bboxList.push_back(currentBboxConverted);
	}

	vector<string> categoryListString = req.type;
	vector<strands_qsr_msgs::ObjectClassification> classificationListInupt = req.group_classification;
	map<string, mapCategoryConfidence> msgMap = Test::convertObjectClassificationMsgToIDS(classificationListInupt);


	vector<int> categoryListInt = convertStringToIntCategoryLabelVector(categoryListString);

	ApiConvertServiceFormat::parseScene(objectInstanceNameList, bboxList, poseList, testScene);

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

	string storingFolder = "params";
	ModelTrainedIO::loadTrainedGMMsFile(storingFolder, testingScene);
	ModelTrainedIO::loadfrequencies(storingFolder, testingScene);

	path resultsPath;
	int optionTestFunction = 1;
	int normalizationOption = 0;

	if (optionTestFunction == 0) {

		resultsPath = testingScene.predictObjectClassesOnlySOF(arrangeFeaturesTest, normalizationOption);
	}

	if (optionTestFunction == 1) {

		// prepare the input for the voting strategy
		vector<vector<double> > votingTable;

		// resultsPath = testingScene.voting(arrangeFeaturesTest, normalizationOption, votingTable);

		vector<strands_qsr_msgs::ObjectClassification> results = testingScene.voting(arrangeFeaturesTest, normalizationOption, votingTable, categoryListInt, categoryListString, msgMap);

	}


	 // Compute probabilities for all modeled object categories + classify objects.
	  //vector<vector<double> > weights = unknownScene.predictObjectClasses();


	/*
	  vector<strands_qsr_msgs::GroupEstimate> estimate;

	  // for each object in the test scene
	  for ( int i = 0; i < object_id.size(); i++ ) {

	    strands_qsr_msgs::GroupEstimate currentObjectEstimate;
	    currentObjectEstimate.object_id = object_id.at(i);

	    ROS_INFO("Creating response for object_id: %s ", currentObjectEstimate.object_id.c_str());

	    // for each of the modeled object cateogories the test object is tested against
	    for ( int j = 0; j < type.size(); j++ ) {
	      double currentWeight = weights[i][j];
	      string currentType = type[j];

	      currentObjectEstimate.weight.push_back(currentWeight);
	      currentObjectEstimate.type.push_back(currentType);

	      ROS_INFO("The weight is: %f for object type: %s", currentWeight, currentType.c_str());
	    }

	    estimate.push_back(currentObjectEstimate);
	  }

	  res.estimate = estimate;
	  */

	  return true;

}


int main(int argc, char **argv) {
	/*

  // the last parameter is a string containing the name of the ROS node
  ros::init(argc, argv, "spatial_relation_classifier_server");
  ros::NodeHandle n;
  ros::ServiceServer service = n.advertiseService("spatial_relation_classifier", handle_group_estimate);
  ROS_INFO("Ready to estimate the test scene");
  ros::spin();
  */
  return 0;
}





/*

int main() {


		string dir = "/home/marina/workspace_eclipse_scene_object_classification/data/data_more_objects/";

		vector<string> listXMLfiles =  storeFileNames(dir);
		DatabaseInformation db;
		db.loadAnnotations_KTH(listXMLfiles);

		vector<SceneInformation> allScenes = db.getSceneList();
		vector<SceneInformation> trainingScenes;
		// vector<SceneInformation> testScenes;

		for (int i = 0 ; i < 35; i++) {
			trainingScenes.push_back(allScenes.at(i));
		}
		DatabaseInformation trainingDB(trainingScenes);




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


		// for each test scene among the test scenes


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

			// vector<strands_qsr_msgs::ObjectClassification> estimate ;

			int optionTestFunction = 1;
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


	return 0;
}


*/

