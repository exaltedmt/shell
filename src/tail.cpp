#include <iostream>
#include <string>
#include <cstdlib>
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
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
void tailline(long &lines, bool more_files, bool last_file, bool input_loop, const char* file); //loop
void tailbyte(long &bytes_r, bool more_files, bool last_file, bool input_loop, const char* file);

int main(const int argc, const char* argv[]){
    
    if(argc == 1){
        hyph();
    } 

    long bytes = 0;
    long lines = 10;
    char * lEnd;
    char * bEnd;
    string file_name = "";
    bool more_files = false;
    bool last_file = false;
    bool on_lines = false;
    bool line_flag = true;
    bool files_exist = false;
    bool on_bytes = false;
    bool byte_flag = false;
    bool input_loop = false;

    //turn off buffering for cout
    cout.setf(std::ios::unitbuf);
    std::setlocale(LC_ALL, "en_US.utf8");

    //args
    for(int i = 1; i < argc; ++i){

        struct stat file_stat;
        cout.flush();
        
        //finds hyphen, checks for n, make sure its not file
        if(argv[i][0] == '-' && argv[i][1] && lstat(argv[i], &file_stat) == -1){ 
            string command(argv[i]);

            if(command.at(1) == 'f'){ //read last line tag
                if((file_stat.st_mode & S_IFREG) || (file_stat.st_mode & S_IFIFO))
                    input_loop = true;

                if(!argv[i+1]){ //nothing after -f
                    cout << "tail: warning: following standard input indefinitely is ineffective" << endl;
                    hyph();
                } // if 
            }

            else if(command.at(1) == 'n'){

                if(argv[i+1]){ //an arg after n
                    on_lines = true;
                    line_flag = true;
                    byte_flag = false;
                }

                else if(!argv[i+1]){ //no arg for -n
                    cout << "tail: option requires an argument -- \'n\'" << endl;
                    cout << "Try `tail --help' for more information." << endl;
                    exit(EXIT_FAILURE);
                }
            } // if -n

            else if(command.at(1) == 'c'){ //arg after c
                if(argv[i+1]){
                    on_bytes = true;
                    line_flag = false;
                    byte_flag = true;
                }

                else if(!argv[i+1]){ //no arg for -c
                    cout << "tail: option requires an argument -- \'n\'" << endl;
                    cout << "Try `tail --help' for more information." << endl;
                    exit(EXIT_FAILURE);
                }
            } //if -c

        } //if

        else if(on_lines){ //theres an arg after -n passed, only needed once
            if(strtol(argv[i], &lEnd, 10) == 0){ //check if number either actually a 0 or a file
                if(argv[i] == lEnd){ //determine if string instead of lines
                    file_name = string(argv[i]);
                    cout << "tail: " << file_name << ": invalid number of lines" << endl;
                    exit(EXIT_FAILURE);
                } //if string

                else lines = 0; //if string not equal lines is actually 0
            } //if lines

            else{
                lines = strtol(argv[i], &lEnd, 10);
            } //else lines

            on_lines = false;
        } //else if arg for n

        else if(on_bytes){ //theres an arg after -n passed, only needed once
            if(strtol(argv[i], &bEnd, 10) == 0){ //check if number either actually a 0 or a file
                if(argv[i] == bEnd){ //determine if string instead of lines
                    file_name = string(argv[i]);
                    cout << "tail: " << file_name << ": invalid number of lines" << endl;
                    exit(EXIT_FAILURE);
                } //if string

                else bytes = 0; //if string not equal lines is actually 0
            } //if bytes

            else{
                bytes = strtol(argv[i], &bEnd, 10);
            } //else bytes

            on_bytes = false;
        } //else if arg for c

        else if(argv[i+1]){ //more args after current
            more_files = true;

            if(line_flag && !byte_flag)
                tailline(lines, more_files, last_file, input_loop, argv[i]);

            if(byte_flag && !line_flag)
                tailbyte(bytes, more_files, last_file, input_loop, argv[i]);

            files_exist = true;
        }

        else{
            last_file = true;
            files_exist = true;
            
            if(line_flag && !byte_flag)
                tailline(lines, more_files, last_file, input_loop, argv[i]);

            if(byte_flag && !line_flag)
                tailbyte(bytes, more_files, last_file, input_loop, argv[i]);
        } //else

        if(argv[i][0] == '-' && !argv[i][1]){ //a hyphen with -f
            if(input_loop){
                cout << "tail: warning: following standard input indefinitely is ineffective" << endl;
            }
        } //if

        if(lines > 0 && !argv[i+1] && !files_exist){ //are lines, no files, and nothing extra
            hyph();
        } //if lines

        else if(bytes > 0 && !argv[i+1] && !files_exist){ //are lines, no files, and nothing extra
            hyph();
        } //if bytes   
    } //for

    return EXIT_SUCCESS;
}

void tailline(long &lines, bool more_files, bool last_file, bool input_loop, const char* file){

    --lines;
    char buff[BUFFSIZE];
    bool hyphen = false;
    long line_ctr = 0;
    long chars = 0;
    string head_str = "";
    string sub_str = "";
    int fd = 0;
    int bytes = 0;

    //turn off buffering for cout
    cout.setf(std::ios::unitbuf);
    std::setlocale(LC_ALL, "en_US.utf8");
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

            fseek(file_r , 0 , SEEK_END);
            lSize = ftell(file_r);
            rewind(file_r);

            fread(buff, 1, lSize, file_r);

            // to get the last lines to print, subtract input from max lines. 

            for(chars = 0; chars < lSize; ++chars){ 

                if(chars + 1 > lSize && buff[chars] == '\n')
                    break;
                if(buff[chars] == '\n') //check if a line
                    ++line_ctr;
            } //for lsize

            long sub_line = line_ctr - lines; // i.e 324 - 10
            long max_lines = line_ctr; // 324

            //if line counter exceed max lines make sure they dont exceed max
            if(lines > max_lines){ // if subline 
                lines = max_lines;
            }

            line_ctr = 0;

            for(chars = 0; chars < lSize; ++chars){ 

                //break after max passed
                if(line_ctr > max_lines){ 
                    break;   
                }

                //dont add chars until after passing the sub_line
                if(line_ctr >= sub_line){
                    head_str += buff[chars];
                }
                
                if(buff[chars] == '\n'){ //check if a line
                        ++line_ctr;
                }
            }

            if(!last_file){ //not last file
                head_str += "\n";
                cout << head_str;
            }

            else{ 
                cout << head_str;
            }
    
            fclose(file_r);

            if(input_loop){
                
                if((fd = open(file, O_RDWR)) < 0){
                    cout << "tail: cannot open `" << file << "' for reading: No such file or directory" << endl;
                }  

                while((bytes = read(STDIN_FILENO, buff, BUFFSIZE)) > 0)
                    if(write(fd, buff, bytes) != bytes)
                        perror("Write Error");
            } //if input_loop
        }

        else{
            cout << "tail: cannot open `" << file << "' for reading: No such file or directory" << endl;
        }
    }
}

void tailbyte(long &bytes_r, bool more_files, bool last_file, bool input_loop, const char* file){

    char buff[BUFFSIZE];
    bool hyphen = false;
    int fd;
    int bytes;
    long sub_offst;
    long new_offst;
    string head_str = "";

    //turn off buffering for cout
    cout.setf(std::ios::unitbuf);
    std::setlocale(LC_ALL, "en_US.utf8");
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
       
        if((fd = open(file, O_RDWR)) < 0){
            cout << "tail: cannot open `" << file << "' for reading: No such file or directory" << endl;
        }

        else{

            if(more_files){
                cout << "==> " << file << " <==" << endl;
            }

            sub_offst = lseek(fd, 0, SEEK_END);

            if(bytes_r > sub_offst)
                bytes_r = sub_offst;

            new_offst = sub_offst - bytes_r;

            lseek(fd, new_offst, SEEK_SET);

            //only read last (bytes_r) of file.
            if(!input_loop){
                while((bytes = read(fd, buff, BUFFSIZE)) > 0){
                    if(write(STDOUT_FILENO, buff, bytes) != bytes)
                            perror("Write Error");
                } //while read fd
            }   

            else{ 

                while((bytes = read(STDIN_FILENO, buff, BUFFSIZE)) > 0){
                    if(write(fd, buff, bytes) != bytes)
                        perror("Write Error");
                }
            } //if input_loop
        }
    }
} 

void hyph(){

    int bytes;
    char buff[BUFFSIZE];

    while((bytes = read(STDIN_FILENO, buff, BUFFSIZE)) > 0){}
}
