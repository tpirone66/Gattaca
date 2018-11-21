/* Trevor Pirone Test04.cpp*/

#include <stdio.h>
#include "Strategy.h"
#include "Game.h"
#include "Kpax.h"
#include "Helpers.h"
#include "Population.h"
#include "GA.h"

#define NUM_BLOCKS 2
#define NUM_THREADS_PER_BLOCK 5
#define NUM_THREADS_TOTAL (NUM_BLOCKS * NUM_THREADS_PER_BLOCK)
#define NUM_STRATEGIES NUM_THREADS_TOTAL 
#define NUM_GAMES 1000
#define DIVERSITY_THRESHOLD 108
#define POPULATION_SIZE 100

void randomize(Strategy*);
void zero(Population*);
void average(Game*, Strategy*);
void evaluate(Strategy*);
void mutate(Strategy*);
Population evolve(Population*);
double getDiversity(Population*);
void evolver(int);
char* outputPlays(int);

void test04(void) {
	Strategy strategies[NUM_STRATEGIES];
	evolver(1);
}


void evolver(int maxGens) {
	Population curPop = Population_();
	Game statistics[NUM_STRATEGIES];
	int curGen = 0;
	double diversity = 0;

	for (int i = 0; i < NUM_STRATEGIES; i++) {
		statistics[i] = Game_();
	}

	for (int i = 0; i < curPop.size; i++) {
		randomize(&curPop.individuals[i]);
	}

	printf("%5s %5s %5s %5s\n", "Generation:", "Diversity:", "Fittest Individual:", "P&L Value:");

	while (true) {
		zero(&curPop);
		int status = evaluate(NUM_BLOCKS, NUM_STRATEGIES, curPop.individuals, NUM_GAMES, statistics);

		if (status == 0) {
			average(statistics, curPop.individuals);
			Population newPop = evolve(&curPop);

			for (int i = 0; i < newPop.size; i++) {
				mutate(&curPop.individuals[i]);
			}

			diversity = getDiversity(&curPop);
			curGen++;

			printf("%d, \t    %6f,  %d, \t\t   %6f\n\n", curGen, diversity, curPop.fittest, curPop.individuals[curPop.fittest].pl);

			if (curGen >= maxGens || diversity <= DIVERSITY_THRESHOLD) {
				break;
			}

			curPop = newPop;
		}
		else {
			fprintf(stderr, "evaluate returned code = %d\n", status);
		}
	}

	printf("%s%15s\n", "Fittest Individual:", "Fittest P&L:");
	printf("%d %29.6f\n\n", curPop.fittest, curPop.individuals[curPop.fittest].pl);
	printf("Fittest Strategy:\n");
	printf("    23456789TA\n");
	int j = 0;
	for (int i = 0; i < NUMBER_RULES; i += 10) {
		if (i < 100) {
			printf("  %d %s%s%s%s%s%s%s%s%s%s\n", j, outputPlays(curPop.individuals[curPop.fittest].rules[i]),
				outputPlays(curPop.individuals[curPop.fittest].rules[i + 1]),
				outputPlays(curPop.individuals[curPop.fittest].rules[i + 2]),
				outputPlays(curPop.individuals[curPop.fittest].rules[i + 3]),
				outputPlays(curPop.individuals[curPop.fittest].rules[i + 4]),
				outputPlays(curPop.individuals[curPop.fittest].rules[i + 5]),
				outputPlays(curPop.individuals[curPop.fittest].rules[i + 6]),
				outputPlays(curPop.individuals[curPop.fittest].rules[i + 7]),
				outputPlays(curPop.individuals[curPop.fittest].rules[i + 8]),
				outputPlays(curPop.individuals[curPop.fittest].rules[i + 9])
			);
			j++;
		}
		else {
			printf(" %d %s%s%s%s%s%s%s%s%s%s\n", j, outputPlays(curPop.individuals[curPop.fittest].rules[i]),
				outputPlays(curPop.individuals[curPop.fittest].rules[i + 1]),
				outputPlays(curPop.individuals[curPop.fittest].rules[i + 2]),
				outputPlays(curPop.individuals[curPop.fittest].rules[i + 3]),
				outputPlays(curPop.individuals[curPop.fittest].rules[i + 4]),
				outputPlays(curPop.individuals[curPop.fittest].rules[i + 5]),
				outputPlays(curPop.individuals[curPop.fittest].rules[i + 6]),
				outputPlays(curPop.individuals[curPop.fittest].rules[i + 7]),
				outputPlays(curPop.individuals[curPop.fittest].rules[i + 8]),
				outputPlays(curPop.individuals[curPop.fittest].rules[i + 9])
			);
			j++;
		}
	}

	printf("\n");
	report(curPop.individuals, statistics, NUM_STRATEGIES);
}

void randomize(Strategy* strategy) {
	Play plays[] = { STAY, HIT, DOUBLE_DOWN, SPLIT };
	for (int index = 0; index < NUMBER_RULES; index++) {
		strategy->rules[index] = plays[rand() % 4];
	}
}

void zero(Population* population) {
	for (int i = 0; i < population->size; i++) {
		population->individuals[i].pl = 0;
	}
}

void average(Game* game, Strategy* strategy) {
	float meanpl = 0;
	for (int i = 0; i < NUM_STRATEGIES; i++) {
		meanpl = game[i].pl / game[i].nohands;
		strategy[i].pl = meanpl;
	}
}

double getDiversity(Population* population) {
	double diversity = 0;
	int size = population->size;
	Strategy* strategy = population->individuals;

	for (int j = 0; j < size - 1; j++) {
		for (int i = 0; i < NUMBER_RULES; i++) {
			if (strategy[j].rules[i] != strategy[j + 1].rules[i]);
				diversity++;
		}
	}
	diversity = ((2 * diversity) / ((POPULATION_SIZE * (POPULATION_SIZE - 1))));
	return diversity;
}

char* outputPlays(int play) {
	switch (play) {
	case 1: 
		return "S";
	case 2:
		return "H";
	case 3:
		return "D";
	case 4:
		return "P";
	default:
		return "X";
	}
	return "??";
}

