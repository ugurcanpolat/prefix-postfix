/* @Author
 * Student Name: Ugurcan Polat
 */

#ifndef functions_h
#define functions_h

#include <cctype>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib> 
#include <cstring>
#include <ctime>
#include <iomanip>  
#include <iostream>

#define ERROR   -1

#define UNARY    1
#define BINARY   2
#define POLY  1000

#define ABS	 0
#define SQRT	 1

#define SUB	 2
#define DIV	 3

#define EXP	 4
#define LOG	 5
#define MOD	 6

#define SUM	 7
#define PRODUCT	 8
#define MIN	 9
#define MAX	10

struct Node {
    char *data;
    Node *next;
};

struct Queue {
    Node  *front;
    Node  *back;
    void  createQueue();
    void  closeQueue();
    void  enqueue(char*);
    char* dequeue();
};

struct Stack {
    Node  *head;
    void  createStack();
    void  closeStack();
    void  push(char*);
    char* pop();
};

struct Functions {
    FILE  *fp;
    int   unaryBinaryPolynary(char*);
    int   whichOperator(char*);
    long  checkOperand(char*);
    bool  isOperator(char*);
    char* convertToPostfix(char*);
    char* integerToString(long);
    void  initialize();
    void  evaluatePostfix(char*);
    void  openInputFile(char*);
    void  readFromInputFile();
    void  printPostfix(char*);
    void  quit();
};

#endif
