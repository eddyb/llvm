//===- Loads.h - Local load analysis --------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares simple local analyses for load instructions.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_ANALYSIS_LOADS_H
#define LLVM_ANALYSIS_LOADS_H

#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/Support/CommandLine.h"

namespace llvm {

class DataLayout;
class MDNode;

/// isSafeToLoadUnconditionally - Return true if we know that executing a load
/// from this value cannot trap.  If it is not obviously safe to load from the
/// specified pointer, we do a quick local scan of the basic block containing
/// ScanFrom, to determine if the address is already accessed.
bool isSafeToLoadUnconditionally(Value *V,
                                 unsigned MaxAlign, uint64_t MaxSize,
                                 Instruction *ScanFrom);

/// DefMaxInstsToScan - the default number of maximum instructions
/// to scan in the block, used by FindAvailableLoadedValue().
extern cl::opt<unsigned> DefMaxInstsToScan;

/// FindAvailableLoadedValue - Scan the ScanBB block backwards (starting at
/// the instruction before ScanFrom) checking to see if we have the value at
/// the memory address *Ptr locally available within a small number of
///  instructions. If the value is available, return it.
///
/// If not, return the iterator for the last validated instruction that the
/// value would be live through.  If we scanned the entire block and didn't
/// find something that invalidates *Ptr or provides it, ScanFrom would be
/// left at begin() and this returns null.  ScanFrom could also be left
///
/// MaxInstsToScan specifies the maximum instructions to scan in the block.
/// If it is set to 0, it will scan the whole block. You can also optionally
/// specify an alias analysis implementation, which makes this more precise.
///
/// If AATags is non-null and a load or store is found, the AA tags from the
/// load or store are recorded there.  If there are no AA tags or if no access
/// is found, it is left unmodified.
Value *FindAvailableLoadedValue(LoadInst *Load, BasicBlock *ScanBB,
                                BasicBlock::iterator &ScanFrom,
                                unsigned MaxInstsToScan = DefMaxInstsToScan,
                                AliasAnalysis *AA = nullptr,
                                AAMDNodes *AATags = nullptr);

}

#endif
