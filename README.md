# Mandelbrot - Implementação 2 (Infraestrutura de Software)

Aluno(a): plgg

Gera o conjunto de Mandelbrot em quatro implementações (serial, OpenMP,
Pthreads com blocos contíguos e Pthreads com divisão entrelaçada),
comparando o tempo de execução de cada uma.

## Compilar

```
make
```

## Executar

```
./mandelbrot [largura] [altura] [max_iteracoes] [num_threads]
```

Exemplo:

```
./mandelbrot 800 600 1000 4
```

Gera:
- `mandelbrot_plgg_serial.pgm`
- `mandelbrot_plgg_openmp.pgm`
- `mandelbrot_plgg_pthreads1.pgm`
- `mandelbrot_plgg_pthreads2.pgm`
- `times.txt` (tempo de cada implementação)

## Limpar

```
make clean
```
