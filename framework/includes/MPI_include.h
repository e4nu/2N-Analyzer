#ifndef MPI_HELPER_H
#define MPI_HELPER_H

/*
 * This header enables optional MPI support.
 *
 * If the code is compiled with -DUSE_MPI and an MPI compiler
 * (e.g. mpicxx), the MPI header is included and MPI functionality
 * becomes available.
 *
 * If USE_MPI is not defined, the code can still compile and run
 * in serial or OpenMP-only mode without MPI.
 */

#ifdef USE_MPI
#include <mpi.h>
#endif

#endif // MPI_HELPER_H