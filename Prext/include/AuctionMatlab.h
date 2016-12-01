
#ifndef AUCTIONMATLAB_H_
#define AUCTIONMATLAB_H_

#include <vector>
using namespace std;

class AuctionMatlab {

    static int findQge0(const vector<int> & q);
    static void findQeqMinusShift(vector<int> &q, int prevWinner);
    static void removeii(vector<int> & q, int ii);
    static int findMax(const vector< vector<double> > & w, int ii, const vector<double> & p) ;
public:

    static vector<int>  run(vector< vector<double> > & w, bool bInteger);

};

#endif /* AUCTIONMATLAB_H_ */
