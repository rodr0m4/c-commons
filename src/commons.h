#ifndef __C_COMMONS__
#define __C_COMMONS__

#define null 0

/// It takes anything as input and does nothing (dummy destructor, for example)
void noop(void* _);

/// It takes anything as input and returns it (identity function).
void* id(void* x);

#endif
