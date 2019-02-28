#ifndef __KERNEL_H__
#define __KERNEL_H__

#define null 0

/// It takes anything as input and does nothing (dummy destructor, for example)
void noop(void* _);

/// It takes anything as input and returns it (identity function).
void* id(void* x);

#define DEF(name) struct name ## Module
#define IMPL(name) static const struct name ## Module name =

#endif
