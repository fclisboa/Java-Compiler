
%pointer

%X comentario

%{
	
	#include "structures.h"
	#include "y.tab.h"
	#include <string.h>
        
    int beginComentCol;
    int beginComentLine;
    int line=1;
    int col=0;
    char* text;
%}

%%
"/*"                         {BEGIN comentario;beginComentCol=col;beginComentLine=line;col=col+2;}
<comentario>\*\/             {BEGIN 0; col+=2;}
<comentario>.                {col+=1;}
<comentario>\n               {col=0;line++;}
<comentario><<EOF>>          {printf("Line %d, col %d: unterminated comment\n",beginComentLine,beginComentCol+1);return 0;}
"//".*                       {col+=yyleng;}
\n                           {col=0;line++;}
("true"|"false")             {col+=yyleng; yylval.type=strdup(yytext);return BOOLLIT; }
"int" 						 {col+=yyleng; return INT;}
"boolean"					 {col+=yyleng; return BOOL;}
"new" 						 {col+=yyleng; return NEW;}
"if" 						 {col+=yyleng; return IF;}
"else" 						 {col+=yyleng; return ELSE;}
"while"						 {col+=yyleng; return WHILE;}
"System.out.println"		 {col+=yyleng; return PRINT;}
"Integer.parseInt"			 {col+=yyleng; return PARSEINT;}
"class" 					 {col+=yyleng; return CLASS;}
"public" 					 {col+=yyleng; return PUBLIC;}
"static" 					 {col+=yyleng; return STATIC;}
"void" 						 {col+=yyleng; return VOID;}
"String" 					 {col+=yyleng; return STRING;}
".length" 					 {col+=yyleng; return DOTLENGTH;}
"return" 					 {col+=yyleng; return RETURN;}
"&&"						 {col+=yyleng; return AND;}
"||" 					 	 {col+=yyleng; return OR;}
"<="						 {col+=yyleng; return LET;}
">="	 		 		 	 {col+=yyleng; return GET;}
"=="						 {col+=yyleng; return EQUALS;}
"!="					 	 {col+=yyleng; return DIFF;}					
","|";"|"!"|"("|")"|"{" 	 {col+=yyleng; return yytext[0];}
"}"|"["|"]"|"*"|"/"|"%"		 {col+=yyleng; return yytext[0];}
"+"|"-"|"<"|">"|"="			 {col+=yyleng; return yytext[0];}

"abstract"|"continue"|"for"|"switch"|"assert"|"default"|"goto"|"package"					{col+=yyleng; return RESERVED;}
"do"|"private"|"this"|"break"|"double"|"implements"|"protected"|"throw"|"--"				{col+=yyleng; return RESERVED;}
"byte"|"import"|"throws"|"case"|"enum"|"instanceof"|"transient"|"null"|"++"					{col+=yyleng; return RESERVED;}
"catch"|"extends"|"short"|"try"|"char"|"final"|"interface"|"synchronized"					{col+=yyleng; return RESERVED;}
"finally"|"long"|"strictfp"|"volatile"|"const"|"float"|"native"|"super"						{col+=yyleng; return RESERVED;}

[a-zA-Z\_\$][a-zA-Z0-9\_\$]* {col+=yyleng; yylval.id=(char*)strdup(yytext);return ID;}
[0-9]+						 {col+=yyleng; yylval.id=(char*)strdup(yytext);return INTLIT;}
0x[0-9a-fA-F]+				 {col+=yyleng; yylval.id=(char*)strdup(yytext);return INTLIT;}
\ |\t						 {col+=yyleng;}
.							 {col+=yyleng; printf("Line %d, col %d: illegal character ('%c')\n",line,col,yytext[0]);}


%%

int yywrap()
{
	return 1;
}


