#define _GNU_SOURCE // sched_getcpu(3) is glibc-specific (see the man page)

#include <omp.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


int main(int argc, char* argv[])
{
  if (argc != 3 && argc != 4)
  {
    printf("Usage: nested num_threads_outer num_threads_inner\n");
    return 0;
  }

  const int num_threads_outer = atoi(argv[1]);
  const int num_threads_inner = atoi(argv[2]);
  const int num_threads_flatten = num_threads_outer*num_threads_inner;

  // Systematically disable dynamic adjustment of the number of threads
  // available for running parallel regions (the default value is
  // otherwise implementation defined)
  omp_set_dynamic(0);

  // Maybe enable nested parallelism
  if (num_threads_inner > 1)
  {
    omp_set_nested(1);
    omp_set_max_active_levels(2);
  }

  // To do some computation so that loops aren't optimized away
  int matrix[num_threads_outer][num_threads_inner];
  int array[num_threads_flatten];

  // Timers
  struct timeval start, end;

  // Computation
  const int nsteps = 90000;
  printf("Now computing %d steps\n", nsteps);
  gettimeofday(&start, NULL);
  for (int n = 0; n < nsteps; n++)
  {
    int i;
    #pragma omp parallel for num_threads(num_threads_outer) firstprivate(i)
    for (i = 0; i < num_threads_outer; i++)
    {
      int j;
      #pragma omp parallel for num_threads(num_threads_inner) firstprivate(j)
      for (j = 0; j < num_threads_inner; j++)
      {
        matrix[i][j] = num_threads_outer*num_threads_inner;
      }
    }

    #pragma omp parallel for num_threads(num_threads_flatten)
    for (int i = 0; i < num_threads_flatten; i++)
    {
      array[i] = num_threads_flatten*2;
    }
  }
  gettimeofday(&end, NULL);

  // Avoid loops are optimized away
  int sum0 = 0, sum1 = 0;
  for (int i = 0; i < num_threads_outer; i++)
  {
    for (int j = 0; j < num_threads_inner; j++)
    {
      sum0 += matrix[i][j];
    }
  }
  for (int i = 0; i < num_threads_flatten; i++)
  {
    sum1 += array[i];
  }

  double elapsed = (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_usec - start.tv_usec)/1000000;
  printf("Computed <%d,%d> in %f secs\n", sum0, sum1, elapsed);

  return(0);
}
