/*
 * Test.hpp
 *
 *  Created on: Nov 25, 2013
 *      Author: marina
 */

#ifndef TEST_HPP_
#define TEST_HPP_

#include "ArrangeFeatureTestScene.hpp"
#include "Training.hpp"
#include "utils.hpp"
#include "ApiGraph.hpp"
#include "ros/ros.h"
#include "strands_qsr_msgs/GetGroupClassification.h"
#include "strands_qsr_msgs/BBox.h"
#include "strands_qsr_msgs/ObjectClassification.h"
// #include <tuple>


using namespace std;

typedef pair<int, int> idCategoryPair;
typedef pair<idCategoryPair, double> pairScore;
typedef map<int, double> mapCategoryConfidence;

class Test {

private:

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
	**************************************************************************************   */

	//  <nObjCat x featDim x nclusters>
	vector<cv::Mat> meansSingleObject;

	//  <nObjCat x nclusters x 1>
	vector<cv::Mat> weightsSingleObject;

	// <nObjCat x nclusters x featDim x featDim>
	vector< vector<cv::Mat> > covsSingleObject;

	vector<vector<cv::Mat> > meansObjectPair;
	vector<vector<cv::Mat> > weightsObjectPair;
	vector<vector<vector<cv::Mat> > > covsObjectPair;

	vector<double> thresholdsSingleObject;

	/* *************************************************************************************
		// The occurrence and co-occurrence frequency of object categories
	**************************************************************************************   */

	vector<double> frequencySingleObject;
	vector<vector<double> > frequencyObjectPair;

public:

	// // the functions for loading the data from the training / learning on the training database

	void loadTrainedGMMs(Training & trainedParams);

	void loadLearnedObjectCategoryFrequency(vector<double> frequencySingleObjectin, vector<vector<double> > frequencyObjectPairin );

	// // the functions for the test
	// // note: the test functions will provide in output a "path" which is a map<int, int>, where key = object instance id
	// // and value = categoryLabel

	// // TODO :: use the subset of possible object category labels i.e. the categoryList in input
	path predictObjectClassesOnlySOF(ArrangeFeatureTestScene &, int normalization);


	// // TODO :: use the subset of possible object category labels i.e. the categoryList in input
	path voting(ArrangeFeatureTestScene & testfeatures, int normalization, vector<vector<double> > & votingTable);


	// // Other version of the "voting scheme" function has the additional argument: vector<int> categoryList to limit the search to
	// // a subset of possible object categories
	vector<strands_qsr_msgs::ObjectClassification> voting(ArrangeFeatureTestScene & testfeatures, int normalization, vector<vector<double> > & votingTable, vector<int> categoryList, vector<string> categoryListString, map<string, mapCategoryConfidence> objectClassificationMap);

	static map<string, mapCategoryConfidence> convertObjectClassificationMsgToIDS(vector<strands_qsr_msgs::ObjectClassification> inVector);



	// // TODO : add feature normalization
	path exhaustiveSearch(ArrangeFeatureTestScene & testfeatures, int normalization, vector<path> allPaths);

	path exhaustiveSearchAfterVoting(ArrangeFeatureTestScene & testfeatures, vector<vector<pairScore> > votingTable, int normalization);

	path optimizationGreedy(ArrangeFeatureTestScene & testfeatures, vector<vector<pairScore> > votingTable, int normalization);

	vector<vector<pairScore> > prepareVotingTableOptimizationSOFbasedScores(ArrangeFeatureTestScene &, int normalization, vector<int> categoryList);

	double computeScoreClique(ArrangeFeatureTestScene & testfeatures, int normalization, path myPath);

	// TODO: think of a better way to compute the SCORING FUNCTION that assigns a score/cost to a path/assignment!!!
	double computeScoreCliqueProduct(ArrangeFeatureTestScene & testfeatures, int normalization, path myPath);

	double computeScoreCliqueVotingStrategy(ArrangeFeatureTestScene & testfeatures, int normalization, path myPath);


	static SingleObjectFeature findSOF(ArrangeFeatureTestScene & testfeatures, int currentInstanceID);
	static ObjectPairFeature findOPF(ArrangeFeatureTestScene & testfeatures, int currentInstanceID, int targetInstanceID);

	static idCategoryPair findMaximumScoreVotingTable(vector<vector<pairScore> > votingTable);
	static vector<vector<pairScore> > createVotingTableComplete(vector<vector<double> > votingTableOld, ArrangeFeatureTestScene features );
	static vector<vector<pairScore> > createShortlistedVotingTable(vector<vector<pairScore> > inputVotingTable);

	static void printPath(path);

	// // the set functions for the private data members

	void setmeanNormalizationSingleObject(vector<vector<double> > in ) { meanNormalizationSingleObject = in; }
	void setstdNormalizationSingleObject(vector<vector<double> > in ) { stdNormalizationSingleObject = in; }
	void setminFeatSingleObject(vector<vector<double> > in ) { minFeatSingleObject = in; }
	void setmaxFeatSingleObject(vector<vector<double> > in ) { maxFeatSingleObject = in; }
	void setmeanNormalizationObjectPair(vector<vector<vector<double> > >  in ) { meanNormalizationObjectPair = in; }
	void setstdNormalizationObjectPair(vector<vector<vector<double> > >  in ) { stdNormalizationObjectPair = in; }
	void setminFeatObjectPair(vector<vector<vector<double> > >  in ) { minFeatObjectPair = in; }
	void setmaxFeatObjectPair(vector<vector<vector<double> > > in ) { maxFeatObjectPair = in; }
	void setmeansSingleObject( vector<cv::Mat> in ) { meansSingleObject = in ; }
    void setweightsSingleObject (vector<cv::Mat> in ) { weightsSingleObject = in;}
    void setcovsSingleObject(vector<vector<cv::Mat> > in) {covsSingleObject = in; }
    void setmeansObjectPair(vector<vector<cv::Mat> > in) {meansObjectPair = in; }
    void setweightsObjectPair(vector<vector<cv::Mat> > in ) {weightsObjectPair = in; }
    void setcovsObjectPair(vector<vector<vector<cv::Mat> > > in) {covsObjectPair = in;}
    void setThresholdSingleObject(vector<double> in) { thresholdsSingleObject = in; }

	void printmeanNormalizationSingleObject();
	void printmeanNormalizationObjectPair();

	void setfrequencySingleObject(vector<double> in) {frequencySingleObject = in; }
	void setfrequencyObjectPair(vector<vector<double> > in) {frequencyObjectPair = in; }




};




#endif /* TEST_HPP_ */
