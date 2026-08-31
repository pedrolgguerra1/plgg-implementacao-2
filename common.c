#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include "common.h"


static int parse_int_estrito(const char *s, int *out) {
    char *end;
    errno = 0;
    long v = strtol(s, &end, 10);
    if (s[0] == '\0' || *end != '\0') return 0;
    if (errno == ERANGE || v > INT_MAX || v < INT_MIN) return 0;
    *out = (int)v;
    return 1;
}

int validar_argumentos(int argc, char **argv, Params *p) {
    if (argc != 5) {
        fprintf(stderr,
            "Erro: numero de argumentos invalido.\n"
            "Uso: %s [largura] [altura] [max_iteracoes] [num_threads]\n",
            argc > 0 ? argv[0] : "mandelbrot");
        return -1;
    }

    int largura, altura, max_iter, num_threads;

    if (!parse_int_estrito(argv[1], &largura) || largura <= 0) {
        fprintf(stderr, "Erro: largura invalida (%s). Deve ser um inteiro positivo.\n", argv[1]);
        return -1;
    }
    if (!parse_int_estrito(argv[2], &altura) || altura <= 0) {
        fprintf(stderr, "Erro: altura invalida (%s). Deve ser um inteiro positivo.\n", argv[2]);
        return -1;
    }
    if (!parse_int_estrito(argv[3], &max_iter) || max_iter <= 0) {
        fprintf(stderr, "Erro: numero maximo de iteracoes invalido (%s). Deve ser um inteiro positivo.\n", argv[3]);
        return -1;
    }
    if (!parse_int_estrito(argv[4], &num_threads) || num_threads <= 0) {
        fprintf(stderr, "Erro: numero de threads invalido (%s). Deve ser um inteiro positivo.\n", argv[4]);
        return -1;
    }

    p->width = largura;
    p->height = altura;
    p->max_iter = max_iter;
    p->num_threads = num_threads;
    return 0;
}

void pixel_para_complexo(int px, int py, int width, int height,
                          double *cr, double *ci) {
    *cr = RE_MIN + (double)px / (double)(width - 1) * (RE_MAX - RE_MIN);
    *ci = IM_MIN + (double)py / (double)(height - 1) * (IM_MAX - IM_MIN);
}

int mandelbrot_iteracoes(double cr, double ci, int max_iter) {
    double zr = 0.0, zi = 0.0;
    int iter = 0;
    while (zr * zr + zi * zi <= 4.0 && iter < max_iter) {
        double zr_novo = zr * zr - zi * zi + cr;
        double zi_novo = 2.0 * zr * zi + ci;
        zr = zr_novo;
        zi = zi_novo;
        iter++;
    }
    return iter;
}

unsigned char normalizar(int iter, int max_iter) {
    if (iter >= max_iter) return 0; 
    double proporcao = (double)iter / (double)max_iter;
    int valor = (int)(proporcao * 255.0);
    if (valor > 255) valor = 255;
    if (valor < 0) valor = 0;
    return (unsigned char)valor;
}

int escrever_saida(const char *filename, unsigned char *pixels,
                    int width, int height) {
    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        fprintf(stderr, "Erro: nao foi possivel criar o arquivo de saida '%s'.\n", filename);
        return -1;
    }
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            fprintf(f, "%d", pixels[y * width + x]);
            if (x < width - 1) fprintf(f, " ");
        }
        fprintf(f, "\n");
    }
    fclose(f);
    return 0;
}

int registrar_tempo(const char *label, double segundos) {
    FILE *f = fopen("times.txt", "a");
    if (f == NULL) {
        fprintf(stderr, "Erro: nao foi possivel abrir times.txt para registrar o tempo.\n");
        return -1;
    }
    fprintf(f, "%s: %.6f segundos\n", label, segundos);
    fclose(f);
    return 0;
}
