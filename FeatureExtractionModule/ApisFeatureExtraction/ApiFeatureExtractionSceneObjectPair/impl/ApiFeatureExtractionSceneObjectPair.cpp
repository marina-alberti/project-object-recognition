/*
 * ApiFeatureExtractionSceneObjectPair.cpp
 *
 *  Created on: Nov 22, 2013
 *      Author: marina
 */

#include "ApiFeatureExtractionSceneObjectPair.hpp"

#define DEBUG 1

void ApiFeatureExtractionSceneObjectPair::extract(SceneInformation & scene, SceneObjectPairFeature & out) {

	vector<Object> objectList = scene.getObjectList();

	// for each object in the scene as the reference
	for (vector<Object>::iterator it = objectList.begin(); it != objectList.end(); it++ ) {

		// Object referenceObject = *it;

		// for each other object in the scene as target
		for(vector<Object>::iterator it2 = objectList.begin(); it2 != objectList.end(); ++it2) {

			 if (it2 != it) {  // TODO REMOVE

				// Object targetObject = *it2;

				ObjectPairFeature currentObjectFeature;
				ApiFeatureExtractionObjectPair fe;
				fe.extractFeatures( *it, *it2, currentObjectFeature);
				out.addObjectPairFeature(currentObjectFeature);
			 }
		}

    }
}


/*
vector<Object> objectList = inputScene.getObjectList();

  // for each object in the scene as reference
  for(vector<Object>::iterator it = objectList.begin(); it != objectList.end(); ++it) {
    Object referenceObject = *it;

    if (DEBUG) {
      cout << endl << "Adding the features of a new REFERENCE object. " << endl;
    }

    // for each other object in the scene as target
    for(vector<Object>::iterator it2 = objectList.begin(); it2 != objectList.end(); ++it2) {
      if (it2 != it) {
        Object targetObject = *it2;

        if (DEBUG) {
          cout << endl << "Adding the features of a new TARGET object. ";
          cout << "Pair is : " << referenceObject.getObjectName() << ",  " << targetObject.getObjectName() << endl;
        }

        ObjectPairFeatures pairObjectFeatureExtraction(referenceObject, targetObject);
        pairObjectFeatureExtraction.extractFeatures();
        vector<FeatureInformation> _features = pairObjectFeatureExtraction.getAllFeatures();

        if (DEBUG) {
          cout << "The size of the features of the current object pair is : "
               << _features.size() << endl;
        }

        int objectID1 = (*it).getActualObjectID();
        int objectID2 = (*it2).getActualObjectID();
        inputScene.addAllFeatPairObject( _features, objectID1, objectID2);

      }
    }

  }
  */
