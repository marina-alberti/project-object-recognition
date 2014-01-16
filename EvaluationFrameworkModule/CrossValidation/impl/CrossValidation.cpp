/*
 * CrossValidation.cpp
 *
 *  Created on: Dec 19, 2013
 *      Author: marina
 */


#include "CrossValidation.hpp"

#define TESTFLAG 1
#define DEBUG 0
#define NUMBEROFCATEGORIES 7


void CrossValidation::computeLOOCrossValidationReal(string dir) {

	// string dir = "/home/marina/workspace_eclipse_scene_object_classification/data/data_more_objects/";
	vector<string> listXMLfiles =  storeFileNames(dir);
	DatabaseInformation db(NUMBEROFCATEGORIES);
	db.loadAnnotations_KTH(listXMLfiles);

	vector<SceneInformation> allScenes = db.getSceneList();

	int numberOfFolds = db.getNumberOfScenes();

	ConfusionMatrix crossValidationCMatrix;


	// the number of cross validation folds is equal to the number of files/scenes

	// for each cross validation fold
	for (int ifold = 0; ifold < numberOfFolds; ifold++) {   //// numberOfFolds

		vector<SceneInformation> trainingScenes;
		vector<SceneInformation> testScenes;

		testScenes.push_back(allScenes.at(ifold));
		for (int i = 0 ; i < numberOfFolds; i++) {
			if (i != ifold) {
				trainingScenes.push_back(allScenes.at(i));
			}
		}
		DatabaseInformation trainingDB(trainingScenes, NUMBEROFCATEGORIES);

		cout << "the size of the database is: " << trainingDB.getNumberOfScenes() << endl;
		// trainingDB.printSceneInformation();

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

		// ArrangeFeatureTraining::printFeatureMatrixSingleObject(FMSingleObject);
		// ArrangeFeatureTraining::printFeatureMatrixObjectPair(FMObjectPair);

		// Learning
		//cout << "cross 0" << endl;

		int nclusters = 2;
		int normalizationOption = 0;
		Training doTraining;
		doTraining.learnGMMSingleObjectFeature(FMSingleObject, nclusters, normalizationOption);
		doTraining.learnGMMObjectPairFeature(FMObjectPair, nclusters, normalizationOption);

		// compute object frequencies and co-occurrence frequency on training database

		//cout << "cross 1" << endl;

		vector<double> frequenciesSingleObject = ApiStatisticsDatabase::computeFrequenciesSingleObject(trainingDB);
		vector<vector<double> > frequenciesObjectPair = ApiStatisticsDatabase::computeFrequenciesObjectPair(trainingDB);

		// storing to file

		string storingFolder = "params";
		ModelTrainedIO::storeTrainingToFile(doTraining, storingFolder);
		ModelTrainedIO::storefrequencies(frequenciesSingleObject, frequenciesObjectPair, storingFolder);

		// Test

		ConfusionMatrix totalCMatrix;

		// for each test scene among the test scenes
		for (int i = 0; i < testScenes.size(); i++) {

			SceneInformation testScene = testScenes.at(i);

			vector<int> objectids = testScene.getObjectIds();
			vector<int> categoryList;

			int numberOfObject = testScene.getNumberOfObjects();

			// // ADDED to consider only test scenes having limited number of unknown test objects
			/*
			if (numberOfObject > 6) {
				break;
			}
			*/

			for (int k = 0; k < NUMBEROFCATEGORIES; k++) { // TODO: changeback
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

			// // option tag for loading trained models from the files

			bool loadfromfile = true;
			if (loadfromfile) {
				ModelTrainedIO::loadTrainedGMMsFile(storingFolder, testingScene);
				ModelTrainedIO::loadfrequencies(storingFolder, testingScene);
			}
			else {
				// // loading directly from the saved models into the training class - no use of the files
				testingScene.loadTrainedGMMs(doTraining);
				testingScene.loadLearnedObjectCategoryFrequency(frequenciesSingleObject, frequenciesObjectPair);
			}


			int optionTestFunction = 1;
			path resultsPath;

			// // only "single object features"
			if (optionTestFunction == 0) {
				resultsPath = testingScene.predictObjectClassesOnlySOF(arrangeFeaturesTest, normalizationOption);
			}

			// // Voting Scheme
			if (optionTestFunction == 1) {
				// prepare the input for the voting strategy
				vector<vector<double> > votingTable;
				resultsPath = testingScene.voting(arrangeFeaturesTest, normalizationOption, votingTable);
			}

			// // Exhaustive search
			if (optionTestFunction == 2) {
				resultsPath = testingScene.exhaustiveSearch(arrangeFeaturesTest, normalizationOption, allPaths);
			}

			// // Voting scheme + successive greedy optimization
			// //  (start from best node, + shortlist of possible object category labels)
			if (optionTestFunction == 3) {
				// prepare the input for the voting strategy
				vector<vector<double> > votingTable;
				path resultVoting = testingScene.voting(arrangeFeaturesTest, normalizationOption, votingTable);
				vector<vector<pairScore> > votingTableComplete = Test::createVotingTableComplete(votingTable, arrangeFeaturesTest);
				resultsPath = testingScene.optimizationGreedy(arrangeFeaturesTest, votingTableComplete, normalizationOption);
			}

			// // Voting scheme + successive exhaustive search
			if (optionTestFunction == 4) {
				vector<vector<double> > votingTable;
				path resultVoting = testingScene.voting(arrangeFeaturesTest, normalizationOption, votingTable);
				vector<vector<pairScore> > votingTableComplete = Test::createVotingTableComplete(votingTable, arrangeFeaturesTest);
				resultsPath = testingScene.exhaustiveSearchAfterVoting(arrangeFeaturesTest, votingTableComplete, normalizationOption);
			}

			// Only Optimization search with a start from the best node indicated by SOF likelihood!
			if (optionTestFunction == 5) {
				path sofPath = testingScene.predictObjectClassesOnlySOF(arrangeFeaturesTest, normalizationOption);
				vector<vector< pairScore> > votingTable = testingScene.prepareVotingTableOptimizationSOFbasedScores(arrangeFeaturesTest, normalizationOption, categoryList);
				resultsPath = testingScene.optimizationGreedy(arrangeFeaturesTest, votingTable, normalizationOption);
			}

			ConfusionMatrix cMatrix;
			ApiConvertionResultsTestConfusionMatrix::convertResultsToMatrix(resultsPath, testScene, cMatrix, categoryList);
			// cMatrix.printConfusionMatrix();
			totalCMatrix.sumConfusionMatrix(cMatrix);
		}

		totalCMatrix.printConfusionMatrix();
		//Evaluation * evaluate;
		//evaluate = new Evaluation(totalCMatrix);
		//evaluate->evaluatePerformance();

		crossValidationCMatrix.sumConfusionMatrix(totalCMatrix);

	}
	crossValidationCMatrix.printConfusionMatrix();
	Evaluation * evaluateCrossValidation;
	evaluateCrossValidation = new Evaluation(crossValidationCMatrix);
	evaluateCrossValidation->evaluatePerformance();


}




void CrossValidation::computeLOOCrossValidationSimulation(string dir) {

	DatabaseInformation db(NUMBEROFCATEGORIES);
	db.loadAnnotations_Simulation(dir);

	vector<SceneInformation> allScenes = db.getSceneList();

	int numberOfFolds = 50; // db.getNumberOfScenes();

	ConfusionMatrix crossValidationCMatrix;


	// the number of cross validation folds is equal to the number of files/scenes

	// for each cross validation fold
	for (int ifold = 0; ifold < numberOfFolds; ifold++) {   //// numberOfFolds

		vector<SceneInformation> trainingScenes;
		vector<SceneInformation> testScenes;

		testScenes.push_back(allScenes.at(ifold));
		for (int i = 0 ; i < numberOfFolds; i++) {
			if (i != ifold) {
				trainingScenes.push_back(allScenes.at(i));
			}
		}
		DatabaseInformation trainingDB(trainingScenes, NUMBEROFCATEGORIES);

		cout << "the size of the database is: " << trainingDB.getNumberOfScenes() << endl;
		// trainingDB.printSceneInformation();

		// feature extraction

		DatabaseSingleObjectFeature dbSof;
		ApiFeatureExtractionDatabaseSingleObject::extract(trainingDB, dbSof);
		DatabaseObjectPairFeature dbOpf;
		ApiFeatureExtractionDatabaseObjectPair::extract(trainingDB, dbOpf);

		cout << "after fe " << endl;
		// arrange the features

		vector<vector<vector<float> > > FMSingleObject;
		ArrangeFeatureTraining::setFeatureMatrixSingleObject(dbSof, FMSingleObject);
		vector<vector<vector<vector<float> > > > FMObjectPair;
		ArrangeFeatureTraining::setFeatureMatrixObjectPair(dbOpf, FMObjectPair);

		cout << "after ArrangeFeatureTraining " << endl;


		// ArrangeFeatureTraining::printFeatureMatrixSingleObject(FMSingleObject);
		// ArrangeFeatureTraining::printFeatureMatrixObjectPair(FMObjectPair);

		// Learning
		//cout << "cross 0" << endl;

		int nclusters = 2;
		int normalizationOption = 0;
		Training doTraining(NUMBEROFCATEGORIES);
		doTraining.learnGMMSingleObjectFeature(FMSingleObject, nclusters, normalizationOption);

		cout << "after Training 1" << endl;


		doTraining.learnGMMObjectPairFeature(FMObjectPair, nclusters, normalizationOption);

		cout << "after Training " << endl;



		// compute object frequencies and co-occurrence frequency on training database

		//cout << "cross 1" << endl;

		vector<double> frequenciesSingleObject = ApiStatisticsDatabase::computeFrequenciesSingleObject(trainingDB);
		vector<vector<double> > frequenciesObjectPair = ApiStatisticsDatabase::computeFrequenciesObjectPair(trainingDB);

		// storing to file

		cout << "after compute freq " << endl;


		string storingFolder = "params";
		ModelTrainedIO::storeTrainingToFile(doTraining, storingFolder);

		cout << "after storing 1" << endl;


		ModelTrainedIO::storefrequencies(frequenciesSingleObject, frequenciesObjectPair, storingFolder);

		// Test

		cout << "after storing " << endl;


		ConfusionMatrix totalCMatrix;

		// for each test scene among the test scenes
		for (int i = 0; i < testScenes.size(); i++) {

			SceneInformation testScene = testScenes.at(i);

			vector<int> objectids = testScene.getObjectIds();
			vector<int> categoryList;

			int numberOfObject = testScene.getNumberOfObjects();

			// // ADDED to consider only test scenes having limited number of unknown test objects
			/*
			if (numberOfObject > 6) {
				break;
			}
			*/

			for (int k = 0; k < NUMBEROFCATEGORIES; k++) { // TODO: changeback
				categoryList.push_back(k);
			}


			ApiGraph mygraph(objectids, categoryList);
			// mygraph.findAllPaths();
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

			// // option tag for loading trained models from the files

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


			int optionTestFunction = 5;
			path resultsPath;

			// // only "single object features"
			if (optionTestFunction == 0) {
				resultsPath = testingScene.predictObjectClassesOnlySOF(arrangeFeaturesTest, normalizationOption);
			}

			// // Voting Scheme
			if (optionTestFunction == 1) {
				// prepare the input for the voting strategy
				vector<vector<double> > votingTable;
				resultsPath = testingScene.voting(arrangeFeaturesTest, normalizationOption, votingTable);
			}

			// // Exhaustive search
			if (optionTestFunction == 2) {
				resultsPath = testingScene.exhaustiveSearch(arrangeFeaturesTest, normalizationOption, allPaths);
			}

			// // Voting scheme + successive greedy optimization
			// //  (start from best node, + shortlist of possible object category labels)
			if (optionTestFunction == 3) {
				// prepare the input for the voting strategy
				vector<vector<double> > votingTable;
				path resultVoting = testingScene.voting(arrangeFeaturesTest, normalizationOption, votingTable);
				vector<vector<pairScore> > votingTableComplete = Test::createVotingTableComplete(votingTable, arrangeFeaturesTest);
				resultsPath = testingScene.optimizationGreedy(arrangeFeaturesTest, votingTableComplete, normalizationOption);
			}

			// // Voting scheme + successive exhaustive search
			if (optionTestFunction == 4) {
				vector<vector<double> > votingTable;
				path resultVoting = testingScene.voting(arrangeFeaturesTest, normalizationOption, votingTable);
				vector<vector<pairScore> > votingTableComplete = Test::createVotingTableComplete(votingTable, arrangeFeaturesTest);
				resultsPath = testingScene.exhaustiveSearchAfterVoting(arrangeFeaturesTest, votingTableComplete, normalizationOption);
			}

			// Only Optimization search with a start from the best node indicated by SOF likelihood!
			if (optionTestFunction == 5) {
				path sofPath = testingScene.predictObjectClassesOnlySOF(arrangeFeaturesTest, normalizationOption);
				vector<vector< pairScore> > votingTable = testingScene.prepareVotingTableOptimizationSOFbasedScores(arrangeFeaturesTest, normalizationOption, categoryList);
				resultsPath = testingScene.optimizationGreedy(arrangeFeaturesTest, votingTable, normalizationOption);
			}

			ConfusionMatrix cMatrix;
			ApiConvertionResultsTestConfusionMatrix::convertResultsToMatrix(resultsPath, testScene, cMatrix, categoryList);
			// cMatrix.printConfusionMatrix();
			totalCMatrix.sumConfusionMatrix(cMatrix);
		}

		totalCMatrix.printConfusionMatrix();
		//Evaluation * evaluate;
		//evaluate = new Evaluation(totalCMatrix);
		//evaluate->evaluatePerformance();

		crossValidationCMatrix.sumConfusionMatrix(totalCMatrix);

	}
	crossValidationCMatrix.printConfusionMatrix();
	Evaluation * evaluateCrossValidation;
	evaluateCrossValidation = new Evaluation(crossValidationCMatrix);
	evaluateCrossValidation->evaluatePerformance();

}



