/*
 * header.cpp
 *
 *  Created on: Nov 19, 2013
 *      Author: marina
 */

#include "utils.hpp"

int computeMaximum(vector<double> in) {
	double maximum;
	int maxposition;
	for (int i = 0; i < in.size(); i++) {
		if (i == 0) {
			maximum = in.at(i);
			maxposition = i;
		}
		else {
			if (in.at(i) > maximum) {
				maximum = in.at(i);
				maxposition = i;
			}
		}
	}
	return maxposition;
}


double computeAverage(vector<double> in) {

	double average;
	double sumValues = 0;
	for (int i = 0; i < in.size(); i++) {
		sumValues += in.at(i);
	}
	average = sumValues / in.size();
	return average;
}


double computeAverage(vector<int> in) {

	double average;
	double sumValues = 0;
	for (int i = 0; i < in.size(); i++) {
		sumValues += (double)( in.at(i));
	}
	average = (double) (sumValues / in.size());
	return average;
}



int computeEqualPosition(vector<int> in, int test) {

	int position;
	position = -1;
	for (int i = 0; i < in.size(); i++) {

		if (in.at(i) == test) {
			position = i;
		}

	}
	return position;
}


double computeMaximumValue(vector<double> in) {
	double maximum;
	int maxposition;
	for (int i = 0; i < in.size(); i++) {
		if (i == 0) {
			maximum = in.at(i);
			maxposition = i;
		}
		else {
			if (in.at(i) > maximum) {
				maximum = in.at(i);
				maxposition = i;
			}
		}
	}
	return maximum;
}


int computeMaximumValue(vector<int> in) {
	int maximum;
	int maxposition;
	for (int i = 0; i < in.size(); i++) {
		if (i == 0) {
			maximum = in.at(i);
			maxposition = i;
		}
		else {
			if (in.at(i) > maximum) {
				maximum = in.at(i);
				maxposition = i;
			}
		}
	}
	return maximum;
}


void evaluatePerformance(cv::Mat cMatrix) {

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
    if (1) {
      cout << "Object " << i << endl << "Precision: " << _precision << endl
      << "recall:    " << _recall << endl << "Fmeasure:  " << fm << endl;

    }
  }
}


/*
Input: the name of the directory containing the files.
Output: a list of the names of the files.
*/
vector<string> storeFileNames(string dirname) {

  DIR *dp;
  struct dirent *ep;
  struct stat filestat;
  const char * dirnameC = dirname.c_str();
  dp = opendir(dirnameC);
  vector<string> filesList;
  if (dp != NULL) {

    // for each file inside the data directory
    while ((ep = readdir(dp)) != NULL) {
      if (S_ISDIR(filestat.st_mode)) {continue; }
      if ((strcmp(ep->d_name, ".") == 0) || (strcmp(ep->d_name, "..") == 0)) {continue; }
      string filepath = dirname + "/" + ep->d_name;
      filesList.push_back(filepath);
    }
    closedir (dp);
  }
  else {
    perror ("Could not open the directory!");
  }
  return filesList;
}


/*
 * Based on the used list of training objects
 * for real world new dataset (simulation dataset)
 * A single string value is converted into a single int value
 */
int convertStringToIntCategoryLabel(string inputName) {

	int datasetOption = 1;
	int intLabel;
        const char * nameChar = inputName.c_str();


	if (datasetOption == 0) {
		if (strcmp(nameChar, "Monitor") == 0 ) {
			intLabel = 0;
		}
		else if (strcmp(nameChar, "Keyboard") == 0 ){
			intLabel = 1;
		}
		else if (strcmp(nameChar,"Mouse")== 0 ) {
			intLabel = 2;
		}
		else if (strcmp(nameChar,"Mug")== 0 ) {
			intLabel = 3;
		}
		else if (strcmp(nameChar, "Lamp")== 0 ) {
			intLabel = 4;
		}
		else if (strcmp(nameChar, "Notebook")== 0 ) {
			intLabel = 5;
		}
		else if (strcmp(nameChar, "Laptop")== 0 ) {
			intLabel = 6;
		}
		else if (strcmp(nameChar,"Papers")== 0 ) {
			intLabel = 7;
		}
		else if (strcmp(nameChar, "Book")== 0 ) {
			intLabel = 8;
		}
		else if (strcmp(nameChar,"Mobile")== 0 ) {
			intLabel = 9;
		}
		//else if (strcmp(nameChar, "Flask")== 0 ) {
			//intLabel = 10;
		//}
		else if (strcmp(nameChar, "Glass")== 0 ) {
			intLabel = 10;
		}
		else if (strcmp(nameChar, "Jug")== 0 ) {
			intLabel = 11;
		}
		else if (strcmp(nameChar, "Headphones")== 0 ) {
			intLabel = 12;
		}
		else if (strcmp(nameChar, "Bottle")== 0 ) {
			intLabel = 13;
		}
		else {
			intLabel = -1;
		}																																																																																																																																																												
	}

        ////////////////////////////////////////
	if (datasetOption == 1) {
		if ( strcmp(nameChar, "Monitor") == 0 ) {
		intLabel = 0;
		}
		if ( strcmp(nameChar, "Keyboard") == 0 ) {
		intLabel = 1;
		}
		if (strcmp(nameChar, "Mouse") == 0 ) {
		intLabel = 2;
		}
		if ( strcmp(nameChar, "Cup") == 0 ) {
		intLabel = 3;
		}
		if (strcmp(nameChar, "Lamp") == 0 ) {
		intLabel = 4;
		}
		if (strcmp(nameChar, "Pencil") == 0) {
		intLabel = 5;
		}
		if (strcmp(nameChar, "Laptop") == 0 ) {
		intLabel = 6;
		}

		// new object categories added from the classes present in the simulated scenes
		if (strcmp(nameChar, "Book") == 0) {
		intLabel = 7;
		}
		if (strcmp(nameChar, "Bottle") == 0) {
		intLabel = 8;
		}
		if (strcmp(nameChar, "Calculator") == 0) {
		intLabel = 9;
		}
		if (strcmp(nameChar, "PC") == 0) {
		intLabel = 10;
		}
		if (strcmp(nameChar, "Glass") == 0) {
		intLabel = 11;
		}
		if (strcmp(nameChar, "Headphone") == 0) {
		intLabel = 12;
		}
		if (strcmp(nameChar, "Keys") == 0) {
		intLabel = 13;
		}
		if (strcmp(nameChar, "MobilePhone") == 0) {
		intLabel = 14;
		}
		if (strcmp(nameChar, "Stapler") == 0) {
		intLabel = 15;
		}
		if (strcmp(nameChar, "Telephone") == 0) {
		intLabel = 16;
		}
		else {
			intLabel = -1;
		}
	}

	  return intLabel;


}

vector<int> convertStringToIntCategoryLabelVector(vector<string> stringLabels) {

	vector<int> out;
	for (int i = 0; i < stringLabels.size(); i++) {

		int currentIntLabel = convertStringToIntCategoryLabel(stringLabels.at(i));
		out.push_back(currentIntLabel);

	}
	return out;
}

string convertInt(int number)
{
   stringstream ss;   //create a stringstream
   ss << number;      //add number to the stream
   return ss.str();   //return a string with the contents of the stream
}


bool IsFiniteNumber(float x)
{
    return (x <= DBL_MAX && x >= -DBL_MAX);
}



