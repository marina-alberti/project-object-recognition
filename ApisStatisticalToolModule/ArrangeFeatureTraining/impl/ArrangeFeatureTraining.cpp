/*
 * ArrangeFeatureTrianing.cpp
 *
 *  Created on: Nov 23, 2013
 *      Author: marina
 */

#include "ArrangeFeatureTraining.hpp"

#define TESTFLAG 0
#define DEBUG 0

void ArrangeFeatureTraining::setFeatureMatrixSingleObject(DatabaseSingleObjectFeature & featuredb, vector<vector<vector<float> > > & FMSingleObject ) {

	int numberOfCategories = featuredb.getNumberOfCategories();


	  if (DEBUG) {
	    cout << "In setFeatureMatrixSingleObject: reserved space in the matrix = NOBJECTCLASSES." << endl;
	  }

	  // iterate over set of object class IDs, predefined
	  for (int i = 0; i < numberOfCategories ; i++) {

	    if (DEBUG) {
	      cout << "Object class ID: i =  " << i << endl;
	    }

	    vector<SceneSingleObjectFeature> listSceneSingleObjectFeature =  featuredb.getListSceneSingleObjectFeature();
	    int countScene = 0;
	    int nScene = listSceneSingleObjectFeature.size();
	    vector<vector<float> > currentSceneSingleObjectFeature;

	    // iterate over all scenes in the database
	    for(vector<SceneSingleObjectFeature>::iterator it = listSceneSingleObjectFeature.begin(); it != listSceneSingleObjectFeature.end(); ++it) {

	      vector<SingleObjectFeature> allFeatureCurrentObject = (*it).getListSingleObjectFeature();

	      // iterate over all sets of features (from different objects) in the current scene
	      for(vector<SingleObjectFeature>::iterator it2 = allFeatureCurrentObject.begin(); it2 != allFeatureCurrentObject.end(); ++it2) {

	        int currentID  = (*it2).getObjectID();

	        // if the set of features is from the currently considered object class
	        if ( currentID == i) {

	          vector<float> currentSingleObjectFeature = (*it2).getAllFeatures();
	          currentSceneSingleObjectFeature.push_back(currentSingleObjectFeature);
	          if (DEBUG) {
	        	    cout << "In setFeatureMatrixSingleObject: added set of features " << endl;
	        	    cout << "size is : " << currentSingleObjectFeature.size() << endl;
			  }

	        }
	      }
	      countScene++;
	    }
	    FMSingleObject.push_back(currentSceneSingleObjectFeature);
	  }

}

void ArrangeFeatureTraining::setFeatureMatrixObjectPair(DatabaseObjectPairFeature & featuredb, vector<vector<vector<vector<float> > > > & FMPairObject) {


	int numberOfCategories = featuredb.getNumberOfCategories();

	  // iterate over set of object class IDs, predefined
	  for (int i = 0; i < numberOfCategories ; i++) {

	    vector<vector<vector<float > > > vectorFeaturesObject1;

	    // iterate over set of object class IDs, predefined
	    for (int j = 0; j < numberOfCategories ; j++) {

	      //if (i != j) {  // to check TODO

	        if (DEBUG) {
	          cout << "In setFeatureMatrix : object class IDs: i =  " << i << " and j = " << j << endl;
	        }

	        vector<vector<float> > vectorFeaturesObject2;

	        vector<SceneObjectPairFeature> listSceneObjectPairFeature = featuredb.getListSceneObjectPairFeature();

	        int countScene = 0;

	        // iterate over all scenes
	        for(vector<SceneObjectPairFeature>::iterator it = listSceneObjectPairFeature.begin(); it != listSceneObjectPairFeature.end(); ++it) {


	        	vector<ObjectPairFeature> listObjectPairFeature = (*it).getListObjectPairFeature();

	        	// iterate over all sets of features (from different object pairs) in the current scene
	        	for(vector<ObjectPairFeature>::iterator it2 = listObjectPairFeature.begin(); it2 != listObjectPairFeature.end(); ++it2) {

	        		// if the current set of features is from the considered object pair classes
	        		if ( ((*it2).getObjectID1() == i ) && ( (*it2).getObjectID2() == j ) ) {

	        			vector<float> featList = (*it2).getAllFeatures();
	        			vectorFeaturesObject2.push_back(featList);
	        		}
	        	}
	        	countScene++;
	        }
	        vectorFeaturesObject1.push_back(vectorFeaturesObject2);

	    }
	    FMPairObject.push_back(vectorFeaturesObject1);
	  }
}


void ArrangeFeatureTraining::printFeatureMatrixSingleObject(vector<vector<vector<float> > > & FMSingleObject) {

	for (int i = 0; i < FMSingleObject.size(); i++) {

		cout << "Category  " << i << endl;

		for (int j = 0; j < FMSingleObject.at(i).size(); j++) {

			for (int z = 0; z < FMSingleObject.at(i).at(j).size(); z++) {

				cout << FMSingleObject.at(i).at(j).at(z) << "   " ;
			}
			cout << endl;

		}
		//cout << endl << endl << endl;
	}

}


void ArrangeFeatureTraining::printFeatureMatrixObjectPair(vector<vector<vector<vector<float> > > > & FMPairObject) {

	for (int i = 0; i < FMPairObject.size(); i++) {

		for (int i2 = 0; i2 < FMPairObject.at(i).size(); i2++) {

			cout << "Object categories: " << i << "   and " << i2 << endl;
			for (int j = 0; j < FMPairObject.at(i).at(i2).size(); j++) {

				for (int z = 0; z < FMPairObject.at(i).at(i2).at(j).size(); z++) {

					cout << FMPairObject.at(i).at(i2).at(j).at(z) << "   " ;
				}
				cout << endl;
			}
			cout << endl << endl << endl;
		}
	}
}


