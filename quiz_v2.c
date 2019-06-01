#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_QUESTIONS 100
#define SCORE_POINTS 10
#define MAX_SECTION_SIZE 5
#define CATEGORY_COUNT 3
#define CATEGORY_NAMES {"General Knowledge", "Music", "Sports"}

void shuffle_array(int* array, int array_size);

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

	// For readin line from file
	ssize_t read_size;
	size_t len = 0;

	char *temp_line = NULL;
	FILE *questions_file;
	struct question questions[MAX_QUESTIONS];
	unsigned int questions_read = 0;
	/* int entered_option = -1; */
	char* question_categories[CATEGORY_COUNT] = CATEGORY_NAMES;

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
			if((read_size = getline(&questions[i].option_1, &len, questions_file)) == -1) {
				printf("ERROR: Reached end of questions_file without reading option_1.\n");
				break;
			}

			// Reading option 2
			if((read_size = getline(&questions[i].option_2, &len, questions_file)) == -1) {
				printf("ERROR: Reached end of questions_file without reading option_2.\n");
				break;
			}

			// Reading option 3
			if((read_size = getline(&questions[i].option_3, &len, questions_file)) == -1) {
				printf("ERROR: Reached end of questions_file without reading option_3.\n");
				break;
			}

			// Reading option 4
			if((read_size = getline(&questions[i].option_4, &len, questions_file)) == -1) {
				// completed reading a question along with all the options
				printf("ERROR: Reached end of questions_file without reading option_4.\n");
				break;
			}

			questions_read++;

			// Reading empty space (this also serves as a format check)
			if((read_size = getline(&temp_line, &len, questions_file)) == -1) {
				printf("Questions loaded successfully. Your Quiz beings now.\n");
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

	// We will seperate the questions into different array based on the sections
	struct question divided_questions[CATEGORY_COUNT][MAX_SECTION_SIZE];
	int divided_questions_sizes[CATEGORY_COUNT];
	int category_sequences[CATEGORY_COUNT][MAX_SECTION_SIZE];

	for (int j = 0; j < CATEGORY_COUNT; j++) {
		struct question temp_category_questions[MAX_SECTION_SIZE];
		int temp_category_size = 0;
		for (int i = 0; i < questions_read; i++) {
			if (questions[i].category == j+1) {
				divided_questions[j][temp_category_size] = questions[i];
				temp_category_size++;
			}
		}
		divided_questions_sizes[j] = temp_category_size;
		/* int temp_sequence[temp_category_size]; */
		/* shuffle_array(&temp_sequence, temp_category_size); */
		/* category_sequences[j] = &temp_sequence; */
		shuffle_array(category_sequences[j], temp_category_size);
	}

	/* printf("printgin\n"); */
	/* for (int t=0; t<CATEGORY_COUNT; t++){ */
		/* for (int i=0; i<divided_questions_sizes[0]; i++){ */
			/* printf("%d, ",category_sequences[0][i]); */
		/* } */
		/* printf("\n"); */
	/* } */
	int all_entered_answers[CATEGORY_COUNT][MAX_SECTION_SIZE];

	for (int q = 0; q < CATEGORY_COUNT; q++) {
		printf("\n=================== %s section =============================\n", question_categories[q]);

		for(int j = 0; j < divided_questions_sizes[q]; j++) {
			printf("\nQuestion %d: %s", j+1, divided_questions[q][category_sequences[q][j]].statement);
			printf("\t1) %s", divided_questions[q][category_sequences[q][j]].option_1);
			printf("\t2) %s", divided_questions[q][category_sequences[q][j]].option_2);
			printf("\t3) %s", divided_questions[q][category_sequences[q][j]].option_3);
			printf("\t4) %s", divided_questions[q][category_sequences[q][j]].option_4);

			do {
				printf("Please enter your answer (1, 2, 3 or 4): ");
				scanf("%d", &all_entered_answers[q][j]);
			} while (all_entered_answers[q][j] < 1 || all_entered_answers[q][j]  > 4);

			/* if (entered_option == divided_questions[q][category_sequences[q][j]].answer) { */
				/* printf("Your answer is correct!\n"); */
				/* total_score += SCORE_POINTS; */
				/* correct_answers_count++; */
			/* } else { */
				/* printf("Sorry, your answer is incorrect. The correct answer is %d.\n", divided_questions[q][category_sequences[q][j]].answer); */
				/* wrong_answers_count++; */
			/* } */

			// Resetting entered_option
			/* entered_option = -1; */
		}

		printf("You have completed this section successfully!\n");
	}

	printf("\n\n================================================================================\n");
	printf("Your quiz is complete. Thanks for taking the quiz.\n");

	printf("\n================================================================================\n");
	printf("Section wise analysis:\n\n");

	for (int q = 0; q < CATEGORY_COUNT; q++) {
		unsigned int total_score = 0;
		unsigned int correct_answers_count = 0;
		unsigned int wrong_answers_count = 0;

		printf("\n=================== %s section =============================\n", question_categories[q]);
		for(int j = 0; j < divided_questions_sizes[q]; j++) {
			printf("\nQuestion %d: %s", j+1, divided_questions[q][category_sequences[q][j]].statement);
			printf("\t1) %s", divided_questions[q][category_sequences[q][j]].option_1);
			printf("\t2) %s", divided_questions[q][category_sequences[q][j]].option_2);
			printf("\t3) %s", divided_questions[q][category_sequences[q][j]].option_3);
			printf("\t4) %s", divided_questions[q][category_sequences[q][j]].option_4);

			if (all_entered_answers[q][j] == divided_questions[q][category_sequences[q][j]].answer) {
				printf("Your answer %d is correct!\n", all_entered_answers[q][j]);
				total_score += SCORE_POINTS;
				correct_answers_count++;
			} else {
				printf("Sorry, your answer %d is incorrect. The correct answer is %d.\n", all_entered_answers[q][j], divided_questions[q][category_sequences[q][j]].answer);
				wrong_answers_count++;
			}
		}

		printf("================================================================================\n");
		printf("You've answered %d questions correctly and %d questions incorrectly in this section.\n", correct_answers_count, wrong_answers_count);
		printf("You've scored %d points in this section\n", total_score);

	}

	return 0;
}

// Takes an empty array, initialized it with sequential numbers and then shuffles the numbers
void shuffle_array(int* array, int array_size) {
	/* printf("\ngtot array\n"); */
	/* for(int k=0; k<array_size;k++){ */
		/* printf("%d, ", array[k]); */
	/* } */
	// Time for seed of random number
	time_t time_value;
	// Initializing random number generator
	srand((unsigned) time(&time_value));

	// Populating the array
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

	/* printf("\nchanged to gtot array\n"); */
	/* for(int k=0; k<array_size;k++){ */
		/* printf("%d, ", array[k]); */
	/* } */
	/* printf("done\n\n"); */
}
