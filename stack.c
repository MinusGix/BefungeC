#include "stack.h"
#include <stdio.h>

struct StackNode* createNode (int data) {
	struct StackNode* node = (struct StackNode*) calloc(1, sizeof(struct StackNode));
	node->next = NULL;
	node->data = data;

	return node;
}

// Takes a pointer to the pointer to root.
void push (struct StackNode** root, int data) {
	printf("adding %d to stack\n", data);
	// Constructs a new stacknode
	struct StackNode* node = createNode(data);
	// links it to root, so after this node is root 
	node->next = *root;
	// Sets the dereferenced pointer (so the original pointer) to this node.
	// This is done because the stack root should be the current top item in the stack (aka most recently added)
	// So, we create the stack, and normally store it as a StackNode*
	// A pointer to the memory address where StackNode lies
	// But here, we pass a pointer to that pointer, a StackNode**.
	// This is so we can change where that pointer actually refers to, and thus modify their ptr
	*root = node;
}

int isEmpty (struct StackNode* root) {
	return !root;
}

// Take the current stacknode, which would be the topmost item
int pop (struct StackNode** root) {
	// Dereference it into the pointer to the actual stacknode, and if it doesn't actually exist (so it's an 'empty' stacknode)
	if (isEmpty(*root)) {
		printf("root was empty");
		return 0; // Original impl used INT_MIN, but for my uses I prefer 0
	}

	printf("pop root value: %d\n", (*root)->data);
	// Store the last node temp
	struct StackNode* temp = *root;
	// Set the current pointer of root to the pointer for the next node
	*root = (*root)->next;

	// The integer that root stored
	int popped = temp->data;
	printf("popped value: %d\n", popped);

	// Free the data. I'm unsure why this doesn't free popped?
	// Is it because it's not passing a pointer, but rather temp->data?
	free(temp);

	return popped;
}

// Peek at the topmost value without removing it
int peek (struct StackNode* root) {
	if (isEmpty(root)) {
		return 0;
	}

	return root->data;
}

// Peek at a certain amount of values, and stuff them into an array.
// I imagine this could easily mess up if the amount given by a person using this was greater than the arrays (peekview) actual length
void peekPreview (struct StackNode* root, int* peekView, int amount) {
	// Store the current node, because this will be messed with
	struct StackNode* current = root;

	for (int i = 0; i < amount; i++) {
		if (isEmpty(current)) {
			peekView[i] = 0;
		} else {
			peekView[i] = peek(current);
			current = current->next;
		}
	}
}