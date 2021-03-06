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

#include "RedString.h"
#include "RedNumber.h"

namespace Red {
namespace Core {

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

typedef enum TEventLogType {
    eErrorEvent,
    eWarningEvent,
    eInfoEvent
} TEventLogType;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

class RedLogEvent
{
public:

    // Constructors
    RedLogEvent(const TEventLogType NewLogType)                           : LogType(NewLogType) {};
    RedLogEvent(const TEventLogType NewLogType, const RedString& NewText) : LogType(NewLogType), text(NewText) {};
    RedLogEvent(const RedString& NewText)                                 : LogType(eInfoEvent), text(NewText) {};
    RedLogEvent(const RedLogEvent& CurrEvent)                             : LogType(CurrEvent.LogType), text(CurrEvent.text) {};

    // Basic Accessors
    void                Init(void)                           { LogType = eInfoEvent; text.Init(); };
    TEventLogType       EventType(void) const                { return LogType; };
    RedString           Text(void) const                     { return text; };
    void                AppendText(const RedString& newText) { text.Append(newText); };
    void                AppendText(const RedNumber& newNum)  { text.Append(newNum.DecimalString()); };

    void                Set(const TEventLogType newtype, const RedString& newtext) { LogType=newtype; text=newtext; };

    void operator =(const RedLogEvent& newVal)      { Set(newVal.EventType(), newVal.Text()); };

private:
    TEventLogType LogType;
    RedString     text;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

} // Core
} // Red


