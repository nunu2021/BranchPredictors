#include "cpu/pred/random.hh"
#include <cstdlib>

RandomBP::RandomBP(const RandomBPParams *params)
    : BPredUnit(params)
{
}


void RandomBP::uncondBranch(ThreadID tid, Addr pc, void * &bp_history) {
}

bool RandomBP::lookup(ThreadID tid, Addr instPC, void * &bp_history) {

    int r = (int) rand();

    if ((r % 2) == 0)
        return false;
    else
        return true;
}


void RandomBP::btbUpdate(ThreadID tid, Addr instPC, void * &bp_history) {
}


void RandomBP::update(ThreadID tid, Addr instPC, bool taken,
                          void *bp_history, bool squashed, const StaticInstPtr & inst, Addr corrTarget) {
}

RandomBP*
RandomBPParams::create()
{
    return new RandomBP(this);
}
