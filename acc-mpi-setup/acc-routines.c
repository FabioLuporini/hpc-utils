#include <math.h>
#include <string.h>
#include <stdio.h>
#include <openacc.h>
#include <mpi.h>

int main(int argc, char** argv)
{
  int rank = 0;
  int size = 1;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  
  int ngpus=acc_get_num_devices(acc_device_nvidia);
  int devicenum=rank%ngpus;
  acc_set_device_num(devicenum,acc_device_nvidia);

  // Must call `acc_init` after `acc_set_device_num` to avoid multiple
  // contexts on device 0 in multi GPU systems
  acc_init(acc_device_nvidia);

  printf("rank %d/%d: GPU %d/%d\n", rank, size, devicenum, ngpus);

  MPI_Finalize();
  return 0;
}
