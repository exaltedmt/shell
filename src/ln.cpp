#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <cstdlib>
#include <climits>
#include <string>
#include <iostream>
#include <cstring>

using std::string;
using std::cout;
using std::endl;

/**
 * Recreate the ln program
 * @author Dustin Nguyen
 * @author Royce Harley
 * @author Trevin Metcalf
 */

/**
 * Print a custom error message and exit the program.
 */
void error_out(const string & error = "") {
  cout << error << endl;
  exit(EXIT_SUCCESS);
} // error_out

/**
 * Print out the errno, with an optional message beforehand.
 */
void strerror_out(const string & error = "") {
  cout << error << strerror(errno) << endl;
  exit(EXIT_SUCCESS);
} // error_out

/**
 * Handles file linking and error checking.
 */
void lnk(string & src, string & dest, struct stat & stats, const bool & s) {
  DIR * dir; size_t pos;

  /**
   * Force . and .. to absolute paths.
   */

  if (src == "." || src == "./") {
    char buf [PATH_MAX];
    getcwd(buf, PATH_MAX);
    src = buf;
  } // if
  else if (src == ".." || src == "../") {
    char buf [PATH_MAX];
    getcwd(buf, PATH_MAX);
    src = buf;
    pos = src.find_last_of("/");
    src = src.substr(0,pos);
  } // else if

  if (dest == "." || dest == "./") {
    char buf [PATH_MAX];
    getcwd(buf, PATH_MAX);
    dest = buf;
  } // if
  else if (dest == ".." || dest == "../") {
    char buf [PATH_MAX];
    getcwd(buf, PATH_MAX);
    dest = buf;
    pos = src.find_last_of("/");
    dest = src.substr(0,pos);
  } // else if

  // If not creating a symlink and the source file is a dir, error out.
  if (!s && (dir = opendir(src.c_str())) != NULL) error_out("ln: `" + src + "': hard link not allowed for directory");
  

  if ((dir = opendir(dest.c_str())) != NULL) {
    closedir(dir);
    /**
     * Format the new path appropriately.
     */
    if ((dir = opendir(src.c_str())) != NULL) {
      if (src[src.length()-1] == '/') src = src.substr(0,src.length()-1);
      if (dest[dest.length()-1] != '/') dest += "/";
      if ((pos = src.find_last_of("/")) != std::string::npos) dest += src.substr(pos+1);
      else dest += src;
      symlink(src.c_str(), dest.c_str());
    } // if
    else if (dir == NULL) {
      if (src[src.length()-1] == '/') src = src.substr(0,src.length()-1);
      if (dest[dest.length()-1] != '/') dest += "/";
      if ((pos = src.find_last_of("/")) != std::string::npos) dest += src.substr(pos+1);
      else dest += src;
      if (s) symlink(src.c_str(), dest.c_str());
      else link(src.c_str(), dest.c_str());
    } // else if
    closedir(dir);
  } // if
  
  else if ((dir = opendir(dest.c_str())) == NULL) {
    closedir(dir);
    if ((dir = opendir(src.c_str())) != NULL) {
      if (stat(dest.c_str(),&stats) == -1) symlink(src.c_str(),dest.c_str());
      else error_out("ln: creating symbolic link `" + dest + "': file exists");
    } // if
    else if (dir == NULL) {
      if (stat(dest.c_str(),&stats) == -1) {
	if (s) symlink(src.c_str(),dest.c_str());
	else link(src.c_str(),dest.c_str());
      } // if
      else if (s) error_out("ln: creating symbolic link `" + dest + "': file exists");
      else error_out("ln: creating hard link `" + dest + "': file exists");
    } // else if
    closedir(dir);
  } // else if
} // lnk

int main(int argc, char ** argv) {
  bool s = false;
  const char * opstring = "s";
  char option;
  string src = "", dest = "";
  struct stat stats;  

  // check option flags
  while ((option = getopt(argc,argv,opstring)) != -1) {
    switch(option) {
    case 's':
      s = true;
      break;
    default:
      exit(EXIT_FAILURE);
      break;
    } // switch
  } // while  
  
  for (int index = 1; index < argc; index++) {
    if (argv[index][0] == '-') continue;
    if (src == "") src = argv[index];
    if (src != "") dest = argv[index];
  } // for
  
    if (stat(src.c_str(),&stats) == -1) strerror_out();
    lnk(src,dest,stats,s);
} // main
