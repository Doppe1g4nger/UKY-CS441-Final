%{
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "apm.h"
#include "apm.y.h"

extern YYSTYPE yylval; /* Expression for ID and NUM */

%}

	/* identifiers */
ident	[a-zA-Z][a-zA-Z0-9_]*

integer	-?[0-9]+
	/* numbers */

number	-?(([0-9]+\.[0-9]*)?|\.[0-9]+)([eE][+-]?[0-9]+)?

%%

	/* Skip whitespace */
[ \t]*			{ }

"DATA"			{ return DATA; }

":"			{ return ':'; }

"\n"			{ return '\n'; }

	/* Single-character tokens */
{ident}			{ yylval.word = strdup(yytext); return T_WORD; }

{integer}		{	long i;
				char *endptr;
				errno = 0;
				i=strtol(yytext, &endptr, 10);
				if(*endptr != '\0' || errno == ERANGE) {
					yylval.integer = 0;
				} else {
					yylval.integer = (int) i;
				}
				return INTEGER;
			}

	/* Number */
{number}		{	double d;
				char *endptr;
				errno = 0;
				d=strtod(yytext, &endptr);
				if(*endptr != '\0' || errno == ERANGE) {
					yylval.real = 0.0f;
				} else {
					yylval.real = (float) d;
				}
				return REAL;
			}

	/* Unknown */
.			{ return UNKNOWN; }

%{
/* For emacs:
%%% Local Variables: 
%%% mode: fundamental
%%% End:
*/
%}
