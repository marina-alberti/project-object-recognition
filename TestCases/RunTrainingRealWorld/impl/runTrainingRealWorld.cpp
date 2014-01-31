/*
 * RunTrainingRealWorld.cpp
 *
 *  Created on: Jan 22, 2014
 *      Author: marina
 */



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
#include "ApiConvertRealWorldDB.hpp"

#include "ros/ros.h"
#include "strands_qsr_msgs/GetGroupClassification.h"
#include "strands_qsr_msgs/BBox.h"
#include "strands_qsr_msgs/ObjectClassification.h"


using namespace std;

int main() {

		int numberOfCategories = 11;

		//string dir = "/home/marina/workspace_eclipse_scene_object_classification/data/data_more_objects/";
		// string dir = "/home/marina/workspace_eclipse_scene_object_classification/data/data_simulation/simulation/bham_office_desk_500_modifiedroot.json";
		// string dir = "/home/marina/workspace_eclipse_scene_object_classification/data/data_simulation/simulation/after2m.json";


		//string dir = "/home/marina/catkin_strands_qsr_ws/src/strands_qsr/data/real-world/131107_All_Scenes_mod.json";
		 string dir = "/home/marina/catkin_strands_qsr_ws/src/strands_qsr/data/real-world/131110_All_Scenes_mod.json";
		// string dir = "/home/marina/catkin_strands_qsr_ws/src/strands_qsr/data/real-world/Two_Days_All_Scenes_mod.json";

	/*
		DatabaseInformation db(numberOfCategories);
		db.loadAnnotations_Simulation(dir);
	*/

		cout << "Start Training " << endl;

		DatabaseInformation dbfirst(numberOfCategories);

		cout << "Start Training before load annotations " << endl;

		dbfirst.loadAnnotations_RealWorld(dir);

		cout << "Start Training after load annotations" << endl;

		vector<SceneInformation> allScenes = dbfirst.getSceneList();

		int numberOfFolds = dbfirst.getNumberOfScenes();
		vector<SceneInformation> trainingScenes;
		for (int i = 0 ; i < numberOfFolds; i++) {

			trainingScenes.push_back(allScenes.at(i));
		}
		DatabaseInformation db(trainingScenes, numberOfCategories);


		cout << "feature extraction" << endl;

		// feature extraction

		DatabaseSingleObjectFeature dbSof;

		ApiFeatureExtractionDatabaseSingleObject::extract(db, dbSof);
		cout << "feature extraction 1" << endl;
		DatabaseObjectPairFeature dbOpf;
		ApiFeatureExtractionDatabaseObjectPair::extract(db, dbOpf);

		cout << "feature arrange" << endl;


		// arrange the features

		vector<vector<vector<float> > > FMSingleObject;
		ArrangeFeatureTraining::setFeatureMatrixSingleObject(dbSof, FMSingleObject);
		vector<vector<vector<vector<float> > > > FMObjectPair;
		ArrangeFeatureTraining::setFeatureMatrixObjectPair(dbOpf, FMObjectPair);


		// ArrangeFeatureTraining::printFeatureMatrixSingleObject(FMSingleObject);
		// ArrangeFeatureTraining::printFeatureMatrixObjectPair(FMObjectPair);

		// Learning

		int nclusters = 2;
		int normalizationOption = 0;

		Training doTraining;
		doTraining.learnGMMSingleObjectFeature(FMSingleObject, nclusters, normalizationOption);
		doTraining.learnGMMObjectPairFeature(FMObjectPair, nclusters, normalizationOption);   // ERROR ONLY IN ONE FILE !!!

		cout << "Compute frequencies" << endl;
		// compute object frequencies and co-occurrence frequency on training database

		vector<double> frequenciesSingleObject = ApiStatisticsDatabase::computeFrequenciesSingleObject(db);

		cout << "Compute frequencies pair" << endl;
		vector<vector<double> > frequenciesObjectPair = ApiStatisticsDatabase::computeFrequenciesObjectPair(db);

		// storing to file

		string storingFolder = "/home/marina/catkin_strands_qsr_ws/src/strands_qsr/spatial_relation_classifier/src/paramsRealWorld";
		// string storingFolder = "src/paramsRealWorld";

		cout << "store training " << endl;
		ModelTrainedIO::storeTrainingToFile(doTraining, storingFolder);
		cout << "store frequencies" << endl;
		ModelTrainedIO::storefrequencies(frequenciesSingleObject, frequenciesObjectPair, storingFolder);



		return 0;
}

