/*
 * ApiStatisticsDatabase.cpp
 *
 *  Created on: Nov 19, 2013
 *      Author: marina
 */

#include "ApiStatisticsDatabase.hpp"

#define TESTFLAG 0
#define DEBUG 0


vector<double> ApiStatisticsDatabase::computeFrequenciesSingleObject(DatabaseInformation & database) {


	int numberOfCategories = database.getNumberOfCategories();

	  if (TESTFLAG) {
	    cout << "Inside computeFrequenciesSingleObject() start" << endl;
	  }
	  vector<vector<int> > objectFrequencies;
	  objectFrequencies.reserve(numberOfCategories);

	  objectFrequencies = ApiStatisticsDatabase::arrangeFrequencyMatrix(database);

	// ********************************************************************************

	  if (TESTFLAG) {
	    cout << "Inside DatabaseInformation::computeObjectFrequencies() 1" << endl;
	  }

	  vector<int> countObjectFrequencies;

      // initializes the vector to zeros

	  countObjectFrequencies.reserve(numberOfCategories);
	  for (int i = 0; i < objectFrequencies.size(); i++) {
	    countObjectFrequencies.push_back(0);
	  }

	  // counts how many times 1 or more instances of the object category appear in the scene

	  for (int i = 0; i < objectFrequencies.size(); i++) {
	    for (int j = 0; j < objectFrequencies[i].size() ; j++) {
	      if (objectFrequencies[i][j] > 0) {
	        countObjectFrequencies[i] ++;
	      }
	    }
	  }

	  if (TESTFLAG) {
	    cout << "Inside DatabaseInformation::computeObjectFrequencies() 2" << endl;
	  }
	  vector<int> countObjectFrequencies1;

      // initializes the vector to zeros

	  countObjectFrequencies1.reserve(numberOfCategories);
	  for (int i = 0; i < objectFrequencies.size(); i++) {
	    countObjectFrequencies1.push_back(0);
	  }
	  // counts how many instances of the object category appear in the scenes (total number of instances)

	  for (int i = 0; i < objectFrequencies.size(); i++) {
	    for (int j = 0; j < objectFrequencies[i].size() ; j++) {
	      if (objectFrequencies[i][j] > 0) {
	        countObjectFrequencies1[i] += objectFrequencies[i][j] ;
	      }
	    }
	  }

	  if (TESTFLAG) {
	    cout << "Inside DatabaseInformation::computeObjectFrequencies() 3" << endl;
	  }
	  // divide by the number of scenes in the database to find the ratio
	  vector<SceneInformation> sceneList = database.getSceneList();
	  int nScenes = sceneList.size();

	  vector<double> freqratio;
	  // freqratio.reserve(countObjectFrequencies.size());
	  for (int i = 0; i < countObjectFrequencies.size(); i++ ) {
		  freqratio.push_back((double)(countObjectFrequencies.at(i)) / (double)nScenes);
	  }

	  if (TESTFLAG) {
	    cout << "Inside DatabaseInformation::computeObjectFrequencies() 4" << endl;
	  }
	  return freqratio;

}


vector<vector<double> > ApiStatisticsDatabase::computeFrequenciesObjectPair(DatabaseInformation & database) {

	int numberOfCategories = database.getNumberOfCategories();

	  if (TESTFLAG) {
	    cout << "Inside DatabaseInformation::computeFrequenciesObjectPair() start" << endl;
	  }

	vector<vector< int> > objectFrequencies;
	objectFrequencies.reserve(numberOfCategories);
	objectFrequencies = ApiStatisticsDatabase::arrangeFrequencyMatrix(database);

	vector<SceneInformation> sceneList = database.getSceneList();
	int nScenes = sceneList.size();
    // ********************************************************************************

	// The actual part of co-occurrence frequencies computation

	vector<vector<vector<int > > > objectPairFrequencies;
	objectPairFrequencies.reserve(numberOfCategories);

	// initialize matrix with zeros
	for (int i = 0; i < numberOfCategories; i++) {
		vector<vector<int> > frequenciesCurrentObjectRef;
		for (int j = 0; j < numberOfCategories; j++) {
			vector<int> frequenciesCurrentPair;
			for (int k = 0; k < nScenes; k++) {
				frequenciesCurrentPair.push_back(0);
			}
			frequenciesCurrentObjectRef.push_back(frequenciesCurrentPair);
		}
		objectPairFrequencies.push_back(frequenciesCurrentObjectRef);
	}

	// for every Scene in the database, indicate for each pair of objects
	// if they both appear (1) or not (0)
	for (int i = 0; i < nScenes; i++) {
		for (int j = 0; j < numberOfCategories; j++) {
			for (int k = 0; k < numberOfCategories; k++) {
				if (j != k) {
					if ( (objectFrequencies[j][i] > 0 ) && (objectFrequencies[k][i] > 0 )) {
						objectPairFrequencies[j][k][i] = 1;
						objectPairFrequencies[k][j][i] = 1;
					}
				}
				else{
					if ( (objectFrequencies[j][i] > 1 ) && (objectFrequencies[k][i] > 1 )) {
						objectPairFrequencies[j][k][i] = 1;
						objectPairFrequencies[k][j][i] = 1;
					}

				}
			}
		}
	}

    // ********************************************************************************

	vector<vector<int> > countObjectPairFrequencies;

	// count the co-occurrence frequencies of different pairs of obj.categories in the training dataset
	countObjectPairFrequencies.reserve(numberOfCategories);

	// Initialize to all values == 0
	for (int i = 0; i < objectPairFrequencies.size(); i++) {
		vector<int> tmp;
		for (int j = 0; j < numberOfCategories; j++) {
			tmp.push_back(0);
		}
		countObjectPairFrequencies.push_back(tmp);
	}
	// fills in the co-occurrence count iterating over all the scenes of the database
	for (int i = 0; i < objectPairFrequencies.size(); i++) {
		for (int j = 0; j < objectPairFrequencies[i].size() ; j++) {
			for (int k = 0; k < objectPairFrequencies[i][j].size(); k++) {
				if (objectPairFrequencies[i][j][k] > 0) {
					countObjectPairFrequencies[i][j] += 1;
				}
			}
		}
	}
	// prints the co-occurrence matrix
	if (TESTFLAG) {
		for (int i = 0; i < numberOfCategories; i++) {
			cout << endl;
			for (int j = 0; j < numberOfCategories; j++) {
				cout << countObjectPairFrequencies[i][j] << "     ";
			}
		}
	}

	  // divide by the number of scenes in the database to find the ratio

	  //vector<SceneInformation> sceneList = database.getSceneList();
	  //int nScenes = sceneList.size();

	  vector<vector<double> > freqratio;
	  freqratio.reserve(countObjectPairFrequencies.size());

	  for (int i = 0; i < countObjectPairFrequencies.size(); i++ ) {
		  vector<double> tmp2;
		  for (int j = 0; j < countObjectPairFrequencies.at(0).size(); j++) {
			  tmp2.push_back( ((double)countObjectPairFrequencies.at(i).at(j)) / ((double)nScenes));
		  }
		  freqratio.push_back(tmp2);
	  }
	  return freqratio;

}


vector<vector< int> > ApiStatisticsDatabase::arrangeFrequencyMatrix(DatabaseInformation & database) {

	int numberOfCategories = database.getNumberOfCategories();

	// objectFrequencies is a matrix where rows = object categories and columns = scenes of the database

	vector<vector< int> > objectFrequencies;
	objectFrequencies.reserve(numberOfCategories);
	vector<SceneInformation> sceneList = database.getSceneList();
	int nScenes = sceneList.size();

	// initialize matrix with zeros

	for (int i = 0; i < numberOfCategories; i++) {
		vector<int> frequenciesCurrentObject;
		for (int j = 0; j < nScenes; j++) {
			frequenciesCurrentObject.push_back(0);
		}
		objectFrequencies.push_back(frequenciesCurrentObject);
	}

	int countScene = 0;

	// for everyScene in the database
	for (vector<SceneInformation>::iterator it = sceneList.begin(); it != sceneList.end(); ++it) {
		vector<Object> currentSceneObjects = (*it).getObjectList();

		// for every object instance in the current scene
		for (vector<Object>::iterator it2 = currentSceneObjects.begin(); it2 != currentSceneObjects.end(); ++it2) {

			// get the object category of the considered object instance
			int currentObjectID = (*it2).getActualObjectID();

			if (currentObjectID != -1) {
				objectFrequencies[currentObjectID][countScene] += 1;
			}

		}
		countScene++;
	}

	if (DEBUG) {
		for (int i = 0; i < numberOfCategories; i++) {
			cout << endl;
			for (int j = 0; j < nScenes; j++) {
				cout << objectFrequencies[i][j] << "     ";
			}
		}
	}

	return objectFrequencies;

}


