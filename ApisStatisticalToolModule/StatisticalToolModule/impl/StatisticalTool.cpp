/*
 * GMMLearning.cpp
 *
 *  Created on: Nov 19, 2013
 *      Author: marina
 */

#include "StatisticalTool.hpp"

#define DEBUG 0
#define TESTFLAG 1

void StatisticalTool::trainGMM(vector<vector<float> > features, int nclusters, cv::Mat & means, cv::Mat & weights, vector<cv::Mat> & covs) {

	 // ***********************************************************************
	 // Initialize the feature matrix "FeatMat" as a cv::Mat object, empty

	 cv::Mat FeatMat = cv::Mat::zeros ( features.size(), features[0].size(),  CV_64F );

	 // Populate cv::Mat from a vector of vectors.

	 for ( int i = 0; i < features.size() ; i++ ) {
		 for ( int j = 0; j < features[0].size() ; j++ ) {
			 FeatMat.at<double>(i, j) = (double) (features[i][j]);
		 }
	 }

	 /*
	// ***********************************************************************
	// Normalization of the feature matrix

	cv::Mat normalizedFeatMat;
	meansVector = computeMean(FeatMat);
	stdVector = computeStd(FeatMat, meansVector);
	maxVector = computeMax(FeatMat);
	minVector = computeMin(FeatMat);

	if (normalization == 1) {
		normalizedFeatMat = doNormalization(FeatMat, meansVector, stdVector);
	}
	else if (normalization == 2) {
		normalizedFeatMat = doNormalizationMinMax(FeatMat, maxVector, minVector);
	}
	else {
		normalizedFeatMat = FeatMat.clone();
	}

*/
	 // ***********************************************************************
	 // Training the EM model

	 cv::EM em_model(nclusters);
	 if (DEBUG) {
		 std::cout << "Training the EM model." << std::endl;
	 }
	 em_model.train ( FeatMat );

	 // Output: the trained GMM model parameters

	 means = em_model.get<cv::Mat>("means");
	 weights = em_model.get<cv::Mat> ("weights");
	 covs = em_model.get<vector<cv::Mat> > ("covs");

 }


 /*
 Computes the probability value of a GMM (multivariate normal distribution)
 given means, covariance matrices, mixture weights
 and the input feature vector = test sample
 */
 double StatisticalTool::computeGMMProbability(vector<float> featureVector, cv::Mat _means, vector<cv::Mat> _covs, cv::Mat _weights) {

	int fsize = featureVector.size();

	cv::Mat feats = cv::Mat::zeros ( 1, fsize,  CV_64F );

	// Converts the features in input from vector to cv::Mat object
    for ( int j = 0; j < featureVector.size() ; j++ ) {
    	feats.at<double>(j) = (double) featureVector[j];
     }

   int nclusters = _weights.cols;

   if (DEBUG) {
     cout << endl << endl << "The number of mixture components is : " << nclusters << endl;
       std::cout << "The size of the means is:  " << _means.size()  << endl <<
       "  and weights : " << _weights.size() << std::endl <<
       "  and feats : " << feats.size() << std::endl <<
       "  and covs : " << _covs.size() << std::endl ;
   }

   double totProb = 0;

   for (int i = 0; i < nclusters; i++) {

     if (DEBUG) {
       std::cout<< std::endl << "The current cluster is :  "  << i << std::endl;
     }

     cv::Mat current_mean = _means.row(i);
     double current_weight = _weights.at<double>(i);
     cv::Mat current_cov = _covs.at(i);

     double dim = (double) current_mean.cols;

     if (DEBUG) {
       std::cout << "The current mean is : " << current_mean <<  std::endl ;
       std::cout << "The current covariance matrix is : " << current_cov <<  std::endl ;
     }

     //  compute the determinant of standard deviation matrix
     double detCov = determinant(current_cov);

     if (DEBUG) {
       std::cout << "The determinant of the covariance matrix is : " << detCov <<  std::endl ;
     }

     //  compute the first term of the multivariate normal distribution
     double term1_ = sqrt ( pow ( (2*M_PI) , dim ) *  detCov  ) ;
     double term1 = 1 / term1_;

     if (DEBUG) {
       std::cout << "INSIDE STAT: The  first term is : " << term1 <<  std::endl ;
     }

     cv::Mat test_samplet = feats; //.t();

     // compute the exponential term
     cv::Mat terma = (test_samplet - current_mean);
     cv::Mat termb = ((current_cov)).inv();
     cv::Mat termc = (test_samplet - current_mean).t();

     if (DEBUG) {
       std::cout << "x is : " << test_samplet <<  std::endl ;
       std::cout << "(x - mu) is : " << terma <<  std::endl ;
       std::cout << "cov.inv() is : " << termb <<  std::endl ;
       std::cout << "cov * cov.inv() = " << current_cov * current_cov.inv() << endl;
       std::cout << "(x - mu ^ T) is : " << termc <<  std::endl ;
     }
     cv::Mat termexp =  - 0.5 * terma * termb * termc;

     if (DEBUG) {
       std::cout << "The  exponential term is : " << termexp <<  std::endl ;
     }

     double termExp = termexp.at<double>(0);
     double term2 = exp(termExp);
     double probCluster = term1 * term2;

     if (DEBUG) {
       std::cout << "The final probability is : " << probCluster << std::endl;
     }

     totProb += probCluster * current_weight;
   }
   return totProb;
 }



 /* This function computes the mean per feature from a feature matrix N_samples x N_features */
 vector<double> StatisticalTool::computeMean(cv::Mat & FeatMat) {

   vector<double> meansVector;
   // for each column i.e. each 1-D feature
   for ( int icolumn = 0 ; icolumn < FeatMat.cols; icolumn++ ) {
     double s = 0;
     for (int irow = 0; irow < FeatMat.rows ; irow++ ) {
       s += FeatMat.at<double>(irow, icolumn);
     }
     // compute mean
     double _mean = s / FeatMat.rows;
     // store the mean and std values for the current object class and the current feature
     meansVector.push_back(_mean);
   }
   return meansVector;
 }


 vector<double> StatisticalTool::computeMeanv(vector<vector<float> > & in) {

 	  vector<double> meansVector;

 	  // for each column i.e. each 1-D feature
 	  for ( int icolumn = 0 ; icolumn < in[0].size(); icolumn++ ) {
 	    double s = 0;
 	    for (int irow = 0; irow < in.size() ; irow++ ) {
 	      s += in.at(irow).at(icolumn);
 	    }

 	    // compute mean
 	    double _mean = s / (double) in.size();

 	    // store the mean and std values for the current object class and the current feature
 	    meansVector.push_back(_mean);
 	  }
 	  return meansVector;

 }



 /* This function computes the std per feature from a feature matrix N_samples x N_features */
 vector<double> StatisticalTool::computeStd(cv::Mat & FeatMat, vector<double> meansVector) {
   vector<double> stdVector;
   for ( int icolumn = 0 ; icolumn < FeatMat.cols; icolumn++ ) {
     double s_std = 0;

     for (int irow = 0; irow < FeatMat.rows ; irow++ ) {
       s_std += pow( ( FeatMat.at<double>(irow, icolumn) - meansVector[icolumn]) , 2);
     }
     double _std =sqrt(s_std / FeatMat.rows );
     if (_std == 0) { _std = 1; }
     stdVector.push_back(_std);
   }
   return stdVector;
 }


 vector<double> StatisticalTool::computeStdv(vector<vector<float> > & FeatMat, vector<double> meansVector) {
 	  vector<double> stdVector;
 	  for ( int icolumn = 0 ; icolumn < FeatMat.at(0).size(); icolumn++ ) {
 	    double s_std = 0;

 	    for (int irow = 0; irow < FeatMat.size() ; irow++ ) {
 	      s_std += pow( ( (double)FeatMat.at(irow).at(icolumn) - meansVector[icolumn]) , 2);
 	    }
 	    double _std =sqrt(s_std / FeatMat.size() );
 	    if (_std == 0) { _std = 1; }
 	    stdVector.push_back(_std);
 	  }
 	  return stdVector;
 }


 /* This function does feature matrix normalization */
 cv::Mat StatisticalTool::doNormalization(cv::Mat & FeatMat, vector<double> meansVector, vector<double> stdVector) {

 	cv::Mat normalizedFeatMat = FeatMat.clone();
 	// normalize the current column (i.e. feature) of the feature matrix
 	for ( int icolumn = 0 ; icolumn < FeatMat.cols; icolumn++ ) {
 		for (int irow = 0 ; irow < normalizedFeatMat.rows ; irow++ ) {
 			normalizedFeatMat.at<double>(irow, icolumn) = (normalizedFeatMat.at<double>(irow, icolumn) - meansVector[icolumn]) / (stdVector[icolumn] );
 		}
 	}
 	return normalizedFeatMat;
 }

 vector<vector<float> > StatisticalTool::doNormalizationv(vector<vector<float> > & FeatMat, vector<double> meansVector, vector<double> stdVector) {

 	vector<vector<float> >  normalizedFeatMat = FeatMat;
 	// normalize the current column (i.e. feature) of the feature matrix
 	for ( int icolumn = 0 ; icolumn < FeatMat[0].size(); icolumn++ ) {
 		for (int irow = 0 ; irow < normalizedFeatMat.size() ; irow++ ) {
 			normalizedFeatMat.at(irow).at(icolumn) = (normalizedFeatMat.at(irow).at(icolumn) - meansVector[icolumn]) / (stdVector[icolumn] );
 		}
 	}
 	return normalizedFeatMat;
 }



 vector<float> StatisticalTool::doNormalizationFeatureVector(vector<float> & FeatMat, vector<double> meansVector, vector<double> stdVector) {

 	vector<float> normalizedFeatMat = FeatMat;

 	// normalize the current column (i.e. feature) of the feature matrix
 	for ( int icolumn = 0 ; icolumn < FeatMat.size(); icolumn++ ) {
 		normalizedFeatMat.at(icolumn) = (normalizedFeatMat.at(icolumn) - meansVector[icolumn]) / (stdVector[icolumn] );
 	}
 	return normalizedFeatMat;
 }



 vector<double> StatisticalTool::computeMin(cv::Mat FeatMat) {
   vector<double> minvector;
   if (FeatMat.rows > 0) {
     for ( int icolumn = 0 ; icolumn < FeatMat.cols; icolumn++ ) {
       double mymin = FeatMat.at<double>(0, icolumn);
       for (int irow = 1; irow < FeatMat.rows ; irow++ ) {
         if( FeatMat.at<double>(irow, icolumn) < mymin) {
           mymin = FeatMat.at<double>(irow, icolumn);
         }
       }
       minvector.push_back(mymin);
     }
   }
   return minvector;
 }




 vector<double> StatisticalTool::computeMinv(vector<vector<float> >  FeatMat) {

 	  vector<double> minvector;
 	  if (FeatMat.size()> 0) {
 	    for ( int icolumn = 0 ; icolumn < FeatMat.at(0).size(); icolumn++ ) {
 	      double mymin = FeatMat.at(0).at(icolumn);

 	      for (int irow = 1; irow < FeatMat.size() ; irow++ ) {

 	        if( FeatMat.at(irow).at(icolumn) < mymin) {
 	          mymin = (double)FeatMat.at(irow).at(icolumn);
 	        }
 	      }
 	      minvector.push_back(mymin);
 	    }
 	  }
 	  return minvector;
 }




 vector<double> StatisticalTool::computeMax(cv::Mat FeatMat) {
   vector<double> maxvector;
   if (FeatMat.rows > 0) {
     for ( int icolumn = 0 ; icolumn < FeatMat.cols; icolumn++ ) {

       double mymax = FeatMat.at<double>(0, icolumn);
       for (int irow = 1; irow < FeatMat.rows ; irow++ ) {
         if( FeatMat.at<double>(irow, icolumn) > mymax ) {
           mymax = FeatMat.at<double>(irow, icolumn);
         }
       }
       maxvector.push_back(mymax);

     }
   }
   return maxvector;
 }


 vector<double> StatisticalTool::computeMaxv(vector<vector<float> > FeatMat) {

 	  vector<double> maxvector;
 	  if (FeatMat.size() > 0) {
 	    for ( int icolumn = 0 ; icolumn < FeatMat.at(0).size(); icolumn++ ) {

 	      double mymax = FeatMat.at(0).at(icolumn);
 	      for (int irow = 1; irow < FeatMat.size() ; irow++ ) {
 	        if( FeatMat.at(irow).at(icolumn) > mymax ) {
 	          mymax = (double)FeatMat.at(irow).at(icolumn);
 	        }
 	      }
 	      maxvector.push_back(mymax);

 	    }
 	  }
 	  return maxvector;

 }




 // other way of normalization meaning (x - min)/(max - min)
 cv::Mat StatisticalTool::doNormalizationMinMax(cv::Mat & FeatMat, vector<double> maxvector, vector<double> minvector) {

   cv::Mat normalizedFeatMat = FeatMat.clone();
   // normalize the current column (i.e. feature) of the feature matrix
   for ( int icolumn = 0 ; icolumn < FeatMat.cols; icolumn++ ) {
     for (int irow = 0 ; irow < normalizedFeatMat.rows ; irow++ ) {
       if (maxvector[icolumn] - minvector[icolumn] == 0) {
         //normalizedFeatMat.at<double>(irow, icolumn) = normalizedFeatMat.at<double>(irow, icolumn);
       }
       else {
         normalizedFeatMat.at<double>(irow, icolumn) = (normalizedFeatMat.at<double>(irow, icolumn) - minvector[icolumn]) / (maxvector[icolumn] - minvector[icolumn] );
       }
     }
   }
   return normalizedFeatMat;
 }



 // other way of normalization meaning (x - min)/(max - min)
 vector<vector<float> > StatisticalTool::doNormalizationMinMaxv(vector<vector<float> > & FeatMat, vector<double> maxvector, vector<double> minvector) {

   vector<vector<float> > normalizedFeatMat = FeatMat;
   // normalize the current column (i.e. feature) of the feature matrix
   for ( int icolumn = 0 ; icolumn < FeatMat.at(0).size(); icolumn++ ) {
     for (int irow = 0 ; irow < normalizedFeatMat.size() ; irow++ ) {
       if (maxvector[icolumn] - minvector[icolumn] == 0) {
         //normalizedFeatMat.at<double>(irow, icolumn) = normalizedFeatMat.at<double>(irow, icolumn);
       }
       else {
         normalizedFeatMat.at(irow).at(icolumn) = (normalizedFeatMat.at(irow).at(icolumn) - minvector[icolumn]) / (maxvector[icolumn] - minvector[icolumn] );
       }
     }
   }
   return normalizedFeatMat;
 }



 // other way of normalization meaning (x - min)/(max - min)
 vector<float> StatisticalTool::doNormalizationMinMaxFeatureVector(vector<float> & FeatMat, vector<double> maxvector, vector<double> minvector) {

   vector<float> normalizedFeatMat = FeatMat;
   // normalize the current column (i.e. feature) of the feature matrix
   for ( int icolumn = 0 ; icolumn < FeatMat.size(); icolumn++ ) {
       if (maxvector[icolumn] - minvector[icolumn] == 0) {
         //normalizedFeatMat.at<double>(irow, icolumn) = normalizedFeatMat.at<double>(irow, icolumn);
       }
       else {
         normalizedFeatMat.at(icolumn) = (normalizedFeatMat.at(icolumn) - minvector[icolumn]) / (maxvector[icolumn] - minvector[icolumn] );
       }
   }
   return normalizedFeatMat;
 }



