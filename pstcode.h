#ifndef PSTCODE_H_INCLUDED
#define PSTCODE_H_INCLUDED

#include <cstdio>
#include <string>
#include <list>
#include <vector>
#include "symbtable.h"
#include "pstack/apm.h"

// Temporaried used by Code::add_*()
#define ADDR_TEMP  (STORAGE - 2)

class PstackCode 
{
    public:
	PstackCode()
		: code(), strings()
	{}
	PstackCode(const PstackCode &ps)
		: code(ps.code), strings(ps.strings)
	{}
	
	// Add an instruction or (non-relocatable) operand
	void add(int op);
	
	// Generate code to duplicate the top element of the stack.
	void add_dup();

	// Generate code to begin a program.
	void begin_prog();

	// Generate code for standard functions.
	void prolog(SymbolTable &fvsyms);

	// Generate code to end a program.
	void end_prog();
	
	// Write the code to a file.
	bool write(std::FILE *file, bool binary = true) const;

	// Return a copy of the code vector.
	std::vector<int> codevec() const;
	
	// Return the current position.
	int pos(void) const;
	
	// Return a reference to the instruction/operand at the specified
	// position.
	int &at(int p);
	// Or the value of that instruction/operand.
	int at(int p) const;

	// Add a string constant to be appended to the completed program.
	void add_string(const std::string &value, int pos);
    private:
	// Sequence of instructions and operands. Supports integers only;
	// need a vector of unions to handle doubles.
	std::vector<int> code;
	// Strings and the code addresses that will be backpatched to contain
	// those strings' addresses.
	std::list<std::pair<std::string, int> > strings;
};

#endif // PSTCODE_H_INCLUDED
