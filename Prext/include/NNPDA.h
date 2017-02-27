/*
 * NNPDA.h
 *
 *  Created on: Jul 14, 2016
 *      Author: karim
 */

#ifndef NNPDA_H_
#define NNPDA_H_
#include <fstream>
#include <map>
#include <vector>
#include <set>
#include "kalmanTrack.h"
using namespace std;

class EvaluationMetric {
public:
    EvaluationMetric() {
        vehStartTime = vehEndTime = nPsynmChange = nConfusion =  bestTrackId = bestTrackLifetime = 0;
        psynmChngSumEntropy = psynmChngMaxEntropy = 0;
        psynmChngMaxASS = 0;
        lastPsynm = ULONG_MAX;
    }
    uint64_t lastPsynm;
    unsigned int vehStartTime;
    unsigned int vehEndTime;
    unsigned int nPsynmChange;
    unsigned int nConfusion;
    double psynmChngSumEntropy;
    double psynmChngMaxEntropy;
    unsigned int psynmChngMaxASS;
    unsigned int bestTrackLifetime;
    unsigned int bestTrackId;
};

class NNPDA {
    ofstream out2log;
    bool bNNPDAParamInitialized;
    bool bEvaluation;
    bool bRecordPseudonyms;
    bool bDebug;
    double gateSize;
    unsigned int waitBfrDelete;
    unsigned int maxSilence;
    double anonomitySetPthreshold;
    unsigned int curT;
    // skipatfirst
    // recorddrifts

    map <uint64_t, kalmanTrack> trackList;
    map<uint64_t, bcnSample> tmpScan;
    set<uint64_t> encounteredPsynms;
    class AssignTrackTime {
    public:
        AssignTrackTime(unsigned int tr, unsigned int mt, unsigned int ut)
            : trackId(tr), timesteps(1), lastMappingTime(mt), lastTrackUpdateTime(ut) {}
        unsigned int trackId, timesteps, lastMappingTime, lastTrackUpdateTime;
    };

    map<unsigned int, vector<AssignTrackTime> > vehTrackTimeMap;

    void recordInitialEvaluation();
    void recordTrackEvaluation();
    void recordASEntropyEvaluation(map<unsigned int, pair<unsigned int, double> >  & asSizeEntropy);
    void fillUnEncryptedBcns(const map<uint64_t, bcnSample> & rcvdBcns);

    vector< vector<double> > calcAssignmentProbability(const vector<map <uint64_t, kalmanTrack>::iterator> & unmatchedTracks, vector<map <uint64_t, bcnSample>::iterator> & scanIter, map<unsigned int, pair<unsigned int, double> > &asentropy);
    void eliminateBeaconsOfSeenPsynms();
    map <uint64_t, kalmanTrack>::iterator  outdateUnMatchedTrack(map <uint64_t, kalmanTrack>::iterator track);
    void predictTrackList();
    void matchBeaconsToTracksByPsynm(vector<map <uint64_t, kalmanTrack>::iterator> & anonyInactiveSet, vector<map <uint64_t, kalmanTrack>::iterator> & anonyActiveSet);
    void matchBeaconsToTracksByNNPDA(const vector<int> & AssignArr,
    const vector<map <uint64_t, bcnSample>::iterator> & scanIter,
    vector<map <uint64_t, kalmanTrack>::iterator> & AnonySetTracks);
    void createNewTracksforUnmatchedBcns(map<uint64_t, bcnSample> scan);
    void calcVehicleNormEntropy(unsigned int v, const vector< vector<double> > & pTO, double Sm, double & entropy, unsigned int & asSize);
    void printMapping(ofstream &out);

public:
    map<unsigned int, EvaluationMetric> vehEvaluation;

    void initNNPDAParam(double gate_size, unsigned int wait_bfr_delete, unsigned int max_silence);
    inline void enableEvaluation(double anonomity_set_p_thershold = 0.1) { bEvaluation = true;  anonomitySetPthreshold = anonomity_set_p_thershold;  }
    inline void disableRecordingPseudonym() { bRecordPseudonyms = false;    }
    inline void enableDebug() { bDebug = true;  out2log.open("NNPDA.log"); }
    inline unsigned int getCurTimestep() { return curT; }

    void finalizeEvaluation();
    vector<kalmanTrack> getTrackList();
    void trackBeacons(const map<uint64_t, bcnSample> & scan);
    void deleteFarTracks(double distanceSq, double localx, double localy);

    NNPDA();
    virtual ~NNPDA();
};

#endif /* NNPDA_H_ */
