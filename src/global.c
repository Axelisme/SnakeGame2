#include <global.h>
#include <stdio.h>
#include <string.h>

// Global parameter
bool GameWindow_Mute;
int GameWindow_want_level;

// Error
inline void raise_err(const char* msg) {if (Debug) fprintf(stderr,"error: %s\n", msg);}
inline void raise_warn(const char* msg){if (Debug) fprintf(stderr,"warning: %s\n", msg);}
inline void show_msg(const char* msg)  {if (Debug) fprintf(stdout,"Messenger: %s\n", msg);}

// Init
void init_game() {return;}

// Path
void path_join(char* A, const char* B) {
    strcat(A, "/");
    strcat(A, B);
}