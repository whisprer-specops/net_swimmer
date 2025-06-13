/* duktape.h - example header */
#ifndef DUKTAPE_H
#define DUKTAPE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct duk_hthread duk_context;

duk_context *duk_create_heap_default(void);
void duk_destroy_heap(duk_context *ctx);
int duk_peval_string(duk_context *ctx, const char *src);
void duk_eval_string(duk_context *ctx, const char *src);
const char *duk_safe_to_string(duk_context *ctx, int index);

#ifdef __cplusplus
}
#endif

#endif
