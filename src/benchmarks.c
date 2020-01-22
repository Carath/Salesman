#include <stdio.h>
#include <stdlib.h>

#include "benchmarks.h"


static int* getField(GenParameters *params, FieldIndex index)
{
	switch (index)
	{
		case PopSizeIndex:
			return &(params -> PopulationSize);

		case MaxMutationNumberIndex:
			return &(params -> MaxMutationNumber);

		case SelectOptIndex:
			return (int*) &(params -> SelectOpt);

		case MutationOptIndex:
			return (int*) &(params -> MutationOpt);

		case GooseOptIndex:
			return (int*) &(params -> GooseOpt);

		case SymOptIndex:
			return (int*) &(params -> SymOpt);

		default:
			return NULL;
	}
}


static void printArray(int *array, int len)
{
	for (int i = 0; i < len; ++i)
		printf("%4d ", array[i]);

	printf("\n");
}


// 'index' must start at 0.
void printCartesianProd(int *buffer, int index, int len, int *values_number, int **values)
{
	if (index == len)
	{
		printArray(buffer, len);
		return;
	}

	for (int i = 0; i < values_number[index]; ++i)
	{
		buffer[index] = values[index][i];

		printCartesianProd(buffer, index + 1, len, values_number, values);
	}
}


static void tuneAndBench(int field_index, int result_index, int branch_number, Map *map, int *best_path, Number ref_pathLength,
	GenParameters *params, int *test_numbers, int **test_values, double *perf_array, double *time_array, float *ratio_array,
	int *epochs_array, int setup)
{
	if (field_index == FIELDS_NUMBER)
	{
		params -> Epochs = epochs_array[result_index];

		if (setup)
		{
			// printGenParameters(params);

			printf("Index: %3d,  param: (P = %3d, MM = %3d, C = %d, M = %d, G = %d, S = %d)\n",
				result_index, params -> PopulationSize, params -> MaxMutationNumber,
				params -> SelectOpt, params -> MutationOpt, params -> GooseOpt, params -> SymOpt);
		}

		Number best_pathLength;
		double time;
		float epoch_ratio;

		smallest_genetic(map, best_path, &best_pathLength, &time, params, &epoch_ratio);

		if (!setup)
		{
			perf_array[result_index] += compare(ref_pathLength, best_pathLength);

			ratio_array[result_index] += epoch_ratio;
		}

		time_array[result_index] += time;

		return;
	}

	int *field = getField(params, field_index);

	if (field == NULL)
	{
		printf("NULL field.\n");
		exit(EXIT_FAILURE);
	}

	branch_number /= test_numbers[field_index];

	for (int test = 0; test < test_numbers[field_index]; ++test)
	{
		*field = test_values[field_index][test];

		tuneAndBench(field_index + 1, result_index, branch_number, map, best_path, ref_pathLength,
			params, test_numbers, test_values, perf_array, time_array, ratio_array, epochs_array, setup);

		result_index += branch_number;
	}
}


// Benchmark framework for the genetic search. Will test the given parameters for the same amount of time,
// as the epochs number will be automaticaly tuned. 'time_granted' is in seconds (does not include the setup time).
void benchmarkGenParams(int citiesNumber, int *test_numbers, int **test_values, int bench_pass_number, double time_granted)
{
	const Number ref_pathLength = 120; // Somewhat arbitrary, 120 works well for 50 cities.
	const int setup_epochs = 1000000; // This should be tuned so that the setup part takes around 1-2 seconds.

	clock_t start = clock();

	printf("Benchmark's meta parameters: citiesNumber = %d,  ref_pathLength = %.1f,  setup_epochs = %d\n\n",
		citiesNumber, ref_pathLength, setup_epochs);

	int total_bench_number = 1; // will be the product of every test_numbers[i]:

	for (int i = 0; i < FIELDS_NUMBER; ++i)
		total_bench_number *= test_numbers[i];

	double *perf_array = (double*) calloc(total_bench_number, sizeof(double*));
	double *time_array = (double*) calloc(total_bench_number, sizeof(double*));
	float *ratio_array = (float*) calloc(total_bench_number, sizeof(float*));
	int *epochs_array = (int*) calloc(total_bench_number, sizeof(int));

	Map *map = createMap(citiesNumber);

	int *best_path = (int*) calloc(citiesNumber, sizeof(int));

	GenParameters *genParams = initGenParameters();

	genParams -> Binfo = INFO; // In order to get the epoch ratios.

	///////////////////////////////////////////////////////
	// Setup:

	int setup = 1;

	for (int i = 0; i < total_bench_number; ++i)
		epochs_array[i] = setup_epochs;

	// Doing the time estimation:

	tuneAndBench(0, 0, total_bench_number, map, best_path, ref_pathLength, genParams,
		test_numbers, test_values, perf_array, time_array, ratio_array, epochs_array, setup);

	// Computing the number of epochs per benchmark:

	double epoch_basis = setup_epochs * time_granted / (total_bench_number * bench_pass_number);

	printf("\nSetup:\n\n");

	for (int i = 0; i < total_bench_number; ++i)
	{
		epochs_array[i] = epoch_basis / time_array[i];

		printf("Index: %3d,  time = %.3f s  =>  epochs = %d\n", i, time_array[i], epochs_array[i]);
	}

	// Resetting 'time_array':

	for (int i = 0; i < total_bench_number; ++i)
		time_array[i] = 0;

	///////////////////////////////////////////////////////
	// Actual benchmark:

	printf("\nSetup done (time elapsed: %.3f s). Starting the actual benchmark:\n", time_elapsed(start));

	setup = 0;

	for (int i = 0; i < bench_pass_number; ++i)
	{
		initMap(map);

		tuneAndBench(0, 0, total_bench_number, map, best_path, ref_pathLength, genParams,
			test_numbers, test_values, perf_array, time_array, ratio_array, epochs_array, setup);
	}

	///////////////////////////////////////////////////////
	// Printing the results:

	printf("\nResults: (bench_pass_number = %d)\n\n -> The smaller the error, the better:\n\n", bench_pass_number);

	for (int i = 0; i < total_bench_number; ++i)
	{
		perf_array[i] /= bench_pass_number;
		time_array[i] /= bench_pass_number;
		ratio_array[i] /= bench_pass_number;

		printf("Index: %3d,  error = %6.2f %%,  epoch_ratio = %5.2f %%,  time = %5.3f s,  effective time = %5.3f s\n",
			i, 100 * perf_array[i], 100 * ratio_array[i], time_array[i], ratio_array[i] * time_array[i]);
	}

	///////////////////////////////////////////////////////
	// Freeing stuff:

	free(perf_array);
	free(time_array);
	free(ratio_array);
	free(epochs_array);
	free(best_path);
	free(genParams);
	freeMap(map);

	printf("\n -> Time elapsed: %.3f s\n\n", time_elapsed(start));
}
