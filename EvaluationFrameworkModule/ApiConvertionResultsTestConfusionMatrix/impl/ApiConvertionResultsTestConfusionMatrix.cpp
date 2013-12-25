/*
 * ApiConvertionResultsTestConfusionMatrix.cpp
 *
 *  Created on: Dec 19, 2013
 *      Author: marina
 */

#include "ApiConvertionResultsTestConfusionMatrix.hpp"


/*
 * For each object instance in the scene
 *  = element of the vector of Object
 *  get the objectInstnaceID
 *  get the corresponding actual categoryLabel
 *  retrieve from the map path the predicted categoryLabe (based on the key = objectInstanceID)
 *
 */
void ApiConvertionResultsTestConfusionMatrix::convertResultsToMatrix(path myPath, SceneInformation scene, ConfusionMatrix & cMatrix, vector<int> categoryList) {

	//cMatrix = new ConfusionMatrix(categoryList);

	cMatrix.setConfusionMatrix(categoryList);

	// cMatrix = cv::Mat::zeros(size, size + 1, CV_32S);

	vector<Object> objectList = scene.getObjectList();

	for (vector<Object>::iterator it = objectList.begin(); it != objectList.end(); ++it) {

		int objectInstanceId = (*it).getInstanceID();
		int categoryLabelActual = (*it).getActualObjectID();
		int categoryLabelPredicted = myPath[objectInstanceId];

		if (categoryLabelPredicted != -1) {

			cMatrix.incrementConfusionMatrix(categoryLabelActual, categoryLabelPredicted);

		    // cMatrix.at<int>(categoryLabelActual, categoryLabelPredicted) ++;
		}

		//else {
		//  cMatrix.at<int>(categoryLabelActual, (size)) ++;
		//}


	}

}
