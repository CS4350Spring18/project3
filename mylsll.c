#include "mylsll.h"


void insert(Node **headPtr, char *data, size_t data_size) {
   if (new_data == NULL) return;

   // Allocate memory for the new node
   Node* new_node = (Node*) malloc(sizeof(Node));
   new_node->data = (char*) malloc(data_size);
   new_node->next = NULL;
   strncpy(new_node->data, new_data, data_size);

   // empty LL
   if (*headPtr == NULL) *headPtr = new_node;
   // find where to add the new_node
   else if (strcmp((*headPtr)->data, new_data) > 0) {
      new_node->next = *headPtr;
      *headPtr = new_node;
   }
   else {
      Node* node = *headPtr;
      // Find the insertion spot
      while (node->next != NULL && (strcmp(node->next->data, new_data) < 0))
         node=node->next;

      // Update links
      new_node->next = node->next;
      node->next = new_node;
   }
}

void printList(Node *node, void (*fptr)(void *)) {
   while (node != NULL) {
      (*fptr)(node->data);
      node = node->next;
   }
}

void freeNodes(Node *node) {
   if (node == NULL) return;
   free(node->data);
   freeNodes(node->next);
   free(node);
}

void printLine(void *s) {
   printf("%s\n", (char *)s);
}


/* Driver program to test above function
int main() {
   Node *start = NULL;

   // Test strings
   char * file;
   for (int i = 4; i >= 0; i--) {
      sprintf(file, "string %d", i);
      insert(&start, file, sizeof(char) * strlen(file));
   }
   printf("\n\nCreated string linked list is \n");
   printList(start, printLine);
   freeNodes(start);

   return 0;
}
*/

