#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_QUESTIONS 100
#define SCORE_POINTS 10
#define CATEGORY_MAX_QUESTIONS 4
#define CATEGORY_COUNT 3
#define CATEGORY_NAMES {"General Knowledge", "Music", "Sports"}

void shuffle_array(int* array, int array_size);

struct question {
	int category;
	char *statement;
	int answer;
	// size of options is 5 because we are ignoring the zeroeth element and starting from 1
	char *options[5];
};

int main() {

	// For readin line from file
	ssize_t read_size;
	size_t len = 0;

	char *temp_line = NULL;
	FILE *questions_file;
	struct question questions[MAX_QUESTIONS];
	unsigned int questions_read = 0;
	char* question_categories[CATEGORY_COUNT] = CATEGORY_NAMES;
	int all_entered_answers[CATEGORY_COUNT][CATEGORY_MAX_QUESTIONS];
	char restart_key = '\0';

	// Time for seed of random number
	time_t time_value;
	// Initializing random number generator
	srand((unsigned) time(&time_value));

	questions_file = fopen("questions_v2.txt", "r");

	if (questions_file) {
		char *temp_char;
		for (int i = 0; i < MAX_QUESTIONS; i++) {
			// Reading the category of the question
			if((read_size = getline(&temp_char, &len, questions_file)) == -1) {
				printf("ERROR: Reached end of questions_file without reading the category.\n");
				break;
			} else {
				questions[i].category = *temp_char - '0';
			}

			// Reading the question
			if((read_size = getline(&questions[i].statement, &len, questions_file)) == -1) {
				printf("ERROR: Reached end of questions_file without reading the question.\n");
				break;
			}

			// Reading the correct answer
			if((read_size = getline(&temp_char, &len, questions_file)) == -1) {
				printf("ERROR: Reached end of questions_file without reading the correct answer.\n");
				break;
			} else {
				questions[i].answer = *temp_char - '0';
			}

			// Reading option 1
			for(int t = 1; t <= 4; t++) {
				if((read_size = getline(&questions[i].options[t], &len, questions_file)) == -1) {
					printf("ERROR: Reached end of questions_file without reading all options.\n");
					break;
				}
			}

			questions_read++;

			// Reading empty space (this also serves as a format check)
			if((read_size = getline(&temp_line, &len, questions_file)) == -1) {
				printf("Questions loaded successfully. Your Quiz beings now.\n");
				break;
			} else {
				// 10 is the newline character thingy. At least in linux, don't know about windows.
				if (*temp_line != 10) {
					printf("ERROR: Something is wrong with the formatting of the questions_file.\n");
					printf("\tGot %d instead of empty line.\n", *temp_line);
				}
			}
		}
	}

	fclose(questions_file);

	do {
		// We will seperate the questions into different array based on the sections
		struct question divided_questions[CATEGORY_COUNT][CATEGORY_MAX_QUESTIONS];
		int divided_questions_sizes[CATEGORY_COUNT];
		int category_sequences[CATEGORY_COUNT][CATEGORY_MAX_QUESTIONS];

		for (int j = 0; j < CATEGORY_COUNT; j++) {
			int temp_category_size = 0;
			for (int i = 0; i < questions_read; i++) {
				if (questions[i].category == j+1 && temp_category_size < CATEGORY_MAX_QUESTIONS) {
					divided_questions[j][temp_category_size] = questions[i];
					temp_category_size++;
				}
			}
			divided_questions_sizes[j] = temp_category_size;
			shuffle_array(category_sequences[j], temp_category_size);
		}

		int category_wise_score[CATEGORY_COUNT];
		int category_correct_answers_count[CATEGORY_COUNT];
		int total_correct_answers_count = 0;
		int total_score = 0;

		for (int q = 0; q < CATEGORY_COUNT; q++) {
			printf("\n=================== %s section =============================\n", question_categories[q]);

			for(int j = 0; j < divided_questions_sizes[q]; j++) {
				struct question current_question = divided_questions[q][category_sequences[q][j]];

				printf("\nQuestion: %s", current_question.statement);
				printf("\t1) %s", current_question.options[1]);
				printf("\t2) %s", current_question.options[2]);
				printf("\t3) %s", current_question.options[3]);
				printf("\t4) %s", current_question.options[4]);

				printf("Please enter your answer: ");
				while (scanf("%d",&all_entered_answers[q][j]) != 1 || all_entered_answers[q][j] < 1 || all_entered_answers[q][j]  > 4) {
					printf("Please enter a valid option (1, 2, 3 or 4): ");
					// Asterisk * tells scanf to read and ignore the value
					scanf("%*s");
				}

				if (all_entered_answers[q][j] == divided_questions[q][category_sequences[q][j]].answer) {
					category_wise_score[q] += SCORE_POINTS;
					category_correct_answers_count[q]++;
					total_correct_answers_count++;
				}
			}

			printf("You have completed this category successfully!\n");
		}

		printf("\n\n================================================================================\n");
		printf("Congratulations. You have completed the quiz.\n");
		printf("You have got %d correct out of %d questions.\n", total_correct_answers_count, questions_read);

		printf("\nHere are your answers:\n");

		for (int q = 0; q < CATEGORY_COUNT; q++) {
			printf("\nQuestion Category: %s", question_categories[q]);
			for(int j = 0; j < divided_questions_sizes[q]; j++) {
				struct question current_question = divided_questions[q][category_sequences[q][j]];
				printf("\nQuestion: %s", current_question.statement);
				printf("\tCorrect Answer: %s", current_question.options[current_question.answer]);
				printf("\tYour Answer: %s", current_question.options[all_entered_answers[q][j]]);
				printf("\tMarks Awarded: %d\n", (all_entered_answers[q][j] == current_question.answer) ? SCORE_POINTS : 0);
			}

			total_score += category_wise_score[q];
		}

		printf("\nCategory Wise Marks:\n");
		for (int m = 0; m < CATEGORY_COUNT; m++){
			printf("\t%s: %d\n", question_categories[m], category_wise_score[m]);
		}

		printf("Your Final Total Score: %d out of %d\n", total_score, questions_read * SCORE_POINTS);

		// Reading out any characters from stream.
		scanf("%*s");
		printf("Press any key to go to main menu.\n");
		scanf("%c", &restart_key);
	} while (restart_key != '\0');

	return 0;
}

// Takes an empty array, initialized it with sequential numbers and then shuffles the numbers
void shuffle_array(int* array, int array_size) {
	// Populating the array with sequential numbers starting from 0
	for (int i = 0; i < array_size; i++) {
		array[i] = i;
	}

	// Shuffling the elements of the array
	for (int i = 0; i < array_size; i++) {
		int temp_element = array[i];
		int random_index = rand() % array_size;
		array[i] = array[random_index];
		array[random_index] = temp_element;
	}
}
