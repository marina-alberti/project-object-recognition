/*
 * ConfusionMatrix.cpp
 *
 *  Created on: Dec 19, 2013
 *      Author: marina
 */


#include "ConfusionMatrix.hpp"
#include <vector>

#define TESTFLAG 1

ConfusionMatrix::ConfusionMatrix() {

	size = 0;
}

void ConfusionMatrix::setConfusionMatrix(vector<int> categoryList) {

	// the size of the confusion matrix should be the maximum category label
	size = computeMaximumValue(categoryList) +1;

	if (TESTFLAG) {
		cout << "setConfusionMatrix: size is =  " << size << endl;
	}

	// cMatrix = cv::Mat::zeros(size, size + 1, CV_32S);  // TODO change size
	cMatrix = cv::Mat::zeros(size, size, CV_32S);  // TODO change size

	if (TESTFLAG) {
		cout << "setConfusionMatrix: size computed is ==  " << cMatrix.rows << "   " << cMatrix.cols << endl;
	}

}


void ConfusionMatrix::printConfusionMatrix() {

	cout << "The confusion matrix is: " << endl;
	cout << cMatrix << endl;

}

void ConfusionMatrix::incrementConfusionMatrix(int categoryLabelActual, int categoryLabelPredicted) {

	if (categoryLabelActual < size && categoryLabelPredicted < size) {
		if (categoryLabelActual >= 0) {
			cMatrix.at<int>(categoryLabelActual, categoryLabelPredicted) ++;

			if (TESTFLAG) {
				cout << "setConfusionMatrix: size computed is =  " << cMatrix.rows << "   " << cMatrix.cols <<endl;
			}
		}
		else {
			if (TESTFLAG) {
				cout << "The actual label is another category not modeled!" << endl;
			}
		}
	}
	else {
		cout << "Error: the indexes of the categories are bigger than the size of the confusion matrix" << endl;
		cout << "Categories: " << categoryLabelActual << "   " << categoryLabelPredicted << endl;
	}



}

void ConfusionMatrix::sumConfusionMatrix(ConfusionMatrix in) {

	if (TESTFLAG) {
		cout << "In : ConfusionMatrix::sumConfusionMatrix " << endl;
	}

	if (in.getConfusionMatrixSize() == size) {
		cMatrix = cMatrix + (in.getConfusionMatrix()).clone();
	}
	else if (size == 0) {
		cMatrix = (in.getConfusionMatrix()).clone();
		size = in.getConfusionMatrixSize();
	}
	else {
		cout << "Error: the size of the 2 confusion matrices is not same " << endl;
	}

}
