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
#include "RedVSITokenBuffer.h"
#include "RedLog.h"
#include "RedVSIContextInterface.h"
#include "RedVSIErrorCodes.h"

namespace Red {
namespace VSI {

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/// Class to create all the command classes.
/// The construction code was taken out of the command classes to allow them to
/// focus on their core functionality of performing commands. Additional processing
/// will use SetDetails/GetDetails on each of the commands and deal with their
/// own domain (such as GUI or serialisation).
class RedVSICmdFactory
{
public:

    static RedVSICmdInterface* RunConstuctionCompetition(RedVSITokenBuffer& cInputBuffer, RedLog& RedLog);

private:

    static bool                EOFComp   (RedVSITokenBuffer& cInputBuffer, RedLog& RedLog);

    static RedVSICmdInterface* ExprComp  (RedVSITokenBuffer& cInputBuffer, RedLog& RedLog);
	static RedVSICmdInterface* IfComp    (RedVSITokenBuffer& cInputBuffer, RedLog& RedLog);
    static RedVSICmdInterface* LogComp   (RedVSITokenBuffer& cInputBuffer, RedLog& RedLog);
	static RedVSICmdInterface* NewComp   (RedVSITokenBuffer& cInputBuffer, RedLog& RedLog);
    static RedVSICmdInterface* ReturnComp(RedVSITokenBuffer& cInputBuffer, RedLog& RedLog);
    static RedVSICmdInterface* WhileComp (RedVSITokenBuffer& cInputBuffer, RedLog& RedLog);
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

} // VSI
} // Red
