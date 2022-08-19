%option noyywrap

%{
    #include "tokens.hh"
%}

%%

"hfloat"  return KEY_HFLOAT;//keywords
"hchar"  return KEY_HCHAR;//keywords
"hint"  return KEY_HINT;//keywords
"hstring" return KEY_HSTRING; // keyword
"hwhile" return KEY_HWHILE; // keyword
"hprint" return KEY_HPRINT; // keyword
"hif" return KEY_HIF; //keyword

"+" return OP_PLUS;//operator
"-" return OP_MINUS;//operator
"*" return OP_MULT;//operator
"/" return OP_DIV;//operator
"=" return OP_EQUALS;//operator
"!" return OP_NOT;//operator
"<" return OP_LESS_THAN;//operator
">" return OP_GREATER_THAN;//operator
"<=" return OP_LESS_EQUAL; // operator
">=" return OP_GREAT_EQUAL; // operator
"==" return OP_DOUB_EQUAL; // operator
"!=" return OP_NOT_EQUAL; // operator
";" return SEMI_COL; // ends
"," return COMMA;
"(" return OPEN_PAREN;
")" return CLOSE_PAREN;
"{" return OPEN_CURLY;
"}" return CLOSE_CURLY;
"\n" ;
" " ;

[0-9]* return LIT_HINT;
[0-9]*"."[0-9]* return LIT_HFLOAT;
"'"."'" return LIT_HCHAR; 
\"(\\.|[^"\\])*\" return LIT_HSTRING;
[a-zA-Z_][a-zA-Z0-9_]* return IDENTIFIER;

. ;

%%
