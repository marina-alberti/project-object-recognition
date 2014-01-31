/*
 * CrossValidation.hpp
 *
 *  Created on: Dec 19, 2013
 *      Author: marina
 */

#ifndef CROSSVALIDATION_HPP_
#define CROSSVALIDATION_HPP_

#include "DatabaseInformation.hpp"
#include "Test.hpp"
#include "ConfusionMatrix.hpp"
#include "SceneInformation.hpp"
#include "ApiConvertKTHDB.hpp"
#include "DatabaseInformation.hpp"
#include "ApiFeatureExtractionDatabaseSingleObject.hpp"
#include "ApiFeatureExtractionDatabaseObjectPair.hpp"
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
#include "time.h"



class CrossValidation {

private:

	// int databaseType;


public:

	static void computeLOOCrossValidationReal(string);

	static void computeLOOCrossValidationRealFolds(string);
	static void computeLOOCrossValidationSimulation(string dir);
	static void computeLOOCrossValidationRealWorld(string dir);

	static void do_crossValidation(vector<int> trainingFolderIds, int testFolderId) ;
	static void computeLOOCrossValidationRealWorldFolds(string dir);


};



#endif /* CROSSVALIDATION_HPP_ */
