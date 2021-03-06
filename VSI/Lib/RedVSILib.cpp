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

#include "RedVSILib.h"
#include "RedTmlNamespace.h"
#include "RedVSILibFactory.h"

using namespace Red::TinyML;

namespace Red {
namespace VSI {

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void RedVSILib::AddClass(RedVSILibClass* pNewClass)
{
    // add the class
    cClassList.AddLast(pNewClass);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void RedVSILib::DelClass(const RedString& cClassName)
{
    IteratorType cIt(&cClassList);

    cIt.First();
    while (!cIt.IsDone())
    {
        RedVSILibClass* pCurrClass = cIt.CurrentItem();

        if (pCurrClass->ClassName() == cClassName)
        {
            cClassList.Del(cIt.CollectionIndex());
        }
        cIt.Next();
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

RedVSILibClass* RedVSILib::FindClass(const RedString& cClassName)
{
    IteratorType cIt(&cClassList);

    // Iterate through the loaded classes for the classname
    cIt.First();
    while (!cIt.IsDone())
    {
        RedVSILibClass* pCurrClass = cIt.CurrentItem();

        if (pCurrClass->ClassName() == cClassName)
            return pCurrClass;

        cIt.Next();
    }

    // Not found, search for the class in a library directory if one is setup
    if (!LibFilePath.IsEmpty())
    {
        RedString fullpath = LibFilePath + cClassName + ".tml";

        if (RedIOHandler::FileExists(fullpath))
        {
            // Load the class
            RedTmlElement* newX = NULL;
            if (RedTmlAction::CreateTmlFromFile(fullpath, &newX) != kResultSuccess)
                return NULL;

            RedVSILibFactory vsiCodeLibFactory(this);
            RedTmlNode*    tmlTreeNode    =  dynamic_cast<RedTmlNode*>(newX);

            RedLog log;
            vsiCodeLibFactory.InputTmlClass(*tmlTreeNode, log);

            cIt.First();
            while (!cIt.IsDone())
            {
                RedVSILibClass* pCurrClass = cIt.CurrentItem();

                if (pCurrClass->ClassName() == cClassName)
                    return pCurrClass;

                cIt.Next();
            }
        }
    }

    return NULL;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

RedVSILibRoutineInterface* RedVSILib::FindRoutine(const RedVSIRoutineCallInterface& cSig)
{
    RedVSILibClass*            pClass   = FindClass(cSig.ClassName());
    RedVSILibRoutineInterface* pRoutine = NULL;

    // if we found the first class
    if (pClass)
    { 
        // loop until we run out of parent classes or find a routine
        while (!pRoutine)
        {
            // look for the routine
            pRoutine = pClass->FindRoutine(cSig);

            // if we have no routine, look for a parent class
            if (!pRoutine)
            {
                // find the parent class or exit
                if (!pClass->HasParentClass())
                    return NULL;
                pClass = FindClass(pClass->ParentClassName());
                if (!pClass) 
                    return NULL;
            }
        }
    }

    return pRoutine;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

RedVSILibRoutineInterface* RedVSILib::FindRoutine(const RedString& cClassName, const RedString& cRoutineName)
{
    RedVSILibClass*            pClass   = FindClass(cClassName);
    RedVSILibRoutineInterface* pRoutine = NULL;

    if (pClass)
        pRoutine = pClass->FindRoutineByName(cRoutineName);

    return pRoutine;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

bool RedVSILib::DoesClassFileExist(const RedString& classname)
{
    bool retval = false;

    RedString fullpath = LibFilePath + classname + ".tml";

    retval = RedIOHandler::FileExists(fullpath);

    return retval;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

} // VSI
} // Red


