#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>
#include <cerrno>
#include <string.h>
#include <string>
#include <cerrno>
#include <iostream>
#include <math.h>

using namespace std;

#define BUFFSIZE 4096

int numOfOctets(char fileName[]);
int cyclicRedundancyCheck(char fileName[]);

int main(int agrc, char* argv[]){
  //writing CRC                                                                           
  cyclicRedundancyCheck(argv[1]);
  cout<<" ";
  //writing number of octets                                                              
  cout<<numOfOctets(argv[1])<<" ";
  //writing filename                                                                      
  cout<<argv[1]<<endl;

}
//good luck with that                                                                     
int cyclicRedundancyCheck(char fileName[]){
  //int crc=0;                                                                            
  //int fd;                                                                               
  int polynomial=2187366103;
  //int remainder;                                                                        
  //char buff[BUFFSIZE];                                                                  
  // fd=open(fileName, O_RDWR);                                                           
  cout<<numOfOctets(fileName)*8%polynomial;


  return EXIT_SUCCESS;
}
int numOfOctets(char fileName[]){
  int fd;
  int p;
  char buff[BUFFSIZE];

    if((fd=open(fileName,O_RDWR))<0){
      cout<<"error"<<endl;
    }
  p = read(fd,buff,BUFFSIZE);
  return p;
}

