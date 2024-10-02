#include "cpu/pred/never_taken.hh"

NeverTakenBP::NeverTakenBP(const NeverTakenBPParams *params)
    : BPredUnit(params)
{
}

void NeverTakenBP::uncondBranch(ThreadID tid, Addr pc, void * &bp_history) {
}

bool NeverTakenBP::lookup(ThreadID tid, Addr instPC, void * &bp_history) {
    return false; // predict not taken
}

void NeverTakenBP::btbUpdate(ThreadID tid, Addr instPC, void * &bp_history) {
}

void NeverTakenBP::update(ThreadID tid, Addr instPC, bool taken,
                          void *bp_history, bool squashed, const StaticInstPtr & inst, Addr corrTarget) {
}

NeverTakenBP*
NeverTakenBPParams::create()
{
    return new NeverTakenBP(this);
}

