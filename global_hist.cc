/* 
 * CMU 18-740 
 * Fall 2019
 * Lab 1
*/

#include "cpu/pred/global_hist.hh"
#include <stdlib.h>
#include <math.h>
#include <iostream>

GlobalHistBP::GlobalHistBP(const GlobalHistBPParams *params)
    : BPredUnit(params)
    , historyLength(params->historyLength)
    // initialize other variables here
{
    // Initialize predition table and branch history
    int tableLength = pow(2, historyLength);
    predictionTable = (int*) malloc(tableLength * sizeof(int));
    for (int i = 0; i < tableLength; i++) {
        predictionTable[i] = 3;
    }
    currentHistory = 0;
}

bool
GlobalHistBP::lookup(ThreadID tid, Addr branchAddr, void * &bpHistory)
{
    // Predict based on table value and cutoff
    bool guess = false;
    if (predictionTable[currentHistory] >= 4) {
        guess = true;
    }
    return guess;
}

void
GlobalHistBP::update(ThreadID tid, Addr branchAddr, bool taken, void *bpHistory,
                 bool squashed, const StaticInstPtr & inst, Addr corrTarget)
{
    // Update history and prediction table
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

// add your helper functions here


/**
 * Important function to initialize BP with Python parameters
 *
 * You should not have to modify this.
 */
GlobalHistBP*
GlobalHistBPParams::create()
{
    return new GlobalHistBP(this);
}
