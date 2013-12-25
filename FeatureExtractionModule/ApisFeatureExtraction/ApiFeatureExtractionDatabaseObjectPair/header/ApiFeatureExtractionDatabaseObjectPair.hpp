/*
 * ApiFeatureExtractionDatabaseObjectPair.hpp
 *
 *  Created on: Nov 23, 2013
 *      Author: marina
 */

#ifndef APIFEATUREEXTRACTIONDATABASEOBJECTPAIR_HPP_
#define APIFEATUREEXTRACTIONDATABASEOBJECTPAIR_HPP_

#include "ApiFeatureExtractionSceneObjectPair.hpp"
#include "DatabaseObjectPairFeature.hpp"
#include "DatabaseInformation.hpp"
#include <string>
#include <vector>


using namespace std;

class ApiFeatureExtractionDatabaseObjectPair {

private:

public:

static void extract(DatabaseInformation &, DatabaseObjectPairFeature & );

};


#endif /* APIFEATUREEXTRACTIONDATABASEOBJECTPAIR_HPP_ */
