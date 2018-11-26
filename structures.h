#ifndef _STRUCTURES_
#define _STRUCTURES_


typedef enum {decl, method, formalParam, var} decl_type;
typedef enum {class_, method_} table_type;
typedef enum {integer, intArray, boolean, boolArray, string_, stringArray, Void, nd} var_type;
typedef enum {IF_, IFELSE_, WHILE_, PRINT_, RETURN_, ATTR_, block} state_type; 
typedef enum {plus, minus, divide, mult, and_, or_, let, get, diff, equals, gt, lt, not_, uplus, uminus, leftover, intlit, boolit, dotlength, id, indexacao, newarray, parseint, function, void_} operand_type;

typedef struct _is_decl_list{
	char* id;
	struct _is_decl_list* next;
}is_decl_list;

typedef struct _is_var_decl{
    var_type vt;
    struct _is_decl_list* idl;
    struct _is_var_decl* next;
}is_var_decl;

typedef struct _is_formal_params{
    var_type vt;
    char* id;
    struct _is_formal_params* next;
}is_formal_params;


typedef struct _is_method_and_global_vars{
	decl_type dt;
    var_type vt;
	union{
		struct _is_decl_list* idl;
		struct _is_method_decl* imd;
	}data;
    struct _is_method_and_global_vars* next;
}is_method_and_global_vars;

typedef struct _is_unary_expression{
    struct _is_expression* p1;
}is_unary_expression;

typedef struct _is_two_arguments_expression{
    struct _is_expression* p1;
    struct _is_expression* p2;
}is_two_arguments_expression;

typedef struct _is_boolit_expression{
    char* p1;
}is_boolit_expression;

typedef struct _is_integer_expression{
    char* p1;
    int p;
}is_integer_expression;

typedef struct _is_index{
    struct _is_expression* variable;
    struct _is_expression* index;
}is_index;

typedef struct _is_new_array_expression{
    struct _is_expression* size;
    var_type type;
}is_new_array_expression;

typedef struct _is_expr_identifier{
    char* id;
}is_expr_identifier;

typedef struct _is_parseint_expression{
    char* id;
    struct _is_expression* ie;
}is_parseint_expression;

typedef struct _is_ifelse{
    struct _is_expression* ie;
    struct _is_statement_list* ie_true;
    struct _is_statement_list* ie_false;
}is_ifelse;

typedef struct _is_method_decl{
    char* id;
	struct _is_formal_params* ifp;
	struct _is_var_decl* ivd;
	struct _is_statement_list* isl;
}is_method_decl;

typedef struct _is_if{
    struct _is_expression* ie;
    struct _is_statement_list* isl;
}is_if;

typedef struct _is_while{
    struct _is_expression* ie;
    struct _is_statement_list* isl;
}is_while;

typedef struct _is_block{
    struct _is_statement_list* isl;
}is_block;

typedef struct _is_print{
    struct _is_expression* ie;
}is_print;

typedef struct _is_return{
    struct _is_expression* ie;
}is_return;

typedef struct _is_attr{
    char* id;
    struct _is_expression* index;
    struct _is_expression* ie;
}is_attr;

typedef struct _is_function_expression{
    char* id;
    struct _is_expression_list* iel;
}is_function_expression;

typedef struct _is_expression{
    operand_type ot;
    var_type vt;
    int llvmExp;
    char tipo;
    union{
        struct _is_unary_expression* i1e;
        struct _is_two_arguments_expression* i2e;
        struct _is_boolit_expression* ibe;
        struct _is_integer_expression* iie;
        struct _is_index* iindex;
        struct _is_expr_identifier* iid;
        struct _is_new_array_expression* inew;
        struct _is_parseint_expression* ipe;
        struct _is_function_expression* ife;
    }data;
}is_expression;

typedef struct _is_expression_list{
    struct _is_expression* ie;
    struct _is_expression_list* next;
}is_expression_list;


typedef struct _is_statement_list{
    state_type st;
    union{
        struct _is_attr* ia;
        struct _is_return* ir;
        struct _is_while* iw;
        struct _is_if* ii;
        struct _is_ifelse* iie;
        struct _is_print* ip;
        struct _is_block* ib;
    }data;
    struct _is_statement_list* next;
}is_statement_list;

typedef struct _is_program{
	struct _is_method_and_global_vars* imagv;
	char* id;
}is_program;

typedef struct _classe{
    char* name;
    struct _table* fm;
}classe;

typedef struct _table{
    decl_type dt;
    char* name;
    var_type type;
    int llvmExp;
    int size;
    char tipo;
    struct _table* next;
    struct _table* in;
}tabela;

#endif


