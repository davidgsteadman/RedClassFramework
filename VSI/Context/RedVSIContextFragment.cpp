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

#include "RedVSIContextFragment.h"
//#include "CDataArray.h"

#include "RedVSIParseStackTraverser.h"

using namespace Red::Core;

namespace Red {
namespace VSI {

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Construction Routines
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

RedVSIContextFragment::RedVSIContextFragment(RedLog& initAnalysis) : cAnalysis(initAnalysis)
{
    pCurrCmd = NULL;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

RedVSIContextFragment::RedVSIContextFragment(RedLog& initAnalysis, RedVSICmdInterface* pSnippetCmd) : cAnalysis(initAnalysis)
{
    pCurrCmd = pSnippetCmd;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

RedVSIContextFragment::~RedVSIContextFragment(void)
{
    cLocalVariables.DelAll();
    
    //if (pReturnValue)
    //    delete pReturnValue;

    // Clear down any owned code
    SetTopCmd(NULL);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Data
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

RedType* RedVSIContextFragment::CreateDataItem(const RedVSILangElement& cLocation, const RedVSILangElement& cType, const RedString& cName)
{
    RedType* pNewData = NULL;

    // Basic Validation
    if (!cLocation.IsLocation()) throw;
    if (!cType.IsType()) throw;

    RedDataType DataType = RedVSILangElement::DataTypeForLangElemType(cType);

    if (cLocation.IsLocationStack())
    {
        pNewData = cLocalVariables.CreateAndAdd(cName, DataType);
    }
//    else if (cLocation.IsLocationAttribute())
//    {
//        RedFlexRecord* pData = FindDataItem(cName, kDataTypeRecord);
//    }
    else
    {
        throw;
    }

    // return the pointer to the new object (or zero)
    return pNewData;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

RedType* RedVSIContextFragment::DuplicateDataItem(const RedVSILangElement& cLocation, const RedType* pDataItem, const RedString& cName)
{
    RedType* pNewData = pDataItem->Clone();

    // Basic Validation
    if (!cLocation.IsLocation()) throw;

    if (cLocation.IsLocationStack())
    {
        cLocalVariables.Add(cName, pNewData);
    }

    // return the pointer to the new object (or zero)
    return pNewData;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

bool RedVSIContextFragment::FindDataItem(const RedString& cName, RedType*& pData)
{
    // first try and get the data from the local routine
    if (cLocalVariables.Find(cName, pData))
        return true;

    return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

RedVariant RedVSIContextFragment::DataItemAsVariant(const RedString& cName)
{
    RedVariant RetVar;

    RedType* pData = NULL;

    bool found = FindDataItem(cName, pData);

    if (found)
    {
        RetVar = pData;
    }

    return RetVar;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void RedVSIContextFragment::QueueExpr(RedVSIParseTreeInterface* pExpr)
{
    RedVSIParseStackTraverser::PopulateStack(cExprStack, pExpr, cAnalysis);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void RedVSIContextFragment::SetExprResult(RedVSIParseTreeInterface* pExpr, const RedVariant& result)
{
    cExprResultList.Add(pExpr, result);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

RedVariant RedVSIContextFragment::ExprResult(RedVSIParseTreeInterface* pExpr)
{
    RedVariant cResult;
    if (!cExprResultList.Find(pExpr, cResult)) throw;

    return cResult;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void RedVSIContextFragment::ExecuteExprQueue(void)
{
    while ( !cExprStack.IsEmpty() )
    {
        pCurrExpr = cExprStack.Pop();

        // Need to abort any expression with a function call in
        if (pCurrExpr->Type().IsParseFuncCall())
        {
            cAnalysis.AddErrorEvent("Routine call parse-node found inside code fragment.");
            return;
        }

        pCurrExpr->CalcResult(this);
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Execution
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void RedVSIContextFragment::Execute(const unsigned CmdCount)
{
    // Create and initialse the countdown
    unsigned CommandCountdown = CmdCount;

	// No further execution on an error
	if (cAnalysis.IsError())
		return;

	// Iterate until no command or count done
    while ( (CommandCountdown > 0) && (HasCmdToExecute()) )
    {
        // On entering the loop, check we have a command to execute. Prior HasCmd call ensures this
        // will pass.
        if (pCurrCmd == NULL)
			pCurrCmd = cCmdStack.Pop();

        // Queue up all the expressions needed by the command
        pCurrCmd->QueueExpr(this);
        ExecuteExprQueue();

        // Execute the command (it will queue the next command as part of its execution)
        pCurrCmd->Execute(this);

        // Clean up after execution
        cExprResultList.DelAll();
        CommandCountdown--;
        pCurrCmd = NULL;

        // Look for the next command to execute
        if (!cCmdStack.IsEmpty())
            pCurrCmd = cCmdStack.Pop();
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

bool RedVSIContextFragment::HasCmdToExecute(void)
{ 
    if (pCurrCmd)             return true;
    if (!cCmdStack.IsEmpty()) return true;

    return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

bool RedVSIContextFragment::IsExecutionComplete(void) const
{ 
    if ( (cCmdStack.IsEmpty()) && (pCurrCmd == NULL) )
        return true;

    return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Private
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/// Routine takes ownership of the fragment, and will delete it when replaced, or when context is
/// deleted.
/// Routine designed to allow assigning a null pointer to clear down owned code.
void RedVSIContextFragment::SetTopCmd(RedVSICmdInterface* pFragmentCmd)
{
    // Clear down any current execution
    pCurrCmd = NULL;
    cCmdStack.DelAll();
    cExprResultList.DelAll();

    // Delete any previous top command
    if (pTopCmd == NULL)
    {
        delete pTopCmd;
        pTopCmd = NULL;
    }

    pTopCmd = pFragmentCmd;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

} // VSI
} // Red

