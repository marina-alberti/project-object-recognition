/*
 * Training.hpp
 *
 *  Created on: Nov 23, 2013
 *      Author: marina
 */

#ifndef TRAINING_HPP_
#define TRAINING_HPP_


#include <string.h>
#include <boost/property_tree/ptree.hpp>
#include "Object.hpp"
#include "ApiConvertKTHDB.hpp"
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <cstring>
#include <dirent.h>
#include <cstdlib>
#include <sys/stat.h>
#include <sys/types.h>
#include <opencv2/opencv.hpp>
#include "opencv2/ml/ml.hpp"
#include <cmath>
#include "utils.hpp"
#include "ApiConvertSimulationDB.hpp"
#include "DatabaseSingleObjectFeature.hpp"
#include "StatisticalTool.hpp"
#include "DatabaseObjectPairFeature.hpp"

#define NOBJECTCLASSES 7

using namespace std;

/*
 * Input:
 * Feature Matrix Single Object
 * Feature Matrix Object Pair
 */

class Training {

private:

	int numberOfCategories;

	  /*************************************************************************************
	   * The normalization parameters
	  **************************************************************************************/

	  // SingleObject
	  vector<vector<double> > meanNormalizationSingleObject;
	  vector<vector<double> > stdNormalizationSingleObject;
	  vector<vector<double> > minFeatSingleObject;
	  vector<vector<double> > maxFeatSingleObject;

	  // ObjectPair
	  vector<vector<vector<double> > > meanNormalizationObjectPair;
	  vector<vector<vector<double> > > stdNormalizationObjectPair;
	  vector<vector<vector<double> > > minFeatObjectPair;
	  vector<vector<vector<double> > > maxFeatObjectPair;

	  /* *************************************************************************************
	  // The EM output parameters: means, co-variance matrices, and weights of the mixture components
	   * Note: OpenCV classes cv::Mat
      **************************************************************************************   */

	  // SingleObject
	  vector<cv::Mat> meansSingleObject;
	  vector<cv::Mat> weightsSingleObject;
	  vector< vector<cv::Mat> > covsSingleObject;

      // ObjectPair
	  vector<vector<cv::Mat> > meansObjectPair;
	  vector<vector<cv::Mat> > weightsObjectPair;
	  vector<vector<vector<cv::Mat> > > covsObjectPair;

	  vector<double> thresholdsSingleObject;


public:

	  Training(int = 7);

	  // Calls the learning method (SingleObject)
	  void learnGMMSingleObjectFeature(vector<vector<vector<float> > >  , int, int);

	  // Calls the learning method (ObjectPair)
	  void learnGMMObjectPairFeature(vector<vector<vector<vector<float> > > >  , int, int);

	  void setNumberOfCategories(int n) {numberOfCategories = n; }
	  int getNumberOfCategories() { return numberOfCategories; }

	  // All the get functions for normalization parameters and trained GMMS parameters for both single object and object pair

	  vector<vector<double> > getmeanNormalizationSingleObject() { return meanNormalizationSingleObject; }
	  vector<vector<double> > getstdNormalizationSingleObject() { return stdNormalizationSingleObject; }
	  vector<vector<double> > getminFeatSingleObject() { return minFeatSingleObject; }
	  vector<vector<double> > getmaxFeatSingleObject() { return maxFeatSingleObject; }
	  vector<vector<vector<double> > > getmeanNormalizationObjectPair() {return meanNormalizationObjectPair; }
	  vector<vector<vector<double> > > getstdNormalizationObjectPair() {return stdNormalizationObjectPair; }
	  vector<vector<vector<double> > > getminFeatObjectPair() {return minFeatObjectPair; }
	  vector<vector<vector<double> > > getmaxFeatObjectPair() {return maxFeatObjectPair; }
	  vector<cv::Mat> getmeansSingleObject() { return meansSingleObject; }
	  vector<cv::Mat> getweightsSingleObject() { return weightsSingleObject; }
	  vector< vector<cv::Mat> > getcovsSingleObject() { return covsSingleObject; }
	  vector<vector<cv::Mat> > getmeansObjectPair() { return meansObjectPair; }
	  vector<vector<cv::Mat> > getweightsObjectPair() { return weightsObjectPair; }
	  vector<vector<vector<cv::Mat> > > getcovsObjectPair() { return covsObjectPair; }

	  vector<double> geThresholdsSingleObject(){ return thresholdsSingleObject; }

};


#endif /* TRAINING_HPP_ */
