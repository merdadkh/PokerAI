
#include <iostream>
#include "HandEval.h"
#include "TestCases.h"
//#include "PlayerParent.h"
//#include "Dealer.h"
#include "../project_acpc_server/CPlayer.h"
#include "ProbEval.h"


int main(int argc, char *argv[]) {

    
    string handStr = "AsQs";

    unsigned long long handMask = CHandEval::ParseHand(handStr);
    size_t hand_index = CHandIterator::PocketMask2IndexMap.find(handMask)->second;

    cout << "HandIndex: " << hand_index << endl;

    double sum = 0, WinProb = -1;
    int count = 0;
    vector<vector<double> >* vec = CProbEval::LoadPreFlopProb_fromFile();
        for (size_t i = 0; i < POCKET_HAND_COUNT; i++) {
          //  cout << vec->at(hand_index).at(i) << "\t";
            if (vec->at(hand_index).at(i) >= 0)
            {
                sum += vec->at(hand_index).at(i);
                count++;    
            }	
        }
        WinProb = sum / count;

        cout << "WinProb: " << WinProb << "\t Sum / Count :" << sum << "/"<<count;
    
    
    return 0;

}