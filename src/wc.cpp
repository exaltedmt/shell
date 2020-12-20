#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <cwchar>
#include <locale>
#include <cstdlib>
#include <clocale>

using namespace std;

/**
 * Recreate the wc program
 * @author Dustin Nguyen
 * @author Trevin Metcalf
 * @author Royce Harley
 */

/**
 * Print an error, but do not exit out of the program.
 */
void print_error(const string & error = "") {
  cout << error << strerror(errno) << endl;
} // error

/**
 * Read standard input
 */
void printToStd() {
  char * content = new char [1];
  int bytes;
  while ((bytes = read(STDIN_FILENO, content, sizeof(content))) > 0);
} // printToStd

int main(int argc, char ** argv) {
  bool c = false, m = false, l = false, w = false;
  const char * opstring = "cmlw";
  char option; 
  struct stat stats;
  setlocale(LC_ALL, "en_US.utf8");
  setvbuf(stdout, NULL, _IONBF, 1024);
  cout.setf(std::ios::unitbuf);

  // Check option flags
  while ((option = getopt(argc,argv,opstring)) != -1) {
    switch (option) {
    case 'c': 
      c = true;
      break;
    case 'm': 
      m = true;
      break;
    case 'l': 
      l = true;
      break;
    case 'w': 
      w = true;
      break;
    default:
      exit(EXIT_SUCCESS);
    } // switch
  } // while

  char * buf; int id;
  int * newlines = new int [argc];
  int * words = new int [argc];
  int * characters = new int [argc];
  long long * sizes = new long long [argc];
  int * padding = new int[argc];
  string * files = new string[argc];

  // Default values
  for (int index = 0; index < argc; index++) {
    newlines[index] = -1;
    characters[index] = -1;
    words[index] = -1;
    sizes[index] = -1;
    padding[index] = -1;
    files[index] = "";
  } // for
  
  if (argc == 1) printToStd(); // If we have no args, then read stdout

  for (int index = 1; index < argc; index++) {

    if (argv[index][0] == '-' && argv[index][1] != '\0' && index == argc-1) printToStd();
    else if (argv[index][0] == '-' && argv[index][1] != '\0') continue;
    files[index] = argv[index];

    /**
     * If the file exists and is not hyphen, run calculations.
     */

    if (stat(argv[index], &stats) != -1 && files[index] != "-") {

      newlines[index] = 0;
      characters[index] = 0;
      words[index] = 0;
      sizes[index] = stats.st_size;
      padding[index] = 1;
      buf = new char[stats.st_size+1];
      id = open(argv[index], O_RDONLY);
      read(id,buf,stats.st_size);
      
      const char * buffer = buf;
      wchar_t * widebuf = new wchar_t[stats.st_size+1];
      characters[index] = mbstowcs(widebuf,buffer,stats.st_size);

      for (int i = 0; i <= stats.st_size; i++) {
	if (widebuf[i] != '\0' && widebuf[i] != ' ' && widebuf[i] != '\n' && i == 0) words[index]++;
	else if ((widebuf[i] != '\0' && widebuf[i] != ' ' && widebuf[i] != '\n') && (widebuf[i-1] == ' ' || widebuf[i-1] == '\n')) words[index]++;
	else if (widebuf[i] == '\n') newlines[index]++;
      } // for
      
      int comparison = 10;
      while (characters[index] >= comparison || words[index] >= comparison || newlines[index] >= comparison || sizes[index] >= comparison) {
	comparison *= 10;
	padding[index]++;	
      } // while
      delete[] buffer;
      delete[] widebuf;
    } // if

    /**
     * If the files is hyphen, go to stdout.
     */
    else if (files[index] == "-") printToStd();
  } // for
  
  /**
   * Find the largest padding.
   */

  for (int index = 0; index < argc; index++) {
    for (int index2 = index+1; index2 < argc; index2++) {
      if (padding[index] > padding[index2]) {
	int temp = padding[index];
	padding[index] = padding[index2];
	padding[index2] = temp;
      } // if
    } // for
  } // for

  /**
   * Formatting output
   */

  for (int index = 0; index < argc; index++) {
    if (newlines[index] != -1) {
      if (l) printf("%*d ", padding[argc-1],newlines[index]);	
      if (w) printf("%*d ", padding[argc-1],words[index]);
      if (m) printf("%*d ", padding[argc-1],characters[index]);
      if (c) printf("%*lld ", padding[argc-1],sizes[index]);
      if (!l && !w && !m && !c) printf("%*d %*d %*lld ", padding[argc-1], newlines[index], padding[argc-1], words[index], padding[argc-1], sizes[index]);
      files[index] += "\n";
      write(STDOUT_FILENO, files[index].c_str(), files[index].length());
    } // if
    else if (files[index] != "") { 
      stat(files[index].c_str(),&stats);
      print_error("wc: " + files[index] + ": ");
    } // else
  } // for
  
  /**
   * Print totals if we have more than one file listed.
   */
  int count = 0, nltotal = 0, wordstotal = 0, chartotal = 0;
  long long sizestotal = 0;
  for (int index = 0; index < argc; index++) {
    if (files[index] != "") {
      count++;
      if (stat(files[index].c_str(),&stats) != -1) {
	nltotal += newlines[index];
	wordstotal += words[index];
	chartotal += characters[index];
	sizestotal += sizes[index];
      } // if
    } // if
  } // for

  if (count > 1) {
    if (l) printf("%*d ", padding[argc-1],nltotal);	
    if (w) printf("%*d ", padding[argc-1],wordstotal);
    if (m) printf("%*d ", padding[argc-1],chartotal);
    if (c) printf("%*lld ", padding[argc-1],sizestotal);
    if (!l && !w && !m && !c) printf("%*d %*d %*lld ", padding[argc-1], nltotal, padding[argc-1], wordstotal, padding[argc-1], sizestotal);
    const char t [8] = "total\n\0";
    write(STDOUT_FILENO, t, 8);
  } // if
  
  delete[] newlines;
  delete[] words;
  delete[] characters;
  delete[] sizes;
  delete[] padding;
  delete[] files;

} // main
