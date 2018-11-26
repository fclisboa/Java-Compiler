#include "structures.h"

is_program* insert_program(char* a, is_method_and_global_vars* imagv_);

is_method_and_global_vars* insert_decl_list(is_method_and_global_vars* idl_, is_method_and_global_vars* n);

is_method_and_global_vars* create_field_decl_list(is_decl_list* idl_, var_type t);

is_decl_list* insert_id(is_decl_list* list, char* id_);

is_formal_params* insert_formal_params(is_formal_params* list, char* id_, var_type t);

is_method_and_global_vars* create_method(var_type t, char* id_,is_formal_params* ifp, is_var_decl* ivd, is_statement_list* isl);

is_var_decl* insert_inside_list(is_var_decl* ivd_, is_var_decl* n);

is_var_decl* create_inside_decl_list(is_decl_list* idl_, var_type t);

is_statement_list* create_return(is_expression* ie);

is_statement_list* create_print(is_expression* ie);

is_statement_list* create_attr(char* id, is_expression* index, is_expression* ie);

is_statement_list* create_while(is_expression* ie, is_statement_list* isl);

is_statement_list* create_if(is_expression* ie, is_statement_list* isl);

is_statement_list* create_ifelse(is_expression* ie, is_statement_list* ist, is_statement_list* isf);

is_statement_list* insert_statement(is_statement_list* list, is_statement_list* n);

is_expression* create_expression(operand_type ot, is_expression* ie1, is_expression* ie2);

is_expression* create_expression_id(operand_type ot, char* id);

is_expression* create_expression_intlit(operand_type ot, char* id);

is_expression* create_expression_boolit(operand_type ot, char* id);

is_expression* create_expression_index(is_expression* variable , is_expression* index);

is_expression* create_new_array_expression( var_type type, is_expression* size);

is_expression* create_unary_expression(operand_type ot, is_expression* ie1);

is_expression* create_parseint_expression(operand_type ot, char* id, is_expression* ie);
        
is_expression* create_is_function_expression(operand_type ot, char* id, is_expression_list* iel);

is_expression_list* insert_expression(is_expression_list* list, is_expression* n);

is_statement_list* create_statement_block(is_statement_list* isl);











