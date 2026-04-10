#include "../parser.h"

inline void top_level_decl_fail(Parser* p, AstNode* node) {
    node -> kind = AST_ERROR;
    recover_top_level_decl(p);
}
