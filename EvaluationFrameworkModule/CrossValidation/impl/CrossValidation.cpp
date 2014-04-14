/*
 * CrossValidation.cpp
 *
 *  Created on: Dec 19, 2013
 *      Author: marina
 */


#include "CrossValidation.hpp"

#define TESTFLAG 0
#define DEBUG 0
#define NUMBEROFCATEGORIES 14 // // 6 14


void CrossValidation::computeLOOCrossValidationReal(string dir) {

	// string dir = "/home/marina/workspace_eclipse_scene_object_classification/data/data_more_objects/";

	int optionTestFunction = 0;

	vector<string> listXMLfiles =  storeFileNames(dir);
	DatabaseInformation db(NUMBEROFCATEGORIES);

	db.loadAnnotations_KTH(listXMLfiles);

	vector<SceneInformation> allScenes = db.getSceneList();
	int numberOfFolds = db.getNumberOfScenes();
	ConfusionMatrix crossValidationCMatrix;

	vector<double> timesTesting;
	vector<int> numberOfObjectsPerScene;

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
		cout << "Learn GMM single object features" << endl;
		doTraining.learnGMMSingleObjectFeature(FMSingleObject, nclusters, normalizationOption);
		cout << "Learn GMM object pair features" << endl;
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


			cout <<"Number of Objects::   " << numberOfObject << endl;
			numberOfObjectsPerScene.push_back(numberOfObject);

			/*
			if (numberOfObject > 99) {
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
			ApiFeatureExtractionSceneSingleObject::extractNoReference(testScene, sceneSof);
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


			cout << "after loading" << endl;

			path resultsPath;
			const clock_t begin_time = clock();


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

			// compute the time elapsed for the inference phase
			std::cout << "TIME::   " << double( clock () - begin_time ) /  CLOCKS_PER_SEC << endl << endl;
			double timeDifference = double( clock () - begin_time ) /  CLOCKS_PER_SEC ;
			timesTesting.push_back(timeDifference);

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



	double averageTimeElapsedTesting = computeAverage(timesTesting);
	cout << "Cross Validation: The average time for the inference phase is :: "  << averageTimeElapsedTesting << endl;

	double averageNumberOfObjects = computeAverage(numberOfObjectsPerScene);
	cout << "The average number of objects per scene is :: " << averageNumberOfObjects << endl;

	for (int l = 0; l < numberOfObjectsPerScene.size(); l++ ) {
		cout << "Number of Objects is:: " << numberOfObjectsPerScene.at(l) << endl;
		cout << "Time elapsed testing is:  " << timesTesting.at(l) << endl;
	}

}




void CrossValidation::computeLOOCrossValidationRealFolds(string dir) {

	int optionTestFunction = 0;    // // // // // 0: SOF, 2:Exhaustive, 5:optimization
	ConfusionMatrix crossValidationCMatrix;
	vector<double> timesTesting;
	vector<int> numberOfObjectsPerScene;

	static int listFolds[6] = {1, 2, 3, 4, 5, 6};
	int numberOfFolds = 6;

	// cross validation folds
	for (int iCount = 0 ; iCount < numberOfFolds; iCount++) {

	    int testFolderId = listFolds[iCount];
		vector<int> trainingFolderIds; // = new vector<int>;

		for (int jCount = 0; jCount < numberOfFolds; jCount++) {
			 if (iCount != jCount) {
				   trainingFolderIds.push_back(listFolds[jCount]) ;
			 }
		}
		// // do_crossValidation(trainingFolderIds, testFolderId);
		// **********************************************************************
		// convert the "int" values into names of directories

		string testFolderName = dir + convertInt(testFolderId) ;
		vector<string> trainingFolderNames;
		for (int j = 0; j < trainingFolderIds.size(); j++) {
			string currentTrainingFolderName = dir + convertInt(trainingFolderIds.at(j)) ;
			trainingFolderNames.push_back(currentTrainingFolderName);
		}

		cout << testFolderName << endl;

		//string filepath = dirname + "/" + ep->d_name;

		// **********************************************************************

		// Finds all the XML files into the directories

		vector<string> listXMLfilesTest = storeFileNames(testFolderName);

		//cout << "after storing test names" << endl;
		vector<string> listXMLfilesTraining;
		for (int j = 0; j < trainingFolderNames.size(); j++) {

				string currentTrainingFolderName = trainingFolderNames.at(j);
			//	cout << "before storing training names    " << j << "   " << currentTrainingFolderName<< endl;
				vector<string> currentlistXMLfilesTraining = storeFileNames(currentTrainingFolderName);
				for (int z = 0; z < currentlistXMLfilesTraining.size(); z++) {
					listXMLfilesTraining.push_back(currentlistXMLfilesTraining.at(z));
				}
			//  cout << "after storing training names" << j <<endl;
		}

		// **********************************************************************
		// Prints files to check

		for (int j = 0; j < listXMLfilesTest.size(); j ++) {
			cout << "test    "<< listXMLfilesTest.at(j) << endl;
		}
		for (int j = 0; j < listXMLfilesTraining.size(); j ++) {
			cout << "training    "<< listXMLfilesTraining.at(j) << endl;
		}
		cout << endl << endl;

		// **********************************************************************

		// prepare the training database
		DatabaseInformation trainingDB(NUMBEROFCATEGORIES);
		trainingDB.loadAnnotations_KTH(listXMLfilesTraining);

		// prepare the test scenes
		DatabaseInformation testDB(NUMBEROFCATEGORIES);
		testDB.loadAnnotations_KTH(listXMLfilesTest);
		vector<SceneInformation> testScenes = testDB.getSceneList();

		//*********************************************************************************

		cout << "the size of the database is: " << trainingDB.getNumberOfScenes() << endl;
			// trainingDB.printSceneInformation();

		// feature extraction

		DatabaseSingleObjectFeature dbSof;
		ApiFeatureExtractionDatabaseSingleObject::extract(trainingDB, dbSof);
		DatabaseObjectPairFeature dbOpf;
		ApiFeatureExtractionDatabaseObjectPair::extract(trainingDB, dbOpf);

		// arrange the features

		cout << "Arranging the features" << endl;

		vector<vector<vector<float> > > FMSingleObject;
		ArrangeFeatureTraining::setFeatureMatrixSingleObject(dbSof, FMSingleObject);
		vector<vector<vector<vector<float> > > > FMObjectPair;
		ArrangeFeatureTraining::setFeatureMatrixObjectPair(dbOpf, FMObjectPair);

		// ArrangeFeatureTraining::printFeatureMatrixSingleObject(FMSingleObject);
		// ArrangeFeatureTraining::printFeatureMatrixObjectPair(FMObjectPair);

		cout << "Learning" << endl;

		// Learning
		int nclusters = 2;
		int normalizationOption = 0;
		Training doTraining;

		cout << "Learn single object GMM" << endl;

		doTraining.learnGMMSingleObjectFeature(FMSingleObject, nclusters, normalizationOption);

		cout << "Learn object pair GMM" << endl;

		doTraining.learnGMMObjectPairFeature(FMObjectPair, nclusters, normalizationOption);

		// compute object frequencies and co-occurrence frequency on training database

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


			cout <<"Number of Objects::   " << numberOfObject << endl;
			numberOfObjectsPerScene.push_back(numberOfObject);

			//
			//if (numberOfObject > 99) {
			//	break;
			//}
			//


			for (int k = 0; k < NUMBEROFCATEGORIES; k++) { // TODO: changeback
				categoryList.push_back(k);
			}

			ApiGraph mygraph(objectids, categoryList);
			//mygraph.findAllPaths();   //////////// huge computational cost!!!
			// mygraph.printAllPaths();
			vector<path> allPaths = mygraph.getAllPaths();


			// // feature extraction

			SceneSingleObjectFeature sceneSof;
			SceneObjectPairFeature sceneOpf;
			ApiFeatureExtractionSceneSingleObject::extractNoReference(testScene, sceneSof);
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


			cout << "after loading" << endl;

			path resultsPath;
			const clock_t begin_time = clock();


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

			// compute the time elapsed for the inference phase
			std::cout << "TIME::   " << double( clock () - begin_time ) /  CLOCKS_PER_SEC << endl << endl;
			double timeDifference = double( clock () - begin_time ) /  CLOCKS_PER_SEC ;
			timesTesting.push_back(timeDifference);

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
		crossValidationCMatrix.printConfusionMatrix();




		//**************************************************************************************

	}

	crossValidationCMatrix.printConfusionMatrix();
	Evaluation * evaluateCrossValidation;
	evaluateCrossValidation = new Evaluation(crossValidationCMatrix);
	evaluateCrossValidation->evaluatePerformance();

	double averageTimeElapsedTesting = computeAverage(timesTesting);
	cout << "Cross Validation: The average time for the inference phase is :: "  << averageTimeElapsedTesting << endl;

	double averageNumberOfObjects = computeAverage(numberOfObjectsPerScene);
	cout << "The average number of objects per scene is :: " << averageNumberOfObjects << endl;

	for (int l = 0; l < numberOfObjectsPerScene.size(); l++ ) {
		cout << "Number of Objects is:: " << numberOfObjectsPerScene.at(l) << endl;
		cout << "Time elapsed testing is:  " << timesTesting.at(l) << endl;
	}

}

/*
void CrossValidation::do_crossValidation(vector<int> trainingFolderIds, int testFolderId) {

	int optionTestFunction = 2;

	vector<string> listXMLfiles =  storeFileNames(dir);
	DatabaseInformation db(NUMBEROFCATEGORIES);

	db.loadAnnotations_KTH(listXMLfiles);

	vector<SceneInformation> allScenes = db.getSceneList();
	int numberOfFolds = db.getNumberOfScenes();
	ConfusionMatrix crossValidationCMatrix;

	vector<double> timesTesting;
	vector<int> numberOfObjectsPerScene;

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


			cout <<"Number of Objects::   " << numberOfObject << endl;
			numberOfObjectsPerScene.push_back(numberOfObject);

			//
			//if (numberOfObject > 99) {
			//	break;
			//}
			//


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
			ApiFeatureExtractionSceneSingleObject::extractNoReference(testScene, sceneSof);
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


			cout << "after loading" << endl;

			path resultsPath;
			const clock_t begin_time = clock();


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

			// compute the time elapsed for the inference phase
			std::cout << "TIME::   " << double( clock () - begin_time ) /  CLOCKS_PER_SEC << endl << endl;
			double timeDifference = double( clock () - begin_time ) /  CLOCKS_PER_SEC ;
			timesTesting.push_back(timeDifference);

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



	double averageTimeElapsedTesting = computeAverage(timesTesting);
	cout << "Cross Validation: The average time for the inference phase is :: "  << averageTimeElapsedTesting << endl;

	double averageNumberOfObjects = computeAverage(numberOfObjectsPerScene);
	cout << "The average number of objects per scene is :: " << averageNumberOfObjects << endl;

	for (int l = 0; l < numberOfObjectsPerScene.size(); l++ ) {
		cout << "Number of Objects is:: " << numberOfObjectsPerScene.at(l) << endl;
		cout << "Time elapsed testing is:  " << timesTesting.at(l) << endl;
	}




}
*/



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



void CrossValidation::computeLOOCrossValidationRealWorld(string dir) {

	int optionTestFunction = 5;

	DatabaseInformation db(NUMBEROFCATEGORIES);
	db.loadAnnotations_RealWorld(dir);

	exit(1);

	vector<SceneInformation> allScenes = db.getSceneList();

	int numberOfFolds =  db.getNumberOfScenes();

	ConfusionMatrix crossValidationCMatrix;

	// // the number of cross validation folds is equal to the number of files/scenes

	vector<double> timesTesting;
	vector<int> numberOfObjectsPerScene;


	// // For each cross validation fold

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


		// // Feature extraction

		DatabaseSingleObjectFeature dbSof;
		ApiFeatureExtractionDatabaseSingleObject::extract(trainingDB, dbSof);
		DatabaseObjectPairFeature dbOpf;
		ApiFeatureExtractionDatabaseObjectPair::extract(trainingDB, dbOpf);


		// // Arrange the features

		vector<vector<vector<float> > > FMSingleObject;
		ArrangeFeatureTraining::setFeatureMatrixSingleObject(dbSof, FMSingleObject);
		vector<vector<vector<vector<float> > > > FMObjectPair;
		ArrangeFeatureTraining::setFeatureMatrixObjectPair(dbOpf, FMObjectPair);

		// ArrangeFeatureTraining::printFeatureMatrixSingleObject(FMSingleObject);
		// ArrangeFeatureTraining::printFeatureMatrixObjectPair(FMObjectPair);


		// // Learning

		int nclusters = 2;
		int normalizationOption = 0;
		Training doTraining(NUMBEROFCATEGORIES);
		doTraining.learnGMMSingleObjectFeature(FMSingleObject, nclusters, normalizationOption);

		doTraining.learnGMMObjectPairFeature(FMObjectPair, nclusters, normalizationOption);

		// compute object frequencies and co-occurrence frequency on training database

		//cout << "cross 1" << endl;

		vector<double> frequenciesSingleObject = ApiStatisticsDatabase::computeFrequenciesSingleObject(trainingDB);
		vector<vector<double> > frequenciesObjectPair = ApiStatisticsDatabase::computeFrequenciesObjectPair(trainingDB);

		// // Storing to file

		string storingFolder = "params";
		ModelTrainedIO::storeTrainingToFile(doTraining, storingFolder);
		ModelTrainedIO::storefrequencies(frequenciesSingleObject, frequenciesObjectPair, storingFolder);

		// // Test

		ConfusionMatrix totalCMatrix;

		// for each test scene among the test scenes
		for (int i = 0; i < testScenes.size(); i++) {


			SceneInformation testScene = testScenes.at(i);

			vector<int> objectids = testScene.getObjectIds();
			vector<int> categoryList;

			int numberOfObject = testScene.getNumberOfObjects();
			numberOfObjectsPerScene.push_back(numberOfObject);

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
			ApiFeatureExtractionSceneSingleObject::extractNoReference(testScene, sceneSof);
			ApiFeatureExtractionSceneObjectPair::extract(testScene, sceneOpf);

			// // Arrange features of test scene

			ArrangeFeatureTestScene arrangeFeaturesTest;
			arrangeFeaturesTest.arrangeTestFeatures(sceneSof, sceneOpf);

			// // testing

			cout << "start testing " << endl;

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


			path resultsPath;


			const clock_t begin_time = clock();

			// //  0: Only "single object features"
			if (optionTestFunction == 0) {
				resultsPath = testingScene.predictObjectClassesOnlySOF(arrangeFeaturesTest, normalizationOption);
			}

			// //  1: Voting Scheme

			if (optionTestFunction == 1) {
				// prepare the input for the voting strategy
				vector<vector<double> > votingTable;
				resultsPath = testingScene.voting(arrangeFeaturesTest, normalizationOption, votingTable);
			}

			// //  2: Exhaustive search

			if (optionTestFunction == 2) {
				resultsPath = testingScene.exhaustiveSearch(arrangeFeaturesTest, normalizationOption, allPaths);
			}

			// //  3: Voting scheme + successive greedy optimization
			// //  (start from best node, + shortlist of possible object category labels)

			if (optionTestFunction == 3) {
				// prepare the input for the voting strategy
				vector<vector<double> > votingTable;
				path resultVoting = testingScene.voting(arrangeFeaturesTest, normalizationOption, votingTable);
				vector<vector<pairScore> > votingTableComplete = Test::createVotingTableComplete(votingTable, arrangeFeaturesTest);
				resultsPath = testingScene.optimizationGreedy(arrangeFeaturesTest, votingTableComplete, normalizationOption);
			}

			// //  4: Voting scheme + successive exhaustive search

			if (optionTestFunction == 4) {
				vector<vector<double> > votingTable;
				path resultVoting = testingScene.voting(arrangeFeaturesTest, normalizationOption, votingTable);
				vector<vector<pairScore> > votingTableComplete = Test::createVotingTableComplete(votingTable, arrangeFeaturesTest);
				resultsPath = testingScene.exhaustiveSearchAfterVoting(arrangeFeaturesTest, votingTableComplete, normalizationOption);
			}

			// //  5: Only greedy optimization search with a start from the best node indicated by SOF likelihood

			if (optionTestFunction == 5) {

				path sofPath = testingScene.predictObjectClassesOnlySOF(arrangeFeaturesTest, normalizationOption);
				vector<vector< pairScore> > votingTable = testingScene.prepareVotingTableOptimizationSOFbasedScores(arrangeFeaturesTest, normalizationOption, categoryList);
				resultsPath = testingScene.optimizationGreedy(arrangeFeaturesTest, votingTable, normalizationOption);
			}


			// compute the time elapsed for the inference phase
			std::cout << "TIME::   " << double( clock () - begin_time ) /  CLOCKS_PER_SEC << endl << endl;
			double timeDifference = double( clock () - begin_time ) /  CLOCKS_PER_SEC ;
			timesTesting.push_back(timeDifference);


			ConfusionMatrix cMatrix;

			if (TESTFLAG) {
				cout << "Before converting results to confusion matrix" << endl;
			}

			ApiConvertionResultsTestConfusionMatrix::convertResultsToMatrix(resultsPath, testScene, cMatrix, categoryList);

			if (TESTFLAG) {
				cout << "After converting results to confusion matrix" << endl;
			}
			cMatrix.printConfusionMatrix();
			totalCMatrix.sumConfusionMatrix(cMatrix);

			if (TESTFLAG) {
				cout << "After : totalCMatrix.sumConfusionMatrix(cMatrix);" << i << endl << testScenes.size() << endl;
			}
		}

		if (TESTFLAG) {
			cout << "After all scenes of current fold" << endl;
		}

		totalCMatrix.printConfusionMatrix();
		//Evaluation * evaluate;
		//evaluate = new Evaluation(totalCMatrix);
		//evaluate->evaluatePerformance();

		crossValidationCMatrix.sumConfusionMatrix(totalCMatrix);

	}

	if (TESTFLAG) {
		cout << "After all iterations over the folds" << endl;
	}

	crossValidationCMatrix.printConfusionMatrix();
	Evaluation * evaluateCrossValidation;
	evaluateCrossValidation = new Evaluation(crossValidationCMatrix);
	evaluateCrossValidation->evaluatePerformance();

	double averageTimeElapsedTesting = computeAverage(timesTesting);
	cout << "Cross Validation: The average time for the inference phase is :: "  << averageTimeElapsedTesting << endl;

	double averageNumberOfObjects = computeAverage(numberOfObjectsPerScene);
	cout << "The average number of objects per scene is :: " << averageNumberOfObjects << endl;

	for (int l = 0; l < numberOfObjectsPerScene.size(); l++ ) {
		cout << "Number of Objects is:: " << numberOfObjectsPerScene.at(l) << endl;
		cout << "Time elapsed testing is:  " << timesTesting.at(l) << endl;
	}
}




void CrossValidation::computeLOOCrossValidationRealWorldFolds(string dir) {

	// // 5: Only greedy optimization search with a start from the best node indicated by SOF likelihood
	int optionTestFunction = 0;
	ConfusionMatrix crossValidationCMatrix;
	vector<double> timesTesting;
	vector<int> numberOfObjectsPerScene;

	DatabaseInformation db(NUMBEROFCATEGORIES);
	db.loadAnnotations_RealWorld(dir);
	vector<SceneInformation> allScenes = db.getSceneList();
	cout << "CrossValidation:: The total number of scenes is:: "  << allScenes.size() << endl << endl;

	 const char* sceneFoldNamesChar[] = {"Francisco", "Kaiyu", "Florian", "Puren", "Ali", "Rares", "Marina", "Akshaya", "Miro", "Yasemin", "Hossein", "Nils", "Carl", "Yuquan", "Petter", "Michele", "Oscar", "Magnus", "David", "Rasmus"};
	 std::vector<std::string> sceneFoldNames(sceneFoldNamesChar, sceneFoldNamesChar + 20);
	 int numberOfFolds =  20;


	 // ****************************************************************************************************
	 // // The vector of selected dates::
	 const char* selectedDatesChar[] = {"131023", "131024", "131028", "131029", "131030", "131031", "131101", "131106", "131107", "131110"};
	 std::vector<std::string> selectedDates(selectedDatesChar, selectedDatesChar + 10);


	 // // Selects only the scenes of the set of selected dates
	 vector<SceneInformation> allScenesSelectedDates;
	 for (int i = 0; i < allScenes.size(); i++) {
			SceneInformation currentSceneInformation = allScenes.at(i);
			const char* currentSceneDate = (currentSceneInformation.getSceneDateString()).c_str();

			if ( (strcmp(currentSceneDate, selectedDates.at(0).c_str()) == 0) ||  (strcmp(currentSceneDate, selectedDates.at(1).c_str()) == 0) || (strcmp(currentSceneDate, selectedDates.at(2).c_str()) == 0) || (strcmp(currentSceneDate, selectedDates.at(3).c_str()) == 0) || (strcmp(currentSceneDate, selectedDates.at(4).c_str()) == 0) || (strcmp(currentSceneDate, selectedDates.at(5).c_str()) == 0) || (strcmp(currentSceneDate, selectedDates.at(6).c_str()) == 0) || (strcmp(currentSceneDate, selectedDates.at(7).c_str()) == 0) || (strcmp(currentSceneDate, selectedDates.at(8).c_str()) == 0) || (strcmp(currentSceneDate, selectedDates.at(9).c_str()) == 0) ) {
				allScenesSelectedDates.push_back(currentSceneInformation);
			}
	 }
	cout << "CrossValidation:: The total number of SELECTED DATE scenes is:: "  << allScenesSelectedDates.size() << endl << endl;

	//exit(1);
	 // ****************************************************************************************************


	// // For each cross validation fold
	for (int ifold = 0; ifold < numberOfFolds; ifold++) {   //// numberOfFolds

		cout << sceneFoldNames.at(ifold) << endl;

		vector<SceneInformation> trainingScenes;
		vector<SceneInformation> testScenes;


		// // Creates the training dataset and the group of test scene for current cross validation fold
		string currentSceneFoldTestName = sceneFoldNames.at(ifold);

		// for each of the scenes
		for (int i = 0; i < allScenesSelectedDates.size(); i++) {

			SceneInformation currentSceneInformation = allScenesSelectedDates.at(i);
			if (strcmp( (currentSceneInformation.getSceneFold()).c_str(), (currentSceneFoldTestName).c_str() ) == 0) {
				testScenes.push_back(currentSceneInformation);
			}
			else {
				trainingScenes.push_back(currentSceneInformation);
			}
		}

		DatabaseInformation trainingDB(trainingScenes, NUMBEROFCATEGORIES);

		if (TESTFLAG) {
			cout << "the size of the database is: " << trainingDB.getNumberOfScenes() << endl;
		}
		// trainingDB.printSceneInformation();


		// // Feature extraction from training dataset

		DatabaseSingleObjectFeature dbSof;
		ApiFeatureExtractionDatabaseSingleObject::extract(trainingDB, dbSof);
		DatabaseObjectPairFeature dbOpf;
		ApiFeatureExtractionDatabaseObjectPair::extract(trainingDB, dbOpf);


		// // Arrange the features from training dataset

		vector<vector<vector<float> > > FMSingleObject;
		ArrangeFeatureTraining::setFeatureMatrixSingleObject(dbSof, FMSingleObject);
		vector<vector<vector<vector<float> > > > FMObjectPair;
		ArrangeFeatureTraining::setFeatureMatrixObjectPair(dbOpf, FMObjectPair);

		// // Prints features from training dataset

		// ArrangeFeatureTraining::printFeatureMatrixSingleObject(FMSingleObject);
		// ArrangeFeatureTraining::printFeatureMatrixObjectPair(FMObjectPair);

		//string fileNameFeatureSO = "/home/marina/Testing_PCA_Features_Matlab/featuresSO.txt";
		//string fileNameFeatuerOP = "/home/marina/Testing_PCA_Features_Matlab/featuresOP.txt";
		string fileNameFeatureSO = "/home/marina/API_Scene_Structure_from_DB_Matlab/featuresSO.txt";
		string fileNameFeatureOP = "/home/marina/API_Scene_Structure_from_DB_Matlab/featuresOP.txt";

		//ArrangeFeatureTraining::printFeatureSingleObjectToFile(FMSingleObject, fileNameFeatureSO);
		//ArrangeFeatureTraining::printFeatureObjectPairToFile(FMObjectPair, fileNameFeatureOP);

		//break;    // // TODO: remove!


		// // Learning (from training dataset)

		int nclusters = 2;
		int normalizationOption = 0;
		Training doTraining(NUMBEROFCATEGORIES);
		doTraining.learnGMMSingleObjectFeature(FMSingleObject, nclusters, normalizationOption);

		doTraining.learnGMMObjectPairFeature(FMObjectPair, nclusters, normalizationOption);

		// // Computes object frequencies and co-occurrence frequency on training database

		vector<double> frequenciesSingleObject = ApiStatisticsDatabase::computeFrequenciesSingleObject(trainingDB);
		vector<vector<double> > frequenciesObjectPair = ApiStatisticsDatabase::computeFrequenciesObjectPair(trainingDB);

		// // Storing to file

		string storingFolder = "params";
		ModelTrainedIO::storeTrainingToFile(doTraining, storingFolder);
		ModelTrainedIO::storefrequencies(frequenciesSingleObject, frequenciesObjectPair, storingFolder);


		// // Test

		ConfusionMatrix totalCMatrix;

		// for each test scene among the test scenes
		for (int i = 0; i < testScenes.size(); i++) {

			SceneInformation testScene = testScenes.at(i);

			vector<int> objectids = testScene.getObjectIds();
			vector<int> categoryList;

			int numberOfObject = testScene.getNumberOfObjects();
			numberOfObjectsPerScene.push_back(numberOfObject);

			for (int k = 0; k < NUMBEROFCATEGORIES; k++) {
				categoryList.push_back(k);
			}

			ApiGraph mygraph(objectids, categoryList);
			// mygraph.findAllPaths();
			// mygraph.printAllPaths();
			vector<path> allPaths = mygraph.getAllPaths();

			// // feature extraction

			SceneSingleObjectFeature sceneSof;
			SceneObjectPairFeature sceneOpf;
			ApiFeatureExtractionSceneSingleObject::extractNoReference(testScene, sceneSof);
			ApiFeatureExtractionSceneObjectPair::extract(testScene, sceneOpf);

			// // Arrange features of test scene

			ArrangeFeatureTestScene arrangeFeaturesTest;
			arrangeFeaturesTest.arrangeTestFeatures(sceneSof, sceneOpf);

			// // testing

			cout << "start testing " << endl;

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

			cout << "after load models " << endl;

			path resultsPath;


			const clock_t begin_time = clock();

			// //  0: Only "single object features"
			if (optionTestFunction == 0) {
				resultsPath = testingScene.predictObjectClassesOnlySOF(arrangeFeaturesTest, normalizationOption);
			}

			// //  1: Voting Scheme

			if (optionTestFunction == 1) {
				// prepare the input for the voting strategy
				vector<vector<double> > votingTable;
				resultsPath = testingScene.voting(arrangeFeaturesTest, normalizationOption, votingTable);
			}

			// //  2: Exhaustive search

			if (optionTestFunction == 2) {
				resultsPath = testingScene.exhaustiveSearch(arrangeFeaturesTest, normalizationOption, allPaths);
			}

			// //  3: Voting scheme + successive greedy optimization
			// //  (start from best node, + shortlist of possible object category labels)

			if (optionTestFunction == 3) {
				// prepare the input for the voting strategy
				vector<vector<double> > votingTable;
				path resultVoting = testingScene.voting(arrangeFeaturesTest, normalizationOption, votingTable);
				vector<vector<pairScore> > votingTableComplete = Test::createVotingTableComplete(votingTable, arrangeFeaturesTest);
				resultsPath = testingScene.optimizationGreedy(arrangeFeaturesTest, votingTableComplete, normalizationOption);
			}

			// //  4: Voting scheme + successive exhaustive search

			if (optionTestFunction == 4) {
				vector<vector<double> > votingTable;
				path resultVoting = testingScene.voting(arrangeFeaturesTest, normalizationOption, votingTable);
				vector<vector<pairScore> > votingTableComplete = Test::createVotingTableComplete(votingTable, arrangeFeaturesTest);
				resultsPath = testingScene.exhaustiveSearchAfterVoting(arrangeFeaturesTest, votingTableComplete, normalizationOption);
			}

			// //  5: Only greedy optimization search with a start from the best node indicated by SOF likelihood

			if (optionTestFunction == 5) {

				path sofPath = testingScene.predictObjectClassesOnlySOF(arrangeFeaturesTest, normalizationOption);
				vector<vector< pairScore> > votingTable = testingScene.prepareVotingTableOptimizationSOFbasedScores(arrangeFeaturesTest, normalizationOption, categoryList);
				resultsPath = testingScene.optimizationGreedy(arrangeFeaturesTest, votingTable, normalizationOption);
			}


			// compute the time elapsed for the inference phase
			std::cout << "TIME::   " << double( clock () - begin_time ) /  CLOCKS_PER_SEC << endl << endl;
			double timeDifference = double( clock () - begin_time ) /  CLOCKS_PER_SEC ;
			timesTesting.push_back(timeDifference);


			ConfusionMatrix cMatrix;

			if (TESTFLAG) {
				cout << "Before converting results to confusion matrix" << endl;
			}

			ApiConvertionResultsTestConfusionMatrix::convertResultsToMatrix(resultsPath, testScene, cMatrix, categoryList);

			if (TESTFLAG) {
				cout << "After converting results to confusion matrix" << endl;
			}
			cMatrix.printConfusionMatrix();
			totalCMatrix.sumConfusionMatrix(cMatrix);

			if (TESTFLAG) {
				cout << "After : totalCMatrix.sumConfusionMatrix(cMatrix);" << i << endl << testScenes.size() << endl;
			}
		}

		if (TESTFLAG) {
			cout << "After all scenes of current fold" << endl;
		}

		totalCMatrix.printConfusionMatrix();
		//Evaluation * evaluate;
		//evaluate = new Evaluation(totalCMatrix);
		//evaluate->evaluatePerformance();

		crossValidationCMatrix.sumConfusionMatrix(totalCMatrix);
		crossValidationCMatrix.printConfusionMatrix();


	}

	if (TESTFLAG) {
		cout << "After all iterations over the folds" << endl;
	}

	crossValidationCMatrix.printConfusionMatrix();
	Evaluation * evaluateCrossValidation;
	evaluateCrossValidation = new Evaluation(crossValidationCMatrix);
	evaluateCrossValidation->evaluatePerformance();

	double averageTimeElapsedTesting = computeAverage(timesTesting);
	cout << "Cross Validation: The average time for the inference phase is :: "  << averageTimeElapsedTesting << endl;

	double averageNumberOfObjects = computeAverage(numberOfObjectsPerScene);
	cout << "The average number of objects per scene is :: " << averageNumberOfObjects << endl;

	for (int l = 0; l < numberOfObjectsPerScene.size(); l++ ) {
		cout << "Number of Objects is:: " << numberOfObjectsPerScene.at(l) << endl;
		cout << "Time elapsed testing is:  " << timesTesting.at(l) << endl;
	}





}
