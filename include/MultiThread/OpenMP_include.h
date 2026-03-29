#ifndef OPENMP_UTILS_H
#define OPENMP_UTILS_H

/*
 * This header conditionally includes the OpenMP runtime header.
 *
 * The macro _OPENMP is automatically defined by the compiler
 * only when OpenMP support is enabled (e.g. using -fopenmp).
 *
 * If OpenMP is enabled:
 *   - <omp.h> is included
 *   - OpenMP runtime functions (omp_get_thread_num, etc.) are available
 *
 * If OpenMP is NOT enabled:
 *   - <omp.h> is not included
 *   - The code can still compile and run in serial mode
 *
 * This allows the same source code to be portable and usable
 * both with and without OpenMP support.
 */

#ifdef _OPENMP
    #include <omp.h>
#endif

#endif /* OPENMP_UTILS_H */