/* 
 * CMU 18-740 
 * Fall 2019
 * Lab 1
*/

#include "cpu/pred/local_hist.hh"

LocalHistBP::LocalHistBP(const LocalHistBPParams *params)
    : BPredUnit(params)
    , pcBits(params->pcBits)
    , historyLength(params->historyLength)
    // initialize other variables here
{
}

bool
LocalHistBP::lookup(ThreadID tid, Addr branchAddr, void * &bpHistory)
{
    // right now, this just always returns false
    return false;
}

void
LocalHistBP::update(ThreadID tid, Addr branchAddr, bool taken, void *bpHistory,
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
LocalHistBP*
LocalHistBPParams::create()
{
    return new LocalHistBP(this);
}
