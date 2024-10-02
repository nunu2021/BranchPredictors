/* 
 * CMU 18-740 
 * Fall 2019
 * Lab 1
*/

#include "cpu/pred/global_hist.hh"

GlobalHistBP::GlobalHistBP(const GlobalHistBPParams *params)
    : BPredUnit(params)
    , historyLength(params->historyLength)
    // initialize other variables here
{
}

bool
GlobalHistBP::lookup(ThreadID tid, Addr branchAddr, void * &bpHistory)
{
    // right now, this just always returns false
    return false;
}

void
GlobalHistBP::update(ThreadID tid, Addr branchAddr, bool taken, void *bpHistory,
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
GlobalHistBP*
GlobalHistBPParams::create()
{
    return new GlobalHistBP(this);
}
