#include <global.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

// Global parameter
bool GameWindow_Mute;
int GameWindow_want_level;

// Error
inline void raise_err(const char* msg) {if (Debug) fwprintf(stderr,L"error: %s\n", msg);}
inline void raise_warn(const char* msg){if (Debug) fwprintf(stderr,L"warning: %s\n", msg);}
inline void show_msg(const char* msg)  {if (Debug) fwprintf(stdout,L"Messenger: %s\n", msg);}

// Init
void init_game() {return;}

// Path
void path_join(char* A, const char* B) {
    strcat(A, "/");
    strcat(A, B);
}