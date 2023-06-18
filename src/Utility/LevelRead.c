
#include "global.h"
#include "Utility/LevelRead.h"

int readline(FILE *stream, char *line, size_t n) {
    while (fgets(line, n, stream) != NULL) {
        if (line[0] == '#' || line[0] == '\n')
            continue;
        return 1;
    }
    raise_err("readline: EOF");
    return 0;
}