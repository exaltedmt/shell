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
#include <stdlib.h>
#include <string>
#include <cstring>
#include <clocale>
#include <locale.h>
#include <wchar.h>

#define BUFFSIZE 2048*10

using std::string;
using std::endl;
using std::cout;

void hyph(); //hyphen
void headline(long &lines, bool more_files, bool last_file, const char* file); //loop

int main(const int argc, const char* argv[]){
    
    if(argc == 1){
        hyph();
    } 

    long lines = 10;
    char * pEnd;
    string file_name = "";
    bool more_files = false;
    bool last_file = false;
    bool on_lines = false;
    bool files_exist = false;

    //turn off buffering for cout
    cout.setf(std::ios::unitbuf);

    //args
    for(int i = 1; i < argc; ++i){

        struct stat file_stat;
        cout.flush();
        
        //finds hyphen, checks for n, make sure its not file
        if(argv[i][0] == '-' && argv[i][1] && lstat(argv[i], &file_stat) == -1){ 
            string command(argv[i]);

            if(command.at(1) == 'n'){

                if(argv[i+1]){ //an arg after n
                    on_lines = true;
                }

                else if(!argv[i+1]){ //no arg for -n
                    cout << "head: option requires an argument -- \'n\'" << endl;
                    cout << "Try `head --help' for more information." << endl;
                    exit(EXIT_FAILURE);
                }
            }
        } //if

        else if(on_lines){ //theres an arg after -n passed, only needed once
            if(strtol(argv[i], &pEnd, 10) == 0){ //check if number either actually a 0 or a file
                if(argv[i] == pEnd){ //determine if string instead of lines
                    file_name = string(argv[i]);
                    cout << "head: " << file_name << ": invalid number of lines" << endl;
                    exit(EXIT_FAILURE);
                } //if string

                else lines = 0; //if string not equal lines is actually 0
            } //if lines

            else{
                lines = strtol(argv[i], &pEnd, 10);
            } //else lines

            on_lines = false;
        } //else if arg for n

        else if(argv[i+1]){ //more args after current
            more_files = true;
            headline(lines, more_files, last_file, argv[i]);
            files_exist = true;
        }

        else{
            last_file = true;
            files_exist = true;
            headline(lines, more_files, last_file, argv[i]);
        } //else

        if(lines > 0 && !argv[i+1] && !files_exist){ //are lines, no files, and nothing extra
            hyph();
        } //if  
    } //for

    return EXIT_SUCCESS;
}

void headline(long &lines, bool more_files, bool last_file, const char* file){

    char buff[BUFFSIZE];
    bool hyphen = false;
    long line_ctr = 0;
    long chars = 0;
    string head_str = "";

    //turn off buffering for cout
    cout.setf(std::ios::unitbuf);
    cout.flush();

    if(file[0] == '-' && !file[1]){ //check for hyphen
        hyphen = true;
        if(more_files){ //for header
            cout << "==> " << "standard output" << " <==" << endl;
            hyph();
        }

        else hyph();
    } //if hyph

    if(!hyphen){ //dont run this if we did hyphens
       
        FILE * file_r;
        file_r = fopen(file,"rb");
        long lSize;

        if(file_r != NULL){

            if(more_files){
                cout << "==> " << file << " <==" << endl;
            }

            fseek(file_r , 1 , SEEK_END);
            lSize = ftell(file_r);
            rewind(file_r);

            fread(buff, 1, lSize, file_r);

            for(chars = 0; chars < lSize; ++chars){ 

                if(buff[chars] == '\n') //check if a line
                    ++line_ctr;
            }

            if(lines > line_ctr)
                lines = line_ctr;

            line_ctr = 0;

            for(chars = 0; chars < lSize; ++chars){ 

                if(buff[chars] == '\n') //check if a line
                    ++line_ctr;
                if(line_ctr == lines){ 
                    break;   
                }
                
                head_str += buff[chars];
            }

            if(!last_file){ //not last file
                head_str += "\n";
                cout << head_str;
            }

            else{ 
                cout << head_str << endl;
            }
        
            fclose(file_r);
        } 

        else {
            cout << "head: cannot open `" << file << "' for reading: No such file or directory" << endl;
        }
    }
}

void hyph(){

    int bytes;
    char buff[BUFFSIZE];

    while((bytes = read(STDIN_FILENO, buff, BUFFSIZE)) > 0)
        if(write(STDOUT_FILENO, buff, bytes) != bytes)
            perror("Write Error");
}
