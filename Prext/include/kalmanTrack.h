/*
 * kalman.h
 *
 *  Created on: Jul 13, 2016
 *      Author: karim
 */

#ifndef KALMANTRACK_H_
#define KALMANTRACK_H_

#include <Eigen/Dense>
#include <stdint.h>
using namespace Eigen;

class bcnSample {
public:
   bcnSample() { bEncrypted = false;  vehId = psynm = 0; timestampe = 0; }

   double timestampe, px, vx, py, vy, angle;
   uint64_t psynm;
   unsigned int vehId;
   bool bEncrypted;
};
class kalmanTrack {

    static int p0;
    static double q;
    static double rp;
    static double rv;
    static double bcnT; // beacon interval
    static unsigned int lastTrackId;
    static bool bKalmanParamInitialized;
    void init();
public:
    inline static unsigned int getLastTrackId() { return lastTrackId;}
    static void initKalmanParam(double bcnInterval, double _p0 = 50, double _q = 1, double _rp = 7, double _rv = 2) {
        lastTrackId = 0;
        p0 = _p0;
        q = _q;
        rp = _rp;
        rv = _rv;
        bcnT = bcnInterval; // beacon interval
        bKalmanParamInitialized = true;
    }

    bool bActive;
    unsigned int id;
    unsigned int lifeTime;
    unsigned int lastUpdateTime;
    unsigned int vehId;
    uint64_t pseudonym;

    MatrixXd H, F, P, Q, S, R, K, Sinv;
    VectorXd x;
    double Sdet;

    kalmanTrack();
    kalmanTrack(bcnSample bcn, unsigned int tm);
    virtual ~kalmanTrack();

    void predict();
    void update(bcnSample z, unsigned int tm);
    double calcD(const bcnSample & bcn) ;

};

#endif /* KALMANTRACK_H_ */
