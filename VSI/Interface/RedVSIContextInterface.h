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

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#include "RedCoreNamespace.h"

#include "RedVSIRoutineCallInterface.h"
#include "RedVSILangElement.h"

using namespace Red::Core;

namespace Red {
namespace VSI {

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// define the RedVSICmdInterface class without a #include specifically to avoid
// a circular dependency.
class RedVSICmdInterface;
class RedVSIParseTreeInterface;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/// Class interface to pass to commands and parse-tree elements. Interface does not need to be
/// complicated with the additional operations the routine/thread contexts will need to manage
/// the execution at a higher level.
/// Interface allows for routines/fragments to exist without commands/expressions needing to know
/// about this level of detail.
class RedVSIContextInterface
{
public:

    // Routine creation and control
    virtual void          SetupRoutineCall(const RedVSIRoutineCallInterface& cSignature) =0;

    // Data accessors
    virtual RedType*      CreateDataItem(const RedVSILangElement& cLocation, const RedVSILangElement& cType, const RedString& cName) =0;
    virtual RedType*      DuplicateDataItem(const RedVSILangElement& cLocation, const RedType* pDataItem, const RedString& cName) =0;
    virtual bool          FindDataItem(const RedString& cName, RedType*& pData) =0;
    virtual RedVariant    DataItemAsVariant(const RedString& cName) =0;

    // Return value
    virtual void          SetValueToReturn(const RedVariant& cData) =0;

    // Expressions
    virtual void          QueueExpr(RedVSIParseTreeInterface* pExpr) =0;
    virtual void          SetExprResult(RedVSIParseTreeInterface* pExpr, const RedVariant& result) =0;
    virtual RedVariant    ExprResult(RedVSIParseTreeInterface* pExpr) =0;
    virtual void          ExecuteExprQueue(void) =0;

    // Commands
    virtual void          QueueCommand(RedVSICmdInterface* pCmd) =0;
    virtual void          ClearCommandQueue(void) =0;

    // Error reporting and debugging
    virtual RedLog&       Log(void) =0;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

} // VSI
} // Red

