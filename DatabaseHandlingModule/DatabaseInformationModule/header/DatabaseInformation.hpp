/*
 * DatabaseInformation.hpp
 *
 *  Created on: Nov 18, 2013
 *      Author: marina
 */

#ifndef DATABASEINFORMATION_HPP_
#define DATABASEINFORMATION_HPP_

#include <string.h>
#include <boost/property_tree/ptree.hpp>
#include "Object.hpp"
#include "ApiConvertKTHDB.hpp"
#include "SceneInformation.hpp"
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
#include "ApiConvertSimulationDB.hpp"
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include "ApiConvertRealWorldDB.hpp"


class DatabaseInformation{

private:

  int numberOfScenes;

  vector<SceneInformation> sceneList;

  int numberOfCategories;

public:

  DatabaseInformation(int = 7);

  DatabaseInformation(vector<SceneInformation>, int = 7);

  /*
  Parses all the XML files and adds each scene to the sceneList
  */
  void loadAnnotations_KTH(vector<string>);

  void loadAnnotations_Simulation(string);

  void loadAnnotations_RealWorld(string);

  int getNumberOfScenes() { return numberOfScenes; }

  vector<SceneInformation> getSceneList() { return sceneList; }

  void printSceneInformation();

  void setNumberOfCategories(int n) {numberOfCategories = n; }

  int getNumberOfCategories() { return numberOfCategories; }

};



#endif /* DATABASEINFORMATION_HPP_ */
