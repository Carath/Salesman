#include <stdio.h>
#include <stdlib.h>
#include <time.h> // For RAND_INIT()

#include "animation.h"
#include "benchmarks.h"


// Benchmarking the genetic search parameters:
void mainBenchmark(void);


int main(void)
{
	RAND_INIT(time(NULL)); // Initialization of the pseudo-random number generator.

	///////////////////////////////////////////////////////
	// Graphical animation:

	animation();

	///////////////////////////////////////////////////////
	// Benchmarking:

	// mainBenchmark();


	return EXIT_SUCCESS;
}


// Benchmarking the genetic search parameters:
void mainBenchmark(void)
{
	int citiesNumber = 50;

	double time_granted = 15.; // In seconds. Does not include the setup time.

	// Do not set the epochs number! This will be automaticaly tuned!
	// Choose one of the following benchmarks:

	///////////////////////////////////////////////////////

	// printf("\nPopulation size benchmark:\n\n");

	int bench_pass_number = 75;

	int PopulationSize_array[] = {1, 32, 64};
	int MaxMutationNumber_array[] = {1};
	int SelectOpt_array[] = {UNIFORMLY};
	int MutationOpt_array[] = {MIXED};
	int GooseOpt_array[] = {NO_GOOSE_FULL};
	int SymOpt_array[] = {SYM_ALLOWED};


	// -> Each population has had enough time to reach the stagnation point (epoch_ratio is well below 1),
	// and we can notice that the greatest population size works best.

	///////////////////////////////////////////////////////

	// printf("\nMutation number benchmark:\n\n");

	// int bench_pass_number = 10;

	// int PopulationSize_array[] = {8};
	// int MaxMutationNumber_array[] = {1, 10, 20};
	// int SelectOpt_array[] = {CUMULATIVELY};
	// int MutationOpt_array[] = {MIXED};
	// int GooseOpt_array[] = {NO_GOOSE_FULL};
	// int SymOpt_array[] = {SYM_ALLOWED};


	// -> This benchmark is a bit inconsistent... For a small cities number, a MaxMutationNumber > 1 may help,
	// but this isn't necessary the case when said number of citites increases. However, setting MaxMutationNumber > 1
	// yield to far greater effective_time, i.e the stagnation point is reached much later, so we will stick with a value of 1.

	///////////////////////////////////////////////////////

	// printf("\nSelection option benchmark:\n\n");

	// int bench_pass_number = 100;

	// int PopulationSize_array[] = {32};
	// int MaxMutationNumber_array[] = {1};
	// int SelectOpt_array[] = {UNIFORMLY, CUMULATIVELY, BEST};
	// int MutationOpt_array[] = {MIXED};
	// int GooseOpt_array[] = {NO_GOOSE_FULL};
	// int SymOpt_array[] = {SYM_ALLOWED};


	// -> BEST is clearly the worst. UNIFORMLY performs as well as CUMULATIVELY, but is a bit faster.

	///////////////////////////////////////////////////////

	// printf("\nMutation type benchmark:\n\n");

	// int bench_pass_number = 20;

	// int PopulationSize_array[] = {8};
	// int MaxMutationNumber_array[] = {1};
	// int SelectOpt_array[] = {CUMULATIVELY};
	// int MutationOpt_array[] = {SWAP_CONSECUTIVE, SWAP_ANY, ROTATE, MIRROR, MIXED};
	// int GooseOpt_array[] = {NO_GOOSE_FULL};
	// int SymOpt_array[] = {SYM_ALLOWED};


	// -> MIXED seems to yield the best results if given enough time. MIRROR is just behind, but is also faster.

	///////////////////////////////////////////////////////

	// printf("\nGoose option benchmark:\n\n");

	// int bench_pass_number = 100;

	// int PopulationSize_array[] = {8};
	// int MaxMutationNumber_array[] = {10}; // This needs to be > 1 for this bench.
	// int SelectOpt_array[] = {CUMULATIVELY};
	// int MutationOpt_array[] = {MIXED};
	// int GooseOpt_array[] = {GOOSE, NO_GOOSE_MAX, NO_GOOSE_FULL};
	// int SymOpt_array[] = {SYM_ALLOWED};


	// -> NO_GOOSE_MAX and NO_GOOSE_FULL are more or less as good as the other, but GOOSE is worse than the
	// other two: worse solutions and greater epoch_ratio.

	///////////////////////////////////////////////////////

	// printf("\nSymmetry option benchmark:\n\n");

	// int bench_pass_number = 500;

	// int PopulationSize_array[] = {32};
	// int MaxMutationNumber_array[] = {1};
	// int SelectOpt_array[] = {UNIFORMLY};
	// int MutationOpt_array[] = {MIXED};
	// int GooseOpt_array[] = {NO_GOOSE_FULL};
	// int SymOpt_array[] = {SYM_ALLOWED, SYM_UNALLOWED};


	// -> Doesn't make much a difference.

	///////////////////////////////////////////////////////
	// This should not be modified:

	int *test_values[FIELDS_NUMBER];

	test_values[PopSizeIndex] = PopulationSize_array;
	test_values[MaxMutationNumberIndex] = MaxMutationNumber_array;
	test_values[SelectOptIndex] = SelectOpt_array;
	test_values[MutationOptIndex] = MutationOpt_array;
	test_values[GooseOptIndex] = GooseOpt_array;
	test_values[SymOptIndex] = SymOpt_array;

	int test_numbers[FIELDS_NUMBER] = {
		ARRAY_SIZE(PopulationSize_array),
		ARRAY_SIZE(MaxMutationNumber_array),
		ARRAY_SIZE(SelectOpt_array),
		ARRAY_SIZE(MutationOpt_array),
		ARRAY_SIZE(GooseOpt_array),
		ARRAY_SIZE(SymOpt_array) };

	// Starting the benchmark:
	benchmarkGenParams(citiesNumber, test_numbers, test_values, bench_pass_number, time_granted);
}
