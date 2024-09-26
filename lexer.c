/***************************************************************************************************************************************************
*Author         :MANOHARAN BALAKRISHNAN
*Date           :02/08/2024
*File           :lexer.c
*Title          :Lexer function to get tokens.
*Description    :This file's main objective is to fetch tokens from source code and decide it token type and return that token node.
****************************************************************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

//an array to hold all the keywords.
static const char* keywords[MAX_KEYWORDS] = {                            
    "int", "float", "return", "if", "else", "while", "for", "do", "break", "continue",
    "char", "double", "void", "switch", "case", "default", "const", "static", "sizeof", "struct"
};

//a string to store all operators.
static const char* operators = "+=-*/%!<>|&";
//a string to store all special characters.
static const char* specialCharacters = ",;{}()[]";
FILE *fptr=NULL;                                //file pointer to access the source file.

/**
 * @brief function to initialize and open the file into a file pointer.
 * @param filename source filename which have the source to code which is going to be analysed. 
 * @return void function.
 */
void initializeLexer(const char* filename){     //initialise function to initialise and skip preprocessor directives.
    fptr=fopen(filename, "r");                  //opens the source file into file pointer.
    char ch=fgetc(fptr);                        //fetching a single character from that file pointer.
    int flag=1;
    while(flag){
        if(ch=='#'){
            while(ch!='\n'){                    
                ch=fgetc(fptr);                 //running a loop to skip the preprocessor directives.
            }
        }
        ch=fgetc(fptr);
        if(ch!='#') flag=0;
    }
    fseek(fptr, -1, SEEK_CUR);
}

/**
 * @brief getNextToken->function to read token by token from the source file. 
 * @return return token(aka struct node).
 */
Token getNextToken(){                           //function to fetch every tokens from source file.
    Token token;                                //variable to store the token and token type.
    char ch;
    int i=0;
    token.type=UNKNOWN;                         //initially declaring token type as unknown.

    do {
        ch = fgetc(fptr);
        if (ch == EOF) {
            token.lexeme[0] = '\0';             //snippet to skip the whitespaces in the source code.
            token.type = END_OF_FILE;
            return token;
        }
    } while (isspace(ch));
    
    if(ch=='(') c1++;           
    if(ch==')') c1--;                           //incrementing and decrementing the count values for validation of braces and paranthesis.
    if(ch=='{') c2++;                       
    if(ch=='}') c2--;

    if(isOperator(ch)){                         //function call to check whether the character is operator or not.               
        token.lexeme[0]=ch;
        token.lexeme[1]='\0';                   
        token.type=OPERATOR;
        return token;
    }                                                   
    else if(isSpecialCharacter(ch)){             //function call to check whether the character is special character or not.
        token.lexeme[0]=ch;
        token.lexeme[1]='\0';
        token.type=SPECIAL_CHARACTER;
        return token;
    }
    else{
        if(ch=='\"'){
            ch=fgetc(fptr);                                    
            while(ch!='\"'){                        //condtion to validate a string literal is valid or not.
                token.lexeme[i]=ch;
                ch=fgetc(fptr);
                if(ch==')'||ch==';'||ch=='\n'){     //condtion to check whether the double quates are closed or not.
                    i++;
                    token.lexeme[i]='\0';           //terminating string with NULL character.
                    token.type=UNKNOWN;
                    fseek(fptr,-1,SEEK_CUR);
                    return token;
                }
                i++;
            }
            token.lexeme[i]='\0';
            token.type=CONSTANT;
            fseek(fptr,1,SEEK_CUR);
            return token;
        }

        if(ch=='\''){                                           //condtion to validate a character constant is valid or not.
            ch=fgetc(fptr);
            token.lexeme[0]=ch;
            token.lexeme[1]='\0';
            ch=fgetc(fptr);
            if(ch!='\'') token.type=UNKNOWN;
            else token.type=CONSTANT;
            return token;
        }

    while(isalnum(ch) || ch == '.'|| ch == '_' ){                   //reading characters into  string(lexeme).
        token.lexeme[i]=ch;
        i++;
        ch=fgetc(fptr);
    }


    token.lexeme[i]='\0';
    if(isConstant(token.lexeme)){               //function to check for integer constants.
        token.type=CONSTANT;
        return token;
    }
    else if(isKeyword(token.lexeme)){           //function call for checking keywords.
        token.type=KEYWORD; 
        return token;
    }
    else if(isIdentifier(token.lexeme)){        //function call for checking identifier.
            token.type=IDENTIFIER;
            fseek(fptr,-1,SEEK_CUR); 
            return token;
    }
    else if(ch==EOF){                           //condtion to check for the end of the file.
        token.lexeme[i]='\0';
        token.type=END_OF_FILE;
        return token;
    }
    else{                                       //returning token type as unknown when all other condition fails.
        token.lexeme[i]=ch;
        token.type=UNKNOWN;
        return token;
    }
    }
    

}
/**
 * @brief isKeyword->function to check whether the token is keyword or not.
 * @param  str token fetched from the source file.
 * @return returns 1 on success and 0 on failure.
 */
int isKeyword(const char* str){
    //loop to compare keywords array with token.
     for(int i=0;i<MAX_KEYWORDS;i++){
        if(strcmp(keywords[i],str)==0)          //comparing keywords array element with given token.     
        return 1;
    }
    return 0;
}
/**
 * @brief isOperator->function to check whether the token is operator or not.
 * @param  ch character fetched from the source file.
 * @return returns 1 on success and 0 on failure.
 */
int isOperator(char ch){                            //function for operator.
    for (int i = 0; operators[i] != '\0'; i++) {
        if (operators[i] == ch) {                   //comparing operators with the ch from file.
            return 1;           
        }
    }
    return 0;
}
/**
 * @brief isSpecialCharacter->function to check whether the token is special character or not.
 * @param  ch character fetched from the source file.
 * @return returns 1 on success and 0 on failure.
 */
int isSpecialCharacter(char ch){                                                                 
   for (int i = 0; i < sizeof(specialCharacters) / sizeof(specialCharacters[0]); i++) {
        if (specialCharacters[i] == ch) {                                                      //comparing special characters string with the ch from file.                                           
            return 1;
        }
    }
    return 0;
}
/**
 * @brief isConstant->function to check whether the token is constant or not.
 * @param  str token fetched from the source file.
 * @return returns 1 on success and 0 on failure.
 */
int isConstant(const char* str){
    if(str[0]=='0'&&(str[1]=='x'||str[1]=='X')){
        for(int i=2;str[i]!='\0';i++){
            if(!((str[i]>='0'&&str[i]<='9')||(str[i]>='a'&&str[i]<='f')||(str[i]>='A'&&str[i]<='F'))) return 0;//checking whether the token is hexadecimal or not.
        }
        return 1;
    }
    else  if(str[0]=='0'&&(str[1]=='b'||str[1]=='B')){
        for(int i=2;str[i]!='\0';i++){
            if(!((str[i]>='0'&&str[i]<='1'))) return 0;//checking whether the token is hexadecimal or not.
        }
        return 1;
    }
    else if(str[0]=='0'){
        for(int i=1;str[i]!='\0';i++){
            if(!(str[i]>='0'&&str[i]<='7')) return 0;                   //checking whether the token is octal number or not.
        }
        return 1;
    }
    else{
        int i=0;
        int flag=0;
        while(str[i]){
            if(str[i]=='.') flag=1;                                         //checking whether the token is float or not.
            i++;
        }

        if(flag){
            char *endptr;
            double val=strtod(str,&endptr);
            if (*endptr == '\0' && endptr != str) {                         //checking whether the token is valid fkoat number or not.
                 return 1; // It's a valid float
            } 
            else{
                return 0; // It's an unknown token
            }

        }
        else{
            for(int i=0;str[i]!='\0';i++){
                if(!isdigit(str[i])) return 0;                              //checking whether the token is valid decimal number or not.
            }
            return 1;
        }
    }
}
/**
 * @brief isIdentifier->function to check whether the token is identifier or not.
 * @param  str string token fetched from the source file.
 * @return returns 1 on success and 0 on failure.
 */
int isIdentifier(const char* str){
    if(isalpha(str[0])) return 1;       //checking the first character of the string for valid identifier and returns 1 if it is valid.                  
    else return 0;
}