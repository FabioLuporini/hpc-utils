#define _GNU_SOURCE // sched_getcpu(3) is glibc-specific (see the man page)

#include <omp.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    printf("Usage: simd-multiple-vars problem_size\n");
    return 0;
  }

  const int size = atoi(argv[1]);
  const int nsteps = 1000;

  // Data init
  const int halo = 2;
  const int dsize = size + halo*2;
  float *u_vec = (float*) calloc(2*dsize*dsize*dsize, sizeof(float));
  float *r_vec = (float*) calloc(dsize*dsize*dsize, sizeof(float));

  // Data cast
  float (*restrict u)[dsize][dsize][dsize] = (float (*)[dsize][dsize][dsize]) u_vec;
  float (*restrict r)[dsize][dsize] = (float (*)[dsize][dsize]) r_vec;

  // Timers
  struct timeval start, end;

  // Computation (laplacian-like) with two iteration vars
  printf("Now computing %d steps with iter vars z0 and z\n", nsteps);
  gettimeofday(&start, NULL);
  for (int t = 0, t0 = t%2, t1 = (t + 1)%2; t < nsteps; t++, t0 = t%2, t1 = (t + 1)%2)
  {
    for (int x = 0; x < size; x++)
    {
      for (int y = 0; y < size; y++)
      {
        int z0 = 0;
        #pragma omp simd
        for (int z = -1; z < size + 1; z++)
        {
          r[x + 2][y + 2][z0] = u[t0][x][y][z + 1] - u[t0][x][y][z + 2] + u[t0][x][y][z + 3];
          z0++;
        }
      }
    }
    for (int x = 0; x < size; x++)
    {
      for (int y = 0; y < size; y++)
      {
        int z0 = 0;
        #pragma omp simd
        for (int z = 0; z < size; z++)
        {
          u[t1][x + 2][y + 2][z + 2] = 
              (u[t0][x + 1][y + 2][z + 2] + u[t0][x + 3][y + 2][z + 2] + u[t0][x + 2][y + 1][z + 2] + u[t0][x + 2][y + 3][z + 2])*0.002 +
              (r[x + 2][y + 2][z0] + r[x + 2][y + 2][z0 + 1] + r[x + 2][y + 2][z0 + 2] + r[x + 3][y + 2][z0] + r[x + 3][y + 2][z0 + 1])*0.001 + 0.00000001;
          z0++;
       }
      }
    }
  }
  gettimeofday(&end, NULL);

  // Avoid loops are optimized away
  float sum = 0.0;
  for (int t = 0; t < 2; t++)
  {
    for (int x = 0; x < size; x++)
    {
      for (int y = 0; y < size; y++)
      {
        for (int z = 0; z < size; z++)
        {
          sum += u[t][x][y][z];
        }
      }
    }
  }

  double elapsed = (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_usec - start.tv_usec)/1000000;
  printf("Computed <%f> in *** %f secs ***\n", sum, elapsed);

  return(0);
}
