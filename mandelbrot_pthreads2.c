#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "common.h"

typedef struct {
    int tid;
    int stride;
    int width;
    int height;
    int max_iter;
    unsigned char *pixels;
} TarefaEntrelacada;

static void *worker_entrelacado(void *arg) {
    TarefaEntrelacada *t = (TarefaEntrelacada *)arg;
    for (int y = t->tid; y < t->height; y += t->stride) {
        for (int x = 0; x < t->width; x++) {
            double cr, ci;
            pixel_para_complexo(x, y, t->width, t->height, &cr, &ci);
            int iter = mandelbrot_iteracoes(cr, ci, t->max_iter);
            t->pixels[y * t->width + x] = normalizar(iter, t->max_iter);
        }
    }
    return NULL;
}

int rodar_pthreads2(Params p, const char *login) {
    unsigned char *pixels = malloc((size_t)p.width * (size_t)p.height * sizeof(unsigned char));
    if (pixels == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memoria para pthreads2.\n");
        return -1;
    }

    int num_threads = p.num_threads;
    if (num_threads > p.height) num_threads = p.height;

    pthread_t *threads = malloc((size_t)num_threads * sizeof(pthread_t));
    TarefaEntrelacada *tarefas = malloc((size_t)num_threads * sizeof(TarefaEntrelacada));
    if (threads == NULL || tarefas == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memoria para as threads (pthreads2).\n");
        free(pixels); free(threads); free(tarefas);
        return -1;
    }

    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    int criadas = 0;
    for (int i = 0; i < num_threads; i++) {
        tarefas[i].tid = i;
        tarefas[i].stride = num_threads;
        tarefas[i].width = p.width;
        tarefas[i].height = p.height;
        tarefas[i].max_iter = p.max_iter;
        tarefas[i].pixels = pixels;

        if (pthread_create(&threads[i], NULL, worker_entrelacado, &tarefas[i]) != 0) {
            fprintf(stderr, "Erro: falha ao criar a thread %d (pthreads2).\n", i);
            for (int j = 0; j < criadas; j++) pthread_join(threads[j], NULL);
            free(pixels); free(threads); free(tarefas);
            return -1;
        }
        criadas++;
    }

    for (int i = 0; i < criadas; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);
    double segundos = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    char nome_arquivo[256];
    snprintf(nome_arquivo, sizeof(nome_arquivo), "mandelbrot_%s_pthreads2.pgm", login);

    int status = escrever_saida(nome_arquivo, pixels, p.width, p.height);
    free(pixels); free(threads); free(tarefas);
    if (status != 0) return -1;

    return registrar_tempo("Pthreads2 (entrelacado / round-robin)", segundos);
}
