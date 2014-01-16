/*
 * ApiObjectPairFeature.cpp
 *
 *  Created on: Nov 18, 2013
 *      Author: marina
 */

#include "ApiFeatureExtractionObjectPair.hpp"

#define DEBUG 0

void ApiFeatureExtractionObjectPair::extractFeatures(Object & refObject, Object & targetObject, ObjectPairFeature & out){

	int objectID1 =  refObject.getActualObjectID();
	out.setObjectID1(objectID1);
	int objectID2 =  targetObject.getActualObjectID();
	out.setObjectID2(objectID2);

	string instanceName1 = refObject.getInstanceName();
	string instanceName2 = targetObject.getInstanceName();
	out.setInstanceName1(instanceName1);
	out.setInstanceName2(instanceName2);


	int instanceID1 = refObject.getInstanceID();
	int instanceID2 = targetObject.getInstanceID();
	out.setInstanceID1(instanceID1);
	out.setInstanceID2(instanceID2);

	computeEuclideanDistance(refObject, targetObject);
	computeEuclideanDistance2d(refObject, targetObject);
	computeOrientation2d(refObject, targetObject);
	computeSizeDifference(refObject, targetObject);
	computeVerticalHeightDifference(refObject, targetObject);

	out.setEuclideanDistance(EuclideanDistance);
	out.setEuclideanDistance2d(EuclideanDistance2d);
	out.setOrientation2d(orientation2d);
	out.setSizeDifference(sizeDifference);
	out.setVerticalHeightDifference(verticalHeightDifference);

}


void ApiFeatureExtractionObjectPair::computeEuclideanDistance(Object & refObject, Object & targetObject) {

  pcl::PointXYZ centroidReferenceObject = refObject.getCentroid();
  pcl::PointXYZ centroidTargetObject = targetObject.getCentroid();
  EuclideanDistance = pcl::euclideanDistance( centroidReferenceObject, centroidTargetObject);

}


void ApiFeatureExtractionObjectPair::computeEuclideanDistance2d(Object & refObject, Object & targetObject) {

  pcl::PointXYZ centroidReferenceObject = refObject.getCentroid();
  centroidReferenceObject.z = 0;
  pcl::PointXYZ centroidTargetObject = targetObject.getCentroid();
  centroidTargetObject.z = 0;
  EuclideanDistance2d = pcl::euclideanDistance( centroidReferenceObject, centroidTargetObject);

  if (DEBUG) {
    cout << "The Euclidean distance 2d between the 2 objects is : " << EuclideanDistance2d
         << endl;
    cout << "   when REFobject centroid x = "
         << (refObject.getCentroid()).x << " and  y = "
         << (refObject.getCentroid()).y << endl;
      cout << " and TARGETobject centroid x = "
         << (targetObject.getCentroid()).x << " and y = "
         << (targetObject.getCentroid()).y << endl;
  }
}


void ApiFeatureExtractionObjectPair::computeOrientation2d(Object & refObject, Object & targetObject) {

  float dx = (targetObject.getCentroid()).x - (refObject.getCentroid()).x;
  float dy = (targetObject.getCentroid()).y - (refObject.getCentroid()).y;
  float slope = ( dy ) / ( dx );
  float _angle = atan2 (dy, dx);
  // float dx_horizontal =

  if ( _angle < 0 ) {
    _angle = _angle + 2 * M_PI;
  }
  orientation2d = _angle;

  if (DEBUG) {

    cout << "The angle between the 2 objects is : " << _angle
          << " = in degrees " << ( _angle  * 180 / 3.14) << endl;
    cout << "   when REFobject centroid x = "
         << (refObject.getCentroid()).x << " and  y = "
         << (refObject.getCentroid()).y << endl;
      cout << " and TARGETobject centroid x = "
         << (targetObject.getCentroid()).x << " and y = "
         << (targetObject.getCentroid()).y << endl;
  }

}


void ApiFeatureExtractionObjectPair::computeSizeDifference(Object & refObject, Object & targetObject) {

  pcl::PointCloud<pcl::PointXYZ> targetBoundingBox = targetObject.getBoundingBox();
  float dim1 = pcl::euclideanDistance(targetBoundingBox.points[1], targetBoundingBox.points[0]);
  float dim2 = pcl::euclideanDistance(targetBoundingBox.points[3], targetBoundingBox.points[0]);
  float dim3 = pcl::euclideanDistance(targetBoundingBox.points[4], targetBoundingBox.points[0]);
  float sizeTarget = (dim1 * dim2 * dim3);

  pcl::PointCloud<pcl::PointXYZ> referenceBoundingBox = refObject.getBoundingBox();
  float dim1r = pcl::euclideanDistance(referenceBoundingBox.points[1], referenceBoundingBox.points[0]);
  float dim2r = pcl::euclideanDistance(referenceBoundingBox.points[3], referenceBoundingBox.points[0]);
  float dim3r = pcl::euclideanDistance(referenceBoundingBox.points[4], referenceBoundingBox.points[0]);
  float sizeReference = (dim1r * dim2r * dim3r);

  sizeDifference = sizeTarget / sizeReference;

  if (DEBUG)  {
    cout << "The difference is SIZE between the 2 objects is : " << sizeDifference << endl;
  }
}

void ApiFeatureExtractionObjectPair::computeVerticalHeightDifference(Object & refObject, Object & targetObject) {

   verticalHeightDifference = (targetObject.getCentroid()).z - (refObject.getCentroid()).z;

   if (DEBUG)  {
    cout << "The difference is Vertical Height between the 2 objects is : " << verticalHeightDifference << endl;
  }
}



