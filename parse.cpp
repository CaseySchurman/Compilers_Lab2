#include <iostream>
#include "lex.h"
#include "parse.h"

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
//*******************************************
bool FindPROG()
{
    Error("Program");
    return false;
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

