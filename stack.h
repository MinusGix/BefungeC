#ifndef MSTACK_H_
#define MSTACK_H_

#include <stdlib.h>

// Creates a structure which holds a pointer to another stacknode and some int data
struct StackNode {
	struct StackNode* next;
	int data;
};

struct StackNode* createNode (int data);

void push (struct StackNode** root, int data);

int isEmpty (struct StackNode* root);

int pop (struct StackNode** root);

int peek (struct StackNode* root);

void peekPreview (struct StackNode* root, int* peekView, int amount);

#endif // MSTACK_H_