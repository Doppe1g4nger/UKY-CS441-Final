%{
#include <stdio.h>
#include <stdlib.h>
#include "apm.h"
#include "apname.h"
#include "asm.h"
#include "misc.h"

/* Report an error */
void yyerror(const char *msg)
{
	fprintf(stderr, "%s", msg);
}

typedef struct wordList {
	WORD value;
	struct wordList *next;
} wordList;

static wordList *reverse(wordList *l) {
	wordList *curr = NULL;
	wordList *result = NULL;
	for(curr = l; curr != NULL; curr = curr->next) {
		wordList *n = malloc(sizeof(wordList));
		n->value = curr->value;
		n->next = result;
		result = n;
	}
	return result;
}


static void freeList(wordList *l) {
	while(l != NULL) {
		wordList *next = l->next;
		free(l);
		l = next;
	}
	
}

static int DEBUG = 0;

%}

%union {
	float real;
	int integer;
	WORD num;
	char *word;
	struct wordList *values;
}

/* type declarations */

%token <word> T_WORD
%token <integer> INTEGER
%token <real> REAL
%token '\n' ':' DATA UNKNOWN

%type <values> arglist line
%type <num> num arg

%start start

%%

start:	lines
     {
     }
     ;

lines: lines INTEGER line
    {
    	int loc = $2;
	wordList *l = $3;

    	if(DEBUG)
		printf("lines ::= lines INTEGER line\n");

	while(l != NULL) {
		if(loc > maxStorage)
			maxStorage = loc;
		storage[loc++] = l->value;
		l = l->next;
	}
    }
    | /* empty */
    {
    	if(DEBUG)
		printf("lines ::= \\epsilon\n");
    }
    ;

line: T_WORD arglist '\n'
    {
    	wordList *l;
    	if(DEBUG)
		printf("line ::= WORD arglist\n");
	l = malloc(sizeof(wordList));
	l->value.Integer = name2opcode($1);
	l->next = reverse($2);
	$$ = l;
    }
    | DATA num '\n'
    {
    	wordList *l;
    	if(DEBUG)
		printf("line ::= 'data' INTEGER\n");
	l = malloc(sizeof(wordList));
	l->value = $2;
	l->next = NULL;
	$$ = l;
    }
    ;

arglist: arglist arg
    {
    	if(DEBUG)
		printf("arglist ::= arglist arg\n");
    	$$ = malloc(sizeof(wordList));
	$$->value = $2;
	$$->next = $1;
    }
    | /* empty */
    {
    	if(DEBUG)
		printf("arglist ::= \\epsilon\n");
	$$ = NULL;
    }
    ;

arg: T_WORD ':' num
    {
    	if(DEBUG)
		printf("arg ::= WORD : INTEGER\n");
    	$$ = $3;
    }
    | num
    {
    	if(DEBUG)
		printf("arg ::= INTEGER\n");
    	$$ = $1;
    }
    ;

num: INTEGER
   {
   	$$.Integer = $1;
   }
   | REAL
   {
   	$$.Real = $1;
   }
   ;
