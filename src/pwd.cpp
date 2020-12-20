#include <unistd.h>
#include <climits>
#include <cstdlib>
#include <cstring>

/**
 * Recreate the pwd program
 * @author Dustin Nguyen
 * @author Trevin Metcalf
 * @author Royce Harley
 */

int main() {
  char * buf = get_current_dir_name();
  char nl[1] = {'\n'};
  write(STDOUT_FILENO, buf, strlen(buf));
  write(STDOUT_FILENO, nl, 1);
} // main
