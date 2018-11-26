# Simple-Java-Compiler

This compiler was built using Yacc/Bison. It takes as input a Java program and is able to describe its syntatic tree and identify potential errors. You need Yacc installed to run this project.

```python
flex ijparser.l
bison -d -o y.tab.c ijparser.y
gcc y.tab.c lex.yy.c table.c show.c functions.c genCode.c
```
