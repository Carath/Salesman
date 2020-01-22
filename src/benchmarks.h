#ifndef BENCHMARKS_H
#define BENCHMARKS_H


#include "genetic.h"
#include "utilities.h"


#define FIELDS_NUMBER 6 // Number of GenParameters fields other than Epochs or Binfo.


typedef enum {PopSizeIndex, MaxMutationNumberIndex, SelectOptIndex, MutationOptIndex, GooseOptIndex, SymOptIndex} FieldIndex;


// 'index' must start at 0.
void printCartesianProd(int *buffer, int index, int len, int *values_number, int **values);


// Benchmark framework for the genetic search. Will test the given parameters for the same amount of time,
// as the epochs number will be automaticaly tuned. 'time_granted' is in seconds (does not include the setup time).
void benchmarkGenParams(int citiesNumber, int *test_numbers, int **test_values, int bench_pass_number, double time_granted);


#endif
