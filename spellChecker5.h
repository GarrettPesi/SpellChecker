#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define MAXWORDS 112000 
#define MAXSIZE  30 
#define PRIME 991
#define SAME_HASH 10 



char  gDictionary[MAXWORDS] [MAXSIZE] ;
int   gDictCount = 0; 			// How many are in the dictionary array 

int missed = 0, checked = 0;

void Build_dictionary( char dicfile [] ) ;
void Check_spelling(char file[] , char rep[],  int toscreen);
void oldCheck_spelling(char file[] , char rep[],  int toscreen);
void Word_shift(char word[], int pos);

void Hash_Check_spelling(char file[], char rep[], int toscreen);
void Hash_Build_dictionary(char dicfile[]);
int hash(char word[]);
