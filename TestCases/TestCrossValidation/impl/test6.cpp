/*
 * test6.cpp
 *
 *  Created on: Dec 19, 2013
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
#include "CrossValidation.hpp"


#define DEBUG 1



using namespace std;

int main() {

	string dir = "/home/marina/workspace_eclipse_scene_object_classification/data/data_more_objects_folds/";
	// // // // CrossValidation::computeLOOCrossValidationReal(dir);
	CrossValidation::computeLOOCrossValidationRealFolds(dir);


	//string filename = "/home/marina/workspace_eclipse_scene_object_classification/data/data_simulation/simulation/after2m.json";
	// CrossValidation::computeLOOCrossValidationSimulation(filename);


	// string dir = "/home/marina/catkin_strands_qsr_ws/src/strands_qsr/data/real-world/131107_All_Scenes_mod.json";
	// string dir = "/home/marina/catkin_strands_qsr_ws/src/strands_qsr/data/real-world/131110_All_Scenes_mod.json";
	// string dir = "/home/marina/catkin_strands_qsr_ws/src/strands_qsr/data/real-world/Two_Days_All_Scenes_mod.json";
   //  string dir = "/home/marina/workspace_eclipse_scene_object_classification/data/real-world/Three_Days.json";
    //string dir = "/home/marina/workspace_eclipse_scene_object_classification/data/real-world/Five_Days.json";


    // // // CrossValidation::computeLOOCrossValidationRealWorld(dir);
	//CrossValidation::computeLOOCrossValidationRealWorldFolds(dir);

	return 0;

}




