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
    , gshareHistoryLength(params->gshareHistoryLength)
    
    // initialize other variables here
{
    // Initialize local history history table
    int BHTLength = pow(2, pcBits);
    localBHT = new int[BHTLength];
    for (int i = 0; i < BHTLength; i++) {
        localBHT[i] = 0;
    }

    // Initialize local history prediciton table
    int localPHTLength = pow(2, localHistoryLength);
    localPHT = new int*[BHTLength];
    for (int i = 0; i < BHTLength; i++) {
        localPHT[i] = new int[localPHTLength];
        for (int j = 0; j < localPHTLength; j++) {
            localPHT[i][j] = 3;
        }
    }

    // Initialize global history prediction table and branch history
    int gshareHistoryTableLength = pow(2, gshareHistoryLength);
    globalHistory = new int[gshareHistoryTableLength];
    for (int i = 0; i < gshareHistoryTableLength; i++) {
        globalHistory[i] = 3;
    }
    currentHistory = 0;

    // Initialize meta predictor table
    metaHistory = new int[BHTLength];
    for (int i = 0; i < BHTLength; i++) {
        metaHistory[i] = 3;
    }

    // Initialize helper variables
    gPrediction = false;
    lPrediction = false;


    // Initialize perceptron table
    int tableLength = pow(2, pcBits);
    perceptronTable = new int*[tableLength];
    for (int i = 0; i < tableLength; i++) {
        perceptronTable[i] = new int[historyLength + 1];
        for (unsigned int j = 0; j < historyLength + 1; j++) {
            perceptronTable[i][j] = 0;
        }
    }
    globalHistoryP = 0;
}

bool
CustomBP::predictUsingPerceptron(int* perceptron) {
    int sum = 0;
    for (unsigned int i = 0; i < historyLength; i++) {
        int iWeight = perceptron[historyLength - i];
        int iHistory = (globalHistoryP >> i) % 2;
        if (iHistory == 0) {
            iHistory = -1;
        }
        sum += iWeight * iHistory;
    }

    // Constant weight
    sum += perceptron[0];

    lastPredictionValue = sum;
    if (sum > 0) {
        lastPrediction = true;
        return true;
    } else {
        lastPrediction = false;
        return false;
    }
}

bool
CustomBP::lookup(ThreadID tid, Addr branchAddr, void * &bpHistory)
{
     // Find meta predictor value
    int pcEnd = (branchAddr) & ((1 << pcBits) - 1);

    // Find gshare predictor value
    int index = (branchAddr & ((1 << gshareHistoryLength) - 1)) ^ currentHistory;
    int gPredictionValue = globalHistory[index];

    // Find local predictor value
    int localHistory = localBHT[pcEnd];
    int lPredictionValue = localPHT[pcEnd][localHistory];

    // Store predictions based on cutoff
    gPrediction = (gPredictionValue >= 4);
    lPrediction = (lPredictionValue >= 4);

    pcEnd = (branchAddr) & ((1 << pcBits) - 1);
    int* perceptron = perceptronTable[pcEnd];
    bool pPrediction = predictUsingPerceptron(perceptron);

    // Pick prediction to use based off of meta predictor cutoff
    if((gPrediction + lPrediction + pPrediction) > 1){
        return true;
    }
   return false;

}

void
CustomBP::update(ThreadID tid, Addr branchAddr, bool taken, void *bpHistory,
                 bool squashed, const StaticInstPtr & inst, Addr corrTarget)
{
    // update Hybrid BP

     // Calculate indices used in predictors
    int pcEnd = (branchAddr) & ((1 << pcBits) - 1);
    int index = (branchAddr & ((1 << gshareHistoryLength) - 1)) ^ currentHistory;
    int localHistory = localBHT[pcEnd];

    // Update meta predictor based on correctness of both predictors
    int change = 0;
    if (taken == gPrediction) {
        change -= 1;
    }
    if (taken == lPrediction) {
        change += 1;
    }
    metaHistory[pcEnd] = std::min(std::max(metaHistory[pcEnd] + change, 0), 7);

    // Update history tables and predictors based on actual branch execution
    if (taken) {
        globalHistory[index] = std::min(globalHistory[index] + 1, 7);
        localPHT[pcEnd][localHistory] = std::min(localPHT[pcEnd][localHistory] + 1, 7);
        
        currentHistory = ((currentHistory & ((1 << (gshareHistoryLength - 1)) - 1)) << 1) + 1;
        localBHT[pcEnd] = ((localBHT[pcEnd] & ((1 << (localHistoryLength - 1)) - 1)) << 1) + 1;
    } else {
        globalHistory[index] = std::max(globalHistory[index] - 1, 0);
        localPHT[pcEnd][localHistory] = std::max(localPHT[pcEnd][localHistory] - 1, 0);

        currentHistory = (currentHistory & ((1 << (gshareHistoryLength - 1)) - 1)) << 1;
        localBHT[pcEnd] = (localBHT[pcEnd] & ((1 << (localHistoryLength - 1)) - 1)) << 1;
    }

    // update perceptron BP

    pcEnd = (branchAddr) & ((1 << pcBits) - 1);
    int* perceptron = perceptronTable[pcEnd];

    // Training
    if ((lastPrediction != taken) || (unsigned int) abs(lastPredictionValue) <= trainingThreshold) {
        // Train weights
        for (unsigned int i = 0; i < historyLength; i++) {
            bool iHistory = ((globalHistoryP >> i) % 2) != 0;
            if (taken == iHistory) {
                perceptron[historyLength - i] += 1;
            } else {
                perceptron[historyLength - i] -= 1;
            }
        }

        // Constant weight
        if (taken) {
            perceptron[0] += 1;
        } else {
            perceptron[0] -= 1;
        }
    }

    // Update history
    if (taken) {
        globalHistoryP = (globalHistoryP >> 1) + (1 << (historyLength - 1));
    } else {
        globalHistoryP = globalHistoryP >> 1;
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
