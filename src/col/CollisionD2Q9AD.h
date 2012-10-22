/*
 * CollisisonD2Q9AD.h
 * Andreas Bülling, 2012
 * andreas@bulling.se
 *
 * DESCRIPTION - TODO
 */

#ifndef COLLISISOND2Q9AD_H_
#define COLLISISOND2Q9AD_H_

#include "CollisionD2Q9BGK.h"

class CollisionD2Q9AD: public CollisionD2Q9 {
public:
    CollisionD2Q9AD();
    virtual ~CollisionD2Q9AD();
    double fEq(int d, double rho, double jx, double jy);
    void collide();
    void setUx(double **ux){this->ux = ux;};
    void setUy(double **uy){this->uy = uy;};
    void setDPsix(double **dPsix){this->dPsix = dPsix;};
    void setDPsiy(double **dPsiy){this->dPsiy = dPsiy;};
    void setPe(double Pe){this->Pe = Pe;};
    void setZ(double z){this->z = z;};
    void setT(double T){this->T =T;};
    void setW(double w){this->w = w;};
    void setInitC(double C){this->initC = C;};
    void setRHS(double **rhs){this->rhs = rhs;};
    double **getNi(){return rho;};
    double **getdPsix(){return dPsix;};
    double **getdPsiy(){return dPsiy;};
    double getZ(){return z;};
    double getT(){return T;};
    void init();
    void dataToFile(string path);
protected:
    void updateRho();
    double Pe, z, T, w, initC;
    double **rho, **ux, **uy, **dPsix, **dPsiy, **rhs;
};

#endif /* COLLISISOND2Q9AD_H_ */