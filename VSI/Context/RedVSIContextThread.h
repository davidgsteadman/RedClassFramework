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

#include "RedCoreNamespace.h"
#include "RedVSILangElement.h"
#include "RedVSILibInterface.h"

using namespace Red::Core;

namespace Red {
namespace VSI {

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

class RedVSIContextRoutine;
typedef RedStackLIFO<RedVSIContextRoutine*> RedVSIRoutineContextStack;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

class RedVSIContextThread
{
public:

    RedVSIContextThread(void) { pCodeLib = NULL; };

	// Code Library
    void                       SetCodeLib(RedVSILibInterface* pNewCodeLib) { pCodeLib = pNewCodeLib; };
    RedVSILibInterface*        CodeLib(void) const                         { return pCodeLib; };

	// Heap Data
    RedType*                   CreateHeapDataItem(const RedVSILangElement& cType, const RedString& cName);
    bool                       FindHeapDataItem(const RedString& cName, RedType*& pData);
    RedRecord*                 Heap(void) { return &cHeap; };

	// Routine Stack
	RedVSIContextRoutine*      TopRoutineOnStack(void);
	void                       PushRoutineOnStack(RedVSIContextRoutine* newRtn);
	RedVSIContextRoutine*      PopRoutineOffStack(void);
    unsigned                   NumRoutinesOnStack(void) const { return cRoutineStack.NumItems(); };

    // Routine Execution
    void                       Execute(const unsigned NumCmd);

private:

    // A stack of all routine context. The top of the stack is the curently executing.
    RedVSIRoutineContextStack cRoutineStack;

    // Code library
    RedVSILibInterface* pCodeLib;

    // Data attributes
    RedRecord cHeap;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

} // VSI
} // Red
