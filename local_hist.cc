/* 
 * CMU 18-740 
 * Fall 2019
 * Lab 1
*/

#include "cpu/pred/local_hist.hh"
#include <math.h>


LocalHistBP::LocalHistBP(const LocalHistBPParams *params)
    : BPredUnit(params)
    , pcBits(params->pcBits)
    , historyLength(params->historyLength)
    
    // initialize other variables here
{   
    unsigned int BHTlength = 1 << params->pcBits;
    BHT = new int[BHTlength];
    for(unsigned int i = 0; i < BHTlength; i++){
        BHT[i] = 0;
    }

    unsigned int PHTLength = 1 << params->historyLength;
    PHT = new int[PHTLength];
    for(unsigned int i = 0; i < PHTLength; i++){
        PHT[i] = 3;
    }
}

bool
LocalHistBP::lookup(ThreadID tid, Addr branchAddr, void * &bpHistory)
{
    // check the BHT for PHT entry based on most significant bits
    // check the PHT, and if value is less than 4, then return 
    unsigned int mask = (1 << pcBits) - 1;

    // fprintf(stderr, " %d is the lookup for address %d concat %d and hist %d \n", "string format", PHT[BHT[branchAddr & mask]], branchAddr, branchAddr & mask, BHT[branchAddr & mask]);
    return (PHT[BHT[branchAddr & mask]] > 3);

}

void
LocalHistBP::update(ThreadID tid, Addr branchAddr, bool taken, void *bpHistory,
                 bool squashed, const StaticInstPtr & inst, Addr corrTarget)
{
    unsigned int mask = (1 << pcBits) - 1;
    unsigned int phtMask = (1 << historyLength) -1;


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
}

// add your helper functions here


/**
 * Important function to initialize BP with Python parameters
 *
 * You should not have to modify this.
 */
LocalHistBP*
LocalHistBPParams::create()
{
    return new LocalHistBP(this);
}
