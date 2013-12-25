/*
 * ConfusionMatrix.hpp
 *
 *  Created on: Dec 19, 2013
 *      Author: marina
 */

#ifndef CONFUSIONMATRIX_HPP_
#define CONFUSIONMATRIX_HPP_

#include <vector>
#include "Test.hpp"
#include <opencv2/opencv.hpp>

class ConfusionMatrix {

private:

	cv::Mat cMatrix;
	int size;


public:

	ConfusionMatrix();
	void setConfusionMatrix(vector<int>);
	cv::Mat getConfusionMatrix() { return cMatrix; }
	int getConfusionMatrixSize() { return size; }
	void printConfusionMatrix();
	void sumConfusionMatrix(ConfusionMatrix);
	void incrementConfusionMatrix(int, int);

};



#endif /* CONFUSIONMATRIX_HPP_ */
