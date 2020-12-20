#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <locale>
#include <algorithm>
#include <pwd.h>
#include <grp.h>
#include <climits>
#include <ctime>
#include <cstring>
#include <cstdlib>

using std::copy;
using std::string;
using std::strerror;
using std::cout;
using std::endl;
using std::locale;
using std::tolower;


/**
 * Recreate the ls program
 * @author Dustin Nguyen
 * @author Royce Harley
 * @author Trevin Metcalf
 */

/**
 * Print an error, but do not exit.
 */
void print_error(const string & error = "") {
  cout << error << strerror(errno) << endl;
} // print_error

/**
 * Print an error and exit out of the program.
 */
void error_out(const string & error = "") {
  cout << error << strerror(errno) << endl;
  exit(EXIT_FAILURE);
} // error_out

/**
 * Calculate the string representing the symbolic representation of the file permissions
 */
string findperms(struct stat & stats) {
  char perm [11];

  // Check file type
  if (S_ISDIR(stats.st_mode)) perm[0] = 'd';
  else if (S_ISLNK(stats.st_mode)) perm[0] = 'l';
  else if (S_ISSOCK(stats.st_mode)) perm[0] = 's';
  else if (S_ISFIFO(stats.st_mode)) perm[0] = 'p';
  else if (S_ISCHR(stats.st_mode)) perm[0] = 'c';
  else if (S_ISBLK(stats.st_mode)) perm[0] = 'b';
  else perm[0] = '-';

  /**
   * Calculate the files user, group, and other permissions.
   */

  if ((stats.st_mode & S_IRUSR) == S_IRUSR) perm[1] = 'r';
  else perm[1] = '-';

  if ((stats.st_mode & S_IWUSR) == S_IWUSR) perm[2] = 'w';
  else perm[2] = '-';

  if ((stats.st_mode & S_ISUID) == S_ISUID && (stats.st_mode & S_IXUSR) == S_IXUSR) perm[3] = 's';
  else if ((stats.st_mode & S_ISUID) == S_ISUID) perm[3] = 'S';
  else if ((stats.st_mode & S_IXUSR) == S_IXUSR) perm[3] = 'x';
  else perm[3] = '-';

  if ((stats.st_mode & S_IRGRP) == S_IRGRP) perm[4] = 'r';
  else perm[4] = '-';

  if ((stats.st_mode & S_IWGRP) == S_IWGRP) perm[5] = 'w';
  else perm[5] = '-';

  if ((stats.st_mode & S_ISGID) == S_ISGID && (stats.st_mode & S_IXGRP) == S_IXGRP) perm[6] = 's';
  else if ((stats.st_mode & S_ISGID) == S_ISGID) perm[6] = 'S';
  else  if ((stats.st_mode & S_IXGRP) == S_IXGRP) perm[6] = 'x';
  else perm[6] = '-';

  if ((stats.st_mode & S_IROTH) == S_IROTH) perm[7] = 'r';
  else perm[7] = '-';

  if ((stats.st_mode & S_IWOTH) == S_IWOTH) perm[8] = 'w';
  else perm[8] = '-';
  
  if ((stats.st_mode & S_ISVTX) == S_ISVTX && (stats.st_mode & S_IXOTH) == S_IXOTH) perm[9] = 't';
  else if ((stats.st_mode & S_ISVTX) == S_ISVTX) perm[9] = 'T';
  else  if ((stats.st_mode & S_IXOTH) == S_IXOTH) perm[9] = 'x';
  else perm[9] = '-';

  perm[10] = '\0';
  return perm;
} // findperms

/**
 * Print the extended information of a file (-l flag)
 */
void print_info(const string * files, const int & arraySize, const bool & dir, const bool & a) {
  blkcnt_t total = 0;
  string * perms = new string[arraySize];
  string * usernames = new string[arraySize];
  string * groupnames = new string[arraySize];
  time_t * modtimes = new time_t[arraySize];
  nlink_t * links = new nlink_t [arraySize];
  off_t * bytes = new off_t [arraySize];
  struct passwd * pswd;
  struct group * grp;
  struct stat stats; 
  
  // Find all the information of the listed files
  for (int index = 0; index < arraySize; index++) {
    if (!a && files[index][0] == '.') continue;
    if (files[index] == "") continue;
    stat(files[index].c_str(),&stats);
    pswd = getpwuid(stats.st_uid);
    grp = getgrgid(stats.st_gid);
    total += stats.st_blocks/2;
    perms[index] = findperms(stats);
    links[index] = stats.st_nlink;
    usernames[index] = pswd->pw_name;
    groupnames[index] = grp->gr_name;
    modtimes[index] = stats.st_mtime;
    bytes[index] = stats.st_size;
  } // for
  
  if (dir) cout << "total " << total << endl;
  for (int index = 0; index < arraySize; index++) {
    if (!a && dir && files[index][0] == '.') continue;
    if (files[index] == "") continue;
    string t = ctime(&modtimes[index]);
    cout << perms[index] << ". " << links[index] << " " << usernames[index] << " " << groupnames[index] << " " 
	 << bytes[index] << " " << t.substr(0,t.length()-1) << " " << files[index] << endl;
  } // for

  delete[] perms;
  delete[] usernames;
  delete[] groupnames;
  delete[] links;
  delete[] modtimes;
  delete[] bytes;
} // print_info

/**
 * Adds an empty string to the list that should be ignored.
 */
void shortenlist(string * filelist, const int & index, const int & size) {
  filelist[index] = "";
} // shortenlist

/**
 * Swaps the postion of two files in the file lists
 */
void swap(string * filesLower, string * files, const int & comparedString, const int & originalString) {
  // sorts the lower-cased files (because the sorting algorithm relies on its own sorting)
  string templower = filesLower[comparedString];
  filesLower[comparedString] = filesLower[originalString];
  filesLower[originalString] = templower;
  // sorts the original files -- what is needed.
  string temp = files[comparedString];
  files[comparedString] = files[originalString];
  files[originalString] = temp;	  
} // swap

/**
 * Alphabetically sort files, and account for hidden files w/ multiple .'s
 */
void sort(string * files, const int & arraySize) { 
  locale loc;
  string * filesLower = new string[arraySize];  
  copy(files, files+(arraySize), filesLower);
  
  for (int i = 0; i < arraySize; i++) {
    for (size_t j = 0; j < filesLower[i].length(); j++) {
      filesLower[i][j] = tolower(filesLower[i][j],loc);
    } // for
  } // for

  for (int originalString = 0; originalString < arraySize; originalString++) {
    for (int comparedString = originalString+1; comparedString < arraySize; comparedString++) {        
      // Handle hidden file sorting...
      if (filesLower[originalString][0] == '.' || filesLower[comparedString][0] == '.') {
	int index = 0, index2 = 0;
	for (size_t i = 0; i < filesLower[originalString].length(); i++) {
	  if (filesLower[originalString][i] != '.') {
	    index = i;
	    i = filesLower[originalString].length(); // break out of loop
	  } // if
	} // for
       	for (size_t i = 0; i < filesLower[comparedString].length(); i++) {
	  if (filesLower[comparedString][i] != '.') {
	    index2 = i;
	    i = filesLower[comparedString].length(); // break out of loop
	  } // if
	} // for
	if (filesLower[comparedString].substr(index2) < filesLower[originalString].substr(index)) {
	  swap(filesLower,files,comparedString,originalString);
	} // if
	else if (filesLower[originalString].substr(index) == filesLower[comparedString].substr(index2) && filesLower[comparedString] < filesLower[originalString]) {
	  swap(filesLower,files,comparedString,originalString);
	} // else if
      } // if
      // Handle not-hidden file sorting...
      else if (filesLower[comparedString] < filesLower[originalString]) {
	swap(filesLower,files,comparedString,originalString);	  
      } // else  if
    } // for
  } // for

  delete [] filesLower;
} // sort

/**
 * Print the contents within a directory
 */
void printdir(const bool & a, const bool & l, const char * o_workingdir, const string & path = ".") {
  DIR * dir = opendir(path.c_str());
  struct dirent * read = readdir(dir);
  string * files; int arraySize;

  if (dir == NULL) error_out();
  for (arraySize = 1; read != NULL; arraySize++) {
    files = new string[arraySize];
    read = readdir(dir);
    if (read != NULL) delete [] files;
  } // for

  closedir(dir);
  dir = opendir(path.c_str()); // reset the dir stream
  read = readdir(dir);

  for (int index = 0; read != NULL; index++) {       
    if (read == NULL) files[index] = "\0";
    else files[index] = read->d_name;
    read = readdir(dir);
  } // for
  
  sort(files,arraySize-1);

  if (!l) {
    for (int index = 0; index < arraySize-1; index++) {
      if (files[index][0] == '.' && !a) continue;
      cout << files[index] << "  ";
    } // for
    cout << endl;
  } else {
    chdir(path.c_str());
    print_info(files,arraySize-1,true, a);
    chdir(o_workingdir);
  } // else

  closedir(dir);
  delete [] files;  
} // printdir

/**
 * Print a list of files/directories
 */
void printfilelist(string * filelist, char * o_workingdir, int size, const bool & a, const bool & l) {
  string * dirlist; DIR * dir;
  int dirlistsize = 0;
  sort(filelist,size);

  for (int index = 0; index < size; index++) {
    if ((dir = opendir(filelist[index].c_str())) != NULL) {
      closedir(dir);
      dirlistsize++;
    } // if
  } // for

  dirlist = new string[dirlistsize];
  
  for (int index = 0, dirindex = 0; index < size; index++) {
    if ((dir = opendir(filelist[index].c_str())) != NULL) {
      dirlist[dirindex] = filelist[index];
      dirindex++;
      shortenlist(filelist,index, size);
      index = -1; // reset
    }// if
  } // for

  if (l) {
    print_info(filelist,size,false,a);
    cout << endl;
  } else {
    for (int index = 0; index < size; index++) {
      if (filelist[index] == "") continue;
      cout << filelist[index] << "  ";
      if (index == size-1) cout << endl << endl;
    } // for
  } // else

  for (int index = 0; index < dirlistsize; index++) {
    cout << dirlist[index] << ": " << endl;
    printdir(a,l,o_workingdir,dirlist[index]);
    cout << endl;
  } // for

  delete [] dirlist;
  delete [] filelist;
} // printfilelist

int main(int argc, char ** argv) {  
  bool a = false, l = false, badarg = false;
  const char * opstring = "al";
  char option; string file;
  string * filelist; int filelistsize = 0;
  DIR * dir;

  cout.setf(std::ios::unitbuf); // set cout to unbuffered

  while ((option = getopt(argc,argv,opstring)) != -1) {
    switch (option) {
    case 'a':
      a = true; 
      break;
    case 'l':
      l = true;
      break;
    default:
      exit(EXIT_FAILURE);
      break;
    } // switch
  } // while
  
  for (int index = 1; index < argc; index++) {
    if (argv[index][0] == '-') continue;
    else if (access(argv[index], F_OK) == 0) filelistsize++;
    if (index == argc-1) filelist = new string[filelistsize];
  } // for

  for (int index = 1, filelistindex = 0; index < argc; index++) {
    file = argv[index];
    if (argv[index][0] == '-' && argv[index][1] == '\0') {
      access(argv[index], F_OK);
      error_out("ls: cannot access " + file + ": ");
    } // if
    else if (argv[index][0] == '-')  continue;
    else if (access(argv[index], F_OK) == 0) {
      filelist[filelistindex] = argv[index];
      filelistindex++;
    } else {
      print_error("ls: cannot access " + file + ": ");
       badarg = true;
    } // else
  } // for
  
  char o_workingdir [PATH_MAX]; // store the current working directory so that we can return to it after changing dirs
  getcwd(o_workingdir, PATH_MAX);  

  if (filelistsize == 0 && badarg) exit(EXIT_SUCCESS);
  else if (filelistsize == 0) printdir(a,l,o_workingdir);
  else if (filelistsize == 1 && (dir = opendir(filelist[0].c_str())) != NULL) printdir(a,l,o_workingdir,filelist[0]);
  else printfilelist(filelist, o_workingdir,filelistsize, a, l);

  return EXIT_SUCCESS;
} // main
