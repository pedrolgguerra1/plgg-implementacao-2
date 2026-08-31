#include <stdio.h>
#include "common.h"

#define LOGIN "plgg"

int rodar_serial(Params p, const char *login);

int main(int argc, char **argv) {
    Params p;
    if (validar_argumentos(argc, argv, &p) != 0) {
        return 1;
    }
    if (rodar_serial(p, LOGIN) != 0) return 1;
    return 0;
}