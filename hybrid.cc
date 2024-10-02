/* 
 * CMU 18-740 
 * Fall 2019
 * Lab 1
*/

#include "cpu/pred/hybrid.hh"
#include <math.h>
#include <iostream>

HybridBP::HybridBP(const HybridBPParams *params)
    : BPredUnit(params)
    , pcBits(params->pcBits)
    , localHistoryLength(params->localHistoryLength)
    , gshareHistoryLength(params->gshareHistoryLength)
    // initialize other variables here
{
    // Initialize local history
    int BHTLength = pow(2, pcBits);
    localBHT = new int[BHTLength]; //(int*) calloc(BHTLength, sizeof(int));

    int localPHTLength = pow(2, localHistoryLength);
    localPHT = new int*[BHTLength]; //(int**) malloc(BHTLength * sizeof(int*));
    for (int i = 0; i < BHTLength; i++) {
        localPHT[i] = new int[localPHTLength]; //(int*) malloc(localPHTLength * sizeof(int));
        for (int j = 0; j < localPHTLength; j++) {
            localPHT[i][j] = 3;
        }
    }

    // Initialize global history
    int gshareHistoryTableLength = pow(2, gshareHistoryLength);
    globalHistory = new int[gshareHistoryTableLength]; //(int*) malloc(gshareHistoryTableLength * sizeof(int));
    for (int i = 0; i < gshareHistoryTableLength; i++) {
        globalHistory[i] = 3;
    }
    currentHistory = 0;

    // Initialize meta-predictor
    metaHistory = new int[BHTLength]; //(int*) malloc(BHTLength * sizeof(int));
    for (int i = 0; i < BHTLength; i++) {
        metaHistory[i] = 3;
    }

    // Initialize helper variables
    gPrediction = false;
    lPrediction = false;
}

bool
HybridBP::lookup(ThreadID tid, Addr branchAddr, void * &bpHistory)
{
    int pcEnd = (branchAddr) & ((1 << pcBits) - 1); //(branchAddr & ((1 << (pcBits + 2)) - 1)) >> 2;
    int metaPrediction = metaHistory[pcEnd];

    // int index = pcEnd ^ currentHistory;
    int index = (branchAddr & ((1 << gshareHistoryLength) - 1)) ^ currentHistory;
    int gPredictionValue = globalHistory[index];

    int localHistory = localBHT[pcEnd];
    int lPredictionValue = localPHT[pcEnd][localHistory];

    gPrediction = (gPredictionValue >= 4);
    lPrediction = (lPredictionValue >= 4);

    if (metaPrediction <= 3) {
        return gPrediction;
    } else {
        return lPrediction;
    }
}

void
HybridBP::update(ThreadID tid, Addr branchAddr, bool taken, void *bpHistory,
                 bool squashed, const StaticInstPtr & inst, Addr corrTarget)
{
    int pcEnd = (branchAddr) & ((1 << pcBits) - 1); //(branchAddr & ((1 << (pcBits + 2)) - 1)) >> 2;
    // int index = pcEnd ^ currentHistory;
    int index = (branchAddr & ((1 << gshareHistoryLength) - 1)) ^ currentHistory;
    int localHistory = localBHT[pcEnd];

    int change = 0;
    if (taken == gPrediction) {
        change -= 1;
    }
    if (taken == lPrediction) {
        change += 1;
    }
    metaHistory[pcEnd] = std::min(std::max(metaHistory[pcEnd] + change, 0), 7);

    if (taken) {
        globalHistory[index] = std::min(globalHistory[index] + 1, 7);
        localPHT[pcEnd][localHistory] = std::min(localPHT[pcEnd][localHistory] + 1, 7);
        
        // currentHistory = (currentHistory >> 1) + (1 << (gshareHistoryLength - 1));
        // localBHT[pcEnd] = (localBHT[pcEnd] >> 1) + (1 << (localHistoryLength - 1));
        currentHistory = ((currentHistory & ((1 << (gshareHistoryLength - 1)) - 1)) << 1) + 1;
        localBHT[pcEnd] = ((localBHT[pcEnd] & ((1 << (localHistoryLength - 1)) - 1)) << 1) + 1;
    } else {
        globalHistory[index] = std::max(globalHistory[index] - 1, 0);
        localPHT[pcEnd][localHistory] = std::max(localPHT[pcEnd][localHistory] - 1, 0);

        // currentHistory = currentHistory >> 1;
        // localBHT[pcEnd] = localBHT[pcEnd] >> 1;
        currentHistory = (currentHistory & ((1 << (gshareHistoryLength - 1)) - 1)) << 1;
        localBHT[pcEnd] = (localBHT[pcEnd] & ((1 << (localHistoryLength - 1)) - 1)) << 1;
    }
}

// add your helper functions here


/**
 * Important function to initialize BP with Python parameters
 *
 * You should not have to modify this.
 */
HybridBP*
HybridBPParams::create()
{
    return new HybridBP(this);
}
