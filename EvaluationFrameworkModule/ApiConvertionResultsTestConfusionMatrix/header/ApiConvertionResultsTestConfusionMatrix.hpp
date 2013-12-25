/*
 * ApiConvertionResultsTestConfusionMatrix.hpp
 *
 *  Created on: Dec 19, 2013
 *      Author: marina
 */

#ifndef APICONVERTIONRESULTSTESTCONFUSIONMATRIX_HPP_
#define APICONVERTIONRESULTSTESTCONFUSIONMATRIX_HPP_

/*
INPUT:
 - the output of the “Test” class = the “resultsPath” mapping
   the fields  (objectInstanceId, categoryLabel);
 - the “SceneInformation” that contains a list of “Object” objects
   and for each of these I will have the field: “objectInstanceId” and actual category ID
OUTPUT:
 - confusion matrix

 The interface class associates to each object (based on recognizing that
 the objectInstanceId is same) the predicted object class.

*/

#include "Test.hpp"
#include "SceneInformation.hpp"
#include "Object.hpp"
#include "ConfusionMatrix.hpp"
#include <vector>


class ApiConvertionResultsTestConfusionMatrix {

private:

public:

	static void convertResultsToMatrix(path myPath, SceneInformation scene, ConfusionMatrix & cMatrix, vector<int> categoryList);



};





#endif /* APICONVERTIONRESULTSTESTCONFUSIONMATRIX_HPP_ */
