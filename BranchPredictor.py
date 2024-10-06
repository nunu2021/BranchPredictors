# Copyright (c) 2012 Mark D. Hill and David A. Wood
# Copyright (c) 2015 The University of Wisconsin
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met: redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer;
# redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in the
# documentation and/or other materials provided with the distribution;
# neither the name of the copyright holders nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# Authors: Nilay Vaish and Dibakar Gope

from m5.SimObject import SimObject
from m5.params import *
from m5.proxy import *

class IndirectPredictor(SimObject):
    type = 'IndirectPredictor'
    cxx_class = 'IndirectPredictor'
    cxx_header = "cpu/pred/indirect.hh"
    abstract = True

    numThreads = Param.Unsigned(Parent.numThreads, "Number of threads")

class SimpleIndirectPredictor(IndirectPredictor):
    type = 'SimpleIndirectPredictor'
    cxx_class = 'SimpleIndirectPredictor'
    cxx_header = "cpu/pred/simple_indirect.hh"

    indirectHashGHR = Param.Bool(True, "Hash branch predictor GHR")
    indirectHashTargets = Param.Bool(True, "Hash path history targets")
    indirectSets = Param.Unsigned(256, "Cache sets for indirect predictor")
    indirectWays = Param.Unsigned(4, "Ways for indirect predictor")
    indirectTagSize = Param.Unsigned(16, "Indirect target cache tag bits")
    indirectPathLength = Param.Unsigned(3,
        "Previous indirect targets to use for path history")
    indirectGHRBits = Param.Unsigned(13, "Indirect GHR number of bits")
    instShiftAmt = Param.Unsigned(2, "Number of bits to shift instructions by")

class BranchPredictor(SimObject):
    type = 'BranchPredictor'
    cxx_class = 'BPredUnit'
    cxx_header = "cpu/pred/bpred_unit.hh"
    abstract = True

    numThreads = Param.Unsigned(Parent.numThreads, "Number of threads")
    BTBEntries = Param.Unsigned(4096, "Number of BTB entries")
    BTBTagSize = Param.Unsigned(16, "Size of the BTB tags, in bits")
    RASSize = Param.Unsigned(16, "RAS size")
    instShiftAmt = Param.Unsigned(2, "Number of bits to shift instructions by")

    indirectBranchPred = Param.IndirectPredictor(SimpleIndirectPredictor(),
      "Indirect branch predictor, set to NULL to disable indirect predictions")


class StatisticalCorrector(SimObject):
    type = 'StatisticalCorrector'
    cxx_class  = 'StatisticalCorrector'
    cxx_header = "cpu/pred/statistical_corrector.hh"
    abstract = True

    # Statistical corrector parameters

    numEntriesFirstLocalHistories = Param.Unsigned(
        "Number of entries for first local histories")

    bwnb = Param.Unsigned("Num global backward branch GEHL lengths")
    bwm = VectorParam.Int("Global backward branch GEHL lengths")
    logBwnb = Param.Unsigned("Log num of global backward branch GEHL entries")

    lnb = Param.Unsigned("Num first local history GEHL lenghts")
    lm = VectorParam.Int("First local history GEHL lengths")
    logLnb = Param.Unsigned("Log number of first local history GEHL entries")

    inb = Param.Unsigned(1, "Num IMLI GEHL lenghts")
    im = VectorParam.Int([8], "IMLI history GEHL lengths")
    logInb = Param.Unsigned("Log number of IMLI GEHL entries")

    logBias = Param.Unsigned("Log size of Bias tables")

    logSizeUp = Param.Unsigned(6,
        "Log size of update threshold counters tables")

    chooserConfWidth = Param.Unsigned(7,
        "Number of bits for the chooser counters")

    updateThresholdWidth = Param.Unsigned(12,
        "Number of bits for the update threshold counter")

    pUpdateThresholdWidth = Param.Unsigned(8,
        "Number of bits for the pUpdate threshold counters")

    extraWeightsWidth = Param.Unsigned(6,
        "Number of bits for the extra weights")

    scCountersWidth = Param.Unsigned(6, "Statistical corrector counters width")


# 18-740 branch predictors

class NeverTakenBP(BranchPredictor):
    type = 'NeverTakenBP'
    cxx_class = 'NeverTakenBP'
    cxx_header = "cpu/pred/never_taken.hh"


class RandomBP(BranchPredictor):
    type = 'RandomBP'
    cxx_class = 'RandomBP'
    cxx_header = "cpu/pred/random.hh"


class LastGlobalBP(BranchPredictor):
    type = 'LastGlobalBP'
    cxx_class = 'LastGlobalBP'
    cxx_header = "cpu/pred/last_global.hh"


class GlobalHistBP(BranchPredictor):
    type = 'GlobalHistBP'
    cxx_class = 'GlobalHistBP'
    cxx_header = "cpu/pred/global_hist.hh"

    # use only these predefined parameters
    historyLength = Param.Unsigned(4, "length of history register")
    # do not add any more!


class LocalHistBP(BranchPredictor):
    type = 'LocalHistBP'
    cxx_class = 'LocalHistBP'
    cxx_header = "cpu/pred/local_hist.hh"

    # use only these predefined parameters
    pcBits = Param.Unsigned(12, "number of PC bits used to index table")
    historyLength = Param.Unsigned(16, "length of history register")
    # do not add any more!


class HybridBP(BranchPredictor):
    type = 'HybridBP'
    cxx_class = 'HybridBP'
    cxx_header = "cpu/pred/hybrid.hh"

    # use only these predefined parameters
    pcBits = Param.Unsigned(12, "number of PC bits used to index PAp table")
    localHistoryLength = Param.Unsigned(4, "length of history register used by PAp")
    gshareHistoryLength = Param.Unsigned(16, "length of history register used by gshare")
    # do not add any more!


class PerceptronBP(BranchPredictor):
    type = 'PerceptronBP'
    cxx_class = 'PerceptronBP'
    cxx_header = "cpu/pred/perceptron.hh"

    # use only these predefined parameters
    pcBits = Param.Unsigned(3, "number of PC bits used to index table")
    historyLength = Param.Unsigned(4, "length of history register")
    # do not add any more!


class CustomBP(BranchPredictor):
    type = 'CustomBP'
    cxx_class = 'CustomBP'
    cxx_header = "cpu/pred/custom.hh"
    

    # add your parameters here
     # use only these predefined parameters
    pcBits = Param.Unsigned(12, "number of PC bits used to index PAp table")
    historyLength = Param.Unsigned(4, "length of history register")
    localHistoryLength = Param.Unsigned(16, "length of history register used by PAp")
    gshareHistoryLength = Param.Unsigned(4, "length of history register used by gshare")
    

class TournamentBP(BranchPredictor):
    type = 'TournamentBP'
    cxx_class = 'TournamentBP'
    cxx_header = "cpu/pred/tournament.hh"

    localPredictorSize = Param.Unsigned(2048, "Size of local predictor")
    localCtrBits = Param.Unsigned(2, "Bits per counter")
    localHistoryTableSize = Param.Unsigned(2048, "size of local history table")
    globalPredictorSize = Param.Unsigned(8192, "Size of global predictor")
    globalCtrBits = Param.Unsigned(2, "Bits per counter")
    choicePredictorSize = Param.Unsigned(8192, "Size of choice predictor")
    choiceCtrBits = Param.Unsigned(2, "Bits of choice counters")
