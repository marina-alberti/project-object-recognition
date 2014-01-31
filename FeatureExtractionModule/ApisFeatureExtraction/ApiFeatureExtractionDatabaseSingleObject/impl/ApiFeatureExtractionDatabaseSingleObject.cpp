/*
 * ApiFeatureExtractionDatabaseSingleObject.cpp
 *
 *  Created on: Nov 23, 2013
 *      Author: marina
 */


#include "ApiFeatureExtractionDatabaseSingleObject.hpp"

/*
 * Input:
 * a database ("DatabaseInformation" object)
 * Output:
 * the features of "single object" organized at the database-level ("DatabaseSingleObjectFeature" object)
 *  */
void ApiFeatureExtractionDatabaseSingleObject::extract(DatabaseInformation & database, DatabaseSingleObjectFeature & out) {

	// gets the list of all the scenes in the database
	vector<SceneInformation> sceneList = database.getSceneList();

	// for each scene in the database
	for (vector<SceneInformation>::iterator it = sceneList.begin(); it != sceneList.end(); it++ ) {

		SceneSingleObjectFeature currentObjectFeature;

		// extracts the features in the current scene (features organized at the scene level)
		// ApiFeatureExtractionSceneSingleObject::extract( *it, currentObjectFeature );
		ApiFeatureExtractionSceneSingleObject::extractNoReference( *it, currentObjectFeature );


		// adds the features organized at the scene level to the features organized at the database level in output
		out.addSceneSingleObjectFeature(currentObjectFeature);

    }

	int numberOfCategories = database.getNumberOfCategories();
	out.setNumberOfCategories(numberOfCategories);


}

