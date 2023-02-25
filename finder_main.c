#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define POPULATION_COUNT 500
#define GENE_LENGHT 18
#define MUTATION 1 /*percent*/

typedef struct {
	float fitness;
	char gene[GENE_LENGHT]; /*genotype*/
} Agent_t;

Agent_t g_populations[POPULATION_COUNT] = {0};

int random_int(int low_num, int hi_num) {
	int ret;

	ret = (rand() % (hi_num - low_num)) + low_num;

	return ret;
}

char random_char() {
	return (char) random_int(97 , 123);	
}

void init_population() {
	Agent_t *agent;

	srand(time(NULL));
	
	for (int i =0; i < POPULATION_COUNT; i++) {
		agent = &g_populations[i];

		for (int i = 0;i <= GENE_LENGHT; i++) {
			agent->gene[i] = random_char();	
		}
		agent = NULL;
	}		
}

//Todo
float calculate_fitness(Agent_t *agent, char *target, int *match){
	
	float fitness = 0.0;
	for (int i = 0; i < GENE_LENGHT ; i++ ) {
		if (agent->gene[i] == target[i])
			fitness++;
	}

	agent->fitness = fitness / GENE_LENGHT;

	if (agent->fitness == 1)
		*match = 1;

	return agent->fitness;
}

typedef struct {
	int count;
	Agent_t *agent;
} tmp_agent_t;



void create_crossover(char *gene_1, char * gene_2, char *child_gene){
	
	int mid = random_int(0 , GENE_LENGHT);
	int i = 0;
	for(; i<mid ;i++)
	      child_gene[i] = gene_1[i];
	for(i = mid; i < GENE_LENGHT; i++)
	      child_gene[i] = gene_2[i];
}

void add_mutation(char *gene){
	for (int i = 0; i < GENE_LENGHT ; i++) {
		if (random_int(0, 1000) <= 1)
			gene[i] = random_char();
	}
}
//TODO
void select_reproducers() {
	tmp_agent_t *tmp_agents = malloc(POPULATION_COUNT * sizeof(tmp_agent_t));
	Agent_t *agent;
	int tmp_agent_count = 0;
	for(int i =0; i < POPULATION_COUNT; i++) {
		agent = &g_populations[i];
		if (agent->fitness > 0) {
			tmp_agents[tmp_agent_count].count = (int)(agent->fitness * 100) + ((tmp_agent_count > 0) ? tmp_agents[tmp_agent_count -1].count : 0 );
			tmp_agents[tmp_agent_count].agent = agent;
			tmp_agent_count++;
		}
	}

	int max_count =  tmp_agents[tmp_agent_count - 1].count;
	if (max_count < 1)
		max_count = 1; 

	for(int i =0; i < POPULATION_COUNT; i++) {
		agent = &g_populations[i];

		Agent_t *parent_1 =  NULL;
		Agent_t *parent_2 =  NULL;

		int count = random_int(1,max_count + 1);
		for (int i = 0; i < tmp_agent_count ;i++) {
			if (tmp_agents[i].count >= count) {
				parent_1 = tmp_agents[i].agent;
				//memcpy(agent, tmp_agents[i].agent, sizeof(Agent_t));
				//agent->fitness = 0;
				break;
			}

		}
		
		count = random_int(1,max_count + 1);
		for (int i = 0; i < tmp_agent_count ;i++) {
			if (tmp_agents[i].count >= count) {
				parent_2 = tmp_agents[i].agent;
				break;
			}

		}

		create_crossover(parent_1->gene, parent_2->gene, agent->gene);
		agent->fitness = 0;
		add_mutation(agent->gene);
	}

	free(tmp_agents);
}

//TODO

//TODO
//TODO

void print_population(char *target) {
	printf("target:                  %s\n", target);
	Agent_t *agent;
	for (int i = 0; i< POPULATION_COUNT; i++) {
		agent = &g_populations[i];
		printf("1. ");
		printf("fitness:%f ", agent->fitness);
		printf("dna :");
		for (int i = 0; i< GENE_LENGHT; i ++) {
			printf("%c", agent->gene[i]);
		}
		printf("\n");
	}
}

int main(int argc, char **argv) {

	(void) argc;
	(void) argv;

	char *target = "helloworldhowareuu";
	int match = 0;
	int generations = 1;
	float avg_score = 0;

	init_population();

	while(!match) {
		avg_score = 0;
		for (int i=0; i < POPULATION_COUNT ; i++) {
			avg_score += calculate_fitness(&g_populations[i], target, &match);
		}
		avg_score /= POPULATION_COUNT;
		select_reproducers();
		generations++;
		
		system("clear");

		printf("Genetic Alog finding target: %s\n\n\n", target);
		printf("mutation: %d\n", MUTATION);
		printf("population: %d\n", POPULATION_COUNT);
		printf("generations : %d\n", generations);
		printf("avg_score: %f\n", avg_score);

	}


	return 0;
};

