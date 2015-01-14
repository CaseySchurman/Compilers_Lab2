#include <iostream>
#include "lex.h"
#include "parse.h"
#include "tokens.h"

int yylval = 0;
int token = -1;
int oldToken = -1;

//*******************************************
void Error(std::string expecting)
{
    std::cout << "Found " << yytext << " when expecting a " << expecting;
    std::cout << " in line " << yylineno << std::endl;
}
//*******************************************
int GetToken()
{
    if (token > 0)
    {
        oldToken = token;
        token = -1;
    } else {
        oldToken = yylex();
    }

    return oldToken;
}
//*******************************************
void UnGetToken()
{
    token = oldToken;
}
//******************************************
bool FindPROG()
{
    if (!FindSTMTS()) 
    {
        return false;
    }
    
    int token = GetToken();
    
    if(token != END)
    {
        Error("end");
        return false;
    }
    
    return true;
}
//******************************************
bool FindSTMTS()
{
    if (FindSTMT())
    {
        if(!FindSMTS())
        {
            Error("statements");
            return false;
        }
        
        return true;
    }
    
    return true;
}
//******************************************
bool FindSTMT()
{
    if (!FindEXPR())
    {
        Error("expression");
        return false;
    }
    
    int token = GetToken();
    
    if(token != ';')
    {
        Error(";");
        return false;
    }
    
    return true;
}
//******************************************
bool FindEXPR()
{
    int token = GetToken();
    
    if(token != '(')
    {
        UnGetToken();
        
        if(!FindTERM())
        {
            Error("term");
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        if(!FindEXPR())
        {
            Error("expression");
            return false;
        }
        
        token = GetToken();
        
        if(token != ')')
        {
            Error(")");
            return false;
        }
        
        if(!FindEXPR_P())
        {
            Error("expression_prime");
            return false;
        }
        
        return true;
    }
}
//******************************************
bool FindEXPR_P()
{
    if(FindPLUSOP())
    {
        int token = GetToken();
        
        if(token != '(')
        {
            Error("(");
            return false;
        }
        
        if(!FindEXPR())
        {
            Error("expression");
            return false;
        }
        
        token = GetToken();
        
        if(token != ')')
        {
            Error(")");
        }
        
        if(!FindEXPR_P())
        {
            Error("expression_prime");
            return false;
        }
        
        return true;
    }
}
//******************************************
bool FindPLUSOP()
{
    int token = GetToken();
    
    if(token != '+')
    {
        if(token != '-')
        {
            Error("plusop");
            return false;
        }
        
        return true;
    }
    
    return true;
}
//******************************************
bool FindTERM()
{
    int token = GetToken();
    
    if(token != '[')
    {
        if(token != INT_VAL)
        {
            Error("term");
            return false;
        }
        
        return true;
    }
    
    if(!FindEXPR())
    {
        Error("expression");
        return false;
    }
    
    token = GetToken();
    
    if(token != ']')
    {
        Error("]");
        return false;
    }
    
    if(!FindTERM_P())
    {
        Error("term_prime");
        return false;
    }
    
    return true;
}
//******************************************
bool FindTERM_P()
{
    if(FindTIMESOP())
    {
        int token = GetToken();
        
        if(token != '[')
        {
            Error("[");
            return false;
        }
        
        if(!FindEXPR())
        {
            Error("expression");
            return false;
        }
        
        token = GetToken();
        
        if(token != ']')
        {
            Error("]");
            return false;
        }
        
        if(!FindTERM_P())
        {
            Error("term_prime");
            return false;
        }
        
        return true;
    }
    
    return true;
}
//******************************************
bool FindTIMESOP()
{
    int token = GetToken();
    
    if(token != '*')
    {
        if(token != '/')
        {
            Error("times_op");
            return false;
        }
        return true;
    }
    
    return true;
}
/*******************************************
bool FindExample()
{
    if (!FindPART1()) return false;
    
    int token = GetToken();
    if (token != '+')
    {
        UnGetToken();
        return false;
    }

    if (!FindPART2()) return false;

    return true;
}
*/