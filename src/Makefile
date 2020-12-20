all:
	g++ -c -Wall -std=c++14 -g -O0 -pedantic-errors cal.cpp
	g++ -c -Wall -std=c++14 -g -O0 -pedantic-errors date.cpp
	g++ -c -Wall -std=c++14 -g -O0 -pedantic-errors pwd.cpp
	g++ -c -Wall -std=c++14 -g -O0 -pedantic-errors env.cpp
	g++ -c -Wall -std=c++14 -g -O0 -pedantic-errors false.cpp
	g++ -c -Wall -std=c++14 -g -O0 -pedantic-errors head.cpp
	g++ -c -Wall -std=c++14 -g -O0 -pedantic-errors ln.cpp
	g++ -c -Wall -std=c++14 -g -O0 -pedantic-errors ls.cpp
	g++ -c -Wall -std=c++14 -g -O0 -pedantic-errors mkdir.cpp
	g++ -c -Wall -std=c++14 -g -O0 -pedantic-errors tail.cpp
	g++ -c -Wall -std=c++14 -g -O0 -pedantic-errors true.cpp
	g++ -c -Wall -std=c++14 -g -O0 -pedantic-errors wc.cpp
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -o cal cal.o
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -o date date.o
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -o pwd pwd.o
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -o env env.o
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -o false false.o
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -o head head.o
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -o ln ln.o
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -o ls ls.o
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -o mkdir mkdir.o
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -o true true.o
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -o wc wc.o

## Making cal
cal: cal.o
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -o cal cal.o

cal.o: cal.cpp
	g++ -c -Wall -std=c++14 -g -O0 -pedantic-errors cal.cpp

## Making pwd
pwd: pwd.o
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -o pwd pwd.o

pwd.o: pwd.cpp
	g++ -c -Wall -std=c++14 -g -O0 -pedantic-errors pwd.cpp

## Making date
date: date.o
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -o date date.o

date.o: date.cpp
	g++ -c -Wall -std=c++14 -g -O0 -pedantic-errors date.cpp

## Making env
env: env.o
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -o env env.o

env.o: env.cpp
	g++ -c -Wall -std=c++14 -g -O0 -pedantic-errors env.cpp

## Making false
false: false.o
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -o false false.o

false.o: false.cpp
	g++ -c -Wall -std=c++14 -g -O0 -pedantic-errors false.cpp

## Making head
head: head.o
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -o head head.o

head.o: head.cpp
	g++ -c -Wall -std=c++14 -g -O0 -pedantic-errors head.cpp

## Making ln
ln: ln.o
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -o ln ln.o

ln.o: ln.cpp
	g++ -c -Wall -std=c++14 -g -O0 -pedantic-errors ln.cpp

## Making ls
ls: ls.o
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -o ls ls.o

ls.o: ls.cpp
	g++ -c -Wall -std=c++14 -g -O0 -pedantic-errors ls.cpp

## Making mkdir
mkdir: mkdir.o
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -o mkdir mkdir.o

mkdir.o: mdkir.cpp
	g++ -c -Wall -std=c++14 -g -O0 -pedantic-errors mkdir.cpp

## Making tail
tail: tail.o
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -o tail tail.o

tail.o: tail.cpp
	g++ -c -Wall -std=c++14 -g -O0 -pedantic-errors tail.cpp

## Making true
true: true.o
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -o true true.o

true.o: true.cpp
	g++ -c -Wall -std=c++14 -g -O0 -pedantic-errors true.cpp

## Making wc
wc: wc.o
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -o wc wc.o

wc.o: wc.cpp
	g++ -c -Wall -std=c++14 -g -O0 -pedantic-errors wc.cpp


clean: 
	rm -rf cal
	rm -rf date
	rm -rf env
	rm -rf false
	rm -rf head
	rm -rf ln
	rm -rf ls
	rm -rf mkdir
	rm -rf tail
	rm -rf true
	rm -rf wc
	rm -rf pwd
	rm -rf *.cpp~
	rm -rf *.o~
	rm -rf *.o
	rm -rf core.*
