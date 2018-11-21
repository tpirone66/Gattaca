#include "Game.h"
#include <time.h>
#include <stdlib.h> 
#include <stdio.h>
#include <assert.h>

/*! Tests one game
*/
void randomize(Strategy* strategy);
double getDiversity(Strategy* s1, Strategy* s2);

void test30() {
	Strategy bs = BasicStrategy_();
	Strategy st = Strategy_();

	randomize(&st);
	getDiversity(&st, &bs);
	Game statistics = start(&st, 2000000, 0);
	float stdev = 0.000746;
	float pl = -0.00495;
	float meanpl = (statistics.pl / statistics.nohands);
	float threshold = (pl - 1.645*stdev);

	//output(&statistics, 0);

	//output(&statistics, 0);
	printf("mean pl = %.6f\n", meanpl);
	printf("threshold = %.6f\n", threshold);
	if (meanpl < threshold) {
		printf("Reject\n");
	}
	else {
		printf("Accept\n");
	}
	printf("diversity = %4d\n", getDiversity(&st, &bs));
}

void randomize(Strategy * strategy) {

	srand(time(NULL));

	Play plays[] = { STAY, HIT, DOUBLE_DOWN, SPLIT };

	for (int k = 0; k < NUMBER_RULES; k++) {
		strategy->rules[k] = plays[rand() % 4];
	}
}

double getDiversity(Strategy * s1, Strategy * s2)
{
	double diversity = 0.00;
	for (int i = 0; i < NUMBER_RULES; i++) {
		/*int j = i + 1;
		if (s1[i].rules == s2[j].rules) {
			diversity++;
		}
		else {
			diversity = diversity;
		}*/
		for (int j = i + 1; j < NUMBER_RULES; j++) {
			if (s1[i].rules == s2[j].rules) {
				diversity++;
			}
			else {
				diversity = diversity;
			}
		}
	}
	return diversity;
}




