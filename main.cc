#include <cstdio>  // fopen, fprintf
#include <cstdlib> // exit
#include <exception>
#include "Absyn.H"
#include "Parser.H"
#include "codegen.h"
#include "pstcode.h"

using namespace std;

int main(int argc, char ** argv)
{
    FILE *input, *output;
    bool binmode = false; // Output in binary mode?

    if (argc > 1) 
    {
        input = fopen(argv[1], "r");
        if (!input)
        {
            fprintf(stderr, "Error opening input file.\n");
            return 1;
        }
    }
    else input = stdin;

    if (argc > 2)
    {
        output = fopen(argv[2], "wb");
        binmode = true;
        if (!output)
        {
            fprintf(stderr, "Error opening output file.\n");
            return 1;
        }
    }
    else output = stdout;

    /* The default entry point is used. For other options see Parser.H */
    Program *parse_tree = pProgram(input);
    if (parse_tree)
    {
        fprintf(stderr, "\nParse Succesful!\n");
        try
        {
            CodeGen cg;
            PstackCode code = cg.generate(parse_tree);
            code.write(output, binmode);
            return 0;
        }
        catch (exception &e)
        {
            fprintf(stderr, "Code-generation error: %s\n", e.what());
            // ... and fall through to return 1
        }
    }
    return 1;
}
