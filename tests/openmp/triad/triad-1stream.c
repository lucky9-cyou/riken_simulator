#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

inline double get_dtime(void) {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return ((double)(ts.tv_sec) + (double)(ts.tv_nsec) * 1e-9);
}

/* #define N 25600 */
#define N 314368

double ALPHA = 3.0;
double a[N] __attribute__((aligned(256)));
double b[N] __attribute__((aligned(256)));
double c[N] __attribute__((aligned(256)));

int main(int argc, char **argv)
{
  int i;
  static double time;

#pragma omp parallel
  {
    printf("%d of %d\n", omp_get_num_threads(), omp_get_thread_num());

#pragma omp for
    for (i = 0; i < N; i++) {
      a[i] = 0.0;
      b[i] = (double)(i);
      c[i] = (double)(i+3);
    }
  }

  time = get_dtime();

#pragma omp parallel for
    for (i = 0; i < N; i++)
      a[i] = b[i] + 3.0 * c[i];

  time = get_dtime() - time;

  printf("time = %lf [msec]\n", time * 1000.0);
  printf("GFLOPS = %lf\n", (2 * N) / time / 1e9);
  printf("BW = %lf [GB/s]\n", (3 * N * sizeof(double)) / time / 1e9);
  printf("dummy: %lf\n", a[N-1]);

  return 0;
}