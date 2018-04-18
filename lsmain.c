#include <stdlib.h>
#include <stdio.h>

#include "myls.h"

int main(int argc, char* argv[]) {
   char* pathname = argv[1];
   myls(pathname);
   exit(EXIT_SUCCESS);
}
