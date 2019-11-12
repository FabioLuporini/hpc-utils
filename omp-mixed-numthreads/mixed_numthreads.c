#define _GNU_SOURCE // sched_getcpu(3) is glibc-specific (see the man page)

#include <omp.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    printf("Usage: nested num_threads_fst num_threads_snd\n");
    return 0;
  }

  const int num_threads_fst = atoi(argv[1]);
  const int num_threads_snd = atoi(argv[2]);

  // Systematically disable dynamic adjustment of the number of threads
  // available for running parallel regions (the default value is
  // otherwise implementation defined)
  omp_set_dynamic(0);

  // To do some computation
  const int padding = 10000;
  int array_fst[num_threads_fst][padding];
  int array_snd[num_threads_snd][padding];

  // Timers
  struct timeval start, end;

  // Computation
  const int nsteps = 100000;
  printf("Now computing %d steps\n", nsteps);
  gettimeofday(&start, NULL);
  for (int n = 0; n < nsteps; n++)
  {
    #pragma omp parallel for num_threads(num_threads_fst)
    for (int i = 0; i < num_threads_fst; i++)
    {
      array_fst[i][0] = num_threads_fst*num_threads_snd;
    }

    #pragma omp parallel for num_threads(num_threads_snd)
    for (int i = 0; i < num_threads_snd; i++)
    {
      array_snd[i][0] = num_threads_fst*num_threads_snd;
    }
  }
  gettimeofday(&end, NULL);

  // Avoid loops are optimized away
  int sum = 0;
  for (int i = 0; i < num_threads_fst; i++)
  {
    for (int j = 0; j < num_threads_snd; j++)
    {
      sum += array_fst[i][0] + array_snd[j][0];
    }
  }

  double elapsed = (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_usec - start.tv_usec)/1000000;
  printf("Computed <%d> in %f secs\n", sum, elapsed);

  return(0);
}
