/*
 * Evaluation.cpp
 *
 *  Created on: Dec 19, 2013
 *      Author: marina
 */

#include "Evaluation.hpp"

#define TESTFLAG 1
#define DEBUG 0


Evaluation::Evaluation (ConfusionMatrix in) {

	cMatrix = (in.getConfusionMatrix()).clone();

}


void Evaluation::evaluatePerformance() {

  // for each object class
  vector<double> precision;
  vector<double> recall;
  vector<double> fmeasure;

  for (int i = 0; i < cMatrix.rows; i++ )  {

    double _precision;
    double _recall;
    int TP = 0;
    int FP = 0;
    int FN = 0;

//    for (int j = 0; j < cMatrix.rows; j++ )

    TP +=  cMatrix.at<int>(i, i);
    for (int j = 0; j < cMatrix.cols; j++ ) {
      if ( j != i) {
        FN += cMatrix.at<int>(i, j);

      }
    }
    for (int j = 0; j < cMatrix.rows; j++ ) {
      if ( j != i) {
        FP += cMatrix.at<int>(j, i);
      }
    }


    if ((TP + FP) == 0) {
      _precision = 0;
    }
    else {
      _precision = ((double)TP) /(double)(TP + FP);
    }
    if ((TP + FN) == 0) {
      _recall = 0;
    }
    else {
      _recall = (double)TP / (double)(TP + FN);
    }
    double fm;
    if (_precision == 0 && _recall == 0) {
      fm = 0;
    }
    else {
      fm = 2 * _precision * _recall / (_precision + _recall );
    }
    precision.push_back(_precision);
    recall.push_back(_recall);
    fmeasure.push_back(fm);

    bool print = true;
    if (print) {
    	cout << "Object Category Label " << i << endl ;
    	cout << "Precision: " << _precision << endl;
    	cout << "recall:    " << _recall << endl ;
    	cout << "Fmeasure:  " << fm << endl;

    }

  }

}


