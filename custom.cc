/* 
 * CMU 18-740 
 * Fall 2019
 * Lab 1
*/

#include "cpu/pred/custom.hh"

CustomBP::CustomBP(const CustomBPParams *params)
    : BPredUnit(params)
    // initialize other variables here
{
}

bool
CustomBP::lookup(ThreadID tid, Addr branchAddr, void * &bpHistory)
{
    // right now, this just always returns false
    return false;
}

void
CustomBP::update(ThreadID tid, Addr branchAddr, bool taken, void *bpHistory,
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
CustomBP*
CustomBPParams::create()
{
    return new CustomBP(this);
}
