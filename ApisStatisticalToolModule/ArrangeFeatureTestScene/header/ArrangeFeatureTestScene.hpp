/*
 * ArrangeFeatureTestScene.hpp
 *
 *  Created on: Nov 25, 2013
 *      Author: marina
 */

#ifndef ARRANGEFEATURETESTSCENE_HPP_
#define ARRANGEFEATURETESTSCENE_HPP_

#include "SingleObjectFeature.hpp"
#include "ObjectPairFeature.hpp"
#include "SceneSingleObjectFeature.hpp"
#include "SceneObjectPairFeature.hpp"



/*
 * Input:
 * the Scene-level Features (both single object and object pair)
 *  - SceneSingleObjectFeature
 *  - SceneObjectPairFeature
 * Output:
 * the information about SOF and OPF at the scene level available into the same class.
  */

class ArrangeFeatureTestScene {

private:

	vector<SingleObjectFeature> listSOF;
	vector<ObjectPairFeature> listOPF;

	vector<vector<ObjectPairFeature> > matrixOPF;

public:

	void arrangeTestFeatures(SceneSingleObjectFeature &, SceneObjectPairFeature &);

	vector<SingleObjectFeature> getListSOF()  { return listSOF; }
	vector<ObjectPairFeature> getListOPF() { return listOPF; }

	vector<vector<ObjectPairFeature> > getMatrixOPF() { return matrixOPF; }

};



#endif /* ARRANGEFEATURETESTSCENE_HPP_ */
