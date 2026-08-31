#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "common.h"

typedef struct {
    int y_inicio;
    int y_fim;
    int width;
    int height;
    int max_iter;
    unsigned char *pixels;
} TarefaBloco;

static void *worker_bloco(void *arg) {
    TarefaBloco *t = (TarefaBloco *)arg;
    for (int y = t->y_inicio; y < t->y_fim; y++) {
        for (int x = 0; x < t->width; x++) {
            double cr, ci;
            pixel_para_complexo(x, y, t->width, t->height, &cr, &ci);
            int iter = mandelbrot_iteracoes(cr, ci, t->max_iter);
            t->pixels[y * t->width + x] = normalizar(iter, t->max_iter);
        }
    }
    return NULL;
}

int rodar_pthreads1(Params p, const char *login) {
    unsigned char *pixels = malloc((size_t)p.width * (size_t)p.width * sizeof(unsigned char));
    if (pixels == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memoria para pthreads1.\n");
        return -1;
    }

    int num_threads = p.num_threads;
    pthread_t *threads = malloc((size_t)num_threads * sizeof(pthread_t));
    TarefaBloco *tarefas = malloc((size_t)num_threads * sizeof(TarefaBloco));

    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    int linhas_por_thread = p.height / num_threads;
    int resto = p.height % num_threads;
    int y_atual = 0;

    for (int i = 0; i < num_threads; i++) {
        int bloco = linhas_por_thread + (i < resto ? 1 : 0);
        tarefas[i].y_inicio = y_atual;
        tarefas[i].y_fim = y_atual + bloco;
        tarefas[i].width = p.width;
        tarefas[i].height = p.height;
        tarefas[i].max_iter = p.max_iter;
        tarefas[i].pixels = pixels;
        y_atual += bloco;
        pthread_create(&threads[i], NULL, worker_bloco, &tarefas[i]);
    }
    for (int i = 0; i < num_threads; i++) pthread_join(threads[i], NULL);

    clock_gettime(CLOCK_MONOTONIC, &fim);
    double segundos = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    char nome_arquivo[256];
    snprintf(nome_arquivo, sizeof(nome_arquivo), "mandelbrot_%s_pthreads1.pgm", login);
    escrever_saida(nome_arquivo, pixels, p.width, p.height);
    free(pixels); free(threads); free(tarefas);
    return registrar_tempo("Pthreads1 (blocos contiguos)", segundos);
}