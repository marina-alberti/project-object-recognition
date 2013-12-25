/*
 * Evaluation.hpp
 *
 *  Created on: Dec 19, 2013
 *      Author: marina
 */

#ifndef EVALUATION_HPP_
#define EVALUATION_HPP_

#include "ConfusionMatrix.hpp"

/*
INPUT:
confusion matrix
OUTPUT:
performance scores
*/

class Evaluation {

private:

	cv::Mat cMatrix;


public:


	Evaluation(ConfusionMatrix);
	void evaluatePerformance();


};



#endif /* EVALUATION_HPP_ */
