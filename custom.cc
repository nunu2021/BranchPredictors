/* 
 * CMU 18-740 
 * Fall 2019
 * Lab 1
*/

#include "cpu/pred/custom.hh"
#include <math.h>
#include <iostream>

CustomBP::CustomBP(const CustomBPParams *params)
    : BPredUnit(params)
    , pcBits(params->pcBits)
    , historyLength(params->historyLength)
    , localHistoryLength(params->localHistoryLength)


    
    // initialize other variables here
{
     // Initialize prediction table and branch history
    int tableLength = pow(2, historyLength);
    predictionTable = (int*) malloc(tableLength * sizeof(int));
    for (int i = 0; i < tableLength; i++) {
        predictionTable[i] = 3;
    }
    currentHistory = 0;

    unsigned int BHTlength = 1 << params->pcBits;
    BHT = new int[BHTlength];
    for(unsigned int i = 0; i < BHTlength; i++){
        BHT[i] = 0;
    }

    visited = new unsigned int[BHTlength];
    for(unsigned int i = 0; i < BHTlength; i++){
        visited[i] = 0;
    }

    unsigned int PHTLength = 1 << localHistoryLength;
    PHT = new int[PHTLength];
    for(unsigned int i = 0; i < PHTLength; i++){
        PHT[i] = 3;
    }
}






bool
CustomBP::lookup(ThreadID tid, Addr branchAddr, void * &bpHistory)


{

    // local prediciton
    unsigned int mask = (1 << pcBits) - 1;
    bool localPrediciton =  (PHT[BHT[branchAddr & mask]] > 3);

    // global prediction
    bool globalPrediction = false;
    if (predictionTable[currentHistory] >= 4) {
        globalPrediction = true;
    }

    // if the branch is new, then use the global prediciton
    if(visited[branchAddr & mask] < pcBits){
        return globalPrediction;
    }

    // else use the local prediction
    return localPrediciton;
  

}

void
CustomBP::update(ThreadID tid, Addr branchAddr, bool taken, void *bpHistory,
                 bool squashed, const StaticInstPtr & inst, Addr corrTarget)
{
    // update local history BP
    unsigned int mask = (1 << pcBits) - 1;
    unsigned int phtMask = (1 << localHistoryLength) -1;

    visited[branchAddr & mask]++;

    int found = false;

    // update the state of the lookup in PHT
    if(taken){
        found = std::min(7, PHT[BHT[(branchAddr) & mask]]+1);
    }else{
        found = std::max(0, PHT[BHT[(branchAddr) & mask]]-1);
    }

    
    PHT[BHT[branchAddr & mask]] = found;

    // update the BHT
    BHT[(branchAddr) & mask] = (BHT[(branchAddr) & mask] << 1) & phtMask;
    if(taken){
        BHT[branchAddr & mask] = BHT[(branchAddr) & mask] | 1;
    }

    // Update global history and prediction table
    if (taken) {
        predictionTable[currentHistory] += 1;
        if (predictionTable[currentHistory] > 7) {
            predictionTable[currentHistory] = 7;
        }
        currentHistory = (currentHistory / 2) + pow(2, historyLength - 1);
    } else {
        predictionTable[currentHistory] -= 1;
        if (predictionTable[currentHistory] < 0) {
            predictionTable[currentHistory] = 0;
        }
        currentHistory = (currentHistory / 2);
    }


   
}



/**
 * Important function to initialize BP with Python parameters
 *
 * You should not have to modify this.
 */
CustomBP*
CustomBPParams::create()
{
    return new CustomBP(this);
}
