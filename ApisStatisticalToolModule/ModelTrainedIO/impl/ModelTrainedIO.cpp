/*
 * StoreTraining.cpp
 *
 *  Created on: Nov 23, 2013
 *      Author: marina
 */

#include "ModelTrainedIO.hpp"

#define TESTFLAG 1
#define DEBUG 1



void ModelTrainedIO::storeMeanNormalizationSingleObjectFile(Training & db, string folder) {

	int ncat;
	int nclusters;
	int featdim;

	vector<vector<double> > means = db.getmeanNormalizationSingleObject();
	ofstream myfile;

	string filepath = folder + "/SOmeans.txt" ;
	const char * filepathC = filepath.c_str();
	myfile.open (filepathC);

	// for each object category
	for (int i = 0; i < means.size(); i++) {

		// if data are present for the current object category
		if (means.at(i).size() > 0) {

			myfile << "y ";


			// for each of the features in the feature list (if data are present for the current object category)
			for (int j = 0; j < means.at(i).size(); j++) {

				myfile << means.at(i).at(j) << " ";
			}
			myfile << endl;
		}

		// if data are NOT present for the current object category
		else {
			myfile << "n" << endl;
		}
	}
	myfile.close();

}


void ModelTrainedIO::loadMeanNormalizationSingleObjectFile(string folder, Test & test) {

	vector<vector<double > > meanNormalizationSingleObject;

	string lineData;
    string filepath = folder + "/SOmeans.txt";
	const char * filepathC = filepath.c_str();

	ifstream myfile (filepathC);

	if (myfile.is_open())	{

		// for each line : in this file, each line is an object category
		while ( getline (myfile,lineData) )
		{
			// takes the first word of the line
			string firstword;
			std::stringstream lineStream(lineData);
			lineStream >> firstword;
			std::vector<double> row;

			// if the first word is "y"
			if (strcmp(firstword.c_str(), "y" ) == 0) {
				double d;

				// for each of the "words" in the line
				while (lineStream >> d) {
					row.push_back(d);
				}
			}
			meanNormalizationSingleObject.push_back(row);

		}
		myfile.close();
		test.setmeanNormalizationSingleObject(meanNormalizationSingleObject);
	}

	else { cout << "Unable to open file"; }
	cout << endl << " meanNormalizationSingleObject : The size of results is: " << meanNormalizationSingleObject.size() << endl;
}


void ModelTrainedIO::storeStdNormalizationSingleObjectFile(Training & db, string folder) {

	int ncat;
	int nclusters;
	int featdim;

	vector<vector<double> > stds = db.getstdNormalizationSingleObject();
	ofstream myfile;

	string filepath = folder + "/SOstds.txt" ;
	const char * filepathC = filepath.c_str();
	myfile.open (filepathC);

	// for each object category
	for (int i = 0; i < stds.size(); i++) {

		// if data are present for the current object category
		if (stds.at(i).size() > 0) {

			myfile << "y ";

			// for each of the features in the feature list (if data are present for the current object category)
			for (int j = 0; j < stds.at(i).size(); j++) {

				myfile << stds.at(i).at(j) << " ";
			}
			myfile << endl;
		}

		// if data are NOT present for the current object category
		else {
			myfile << "n" << endl;
		}
	}
	myfile.close();

}


void ModelTrainedIO::storeMinFeatSingleObjectFile(Training & db, string folder) {

	int ncat;
	int nclusters;
	int featdim;

	vector<vector<double> > minim = db.getminFeatSingleObject();
	ofstream myfile;

	string filepath = folder + "/SOmin.txt" ;
	const char * filepathC = filepath.c_str();
	myfile.open (filepathC);

	// for each object category
	for (int i = 0; i < minim.size(); i++) {

		// if data are present for the current object category
		if (minim.at(i).size() > 0) {

			myfile << "y ";

			// for each of the features in the feature list (if data are present for the current object category)
			for (int j = 0; j < minim.at(i).size(); j++) {

				myfile << minim.at(i).at(j) << " ";
			}
			myfile << endl;
		}

		// if data are NOT present for the current object category
		else {
			myfile << "n" << endl;
		}
	}
	myfile.close();



}

void ModelTrainedIO::storeMaxFeatSingleObjectFile(Training & db, string folder) {


	int ncat;
	int nclusters;
	int featdim;

	vector<vector<double> > minim = db.getmaxFeatSingleObject();
	ofstream myfile;

	string filepath = folder + "/SOmax.txt" ;
	const char * filepathC = filepath.c_str();
	myfile.open (filepathC);

	// for each object category
	for (int i = 0; i < minim.size(); i++) {

		// if data are present for the current object category
		if (minim.at(i).size() > 0) {

			myfile << "y ";

			// for each of the features in the feature list (if data are present for the current object category)
			for (int j = 0; j < minim.at(i).size(); j++) {

				myfile << minim.at(i).at(j) << " ";
			}
			myfile << endl;
		}

		// if data are NOT present for the current object category
		else {
			myfile << "n" << endl;
		}
	}
	myfile.close();

}

// *****************************************************************************************
// *****************************************************************************************
// *****************************************************************************************

void ModelTrainedIO::storeMeanNormalizationObjectPairFile(Training & db, string folder) {

	int ncat;
	int nclusters;
	int featdim;

	vector<vector<vector<double> > > meansOP = db.getmeanNormalizationObjectPair();
	ofstream myfile;

	string filepath = folder + "/OPmeans.txt" ;
	const char * filepathC = filepath.c_str();
	myfile.open (filepathC);

	ncat = meansOP.size();
	featdim = meansOP.at(0).at(0).size();
	myfile << ncat << " " << featdim << endl;

	// for each object category
	for (int i = 0; i < meansOP.size(); i++) {

		// for each object category
		for (int i2 = 0; i2 < meansOP.at(i).size(); i2++) {

			// if data are present for the current object category pair
			if (meansOP.at(i).at(i2).size() > 0) {

				myfile << "y ";

				// for each of the features in the feature list (if data are present for the current object category pair)
				for (int j = 0; j < meansOP.at(i).at(i2).size(); j++) {

					myfile << meansOP.at(i).at(i2).at(j) << " ";
				}
				myfile << endl;

			}

			// if data are NOT present for the current object category pair
			else {
				myfile << "n" << endl;
			}
		}
	}
	myfile.close();

}

void ModelTrainedIO::storeStdNormalizationObjectPairFile(Training & db, string folder) {


	int ncat;
	int nclusters;
	int featdim;

	vector<vector<vector<double> > > vectorstore = db.getstdNormalizationObjectPair();
	ofstream myfile;

	string filepath = folder + "/OPstds.txt" ;
	const char * filepathC = filepath.c_str();
	myfile.open (filepathC);

	ncat = vectorstore.size();
	featdim = vectorstore.at(0).at(0).size();
	myfile << ncat << " " << featdim << endl;

	// for each object category
	for (int i = 0; i < vectorstore.size(); i++) {

		// for each object category
		for (int i2 = 0; i2 < vectorstore.at(i).size(); i2++) {

			// if data are present for the current object category pair
			if (vectorstore.at(i).at(i2).size() > 0) {

				myfile << "y ";

				// for each of the features in the feature list (if data are present for the current object category pair)
				for (int j = 0; j < vectorstore.at(i).at(i2).size(); j++) {

					myfile << vectorstore.at(i).at(i2).at(j) << " ";
				}
				myfile << endl;

			}

			// if data are NOT present for the current object category pair
			else {
				myfile << "n" << endl;
			}
		}
	}
	myfile.close();


}

void ModelTrainedIO::storeMinFeatObjectPairFile(Training & db, string folder) {

	int ncat;
	int nclusters;
	int featdim;

	vector<vector<vector<double> > > vectorstore = db.getminFeatObjectPair();
	ofstream myfile;

	string filepath = folder + "/OPmin.txt" ;
	const char * filepathC = filepath.c_str();
	myfile.open (filepathC);

	ncat = vectorstore.size();
	featdim = vectorstore.at(0).at(0).size();
	myfile << ncat << " " << featdim << endl;

	// for each object category
	for (int i = 0; i < vectorstore.size(); i++) {

		// for each object category
		for (int i2 = 0; i2 < vectorstore.at(i).size(); i2++) {

			// if data are present for the current object category pair
			if (vectorstore.at(i).at(i2).size() > 0) {

				myfile << "y ";

				// for each of the features in the feature list (if data are present for the current object category pair)
				for (int j = 0; j < vectorstore.at(i).at(i2).size(); j++) {

					myfile << vectorstore.at(i).at(i2).at(j) << " ";
				}
				myfile << endl;

			}

			// if data are NOT present for the current object category pair
			else {
				myfile << "n" << endl;
			}
		}
	}
	myfile.close();

}

void ModelTrainedIO::storeMaxFeatObjectPairFile(Training & db, string folder) {

	int ncat;
	int nclusters;
	int featdim;

	vector<vector<vector<double> > > vectorstore = db.getmaxFeatObjectPair();
	ofstream myfile;

	string filepath = folder + "/OPmax.txt" ;
	const char * filepathC = filepath.c_str();
	myfile.open (filepathC);

	ncat = vectorstore.size();
	featdim = vectorstore.at(0).at(0).size();
	myfile << ncat << " " << featdim << endl;

	// for each object category
	for (int i = 0; i < vectorstore.size(); i++) {

		// for each object category
		for (int i2 = 0; i2 < vectorstore.at(i).size(); i2++) {

			// if data are present for the current object category pair
			if (vectorstore.at(i).at(i2).size() > 0) {

				myfile << "y ";

				// for each of the features in the feature list (if data are present for the current object category pair)
				for (int j = 0; j < vectorstore.at(i).at(i2).size(); j++) {

					myfile << vectorstore.at(i).at(i2).at(j) << " ";
				}
				myfile << endl;

			}

			// if data are NOT present for the current object category pair
			else {
				myfile << "n" << endl;
			}
		}
	}
	myfile.close();
}

// *****************************************************************************************
// *****************************************************************************************
// *****************************************************************************************

void ModelTrainedIO::storeMeansSingleObjectFile(Training & db, string folder) {

	int ncat;
	int nclusters;
	int featdim;

	vector<cv::Mat> vectorstore = db.getmeansSingleObject();
	ofstream myfile;
	myfile.precision(16);

	string filepath = folder + "/SOEMmeans.txt" ;
	const char * filepathC = filepath.c_str();

	myfile.open (filepathC);

	// write n. categories, n.clusters, feat dim.
	ncat = vectorstore.size();
	nclusters = vectorstore.at(0).rows;   // TODO check!
	featdim = vectorstore.at(0).cols;

	myfile << ncat << " " << nclusters << " " << featdim << endl;

	// for every object category
	for (int i = 0; i < vectorstore.size(); i++) {

		cv::Mat means = vectorstore.at(i);

		// if data are present for the current object category
		if (vectorstore.at(i).rows > 0) {

			// for every cluster
			for (int j = 0; j < means.rows; j++) {

				myfile << "y ";

				// for all the features (featDim)
				for (int z = 0; z < means.cols; z++) {
					myfile << means.at<double>(j, z) << " ";
				}
				myfile << endl;
			}
		}
		else {

			// for every cluster
			for (int j = 0; j < nclusters; j++) {
				myfile << "n " << endl;
			}
		}
	}



}

void ModelTrainedIO::storeWeightsSingleObjectFile(Training & db, string folder) {

	int ncat;
	int nclusters;
	int featdim;

	vector<cv::Mat> vectorstore = db.getweightsSingleObject();
	ofstream myfile;
	myfile.precision(16);

	string filepath = folder + "/SOEMweights.txt" ;
	const char * filepathC = filepath.c_str();

	myfile.open (filepathC);

	// for every object category
	for (int i = 0; i < vectorstore.size(); i++) {

		cv::Mat weights = vectorstore.at(i);

		// if data are present for the current object category
		if (vectorstore.at(i).cols > 0) {

			myfile << "y ";

			// for every cluster
			for (int j = 0; j < weights.cols; j++) {
				myfile << weights.at<double>(j) << " ";
			}
			myfile << endl;
		}
		else {
			myfile << "n " << endl;
		}
	}
}

void ModelTrainedIO::storeCovsSingleObjectFile(Training & db, string folder) {

	int ncat;
	int nclusters;
	int featdim;

	vector<vector<cv::Mat> > vectorstore =db.getcovsSingleObject();

	ncat = vectorstore.size();
	nclusters = vectorstore.at(0).size();
	featdim = vectorstore.at(0).at(0).rows;

	ofstream myfile;
	myfile.precision(16);

	string filepath = folder + "/SOEMcovs.txt" ;
	const char * filepathC = filepath.c_str();

	myfile.open (filepathC);

	// convert the Mat object into 2-D matrix
	myfile << ncat << " " << nclusters << " " << featdim << endl;

	// for every object category
	for (int i = 0; i < vectorstore.size(); i++) {

		// IF data are present for the current object category
		if (vectorstore.at(i).size() > 0) {

			// for every cluster
			for (int j = 0; j < vectorstore.at(i).size(); j++) {


				cv::Mat covs = vectorstore.at(i).at(j);

				// for all the features (featDim)
				for (int z = 0; z < covs.rows; z++) {

					myfile << "y " ;

					for (int k = 0; k < covs.cols; k++) {

						myfile << covs.at<double>(z, k) << " ";
					}
					myfile << endl;
				}
			}
		}
		else {
			for (int j = 0; j < nclusters; j++) {

				// for all the features (featDim)
				for (int z = 0; z < featdim; z++) {
					myfile << "n " << endl;
				}

			}

		}

	}

}

void ModelTrainedIO::storeMeansObjectPairFile(Training & db, string folder) {

	int ncat;
	int nclusters;
	int featdim;

	// < nCat X nCat X nclusters X featdim >

	vector<vector<cv::Mat> > storedata = db.getmeansObjectPair();
	ofstream myfile;
	myfile.precision(16);
	string filepath = folder + "/OPEMmeans.txt" ;
	const char * filepathC = filepath.c_str();

	ncat = storedata.size();
	nclusters = (storedata.at(0).at(0)).rows;    // TODO change because the first element may be empty!
	featdim = (storedata.at(0).at(0)).cols;

	myfile.open (filepathC);

	// convert the Mat object into 2-D matrix
	myfile << ncat << " " << nclusters << " " << featdim << endl;

	// for every object category
	for (int i = 0; i < storedata.size(); i++) {

		// for every object category
		for (int i2 = 0; i2 < storedata.at(i).size(); i2++) {


			// IF data are present for the current object category Pair
			if (storedata.at(i).at(i2).rows > 0) {

				cv::Mat currentmeansEM = storedata.at(i).at(i2);

				// for every cluster - prints a line with "y " and all the values (one per feature)
				for (int j = 0; j < nclusters; j++) {

					myfile << "y ";

					// for all the features (featDim)
					for (int z = 0; z < currentmeansEM.cols; z++) {
						myfile << currentmeansEM.at<double>(j, z) << " ";
					}
					myfile << endl;
				}
			}

			// IF data are NOT present for the current object category pair
			else {
				// for every cluster prints a line with "n "
				for (int j = 0; j < nclusters; j++) {
					myfile << "n" << endl;
				}
			}
		}
	}

}

void ModelTrainedIO::storeWeightsObjectPairFile(Training & db, string folder) {

	int ncat;
	int nclusters;
	int featdim;

	//// <nCat X nCat < nClusters>

	vector<vector<cv::Mat> > storedata = db.getweightsObjectPair();
	ofstream myfile;
	myfile.precision(16);


	string filepath = folder + "/OPEMweights.txt" ;
	const char * filepathC = filepath.c_str();

	myfile.open (filepathC);

	ncat = storedata.size();
	nclusters = (storedata.at(0).at(0)).cols;    	// TODO change because the first element may be empty!
	myfile << ncat << " " << nclusters << endl;

	// for every object category
	for (int i = 0; i < storedata.size(); i++) {

		// for every object category
		for (int i2 = 0; i2 < storedata.at(i).size(); i2++) {

			// check if there are data for the current object category pair
			if (storedata.at(i).at(i2).cols > 0) {

				cv::Mat currentweightsEM = storedata.at(i).at(i2);

				myfile << "y ";

				// for all the clusters

				for (int z = 0; z < currentweightsEM.cols; z++) {
					myfile << currentweightsEM.at<double>(z) << " ";
				}

				myfile << endl;
			}
			else {
				myfile << "n " << endl;
			}
		}
	}


}

void ModelTrainedIO::storeCovsObjectPairFile(Training & db, string folder) {
	int ncat;
	int nclusters;
	int featdim;




	// // <ncat X ncat x nclusters x featDim x featDim>

	vector<vector<vector<cv::Mat> > > storedata = db.getcovsObjectPair();


	ofstream myfile;
	myfile.precision(16);

	ncat  = storedata.size();                   // TODO: change
	nclusters = storedata.at(0).at(0).size();
	featdim = (storedata.at(0).at(1).at(0)).rows;



	string filepath = folder + "/OPEMcovs.txt" ;
	const char * filepathC = filepath.c_str();

	myfile.open (filepathC);

	// convert the Mat object into 2-D matrix
	myfile << ncat << " " << nclusters << " " << featdim << endl;



	// for every object category
	for (int i = 0; i < storedata.size(); i++) {

		// for every object category
		for (int i2 = 0; i2 < storedata.at(i).size(); i2++) {

			// check if there are data for the current object category pair
			if (storedata.at(i).at(i2).size() > 0) {

				// for every cluster
				for (int i3 = 0; i3 < nclusters; i3++) {

					cv::Mat currentcovsEM = storedata.at(i).at(i2).at(i3);

					// for all the features (featDim)
					for (int j = 0; j < featdim; j++) {

						myfile << "y ";

						// for all the features (featDim)
						for (int z = 0; z < featdim; z++) {
							myfile << currentcovsEM.at<double>(j, z) << " ";
						}
						myfile << endl;
					}
				}
			}
			else {
				// for every cluster
				for (int i3 = 0; i3 < nclusters; i3++) {

					// for all the features (featDim)
					for (int j = 0; j < featdim; j++) {

						myfile << "n " << endl;
					}
				}
			}
		}
	}

}



// **************************************************************************
// **************************************************************************



void ModelTrainedIO::loadStdNormalizationSingleObjectFile(string folder, Test & test) {

	vector<vector<double > > vectorstore;

	string lineData;
    string filepath = folder + "/SOstds.txt";
	const char * filepathC = filepath.c_str();

	ifstream myfile (filepathC);

	if (myfile.is_open())	{

		// for each line : in this file, each line is an object category
		while ( getline (myfile,lineData) )
		{
			// takes the first word of the line
			string firstword;
			std::stringstream lineStream(lineData);
			lineStream >> firstword;
			std::vector<double> row;

			// if the first word is "y"
			if (strcmp(firstword.c_str(), "y")== 0) {
				double d;

				// for each of the "words" in the line
				while (lineStream >> d) {
					row.push_back(d);
				}
			}
			vectorstore.push_back(row);

		}
		myfile.close();
		test.setstdNormalizationSingleObject(vectorstore);
	}

	else { cout << "Unable to open file"; }
	cout << endl << " stdNormalizationSingleObject : The size of results is: " << vectorstore.size() << endl;
}


void ModelTrainedIO::loadMinFeatSingleObjectFile(string folder, Test & test) {

	vector<vector<double > > vectorstore;

	string lineData;
    string filepath = folder + "/SOmin.txt";
	const char * filepathC = filepath.c_str();

	ifstream myfile (filepathC);

	if (myfile.is_open())	{

		// for each line : in this file, each line is an object category
		while ( getline (myfile,lineData) )
		{
			// takes the first word of the line
			string firstword;
			std::stringstream lineStream(lineData);
			lineStream >> firstword;
			std::vector<double> row;

			// if the first word is "y"
			if (strcmp(firstword.c_str(), "y")== 0) {
				double d;

				// for each of the "words" in the line
				while (lineStream >> d) {
					row.push_back(d);
				}
			}
			vectorstore.push_back(row);

		}
		myfile.close();
		test.setminFeatSingleObject(vectorstore);
	}

	else { cout << "Unable to open file"; }
	cout << endl << " minFeatSingleObject : The size of results is: " << vectorstore.size() << endl;
}


void ModelTrainedIO::loadMaxFeatSingleObjectFile(string folder, Test & test) {

	vector<vector<double > > vectorstore;

	string lineData;
    string filepath = folder + "/SOmax.txt";
	const char * filepathC = filepath.c_str();

	ifstream myfile (filepathC);

	if (myfile.is_open())	{

		// for each line : in this file, each line is an object category
		while ( getline (myfile,lineData) )
		{
			// takes the first word of the line
			string firstword;
			std::stringstream lineStream(lineData);
			lineStream >> firstword;
			std::vector<double> row;

			// if the first word is "y"
			if (strcmp(firstword.c_str(), "y")== 0) {
				double d;

				// for each of the "words" in the line
				while (lineStream >> d) {
					row.push_back(d);
				}
			}
			vectorstore.push_back(row);

		}
		myfile.close();
		test.setmaxFeatSingleObject(vectorstore);
	}

	else { cout << "Unable to open file"; }
	cout << endl << " maxFeatSingleObject : The size of results is: " << vectorstore.size() << endl;
}



// ***************************************************


void ModelTrainedIO::loadMeanNormalizationObjectPairFile(string folder, Test & test) {

	vector<vector<double > > storevector;
	string lineData;
	int ncat;
	int featdim;
    string filepath = folder + "/OPmeans.txt";
	const char * filepathC = filepath.c_str();

	ifstream myfile (filepathC);

	if (myfile.is_open())	{

		int countLines = 0;

		// for each line : in this file, each line is an object-object category pair
		while ( getline (myfile,lineData) )
		{
			// takes the first word of the line

			std::stringstream lineStream(lineData);

			if (countLines == 0)
			{
				int singleword;
				lineStream >> singleword;
				ncat = singleword;
				lineStream >> singleword;
				featdim = singleword;
			}

			else {

				string firstword;
				lineStream >> firstword;
				std::vector<double> row;

				// if the first word is "y"
				if (strcmp(firstword.c_str(), "y" ) == 0) {
					double d;

					// for each of the "words" in the line
					while (lineStream >> d) {
						row.push_back(d);
					}
				}
				storevector.push_back(row);
			}
			countLines++;
		}
		myfile.close();
	}

	else { cout << "Unable to open file"; }

	cout << "Before storing in the final data structure" << endl;

	// arrange this data into the vector of vector of vector of double

	vector < vector < vector <double > > > savevector;
	int indexrows = 0;
	for (int i = 0; i < ncat; i++ ) {

		vector < vector < double> >  currentCat ;

		for (int j = 0; j < ncat ; j++) {

			vector<double> currentPair;

			for (int z = 0; z < storevector.at(indexrows).size(); z++) {
				currentPair.push_back(storevector.at(indexrows).at(z));
			}

			currentCat.push_back(currentPair);

			indexrows++;
		}
		savevector.push_back(currentCat);
	}

	test.setmeanNormalizationObjectPair(savevector);

	if (TESTFLAG) {
		cout << endl << "meanNormalizationObjectPair The size of results is: " << savevector.size() << endl;
	}



}


void ModelTrainedIO::loadStdNormalizationObjectPairFile(string folder, Test & test) {

	vector<vector<double > > storevector;
	string lineData;
	int ncat;
	int featdim;
    string filepath = folder + "/OPstds.txt";
	const char * filepathC = filepath.c_str();

	ifstream myfile (filepathC);

	if (myfile.is_open())	{

		int countLines = 0;

		// for each line : in this file, each line is an object-object category pair
		while ( getline (myfile,lineData) )
		{
			// takes the first word of the line

			std::stringstream lineStream(lineData);

			if (countLines == 0)
			{
				int singleword;
				lineStream >> singleword;
				ncat = singleword;
				lineStream >> singleword;
				featdim = singleword;
			}

			else {

				string firstword;
				lineStream >> firstword;
				std::vector<double> row;

				// if the first word is "y"
				if (strcmp(firstword.c_str(), "y" ) == 0) {
					double d;

					// for each of the "words" in the line
					while (lineStream >> d) {
						row.push_back(d);
					}
				}
				storevector.push_back(row);
			}
			countLines++;
		}
		myfile.close();
	}

	else { cout << "Unable to open file"; }


	// arrange this data into the vector of vector of vector of double

	vector < vector < vector <double > > > savevector;
	int indexrows = 0;
	for (int i = 0; i < ncat; i++ ) {

		vector < vector < double> >  currentCat ;

		for (int j = 0; j < ncat ; j++) {

			vector<double> currentPair;

			for (int z = 0; z < storevector.at(indexrows).size(); z++) {
				currentPair.push_back(storevector.at(indexrows).at(z));
			}

			currentCat.push_back(currentPair);

			indexrows++;
		}
		savevector.push_back(currentCat);
	}

	test.setstdNormalizationObjectPair(savevector);

	if (TESTFLAG) {
		cout << endl << "stdNormalizationObjectPair The size of results is: " << savevector.size() << endl;
	}

}


void ModelTrainedIO::loadMinFeatObjectPairFile(string folder, Test & test) {


	vector<vector<double > > storevector;
	string lineData;
	int ncat;
	int featdim;
	string filepath = folder + "/OPmin.txt";
	const char * filepathC = filepath.c_str();

	ifstream myfile (filepathC);

	if (myfile.is_open())	{

		int countLines = 0;

		// for each line : in this file, each line is an object-object category pair
		while ( getline (myfile,lineData) )
		{
			// takes the first word of the line

			std::stringstream lineStream(lineData);

			if (countLines == 0)
			{
				int singleword;
				lineStream >> singleword;
				ncat = singleword;
				lineStream >> singleword;
				featdim = singleword;
			}

			else {

				string firstword;
				lineStream >> firstword;
				std::vector<double> row;

				// if the first word is "y"
				if (strcmp(firstword.c_str(), "y" ) == 0) {
					double d;

					// for each of the "words" in the line
					while (lineStream >> d) {
						row.push_back(d);
					}
				}
				storevector.push_back(row);
			}
			countLines++;
		}
		myfile.close();
	}

	else { cout << "Unable to open file"; }


	// arrange this data into the vector of vector of vector of double

	vector < vector < vector <double > > > savevector;
	int indexrows = 0;
	for (int i = 0; i < ncat; i++ ) {

		vector < vector < double> >  currentCat ;

		for (int j = 0; j < ncat ; j++) {

			vector<double> currentPair;

			for (int z = 0; z < storevector.at(indexrows).size(); z++) {
				currentPair.push_back(storevector.at(indexrows).at(z));
			}

			currentCat.push_back(currentPair);

			indexrows++;
		}
		savevector.push_back(currentCat);
	}

	test.setminFeatObjectPair(savevector);

	if (TESTFLAG) {
		cout << endl << "minNormalizationObjectPair The size of results is: " << savevector.size() << endl;
	}

}


void ModelTrainedIO::loadMaxFeatObjectPairFile(string folder, Test & test) {


	vector<vector<double > > storevector;
	string lineData;
	int ncat;
	int featdim;
	string filepath = folder + "/OPmax.txt";
	const char * filepathC = filepath.c_str();

	ifstream myfile (filepathC);

	if (myfile.is_open())	{

		int countLines = 0;

		// for each line : in this file, each line is an object-object category pair
		while ( getline (myfile,lineData) )
		{
			// takes the first word of the line

			std::stringstream lineStream(lineData);

			if (countLines == 0)
			{
				int singleword;
				lineStream >> singleword;
				ncat = singleword;
				lineStream >> singleword;
				featdim = singleword;
			}

			else {

				string firstword;
				lineStream >> firstword;
				std::vector<double> row;

				// if the first word is "y"
				if (strcmp(firstword.c_str(), "y" ) == 0) {
					double d;

					// for each of the "words" in the line
					while (lineStream >> d) {
						row.push_back(d);
					}
				}
				storevector.push_back(row);
			}
			countLines++;
		}
		myfile.close();
	}

	else { cout << "Unable to open file"; }


	// arrange this data into the vector of vector of vector of double

	vector < vector < vector <double > > > savevector;
	int indexrows = 0;
	for (int i = 0; i < ncat; i++ ) {

		vector < vector < double> >  currentCat ;

		for (int j = 0; j < ncat ; j++) {

			vector<double> currentPair;

			for (int z = 0; z < storevector.at(indexrows).size(); z++) {
				currentPair.push_back(storevector.at(indexrows).at(z));
			}

			currentCat.push_back(currentPair);

			indexrows++;
		}
		savevector.push_back(currentCat);
	}

	test.setmaxFeatObjectPair(savevector);

	if (TESTFLAG) {
		cout << endl << "maxNormalizationObjectPair The size of results is: " << savevector.size() << endl;
	}
}


// ***************************************************



void ModelTrainedIO::loadMeansSingleObjectFile(string folder, Test & test) {

	// needs to know how many clusters are there
	string lineData;

    string filepath = folder + "/SOEMmeans.txt";
	const char * filepathC = filepath.c_str();
	ifstream myfile (filepathC);

	vector<vector<double> > storevector;
	int ncat;
	int nclusters;
	int featdim;

	if (myfile.is_open())	{
		int countLines = 0;
		while ( getline (myfile,lineData) )
		{
			std::stringstream lineStream(lineData);

			if (countLines == 0)
			{
				int singleword;
				lineStream >> singleword;
				ncat = singleword;
				lineStream >> singleword;
				nclusters = singleword;
				lineStream >> singleword;
				featdim = singleword;

				if (TESTFLAG) {
					cout << "ncat  " << ncat << "    featdim  "  << featdim << endl;
				}

				// meansSingleObject.push_back(currentMeansMatrix);
			}
			else {

				string firstword;
				lineStream >> firstword;
				std::vector<double> row;

				// if the first word is "y"
				if (strcmp(firstword.c_str(), "y" ) == 0) {

					if (DEBUG)  {
						// cout << " loaded model" << countLines << endl;
					}

					double d;
					while (lineStream >> d) {
						row.push_back(d);
					}
				}
				storevector.push_back(row);
			}
			countLines++;
		}
		myfile.close();
	}
	else {
		cout << "Unable to open file";
	}

	// arrange this data into the vector of cv::Mat

	int indexrows = 0;
	vector<cv::Mat> savedata;

	// for every category
	for (int i = 0; i < ncat; i++ ) {

		cv::Mat currenCat; //  = cv::Mat::zeros ( nclusters, featdim,  CV_64F );

		// for every cluster
		for (int j = 0; j < nclusters ; j++) {

			cv::Mat currentRow; // = cv::Mat::zeros(1, featdim, CV_64F);


			// if there are data for this category
			if (storevector.at(indexrows).size() > 0 ) {

				currentRow = cv::Mat::zeros(1, featdim, CV_64F);

				// for every feature
				for (int z = 0; z < featdim; z++) {
					// currenCat.at<double>(j, z) = storevector.at(indexrows).at(z);
					currentRow.at<double>(z) = storevector.at(indexrows).at(z);
				}

			}

			else {

			}

			currenCat.push_back(currentRow);
			indexrows++;
		}
		savedata.push_back(currenCat);
	}
	test.setmeansSingleObject(savedata);

	if (DEBUG) {
		cout << endl << "EM means SO : The size of results is: " << savedata.size() << endl;
	}
}




void ModelTrainedIO::loadWeightsSingleObjectFile(string folder, Test & test) {

    string filepath = folder + "/SOEMweights.txt";

	const char * filepathC = filepath.c_str();
	ifstream myfile (filepathC);

	string lineData;
	vector<vector<double> > currentvector;

	if (myfile.is_open())	{

		while ( getline (myfile,lineData) )
		{
			string firstword;
			std::stringstream lineStream(lineData);

			lineStream >> firstword;
			std::vector<double> row;

			// if the first word is "y"
			if (strcmp(firstword.c_str(), "y" ) == 0) {

				double d;
				while (lineStream >> d) {
					row.push_back(d);
				}
			}
			currentvector.push_back(row);
		}
		myfile.close();
	}

	else { cout << "Unable to open file"; }
	if (TESTFLAG) {
		cout << endl << "SOEMweights The size of results is: " << currentvector.size() << endl;
	}


	// arrange this data into the vector of cv::Mat

	vector<cv::Mat> savedata;

	// for each object category
	for (int j = 0; j < currentvector.size() ; j++) {

		cv::Mat currentcatweights; // = cv::Mat::zeros ( currentvector.at(0).size(), 1,  CV_64F );


		if (currentvector.at(j).size() > 0 ) {

			// cv::Mat tmp =  cv::Mat::zeros ( currentvector.at(0).size(), 1,  CV_64F );

			for (int z = 0; z < currentvector.at(j).size(); z++) {
				currentcatweights.push_back( currentvector.at(j).at(z) );
				cout << "the values at row and cols " << j << " " << z << "  are " << currentvector.at(j).at(z) << endl;
			}

			savedata.push_back(currentcatweights.t());
		}
		else {
			savedata.push_back(currentcatweights);
		}

	}
	test.setweightsSingleObject(savedata);

}



void ModelTrainedIO::loadCovsSingleObjectFile(string folder, Test & test) {

// vector< vector<cv::Mat> > covsSingleObject;
// needs to know how many clusters are there

	string lineData;

    string filepath = folder + "/SOEMcovs.txt";
	const char * filepathC = filepath.c_str();
	ifstream myfile (filepathC);
	vector<vector<double> > storevector;
	int ncat;
	int nclusters;
	int featdim;

	if (myfile.is_open())	{
		int countLines = 0;
		while ( getline (myfile,lineData) )
		{
			std::stringstream lineStream(lineData);

			if (countLines == 0)
			{
				int singleword;
				lineStream >> singleword;
				ncat = singleword;
				lineStream >> singleword;
				nclusters = singleword;
				lineStream >> singleword;
				featdim = singleword;

				if (TESTFLAG) {
					cout << "ncat  " << ncat << "   nclusters  " << nclusters << "  featdim  " << featdim << endl;
				}

				// meansSingleObject.push_back(currentMeansMatrix);
			}
			else {

				string firstword;
				lineStream >> firstword;
				std::vector<double> row;

				// if the first word is "y"
				if (strcmp(firstword.c_str(), "y" ) == 0) {

					double d;
					while (lineStream >> d) {
						row.push_back(d);
					}
				}
				storevector.push_back(row);

			}
			countLines++;
		}
		myfile.close();
	}
	else {
		cout << "Unable to open file";
	}

	// arrange the data into the cv::Mat objects
	int indexrows = 0;
	vector<vector<cv::Mat> > savedata;

	for (int i = 0; i < ncat; i++ ) {

		vector<cv::Mat> tempvector;

		for (int j = 0; j < nclusters ; j++) {

			cv::Mat currenCatCovs; // = cv::Mat::zeros ( featdim, featdim,  CV_64F );

			for (int z = 0; z < featdim; z++) {

				cv::Mat currentRow;

				if ( storevector.at(indexrows).size() > 0) {

					currentRow = cv::Mat::zeros(1, featdim, CV_64F);

					for (int k = 0; k < storevector.at(indexrows).size(); k++) {
						// currenCatCovs.at<double>(z, k) = storevector.at(indexrows).at(k);
						currentRow.at<double>(z) = storevector.at(indexrows).at(z);
					}
				}

				indexrows++;
				currenCatCovs.push_back(currentRow);
			}

			tempvector.push_back(currenCatCovs);
		}
		savedata.push_back(tempvector);
	}

	if (DEBUG) {
		cout << endl << " covs SO: The size of results is: " << savedata.size() << endl;
	}

	test.setcovsSingleObject(savedata);

}



void ModelTrainedIO::loadMeansObjectPairFile(string folder, Test & test) {

	if (TESTFLAG) {
		cout << "start loadMeansObjectPairFile" << endl;
	}

    string filepath = folder + "/OPEMmeans.txt";
	const char * filepathC = filepath.c_str();
	ifstream myfile (filepathC);

	// needs to know how many clusters are there
	string lineData;
	vector<vector<double> > storevector;
	int ncat;
	int nclusters;
	int featdim;

	if (myfile.is_open())	{
		int countLines = 0;
		while ( getline (myfile,lineData) )
		{
			std::stringstream lineStream(lineData);

			if (countLines == 0)
			{
				int singleword;
				lineStream >> singleword;
				ncat = singleword;
				lineStream >> singleword;
				nclusters = singleword;
				lineStream >> singleword;
				featdim = singleword;

				if (TESTFLAG) {
					cout << "ncat  " << ncat << "    featdim  "  << featdim << endl;
				}

				// meansSingleObject.push_back(currentMeansMatrix);
			}
			else {

				string firstword;
				lineStream >> firstword;
				std::vector<double> row;

				// if the first word is "y"
				if (strcmp(firstword.c_str(), "y" ) == 0) {

					double d;
					while (lineStream >> d) {
						row.push_back(d);
					}
				}
				storevector.push_back(row);
			}
			countLines++;
		}
		myfile.close();
	}
	else {
		cout << "Unable to open file";
	}

	// arrange this data into the vector of vector of cv::Mat

	int indexrows = 0;
	vector<vector<cv::Mat> > savedata;

	// for every category
	for (int i = 0; i < ncat; i++ ) {

		vector<cv::Mat> currentCat;

		// for every category
		for  (int i2 = 0; i2 < ncat; i2++ ) {

			cv::Mat currenCatPair;    // = cv::Mat::zeros ( nclusters, featdim,  CV_64F );

			// for every cluster
			for (int j = 0; j < nclusters ; j++) {

				cv::Mat currentRow;

				if (storevector.at(indexrows).size() > 0 ) {

					currentRow = cv::Mat::zeros(1, featdim, CV_64F);

					// for every feature
					for (int z = 0; z < storevector.at(indexrows).size(); z++) {


						currentRow.at<double>(z) = storevector.at(indexrows).at(z);

						//currenCatPair.at<double>(j, z) = storevector.at(indexrows).at(z);
					}

				}
				else{

				}
				currenCatPair.push_back(currentRow);
				indexrows++;

			}
			currentCat.push_back(currenCatPair);
		}
		savedata.push_back(currentCat);
	}

	test.setmeansObjectPair(savedata);

	if (TESTFLAG) {
		cout << endl << "EM means OP : The size of results is: " << savedata.size() << endl;
	}
}


void ModelTrainedIO::loadWeightsObjectPairFile(string folder, Test & test) {

	if (TESTFLAG) {
		cout << "start loadWeightObjectPairFile" << endl;
	}
    string filepath = folder + "/OPEMweights.txt";
	const char * filepathC = filepath.c_str();
	ifstream myfile (filepathC);


	// needs to know how many clusters are there
	string lineData;
	vector<vector<double> > storevector;
	int ncat;
	int nclusters;
	int featdim;

	if (myfile.is_open())	{
		int countLines = 0;
		while ( getline (myfile,lineData) )
		{
			std::stringstream lineStream(lineData);

			if (countLines == 0)
			{
				int singleword;
				lineStream >> singleword;
				ncat = singleword;
				lineStream >> singleword;
				nclusters = singleword;
				//lineStream >> singleword;
				//featdim = singleword;

				if (TESTFLAG) {
					cout << "ncat  " << ncat << "    featdim  "  << featdim << endl;
				}

				// meansSingleObject.push_back(currentMeansMatrix);
			}
			else {

				string firstword;
				lineStream >> firstword;
				std::vector<double> row;

				// if the first word is "y"
				if (strcmp(firstword.c_str(), "y" ) == 0) {

					double d;
					while (lineStream >> d) {
						row.push_back(d);
					}
				}
				storevector.push_back(row);
			}
			countLines++;
		}
		myfile.close();
	}
	else {
		cout << "Unable to open file OPEMweights";
	}

	// arrange this data into the vector of vector of cv::Mat

	int indexrows = 0;
	vector<vector<cv::Mat> > savedata;

	// for every category
	for (int i = 0; i < ncat; i++ ) {

		vector<cv::Mat> currentCat;

		// for every category
		for  (int i2 = 0; i2 < ncat; i2++ ) {

			cv::Mat currenCatPair; // = cv::Mat::zeros ( nclusters, 1,  CV_64F );


			if (storevector.at(indexrows).size() > 0 ) {
				currenCatPair = cv::Mat::zeros ( nclusters, 1, CV_64F );
				// for every cluster
				for (int z = 0; z < storevector.at(indexrows).size(); z++) {
					currenCatPair.at<double>(z) = storevector.at(indexrows).at(z);
				}
				indexrows++;

				currentCat.push_back(currenCatPair.t());
			}
			else {
				currentCat.push_back(currenCatPair);
				indexrows++;
			}


		}
		savedata.push_back(currentCat);
	}

	test.setweightsObjectPair(savedata);

}




void ModelTrainedIO::loadCovsObjectPairFile(string folder, Test & test) {


	if (TESTFLAG) {
		cout << "start loadCovsObjectPairFile" << endl;
	}
    string filepath = folder + "/OPEMcovs.txt";
	const char * filepathC = filepath.c_str();
	ifstream myfile (filepathC);

	// needs to know how many clusters are there
	string lineData;
	vector<vector<double> > storevector;
	int ncat;
	int nclusters;
	int featdim;

	if (myfile.is_open())	{
		int countLines = 0;
		while ( getline (myfile,lineData) )
		{
			std::stringstream lineStream(lineData);

			if (countLines == 0)
			{
				int singleword;
				lineStream >> singleword;
				ncat = singleword;
				lineStream >> singleword;
				nclusters = singleword;
				lineStream >> singleword;
				featdim = singleword;

				if (TESTFLAG) {
					cout << "ncat  " << ncat << "    featdim  "  << featdim << endl;
				}

				// meansSingleObject.push_back(currentMeansMatrix);
			}
			else {

				string firstword;
				lineStream >> firstword;
				std::vector<double> row;

				// if the first word is "y"
				if (strcmp(firstword.c_str(), "y" ) == 0) {

					double d;
					while (lineStream >> d) {
						row.push_back(d);
					}
				}
				storevector.push_back(row);
			}
			countLines++;
		}
		myfile.close();
	}
	else {
		cout << "Unable to open file";
	}

	// arrange this data into the vector of vector of cv::Mat

	int indexrows = 0;
	vector<vector<vector<cv::Mat> > > savedata;

	// for every category
	for (int i = 0; i < ncat; i++ ) {

		vector<vector<cv::Mat> > currentCat;

		// for every category
		for  (int i2 = 0; i2 < ncat; i2++ ) {

			vector<cv::Mat> currentCatPair;

			// for every cluster
			for (int j = 0; j < nclusters ; j++) {

				cv::Mat currentMat; // // = cv::Mat::zeros ( featdim, featdim,  CV_64F );

				// for every feature
				for (int k = 0; k < featdim; k++) {

					cv::Mat currentRow = cv::Mat::zeros(1, featdim, CV_64F);

					// for every feature (each column of the data)
					for (int z = 0; z < storevector.at(indexrows).size(); z++) {


						currentRow.at<double>(z) = storevector.at(indexrows).at(z);

						//currentMat.at<double>(k, z) = storevector.at(indexrows).at(z);
					}
					currentMat.push_back(currentRow);
					indexrows++;

				}
				currentCatPair.push_back(currentMat);
			}
			currentCat.push_back(currentCatPair);
		}
		savedata.push_back(currentCat);
	}

	test.setcovsObjectPair(savedata);

}

void ModelTrainedIO::storeThresholdSingleObject(Training & db, string folder) {

	int ncat;
	int nclusters;
	int featdim;

	vector<double>  storedata = db.geThresholdsSingleObject();
	ofstream myfile;

	string filepath = folder + "/SOthre.txt" ;
	const char * filepathC = filepath.c_str();
	myfile.open (filepathC);

	// for each object category
	for (int i = 0; i < storedata.size(); i++) {

		// if data are present for the current object category
		// if (storedata.at(i).size() > 0) {

			myfile << "y ";

			myfile << storedata.at(i)<< " ";

			myfile << endl;
		// }

		// if data are NOT present for the current object category
		//else {
		//	myfile << "n" << endl;
		// }
	}
	myfile.close();
}

void ModelTrainedIO::loadThresholdSingleObject(string folder, Test & test) {


	vector<double > vectorstore;

	string lineData;
    string filepath = folder + "/SOthre.txt";
	const char * filepathC = filepath.c_str();

	ifstream myfile (filepathC);

	if (myfile.is_open())	{

		// for each line : in this file, each line is an object category
		while ( getline (myfile,lineData) )
		{
			// takes the first word of the line
			string firstword;
			std::stringstream lineStream(lineData);
			lineStream >> firstword;
			double d;

			// if the first word is "y"
			if (strcmp(firstword.c_str(), "y")== 0) {


			// for each of the "words" in the line
				lineStream >> d;

			}
			vectorstore.push_back(d);

		}
		myfile.close();
		test.setThresholdSingleObject(vectorstore);
	}

	else { cout << "Unable to open file"; }
	cout << endl << " ThresholdSingleObject : The size of results is: " << vectorstore.size() << endl;


}




void ModelTrainedIO::storeTrainingToFile(Training & db, string folder ) {


	storeMeanNormalizationSingleObjectFile(db, folder);


	if (DEBUG)  {
		cout << " store 1" << endl;
	}
	storeStdNormalizationSingleObjectFile(db, folder);

	if (DEBUG)  {
		cout << " store 2" << endl;
	}

	storeMinFeatSingleObjectFile(db, folder);

	if (DEBUG)  {
		cout << " store 3" << endl;
	}

	storeMaxFeatSingleObjectFile(db, folder);

	if (DEBUG)  {
		cout << " store 4" << endl;
	}

	storeMeanNormalizationObjectPairFile(db, folder);

	if (DEBUG)  {
		cout << " store 5" << endl;
	}
	storeStdNormalizationObjectPairFile(db, folder);
	if (DEBUG)  {
		cout << " store 6" << endl;
	}

	storeMinFeatObjectPairFile(db, folder);
	if (DEBUG)  {
		cout << " store 7" << endl;
	}
	storeMaxFeatObjectPairFile(db, folder);
	if (DEBUG)  {
		cout << " store 8" << endl;
	}

	storeMeansSingleObjectFile(db, folder);
	if (DEBUG)  {
		cout << " store 9" << endl;
	}
	storeWeightsSingleObjectFile(db, folder);
	if (DEBUG)  {
		cout << " store 10" << endl;
	}
	storeCovsSingleObjectFile(db, folder);
	if (DEBUG)  {
		cout << " store 11" << endl;
	}
	storeMeansObjectPairFile(db, folder);
	if (DEBUG)  {
		cout << " store 12" << endl;
	}
	storeWeightsObjectPairFile(db, folder);
	if (DEBUG)  {
		cout << " store 13" << endl;
	}
	storeCovsObjectPairFile(db, folder);
	if (DEBUG)  {
		cout << " store 14" << endl;
	}

	storeThresholdSingleObject(db, folder);
	if (DEBUG)  {
		cout << " store 15" << endl;
	}

}




void ModelTrainedIO::loadTrainedGMMsFile(string folder, Test & test) {

	loadMeanNormalizationSingleObjectFile(folder, test);
	loadStdNormalizationSingleObjectFile(folder, test);
	loadMinFeatSingleObjectFile(folder, test);
	loadMaxFeatSingleObjectFile(folder, test);
	loadMeanNormalizationObjectPairFile(folder, test);
	loadStdNormalizationObjectPairFile(folder, test);
	loadMinFeatObjectPairFile(folder, test);
	loadMaxFeatObjectPairFile(folder, test);
	if (DEBUG)  {
		cout << " load a" << endl;
	}
	loadMeansSingleObjectFile(folder, test);

	if (DEBUG)  {
		cout << " load b" << endl;
	}
	loadWeightsSingleObjectFile(folder, test);
	loadCovsSingleObjectFile(folder, test);
	loadMeansObjectPairFile(folder, test);
	loadWeightsObjectPairFile(folder, test);
	loadCovsObjectPairFile(folder, test);

	loadThresholdSingleObject(folder, test);

}


void ModelTrainedIO::storefrequenciesSingleObject(vector<double> in, string folder) {

		ofstream myfile;

		string filepath = folder + "/SOfrequency.txt" ;
		const char * filepathC = filepath.c_str();
		myfile.open (filepathC);

		// for each object category

		for (int i = 0; i < in.size(); i++) {
			myfile << in.at(i) << endl;
		}

		myfile.close();
}


void ModelTrainedIO::storefrequenciesObjectPair(vector<vector<double> > in, string folder) {

	ofstream myfile;

	string filepath = folder + "/OPfrequency.txt" ;
	const char * filepathC = filepath.c_str();
	myfile.open (filepathC);

	// for each object category

	for (int i = 0; i < in.size(); i++) {
		for (int j = 0; j < in.at(i).size(); j++) {
			myfile << in.at(i).at(j) << " ";
		}
		myfile << endl;
	}

	myfile.close();

}

void ModelTrainedIO::loadfrequenciesSingleObject(string folder, Test & test) {


	if (TESTFLAG) {
		cout << "Start load frequency single object " << endl;
	}
	vector<double > vectorstore;

	string lineData;
    string filepath = folder + "/SOfrequency.txt";
	const char * filepathC = filepath.c_str();

	ifstream myfile (filepathC);

	if (myfile.is_open())	{

		// for each line : in this file, each line is an object category
		while ( getline (myfile,lineData) )
		{
			// takes the first word of the line
			string firstword;
			std::stringstream lineStream(lineData);
			double d;
			// for each of the "words" in the line
			lineStream >> d;
			vectorstore.push_back(d);
		}

		myfile.close();
		test.setfrequencySingleObject(vectorstore);
	}


}

void ModelTrainedIO::loadfrequenciesObjectPair(string folder, Test & test) {

	if (TESTFLAG) {
		cout << "Start load frequency pair object " << endl;
	}

	vector<vector<double > > vectorstore;

	string lineData;
	string filepath = folder + "/OPfrequency.txt";
	const char * filepathC = filepath.c_str();

	ifstream myfile (filepathC);

	if (myfile.is_open())	{

		// for each line : in this file, each line is an object category
		while ( getline (myfile,lineData) )
		{
			vector<double> currentLine;
			// takes the first word of the line
			std::stringstream lineStream(lineData);
			double d;
			// for each of the "words" in the line

			while (lineStream >> d) {
				currentLine.push_back(d);
			}
			vectorstore.push_back(currentLine);
		}

		myfile.close();
		test.setfrequencyObjectPair(vectorstore);
	}

}
