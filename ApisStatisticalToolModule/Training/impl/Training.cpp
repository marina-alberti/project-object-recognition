/*
 * Training.cpp
 *
 *  Created on: Nov 23, 2013
 *      Author: marina
 */

#include "Training.hpp"

#define TESTFLAG 1
#define DEBUG 1

Training::Training(int in) {
	numberOfCategories = in;
}

// Calls the learning method (SingleObject)
void Training::learnGMMSingleObjectFeature(vector<vector<vector<float> > > FMSingleObject, int nclusters, int normalization) {

	int minSamples = 5;

	// int fsize = (FMSingleObject[0][0]).size();        // TODO: check

    // for each considered object category
	for ( int i = 0 ; i < FMSingleObject.size(); i++ ) {

		if (TESTFLAG)  { cout << "Current object:  " << i << endl;
		cout << FMSingleObject.at(i).size() << endl; }

		// Gets the features for the considered object category (<N_SCENES x FEAT_DIM>)
		vector<vector<float> >  FeatureMatrix = FMSingleObject.at(i);

		// ***********************************************************************
		// Normalization of the feature matrix
		// ***********************************************************************

		vector<vector<float> > normalizedFeatMat;

		if (normalization == 1) {
			vector<double> meansVector = StatisticalTool::computeMeanv(FeatureMatrix);
			vector<double> stdVector = StatisticalTool::computeStdv(FeatureMatrix, meansVector);
			normalizedFeatMat = StatisticalTool::doNormalizationv(FeatureMatrix, meansVector, stdVector);
			meanNormalizationSingleObject.push_back(meansVector);
			stdNormalizationSingleObject.push_back(stdVector);
		}
		else if (normalization == 2) {
			vector<double> maxVector = StatisticalTool::computeMaxv(FeatureMatrix);
			vector<double> minVector = StatisticalTool::computeMinv(FeatureMatrix);
			normalizedFeatMat = StatisticalTool::doNormalizationMinMaxv(FeatureMatrix, maxVector, minVector);
			minFeatSingleObject.push_back(minVector);
			maxFeatSingleObject.push_back(maxVector);
		}
		else if (normalization == 0) {
			normalizedFeatMat = FeatureMatrix;
		}

		// ***********************************************************************
		// Training - calls the StatisticalTool::trainGMM function
		// ***********************************************************************

		cv::Mat means;
		cv::Mat weights;
		vector<cv::Mat> covs;
		//StatisticalTool learningGMMSingleObjectFeature;

		if (normalizedFeatMat.size() > minSamples) {
			StatisticalTool::trainGMM(normalizedFeatMat, nclusters,  means, weights, covs);
		}

		// Stores the EM model parameters, weights means and covariance matrices into
		// the data members of this class

		meansSingleObject.push_back(means);
		weightsSingleObject.push_back(weights);
		covsSingleObject.push_back(covs);

		/////**********************************************************************
		/*
		// For the current object category:
        // Computes thresholds for likelihood for the object category (Single Object features)
		// by TESTING the learned GMM (one per object category) on the TRAINING objects of that category
		// and finding the MINIMUM of the likelihoods values.
		// If likelihood of another unknown test object is lower -> the object cannot belong to the considered class / is an outlier.
		*/
		/*

		double minProb = 1000;
		for (int z = 0; z < normalizedFeatMat.size(); z++) {
			vector<float> featsTrainScene = normalizedFeatMat.at(z);

			double prob = StatisticalTool::computeGMMProbability(featsTrainScene, means, covs, weights);

			prob = log(prob);
			if (prob < minProb) {
				minProb = prob;
			}
		}
		if (TESTFLAG) {
			cout << "Model  " << i << "  minprob  " << minProb  << endl;
		}


 		thresholdsSingleObject.push_back(minProb);
 		*/

		/////**********************************************************************

	}
}



// Calls the learning method (ObjectPair)
void Training::learnGMMObjectPairFeature(vector<vector<vector<vector<float> > > > FMPairObject , int nclusters, int normalization) {

	int minSamples = 5;

	// int fsize = (FMPairObject[0][0][0]).size();        // TODO: check

	// for each considered object class
	for ( int i = 0 ; i < FMPairObject.size(); i++ ) {

		vector<vector<double> > meanNormalizationObjectPairV1;
		vector<vector<double> > stdNormalizationObjectPairV1;
		vector<vector<double> > minFeatObjectPairV1;
		vector<vector<double> > maxFeatObjectPairV1;
		vector<cv::Mat> meansObjectPairV1;
		vector<cv::Mat> weightsObjectPairV1;
		vector<vector<cv::Mat> > covsObjectPairV1;

		for ( int j = 0;  j < FMPairObject[i].size(); j++) {



			if (DEBUG)  { cout << "Training. Current object pair:  " << i << "  and  " << j << endl; }

			vector<vector<float> >  FeatureMatrix = FMPairObject.at(i).at(j);


			// ***********************************************************************
			// Normalization of the feature matrix
			// ***********************************************************************

			vector<vector<float> > normalizedFeatMat = FeatureMatrix;
			vector<double> meansVector;
			vector<double> stdVector;
			vector<double> maxVector;
			vector<double> minVector;

			if (normalizedFeatMat.size() > minSamples) {

				// Stores the normalization parameters into the data members of this class
				if (normalization == 1) {
					meansVector = StatisticalTool::computeMeanv(FeatureMatrix);
					stdVector = StatisticalTool::computeStdv(FeatureMatrix, meansVector);
					normalizedFeatMat = StatisticalTool::doNormalizationv(FeatureMatrix, meansVector, stdVector);

				}
				else if (normalization == 2) {
					maxVector = StatisticalTool::computeMaxv(FeatureMatrix);
					minVector = StatisticalTool::computeMinv(FeatureMatrix);
					normalizedFeatMat = StatisticalTool::doNormalizationMinMaxv(FeatureMatrix, maxVector, minVector);

				}
				else if (normalization == 0) {
					// normalizedFeatMat = FeatureMatrix;
				}

			}
			meanNormalizationObjectPairV1.push_back(meansVector);
			stdNormalizationObjectPairV1.push_back(stdVector);
			minFeatObjectPairV1.push_back(minVector);
			maxFeatObjectPairV1.push_back(maxVector);

			// ***********************************************************************
			// Training
			// ***********************************************************************

			cv::Mat means;
			cv::Mat weights;
			vector<cv::Mat> covs;

			//StatisticalTool learningGMMObjectPairFeature;

			if (TESTFLAG) {
			cout << "size of the feature matrix is: " << normalizedFeatMat.size() << endl;
			/*
			for (int m = 0; m < normalizedFeatMat.size(); m++) {
				for (int n = 0; n < normalizedFeatMat.at(m).size(); n++) {
					cout << normalizedFeatMat.at(m).at(n) << "    " ;
				}
				cout << endl;
			}
			*/
			}


			if (normalizedFeatMat.size() > minSamples) {

				if (TESTFLAG) {
					cout << "Before calling trainGMM" << endl;
				}

				StatisticalTool::trainGMM(normalizedFeatMat, nclusters, means, weights, covs);

				if (TESTFLAG) {
					cout << "After calling trainGMM" << endl;
				}
			}

			// cout << "Before push_back" << endl;

			// Stores the EM output parameters: weights means and covariance matrices into
			// the data members of this class
			meansObjectPairV1.push_back(means);
			weightsObjectPairV1.push_back(weights);
			covsObjectPairV1.push_back(covs);
		}

		meanNormalizationObjectPair.push_back(meanNormalizationObjectPairV1);
		stdNormalizationObjectPair.push_back( stdNormalizationObjectPairV1);
		minFeatObjectPair.push_back( minFeatObjectPairV1 );
		maxFeatObjectPair.push_back( maxFeatObjectPairV1 );

		meansObjectPair.push_back(meansObjectPairV1);
		weightsObjectPair.push_back(weightsObjectPairV1);
		covsObjectPair.push_back(covsObjectPairV1);

		}

}

