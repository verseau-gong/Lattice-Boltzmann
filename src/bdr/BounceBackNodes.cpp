/*
 * BounceBackNodes.cpp
 * Andreas Bülling, 2012
 *
 * Full way implementation of the Bounce back rule
 * The boundary nodes are wet nodes.
 *
 * NOTE: updateF() must be called before the streaming
 * step, since the full BB rule is a modification of the
 * collision step. However this is automatically taken
 * care of if LBM::collideAndStream() is called.
 */

#include "BounceBackNodes.h"

template void BounceBackNodes<CollisionD2Q9BGKNSF>::addNode(int x, int y, int z);
template BounceBackNodes<CollisionD2Q9BGKNSF>::BounceBackNodes();

//template void BounceBackNodes<CollisionD2Q9BGKShanChenForce>::addNode(int x, int y, int z);
//template BounceBackNodes<CollisionD2Q9BGKShanChenForce>::BounceBackNodes();

//template void BounceBackNodes<CollisionD2Q9LNP>::addNode(int x, int y, int z);
//template BounceBackNodes<CollisionD2Q9LNP>::BounceBackNodes();

//template void BounceBackNodes<CollisionD2Q9LNPSource>::addNode(int x, int y, int z);
//template BounceBackNodes<CollisionD2Q9LNPSource>::BounceBackNodes();

template <class T>
BounceBackNodes<T>::BounceBackNodes() {
    PRESTREAM = 1;
    cm = NULL;
}

template <class T>
BounceBackNodes<T>::~BounceBackNodes() {}

template <class T>
void BounceBackNodes<T>::updateF(){
	//cout<<"Updating f on BB nodes..."<<endl;

    double *fTemp = new double[lm->UDIRS];
	Node *node;
	int x, y, z, *opp, opp2;

	//print2DArray(f[0], lm->n.x, lm->n.y, 2);
	//	print2DArray(f[0], lm->n.x, lm->n.y, 4);

	for(int i = 0; i < nodes.size(); i++){
		node = nodes[i];
		x = node->x;
		y = node->y;
		z = node->z;
		//cout<<x<<", "<<y<<", "<<z<<endl;

		for(int j = 0; j < lm->UDIRS; j++){
			fTemp[j] = f[z][y][x][j];
		}
		for(int d = 0; d < lm->UDIRS; d++){
			f[z][y][x][d] = fTemp[lm->oppDirs[d]];
		}
	}

	//print2DArray(f[0], lm->n.x, lm->n.y, 2);
	//print2DArray(f[0], lm->n.x, lm->n.y, 4);
}

template <class T>
void BounceBackNodes<T>::addNode(int x, int y, int z){
	nodes.push_back((new Node(x, y, z)));
	cm->addNodeToSkip(x, y);
}
