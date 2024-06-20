#ifndef JVM_H
#define JVM_H

#include "stack.h"

//////////////////////////////////////////////////////////////////////////////
// Types                                                                    //
//////////////////////////////////////////////////////////////////////////////

// Structure representing the MiniJVM
typedef struct
{
    // Bytecode array representing the program being executed
    char* bytecode;        
    
    // Program counter.  Points to the instruction in the bytecode array
    // that is currently being executed
    char* pc;              

    // Operand stack
    stack operands;

    // Local variables
    int locals[10];

    // Return value from the program.  0 = successful termination.  1 = error
    int return_value;

} minijvm;

//////////////////////////////////////////////////////////////////////////////
// Constants                                                                //
//////////////////////////////////////////////////////////////////////////////
#define CLASS_EXT ".mclass" 
    // File extension for MiniJVM programs
#define MAX_CLASS_SIZE 1024     // Maximum size of an .mclass file

//////////////////////////////////////////////////////////////////////////////
// Byte codes accepted by the MiniJVM                                       //
//////////////////////////////////////////////////////////////////////////////

// The following instructions take no operands in the bytecode

#define INST_ICONST0 0x03  // iconst_0 (3)
#define INST_POP     0x57  // pop (87)
#define INST_DUP     0x59  // dup (89)
#define INST_IADD    0x60  // iadd (96)
#define INST_ISUB    0x64  // isub (100)
#define INST_IMUL    0x68  // imul (104)
#define INST_IDIV    0x6c  // idiv (108)
#define INST_IREM    0x70  // irem (112)
#define INST_ISHR    0x7a  // ishr (122)
#define INST_RETURN  0xb1  // return (177)
#define INST_PRINT   0xbb  // print (187) -- not a real Java bytecode

// The following instructions take a single byte operand in the bytecode

#define INST_BIPUSH  0x10  // bipush (16)
#define INST_ILOAD   0x15  // iload (21) 
#define INST_ISTORE  0x36  // istore (54)

// The following instructions take a two byte operand in the bytecode

#define INST_IINC    0x84  // iinc (132)

// The following instructions take a 2-byte offset operand in the bytecode
// (used for branching).  The offset is a big-endian signed integer.

#define INST_IFEQ    0x99  // ifeq (153)
#define INST_GOTO    0xa7  // goto (167)

#endif
