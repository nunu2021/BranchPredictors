/* 
 * CMU 18-740 
 * Fall 2019
 * Lab 1
*/

#ifndef __CPU_PRED_CUSTOM_HH__
#define __CPU_PRED_CUSTOM_HH__

#include <vector>
#include <queue>
#include "cpu/pred/bpred_unit.hh"
#include "base/types.hh"
#include "cpu/pred/bpred_unit.hh"
#include "params/CustomBP.hh"  // made automatically by scons


class CustomBP : public BPredUnit
{
  private:
    // parameters set by BranchPredictor.py
    unsigned int pcBits;
    unsigned int localHistoryLength;
    unsigned int gshareHistoryLength;

   // Hybrid BP parameters
    int** localPHT;
    int* localBHT;
    int* globalHistory;
    int* metaHistory;
    int currentHistory;

    bool gPrediction;
    bool lPrediction;

    // Perceptron parameters
    unsigned int trainingThreshold = 16;

    // include any storage structures (e.g., buffers, counters) here
    int** perceptronTable;
    int globalHistoryP;
    int lastPredictionValue;
    bool lastPrediction;

    // include any helper functions here
    bool predictUsingPerceptron(int* perceptron);
    void displayPerceptron(int* perceptron);

    // include any storage structures (e.g., buffers, counters) here


    // include any helper functions here

  public:

    // none of the below functions should change here (edit in .cc only)

    /**
     * Constructor
     */
    CustomBP(const CustomBPParams *params);

    /**
     * Looks up a given PC in the BP to see if it is taken or not taken.
     * This function is called whenever a prediction is needed (e.g., when
     * a branch instruction is decoded).
     *
     * Parameters:
     *   - tid: current thread ID (we'll ignore this, since our processor
     *          runs only one threads).
     *   - inst_PC: the PC of the branch that needs to be looked up.
     *   - bp_history: a pointer that will be set to an object that
     *                 has the branch predictor state associated with the 
     *                 lookup.
     *
     * Return: true if the branch is taken; false if the branch is not taken
     */
    bool lookup(ThreadID tid, Addr branch_addr, void * &bp_history);

    /**
     * Updates the BP based on what the branch actually did.
     * This function is called whenever the branch instruction is executed.
     *
     * Parameters:
     *   - tid: current thread ID (we'll ignore this, since our processor
     *   - inst_PC: the PC of the branch that needs to be updated.
     *   - taken: whether the branch was actually taken (true = taken).
     *   - bp_history: a pointer to the branch predictor state that is
     *                 associated with the branch lookup that is being 
     *                 updated.
     *   - squashed: set to true when this function is called during a
     *               squash operation (ignore this).
     *   - inst: static instruction information
     *   - corrTarget: the resolved target of the branch (only needed
     *                 for squashed branches) - ignore for now.
     */
    void update(ThreadID tid, Addr branch_addr, bool taken, void *bp_history,
                bool squashed, const StaticInstPtr & inst, Addr corrTarget);

    // no need to implement this function
    virtual void uncondBranch(ThreadID tid, Addr pc, void * &bp_history) {};

    // no need to implement this function
    void btbUpdate(ThreadID tid, Addr branch_addr, void * &bp_history) {};

    // no need to modify this function
    void squash(ThreadID tid, void *bp_history)
    { assert(bp_history == NULL); }
};

#endif // __CPU_PRED_CUSTOM_HH__
