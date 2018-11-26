/* 
 * File:   show.h
 * Author: francisco
 *
 * Created on April 18, 2014, 6:53 PM
 */

#ifndef SHOW_H
#define	SHOW_H
void printProgram(is_program* program, int tab);

void printMethod(is_method_and_global_vars* method, int tab);

void printDecl(is_method_and_global_vars* field, int n);

void printFormalParameters(is_formal_params* ifp, int n);

char* convertType(var_type vt);

void tab(int tab);

void printMethodBody(is_var_decl* ivd, is_statement_list* isl, int n);

void printStatements(is_statement_list* isl, int n);

char* convertType2(var_type vt);

#endif	/* SHOW_H */




