#include "toy_table.h"

#include <stdio.h>

//utils
unsigned int hashUInt(unsigned int x) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

void stress_inserts(unsigned int seed, unsigned int iterations, unsigned int limit) {
	//randomly generate a series of key-value pairs (from a seed) and insert them
	{
		//setup
		Toy_Table* table = Toy_allocateTable();

		for (unsigned int i = 0; i < iterations; i++) {
			//next seed
			seed = hashUInt(seed);

			//don't exceed a certain number of entries
			unsigned int masked = seed & (limit-1); //lol

			//actual values don't matter, as long as they can be recreated
			Toy_Value key = TOY_VALUE_FROM_INTEGER(masked);
			Toy_Value value = TOY_VALUE_FROM_INTEGER(masked);

			Toy_insertTable(&table, key, value);
		}

		//cleanup
		Toy_freeTable(table);
	}
}

int main(int argc, char* argv[]) {
	if (argc != 3) {
		printf("Usage: %s iterations limit\n", argv[0]);
		return 0;
	}

	unsigned int iterations = 0;
	unsigned int limit = 0;

	sscanf(argv[1], "%u", &iterations);
	sscanf(argv[2], "%u", &limit);

	//limit to 16mb
	if (limit * sizeof(Toy_TableEntry) > (1024 * 1024 * 16)) {
		printf("Error: limit must be below %u for safety reasons\n", (1024 * 1024 * 16)/sizeof(Toy_TableEntry));
		return 0;
	}

	//run the stress test
	stress_inserts(42, iterations, limit);

	return 0;
}
