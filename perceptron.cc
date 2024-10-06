/* 
 * CMU 18-740 
 * Fall 2019
 * Lab 1
*/

#include "cpu/pred/perceptron.hh"
#include <math.h>
#include <iostream>

PerceptronBP::PerceptronBP(const PerceptronBPParams *params)
    : BPredUnit(params)
    , pcBits(params->pcBits)
    , historyLength(params->historyLength)
    // initialize other variables here
{
    // Initialize perceptron table
    int tableLength = pow(2, pcBits);
    perceptronTable = new int*[tableLength];
    for (int i = 0; i < tableLength; i++) {
        perceptronTable[i] = new int[historyLength + 1];
        for (unsigned int j = 0; j < historyLength + 1; j++) {
            perceptronTable[i][j] = 0;
        }
    }
    globalHistory = 0;
}

bool
PerceptronBP::predictUsingPerceptron(int* perceptron) {
    int sum = 0;
    for (unsigned int i = 0; i < historyLength; i++) {
        int iWeight = perceptron[historyLength - i];
        int iHistory = (globalHistory >> i) % 2;
        if (iHistory == 0) {
            iHistory = -1;
        }
        sum += iWeight * iHistory;
    }

    // Constant weight
    sum += perceptron[0];

    lastPredictionValue = sum;
    if (sum > 0) {
        return true;
    } else {
        return false;
    }
}

void
PerceptronBP::displayPerceptron(int* perceptron) {
    for (unsigned int i = 0; i < historyLength; i++) {
        std::cout << perceptron[historyLength - i] << " ";
    }
    std::cout << perceptron[0] << std::endl;
}

bool
PerceptronBP::lookup(ThreadID tid, Addr branchAddr, void * &bpHistory)
{
    int pcEnd = (branchAddr) & ((1 << pcBits) - 1);
    int* perceptron = perceptronTable[pcEnd];
    lastPrediction = predictUsingPerceptron(perceptron);
    return lastPrediction;
}

void
PerceptronBP::update(ThreadID tid, Addr branchAddr, bool taken, void *bpHistory,
                 bool squashed, const StaticInstPtr & inst, Addr corrTarget)
{
    int pcEnd = (branchAddr) & ((1 << pcBits) - 1);
    int* perceptron = perceptronTable[pcEnd];

    // Training
    if ((lastPrediction != taken) || (unsigned int) abs(lastPredictionValue) <= trainingThreshold) {
        // Train weights
        for (unsigned int i = 0; i < historyLength; i++) {
            bool iHistory = ((globalHistory >> i) % 2) != 0;
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
        globalHistory = (globalHistory >> 1) + (1 << (historyLength - 1));
    } else {
        globalHistory = globalHistory >> 1;
    }
}

// add your helper functions here


/**
 * Important function to initialize BP with Python parameters
 *
 * You should not have to modify this.
 */
PerceptronBP*
PerceptronBPParams::create()
{
    return new PerceptronBP(this);
}
