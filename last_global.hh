#ifndef __CPU_PRED_LAST_GLOBAL_HH__
#define __CPU_PRED_LAST_GLOBAL_HH__

#include <vector>
#include "cpu/pred/bpred_unit.hh"
#include "base/types.hh"
#include "cpu/pred/bpred_unit.hh"
#include "params/LastGlobalBP.hh"

class LastGlobalBP : public BPredUnit
{
  public:

    LastGlobalBP(const LastGlobalBPParams *params);
    virtual void uncondBranch(ThreadID tid, Addr pc, void * &bp_history);

    bool lookup(ThreadID tid, Addr branch_addr, void * &bp_history);
    void btbUpdate(ThreadID tid, Addr branch_addr, void * &bp_history);

    void update(ThreadID tid, Addr branch_addr, bool taken, void *bp_history,
                bool squashed, const StaticInstPtr & inst, Addr corrTarget);
    void retireSquashed(ThreadID tid, void *bp_history)
    { assert(bp_history == NULL); }
    void squash(ThreadID tid, void *bp_history)
    { assert(bp_history == NULL); }

    void reset();

  private:

    std::vector<unsigned> globalHistoryReg;

};

#endif // __CPU_PRED_LAST_GLOBAL_HH__
