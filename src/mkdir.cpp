#include <iostream>
#include <string>
#include <cstdlib>
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <cstdlib>
#include <cstdio>
#include <sys/types.h>
#include <stdio.h>
#include <string>
#include <cstring>

using std::string;
using std::endl;
using std::cout;

#define BUFFSIZE 2048*10

void modal(mode_t modes, bool intermediate, const char* path);
void lengthy(string &num_hold, const char* argv);

int main(const int argc, const char* argv[]){ //an array of pointers to chars

    bool on_modes = false;
    bool intermediate = false;
    char * pEnd;
    long modes_l = 755;
    mode_t modes = 0755;
    string file_name;
    bool file_exists = false;

    //turn off buffering for cout
    cout.setf(std::ios::unitbuf);

    // no args
    if (argc < 2) {
        cout << "mkir: missing operand" << endl;
        cout << "Try 'mkdir --help' for more information." << endl;
        exit(EXIT_FAILURE);

    } //if

      //args
    else for(int i = 1; i < argc; ++i){

        //finds hyphen, checks for r or f after, make sure its not file
        if (argv[i][0] == '-' && argv[i][1]){ //dir cant exist if being made
            string command(argv[i]);

            if(command.at(1) == 'm'){ //change default mode

                if(argv[i+1]){ //an arg after n
                    on_modes = true;
                }

                else if(!argv[i+1]){ //no arg for -n
                    cout << "mkdir: option requires an argument -- \'n\'" << endl;
                    cout << "Try `mkdir --help' for more information." << endl;
                    exit(EXIT_FAILURE);
                }
            } // if -n

            else if(command.at(1) == 'p'){ //creates non existing sub directory + directory
 				intermediate = true;
            } //if -c

        } //if

        else if(on_modes){ //theres an arg after -n passed, only needed once

        	for(unsigned int c = 0; c < strlen(argv[i]); ++c){
        		if(argv[i][c] == '.'){
        			file_name = std::string(argv[i]);
                    cout << "mkdir: invalid mode `" << file_name << "\'" << endl;
                    exit(EXIT_FAILURE);
        		}
        	}

            if(strtol(argv[i], &pEnd, 10) == 0){ //check if number either actually a 0 or a file
                if(argv[i] == pEnd){ //determine if string instead of lines
                    file_name = std::string(argv[i]);
                    cout << "mkdir: invalid mode `" << file_name << "\'" << endl;
                    exit(EXIT_FAILURE);
                } //if string

                else modes = 0; //if string not equal lines is actually 0
            } //if lines

            else{
            		bool crasher = false;
                	modes_l = strtol(argv[i], &pEnd, 10); //take those 4 digits

                	string num_hold = std::to_string(modes_l);
                	lengthy(num_hold, argv[i]);

                	// highest already
                	if(modes_l > 7777){
                		crasher = true;
					}

					//check for 8s and 9s in long
					else if(modes_l / 1000 == 8 || modes_l /  1000 == 9 ||
							(modes_l % 1000) / 100 == 8 || (modes_l % 1000) / 100 == 9 ||
							(modes_l % 100) / 10 == 8 || (modes_l % 100) / 100 == 9 ||
							(modes_l % 10) / 1 == 8 || (modes_l % 10) / 1 == 9){
						crasher = true;
					} //else if < 7777
				
					if(crasher){
						cout << "mkdir: invalid mode `" << argv[i] << "\'" << endl;
						cout << "Try 'mkdir --help' for more information." << endl;
						exit(EXIT_FAILURE);
					}

                	num_hold = std::to_string(modes_l);
                	lengthy(num_hold, argv[i]);

                	modes = strtol(num_hold.c_str(), &pEnd, 8);
            } //else lines

            on_modes = false;
        } //else if arg for n

        else{

        	file_exists = true;
        	modal(modes, intermediate, argv[i]);
        } //else modal

	} //else for argv

	if(!file_exists){
    	cout << "mkdir: missing operand" << endl;
    	cout << "Try 'mkdir --help' for more information." << endl;
    	exit(EXIT_FAILURE);
	} //if file doesnt exist	

	return EXIT_SUCCESS;

}

void lengthy(string &num_hold, const char * argv){

	if(num_hold.length() == 1){
		string temp = "000";
		temp += num_hold;
		num_hold = temp;
	} // if  l 1

	else if(num_hold.length() == 2){
		string temp = "00";
		temp += num_hold;
		num_hold = temp;
	} // if  l 2

	else if(num_hold.length() == 3){
		string temp = "0";
		temp += num_hold;
		num_hold = temp;
	} // if  l 3

	else if(num_hold.length() == 4){
		string temp = "";
		temp += num_hold;
		num_hold = temp;
	} // if  l 4

	else{
		cout << "mkdir: invalid mode `" << argv << "\'" << endl;
		exit(EXIT_FAILURE);
	}
}

void modal(mode_t modes, bool intermediate, const char* path){

	if(!intermediate){

		string ppath_s(path);
		char check_ch;
		string check_s = "";
		long last_pos = 0;
		DIR * dir;
		string path_s = ppath_s;
		long length = path_s.length(); //long int
		for(long chars = 0; chars < length; ++chars){
			
			path_s = ppath_s;

			check_ch = path_s.at(chars);
			if(check_ch == '/'){
				last_pos = chars;
				path_s.erase((path_s.begin()+last_pos), path_s.end());
				check_s = path_s;
				if((dir = opendir(check_s.c_str())) != NULL){ //if u can open a dir using this path
					closedir(dir);
				} //if opendir

				else{
					cout << "mkdir: cannot create directory `" << path_s << "\': No such file or directory" << endl;
					exit(EXIT_SUCCESS);
				}// if dir null close error
			} // if backslash at char
		}// for path_s

		struct stat file_stat;
		umask(0);

		if(lstat(path, &file_stat) == 0 || mkdir(path, modes) == -1){
			cout << "mkdir: cannot create directory `" << path_s << "\': File exists" << endl;
		} else mkdir(path, modes);
	} //if no -p

	else if(intermediate){

		string ppath_s(path);
		char check_ch;
		string check_s = "";
		long last_pos = 0;
		DIR * dir;
		string path_s = ppath_s;
		long length = path_s.length();

		for(long chars = 0; chars < length; ++chars){
			
			path_s = ppath_s;
			check_ch = path_s.at(chars);
			if(check_ch == '/'){
				last_pos = chars;
				path_s.erase(path_s.begin()+last_pos, path_s.end()); //keep finding the next slash
				check_s = path_s;
				if((dir = opendir(check_s.c_str())) != NULL){ //if u can open a dir using this path
					closedir(dir);
				} //if opendir

				else{
					mkdir(check_s.c_str(), 0755);
				}// if dir null close error
			} // if backslash at char
		}// for path_s

		struct stat file_stat;
		umask(0);

		if(lstat(path, &file_stat) == 0 && mkdir(path, modes) == -1){
		} else mkdir(path, modes);

		
	} //if no -p
}
