#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <cstring>

/**
 * Replicates the env program.
 * @author Dustin Nguyen
 * @author Royce Harley
 * @author Treven Metcalf
 */

int main (const int argc, const char * argv[], const char * envp[]) {
  char newline[1] = {'\n'};

  // check each env var and print
  for (const char ** env = envp; *env != nullptr; ++env) {
    write(STDOUT_FILENO, *env, strlen(*env));
    write(STDOUT_FILENO, newline, 1);
  } // for
  return EXIT_SUCCESS;
} // main
