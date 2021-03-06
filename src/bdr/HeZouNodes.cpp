/*
 * HeZouNodes.cpp
 * Andreas Bülling, 2012
 * andreas@bulling.se
 *
 * DESCRIPTION - TODO
 */

#include "HeZouNodes.h"

#define TYPE_LINE_NODE 1
#define TYPE_CORNER_NODE 2
#define TYPE_TIP_NODE 3

#define DIR1(i) 1 + (bdr + (i-1))%4
#define DIR5(i) 5 + (bdr + (i-1))%4

//template void HeZouNodes<CollisionD2Q9LNP>::addNode(int, int, int, double, int);
//template HeZouNodes<CollisionD2Q9LNP>::HeZouNodes();
//template HeZouNodes<CollisionD2Q9LNP>::~HeZouNodes();
//template void HeZouNodes<CollisionD2Q9LNP>::init();

template void HeZouNodes<CollisionD2Q9LPMChaiRHS>::addNode(int, int, int, double, int);
template HeZouNodes<CollisionD2Q9LPMChaiRHS>::HeZouNodes();
template HeZouNodes<CollisionD2Q9LPMChaiRHS>::~HeZouNodes();
template void HeZouNodes<CollisionD2Q9LPMChaiRHS>::init();

template void HeZouNodes<CollisionD2Q9LPMChai1to1>::addNode(int, int, int, double, int);
template HeZouNodes<CollisionD2Q9LPMChai1to1>::HeZouNodes();
template HeZouNodes<CollisionD2Q9LPMChai1to1>::~HeZouNodes();
template void HeZouNodes<CollisionD2Q9LPMChai1to1>::init();

template <class T>
HeZouNodes<T>::HeZouNodes() {
}

template <class T>
HeZouNodes<T>::~HeZouNodes() {
    //deallocate deadNodes - TODO
}

template <class T>
void HeZouNodes<T>::init(){
    cout<<"Preprocessing HE/Zou nodes..."<<endl;
    int i, j;
    TypeValueNode *n;
    for(int k = 0; k < nodes.size(); k++){
        n = nodes[k];
        i = n->x;
        j = n->y;

        //corner nodes...
//        if(i == 0 && j == 0){
//            n->type = TYPE_CORNER_NODE;
//            n->param = 0;
//            continue;
//        }else if(i == lm->n.x-1 && j == 0){
//            n->type = TYPE_CORNER_NODE;
//            n->param = 1;
//            continue;
//        }else if(i == lm->n.x-1 && j == lm->n.y-1){
//            n->type = TYPE_CORNER_NODE;
//            n->param = 2;
//            continue;
//        }else if(i == 0 && j == lm->n.y-1){
//            n->type = TYPE_CORNER_NODE;
//            n->param = 3;
//            continue;
//        }

        //border nodes
        if(n->v3 == 2){ //
            n->type = TYPE_LINE_NODE;
            n->param = 1;
        }else if(n->v3 == 4){
            n->type = TYPE_LINE_NODE;
            n->param = 3;
        }else if(n->v3 == 1){
            n->type = TYPE_LINE_NODE;
            n->param = 0;
        }else if(n->v3 == 3){
            n->type = TYPE_LINE_NODE;
            n->param = 2;
        }

        //inner nodes
        for(int bdr = 0; bdr < 4; bdr++){ //four eq. cases

        }

        if(n->param == -1 || n->type == -1) cerr<<"Warning: uninitialized HeZouNode!"<<endl;
    }
}

template <class T>
void HeZouNodes<T>::updateF(){
   // cout<<"Updating Chai boundary nodes..."<<endl;
    int i, j;
    double temp, rho, psi;
    for(int k = 0; k < nodes.size(); k++){
        i = nodes[k]->x;
        j = nodes[k]->y;
        rho = nodes[k]->v1;
      //  cout<<"RHO IN HE/ZOU: "<<rho<<endl;
      //  cout<<"NORM: "<<nodes[k]->v3<<endl;
        switch(nodes[k]->type){
        case TYPE_LINE_NODE:
            updateNode(i, j, rho, nodes[k]->param);
            break;
        case TYPE_CORNER_NODE:
            updateCornerNode(i, j, rho, nodes[k]->param);
            break;
        case TYPE_TIP_NODE:
            //updateCornerNode(i, j, rho, psi, nodes[k]->param);
            cerr<<"NO IMPLEMENTATION FOR TIP NODES!"<<endl;
            break;
        }
    }
}

template <class T>
void HeZouNodes<T>::updateNode(int i, int j, double rho, int bdr){
    double feqDiff = fEq(DIR1(1), i, j) - fEq(DIR1(3), i, j);
  //  cout<<"updating middle node..."<<endl;
   // cout<<"f_"<<DIR1(1)<<" = f_"<<DIR1(3)<<endl;
    f[0][j][i][DIR1(1)] = f[0][j][i][DIR1(3)] + feqDiff;
   // cout<<"f_"<<DIR5(5)<<"= -f_"<<DIR5(6)<<endl;
    f[0][j][i][DIR5(5)] = - f[0][j][i][DIR1(3)] \
                          - f[0][j][i][DIR5(6)] \
                          - f[0][j][i][DIR1(2)] \
                          - 0.5*feqDiff + 0.5*rho;
   // cout<<"f_"<<DIR5(8)<<endl;
    f[0][j][i][DIR5(8)] = - f[0][j][i][DIR1(3)] \
                          - f[0][j][i][DIR1(4)] \
                          - f[0][j][i][DIR5(7)] \
                          - 0.5*feqDiff + 0.5*rho;
}

template <class T>
void HeZouNodes<T>::updateCornerNode(int i, int j, double rho, int bdr){
    //cout<<"updating corner node..."<<endl;
    double feqDiff1_3 = fEq(DIR1(1), i, j) - fEq(DIR1(3), i, j);
    double feqDiff2_4 = fEq(DIR1(2), i, j) - fEq(DIR1(4), i, j);
    f[0][j][i][DIR1(1)] = f[0][j][i][DIR1(3)] + feqDiff1_3;
    f[0][j][i][DIR1(2)] = f[0][j][i][DIR1(4)] + feqDiff2_4;
    f[0][j][i][DIR5(5)] = f[0][j][i][DIR5(7)] - 0.5*(feqDiff1_3 + feqDiff2_4);
    f[0][j][i][DIR5(8)] = - f[0][j][i][DIR1(3)] \
                          - f[0][j][i][DIR1(4)] \
                          - f[0][j][i][DIR5(7)] \
                          - 0.5*feqDiff1_3 + 0.5*rho;
    f[0][j][i][DIR5(6)] = - f[0][j][i][DIR1(3)] \
                          - f[0][j][i][DIR1(4)] \
                          - f[0][j][i][DIR5(7)] \
                          - 0.5*feqDiff2_4 + 0.5*rho;

}

template <class T>
void HeZouNodes<T>::addNode(int x, int y, int z, double val, int dir){
    TypeValueNode *v = new TypeValueNode(x, y, z, val, -1, -1);
    v->v2 = val; //backup undestroyed value
    v->v3 = dir; //direction
    nodes.push_back(v);
}

