#include "ast.h"
#include "types.h"
#include <stdio.h>

void print_node(FILE* fd, AstNode node, u32 indent);
void print_indent(FILE* fd, u32 indent);

void print_ast(FILE* fd, Ast ast) {
    u32 indent = 1;

    fprintf(fd, "========= AST Dump ==========\n\n");
    fprintf(fd, "  Count: %d\n", ast.count);
    fprintf(fd, "  Capacity: %d\n\n", ast.capacity);

    for (u32 i = 0; i < ast.count; i++) {
        AstNode node = ast.nodes[i];

        print_node(fd, node, indent);
    }

    fprintf(fd, "\n========= AST Dump ==========\n\n");
}

void print_node(FILE* fd, AstNode node, u32 indent) {
    switch (node.kind) {
        case AST_MODULE:
            print_indent(fd, indent);
            fprintf(fd, "<Module: ");
            
            for (u32 i = 0; i < node.module_decl.count - 1; i++) {
                fprintf(
                    fd,
                    "%.*s.",
                    (i32) node.module_decl.lens[i],
                    node.module_decl.ptrs[i]
                );
            }

            fprintf(
                fd,
                "%.*s>\n",
                (i32) node.module_decl.lens[node.module_decl.count - 1],
                node.module_decl.ptrs[node.module_decl.count - 1]
            );

            break;

        case AST_IMPORT:
            print_indent(fd, indent);
            fprintf(
                fd,
                "<Import: \"%.*s\">\n",
                (u32) node.import_decl.len,
                node.import_decl.ptr
            );
            break;

        default:
            print_indent(fd, indent);
            fprintf(
                fd,
                "<Node type, %s, not supported yet>\n",
                AST_KIND_STRINGS[node.kind]
            );
            break;
    }
}

void print_indent(FILE* fd, u32 indent) {
    for (u32 i = 0; i < indent; i++) {
        fprintf(fd, "  ");
    }
}
