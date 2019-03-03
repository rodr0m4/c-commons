#ifndef __KERNEL_H__
#define __KERNEL_H__

#define null (void*) 0

/// It takes anything as input and does nothing (dummy destructor, for example)
void noop(void* _);

/// It takes anything as input and returns it (identity function).
void* id(void* x);

#endif
