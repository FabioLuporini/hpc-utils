#define _GNU_SOURCE // sched_getcpu(3) is glibc-specific (see the man page)

#include <omp.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


int main(int argc, char* argv[])
{
  if (argc != 5)
  {
    printf("Usage: nested num_threads_fst_outer num_threads_fst_inner num_threads_snd_outer num_threads_snd_inner\n");
    return 0;
  }

  const int num_threads_fst_outer = atoi(argv[1]);
  const int num_threads_fst_inner = atoi(argv[2]);
  const int num_threads_snd_outer = atoi(argv[3]);
  const int num_threads_snd_inner = atoi(argv[4]);

  const int num_threads_fst = num_threads_fst_outer*num_threads_fst_inner;
  const int num_threads_snd = num_threads_snd_outer*num_threads_snd_inner;

  // Systematically disable dynamic adjustment of the number of threads
  // available for running parallel regions (the default value is
  // otherwise implementation defined)
  omp_set_dynamic(0);

  // Maybe enable nested parallelism
  if (num_threads_fst_inner > 1 || num_threads_snd_inner > 1)
  {
    omp_set_nested(1);
    omp_set_max_active_levels(2);
  }

  // To do some computation
  const int padding = 1000;
  int array_fst[num_threads_fst][padding];
  int array_snd[num_threads_snd][padding];

  // Timers
  struct timeval start, end;

  // Computation
  const int nsteps = 10000;
  printf("Now computing %d steps\n", nsteps);
  gettimeofday(&start, NULL);
  for (int n = 0; n < nsteps; n++)
  {
    int i;

    #pragma omp parallel for num_threads(num_threads_fst_outer) firstprivate(i)
    for (i = 0; i < num_threads_fst_outer; i++)
    {
      #pragma omp parallel for num_threads(num_threads_fst_inner)
      for (int j = 0; j < num_threads_fst_inner; j++)
      {
        array_fst[i*num_threads_fst_inner + j][0] = num_threads_fst*num_threads_snd;
      }
    }

    #pragma omp parallel for num_threads(num_threads_snd_outer) firstprivate(i)
    for (i = 0; i < num_threads_snd_outer; i++)
    {
      #pragma omp parallel for num_threads(num_threads_snd_inner)
      for (int j = 0; j < num_threads_snd_inner; j++)
      {
        array_snd[i*num_threads_snd_inner + j][0] = num_threads_fst*num_threads_snd;
      }
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
