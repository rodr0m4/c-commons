#ifndef __KERNEL_H__
#define __KERNEL_H__

// Module system stuff (and it's a dumb warning)
#pragma clang diagnostic ignored "-Wunused-const-variable"

#define null (void*) 0

/// It takes anything as input and does nothing (dummy destructor, for example)
void noop(void* _);

/// It takes anything as input and returns it (identity function).
void* id(void* x);

/// Module sugar

/// Defines a module, with its types and function signatures
#define defm(name) struct name ## Module

/// Starts a module implementation, giving bodies to the module members
#define impl(name) static const struct name ## Module name =

#endif
