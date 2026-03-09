#pragma once

#include "types.h"

void match_delim(DelimStack* stack, Token* token, u32 index);
void delim_stack_push(DelimStack* stack, Token* token, u32 index); 
Token* delim_stack_pop(DelimStack* stack); 
