#include "structures.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "y.tab.h"
#include "table.h"
#include "genCode.h"
int vars=8;
int level=1;

void generateStatements(is_statement_list* isl,tabela* m, tabela* g){
	tabela* var=NULL;
	is_statement_list* aux2;
	for(aux2=isl;aux2!=NULL;aux2=aux2->next){
		switch(aux2->st){
			case(PRINT_):
				generatePrintf(aux2->data.ip->ie, m ,g);
				level++;
				break;
			case(ATTR_):
				generateAttr(aux2->data.ia, m, g);
				level++;
				break;
			case(IF_):
				generateIf(aux2->data.ii,m,g);
				level++;
				break;
			case(IFELSE_):
				generateIfElse(aux2->data.iie,m,g);
				level++;
				break;
			case(block):
				generateStatements(aux2->data.ib->isl,m,g);
				level++;
				break;
			case(WHILE_):
				generateWhile(aux2->data.iw,m,g);
				level++;
				break;
			case(RETURN_):
				generateReturn(aux2->data.ir,m,g);
				level++;
				break;
		}
	}
}

void generateDeclaration(is_var_decl* ivd,tabela* m, tabela* g){
	tabela* ad;
	for (; ivd != NULL; ivd = ivd->next) {
        is_decl_list* aux = ivd->idl;
        for (; aux != NULL; aux = aux->next) {
    		ad=searchVar(m, g, aux->id);
    		ad->llvmExp=vars;
    		ad->tipo='%';
			if((ad->type==intArray || ad->type==boolArray))
				printf("%%var%d = alloca i32*, align 4\n",vars);
			else	
				printf("%%var%d = alloca i32, align 4\n",vars);

			vars++;
        }
    }
}

void generateCode(is_program* program, tabela * g){
	printf("@.str = private unnamed_addr constant [4 x i8] c\"%%d\\0A\\00\", align 1\n");
	printf("@.str1 = private unnamed_addr constant [6 x i8] c\"true\\0A\\00\", align 1\n");
	printf("@.str2 = private unnamed_addr constant [7 x i8] c\"false\\0A\\00\", align 1\n");
	printf("@arraysizehere = common global i32 0, align 4\n");

	printf("define void @__printf__boolean__(i32 %%a) #0 {\n");
	printf("%%1 = alloca i32, align 4\n");
	printf("store i32 %%a, i32* %%1, align 4\n");
	printf("%%2 = load i32* %%1, align 4\n");
	printf("%%3 = icmp ne i32 %%2, 0\n");
	printf("br i1 %%3, label %%4, label %%6\n");
	printf("; <label>:4                                       ; preds = %%0\n");
	printf("%%5 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([6 x i8]* @.str1, i32 0, i32 0))\n");
	printf("br label %%8\n");
	printf("; <label>:6                                       ; preds = %%0\n");
	printf("%%7 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([7 x i8]* @.str2, i32 0, i32 0))\n");
	printf("br label %%8\n");
	printf("; <label>:8                                       ; preds = %%6, %%4\n");
	printf("ret void\n}\n");



	tabela* ad;
	char c;
	is_method_and_global_vars* aux;
	is_formal_params* fp;
	for(aux=program->imagv;aux!=NULL;aux=aux->next){
		if(aux->dt==decl){
			is_decl_list* idl=aux->data.idl;
			for (; idl!= NULL; idl = idl->next) {
	    		ad=searchVar(NULL, g, idl->id);
	    		ad->llvmExp=vars;
	    		ad->tipo='@';
	    		if(ad->type==intArray || ad->type==boolArray)
					printf("@var%d = common global i32* null, align 4\n",vars);
				else
					printf("@var%d = common global i32 0, align 4\n",vars);
				vars++;
	        }
		}
	}

	for(aux=program->imagv;aux!=NULL;aux=aux->next){
		if(aux->dt==method){
			if( aux->data.imd->ifp!=NULL && aux->data.imd->ifp->vt==stringArray && strcmp(aux->data.imd->id,"main\0")==0){
				
				printf("define i32 @main(i32 %%argc, i8** %%argv) #0 {\n");
				printf("%%var1 = alloca i32, align 4\n");
				printf("%%var2 = alloca i8**, align 8\n");
				printf("store i32 %%argc, i32* %%var1, align 4\n");
				printf("store i8** %%argv, i8*** %%var2, align 8\n");
				printf("%%var3 = load i8*** %%var2, align 8\n");
				printf("%%var4 = getelementptr inbounds i8** %%var3, i64 1\n");
				printf("store i8** %%var4, i8*** %%var2, align 8\n");
				printf("%%var5 = load i8*** %%var2, align 8\n");
				printf("%%var6 = load i32* %%var1, align 8\n");
				printf("%%var7 = sub i32 %%var6, 1\n");
			}
			else{
				if(aux->vt==boolArray || aux->vt==intArray)
					printf("define i32* @%s(",aux->data.imd->id);
				else
					printf("define i32 @%s(",aux->data.imd->id);
				c=' ';
				for(fp=aux->data.imd->ifp;fp!=NULL;fp=fp->next){
					ad=searchVar(searchMethod(g, aux->data.imd->id)->in, NULL, fp->id);
					if(fp->vt==intArray || fp->vt==intArray)
						printf("%c i32* %%%s, i32 %%%ssize",c,ad->name,ad->name);
					else
						printf("%c i32 %%%s",c,ad->name);
					c=',';
				}
				printf("){\n");

				for(fp=aux->data.imd->ifp;fp!=NULL;fp=fp->next){
					ad=searchVar(searchMethod(g, aux->data.imd->id)->in, NULL, fp->id);
					ad->llvmExp=vars;
		    		ad->tipo='%';
		    		if((ad->type==intArray || ad->type==boolArray)){	
						printf("%%var%d = alloca i32*, align 4\n",vars);
						printf("store i32* %%%s, i32** %%var%d, align 4\n",ad->name,vars);
						ad->llvmExp=vars;
						vars++;
						printf("%%va_r%d = alloca i32, align 4\n",vars);
						printf("store i32 %%%ssize, i32* %%va_r%d, align 4\n",ad->name,vars);
						printf("%%var%d = load i32* %%va_r%d, align 4\n",vars, vars);
						ad->size=vars;
						ad->tipo='%';
						vars++;
		    		}
		    		else{	
						printf("%%var%d = alloca i32, align 4\n",vars);
						printf("store i32 %%%s, i32* %%var%d, align 4\n",ad->name,vars);
						ad->llvmExp=vars;
						vars++;
		    		}
		    		
				}

			}

			generateDeclaration(aux->data.imd->ivd,searchMethod(g, aux->data.imd->id)->in,g);
			generateStatements(aux->data.imd->isl,searchMethod(g, aux->data.imd->id)->in,g);

			if(aux->vt==integer || aux->vt==boolean || strcmp(aux->data.imd->id,"main\0")==0){
					printf("ret i32 0\n");
			}
			else if(aux->vt==intArray || aux->vt==boolArray){
				printf("ret i32* null\n");
			}else{
				printf("ret void\n");
			}
			printf("}\n");
		}

	}

	printf("declare i32 @printf(i8*, ...)\n");
	printf("declare i32 @atoi(i8*) #1\n");
	printf("declare noalias i8* @calloc(i64, i64) #4\n");
	printf("attributes #0 = { nounwind uwtable \"less-precise-fpmad\"=\"false\" \"no-frame-pointer-elim\"=\"true\" \"no-frame-pointer-elim-non-leaf\" \"no-infs-fp-math\"=\"false\" \"no-nans-fp-math\"=\"false\" \"stack-protector-buffer-size\"=\"8\" \"unsafe-fp-math\"=\"false\" \"use-soft-float\"=\"false\" }\n");
	printf("attributes #1 = { \"less-precise-fpmad\"=\"false\" \"no-frame-pointer-elim\"=\"true\" \"no-frame-pointer-elim-non-leaf\" \"no-infs-fp-math\"=\"false\" \"no-nans-fp-math\"=\"false\" \"stack-protector-buffer-size\"=\"8\" \"unsafe-fp-math\"=\"false\" \"use-soft-float\"=\"false\" }\n");
	printf("attributes #2 = { \"less-precise-fpmad\"=\"false\" \"no-frame-pointer-elim\"=\"true\" \"no-frame-pointer-elim-non-leaf\" \"no-infs-fp-math\"=\"false\" \"no-nans-fp-math\"=\"false\" \"stack-protector-buffer-size\"=\"8\" \"unsafe-fp-math\"=\"false\" \"use-soft-float\"=\"false\" }\n");
	printf("attributes #3 = { nounwind readonly }\n");
	printf("attributes #4 = { nounwind \"less-precise-fpmad\"=\"false\" \"no-frame-pointer-elim\"=\"true\" \"no-frame-pointer-elim-non-leaf\" \"no-infs-fp-math\"=\"false\" \"no-nans-fp-math\"=\"false\" \"stack-protector-buffer-size\"=\"8\" \"unsafe-fp-math\"=\"false\" \"use-soft-float\"=\"false\" }\n");
}

void generateReturn(is_return* ir, tabela* t, tabela* g){
	generateExpression(ir->ie, t,g);
	if(ir->ie->vt==intArray || ir->ie->vt==boolArray){
		if(ir->ie->ot==id){
			printf("store i32 %%var%d, i32* @arraysizehere\n",searchVar(t,g,ir->ie->data.iid->id)->size);
			vars++;
		}
		if(ir->ie->ot==newarray){
			printf("store i32 %%var%d, i32* @arraysizehere\n",ir->ie->data.inew->size->llvmExp);
			vars++;
		}
		printf("ret i32* %%var%d\n",ir->ie->llvmExp);
	}
	else
		if(ir->ie->llvmExp==0)
			printf("ret i32 0\n");
		else
			printf("ret i32 %%var%d\n",ir->ie->llvmExp);
}

void generateWhile(is_while* iw, tabela* t, tabela* g){
	
	int nivelLocal=vars;
	printf("br label %%whileinicio.%d\n", nivelLocal);
	printf("whileinicio.%d:\n",nivelLocal);
	generateExpression(iw->ie, t,g);
	printf("%%whileresultP1.%d = icmp eq i32 %cvar%d, 0\n", nivelLocal, iw->ie->tipo, iw->ie->llvmExp);
	printf("br i1 %%whileresultP1.%d, label %%whileend.%d, label %%whilethen.%d\n", nivelLocal, nivelLocal, nivelLocal);
	printf("\nwhilethen.%d:\n",nivelLocal);
	vars++;
	generateStatements(iw->isl,t,g);

	printf("br label %%whileinicio.%d\n", nivelLocal);
	printf("\nwhileend.%d:\n",nivelLocal);
}

void generatePrintf(is_expression* ie, tabela* t, tabela* g){
	generateExpression(ie,t ,g);
	if(ie->vt==integer){
		printf("%%var%d=call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([4 x i8]* @.str, i32 0, i32 0), i32 %cvar%d)\n",vars,ie->tipo,ie->llvmExp);
		vars++;
	}
	else{
		printf("call void @__printf__boolean__(i32 %cvar%d)\n",ie->tipo,ie->llvmExp);
	}
}

void generateIf(is_if* ii, tabela* t, tabela* g){
	generateExpression(ii->ie, t,g);
	int nivelLocal=vars;
	printf("%%ifresultP1.%d = icmp eq i32 %cvar%d, 0\n", nivelLocal, ii->ie->tipo, ii->ie->llvmExp);
	printf("br i1 %%ifresultP1.%d, label %%ifend.%d, label %%ifthen.%d\n", nivelLocal, nivelLocal, nivelLocal);
	printf("\nifthen.%d:\n",nivelLocal);
	vars++;
	generateStatements(ii->isl,t,g);

	printf("br label %%ifend.%d\n", nivelLocal);
	printf("\nifend.%d:\n",nivelLocal);
}

void generateIfElse(is_ifelse* iie, tabela* t, tabela* g){
	generateExpression(iie->ie, t,g);
	int nivelLocal=vars;
	printf("%%ifresultelseP1.%d = icmp eq i32 %cvar%d, 0\n", nivelLocal, iie->ie->tipo, iie->ie->llvmExp);
	printf("br i1 %%ifresultelseP1.%d, label %%ifelseelse.%d, label %%ifelsethen.%d\n", nivelLocal, nivelLocal, nivelLocal);
	printf("\nifelsethen.%d:\n",nivelLocal);
	vars++;
	generateStatements(iie->ie_true,t,g);
	printf("br label %%ifelseend.%d\n", nivelLocal);
	printf("\nifelseelse.%d:\n",nivelLocal);
	generateStatements(iie->ie_false,t,g);
	printf("br label %%ifelseend.%d\n", nivelLocal);
	printf("\nifelseend.%d:\n",nivelLocal);
}

int loadVariable(tabela* t){
	printf("%%var%d = load i32** %cvar%d, align 4\n",vars,t->tipo,t->llvmExp);
	vars++;
	return vars-1;
}

void generateAttr(is_attr* ia, tabela* t, tabela* g){
	generateExpression(ia->ie,t,g);
	tabela* ad=searchVar(t, g, ia->id);

	if(ia->index==NULL){	
		if(ad->type==intArray || ad->type==boolArray){
			printf("store i32* %%var%d, i32** %cvar%d, align 4\n",ia->ie->llvmExp,ad->tipo,ad->llvmExp);
			if(ia->ie->ot==newarray){
				
				ad->size=ia->ie->data.inew->size->llvmExp;
				
			}
			if(ia->ie->ot==id){
				ad->size=searchVar(t,g,ia->ie->data.iid->id)->size;
			}
			if(ia->ie->ot==function){

				printf("%%var%d= load i32* @arraysizehere, align 4\n",vars);
				ad->size=vars;
				vars++;
			}
		}
		else{
			printf("store i32 %%var%d, i32* %cvar%d, align 4\n",ia->ie->llvmExp,ad->tipo,ad->llvmExp);
		}
	}
	else{
		generateExpression(ia->index,t,g);
		printf("%%longo%d = sext i32 %%var%d to i64\n",vars,ia->index->llvmExp);
  		printf("%%ind%d = getelementptr inbounds i32* %%var%d, i64 %%longo%d\n",vars,loadVariable(ad),vars);
		printf("store i32 %%var%d, i32* %%ind%d, align 4\n",ia->ie->llvmExp,vars);
	}
}


int generateExpression(is_expression* ie, tabela* t, tabela* g){
	int a=0;
	int nivelLocal=0;
	int b=0;
	char c;
	tabela* ad;
	is_expression_list* aux;
	switch(ie->ot){
		case(plus):
		case(minus):
		case(mult):
		case(divide):
		case(leftover):
		case(diff):
		case(equals):
		case(let):
		case(get):
		case(gt):
		case(lt):
			generateExpression(ie->data.i2e->p1, t, g);
			generateExpression(ie->data.i2e->p2, t, g);
			break;

	}
	switch(ie->ot){
		case(plus):
			printf("%%var%d = add i32 %cvar%d, %cvar%d\n",vars, ie->data.i2e->p1->tipo, ie->data.i2e->p1->llvmExp, ie->data.i2e->p2->tipo,ie->data.i2e->p2->llvmExp);
			break;
		case(minus):
			printf("%%var%d = sub i32 %cvar%d, %cvar%d\n",vars, ie->data.i2e->p1->tipo, ie->data.i2e->p1->llvmExp, ie->data.i2e->p2->tipo,ie->data.i2e->p2->llvmExp);
			break;
		case(mult):
			printf("%%var%d = mul i32 %cvar%d, %cvar%d\n",vars, ie->data.i2e->p1->tipo, ie->data.i2e->p1->llvmExp, ie->data.i2e->p2->tipo,ie->data.i2e->p2->llvmExp);
			break;
		case(divide):
			printf("%%var%d = sdiv i32 %cvar%d, %cvar%d\n",vars, ie->data.i2e->p1->tipo, ie->data.i2e->p1->llvmExp, ie->data.i2e->p2->tipo,ie->data.i2e->p2->llvmExp);
			break;
		case(leftover):
			printf("%%var%d = srem i32 %cvar%d, %cvar%d\n",vars, ie->data.i2e->p1->tipo, ie->data.i2e->p1->llvmExp, ie->data.i2e->p2->tipo,ie->data.i2e->p2->llvmExp);
			break;
		case(diff):
			printf("%%var%d = icmp ne i32 %cvar%d, %cvar%d\n",vars, ie->data.i2e->p1->tipo, ie->data.i2e->p1->llvmExp, ie->data.i2e->p2->tipo,ie->data.i2e->p2->llvmExp);
			vars++;
  			printf("%%var%d = zext i1 %%var%d to i32\n", vars, vars-1);
			break;
		case(equals):
			printf("%%var%d = icmp eq i32 %cvar%d, %cvar%d\n",vars, ie->data.i2e->p1->tipo, ie->data.i2e->p1->llvmExp, ie->data.i2e->p2->tipo,ie->data.i2e->p2->llvmExp);
			vars++;
  			printf("%%var%d = zext i1 %%var%d to i32\n", vars, vars-1);
			break;
		case(let):
		  	printf("%%var%d = icmp sle i32 %cvar%d, %cvar%d\n", vars, ie->data.i2e->p1->tipo, ie->data.i2e->p1->llvmExp,ie->data.i2e->p2->tipo, ie->data.i2e->p2->llvmExp);
		  	vars++;
  			printf("%%var%d = zext i1 %%var%d to i32\n", vars, vars-1);
  			break;
		case(get):
			printf("%%var%d = icmp sge i32 %cvar%d, %cvar%d\n", vars, ie->data.i2e->p1->tipo, ie->data.i2e->p1->llvmExp,ie->data.i2e->p2->tipo, ie->data.i2e->p2->llvmExp);
		  	vars++;
  			printf("%%var%d = zext i1 %%var%d to i32\n", vars, vars-1);
  			break;
		case(gt):
			printf("%%var%d = icmp sgt i32 %cvar%d, %cvar%d\n", vars, ie->data.i2e->p1->tipo, ie->data.i2e->p1->llvmExp,ie->data.i2e->p2->tipo, ie->data.i2e->p2->llvmExp);
		  	vars++;
  			printf("%%var%d = zext i1 %%var%d to i32\n", vars, vars-1);
  			break;
		case(lt):
			printf("%%var%d = icmp slt i32 %cvar%d, %cvar%d\n", vars, ie->data.i2e->p1->tipo, ie->data.i2e->p1->llvmExp,ie->data.i2e->p2->tipo, ie->data.i2e->p2->llvmExp);
		  	vars++;
  			printf("%%var%d = zext i1 %%var%d to i32\n", vars, vars-1);
  			break;
  		case(dotlength):
  			if(ie->data.i1e->p1->ot==id){
  				if(searchVar(t,g,ie->data.i1e->p1->data.iid->id)->type==stringArray){
  					ie->llvmExp=7;
  					ie->tipo='%';
				}
				else{
					ie->llvmExp=searchVar(t,g,ie->data.i1e->p1->data.iid->id)->size;
					ie->tipo='%';
					vars++;
					return 1;
				}
			}
  			else{
  				if(ie->data.i1e->p1->ot==newarray){
	  				generateExpression(ie->data.i1e->p1->data.inew->size, t, g);
	  				ie->llvmExp=ie->data.i1e->p1->data.inew->size->llvmExp;
	  				ie->tipo=ie->data.i1e->p1->data.inew->size->tipo;
	  			}
	  			else{
	  				generateExpression(ie->data.i1e->p1, t, g);
	  				printf("%%var%d= load i32* @arraysizehere, align 4\n",vars);
	  				ie->llvmExp=vars;
	  				ie->tipo='%';
	  				vars++;
	  			}
  				return;
  			}
  			break;
  		case(newarray):
  			generateExpression(ie->data.inew->size, t, g);
  			printf("%%bytesmalloc64.%d = sext i32 %%var%d to i64\n",vars,ie->data.inew->size->llvmExp);
  			printf("%%malocreturn%d = call noalias i8* @calloc(i64 %%bytesmalloc64.%d, i64 32) #2\n",vars,vars);
			printf("%%var%d = bitcast i8* %%malocreturn%d to i32*\n", vars, vars);
  		break;
  		case(indexacao):
  			generateExpression(ie->data.iindex->index, t,g);
  			generateExpression(ie->data.iindex->variable, t,g);
  			printf("%%index%d = sext i32 %%var%d to i64\n",vars, ie->data.iindex->index->llvmExp);
  			printf("%%ptr%d = getelementptr inbounds i32* %%var%d, i64 %%index%d\n",vars,ie->data.iindex->variable->llvmExp,vars);
  			printf("%%var%d = load i32* %%ptr%d, align 4\n",vars,vars);
		break;
  		case(id):
			ad=searchVar(t,g,ie->data.iid->id);
			if(ad->type==intArray || ad->type==boolArray){
				printf("%%var%d = load i32** %cvar%d, align 4\n",vars, ad->tipo, ad->llvmExp);	
				ie->llvmExp=vars;
				ie->tipo='%';
				vars++;
				return 1;
			}
			else{
				printf("%%var%d = load i32* %cvar%d, align 4\n",vars, ad->tipo, ad->llvmExp);
			}			
			break;
		case(and_):
			nivelLocal=level;
			level++;
			generateExpression(ie->data.i2e->p1, t, g);
			printf("%%resultP1.%d = icmp ne i32 %cvar%d, 0\n", nivelLocal, ie->data.i2e->p1->tipo, ie->data.i2e->p1->llvmExp);
			printf("br i1 %%resultP1.%d, label %%then.%d, label %%else.%d\n", nivelLocal, nivelLocal, nivelLocal);
			printf("\nthen.%d:\n",nivelLocal);
			level++;
			generateExpression(ie->data.i2e->p2, t, g);
			printf("%%resultP2.%d = icmp ne i32 %cvar%d, 0\n",nivelLocal, ie->data.i2e->p2->tipo, ie->data.i2e->p2->llvmExp);
			printf("br label %%end.%d\n", nivelLocal);
			printf("\nend.%d:\n", nivelLocal);
			printf("br label %%cont.%d\n", nivelLocal);
			printf("\nelse.%d:\n", nivelLocal);
			printf("br label %%cont.%d\n", nivelLocal);
			printf("\ncont.%d:\n",nivelLocal);
			printf("%%resultExp.%d = phi i1 [ false, %%else.%d ], [ %%resultP2.%d, %%end.%d ]\n",nivelLocal,nivelLocal,nivelLocal,nivelLocal);
			printf("%%var%d = zext i1 %%resultExp.%d to i32\n",vars, nivelLocal);

			break;
		case(or_):
			nivelLocal=level;
			level++;
			generateExpression(ie->data.i2e->p1, t, g);
			printf("%%resultP1.%d = icmp ne i32 %cvar%d, 0\n", nivelLocal,  ie->data.i2e->p1->tipo, ie->data.i2e->p1->llvmExp);
			printf("br i1 %%resultP1.%d, label %%else.%d, label %%then.%d\n", nivelLocal, nivelLocal, nivelLocal);
			printf("\nthen.%d:\n",nivelLocal);
			level++;
			generateExpression(ie->data.i2e->p2, t, g);
			printf("%%resultP2.%d = icmp ne i32 %cvar%d, 0\n",nivelLocal,  ie->data.i2e->p2->tipo, ie->data.i2e->p2->llvmExp);
			printf("br label %%end.%d\n", nivelLocal);
			printf("\nend.%d:\n", nivelLocal);
			printf("br label %%cont.%d\n", nivelLocal);
			printf("\nelse.%d:\n", nivelLocal);
			printf("br label %%cont.%d\n", nivelLocal);
			printf("\ncont.%d:\n",nivelLocal);
			printf("%%resultExp.%d = phi i1 [ true, %%else.%d ], [ %%resultP2.%d, %%end.%d ]\n",nivelLocal,nivelLocal,nivelLocal,nivelLocal);
			printf("%%var%d = zext i1 %%resultExp.%d to i32\n",vars, nivelLocal);

			break;
		case(not_):
			generateExpression(ie->data.i1e->p1, t, g);
			printf("%%var%d = icmp ne i32 %cvar%d, 0\n", vars,  ie->data.i1e->p1->tipo, ie->data.i1e->p1->llvmExp);
  			vars++;
  			printf("%%var%d = xor i1 %%var%d, true\n",vars,vars-1);
  			vars++;
  			printf("%%var%d = zext i1 %%var%d to i32\n", vars, vars-1);
  			break;

		case(uplus):
			generateExpression(ie->data.i1e->p1, t, g);
			printf("%%var%d = add i32 %cvar%d, 0\n",vars, ie->data.i1e->p1->tipo, ie->data.i1e->p1->llvmExp);
			break;

		case(uminus):
			generateExpression(ie->data.i1e->p1, t, g);
			printf("%%var%d = sub i32 0, %cvar%d\n",vars, ie->data.i1e->p1->tipo, ie->data.i1e->p1->llvmExp);
			break;

		case(boolit):
			printf("%%var%d = alloca i32, align 4\n",vars);
			printf("store i32 %d, i32* %%var%d, align 4\n", strcmp("true\0",ie->data.ibe->p1)?0:1,vars);
			printf("%%var%d = load i32* %%var%d, align 4\n",vars+1,vars);
			vars++;

			break;
		
		case(intlit):
			printf("%%var%d = alloca i32, align 4\n",vars);
			printf("store i32 %d, i32* %%var%d, align 4\n",ie->data.iie->p, vars);
			printf("%%var%d = load i32* %%var%d, align 4\n",vars+1,vars);
			vars++;
			break;
		case(parseint):
			generateExpression(ie->data.ipe->ie,t,g);
			printf("%%var%d = sext i32 %cvar%d to i64\n",vars, ie->data.ipe->ie->tipo, ie->data.ipe->ie->llvmExp);
			printf("%%var%d = getelementptr inbounds i8** %%var5, i64 %%var%d\n",vars+1,vars);
			printf("%%var%d = load i8** %%var%d, align 8\n",vars+2,vars+1);
			printf("%%var%d = call i32 @atoi(i8* %%var%d) #2\n",vars+3,vars+2);
			vars+=3;
			break;
		case(function):
			aux=ie->data.ife->iel;
			for(;aux!=NULL;aux=aux->next){
				generateExpression(aux->ie,t,g);
			}
			
			aux=ie->data.ife->iel;
			for(;aux!=NULL;aux=aux->next){
				if(aux->ie->vt==boolArray || aux->ie->vt== intArray){		
	  				if(aux->ie->ot==newarray){
		  				generateExpression(aux->ie->data.inew->size, t, g);
		
		  			}
		  			else if(aux->ie->ot==function){
		  				generateExpression(aux->ie, t, g);
		  			}
				}
			}

			aux=ie->data.ife->iel;

			ad=searchMethod(g, ie->data.ife->id);
			aux=ie->data.ife->iel;

			if(ad->type==Void)
				printf("%%var%d = call void @%s(",vars,ie->data.ife->id);
			else{
				if(ad->type==intArray || ad->type==boolArray){
					printf("%%var%d = call i32* @%s(",vars,ie->data.ife->id);
				}
				else{
					printf("%%var%d = call i32 @%s(",vars,ie->data.ife->id);
				}
			}
			c=' ';
			ie->llvmExp=vars;
			ie->tipo='%';
			vars++;			


			for(;aux!=NULL;aux=aux->next){
				if(aux->ie->vt==boolArray || aux->ie->vt== intArray){		
		 			if(aux->ie->ot==id){
							printf("%c i32* %%var%d, i32 %%var%d ", c, aux->ie->llvmExp, searchVar(t,g,aux->ie->data.iid->id)->size);
					}
		  			else{
		  				if(aux->ie->ot==newarray){
			  				printf("%c i32* %%var%d, i32 %%var%d ", c, aux->ie->llvmExp, aux->ie->data.inew->size->llvmExp);
			  			}
			  			else{
			  				printf("%%var%d= load i32* @arraysizehere, align 4\n",vars);
			  				printf("%c i32* %%var%d, i32 %%var%d ", c, aux->ie->llvmExp, vars);
			  			}

		  			}
				}
				else
					printf("%c i32 %%var%d", c, aux->ie->llvmExp);
				c=',';
			}
			printf(")\n");
			if(ad->type==intArray || ad->type==boolArray){	
				printf("%%var%d=load i32* @arraysizehere\n", vars);
				ad->size=vars;
				vars++;
			}

			break;
	}
	switch(ie->ot){
		case(plus):
		case(minus):
		case(mult):
		case(divide):
		case(leftover):
		case(let):
		case(get):
		case(gt):
		case(lt):
		case(intlit):
		case(id):
		case(boolit):
		case(not_):
		case(uminus):
		case(uplus):
		case(parseint):
		case(and_):
		case(or_):
		case(equals):
		case(indexacao):
		case(diff):
		case(newarray):
			ie->llvmExp=vars;
			ie->tipo='%';
			vars++;
		break;
	}
	return 0;
}


