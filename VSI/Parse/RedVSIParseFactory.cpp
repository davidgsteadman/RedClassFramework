
#include "RedVSIParseFactory.h"
#include "RedVSIParseTreeVar.h"
#include "RedVSIParseTreeVal.h"
#include "RedVSIParseTreeBinaryOp.h"
#include "RedVSIParseTreeCall.h"

#include "RedVSIErrorCodes.h"

#include "RedVSIToken.h"

namespace Red {
namespace VSI {

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

RedVSIParseTreeInterface* RedVSIParseFactory::ConstructAssignExpr(RedVSITokenBuffer& cInputBuffer, RedLog& log)
{
    return RedVSIParseFactory::RunExprAssignCompetition(cInputBuffer, log);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

RedVSIParseTreeInterface* RedVSIParseFactory::ConstructStatementExpr(RedVSITokenBuffer& cInputBuffer, RedLog& log)
{
    return RedVSIParseFactory::RunCompareExprCompetition(cInputBuffer, log);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

RedVSIParseTreeInterface* RedVSIParseFactory::ConstructValueExpr(RedVSITokenBuffer& cInputBuffer, RedLog& log)
{
    return RedVSIParseFactory::RunSumExprCompetition(cInputBuffer, log);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Private Operations
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

RedVSIParseTreeInterface* RedVSIParseFactory::RunExprAssignCompetition(RedVSITokenBuffer& cInputBuffer, RedLog& log)
{
    // Top Element, returned to the caller
    RedVSIParseTreeInterface* pTopElem = 0;

    // Read the left hand expression item.
    RedVSIParseTreeInterface* pLeft = RunCompareExprCompetition(cInputBuffer, log);
    if (pLeft==0) { return 0; }
    pTopElem = pLeft;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    int                        iOpFound = 1;
    RedVSIParseTreeInterface*  pRight   = 0;
    RedVSIParseTreeBinaryOp*   pCurrOp  = 0;
    RedVSIParseTreeBinaryOp*   pPrevOp  = 0;

    while ( (iOpFound) && (!log.IsError()) )
    {
        // Read the operator
        RedVSIToken cOp = cInputBuffer.GetToken();
        if ( (cOp.GetPredef().IsAssignOpSymbol()) )
        {
            // Create the operator object
            pCurrOp = new RedVSIParseTreeBinaryOp();
            if      (cOp.GetPredef().IsSymbolAssignEqual())    pCurrOp->SetOp(RedVSILangElement::BinaryOpAssignEqual());
            else if (cOp.GetPredef().IsSymbolAssignPlus())     pCurrOp->SetOp(RedVSILangElement::BinaryOpAssignPlus());
            else if (cOp.GetPredef().IsSymbolAssignMinus())    pCurrOp->SetOp(RedVSILangElement::BinaryOpAssignMinus());
            else if (cOp.GetPredef().IsSymbolAssignDivide())   pCurrOp->SetOp(RedVSILangElement::BinaryOpAssignDivide());
            else if (cOp.GetPredef().IsSymbolAssignMultiply()) pCurrOp->SetOp(RedVSILangElement::BinaryOpAssignMultiply());

            // shuffle nodes to accomodate the new operator
            pTopElem = pCurrOp;
            if (pPrevOp == 0) 
                pCurrOp->SetLeftChild(pLeft);
            else
                pCurrOp->SetLeftChild(pPrevOp);

            // Read the new right hand expression element
            pRight = RunCompareExprCompetition(cInputBuffer, log);
            if (pRight)
                pCurrOp->SetRightChild(pRight);

            // copy the current operator ready for any subsequent iteration
            pPrevOp = pCurrOp;
        }
        else
        {
            iOpFound = 0;
            cInputBuffer.SetTokenIndexBackOne();
        }
    } 

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // properly delete any new object on an error
    if ( (log.IsError()) && (pTopElem!=0) ) { delete pTopElem; pTopElem = 0; }

    return pTopElem;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

RedVSIParseTreeInterface* RedVSIParseFactory::RunCompareExprCompetition(RedVSITokenBuffer& cInputBuffer, RedLog& log)
{
    // Top Element, returned to the caller
    RedVSIParseTreeInterface* pTopElem = 0;

    // Read the left hand expression item.
    RedVSIParseTreeInterface* pLeft = RunLogicExprCompetition(cInputBuffer, log);
    if (pLeft==0) { return 0; }
    pTopElem = pLeft;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    int                 iOpFound = 1;
    RedVSIParseTreeInterface*      pRight   = 0;
    RedVSIParseTreeBinaryOp* pCurrOp  = 0;
    RedVSIParseTreeBinaryOp* pPrevOp  = 0;

    while ( (iOpFound) && (!log.IsError()) )
    {
        // Read the operator
        RedVSIToken cOp = cInputBuffer.GetToken();
        if ( (cOp.GetPredef().IsCompareOpSymbol()) )
        {
            // Create the operator object
            pCurrOp = new RedVSIParseTreeBinaryOp();
            if      (cOp.GetPredef().IsSymbolCompareEqual())        pCurrOp->SetOp(RedVSILangElement::BinaryOpCompareEqual());
            else if (cOp.GetPredef().IsSymbolCompareNotEqual())     pCurrOp->SetOp(RedVSILangElement::BinaryOpCompareNotEqual());
            else if (cOp.GetPredef().IsSymbolCompareGreaterThan())  pCurrOp->SetOp(RedVSILangElement::BinaryOpCompareGreaterThan());
            else if (cOp.GetPredef().IsSymbolCompareLessThan())     pCurrOp->SetOp(RedVSILangElement::BinaryOpCompareLessThan());

            // shuffle nodes to accomodate the new operator
            pTopElem = pCurrOp;
            if (pPrevOp == 0) 
                pCurrOp->SetLeftChild(pLeft);
            else
                pCurrOp->SetLeftChild(pPrevOp);

            // Read the new right hand expression element
            pRight = RunLogicExprCompetition(cInputBuffer, log);
            if (pRight)
                pCurrOp->SetRightChild(pRight);

            // copy the current operator ready for any subsequent iteration
            pPrevOp = pCurrOp;
        }
        else
        {
            iOpFound = 0;
            cInputBuffer.SetTokenIndexBackOne();
        }
    } 

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // properly delete any new object on an error
    if ( (log.IsError()) && (pTopElem!=0) ) { delete pTopElem; pTopElem = 0; }

    return pTopElem;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

RedVSIParseTreeInterface* RedVSIParseFactory::RunLogicExprCompetition(RedVSITokenBuffer& cInputBuffer, RedLog& log)
{
    // Top Element, returned to the caller
    RedVSIParseTreeInterface* pTopElem = 0;

    // Read the left hand expression item.
    RedVSIParseTreeInterface* pLeft = RunSumExprCompetition(cInputBuffer, log);
    if (pLeft==0) { return 0; }
    pTopElem = pLeft;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    int                          iOpFound = 1;
    RedVSIParseTreeInterface*    pRight   = 0;
    RedVSIParseTreeBinaryOp*     pCurrOp  = 0;
    RedVSIParseTreeBinaryOp*     pPrevOp  = 0;

    while ( (iOpFound) && (!log.IsError()) )
    {
        // Read the operator
        RedVSIToken cOp = cInputBuffer.GetToken();
        if ( (cOp.GetPredef().IsLogicOpSymbol()) )
        {
            // Create the operator object
            pCurrOp = new RedVSIParseTreeBinaryOp();
            if      (cOp.GetPredef().IsSymbolLogicAnd()) pCurrOp->SetOp(RedVSILangElement::BinaryOpLogicAnd());
            else if (cOp.GetPredef().IsSymbolLogicOr())  pCurrOp->SetOp(RedVSILangElement::BinaryOpLogicOr());

            // shuffle nodes to accomodate the new operator
            pTopElem = pCurrOp;
            if (pPrevOp == 0) 
                pCurrOp->SetLeftChild(pLeft);
            else
                pCurrOp->SetLeftChild(pPrevOp);

            // Read the new right hand expression element
            pRight = RunSumExprCompetition(cInputBuffer, log);
            if (pRight)
                pCurrOp->SetRightChild(pRight);

            // copy the current operator ready for any subsequent iteration
            pPrevOp = pCurrOp;
        }
        else
        {
            iOpFound = 0;
            cInputBuffer.SetTokenIndexBackOne();
        }
    } 

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // properly delete any new object on an error
    if ( (log.IsError()) && (pTopElem!=0) ) { delete pTopElem; pTopElem = 0; }

    return pTopElem;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

RedVSIParseTreeInterface* RedVSIParseFactory::RunSumExprCompetition(RedVSITokenBuffer& cInputBuffer, RedLog& log)
{
    // Top Element, returned to the caller
    RedVSIParseTreeInterface* pTopElem = 0;

    // Read the left hand expression item.
    RedVSIParseTreeInterface* pLeft = RunMultiExprCompetition(cInputBuffer, log);
    if (pLeft==0) { return 0; }
    pTopElem = pLeft;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    int                  iOpFound = 1;
    RedVSIParseTreeInterface* pRight   = 0;
    RedVSIParseTreeBinaryOp*  pCurrOp  = 0;
    RedVSIParseTreeBinaryOp*  pPrevOp  = 0;

    while ( (iOpFound) && (!log.IsError()) )
    {
        // Read the operator
        RedVSIToken cOp = cInputBuffer.GetToken();
        if ( (cOp.GetPredef().IsSymbolPlus()) || (cOp.GetPredef().IsSymbolMinus()) )
        {
            // Create the operator object
            pCurrOp = new RedVSIParseTreeBinaryOp();
            if      (cOp.GetPredef().IsSymbolPlus())  pCurrOp->SetOp(RedVSILangElement::BinaryOpPlus());
            else if (cOp.GetPredef().IsSymbolMinus()) pCurrOp->SetOp(RedVSILangElement::BinaryOpMinus());

            // shuffle nodes to accomodate the new operator
            pTopElem = pCurrOp;
            if (pPrevOp == 0) 
                pCurrOp->SetLeftChild(pLeft);
            else
                pCurrOp->SetLeftChild(pPrevOp);

            // Read the new right hand expression element
            pRight = RunMultiExprCompetition(cInputBuffer, log);
            if (pRight) 
                pCurrOp->SetRightChild(pRight);

            // copy the current operator ready for any subsequent iteration
            pPrevOp = pCurrOp;
        }
        else
        {
            iOpFound = 0;
            cInputBuffer.SetTokenIndexBackOne();
        }
    } 

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // properly delete any new object on an error
    if ( (log.IsError()) && (pTopElem!=0) ) { delete pTopElem; pTopElem = 0; }

    return pTopElem;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

RedVSIParseTreeInterface* RedVSIParseFactory::RunMultiExprCompetition(RedVSITokenBuffer& cInputBuffer, RedLog& log)
{
    // Top Element, returned to the caller
    RedVSIParseTreeInterface* pTopElem = 0;

    // Read the left hand expression item. Any errors already reported.
    RedVSIParseTreeInterface* pLeft = RunPowExprCompetition(cInputBuffer, log);
    if (pLeft==0) { return 0; }
    pTopElem = pLeft;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    int                   iOpFound = 1;
    RedVSIParseTreeInterface*  pRight   = 0;
    RedVSIParseTreeBinaryOp*   pCurrOp  = 0;
    RedVSIParseTreeBinaryOp*   pPrevOp  = 0;

    while ( (iOpFound) && (!log.IsError()) )
    {
        // Read the operator
        RedVSIToken cOp = cInputBuffer.GetToken();
        if ( (cOp.GetPredef().IsSymbolMultiply()) || (cOp.GetPredef().IsSymbolDivide()) )
        {
            // Create the operator object
            pCurrOp = new RedVSIParseTreeBinaryOp();
            if      (cOp.GetPredef().IsSymbolMultiply()) pCurrOp->SetOp(RedVSILangElement::BinaryOpMultiply());
            else if (cOp.GetPredef().IsSymbolDivide())   pCurrOp->SetOp(RedVSILangElement::BinaryOpDivide());

            // shuffle nodes to accomodate the new operator
            pTopElem = pCurrOp;
            if (pPrevOp == 0) 
                pCurrOp->SetLeftChild(pLeft);
            else
                pCurrOp->SetLeftChild(pPrevOp);

            // Read the new right hand expression element
            pRight = RunPowExprCompetition(cInputBuffer, log);
            if (pRight)
                pCurrOp->SetRightChild(pRight);

            // copy the current operator ready for any subsequent iteration
            pPrevOp = pCurrOp;
        }
        else
        {
            iOpFound = 0;
            cInputBuffer.SetTokenIndexBackOne();
        }
    } 

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // properly delete any new object on an error
    if ( (log.IsError()) && (pTopElem!=0) ) { delete pTopElem; pTopElem = 0; }

    return pTopElem;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

RedVSIParseTreeInterface* RedVSIParseFactory::RunPowExprCompetition(RedVSITokenBuffer& cInputBuffer, RedLog& log)
{
    // Top Element, returned to the caller
    RedVSIParseTreeInterface* pTopElem = 0;

    // Read the left hand expression item. Any errors will already have been reported.
    RedVSIParseTreeInterface* pLeft = RunItemExprCompetition(cInputBuffer, log);
    if (pLeft==0) { return 0; }
    pTopElem = pLeft;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    int                       iOpFound = 1;
    RedVSIParseTreeInterface* pRight   = 0;
    RedVSIParseTreeBinaryOp*  pCurrOp  = 0;
    RedVSIParseTreeBinaryOp*  pPrevOp  = 0;

    while ( (iOpFound) && (!log.IsError()) )
    {
        // Read the operator
        RedVSIToken cOp = cInputBuffer.GetToken();
        if ( cOp.GetPredef().IsSymbolPower() )
        {
            // Create the operator object
            pCurrOp = new RedVSIParseTreeBinaryOp();
            if (cOp.GetPredef().IsSymbolPower())
                pCurrOp->SetOp(RedVSILangElement::BinaryOpPower());

            // shuffle nodes to accomodate the new operator
            pTopElem = pCurrOp;
            if (pPrevOp == 0) 
                pCurrOp->SetLeftChild(pLeft);
            else
                pCurrOp->SetLeftChild(pPrevOp);

            // Read the new right hand expression element
            pRight = RunItemExprCompetition(cInputBuffer, log);
            if (pRight) 
                pCurrOp->SetRightChild(pRight);

            // copy the current operator ready for any subsequent iteration
            pPrevOp = pCurrOp;
        }
        else
        {
            iOpFound = 0;
            cInputBuffer.SetTokenIndexBackOne();
        }
    } 

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // properly delete any new object on an error
    if ( (log.IsError()) && (pTopElem!=0) ) { delete pTopElem; pTopElem = 0; }

    return pTopElem;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

RedVSIParseTreeInterface* RedVSIParseFactory::RunItemExprCompetition(RedVSITokenBuffer& cInputBuffer, RedLog& log)
{
    RedVSIParseTreeInterface* pEntry = 0;

    // Record the start position in the competition, so we can reset it when we don't get a match
    int iCompStartPos = cInputBuffer.GetTokenIndex();
    int iCurrEntry = 1;

    // Loop until we have an entity or an error
    while ( (pEntry == 0) && (!log.IsError()) )
    {
        // Reset the comp start position
        cInputBuffer.SetTokenIndex(iCompStartPos);

        // Attempt to input an entity
        switch(iCurrEntry)
        {
        case 1: pEntry = RunSubExprCompetition(cInputBuffer, log); break;
        case 2: pEntry = RunExternalCallCompetition(cInputBuffer, log); break;
        case 3: pEntry = RunInternalCallCompetition(cInputBuffer, log); break;
        case 4: pEntry = RunValueCompetition(cInputBuffer, log); break;
        case 5: pEntry = RunVariableCompetition(cInputBuffer, log); break;

        // When the counter has incremented past all the entrants, we will never find
        // a match, so flag an error.
        default:
            log.AddErrorEvent("RedVSIParseFactory::RunItemExprCompetition : No Entry");
        }

        // Move counter on for next iteration
        iCurrEntry++;
    }

    // Mop-up. If we have an entry and an error, delete the entity.
    if ( (pEntry != 0) && (log.IsError()) ) { delete pEntry; pEntry = 0; }
    
    return pEntry;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

RedVSIParseTreeInterface* RedVSIParseFactory::RunSubExprCompetition(RedVSITokenBuffer& cInputBuffer, RedLog& log)
{
    RedVSIToken cTok = cInputBuffer.GetToken();

    if (cTok.GetPredef().IsSymbolOpenBracket())
    {
        // read the sub expression
        RedVSIParseTreeInterface* pSubExpr = RunCompareExprCompetition(cInputBuffer, log);
        if (!pSubExpr)
        {
            log.AddText(cTok.GetPos().PosText() + RedVSIErrorCodes::GetErrorString(RedVSIErrorCodes::ePFact_SubExpr_NoExpr));
            return 0;
         }
        
        // ensure the expression ends with a close bracket
        cTok = cInputBuffer.GetToken();
        if (!cTok.GetPredef().IsSymbolCloseBracket())
        {
            log.AddText(cTok.GetPos().PosText() + RedVSIErrorCodes::GetErrorString(RedVSIErrorCodes::ePFact_SubExpr_NoExprEnd));
            return 0;
        }
        return pSubExpr;
    }
    
    // return valid zero
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

RedVSIParseTreeInterface* RedVSIParseFactory::RunValueCompetition(RedVSITokenBuffer& cInputBuffer, RedLog& log)
{
    RedVSIToken cTok = cInputBuffer.GetToken();
  
    if (IsValueToken(cTok))
    {
        RedVariant cNodeVal;
        if (cTok.Type().IsNumber())        cNodeVal = cTok.GetNumber();
        if (cTok.Type().IsStringLiteral()) cNodeVal = cTok.GetText();
        
        RedVSIParseTreeVal* pParseValNode = new RedVSIParseTreeVal(cNodeVal);
        
        return pParseValNode;
    }
    
    // return valid zero
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

RedVSIParseTreeInterface* RedVSIParseFactory::RunVariableCompetition(RedVSITokenBuffer& cInputBuffer, RedLog& log)
{
    RedVSIToken cTok = cInputBuffer.GetToken();

    if (IsVariableToken(cTok))
    {
        // Data to go into the constructor (name already checked)
        RedString          cVarName   = cTok.GetText();
        RedVSIParseTreeInterface* pIndexExpr = 0;
    
        // check for an array index after the variable name
        cTok = cInputBuffer.GetToken();
        if (cTok.GetPredef().IsSymbolOpenBracket())
        {
            // Read the array index expression
            pIndexExpr = RunCompareExprCompetition(cInputBuffer, log);
            if (!pIndexExpr)
            {
                log.AddText(cTok.GetPos().PosText() + RedVSIErrorCodes::GetErrorString(RedVSIErrorCodes::ePFact_Var_NoArrayExpr));
                return 0;
            }

            // read the closing array bracket, which isn't part of the expression.
            cTok = cInputBuffer.GetToken();
            if (!cTok.GetPredef().IsSymbolCloseBracket())
            {
                log.AddText(cTok.GetPos().PosText() + RedVSIErrorCodes::GetErrorString(RedVSIErrorCodes::ePFact_Var_NoArrayExprEnd));
                return 0;
            }
        }  
        else
        {
            // put back token if its not for an array
            cInputBuffer.SetTokenIndexBackOne();
        }  
        
        RedVSIParseTreeVar* pNewVar = new RedVSIParseTreeVar(cVarName);
        return pNewVar;
    }
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

RedVSIParseTreeInterface* RedVSIParseFactory::RunExternalCallCompetition(RedVSITokenBuffer& cInputBuffer, RedLog& log)
{
    // Check the four elements which start a function call
    RedVSIToken cUnitNameTok    = cInputBuffer.GetToken();
    RedVSIToken cSeparatorTok   = cInputBuffer.GetToken();
    RedVSIToken cFuncTok        = cInputBuffer.GetToken();
    RedVSIToken cOpenBracketTok = cInputBuffer.GetToken();

    if (!cUnitNameTok.Type().IsName())                { return 0; }
    //if (!cSeparatorTok.GetPredef().IsSymbolRoutineSeparator()) { return 0; }
    if (!cFuncTok.Type().IsName())                    { return 0; }
    if (!cOpenBracketTok.GetPredef().IsSymbolOpenBracket())    { return 0; }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    // create a call object and assign the basic name information
    RedVSIParseTreeCall*       pNewCmd    = new RedVSIParseTreeCall;
    RedVSIParseList* pParamList = new RedVSIParseList;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // determine the class/object names from the inputs
    RedString cClassName;
    RedString cObjectName;
    if (cSeparatorTok.GetPredef().IsSymbolPeriod())
        cObjectName = cUnitNameTok.GetText();
    else
        cClassName = cUnitNameTok.GetText();

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // iterate to populate the call with parameters
    RedVSIToken cParamTok;
    RedVSIToken cCommaTok;
    int    iParamsComplete = 0;    
    
    while (!iParamsComplete)
    {
        // check for the end of the params first
        cParamTok = cInputBuffer.GetToken();
        if (cParamTok.GetPredef().IsSymbolCloseBracket())
        {
            iParamsComplete = 1;
        }
        else
        {
            // return the token, so we can correctly read the expr.
            cInputBuffer.SetTokenIndexBackOne();

            // Read the parameter, and check the expression object is valid
            RedVSIParseTreeInterface* pExpr = RunCompareExprCompetition(cInputBuffer, log);

            // if we have a valid parameter, add it.
            if (pExpr)
            {
                pParamList->AddLast(pExpr);

                // look for a delimiter
                cCommaTok = cInputBuffer.GetToken();
                if (!cCommaTok.GetPredef().IsSymbolComma())
                    cInputBuffer.SetTokenIndexBackOne();
            }
            else
            {
                log.AddText(cParamTok.GetPos().PosText() + RedVSIErrorCodes::GetErrorString(RedVSIErrorCodes::ePFact_Call_NoExpr));
                delete pNewCmd; 
                delete pParamList; 
                return 0;
            }
        }
    }
    
    // Assign the final details and return the command
    pNewCmd->SetDetails(cObjectName, cClassName, cFuncTok.GetText(), pParamList);
    return pNewCmd;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

RedVSIParseTreeInterface* RedVSIParseFactory::RunInternalCallCompetition(RedVSITokenBuffer& cInputBuffer, RedLog& log)
{
    // Check the elements which start an internal function call
    RedVSIToken cFuncTok        = cInputBuffer.GetToken();    
    RedVSIToken cOpenBracketTok = cInputBuffer.GetToken();

    if (!cFuncTok.Type().IsName())                    { return 0; }
    if (!cOpenBracketTok.GetPredef().IsSymbolOpenBracket())    { return 0; }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    // create a call object and assign the basic name information
    RedVSIParseTreeCall*       pNewCmd    = new RedVSIParseTreeCall;
    RedVSIParseList* pParamList = new RedVSIParseList;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // determine the class/object names from the inputs
    RedString cClassName;
    RedString cObjectName;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // iterate to populate the call with parameters
    RedVSIToken cParamTok;
    RedVSIToken cCommaTok;
    int    iParamsComplete = 0;    
    
    while (!iParamsComplete)
    {
        // check for the end of the params first
        cParamTok = cInputBuffer.GetToken();
        if (cParamTok.GetPredef().IsSymbolCloseBracket())
        {
            iParamsComplete = 1;
        }
        else
        {
            // return the token, so we can correctly read the expr.
            cInputBuffer.SetTokenIndexBackOne();

            // Read the parameter, and check the expression object is valid
            RedVSIParseTreeInterface* pExpr = RunCompareExprCompetition(cInputBuffer, log);

            // if we have a valid parameter, add it.
            if (pExpr)
            {
                pParamList->AddLast(pExpr);

                // look for a delimiter
                cCommaTok = cInputBuffer.GetToken();
                if (!cCommaTok.GetPredef().IsSymbolComma())
                    cInputBuffer.SetTokenIndexBackOne();
            }
            else
            {
                log.AddText(cParamTok.GetPos().PosText() + RedVSIErrorCodes::GetErrorString(RedVSIErrorCodes::ePFact_Call_NoExpr));
                delete pNewCmd; 
                delete pParamList; 
                return 0;
            }
        }
    }
    
    // Assign the final details and return the command
    RedString cNullClassName;
    pNewCmd->SetDetails(cClassName, cObjectName, cFuncTok.GetText(), pParamList);
    return pNewCmd;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int RedVSIParseFactory::IsValueToken(RedVSIToken cTok)
{
    if ( cTok.Type().IsNumber() ) return 1;
    if ( cTok.Type().IsStringLiteral() ) return 1;
    
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int RedVSIParseFactory::IsVariableToken(RedVSIToken cTok)
{
    if ( cTok.Type().IsName() ) return 1;
    
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

} // VSI
} // Red



