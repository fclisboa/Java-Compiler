
#include "structures.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "y.tab.h"
#include "table.h"
#include "show.h"


classe* createClasse(char* id){
	classe* c=(classe*)malloc(sizeof(classe));
	c->name=id;
	c->fm=NULL;
	return c;
}

tabela* createParams( is_decl_list* idl, var_type vt,decl_type d, tabela* t){
	is_decl_list* aux=NULL;
	tabela* newField=NULL;
	for(aux=idl;aux!=NULL;aux=aux->next){
		newField=(tabela*)malloc(sizeof(tabela));
		
		newField->name=aux->id;
		newField->type=vt;
		newField->dt=d;
		
		if(searchParam(t, newField->name)==NULL)
			t = insertParams(newField, t);
		else{
			printf("Symbol %s already defined\n",newField->name);
			exit(0);
		}
	}
	return t;
}

tabela* createFormalParams( is_formal_params* ifp, tabela* t){
	is_formal_params* aux=NULL;
	tabela* newField=NULL;
	for(aux=ifp;aux!=NULL;aux=aux->next){
		newField=(tabela*)malloc(sizeof(tabela));

		newField->name=aux->id;
		newField->type=aux->vt;
		newField->dt=formalParam;

		if(searchParam(t, newField->name)==NULL)
			t = insertParams(newField, t);
		else{
			printf("Symbol %s already defined\n",newField->name);
			exit(0);
		}
		
	}
	return t;
}


tabela* insertParams(tabela* new_, tabela* t){
	if(t!=NULL){

		tabela* aux=t;
		for(;aux->next!=NULL;aux=aux->next);
		aux->next=new_;
		return t;
	}
	else
		return new_;
}

tabela* createMethod(is_method_and_global_vars* met, tabela* t){
	tabela* m=(tabela*)malloc(sizeof(tabela));
	m->name=met->data.imd->id;
	m->type=met->vt;
	m->dt=method;

	if(searchParam(t, m->name)==NULL)
		t = insertParams(m, t);
	else{
		printf("Symbol %s already defined\n",m->name);
		exit(0);
	}

	if(met->data.imd->ifp!=NULL)
		m->in=createFormalParams(met->data.imd->ifp,m->in);

	is_var_decl* aux;
	for(aux=met->data.imd->ivd;aux!=NULL;aux=aux->next)
		m->in=createParams(aux->idl, aux->vt, var, m->in);

	return t;
}

tabela* searchParam(tabela* t, char* nome){

	for(;t!=NULL;t=t->next){
		if(!strcmp(t->name,nome))
			return t;
	}
	return NULL;
}

classe* buildTable(is_program* program){
	classe* tabela=NULL;
	if(program->id!=NULL){
		tabela=createClasse(program->id);
		is_method_and_global_vars* aux;
		for(aux=program->imagv;aux!=NULL;aux=aux->next){
			if(aux->dt==decl)
				tabela->fm=createParams(aux->data.idl,aux->vt,decl, tabela->fm);
			else
				tabela->fm=createMethod(aux, tabela->fm);
		}
	}
	return tabela;
}

var_type checkExpressions(is_expression* ie, tabela* t, tabela* g){
	tabela* tabela_=NULL;
	is_expression_list* aux=NULL;
	int err=-1;
	int n=0;
	var_type temp1;
	var_type temp2;
	int i;
	if(ie->vt==nd){
		switch(ie->ot){
			case(plus):
			case(minus):
			case(divide):
			case(mult):
			case(leftover):
				temp1=checkExpressions(ie->data.i2e->p1, t,g);
				temp2=checkExpressions(ie->data.i2e->p2, t,g);

				if(!(temp1==integer && temp2==integer)){
					printf("Operator %s cannot be applied to types %s, %s\n",printOperator(ie), convertType2(ie->data.i2e->p1->vt), convertType2(ie->data.i2e->p2->vt));
					exit(0);
				}
				ie->vt=integer;
				
				
			
			break;


			case(diff):
			case(equals):
				temp1=checkExpressions(ie->data.i2e->p1, t,g);
				temp2=checkExpressions(ie->data.i2e->p2, t,g);
				if(!((temp1==temp2) && (temp1==integer || temp1==boolean || temp1==intArray || temp1==boolArray || temp1==stringArray))){
						printf("Operator %s cannot be applied to types %s, %s\n",printOperator(ie), convertType2(ie->data.i2e->p1->vt), convertType2(ie->data.i2e->p2->vt));
						exit(0);
				}
				ie->vt=boolean;
			break;

			case(let):
			case(get):
			case(gt):
			case(lt):
				temp1=checkExpressions(ie->data.i2e->p1, t,g);
				temp2=checkExpressions(ie->data.i2e->p2, t,g);
				if(!(temp1==integer && temp2==integer)){
					printf("Operator %s cannot be applied to types %s, %s\n",printOperator(ie), convertType2(ie->data.i2e->p1->vt), convertType2(ie->data.i2e->p2->vt));
					exit(0);
				}
				ie->vt=boolean;
			break;

			case(and_):
			case(or_):
				temp1=checkExpressions(ie->data.i2e->p1, t,g);
				temp2=checkExpressions(ie->data.i2e->p2, t,g);

				if(!(temp1==boolean && temp2==boolean)){
					printf("Operator %s cannot be applied to types %s, %s\n",printOperator(ie), convertType2(ie->data.i2e->p1->vt), convertType2(ie->data.i2e->p2->vt));
					exit(0);
				}
				ie->vt=boolean;
			break;

			case(not_):
				if(checkExpressions(ie->data.i1e->p1,t,g)!=boolean){
					printf("Operator %s cannot be applied to type %s\n",printOperator(ie), convertType2(ie->data.i1e->p1->vt));
					exit(0);
				}
				ie->vt=boolean;
			break;
			case(uplus):
			case(uminus):
				if(checkExpressions(ie->data.i1e->p1, t,g)!=integer){
					printf("Operator %s cannot be applied to type %s\n",printOperator(ie), convertType2(ie->data.i1e->p1->vt));
					exit(0);
				}
				ie->vt=integer;
			break;

			case(intlit):
				if(strlen(ie->data.iie->p1) > 1 && ie->data.iie->p1[0]=='0' && (ie->data.iie->p1[1]!='X' && ie->data.iie->p1[1]!='x')){
					for(i=0;i<strlen(ie->data.iie->p1);i++){
						if(ie->data.iie->p1[i]=='9' || ie->data.iie->p1[i]=='8'){
							printf("Invalid literal %s\n",ie->data.iie->p1);
							exit(0);
						}
					}
				}
				ie->data.iie->p=(int)strtol(ie->data.iie->p1,NULL,0);
				
				ie->vt=integer;
			break;

			case(boolit):
				ie->vt=boolean;
			break;

			case(dotlength):
				temp1=checkExpressions(ie->data.i1e->p1, t,g);
				if(!(temp1==stringArray || temp1==intArray || temp1==boolArray)){
					printf("Operator %s cannot be applied to type %s\n",printOperator(ie), convertType2(ie->data.i1e->p1->vt));
					exit(0);
				}
				ie->vt=integer;
			break;

			case(id):
				tabela_=searchVar(t,g,ie->data.iid->id);
				if(tabela_==NULL){
					printf("Cannot find symbol %s\n",ie->data.iid->id);
					exit(0);
				}
				ie->vt=tabela_->type;
				
			break;

			case(indexacao):
				temp1=checkExpressions(ie->data.iindex->index,t,g);
				temp2=checkExpressions(ie->data.iindex->variable,t,g);
				if(!((temp2==intArray || temp2==boolArray) && temp1==integer)){
					printf("Operator %s cannot be applied to types %s, %s\n",printOperator(ie), convertType2(ie->data.iindex->variable->vt), convertType2(ie->data.iindex->index->vt));
					exit(0);
				}
				ie->vt=ie->data.iindex->variable->vt-1;
			break;

			case(newarray):
				if(checkExpressions(ie->data.inew->size, t,g)!=integer){
					printf("Operator %s cannot be applied to type %s\n",printOperator(ie), convertType2(ie->data.inew->size->vt));
					exit(0);
				}
				ie->vt=ie->data.inew->type+1;
			break;

			case(parseint):
				tabela_=searchVar(t,g,ie->data.ipe->id);
				if(tabela_==NULL){
					printf("Cannot find symbol %s\n",ie->data.iid->id);
					exit(0);
				}
				if(checkExpressions(ie->data.ipe->ie,t,g)!=integer || tabela_->type!=stringArray){
					printf("Operator %s cannot be applied to types %s, %s\n",printOperator(ie), convertType2(tabela_->type),convertType2(ie->data.ipe->ie->vt));
					exit(0);
				}
				ie->vt=integer;
			break;

			case(function):
				
				tabela_=searchMethod(g, ie->data.ife->id);
				if(tabela_==NULL){
					printf("Cannot find symbol %s\n",ie->data.ife->id);
					exit(0);
				}
				aux=ie->data.ife->iel;
				n=0;
				for(;aux!=NULL;aux=aux->next){
					if(checkExpressions(aux->ie,t,g)!=methodParam(tabela_,n++)){
						err=n-1;
						break;
					}
				}
				if(err!=-1){
					printf("Incompatible type of argument %d in call to method %s (got %s, required %s)\n",err,ie->data.ife->id,convertType2(aux->ie->vt),convertType2(methodParam(tabela_,err)));
					exit(0);
				}
				if(methodParam(tabela_,n)!=Void){
					printf("Incompatible type of argument %d in call to method %s (got void, required %s)\n",n,ie->data.ife->id,convertType2(methodParam(tabela_,n)));
					exit(0);
				}
				ie->vt=tabela_->type;		

			break;
		}
	}
	return ie->vt;
}

var_type methodParam(tabela* m, int n){
	tabela* aux;

	for(aux=m->in;aux!=NULL;aux=aux->next){
		if(aux->dt==formalParam){
			if(n==0)
				return aux->type;
			n--;
		}
	}
	return Void;
}

tabela* searchMethod(tabela* g, char* id){
	for(;g!=NULL;g=g->next){
		if(!strcmp(g->name,id) && g->dt==method)
			return g;
	}
	return NULL;
}

tabela* searchVar(tabela* t, tabela* g, char* nome){

	for(;t!=NULL;t=t->next){
		if(!strcmp(t->name,nome))
			return t;
	}

	for(;g!=NULL;g=g->next){
		if(!strcmp(g->name,nome))
			return g;
	}
	return NULL;
}

checkTypes(is_program* program, tabela * g){
	is_method_and_global_vars* aux;
	for(aux=program->imagv;aux!=NULL;aux=aux->next){
		if(aux->dt==method){
			testISL(aux->data.imd->isl,searchMethod(g, aux->data.imd->id),g);
		}
	}
	return 1;
}

int testISL(is_statement_list* isl,tabela* m, tabela* g){
	tabela* var=NULL;
	var_type temp1;
	is_statement_list* aux2;
	for(aux2=isl;aux2!=NULL;aux2=aux2->next){
		switch(aux2->st){
			case(IF_):
				if(checkExpressions(aux2->data.ii->ie, m->in,g)!=boolean){
					printf("Incompatible type in if statement (got %s, required boolean)\n", convertType2(aux2->data.ii->ie->vt));
					exit(0);
				}
				else{
					testISL(aux2->data.ii->isl,m,g);
				}
			break;

			case(IFELSE_):
				if(checkExpressions(aux2->data.iie->ie, m->in,g)!=boolean){
					printf("Incompatible type in if statement (got %s, required boolean)\n", convertType2(aux2->data.iie->ie->vt));
					exit(0);
				}
				else{
					testISL(aux2->data.iie->ie_true,m,g);
					testISL(aux2->data.iie->ie_false,m,g);
				}
			break;

			case(WHILE_):
				if(checkExpressions(aux2->data.iw->ie, m->in,g)!=boolean){
					printf("Incompatible type in while statement (got %s, required boolean)\n", convertType2(aux2->data.iw->ie->vt));
					exit(0);
				}
				else{
					testISL(aux2->data.iw->isl,m,g);
				}
			break;

			case(PRINT_):
				temp1=checkExpressions(aux2->data.ip->ie,m->in,g);
				if(temp1!=boolean && temp1!=integer){
					printf("Incompatible type in System.out.println statement (got %s, required boolean or int)\n", convertType2(aux2->data.ip->ie->vt));
					exit(0);
				}
			break;
			
			case(RETURN_):
				if(checkExpressions(aux2->data.ir->ie, m->in,g)!=m->type){
					printf("Incompatible type in return statement (got %s, required %s)\n", convertType2(aux2->data.ir->ie->vt),convertType2(m->type));
					exit(0);
				}
			break;

			case(ATTR_):
				
				var=searchVar(m->in,g,aux2->data.ia->id);
				if(var==NULL){
					printf("Cannot find symbol %s\n",aux2->data.ia->id);
					exit(0);
				}

				if(aux2->data.ia->index!=NULL && (checkExpressions(aux2->data.ia->index, m->in,g)!=integer || (var->type!=intArray && var->type!=boolArray))){
					printf("Operator [ cannot be applied to types %s, %s\n", convertType2(var->type),convertType2(aux2->data.ia->index->vt));
					exit(0);
				}
				if(checkExpressions(aux2->data.ia->ie, m->in,g)!=var->type-(aux2->data.ia->index!=NULL?1:0)){
					printf("Incompatible type in assignment to %s%s (got %s, required %s)\n", aux2->data.ia->id,aux2->data.ia->index!=NULL?"[]":"",convertType2(aux2->data.ia->ie->vt),convertType2(searchVar(m->in,g,aux2->data.ia->id)->type-(aux2->data.ia->index!=NULL?1:0)));
					exit(0);
				}
			
			break;
			case(block):
				testISL(aux2->data.ib->isl,m, g);
			break;
		}
	}
}


char* printOperator(is_expression* ie){
	switch(ie->ot){
		case(plus):
		return "+";
		break;
		case(minus):
		return "-"; 
		break;
		case(divide):
		return "/";
		break;
		case(mult):
		return "*";
		break;
		case(leftover):
		return "\%";
		break;
		case(diff):
		return "!=";
		break;
		case(equals):
		return "==";
		break;
		case(let):
		return "<=";
		break;
		case(get):
		return ">=";
		break;
		case(gt):
		return ">";
		break;
		case(lt):
		return "<";
		break;
		case(and_):
		return "&&";
		break;
		case(or_):
		return "||";
		break;
		case(not_):
		return "!";
		break;
		case(uplus):
		return "+";
		break;
		case(uminus):
		return "-";
		break;
		case(dotlength):
		return ".length";
		break;
		case(indexacao):
		return "[";
		break;
		case(newarray):
			if(ie->data.inew->type==integer)
				return "new int";
			else
				return "new boolean";
		break;

		case(parseint):
		return "Integer.parseInt";
		break;
	}
}


