#include "functions.h"
#include "structures.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

is_program* insert_program(char* a, is_method_and_global_vars* imagv_){
	is_program* ip=(is_program*)malloc(sizeof(is_program));
	ip->id=a;
	ip->imagv=imagv_;
	return ip;
}

is_formal_params* insert_formal_params(is_formal_params* list, char* id_, var_type t){
    is_formal_params* ifp=(is_formal_params*)malloc(sizeof(is_formal_params));	//Cria novo nó na lista
    ifp->id=strdup(id_);
    ifp->vt=t;
    ifp->next=list;
    return ifp;
}

is_method_and_global_vars* insert_decl_list(is_method_and_global_vars* idl_, is_method_and_global_vars* n){   
    n->next=idl_;
    return n;
}
is_var_decl* insert_inside_list(is_var_decl* ivd_, is_var_decl* n){   
    n->next=ivd_;
    return n;
}

is_decl_list* insert_id(is_decl_list* list, char* id_){
    is_decl_list* idl=(is_decl_list*)malloc(sizeof(is_decl_list));	//Cria novo nó na lista
    idl->id=strdup(id_);
    idl->next=list;
    					//adiciona no final da lista
    return idl;
}

is_statement_list* insert_statement(is_statement_list* list, is_statement_list* n){
    if(n->st==block && n->data.ib->isl==NULL)
        return list;
    if(n!=NULL)
        n->next=list;
    return n;
}

is_expression_list* insert_expression(is_expression_list* list, is_expression* n){
    is_expression_list* iel=(is_expression_list*)malloc(sizeof(is_expression_list));
    iel->ie=n;
    iel->next=list;
    return iel;
}

is_statement_list* create_statement_block(is_statement_list* isl){
    is_statement_list* isl_=(is_statement_list*)malloc(sizeof(is_statement_list));
    is_block* ib_=(is_block*)malloc(sizeof(is_block));
    ib_->isl=isl;
    isl_->st=block;
    isl_->data.ib=ib_;
    return isl_;
}

is_statement_list* create_ifelse(is_expression* ie, is_statement_list* ist, is_statement_list* isf){
    is_ifelse* iie=(is_ifelse*)malloc(sizeof(is_ifelse));
    is_statement_list* isl=(is_statement_list*)malloc(sizeof(is_statement_list));
    iie->ie=ie;
    iie->ie_true=ist;
    iie->ie_false=isf;
    isl->st=IFELSE_;
    isl->data.iie=iie;
    return isl;
}

is_statement_list* create_if(is_expression* ie, is_statement_list* ifCode){
    is_if* ii=(is_if*)malloc(sizeof(is_if));
    is_statement_list* isl=(is_statement_list*)malloc(sizeof(is_statement_list));
    ii->ie=ie;
    ii->isl=ifCode;
    isl->st=IF_;
    isl->data.ii=ii;
    return isl;
}

is_statement_list* create_while(is_expression* ie, is_statement_list* ifCode){
    is_while* iw=(is_while*)malloc(sizeof(is_while));
    is_statement_list* isl=(is_statement_list*)malloc(sizeof(is_statement_list));
    iw->ie=ie;
    iw->isl=ifCode;
    isl->st=WHILE_;
    isl->data.iw=iw;
    return isl;
}

is_statement_list* create_attr(char* id, is_expression* index, is_expression* ie){
    is_attr* ia=(is_attr*)malloc(sizeof(is_attr));
    is_statement_list* isl=(is_statement_list*)malloc(sizeof(is_statement_list));
    ia->id=strdup(id);
    ia->index=index;
    ia->ie=ie;
    isl->st=ATTR_;
    isl->data.ia=ia;
    return isl;
}

is_statement_list* create_print(is_expression* ie){
    is_print* ip=(is_print*)malloc(sizeof(is_print));
    is_statement_list* isl=(is_statement_list*)malloc(sizeof(is_statement_list));
    ip->ie=ie;
    isl->st=PRINT_;
    isl->data.ip=ip;
    return isl;
}

is_statement_list* create_return(is_expression* ie){
    is_return* ir=(is_return*)malloc(sizeof(is_return));
    is_statement_list* isl=(is_statement_list*)malloc(sizeof(is_statement_list));
    if(ie==NULL){
        ie=(is_expression*)malloc(sizeof(is_expression));
        ie->vt=Void;
        ie->ot=void_;
    }
    ir->ie=ie;
    isl->st=RETURN_;
    isl->data.ir=ir;
    return isl;
}

is_var_decl* create_inside_decl_list(is_decl_list* idl_, var_type t){
    is_var_decl* ivd=(is_var_decl*)malloc(sizeof(is_var_decl));	//Cria novo nó na lista
    ivd->vt= t;
    ivd->idl=idl_;
    ivd->next=NULL;
    return ivd;
}

is_method_and_global_vars* create_field_decl_list(is_decl_list* idl_, var_type t){
	is_method_and_global_vars* imagv=(is_method_and_global_vars*)malloc(sizeof(is_method_and_global_vars));	//Cria novo nó na lista
	imagv->data.idl= idl_;
        imagv->dt=decl;
        imagv->vt=t;
        return imagv;
}

is_method_and_global_vars* create_method(var_type t, char* id_,is_formal_params* ifp, is_var_decl* ivd, is_statement_list* isl){
    is_method_and_global_vars* imagv=(is_method_and_global_vars*)malloc(sizeof(is_method_and_global_vars));	//Cria novo nó na lista
	is_method_decl* imd=(is_method_decl*)malloc(sizeof(is_method_decl));
        imd->ivd=ivd;
        imd->ifp=ifp;
        imd->isl=isl;
        imd->id=id_;
        
        imagv->data.imd= imd;
        imagv->dt=method;
        imagv->vt=t;
        return imagv;
}

is_expression* create_expression(operand_type ot, is_expression* ie1, is_expression* ie2){
    is_expression* ie=(is_expression*)malloc(sizeof(is_expression));
    is_two_arguments_expression* exp2=(is_two_arguments_expression*)malloc(sizeof(is_two_arguments_expression));

    exp2->p1=ie1;
    exp2->p2=ie2;
    
    ie->ot=ot;
    ie->data.i2e=exp2;
    ie->vt=nd;
    return ie;
}

is_expression* create_unary_expression(operand_type ot, is_expression* ie1){
    is_expression* ie=(is_expression*)malloc(sizeof(is_expression));
    is_unary_expression* exp2=(is_unary_expression*)malloc(sizeof(is_unary_expression));

    exp2->p1=ie1;
    
    ie->ot=ot;
    ie->data.i1e=exp2;
    ie->vt=nd;
    return ie;
}

is_expression* create_is_function_expression(operand_type ot, char* id, is_expression_list* iel){
    is_function_expression* ife=(is_function_expression*)malloc(sizeof(is_function_expression));
    is_expression* ie=(is_expression*)malloc(sizeof(is_expression));
    ife->id=id;
    ife->iel=iel;
    ie->ot=ot;
    ie->data.ife=ife;
    ie->vt=nd;
    return ie;
}


is_expression* create_parseint_expression(operand_type ot, char* id, is_expression* ie){
    is_expression* ie_=(is_expression*)malloc(sizeof(is_expression));
    is_parseint_expression* ipe=(is_parseint_expression*)malloc(sizeof(is_parseint_expression));
    ipe->id=id;
    ipe->ie=ie;
    ie_->ot=ot;
    ie_->data.ipe=ipe;
    ie_->vt=nd;
    return ie_;
}

is_expression* create_expression_id(operand_type ot, char* id){
    is_expression* ie=(is_expression*)malloc(sizeof(is_expression));
    is_expr_identifier* iei=(is_expr_identifier*)malloc(sizeof(is_expression));
    iei->id=strdup(id);
    ie->ot=ot;
    ie->data.iid=iei;
    ie->vt=nd;
    return ie;
}

is_expression* create_expression_intlit(operand_type ot, char* id){
    is_expression* ie=(is_expression*)malloc(sizeof(is_expression));
    is_integer_expression* iei=(is_integer_expression*)malloc(sizeof(is_integer_expression));
    iei->p1=strdup(id);
    ie->ot=ot;
    ie->data.iie=iei;
    ie->vt=nd;
    return ie;
}


is_expression* create_expression_boolit(operand_type ot, char* id){
    is_expression* ie=(is_expression*)malloc(sizeof(is_expression));
    is_boolit_expression* ibe=(is_boolit_expression*)malloc(sizeof(is_boolit_expression));
    ibe->p1=strdup(id);
    ie->ot=ot;
    ie->data.ibe=ibe;
    ie->vt=nd;
    return ie;
}

is_expression* create_expression_index(is_expression* variable , is_expression* index){
    
    is_expression* ie=(is_expression*)malloc(sizeof(is_expression));
    is_index* ii=(is_index*)malloc(sizeof(is_index));
    ii->index=index;
    ii->variable=variable;
    ie->ot=indexacao;
    ie->data.iindex=ii;
    ie->vt=nd;
    return ie;
}

is_expression* create_new_array_expression( var_type type, is_expression* size){
    is_expression* ie=(is_expression*)malloc(sizeof(is_expression));
    is_new_array_expression* inea=(is_new_array_expression*)malloc(sizeof(is_new_array_expression));
    inea->size=size;
    inea->type=type;
    ie->ot=newarray;
    ie->data.inew=inea;
    ie->vt=nd;
    return ie;
}



