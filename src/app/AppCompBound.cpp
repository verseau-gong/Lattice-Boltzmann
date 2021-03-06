/*
 * AppPoiseuilleForce.cpp
 * Andreas Bülling, 2012
 * DESCRIPTION - TODO
 */

#include <iostream>
#include "../LBM.h"
#include <math.h>

using namespace std;

int main(){
	int nx = 101, ny = 101, tMax = 100;
	double w = 0.75;
	double c = 1.0;

	double **fx = allocate2DArray(ny, nx);
    double **fy = allocate2DArray(ny, nx);

	cout<<"Forced poiseuille flow..."<<endl;
	LatticeModel *lm = new Lattice2D(nx, ny);
	StreamD2Q9Periodic *sm = new StreamD2Q9Periodic();
	CollisionD2Q9BGKNSF *cm = new CollisionD2Q9BGKNSF();

    cm->setW(w);
    cm->setC(c);

	LBM *lbm = new LBM(lm, cm, sm);

	/* Set boundary conditions*/
	BounceBackNodes<CollisionD2Q9BGKNSF> *bbns =
	        new BounceBackNodes<CollisionD2Q9BGKNSF>();
	bbns->setCollisionModel(cm);
//	for(int i = 0; i < nx; i++){
//        bbns->addNode(i, 0, 0);
//        bbns->addNode(i, ny-1, 0);
//	}

//    int xc = nx/4, yc = ny/3;
//    double r = ny/10.0;
//    for(int i = 0; i < nx; i++){
//        for(int j = 0; j < ny; j++){
//            if(sqrt((double)((i - xc)*(i - xc) + (j - yc)*(j - yc))) < r){
//                bbns->addNode(i, j, 0);
//            }
//        }
//    }

//    xc = nx*2/4; yc = ny/2;
//    r = ny/7.5;
//    for(int i = 0; i < nx; i++){
//        for(int j = 0; j < ny; j++){
//            if(sqrt((double)((i - xc)*(i - xc) + (j - yc)*(j - yc))) < r){
//                bbns->addNode(i, j, 0);
//            }
//        }
//    }

    for(int j = 0; j < ny/3; j++){
        bbns->addNode(nx*3/4, j, 0);
        bbns->addNode(nx*3/4+1, j, 0);
    }

    for(int j = 0; j < ny/4; j++){
        bbns->addNode(nx*3/4, ny-1-j, 0);
        bbns->addNode(nx*3/4+1, ny-1-j, 0);
    }

    int x = nx/2, y = ny/2, h = 20, ww = 20;
	for(int i = x; i < x + ww + 1; i++){
		bbns->addNode(i, y, 0);
		bbns->addNode(i, y + h, 0);
	}

	for(int j = y + 1; j < y + h; j++){
		bbns->addNode(x, j, 0);
		bbns->addNode(x + ww, j, 0);
	}
	lbm->addBoundaryNodes(bbns);

	/* Set force */
	for(int i = 0; i < nx; i++){
		for(int j = 0; j < ny; j++){
		    fx[j][i] = 0.0001;
		    fy[j][i] = 0.0;
		}
	}
    cm->setForce(fx, fy);

	/* Initialize solver */
	lbm->init();

	/* Main loop */
	for(int t = 0; t < tMax; t++){
		cout<<t<<endl;
		lbm->collideAndStream();
	}

	cm->dataToFile("vis_scripts/bench_force_poi/");
	cout<<"done cyl."<<endl;

	return 0;
}
