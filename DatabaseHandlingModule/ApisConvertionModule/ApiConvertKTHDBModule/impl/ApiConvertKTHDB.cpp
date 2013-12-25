/*
 * ApiConvertKTHDB.cpp
 *
 *  Created on: Nov 18, 2013
 *      Author: marina
 */

#include "ApiConvertKTHDB.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <pcl/common/transforms.h>
#include <pcl/common/eigen.h>
#include <iostream>


#define DEBUG 0

using namespace boost::property_tree;


static const std::string TAG_SCENARIO = "scenario";
static const std::string TAG_NAME = "name";
static const std::string TAG_OBJECT = "object";
static const std::string TAG_NOBJECTS = "numberOfObjects";
static const std::string TAG_ALLOBJECTS = "allObjects";
static const std::string TAG_POSE = "pose";
static const std::string TAG_DIMENSIONS = "dimensions";
static const std::string TAG_LENGTH = "length";
static const std::string TAG_WIDTH = "width";
static const std::string TAG_HEIGHT = "height";
static const std::string TAG_COLOR = "color";
static const std::string TAG_INDICES = "indices";



/*
Parses the XML file. Stores the data about the reference (table) and the other objects in the
scene into an object of class "SceneInformation", which is passed by reference as an argument.
*/
void ApiConvertKTHDB::parseFileXML(string fileNameXML, SceneInformation & mySceneInformation){

  boost::property_tree::ptree root;
  read_xml(fileNameXML, root);
  std::string scenarioType = root.get<std::string>(TAG_SCENARIO + "." + "type");
  mySceneInformation.setType(scenarioType);

  if (DEBUG) {
    std::cout << "From Api: scenario type =  "<< scenarioType << endl;
    std::cout << "From getType function: the scenario type is " << mySceneInformation.getType() << endl;
  }

  ptree& tableDimensions = root.get_child(TAG_SCENARIO + "." + TAG_DIMENSIONS);
  float  _deskLength = tableDimensions.get<float>(TAG_LENGTH);
  float  _deskWidth = tableDimensions.get<float>(TAG_WIDTH);
  mySceneInformation.setReferenceLength(_deskLength);
  mySceneInformation.setReferenceWidth(_deskWidth);
  mySceneInformation.setReferenceCentroid();

  if (DEBUG) {
    cout << "Parsing the XML file. The Scene Desk parameters have been set to :" << endl
         << "Length = " << mySceneInformation.getReferenceLength() << endl
         << "Width = " <<  mySceneInformation.getReferenceWidth() << endl;
  }

  boost::property_tree::ptree& allObjects = root.get_child(TAG_SCENARIO + "." + TAG_ALLOBJECTS);
  boost::property_tree::ptree::iterator it = allObjects.begin();

  it++;

  for(; it != allObjects.end(); it++){
    if (DEBUG) { cout << "New object will be parsed" << endl;    }
    parseObject(it->second, mySceneInformation);
  }

}


/*
Parses a single object in the scene. Adds the information about the object to
the object_instnace of class "SceneInformation" which is passed by reference
to the function as an argument.
*/
void ApiConvertKTHDB::parseObject(boost::property_tree::ptree & parent, SceneInformation & mySceneInformation){
  Object newObject;
  if (DEBUG) {
  cout << "Inside the parseObject function" << endl;
  }
  float x, y, z, roll, pitch, yaw, length, width, height;
  boost::property_tree::ptree& pose = parent.get_child(TAG_POSE);
  x = pose.get<float>("x");
  y = pose.get<float>("y");
  z = pose.get<float>("z");
  roll = pose.get<float>("roll");
  pitch = pose.get<float>("pitch");
  yaw = pose.get<float>("yaw");
  boost::property_tree::ptree& dimensions = parent.get_child(TAG_DIMENSIONS);
  length = dimensions.get<float>(TAG_LENGTH);
  width = dimensions.get<float>(TAG_WIDTH);
  height = dimensions.get<float>(TAG_HEIGHT);
  if (DEBUG) {
    cout << "The parameters are: " << x << " " << y << " " << z << " " <<
        roll << " " << pitch << " " << yaw << " " << length << " " << width
       << " " << height << endl;
  }
  setmapKTHparameters(x, y, z, roll, pitch, yaw, length, width, height);
  if (DEBUG) {
  cout << "Inside the parseObject function: saved KTH parameters inside map." << endl;
  }
  vector<pcl::PointXYZ> myboundingBoxPoints = convertObjectParameters();
  newObject.setBoundingBox(myboundingBoxPoints);
  newObject.setCentroid();

  newObject.setInstanceName(parent.get<std::string>(TAG_NAME));

  // setting the name sets also the numeric object ID
  newObject.setObjectName(parent.get<std::string>(TAG_NAME));


  int currentnumber =  mySceneInformation.getNumberOfObjects();
  newObject.setInstanceID(currentnumber);


  string currentName = parent.get<std::string>(TAG_NAME);
  const char * currentNameChar = currentName.c_str();

  if (newObject.getActualObjectID() != -1) {
    mySceneInformation.addObject(newObject);
  }
  if (DEBUG) {
  cout << "Inside the parseObject function: Added object " << parent.get<std::string>(TAG_NAME) << endl;
  }
}


void ApiConvertKTHDB::setmapKTHparameters(float x, float y, float z, float roll, float pitch, float yaw, float length, float width, float height) {
  mapKTHparameters["x"] = x;
  mapKTHparameters["y"] = y;
  mapKTHparameters["z"] = z;
  mapKTHparameters["roll"] = roll;
  mapKTHparameters["pitch"] = pitch;
  mapKTHparameters["yaw"] = yaw;
  mapKTHparameters["length"] = length;
  mapKTHparameters["height"] = height;
  mapKTHparameters["width"] = width;
}


vector<pcl::PointXYZ> ApiConvertKTHDB::convertObjectParameters(){
  if (DEBUG) {
  cout << "Inside the convertObjectParameters function" << endl;
  }
  float x = mapKTHparameters.find("x")->second;
  float y = mapKTHparameters.find("y")->second;
  float z = mapKTHparameters.find("z")->second;
  float roll = mapKTHparameters.find("roll")->second;
  float pitch = mapKTHparameters.find("pitch")->second;
  float yaw = mapKTHparameters.find("yaw")->second;
  float length = mapKTHparameters.find("length")->second;
  float width = mapKTHparameters.find("width")->second;
  float height = mapKTHparameters.find("height")->second;

  // work on a point cloud
  //pcl::PointCloud<pcl::PointXYZ> myBoundingBox;

  Eigen::Affine3f myTransformation;

  myTransformation = pcl::getTransformation(x,y,z,roll,pitch,yaw);
  pcl::PointXYZ _FLDPoint, _FRDPoint, _BLDPoint, _BRDPoint, _FLUPoint, _FRUPoint, _BLUPoint, _BRUPoint;

  // Lower face vertices
  // Front face, left lower point
  _FLDPoint.x = 0;
  _FLDPoint.y = 0;
  _FLDPoint.z = 0;
  _FLDPoint = pcl::transformPoint(_FLDPoint, myTransformation);
   // Front face, right lower point
  _FRDPoint.x = length;
  _FRDPoint.y = 0;
  _FRDPoint.z = 0;
  _FRDPoint = pcl::transformPoint(_FRDPoint, myTransformation);
  // Back face, left lower point
  _BLDPoint.x = length;
  _BLDPoint.y = width;
  _BLDPoint.z = 0;
  _BLDPoint = pcl::transformPoint(_BLDPoint, myTransformation);
  // Left face, right lower point
  _BRDPoint.x = 0;
  _BRDPoint.y = width;
  _BRDPoint.z = 0;
  _BRDPoint = pcl::transformPoint(_BRDPoint, myTransformation);

  // Upper face vertices
  // Front face, left upper point
  _FLUPoint.x = 0;
  _FLUPoint.y = 0;
  _FLUPoint.z = height;
  _FLUPoint = pcl::transformPoint(_FLUPoint, myTransformation);
   // Front face, right upper point
  _FRUPoint.x = length;
  _FRUPoint.y = 0;
  _FRUPoint.z = height;
  _FRUPoint = pcl::transformPoint(_FRUPoint, myTransformation);
  // Back face, left upper point
  _BLUPoint.x = length;
  _BLUPoint.y = width;
  _BLUPoint.z = height;
  _BLUPoint = pcl::transformPoint(_BLUPoint, myTransformation);
  // Left face, right upper point
  _BRUPoint.x = 0;
  _BRUPoint.y = width;
  _BRUPoint.z = height;
  _BRUPoint = pcl::transformPoint(_BRUPoint, myTransformation);

  // // myBoundingBox->points[0].x = 0.0; boundingBox->points[0].y = 0.0; boundingBox->points[0].z = 0.0;
 // // myBoundingBox.push_back(_FLDPoint);

  vector<pcl::PointXYZ> out;
  out.push_back(_FLDPoint);
  out.push_back(_FRDPoint);
  out.push_back(_BLDPoint);
  out.push_back(_BRDPoint);
  out.push_back(_FLUPoint);
  out.push_back(_FRUPoint);
  out.push_back(_BLUPoint);
  out.push_back(_BRUPoint);

  if (DEBUG) {
  cout << "Inside the convertObjectParameters function: after pushing back ALL points into the vector of points " << endl;
  }
  return out;
}



