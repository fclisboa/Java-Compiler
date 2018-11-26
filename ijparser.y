%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "structures.h"
#include "functions.h"
#include "show.h"
#include "table.h"
#include "genCode.h"

void yyerror (char *s);
extern int col;
extern int line;
extern char *yytext;
is_program* program;
int error;
int ma=0;
%}

%union{
	int i;
	char* id;
	char* type;
	var_type vt;
	double value;
	is_program* ip;
	is_var_decl* ivd;
	is_decl_list* idl;
	is_expression*  ie;
	is_method_decl* imd;
	is_formal_params* ifp;
	is_statement_list* isl;
	is_expression_list* iel;
	is_method_and_global_vars* imagv;
}

%token <id> BOOLLIT
%token INT
%token BOOL
%token NEW
%token IF
%token ELSE
%token WHILE
%token PRINT
%token PARSEINT
%token CLASS
%token PUBLIC
%token STATIC
%token VOID
%token STRING
%token DOTLENGTH
%token RETURN
%token AND
%token OR
%token LET
%token GET
%token EQUALS
%token DIFF
%token <id> ID
%token <id> INTLIT
%token RESERVED

%type <type> Start 
%type <ip> Program 
%type <imagv> FieldDecl 
%type <imagv> MethodDecl 
%type <ifp> FormalParams 
%type <imagv> VarDecl 
%type <vt> Type 
%type <isl> Statement 
%type <ie> Expr 
%type <iel> Args 
%type <imagv> RepDecl 
%type <ivd> RepVarDecl
%type <isl> RepStatement
%type <ifp> RepCTI
%type <idl> RepCI
%type <iel> RepCE
%type <vt> COTV
%type <vt> COIB
%type <ie> COOP
%type <ifp> OptFPar
%type <i> OptSquares
%type <ie> OptSquaresExpr
%type <ie> OptExpr
%type <iel> OptArgs
%type <ivd> VarDeclInside
%type <ie> AntiMultiDimentionalArray



%right '='
%left OR
%left AND
%left EQUALS DIFF
%left GET LET '>' '<'
%left '+' '-'
%left '*' '/' '%'
%right '!'
%left DOTLENGTH
%left '(' ')' '[' ']'
%nonassoc ELSE

%%

Start : Program  {program=$1;};

Program : CLASS ID '{' RepDecl '}' {$$=insert_program($2,$4);};

FieldDecl : STATIC VarDecl {$$=$2;};

MethodDecl : PUBLIC STATIC COTV ID '(' OptFPar ')' '{' RepVarDecl RepStatement '}' {$$=create_method($3,$4,$6,$9,$10);};

FormalParams : Type ID RepCTI {$$=insert_formal_params($3,$2,$1);}
| STRING '[' ']' ID {$$=insert_formal_params(NULL,$4,stringArray);};

VarDecl : Type ID RepCI ';' {$$=create_field_decl_list(insert_id($3,$2), $1);};

VarDeclInside : Type ID RepCI ';' {$$=create_inside_decl_list(insert_id($3,$2),$1);};

Type : COIB OptSquares {$$=$1+$2;};

Statement : '{' RepStatement '}' {$$=create_statement_block($2);}
| IF '(' Expr ')' Statement ELSE Statement {$$=create_ifelse($3,$5,$7);}
| IF '(' Expr ')' Statement {$$=create_if($3,$5);}
| WHILE '(' Expr ')' Statement {$$=create_while($3,$5);}
| PRINT '(' Expr ')' ';' {$$=create_print($3);}
| ID OptSquaresExpr '=' Expr ';' {$$=create_attr($1,$2,$4);}
| RETURN OptExpr ';' {$$=create_return($2);};

Expr : COOP {$$=$1;}	
	| NEW COIB '[' Expr ']' {$$=create_new_array_expression($2,$4);}
	| AntiMultiDimentionalArray{$$=$1;}
	| '+' Expr %prec '!' {$$=create_unary_expression(uplus, $2);}
	| '!' Expr {$$=create_unary_expression(not_, $2);}
	| '-' Expr %prec '!' {$$=create_unary_expression(uminus, $2);}
	;

AntiMultiDimentionalArray: ID {$$=create_expression_id(id, $1);}
	| AntiMultiDimentionalArray '[' Expr ']' {$$=create_expression_index($1,$3);}
	| INTLIT {$$=create_expression_intlit(intlit, $1);}
	| BOOLLIT {$$=create_expression_boolit(boolit, $1);}
	| Expr DOTLENGTH {$$=create_unary_expression(dotlength, $1);}
	| '(' Expr ')' {$$=$2;}
	| PARSEINT '(' ID '[' Expr ']' ')' {$$=create_parseint_expression(parseint, $3, $5);}
	| ID '(' OptArgs ')' {$$=create_is_function_expression(function, $1,$3);}
	;

Args : Expr RepCE {$$=insert_expression($2, $1);}
		;

RepDecl :  {$$=NULL;}
		| FieldDecl RepDecl  {$$=insert_decl_list($2, $1);}
		| MethodDecl RepDecl {$$=insert_decl_list($2, $1);}
		;

RepVarDecl: {$$=NULL;}
		  | VarDeclInside RepVarDecl {$$=insert_inside_list($2, $1);}
		  ;

RepStatement: {$$=NULL;}
		    | Statement RepStatement {$$=insert_statement($2,$1);}
		    ;

RepCTI: {$$=NULL;}
	  | ',' Type ID RepCTI {$$=insert_formal_params($4,$3,$2);}
	  ;	  

RepCI: {$$=NULL;}
	 | ',' ID RepCI {$$=insert_id($3,$2);}
	;	 

RepCE: {$$=NULL;}
	 | ',' Expr RepCE {$$=insert_expression($3,$2);}
	;	 

COTV: Type {$$=$1;}
	| VOID {$$=Void;}
	;

COIB: INT {$$=integer;}
	| BOOL {$$=boolean;}
	;	

COOP: Expr AND Expr {$$=create_expression(and_, $1,$3);}
	| Expr OR Expr {$$=create_expression(or_,$1,$3);}
	| Expr LET Expr {$$=create_expression(let, $1,$3);}
	| Expr GET Expr {$$=create_expression(get, $1,$3);}
	| Expr DIFF Expr {$$=create_expression(diff, $1,$3);}
	| Expr EQUALS Expr {$$=create_expression(equals, $1,$3);}
	| Expr '+' Expr {$$=create_expression(plus, $1,$3);}
	| Expr '-' Expr {$$=create_expression(minus, $1,$3);}
	| Expr '<' Expr {$$=create_expression(lt, $1,$3);}
	| Expr '>' Expr {$$=create_expression(gt, $1,$3);}
	| Expr '*' Expr {$$=create_expression(mult, $1,$3);}
	| Expr '/' Expr {$$=create_expression(divide, $1,$3);}
	| Expr '%' Expr {$$=create_expression(leftover, $1,$3);}
		;	


OptFPar: {$$=NULL;}
	   | FormalParams {$$=$1;}
		;	   

OptSquares:  {$$=0;}
		  | '[' ']' {$$=1;}
		;		  

   

OptSquaresExpr: {$$=NULL;}
			  | '[' Expr ']' {$$=$2;}
		;			  

OptExpr: {$$=NULL;}
	   | Expr {$$=$1;}
		;	   

OptArgs: {$$=NULL;}
	   | Args {$$=$1;}
		;	   

%%


void yyerror (char *s) {
	printf("Line %d, col %d: %s: %s\n", line, (int)(col+1-strlen(yytext)), s, yytext);
	error=1;
}

int main(int argc, char** argv)
{
    yyparse();
    classe* table;
	int t=0;
	int s=0;
	int semanticError=0;
    if(argc>1){
		int i;
		for(i=1;i<argc;i++){
			if(argv[i][0]=='-' && argv[i][1]=='t') t=1;
			if(argv[i][0]=='-' && argv[i][1]=='s') s=1;
		}
	}


	if(!error){
		if(t)
			printProgram(program,0);
		
		table=buildTable(program);	
		checkTypes(program, table->fm);
		
		if(s){
			printTable(table);
		}
		
		generateCode(program,table->fm);
	}
	
	return 0;

}


