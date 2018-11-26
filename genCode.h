
#ifndef GENCODE_H
#define	GENCODE_H
#include "structures.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "y.tab.h"
#include "table.h"
#include "show.h"
#include "genCode.h"

void generateStatements(is_statement_list* isl,tabela* m, tabela* g);
void generateCode(is_program* program, tabela * g);
void generatePrintf(is_expression* ie, tabela* t, tabela* g);
int generateExpression(is_expression* ie, tabela* t, tabela* g);
void generateAttr(is_attr* ia, tabela* t, tabela* g);
void generateDeclaration(is_var_decl* ivd,tabela* m, tabela* g);
void generateIf(is_if* ii, tabela* t, tabela* g);
void generateIfElse(is_ifelse* iie, tabela* t, tabela* g);
void generateWhile(is_while* iw, tabela* t, tabela* g);
void generateReturn(is_return* ir, tabela* t, tabela* g);

#endif

