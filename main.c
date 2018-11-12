#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <strings.h>

#include "stack.h"

#define BOARD_WIDTH 80
#define BOARD_HEIGHT 30

#define COLOR_RED "\x1b[32m"
#define COLOR_RESET "\x1b[0m"
#define COLOR_SELECTED "\x1b[1m\x1b[43m"
#define CLEAR "\x1b[2J\x1b[;H"

// Microseconds, would prefer milliseconds
#define SLEEP_TIME 200000

// A structure to represent a stack 
enum Direction { UP, LEFT, RIGHT, DOWN };

// Global variables.. it might've been better to make it not global if this was meant to be used for various things.
int drawCount = 0;
int x = 0;
int y = 0;
enum Direction dir = RIGHT;

// Wraps xPos and yPos if they get too large
void wrapPos () {
	if (x >= BOARD_WIDTH) {
		x = 0;
	}

	if (x < 0) {
		x = BOARD_WIDTH - 1;
	}

	if (y >= BOARD_HEIGHT) {
		y = 0;
	}

	if (y < 0) {
		y = BOARD_HEIGHT - 1;
	}
}

void draw (char (*board)[BOARD_WIDTH][BOARD_HEIGHT], char outputString[BOARD_WIDTH], int *peekView) {
	//printf(CLEAR);

	drawCount++;

	printf(COLOR_RED "Drawn: %d\n" COLOR_RESET, drawCount);
	printf("Output: %s\n", outputString);

	for (int dy = 0; dy < BOARD_HEIGHT; dy++) {
		for (int dx = 0; dx < BOARD_WIDTH; dx++) {
			printf("|");
			char res = (char)(*board)[dx][dy];

			if (res == '\0' || ((int)res) == 0) {
				res = ' ';
			}
			
			if (dy == y && dx == x) {
				printf(COLOR_SELECTED "%c" COLOR_RESET, res);
			} else {
				printf("%c", res);
			}
		}
		
		if (dy == 0) {
			printf("| |_STACK_|");
		} else {
			// <= 12 because when dy = 0 it is 'stack', so we need to do 1-12 not 0-11
			if (dy <= 12) {
				// -1, because first stack item starts at dy == 1, so we need to subtract one
				printf("| |  %03d  |", peekView[dy - 1]);
			} else {
				printf("| |-------|");
			}
		}
		printf("\n");
	}
}

void moveNext () {
	if (dir == UP) {
		y = y - 1;
	} else if (dir == LEFT) {
		x = x - 1;
	} else if (dir == RIGHT) {
		x = x + 1;
	} else if (dir == DOWN) {
		y = y + 1;
	}
}

void addToOutput (char text, char *outputString, int *outputPos) {
	printf("communism, %c\n", text);
	//printf("Adding to output string: %c\n", text);
	outputString[*outputPos] = text;
	//printf("output string: %s\n", outputString);
	if ((*outputPos) >= (BOARD_WIDTH-1)) {
		*outputPos = 0;
	} else {
		*outputPos = *outputPos + 1;
	}

}

char currentTickCMD;
void tickDisplay (char (*board)[BOARD_WIDTH][BOARD_HEIGHT], char *outputString, int *outputPos, struct StackNode *stack, int *peekView) {
	wrapPos();
	
	usleep(SLEEP_TIME);

	currentTickCMD = (*board)[x][y];
		
	if (currentTickCMD == 'v') {
		dir = DOWN;
	} else if (currentTickCMD == '<') {
		dir = LEFT;
	} else if (currentTickCMD == '>') {
		dir = RIGHT;
	} else if (currentTickCMD == '^') {
		dir = UP;
	} else if (currentTickCMD == '0' || currentTickCMD == '1' || currentTickCMD == '2' || currentTickCMD == '3' || currentTickCMD == '4' || currentTickCMD == '5' || currentTickCMD == '6' || currentTickCMD == '7' || currentTickCMD == '8' || currentTickCMD == '9') {
		int val;
		// While we could use some function to parse the numbers, they're only these digits so we can just do this
		// Faster, and I couldn't be arsed to learn what the function name was
		switch (currentTickCMD) {
			case '0':
				val = 0;
				break;
			case '1':
				val = 1;
				break;
			case '2':
				val = 2;
				break;
			case '3':
				val = 3;
				break;
			case '4':
				val = 4;
				break;
			case '5':
				val = 5;
				break;
			case '6':
				val = 6;
				break;
			case '7':
				val = 7;
				break;
			case '8':
				val = 8;
				break;
			case '9':
				val = 9;
				break;
		}

		push(&stack, val);
	} else if (currentTickCMD == ',') {
		addToOutput(pop(&stack), outputString, outputPos);
	} else if (currentTickCMD == '*') {
		int firstVal = pop(&stack);
		int secondVal = pop(&stack);

		push(&stack, firstVal * secondVal);
	} else if (currentTickCMD == '+') {
		int firstVal = pop(&stack);
		int secondVal = pop(&stack);

		push(&stack, firstVal + secondVal);
	} else if (currentTickCMD == '-') { // "Subtraction: Pop two values a and b, then push the result of b-a"
		int firstVal = pop(&stack);
		int secondVal = pop(&stack);

		push(&stack, secondVal - firstVal);
	} else if (currentTickCMD == '/') {
		int firstVal = pop(&stack);
		int secondVal = pop(&stack);
		// actually implement this:
		// Integer division: Pop two values a and b, then push the result of b/a, rounded down. According to the specifications, if a is zero, ask the user what result they want.
		push(&stack, secondVal / firstVal);
	} else if (currentTickCMD == '%') {
		int firstVal = pop(&stack);
		int secondVal = pop(&stack);

		push(&stack, secondVal % firstVal);
	} else if (currentTickCMD == '!') {
		int value = pop(&stack);

		if (value == 0) {
			push(&stack, 1);
		} else {
			push(&stack, 0);
		}
	} else if (currentTickCMD == '`') {
		int firstVal = pop(&stack);
		int secondVal = pop(&stack);

		if (secondVal > firstVal) {
			push(&stack, 1);
		} else {
			push(&stack, 0);
		}
	} else if (currentTickCMD == '?') {
		// Random Direction
		// TODO: make this
	} else if (currentTickCMD == '_') {
		int value = pop(&stack);

		if (value == 0) {
			dir = RIGHT;
		} else {
			dir = LEFT;
		}
	} else if (currentTickCMD == '|') {
		int value = pop(&stack);

		if (value == 0) {
			dir = DOWN;
		} else {
			dir = UP;
		}
	} else if (currentTickCMD == '"') {
		// TODO: implement string mode (push char ascii value all the way up to the next ")
	} else if (currentTickCMD == ':') {
		int value = peek(stack);

		push(&stack, value);
	} else if (currentTickCMD == '\\') {
		int firstVal = pop(&stack);
		int secondVal = pop(&stack);

		push(&stack, secondVal);
		push(&stack, firstVal);
	} else if(currentTickCMD == '$') {
		pop(&stack);
	} else if (currentTickCMD == '.') {
		int value = pop(&stack);
		char targetString[3];

		// This seemed to be the 'best' way to convert an int to a string
		snprintf(targetString, 3, "%d", value);
		// Bleh
		for (int i = 0; i < 3; i++) {
			if (targetString[i] != '\0') {
				addToOutput(targetString[i], outputString, outputPos);
			}
		}
	} else if (currentTickCMD == '#') {
		moveNext(); // Skip over one
	} else if (currentTickCMD == 'g') {
		int gy = pop(&stack);
		int gx = pop(&stack);
		int result;

		if (gy >= BOARD_HEIGHT ||
			gy < 0 ||
			gx >= BOARD_WIDTH ||
			gx < 0) {
				result = 0;
		} else {
			result = (*board)[gx][gy];
		}

		push(&stack, result);
	} else if (currentTickCMD == 'p') {
		int gy = pop(&stack);
		int gx = pop(&stack);
		// Char to be put
		char gv = (int)pop(&stack);

		if (gy >= BOARD_HEIGHT ||
			gy < 0 ||
			gx >= BOARD_WIDTH ||
			gx < 0) {
				// Noop, currently
		} else {
			(*board)[gx][gy] = gv;
		}
	} else if (currentTickCMD == '&') {
		// Implement this
	} else if (currentTickCMD == '~') {
		// Implement this
	} else if (currentTickCMD == '@') {
		// We can just return, as tickDisplay is recursive. (Ya know, I should probably change that but this works for now)
		return;
	} else {
		// Beep
	}

	// Shouldn't hardcode the size
	peekPreview(stack, peekView, 12);	

	draw(board, outputString, peekView);

	moveNext();

	tickDisplay(board, outputString, outputPos, stack, peekView);
}


int main () {
	char board[BOARD_WIDTH][BOARD_HEIGHT];

	for (int xPos = 0; xPos < BOARD_WIDTH; xPos++) {
		for (int yPos = 0; yPos < BOARD_HEIGHT; yPos++) {
			board[xPos][yPos] = ' ';
		}
	}

	board[1][0] = 'v';
	board[1][2] = '7';
	board[1][3] = '9';
	board[1][4] = '*';
	board[1][6] = '2';
	board[1][7] = '+';
	board[1][8] = ',';
	board[1][9] = '>';

	board[2][9] = '7';
	board[3][9] = '9';
	board[4][9] = '*';
	board[5][9] = '3';
	board[6][9] = '+';
	board[7][9] = ',';
	
	board[8][9] = '^';
	board[8][8] = '9';
	board[8][7] = '7';
	board[8][6] = '*';
	board[8][5] = '4';
	board[8][4] = '+';
	board[8][2] = ',';
	board[8][0] = '<';

	// A String we'll display with the output data
	char outputString[BOARD_WIDTH+1];
	bzero(outputString, sizeof(char) * BOARD_WIDTH);
	outputString[BOARD_WIDTH] = '\0';

	// Current position in outputString
	int outputPos = 0;

	struct StackNode* main_stack = NULL;
	int peekView[12];
	bzero(peekView, sizeof(int) * 12);

	printf("Starting\n");
	printf("Board Dimensions: %dx%d\n", BOARD_WIDTH, BOARD_HEIGHT);
	//printPosition();

	//push(&main_stack, 10);
	//push(&main_stack, 50);
	//push(&main_stack, 93);

	//printf("%d popped from stack\n", pop(&main_stack));
	//printf("Top element is %d\n", peek(main_stack));

	tickDisplay(&board, outputString, &outputPos, main_stack, peekView);
	return 0;
}