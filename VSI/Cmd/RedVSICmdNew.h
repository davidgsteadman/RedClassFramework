// -------------------------------------------------------------------------------------------------
// This file is covered by: The MIT License (MIT) Copyright (c) 2016 David G. Steadman
// -------------------------------------------------------------------------------------------------
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
// associated documentation files (the "Software"), to deal in the Software without restriction,
// including without limitation the rights to use, copy, modify, merge, publish, distribute,
// sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
// NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// -------------------------------------------------------------------------------------------------
// (http://opensource.org/licenses/MIT)
// -------------------------------------------------------------------------------------------------

#pragma once

#include "RedVSICmdInterface.h"
#include "RedVSIContextInterface.h"
#include "RedDataType.h"
#include "RedVSIParseTreeInterface.h"
#include "RedVSIErrorCodes.h"

namespace Red {
namespace VSI {

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/// A new command creates a data item, with a specific name and in a specific location (stack or heap)
/// The data item has an optional initialising expression.
class RedVSICmdNew : public RedVSICmdInterface
{
public:

    // Construction Routines
    RedVSICmdNew();
    RedVSICmdNew(const RedVSILangElement& cInType, const RedVSILangElement& cInLoc, const RedString& cInName, RedVSIParseTreeInterface* pInRecordIndexExpr, RedVSIParseTreeInterface* pInInitExpr);
    ~RedVSICmdNew(void);

    // RedVSICmdInterface inherited routines
    RedVSILangElement Type(void) const { return kLangElementCommandNew; };
    void              QueueExpr(RedVSIContextInterface* pContext);
    void              Execute(RedVSIContextInterface* pContext);

    // Command Setup
    void SetType(RedVSILangElement cNewType)                 { if (!cLoc.IsType())     throw; cType=cNewType; };
    void SetLoc(RedVSILangElement cNewLoc)                   { if (!cLoc.IsLocation()) throw; cLoc=cNewLoc; };
    void SetName(const RedString& cNewName)                  { cName=cNewName; };
    void SetInitExpr(RedVSIParseTreeInterface* pNewInitExpr) { pDataInitExpr=pNewInitExpr; };

    // Wholesale access/assign operations
    void SetDetails(const RedVSILangElement& cInType,  const RedVSILangElement& cInLoc, const RedString& cInName,  RedVSIParseTreeInterface* pInRecordIndexExpr,  RedVSIParseTreeInterface* pInInitExpr);
    void GetDetails(      RedVSILangElement& cOutType,       RedVSILangElement& cOutLoc,      RedString& cOutName, RedVSIParseTreeInterface* pOutRecordIndexExpr, RedVSIParseTreeInterface*& pOutInitExpr) const;

private:
    
    RedVSILangElement         cType;
    RedVSILangElement         cLoc;
    RedString                 cName;
    RedVSIParseTreeInterface* pRecordIndexExpr;
    RedVSIParseTreeInterface* pDataInitExpr;

};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

} // VSI
} // Red


