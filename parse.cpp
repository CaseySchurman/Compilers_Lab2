// Parse file which contains parsing functions, which functionally 
// are defined by the CFG (defined by lab 2 document)
// Author: Casey Schurman
// casey.schurman@oit.edu

#include <iostream>
#include "lex.h"
#include "parse.h"
#include "tokens.h"

int yylval = 0;
int token = -1;
int oldToken = -1;

//Displays error with what it found, expected token, and line number
void Error(std::string expecting)
{
    std::cout << "Found " << yytext << " when expecting a " << expecting;
    std::cout << " in line " << yylineno << std::endl;
}

//Retrieves the next token
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

//Ungets the token that was just gotten
void UnGetToken()
{
    token = oldToken;
}

//Returns true if a program is found
bool FindPROG()
{
    if (!FindSTMTS()) 
    {
        return false;
    }
    
    int token = GetToken();
    
    if(token != END)
    {
        Error("'end'");
        return false;
    }
    
    return true;
}

//Returns true if a Statements is found
bool FindSTMTS()
{
    if (!FindSTMT())
    {
        int token = GetToken();
        
        if(token == END)
        {
            UnGetToken();
            return true;
        }
        
        UnGetToken();
        
        if(!FindSTMTS())
        {
            return true;
        }
    }
    else if(!FindSTMTS())
    {
        return false;
    }
    
    return true;
}

//Returns true if a statement is found, and outputs it found one
bool FindSTMT()
{
    if (!FindEXPR())
    {
        return false;
    }
    
    int token = GetToken();
    
    if(token != ';')
    {
        Error("';'");
        UnGetToken();
        Sync();
    }
    else
    {
        std::cout << "Found a statement\n";
    }
    
    return true;
}

//Returns true if Expression is found
bool FindEXPR()
{
    int token = GetToken();
    
    if(token != '(')
    {
        UnGetToken();
        
        if(!FindTERM())
        {
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
            return false;
        }
        
        token = GetToken();
        
        if(token != ')')
        {
            Error("')'");
            UnGetToken();
            Sync();
            return false;
        }
        
        if(!FindEXPR_P())
        {
            return false;
        }
        
        return true;
    }
}

//Returns true if Expression Prime is found
bool FindEXPR_P()
{
    if(FindPLUSOP())
    {
        int token = GetToken();
        
        if(token != '(')
        {
            UnGetToken();
            Error("'('");
            Sync();
            return false;
        }
        
        if(!FindEXPR())
        {
            return false;
        }
        
        token = GetToken();
        
        if(token != ')')
        {
            UnGetToken();
            Error("')'");
            Sync();
            return false;
        }
        
        if(!FindEXPR_P())
        {
            return false;
        }
        
        return true;
    }
    
    return true;
}

//Returns true if Plusop Prime is found
bool FindPLUSOP()
{
    int token = GetToken();
    
    if(token != '+' && token != '-')
    {
        UnGetToken();
        return false;
    }
    
    return true;
}

//Returns true if a Term is found
bool FindTERM()
{
    int token = GetToken();
    
    if(token != '[')
    {
        if(token != INT_VAL)
        {
            UnGetToken();
            return false;
        }
        
        return true;
    }
    
    if(!FindEXPR())
    {
        return false;
    }
    
    token = GetToken();
    
    if(token != ']')
    {
        UnGetToken();
        Error("']'");
        Sync();
        return false;
    }
    
    if(!FindTERM_P())
    {
        return false;
    }
    
    return true;
}

//Returns true if a Term Prime is found
bool FindTERM_P()
{
    if(FindTIMESOP())
    {
        int token = GetToken();
        
        if(token != '[')
        {
            UnGetToken();
            Error("'['");
            Sync();
            return false;
        }
        
        if(!FindEXPR())
        {
            return false;
        }
        
        token = GetToken();
        
        if(token != ']')
        {
            UnGetToken();
            Error("']'");
            Sync();
            return false;
        }
        
        if(!FindTERM_P())
        {
            return false;
        }
        
        return true;
    }
    
    return true;
}

//Returns true if a Timesop is found
bool FindTIMESOP()
{
    int token = GetToken();
    
    if(token != '*' && token != '/')
    {
        UnGetToken();
        return false;
    }
    
    return true;
}

//Loops through tokens until it finds a semi-colon or END
void Sync()
{
    int token = GetToken();
    
    while(token != ';' && token != END)
    {
        token = GetToken();
    }
}