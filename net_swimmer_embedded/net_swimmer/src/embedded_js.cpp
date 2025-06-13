#include "duktape.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

std::string run_embedded_js(const std::string& js_path) {
    duk_context *ctx = duk_create_heap_default();
    if (!ctx) {
        std::cerr << "[!] Failed to create Duktape heap" << std::endl;
        return "{}";
    }

    // Redirect print() in JS to capture output into a global string
    duk_eval_string(ctx,
        "var __capturedOutput = '';"
        "print = function(msg) { __capturedOutput += msg; };"
    );

    std::ifstream infile(js_path);
    std::stringstream buffer;
    buffer << infile.rdbuf();

    if (duk_peval_string(ctx, buffer.str().c_str()) != 0) {
        std::cerr << "[!] JS eval error: " << duk_safe_to_string(ctx, -1) << std::endl;
        duk_destroy_heap(ctx);
        return "{}";
    }

    // Get captured output
    duk_eval_string(ctx, "__capturedOutput");
    std::string result = duk_safe_to_string(ctx, -1);
    duk_destroy_heap(ctx);
    return result;
}