
#pragma once

#include "RedSmartPtr.h"
#include "RedType.h"
#include "RedDataType.h"

namespace Red {
namespace Core {

    class RedString;

    class RedChar : public RedType
    {
    public:

        // constructor
        RedChar(void)        { Init(); };
        RedChar(unsigned char NewCh)  { Set(NewCh); };
        ~RedChar(void) { };

        // Inherited: RedType
        void              Init(void)        { Set('\0'); };
        const RedDataType Type(void) const  { return kDataTypeChar; };
        RedType*          Clone(void) const { RedChar* newC = new RedChar(); newC->ch = ch; return (RedChar*)newC; };

        // simple set & get operations
        void       Set(const unsigned char NewCh) { ch = NewCh; };
        void       Set(int NewCh)                 { if ((NewCh > 0) && (NewCh<256)) ch = NewCh; };
        void       Set(const RedChar& cNewVal)    { ch = cNewVal.ch; };

        const unsigned char Char(void)             const { return ch; };
        const int  IsChar(unsigned char NewCh)     const { return ch == NewCh; };
        const int  DecimalNumber(void) const;

        // queries
        const int  IsAlpha(void)          const { return ( ((ch>='a')&&(ch<='z')) || ((ch>='A')&&(ch<='Z')) ); };
        const int  IsAlphaNumeric(void)   const { return ( IsAlpha() || IsDecimalNumber() ); };
        const int  IsBracket(void)        const { return ( (ch=='(') || (ch==')') || (ch=='[') || (ch==']') || (ch=='{') || (ch=='}') ); };
        const int  IsComma(void)          const { return (ch == ','); };
        const int  IsDecimalNumber(void)  const { return ( (ch >= '0') && (ch <= '9') ); };
        const int  IsDecimalPoint(void)   const { return (ch == '.'); };
        const int  IsEOF(void)            const { return (ch == '\0'); };
        const int  IsFullstop(void)       const { return (ch == '.'); };
        const int  IsHexNumber(void)      const { return ( ((ch>='0')&&(ch<='9')) || ((ch>='a')&&(ch<='f')) || ((ch>='A')&&(ch<='F')) ); };
        const int  IsNewline(void)        const { return (ch == '\n'); };
        const int  IsEOL(void)            const { return (ch == '\n'); };
        const int  IsNonPrintable(void)   const { return ( (ch<32) || (ch>128) ); };
        const int  IsNumeric(void)        const { return ( IsDecimalNumber() || IsFullstop() ); };
        const int  IsOperator(void)       const { return ( (ch=='=') || (ch=='+') || (ch=='-') || (ch=='*') || (ch=='/') || (ch=='<') || (ch=='>') ); };
        const int  IsPrintable(void)      const { return ( (ch>=32) && (ch<=128) ); };
        const int  IsQuote(void)          const { return ( (ch == '"') || (ch == '\'') ); };
        const int  IsSymbol(void)         const { return ( IsPrintable() && (!IsAlphaNumeric()) && (!IsQuote()) ); };
        const int  IsWhiteSpace(void)     const { return ( (ch == '\n') || (ch == ' ') || (ch == '\t') ); };

        // Assignment Operators
        void operator =(const int      newVal)      { Set(newVal); };
        void operator =(const unsigned char newVal) { Set(newVal); };
        void operator =(const RedChar& newVal)      { Set(newVal); };

        const int operator==(const RedChar& rhs) const { if (ch==rhs.ch) return 1; return 0; };
        const int operator!=(const RedChar& rhs) const { if (ch!=rhs.ch) return 1; return 0; };

    private:

        unsigned char ch;
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    typedef RedSmartPtr<RedChar> RedCharSmartPtr;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

}
}

