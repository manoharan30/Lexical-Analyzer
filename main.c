/***************************************************************************************************************************************************
*Author         :MANOHARAN BALAKRISHNAN
*Date           :02/08/2024
*File           :main.c
*Title          :To initialise and call the lexer function.
*Description    :This main file contains CLA error validation and then initialiser function call and lexer function call
****************************************************************************************************************************************************/
#include "lexer.h"                 

int c1=0;                                                                 //count1 for braces validation
int c2=0;                                                                 //count2 for parenthesis validation
/**
 * @brief main->function to initialize .
 * @param argc command line agument count.
 * @param argv arguments passed through command line. 
 * @return return 0 upon success and non zero upon failure.
 */
int main(int argc, char *argv[]) {
    if(argc==1){                                                         //validation for whether the user has passed the source file through CLA or not
     printf("ERROR:PLEASE PASS THE FILE NAME THROUGH COMMAND LINE");
     return 0;
    }
    initializeLexer(argv[1]);                                            //function call to open the source file in file ptr and skip preprocessor directives

    Token token;                                                         //declaring a token variable to collect every single token
    while ((token = getNextToken()).type != END_OF_FILE) {
        ptoken(&token);                                               //function call to print tokens and token types
    }

     if (c1 != 0) printf("ERROR: Unmatched parentheses in the program\n");     
     if (c2 != 0) printf("ERROR: Unmatched braces in the program\n");


    return 0;
}

/**
 * @brief ptoken->function to print tokens.
 * @param token(aka struct node). 
 * @return void function.
 */
void ptoken(Token *token){                                           
    switch (token->type)                                              //switch case using token type to print the tokens.
    {
    case 0:
         printf("%-15s:Keyword\n", token->lexeme);
        break;

    case 1:
         printf("%-15s:Operator\n", token->lexeme);
        break;

    case 2:
         printf("%-15s:Special Character\n", token->lexeme);
         break;

    case 3:
         printf("%-15s:Constant\n", token->lexeme);
         break;

    case 4:
         printf("%-15s:Identifier\n", token->lexeme);
         break;

    case 5:
         printf("%-15s:Unknown Token\n", token->lexeme);
         printf("ERROR: %s is an Unknown token\n",token->lexeme);
         break;

    case 6:
         printf("%-15s:End of File\n", token->lexeme);
         break;

    default:
        break;
    }
}
