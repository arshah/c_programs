#include<stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_QUESTIONS 100
#define SCORE_POINTS 10
#define SECTION_SIZE 2

struct question {
	int category;
	char *statement;
	int answer;
	char *option_1;
	char *option_2;
	char *option_3;
	char *option_4;
};

int main() {
	ssize_t read_size;
	size_t len = 0;
	char *temp_line = NULL;
	FILE *questions_file;
	struct question questions[MAX_QUESTIONS];
	unsigned int questions_read = 0;
	int entered_option = -1;
	unsigned int total_score = 0;
	unsigned int correct_answers_count = 0;
	unsigned int wrong_answers_count = 0;

	questions_file = fopen("questions_v2.txt", "r");
	if (questions_file) {
		char *temp_char;
		for (int i = 0; i < MAX_QUESTIONS; i++) {
			// Reading the category of the question
			if((read_size = getline(&temp_char, &len, questions_file)) == -1) {
				printf("reached end of questions_file\n");
				break;
			} else {
				questions[i].category = *temp_char - '0';
				/* printf("entered: %d\n", questions[i].category); */
			}

			// Reading the question
			if((read_size = getline(&questions[i].statement, &len, questions_file)) == -1) {
				printf("reached end of questions_file\n");
				break;
			}
			/* else { */
			/* printf("entered: %s\n", questions[i].statement); */
			/* } */

			// Reading the correct answer
			if((read_size = getline(&temp_char, &len, questions_file)) == -1) {
				printf("reached end of questions_file\n");
				break;
			} else {
				questions[i].answer = *temp_char - '0';
				/* printf("entered: %d\n", questions[i].answer); */
			}

			// Reading option 1
			if((read_size = getline(&questions[i].option_1, &len, questions_file)) == -1) {
				printf("reached end of questions_file\n");
				break;
			}
			/* else { */
			/* printf("entered: %s\n", questions[i].option_1); */
			/* } */

			// Reading option 2
			if((read_size = getline(&questions[i].option_2, &len, questions_file)) == -1) {
				printf("reached end of questions_file\n");
				break;
			}
			/* else { */
			/* printf("entered: %s\n", questions[i].option_2); */
			/* } */

			// Reading option 3
			if((read_size = getline(&questions[i].option_3, &len, questions_file)) == -1) {
				printf("reached end of questions_file\n");
				break;
			}
			/* else { */
			/* printf("entered: %s\n", questions[i].option_3); */
			/* } */

			// Reading option 4
			if((read_size = getline(&questions[i].option_4, &len, questions_file)) == -1) {
				printf("reached end of questions_file\n");
				break;
			}
			/* else { */
			/* printf("entered: %s\n", questions[i].option_4); */
			/* } */

			questions_read++;

			// Reading empty space (this also serves as a format check)
			if((read_size = getline(&temp_line, &len, questions_file)) == -1) {
				printf("reached end of questions_file\n");
				break;
			} else {
				// 10 is the newline character thingy. At least in linux, don't know about windows.
				if (*temp_line != 10) {
					printf("Something is wrong with the formatting of the questions_file.\n");
					printf("Got %d instead of empty line.\n", *temp_line);
				}
			}
		}
	}
	fclose(questions_file);

	// We will seperate the question into different array based on the sections
	
	struct question section_1_questions[SECTION_SIZE];
	struct question section_2_questions[SECTION_SIZE];
	struct question section_3_questions[SECTION_SIZE];
	int section_1_index = 0;
	int section_2_index = 0;
	int section_3_index = 0;

	for (int i = 0; i < questions_read; i++) {
		if (questions[i].category == 1){
			section_1_questions[section_1_index] = questions[i];
			section_1_index++;
		} else if (questions[i].category == 2){
			section_2_questions[section_2_index] = questions[i];
			section_2_index++;
		} else if (questions[i].category == 3){
			section_3_questions[section_3_index] = questions[i];
			section_3_index++;
		}
	}

	// Time for seed of random number
	time_t t;

	// Initializing random number generator
	srand((unsigned) time(&t));

	struct question* divided_questions[3];
	divided_questions[0] = section_1_questions;
	divided_questions[1] = section_2_questions;
	divided_questions[2] = section_3_questions;
	int divided_questions_indicies[3] = {section_1_index, section_2_index, section_3_index};

/*
 *     for(int q=0; q< 3; q++){
 *         printf("SECTION %d\n", q);
 * 
 *         for(int p=0; p < 2; p++){
 *             printf("\nQuestion: %s", divided_questions[q][p].statement);
 *         }
 *     }
 */

	for (int q = 0; q < 3; q++) {
		printf("\n=============== Section %d ===========\n", q + 1);

		int sequence[SECTION_SIZE];
		// Populating the array
		for (int i = 0; i < SECTION_SIZE; i++) {
			sequence[i] = i;
		}

		// Shuffling the elements of the array
		for (int i = 0; i < SECTION_SIZE; i++) {
			int temp_element = sequence[i];
			int random_index = rand() % SECTION_SIZE;
			sequence[i] = sequence[random_index];
			sequence[random_index] = temp_element;
		}

		for(int j = 0; j < divided_questions_indicies[q]; j++) {
			printf("\nQuestion: %s", divided_questions[q][sequence[j]].statement);
			printf("1: %s", divided_questions[q][sequence[j]].option_1);
			printf("2: %s", divided_questions[q][sequence[j]].option_2);
			printf("3: %s", divided_questions[q][sequence[j]].option_3);
			printf("4: %s", divided_questions[q][sequence[j]].option_4);

			while (entered_option < 1 || entered_option > 4) {
				printf("Please enter your answer (1, 2, 3 or 4): ");
				scanf("%d", &entered_option);
			}

			if (entered_option == divided_questions[q][sequence[j]].answer) {
				printf("Your answer is correct!\n");
				total_score += SCORE_POINTS;
				correct_answers_count++;
			} else {
				printf("Sorry, your answer is incorrect. The correct answer is %d.\n", divided_questions[q][sequence[j]].answer);
				wrong_answers_count++;
			}
			// Resetting entered_option
			entered_option = -1;
		}

		printf("You have completed this section. Now you'll be moving to the next section.\n");
	}

	printf("\n\n======================================\n");
	printf("======================================\n");
	printf("Thanks for taking the quiz. You've answered %d questions correctly and %d questions incorrectly.\nYour total score is %d.\n", correct_answers_count, wrong_answers_count, total_score);
	printf("======================================\n");
	printf("======================================\n");

	return 0;
}
