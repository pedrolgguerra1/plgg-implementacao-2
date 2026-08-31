#ifndef COMMON_H
#define COMMON_H


typedef struct {
    int width;
    int height;
    int max_iter;
    int num_threads;
} Params;

#define RE_MIN -2.0
#define RE_MAX  1.0
#define IM_MIN -1.5
#define IM_MAX  1.5

int validar_argumentos(int argc, char **argv, Params *p);

void pixel_para_complexo(int px, int py, int width, int height, double *cr, double *ci);

int mandelbrot_iteracoes(double cr, double ci, int max_iter);

unsigned char normalizar(int iter, int max_iter);

int escrever_saida(const char *filename, unsigned char *pixels, int width, int height);

int registrar_tempo(const char *label, double segundos);

#endif
