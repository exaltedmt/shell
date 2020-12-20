#include <ctime>
#include <string>
#include <cstring>
#include <unistd.h>
#include <iostream>

using std::cout;
using std::endl;
using std::string;

/**
 * Recreates the date program
 * @author Dustin Nguyen
 * @author Trevin Metcalf
 * @author Royce Harley
 */

/**
 * After providing a custom error message, exit the program.
 */
void error_out(const string & error) {
  cout << error << endl;
  exit(EXIT_SUCCESS);
} // error_out

int main (int argc, const char ** argv) {
  
  cout.setf(std::ios::unitbuf); // unbuffered io
  char nl[1] = {'\n'};
  char * buf;
  time_t t;
  time(&t); // getting time
  string format;
  const struct tm * timeptr = localtime(&t); // converting to local time

  if (argc > 2) {
    format = argv[2];
    error_out("date: extra operand `" + format + "'");
  } // if
  else if (argc == 1) {
    buf = new char [1024];
    format = "%a %b %d %T %Z %Y"; // DEFAULT FORMAT
    strftime(buf, 1024, format.c_str(), timeptr);
    write(STDOUT_FILENO, buf, strlen(buf));
    write(STDOUT_FILENO, nl, 1);
    delete[] buf;
    exit(EXIT_SUCCESS);
  } // if

  if (argv[1][0] == '+') {
    buf = new char [1024];
    format = argv[1];
    format = format.substr(1); // skip the +
    strftime(buf, 1024, format.c_str(),timeptr);
    write(STDOUT_FILENO, buf, strlen(buf));
    write(STDOUT_FILENO, nl, 1);
    delete[] buf;
  } // if
  else if (argv[1][0] != '+') {
    format = argv[1];
    error_out("date: invalid date `" + format + "'");
  } // else if

  return EXIT_SUCCESS;
} // main
