#include "functions.h"
#include "structures.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "show.h"
#include "y.tab.h"



void printProgram(is_program* program, int n) {
    tab(n);
    if (program == NULL)
        printf("Null\n");
    else {
        printf("Program\n");
        n++;
        tab(n);
        printf("Id(%s)\n", program->id);
        is_method_and_global_vars* aux;
        for (aux = program->imagv; aux != NULL; aux = aux->next) {
            if (aux->dt == decl) {
                printDecl(aux, n);
            } else {
                printMethod(aux, n);
            }
        }
    }
}

void printStuff(tabela* table){
    tabela* aux;
    for(aux=table;aux!=NULL;aux=aux->next){
        switch(aux->dt){
            case(decl):
                printf("%s\t%s\n",aux->name, convertType2(aux->type));
                break;
            case(var):
                printf("%s\t%s\n",aux->name, convertType2(aux->type));
                break;
            case(method):
                printf("%s\tmethod\n",aux->name);
                break;
            case(formalParam):
                printf("%s\t%s\tparam\n",aux->name, convertType2(aux->type));
                break;
        }
    }
}

void printTable(classe* table){
    if(table!=NULL){
        printf("===== Class %s Symbol Table =====\n",table->name);
        if(table->fm!=NULL){
            printStuff(table->fm);
        }
        tabela* aux;
        for(aux=table->fm;aux!=NULL;aux=aux->next){
            if(aux->dt==method){
                printf("\n===== Method %s Symbol Table =====\n",aux->name);
                printf("return\t%s\n",convertType2(aux->type));
                printStuff(aux->in);
            }
        }
    }
}

void printDecl(is_method_and_global_vars* field, int n) {
    tab(n++);
    is_decl_list* aux = field->data.idl;
    printf("VarDecl\n");
    tab(n);
    printf("%s\n",convertType(field->vt));
    for (; aux != NULL; aux = aux->next) {
        tab(n);
        printf("Id(%s)\n", aux->id);
    }
    n--;

}

void printMethod(is_method_and_global_vars* method, int n) {
    tab(n);
    printf("MethodDecl\n");
    tab(++n);
    printf("%s\n", convertType(method->vt));
    tab(n);
    printf("Id(%s)\n", method->data.imd->id);
    printFormalParameters(method->data.imd->ifp, n);
    printMethodBody(method->data.imd->ivd, method->data.imd->isl, n);


}

void printExpressions(is_expression* ie, int n){
    
    switch(ie->ot){
        case(boolit):
            tab(n++);
            printf("BoolLit(%s)\n",ie->data.ibe->p1);
            n--;
            break;
        case(parseint):
            tab(n++);
            printf("ParseArgs\n");
            tab(n);
            printf("Id(%s)\n",ie->data.ipe->id);
            printExpressions(ie->data.ipe->ie,n);
            n--;
            break;
        case(intlit):
            tab(n);
            printf("IntLit(%s)\n",ie->data.iie->p1);
            break;
        case(id):
            tab(n);
            printf("Id(%s)\n",ie->data.iid->id);
            break;
        case(plus):
            tab(n);
            printf("Add\n");
            n++;
            printExpressions(ie->data.i2e->p1,n);
            printExpressions(ie->data.i2e->p2,n);
            n--;
            break;
        case(minus):
            tab(n);
            printf("Sub\n");
            n++;
            printExpressions(ie->data.i2e->p1,n);
            printExpressions(ie->data.i2e->p2,n);
            n--;
            break;
        case(mult):
            tab(n);
            printf("Mul\n");
            n++;
            printExpressions(ie->data.i2e->p1,n);
            printExpressions(ie->data.i2e->p2,n);
            n--;
            break;
        case(divide):
            tab(n);
            printf("Div\n");
            n++;
            printExpressions(ie->data.i2e->p1,n);
            printExpressions(ie->data.i2e->p2,n);
            n--;
            break;
        case(and_):
            tab(n);
            printf("And\n");
            n++;
            printExpressions(ie->data.i2e->p1,n);
            printExpressions(ie->data.i2e->p2,n);
            n--;
            break;
        case(or_):
            tab(n);
            printf("Or\n");
            n++;
            printExpressions(ie->data.i2e->p1,n);
            printExpressions(ie->data.i2e->p2,n);
            n--;
            break;
        case(let):
            tab(n);
            printf("Leq\n");
            n++;
            printExpressions(ie->data.i2e->p1,n);
            printExpressions(ie->data.i2e->p2,n);
            n--;
            break;
        case(get):
            tab(n);
            printf("Geq\n");
            n++;
            printExpressions(ie->data.i2e->p1,n);
            printExpressions(ie->data.i2e->p2,n);
            n--;
            break;
        case(diff):
            tab(n);
            printf("Neq\n");
            n++;
            printExpressions(ie->data.i2e->p1,n);
            printExpressions(ie->data.i2e->p2,n);
            n--;
            break;
        case(equals):
            tab(n);
            printf("Eq\n");
            n++;
            printExpressions(ie->data.i2e->p1,n);
            printExpressions(ie->data.i2e->p2,n);
            n--;
            break;
        case(gt):
            tab(n);
            printf("Gt\n");
            n++;
            printExpressions(ie->data.i2e->p1,n);
            printExpressions(ie->data.i2e->p2,n);
            n--;
            break;
        case(lt):
            tab(n);
            printf("Lt\n");
            n++;
            printExpressions(ie->data.i2e->p1,n);
            printExpressions(ie->data.i2e->p2,n);
            n--;
            break;
        case(not_):
            tab(n);
            printf("Not\n");
            n++;
            printExpressions(ie->data.i1e->p1,n);
            n--;
            break;
        case(leftover):
            tab(n);
            printf("Mod\n");
            n++;
            printExpressions(ie->data.i2e->p1,n);
            printExpressions(ie->data.i2e->p2,n);
            n--;
            break;
        case(dotlength):
            tab(n);
            printf("Length\n");
            n++;
            printExpressions(ie->data.i1e->p1,n);
            n--;
            break;
        case(uplus):
            tab(n);
            printf("Plus\n");
            n++;
            printExpressions(ie->data.i1e->p1,n);
            n--;
            break;
        case(uminus):
            tab(n);
            printf("Minus\n");
            n++;
            printExpressions(ie->data.i1e->p1,n);
            n--;
            break;
        case(indexacao):
            tab(n);
            printf("LoadArray\n");
            n++;
            printExpressions(ie->data.iindex->variable,n);
            printExpressions(ie->data.iindex->index,n);
            n--;
            break;
        case(newarray):
            tab(n);
            if(ie->data.inew->type==integer)
                printf("NewInt\n");
            if(ie->data.inew->type==boolean)
                printf("NewBool\n");
            n++;
            printExpressions(ie->data.inew->size,n);
            n--;
            break;
        case(function):
            tab(n++);
            printf("Call\n");
            tab(n);
            printf("Id(%s)\n",ie->data.ife->id);
            is_expression_list* aux;
            for(aux=ie->data.ife->iel;aux!=NULL;aux=aux->next){
                printExpressions(aux->ie,n);
            }
            n--;
            break;
    }
}

void printStatements(is_statement_list* isl, int n){
    int auxfortab;
    for(;isl!=NULL;isl=isl->next){
        
        if(isl->st!= block)
            tab(n++);
        switch(isl->st){
            case(block):
                auxfortab=n;
                if(isl->data.ib->isl!=NULL && isl->data.ib->isl->next!=NULL){
                    tab(n++);
                    printf("CompoundStat\n");
                    printStatements(isl->data.ib->isl,n);
                }

                if(isl->data.ib->isl!=NULL && isl->data.ib->isl->next==NULL){
                    printStatements(isl->data.ib->isl,n);   
                }

               
                n=auxfortab;

                break;
            case(IF_):
                printf("IfElse\n");
                printExpressions(isl->data.ii->ie,n);
                if(isl->data.ii->isl->st==block && isl->data.ii->isl->data.ib->isl==NULL ){
                    tab(n);
                    printf("Null\n");     
                }
                else
                    printStatements(isl->data.ii->isl,n);
                tab(n);
                printf("Null\n");
                break;
            case(IFELSE_):
                printf("IfElse\n");
                printExpressions(isl->data.iie->ie,n);
                if(isl->data.iie->ie_true->st==block && isl->data.iie->ie_true->data.ib->isl==NULL ){
                    tab(n);
                    printf("Null\n");
                    
                }
                else
                    printStatements(isl->data.iie->ie_true,n);
                if(isl->data.iie->ie_false->st==block && isl->data.iie->ie_false->data.ib->isl==NULL ){
                    tab(n);
                    printf("Null\n");
                    
                }
                printStatements(isl->data.iie->ie_false,n);
                break;
            case(WHILE_):
                printf("While\n");
                printExpressions(isl->data.iw->ie,n);
                if(isl->data.iw->isl->st==block && isl->data.iw->isl->data.ib->isl==NULL ){
                    tab(n);
                    printf("Null\n");
                    
                }
                else
                    printStatements(isl->data.iw->isl,n);
                break;
            case(PRINT_):
                printf("Print\n");
                printExpressions(isl->data.ip->ie,n);
                break;
            case(RETURN_):
                printf("Return\n");
                if(isl->data.ir->ie->vt!=Void)
                    printExpressions(isl->data.ir->ie,n);
                break;
            case(ATTR_):
                if(isl->data.ia->index!=NULL){
                    printf("StoreArray\n");
                    tab(n);
                    printf("Id(%s)\n",isl->data.ia->id);
                    printExpressions(isl->data.ia->index,n);
                }
                else{
                    printf("Store\n");
                    tab(n);
                    printf("Id(%s)\n",isl->data.ia->id);
                }
                printExpressions(isl->data.ia->ie,n);
                
                break;
        }
        if(isl->st!= block)
            n--;
    }

}

void printMethodBody(is_var_decl* ivd, is_statement_list* isl, int n) {
    tab(n++);
    printf("MethodBody\n");

    for (; ivd != NULL; ivd = ivd->next) {
        tab(n++);
        printf("VarDecl\n");
        is_decl_list* aux = ivd->idl;
        tab(n);
        printf("%s\n",convertType(ivd->vt));
        for (; aux != NULL; aux = aux->next) {
            tab(n);
            printf("Id(%s)\n", aux->id);
        }
        n--;
    }

    printStatements(isl,n);
}

void printFormalParameters(is_formal_params* ifp, int n) {
    tab(n++);
    printf("MethodParams\n");


    for (; ifp != NULL; ifp = ifp->next) {
        tab(n++);
        printf("ParamDeclaration\n");
        tab(n);
        printf("%s\n", convertType(ifp->vt));
        tab(n);
        printf("Id(%s)\n", ifp->id);
        n--;
    }
}

char* convertType(var_type vt) {
    if (vt == integer)
        return "Int";
    if (vt == intArray)
        return "IntArray";
    if (vt == boolean)
        return "Bool";
    if (vt == boolArray)
        return "BoolArray";
    if (vt == stringArray)
        return "StringArray";
    if (vt == Void)
        return "Void";

}


char* convertType2(var_type vt) {
    if (vt == integer)
        return "int";
    if (vt == intArray)
        return "int[]";
    if (vt == boolean)
        return "boolean";
    if (vt == boolArray)
        return "boolean[]";
    if (vt == stringArray)
        return "String[]";
    if (vt==string_)
        return "String";
    if (vt == Void)
        return "void";

}
void tab(int n) {
    while (n--)
        printf("  ");
}


