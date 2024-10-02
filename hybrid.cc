/* 
 * CMU 18-740 
 * Fall 2019
 * Lab 1
*/

#include "cpu/pred/hybrid.hh"

HybridBP::HybridBP(const HybridBPParams *params)
    : BPredUnit(params)
    , pcBits(params->pcBits)
    , localHistoryLength(params->localHistoryLength)
    , gshareHistoryLength(params->gshareHistoryLength)
    // initialize other variables here
{
}

bool
HybridBP::lookup(ThreadID tid, Addr branchAddr, void * &bpHistory)
{
    // right now, this just always returns false
    return false;
}

void
HybridBP::update(ThreadID tid, Addr branchAddr, bool taken, void *bpHistory,
                 bool squashed, const StaticInstPtr & inst, Addr corrTarget)
{
    // right now, this does nothing
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
