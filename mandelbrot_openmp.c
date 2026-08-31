#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "common.h"

int rodar_openmp(Params p, const char *login) {
    unsigned char *pixels = malloc((size_t)p.width * (size_t)p.height * sizeof(unsigned char));
    if (pixels == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memoria para a implementacao OpenMP.\n");
        return -1;
    }

    omp_set_num_threads(p.num_threads);

    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    /* Paralelizamos o laco externo (linhas). Cada linha e independente das
     * demais, entao nao ha necessidade de secao critica ou reducao. */
    #pragma omp parallel for schedule(dynamic, 4)
    for (int y = 0; y < p.height; y++) {
        for (int x = 0; x < p.width; x++) {
            double cr, ci;
            pixel_para_complexo(x, y, p.width, p.height, &cr, &ci);
            int iter = mandelbrot_iteracoes(cr, ci, p.max_iter);
            pixels[y * p.width + x] = normalizar(iter, p.max_iter);
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);
    double segundos = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    char nome_arquivo[256];
    snprintf(nome_arquivo, sizeof(nome_arquivo), "mandelbrot_%s_openmp.pgm", login);

    int status = escrever_saida(nome_arquivo, pixels, p.width, p.height);
    free(pixels);
    if (status != 0) return -1;

    return registrar_tempo("OpenMP", segundos);
}
