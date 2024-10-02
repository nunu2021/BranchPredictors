#include "cpu/pred/last_global.hh"

LastGlobalBP::LastGlobalBP(const LastGlobalBPParams *params)
    : BPredUnit(params),
      globalHistoryReg(params->numThreads, 0)
{
}

void LastGlobalBP::uncondBranch(ThreadID tid, Addr pc, void * &bp_history) {
}

bool
LastGlobalBP::lookup(ThreadID tid, Addr branchAddr, void * &bpHistory)
{
    if (globalHistoryReg[tid])
        return true;
    else
        return false;
}

void LastGlobalBP::btbUpdate(ThreadID tid, Addr instPC, void * &bp_history) {
}

void
LastGlobalBP::update(ThreadID tid, Addr branchAddr, bool taken, void *bpHistory,
                 bool squashed, const StaticInstPtr & inst, Addr corrTarget)
{
    if (taken)
        globalHistoryReg[tid] = 1;
    else
        globalHistoryReg[tid] = 0;
}

LastGlobalBP*
LastGlobalBPParams::create()
{
    return new LastGlobalBP(this);
}
