/*
 * ApiGraph.hpp
 *
 *  Created on: Dec 13, 2013
 *      Author: marina
 */

#ifndef APIGRAPH_HPP_
#define APIGRAPH_HPP_


#include "SceneInformation.hpp"
#include "Object.hpp"
//#include "Test.hpp"


/*
 * Input:
 *
 * * the object IDs of the objects in the test scene
 * * the category list indicating the possible categories
 *
 * Output:
 * * the list of all possible paths for the exhaustive search *
 * */

typedef map<int, int> path;
typedef pair<int, int> idCategoryPair;
typedef pair<idCategoryPair, double> pairScore;

class ApiGraph {

private:

	vector<int> objectids;
	vector<int> categoryList;
	vector<path> allPaths;
	vector<vector<pairScore> > votingTable;

public:

	ApiGraph(vector<int> , vector<int> );
	ApiGraph(vector<vector<pairScore> >);

	void findAllPaths();
	void findAllPathsUtil(int nobjects, int objectidIndex, int categoryIndex, path & mypath);
	void findAllPathsShortlisted();
	void findAllPathsShortlistedUtil(int nobjects, int objectidIndex, int categoryIndex, path & mypath);

	void printAllPaths();

	vector<path> getAllPaths() { return allPaths; }

	vector<int> getObjectIds() {return objectids; }
	vector<int> getCategoryList() {return categoryList; }
	vector<vector<pairScore> > returnVotingTable() { return votingTable; }

};



#endif /* APIGRAPH_HPP_ */
