/*
 * ApiStatisticsDatabase.hpp
 *
 *  Created on: Nov 19, 2013
 *      Author: marina
 */

#ifndef APISTATISTICSDATABASE_HPP_
#define APISTATISTICSDATABASE_HPP_


#include <string.h>
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
#include <cmath>
#include "utils.hpp"
#include "DatabaseInformation.hpp"
#include "SceneInformation.hpp"
#include "Object.hpp"

#define NOBJECTCLASSES 7

/*
 * Input:
 * The DatabaseInformation
 *
 * Output:
 * frequencies of the object categories occurrences
 * and the object categories co-occurrences (pair of categories)
 *
 * TODO : add also groups of object co-occurrences!!!!
 */

class ApiStatisticsDatabase {

private:

public:

	static vector<double> computeFrequenciesSingleObject(DatabaseInformation &);

	static vector<vector<double> > computeFrequenciesObjectPair(DatabaseInformation &);

	static vector<vector< int> > arrangeFrequencyMatrix(DatabaseInformation &);



};



#endif /* APISTATISTICSDATABASE_HPP_ */
