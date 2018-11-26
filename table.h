
#ifndef TABLE_H
#define	TABLE_H
#include "structures.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "y.tab.h"
#include "table.h"
#include "show.h"


classe* createClasse(char* id);

tabela* createParams( is_decl_list* idl, var_type vt,decl_type d, tabela* t);

tabela* createFormalParams( is_formal_params* ifp, tabela* t);

tabela* insertParams(tabela* new_, tabela* t);

tabela* createMethod(is_method_and_global_vars* met, tabela* t);

tabela* searchParam(tabela* t, char* nome);

classe* buildTable(is_program* program);

var_type checkExpressions(is_expression* ie, tabela* t, tabela* g);

var_type methodParam(tabela* m, int n);

tabela* searchMethod(tabela* g, char* id);

tabela* searchVar(tabela* t, tabela* g, char* nome);

int checkTypes(is_program* program, tabela * g);

int testISL(is_statement_list* isl,tabela* m, tabela* g);

char* printOperator(is_expression* ie);

#endif

