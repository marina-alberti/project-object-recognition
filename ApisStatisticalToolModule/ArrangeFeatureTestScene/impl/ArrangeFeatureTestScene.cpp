/*
 * ArrangeFeatureTestScene.cpp
 *
 *  Created on: Nov 25, 2013
 *      Author: marina
 */

#include "ArrangeFeatureTestScene.hpp"

#define DEBUG 0


void ArrangeFeatureTestScene::arrangeTestFeatures(SceneSingleObjectFeature & sof, SceneObjectPairFeature & opf) {

	listSOF = sof.getListSingleObjectFeature();
	listOPF = opf.getListObjectPairFeature();

	// get the number of test objects Z

	int nTestObject = listSOF.size();

	// create two indexes to loop over the new matrix
	int i = 0;
	int j = 0;

	// create the index to loop over the elements in the vector of object pair features
	int countVector = 0;

	// create a ZxZ matrix of object pair features
	//vector<vector<ObjectPairFeature> > matrixOPF;
	//matrixOPF.reserve(nTestObject);

	if (DEBUG) {
	cout << "nTestObject = "  << nTestObject << endl;

	}

	for (int i = 0; i < nTestObject; i++) {

		vector<ObjectPairFeature> tmp;
		//tmp.reserve(nTestObject);
		for (int j = 0; j < nTestObject; j++) {

			if (i != j) {
				tmp.push_back(listOPF.at(countVector));
				countVector++;
			}
			else {

				ObjectPairFeature empty;
				tmp.push_back(empty);
			}

		}

		matrixOPF.push_back(tmp);
	}

}




