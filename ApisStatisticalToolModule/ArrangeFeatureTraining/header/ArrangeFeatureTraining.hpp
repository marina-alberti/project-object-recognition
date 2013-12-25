/*
 * ArrangeFeatureTraining.hpp
 *
 *  Created on: Nov 23, 2013
 *      Author: marina
 */

#ifndef ARRANGEFEATURETRAINING_HPP_
#define ARRANGEFEATURETRAINING_HPP_


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
#include "DatabaseObjectPairFeature.hpp"

#define NOBJECTCLASSES 7


using namespace std;

/*
 * Input:
 * DatabaseSingleObjectFeature
 * DatabaseObjectPairFeature
 *
 * Output:
 * FeatureMatrix Single Object
 * FeatureMatrix Object Pair
 *
 * features are rearranged by object categories for preparing
 * to call the learning method in the statistical tool
 */

class ArrangeFeatureTraining {

private:

	  /*
	   * The feature matrices
	   * N_objectCategories x N_scenes x N_features  (SingleObject)
	   * N_objectCategories x N_objectCategories x N_scenes x N_features (ObjectPair)
	   */
	  // vector<vector<vector<float> > > FMSingleObject;
	  // vector<vector<vector<vector<float> > > > FMPairObject;
public:

	  // Rearranges the features (SingleObject)
	  static void setFeatureMatrixSingleObject(DatabaseSingleObjectFeature &, vector<vector<vector<float> > > & FMSingleObject);

	  // Rearranges the features (ObjectPair)
	  static void setFeatureMatrixObjectPair(DatabaseObjectPairFeature &, vector<vector<vector<vector<float> > > > & FMPairObject);

	  static void printFeatureMatrixSingleObject(vector<vector<vector<float> > > & FMSingleObject);

	  static void printFeatureMatrixObjectPair(vector<vector<vector<vector<float> > > > & FMPairObject);

};



#endif /* ARRANGEFEATURETRAINING_HPP_ */
