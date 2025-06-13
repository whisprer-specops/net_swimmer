/* duktape.c - stub */
#include "duktape.h"
#include <stdio.h>
#include <stdlib.h>

struct duk_hthread { int dummy; };

duk_context *duk_create_heap_default(void) {
    return malloc(sizeof(duk_context));
}

void duk_destroy_heap(duk_context *ctx) {
    free(ctx);
}

int duk_peval_string(duk_context *ctx, const char *src) {
    (void)ctx;
    printf("EVAL: %s\n", src);
    return 0;
}

void duk_eval_string(duk_context *ctx, const char *src) {
    duk_peval_string(ctx, src);
}

const char *duk_safe_to_string(duk_context *ctx, int index) {
    (void)ctx; (void)index;
    return "{\"js\":\"ok\"}";
}
