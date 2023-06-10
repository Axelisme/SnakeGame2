#include <global.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

// Error
inline void raise_err(const char* msg) {fwprintf(stderr,L"error: %s\n", msg);}
inline void raise_warn(const char* msg){if (Debug) fwprintf(stderr,L"warning: %s\n", msg);}
inline void show_msg(const char* msg)  {if (Debug) fwprintf(stdout,L"Messenger: %s\n", msg);}