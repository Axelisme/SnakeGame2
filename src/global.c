#include <global.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

// Error
inline void raise_err(const char* msg) {fwprintf(stderr,L"error: %s\n", msg);exit(1);}
inline void raise_warn(const char* msg){fwprintf(stderr,L"warning: %s\n", msg);}
inline void show_msg(const char* msg)  {if (Debug) fwprintf(stdout,L"Message: %s\n", msg);}