#define _GNU_SOURCE // sched_getcpu(3) is glibc-specific (see the man page)

#include <omp.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char* argv[])
{
  if (argc != 3 && argc != 4)
  {
    printf("Usage: nested num_threads_outer num_threads_inner [offset]\n");
    return 0;
  }

  const int num_threads_outer = atoi(argv[1]);
  const int num_threads_inner = atoi(argv[2]);

  int offset = 0;
  if (argc == 4)
  {
    offset = atoi(argv[3]);
  }

  printf("Configuration: %dx%d threads\n", num_threads_outer, num_threads_inner);

  // Systematically disable dynamic adjustment of the number of threads
  // available for running parallel regions (the default value is
  // otherwise implementation defined)
  omp_set_dynamic(0);

  // Systematically enable nested parallelism
  omp_set_nested(1);
  omp_set_max_active_levels(2);

  int places[num_threads_outer][num_threads_inner];

  #pragma omp parallel for num_threads(num_threads_outer)
  for (int i = 0; i < num_threads_outer; i++)
  {
    const int thread_outer_num = omp_get_thread_num();
    #pragma omp parallel for num_threads(num_threads_inner)
    for (int j = 0; j < num_threads_inner; j++)
    {
      #pragma omp critical
      {
        const int thread_inner_num = omp_get_thread_num();
        const int cpu_num = sched_getcpu();
        places[thread_outer_num][thread_inner_num] = cpu_num;
      }
    }
  }

  // Emit placement
  for (int i = 0; i < num_threads_outer; i++)
  {
    printf("Outer thread %d\n", i);
    for (int j = 0; j < num_threads_inner; j++)
    {
      printf("  Inner thread %d -> logical core %d\n", j, places[i][j] + offset);
    }
  }

  // Spinlock
  printf("Now looping forever -- use `htop` to check the thread affinity\n");
  #pragma omp parallel for num_threads(num_threads_outer)
  for (int i = 0; i < num_threads_outer; i++)
  {
    const int thread_outer_num = omp_get_thread_num();
    #pragma omp parallel for num_threads(num_threads_inner)
    for (int j = 0; j < num_threads_inner; j++)
    {
      const int thread_inner_num = omp_get_thread_num();
      places[thread_outer_num][thread_inner_num] = 0;
    }
    while(1);
  }

  return(0);
}
