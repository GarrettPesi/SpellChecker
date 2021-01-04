//Garrett Pesi	Spell Checker	COE 1750

#include "spellChecker5.h"

//#define PHASE_IIA  //faster with smaller documents

#define PHASE_III   //much faster with larger documents

#ifdef PHASE_IIA
int gletterRange[128] [2] ;
#endif

#ifdef PHASE_III
char hashArr[128][PRIME][SAME_HASH][MAXSIZE] = {0};
int hashLetterRange[128];
#endif
	/* [k][0] has starting Dictionary index for ASCII k value.
	* [k][1]  has (ending +1) index for ASCII k value. 
	* This table is looked up using ASCII 
	* value of 1st char of word to be checked. For non alphabet chars
	*set the indices to 0.  Both cases, upper or lower get the same indices. 
	*/

/* (i)	Develop a function to copy a char string to another char array. 
* Both are given as incoming arguments. 
* This function does not modify the original string.   
*/
int copyStrCase (char src[], char dest[]) {  
	int i = 0;
	if(src[0] >= 'A' && src[0] <= 'Z')
		src[0] = src[0] - 'A' + 'a';
	if((int)src[1] == 39 || (int)src[1] == 47) //accounts for several words that have special characters in position 2
		Word_shift(src, 1);
	if(src[1] >= 'A' && src[1] <= 'Z')
		src[1] = src[1] - 'A' + 'a';
	while(src[i] != NULL){
		dest[i] = src[i];
		i++;		
	}
	dest[i] = NULL;
	return i;
}

#ifdef PHASE_III
int copyStrCaseHash (char src[], char dest[][PRIME][SAME_HASH][MAXSIZE]) {  
	int i = 0, j = 0;
	if(src[0] >= 'A' && src[0] <= 'Z')
		src[0] = src[0] - 'A' + 'a';
	if((int)src[1] == 39 || (int)src[1] == 47) //accounts for several words that have special characters in position 2
		Word_shift(src, 1);
	if(src[1] >= 'A' && src[1] <= 'Z')  //accounts for several words that have uppercase second letter
		src[1] = src[1] - 'A' + 'a';
	if(src[1] < hashLetterRange[(int)src[0]]){
		while(dest[(int)src[0]][hash(src)][j][0] != NULL)
			j++;
		while(src[i] != NULL){
			dest[(int)src[0]][hash(src)][j][i] = src[i]; 
			i++;		
		}
		dest[(int)src[0]][hash(src)][j][i] = NULL;
	}
	else{
		while(dest[(int)src[0] - 'a' + 'A'][hash(src)][j][0] != NULL)
			j++;
		while(src[i] != NULL){
			dest[(int)src[0] - 'a' + 'A'][hash(src)][j][i] = src[i]; 
			i++;		
		}
		dest[(int)src[0] - 'a' + 'A'][hash(src)][j][i] = NULL;
	}
	return i;
}

int equalStrCaseHash(char src[], char dest[][PRIME][SAME_HASH][MAXSIZE]){
	int i = 0, j = 0;
	if(src[1] < hashLetterRange[(int)src[0]]){
		while(dest[(int)src[0]][hash(src)][j][0] != NULL){
			while(src[i] == dest[(int)src[0]][hash(src)][j][i]){
				if(src[i] == NULL)
					return 1;  //match successful
				i++;
			}
			j++;
			i = 0;
		}
	}
	else{
		while(dest[(int)src[0] - 'a' + 'A'][hash(src)][j][0] != NULL){
			while(src[i] == dest[(int)src[0] - 'a' + 'A'][hash(src)][j][i]){
				if(src[i] == NULL)
					return 1;  //match successful
				i++;
			}
			j++;
			i = 0;
		}		
	}
	return 0;  //no match
}

#endif

/* (ii)	Develop a function to compare 1st string to 2nd string. Both are  
* incoming arguments.  It returns 0 
* if they match, else returns index value where mismatch occurred. It does not 
* change either of the given strings. 
*/ 
int equalStrCase (char src[], char dest[]) {  
	int i = 0;
	while(src[i] == dest[i]){
		if(src[i] == NULL)
			return 0;
		i++;
	}
	return i + 1;
}

/* (iii)	 Develop a function to count the number of chars in a string
*  and return that count plus 1 (to accommodate '\0' at the end). 
* It is to be named strlen1.
*/
int  strlen1(  char w[]) {   
	int count = 0;
	while(w[count] != NULL){
		count++;
	}
	count++;//Account for \0
	return count;
}

/* (iv)	 Main function simply calls other functions (or subroutines):
a.	Open dictionary file and build it.
b.	Open document text file to read each word, check against dictionary words and report misspelled words.
c.	Close all the files, write statistics of misspelled words, correct words, etc.  
*/
int main(int argc, char *argv[]) {
	int  screen = 0; 
	printf("Created by Garrett Pesi\n\n");
	clock_t T1, T2, T3; 
	if ( argc < 3 ) 
	{ 
		printf("Usage: \n\t program dictFile documentFile  OutFile\n"); 
		exit(-1) ; 
	} 
	T1 = clock();
	#ifdef PHASE_III
	Hash_Build_dictionary(argv[1]);
	#else
	Build_dictionary( argv[1]) ;
	#endif
	T2 = clock();
	printf(" Building Dictionary %f seconds \n", (T2-T1)/1000.0 );
	//printf("Total of %i\n", gDictCount);
	if (argc < 4 ){   //misspelled words into screen only 
		#ifdef PHASE_III
		Hash_Check_spelling(argv[2], (char *)NULL, 1);
		#else
			#ifdef PHASE_IIA	
			Check_spelling(argv[2] , (char *)NULL, 1); 
			#else
			oldCheck_spelling(argv[2] , (char *)NULL, 1); 
			#endif
		#endif
	}
	else {
		//output file given, no screen dump
		#ifdef PHASE_III
		Hash_Check_spelling(argv[2], argv[3], 0);
		#else
			#ifdef PHASE_IIA	
			Check_spelling(argv[2] , argv[3], 0); 
			#else
			oldCheck_spelling(argv[2] , argv[3], 0); 
			#endif
		#endif
		printf("Misspelled words in %s file written to %s file\n",argv[2], argv[3] );
	}
	printf("Results----------------------------------\n");
	printf("Total checked words:  %i", checked);
	printf("\nTotal misspelled words:  %i\n", missed); 
	T3 = clock();
	printf("total time used: =%f seconds\n", (T3-T1)/1000.0 ) ;
	return 0;
}

// Reads the given file, each word goes to one row of the array. 
// Dictionary words are in sorted order at least for 1st letter in each word. 
// so, group A words 1st, B words next, etc. 
void Build_dictionary( char dicfile [] ) { 
  	FILE  *fp = fopen( dicfile, "r") ;
	if(fp == (FILE *)NULL){
		printf("There was an error accessing %s", dicfile);
		printf("\nExiting...");
		exit(27);
	}
	
	int index = 0;
	
	while(feof(fp) == 0){
		char buf[30];
		fscanf(fp, "%s", buf);
		copyStrCase(buf, gDictionary[index]);
		index++;
	}
	fclose(fp);
	gDictCount = index;
	
	//Create dictionary ranges
	#ifdef PHASE_IIA
	gletterRange[(int)('a')][0] = 0; //a
	gletterRange[(int)'a'][1] = 3724; //an
	gletterRange[(int)'b'][0] = 7774; //b
	gletterRange[(int)'b'][1] = 10563; //bl
	gletterRange[(int)'c'][0] = 13227; //c 
	gletterRange[(int)'c'][1] = 17872; //co
	gletterRange[(int)'d'][0] = 23481; //d
	gletterRange[(int)'d'][1] = 26350; //di
	gletterRange[(int)'e'][0] = 30397; //e
	gletterRange[(int)'e'][1] = 32054; //en
	gletterRange[(int)'f'][0] = 35749; //f
	gletterRange[(int)'f'][1] = 37412; //fl
	gletterRange[(int)'g'][0] = 39943; //g
	gletterRange[(int)'g'][1] = 41295; //gl
	gletterRange[(int)'h'][0] = 43348; //h
	gletterRange[(int)'h'][1] = 45368; //hi
	gletterRange[(int)'i'][0] = 47442; //i
	gletterRange[(int)'i'][1] = 48842; //in
	gletterRange[(int)'j'][0] = 52978; //j
	gletterRange[(int)'j'][1] = 53407; //jo
	gletterRange[(int)'k'][0] = 53801; //k
	gletterRange[(int)'k'][1] = 54120; //ki
	gletterRange[(int)'l'][0] = 54659; //l
	gletterRange[(int)'l'][1] = 56308; //li
	gletterRange[(int)'m'][0] = 58095; //m
	gletterRange[(int)'m'][1] = 61012; //mh
	gletterRange[(int)'n'][0] = 64112; //n
	gletterRange[(int)'n'][1] = 65004; //ni 
	gletterRange[(int)'o'][0] = 66148; //o
	gletterRange[(int)'o'][1] = 67946; //os
	gletterRange[(int)'p'][0] = 69352; //p
	gletterRange[(int)'p'][1] = 74024; //pl
	gletterRange[(int)'q'][0] = 79438; //q
	gletterRange[(int)'q'][1] = 79441; //qu
	gletterRange[(int)'r'][0] = 80082; //r
	gletterRange[(int)'r'][1] = 83532; //rh
	gletterRange[(int)'s'][0] = 85019; //s
	gletterRange[(int)'s'][1] = 91091; //so
	gletterRange[(int)'t'][0] = 97345; //t
	gletterRange[(int)'t'][1] = 100835; //tr
	gletterRange[(int)'u'][0] = 103033; //u
	gletterRange[(int)'u'][1] = 105071; //up
	gletterRange[(int)'v'][0] = 105516; //v
	gletterRange[(int)'v'][1] = 106482; //vi
	gletterRange[(int)'w'][0] = 107318; //w
	gletterRange[(int)'w'][1] = 108225; //wh
	gletterRange[(int)'x'][0] = 109569; //x
	gletterRange[(int)'x'][1] = 109647; //xy
	gletterRange[(int)'y'][0] = 109696; //y
	gletterRange[(int)'y'][1] = 109886; //yf
	gletterRange[(int)'z'][0] = 110026; //z
	gletterRange[(int)'z'][1] = 110380; //zy
 	#endif
}

//***********************************************************
//total time used: =0.465000 seconds
#ifdef PHASE_III

int hash(char word[]){
	int i = 1, hashVal = 0;
	while(word[i] != '\0'){
		hashVal += (int)word[i];
		i++;
	}
	return hashVal%PRIME;
}

void Hash_Build_dictionary(char dicfile[]){
  	FILE  *fp = fopen( dicfile, "r") ;
	if(fp == (FILE *)NULL){
		printf("There was an error accessing %s", dicfile);
		printf("\nExiting...");
		exit(27);
	}
	
	hashLetterRange[(int)('a')] = (int)'n'; //a
	//hashLetterRange[(int)'a'][1] = 3724; //an
	hashLetterRange[(int)'b'] = 'l'; //b
	//hashLetterRange[(int)'b'][1] = 10563; //bl
	hashLetterRange[(int)'c'] = 'o'; //c 
	//hashLetterRange[(int)'c'][1] = 17872; //co
	hashLetterRange[(int)'d'] = 'i'; //d
	//hashLetterRange[(int)'d'][1] = 26350; //di
	hashLetterRange[(int)'e'] = 'n'; //e
	//hashLetterRange[(int)'e'][1] = 32054; //en
	hashLetterRange[(int)'f'] = 'l'; //f
	//hashLetterRange[(int)'f'][1] = 37412; //fl
	hashLetterRange[(int)'g'] = 'l'; //g
	//hashLetterRange[(int)'g'][1] = 41295; //gl
	hashLetterRange[(int)'h'] = 'i'; //h
	//hashLetterRange[(int)'h'][1] = 45368; //hi
	hashLetterRange[(int)'i'] = 'n'; //i
	//hashLetterRange[(int)'i'][1] = 48842; //in
	hashLetterRange[(int)'j'] = 'o'; //j
	//hashLetterRange[(int)'j'][1] = 53407; //jo
	hashLetterRange[(int)'k'] = 'i'; //k
	//hashLetterRange[(int)'k'][1] = 54120; //ki
	hashLetterRange[(int)'l'] = 'i'; //l
	//hashLetterRange[(int)'l'][1] = 56308; //li
	hashLetterRange[(int)'m'] = 'h'; //m
	//hashLetterRange[(int)'m'][1] = 61012; //mh
	hashLetterRange[(int)'n'] = 'i'; //n
	//hashLetterRange[(int)'n'][1] = 65004; //ni 
	hashLetterRange[(int)'o'] = 's'; //o
	//hashLetterRange[(int)'o'][1] = 67946; //os
	hashLetterRange[(int)'p'] = 'l'; //p
	//hashLetterRange[(int)'p'][1] = 74024; //pl
	hashLetterRange[(int)'q'] = 'u'; //q
	//hashLetterRange[(int)'q'][1] = 79441; //qu
	hashLetterRange[(int)'r'] = 'h'; //r
	//hashLetterRange[(int)'r'][1] = 83532; //rh
	hashLetterRange[(int)'s'] = 'o'; //s
	//hashLetterRange[(int)'s'][1] = 91091; //so
	hashLetterRange[(int)'t'] = 'r'; //t
	//hashLetterRange[(int)'t'][1] = 100835; //tr
	hashLetterRange[(int)'u'] = 'p'; //u
	//hashLetterRange[(int)'u'][1] = 105071; //up
	hashLetterRange[(int)'v'] = 'i'; //v
	//hashLetterRange[(int)'v'][1] = 106482; //vi
	hashLetterRange[(int)'w'] = 'h'; //w
	//hashLetterRange[(int)'w'][1] = 108225; //wh
	hashLetterRange[(int)'x'] = 'y'; //x
	//hashLetterRange[(int)'x'][1] = 109647; //xy
	hashLetterRange[(int)'y'] = 'f'; //y
	//hashLetterRange[(int)'y'][1] = 109886; //yf
	hashLetterRange[(int)'z'] = 'y'; //z
	//hashLetterRange[(int)'z'][1] = 110380; //zy

	int index = 0;
	while(feof(fp) == 0){
		char buf[30];
		fscanf(fp, "%s", buf);
		copyStrCaseHash(buf, hashArr);
		index++;
	}
	fclose(fp);
	gDictCount = index;
	
}


void Hash_Check_spelling(char document[], char report[], int toscreen){
 FILE  *docfp = fopen( document, "r") ;
 FILE  *repfp  = NULL;
 
 char buf[MAXSIZE];
 
 int start1, end1;
  
  if (docfp == NULL) { printf( "can not open %s", document); exit(40); } 
  if ( report != NULL ) { 
  	 if ( (repfp = fopen( report, "w")) == NULL) 
	  { printf( "can not open %s", report); exit(41); } 	 
  } 
  else repfp = stdout;
  
  int i;
  
  while(!feof(docfp)){
  	fscanf(docfp, "%s", buf);
  	//Strip front
  	while(!(buf[0] >= 'A' && buf[0] <= 'Z') && !(buf[0] >= 'a' && buf[0] <= 'z') && !(buf[0] >= '0' && buf[0] <= '9')){
  		Word_shift(buf, 0);
	}

  	if(buf[0] >= 'A' && buf[0] <= 'Z')
		buf[0] = buf[0] - 'A' + 'a';
	if((int)buf[1] == 39 || (int)buf[1] == 47) //accounts for several words that have special characters in position 2
		Word_shift(buf, 1);
	if(buf[1] >= 'A' && buf[1] <= 'Z')
		buf[1] = buf[1] - 'A' + 'a';
  	checked++;
  	
  	if(!(buf[0] >= 'a' && buf[0] <= 'z')){
  		fprintf(repfp, "%s\n", buf);
		missed++;
		continue;
	}
	
	if(!(buf[0] >= 'a' && buf[0] <= 'z')){
  		fprintf(repfp, "%s\n", buf);
		missed++;
		continue;
	}
	
	if(equalStrCaseHash(buf, hashArr))
		continue;
	else{
		//Strip end
		while(!(buf[strlen1(buf) - 2] >= 'A' && buf[strlen1(buf) - 2] <= 'Z') && !(buf[strlen1(buf) - 2] >= 'a' && buf[strlen1(buf) - 2] <= 'z') && !(buf[strlen1(buf) - 2] >= '0' && buf[strlen1(buf) - 2] <= '9')){
	  		Word_shift(buf, (strlen1(buf) - 2));
		}

  		//'s
		if(buf[strlen1(buf) - 2] == 's'){
			if((char)buf[strlen1(buf) - 3] == 39){
				Word_shift(buf, (strlen1(buf) - 3));
				Word_shift(buf, (strlen1(buf) - 2));
			}
		}
		
		if(equalStrCaseHash(buf, hashArr))
			continue;
		else{
			fprintf(repfp, "%s\n", buf);
		}	missed++;		
	}

	
  }
  fclose(docfp);
  if(repfp != stdout)
  	fclose(repfp);  	
}
#endif

//***********************************************************
//total time used: =1.472000 seconds
#ifdef PHASE_IIA
void Check_spelling(char document[], char report[], int toscreen){
 FILE  *docfp = fopen( document, "r") ;
 FILE  *repfp  = NULL;
 
 char buf[MAXSIZE];
 
 int start1, end1;
  
  if (docfp == NULL) { printf( "can not open %s", document); exit(40); } 
  if ( report != NULL ) { 
  	 if ( (repfp = fopen( report, "w")) == NULL) 
	  { printf( "can not open %s", report); exit(41); } 	 
  } 
  else repfp = stdout;

  int i;
  
  while(!feof(docfp)){
  	fscanf(docfp, "%s", buf);
  	//Strip front
  	while(!(buf[0] >= 'A' && buf[0] <= 'Z') && !(buf[0] >= 'a' && buf[0] <= 'z') && !(buf[0] >= '0' && buf[0] <= '9')){
  		Word_shift(buf, 0);
	}

  	if(buf[0] >= 'A' && buf[0] <= 'Z')
		buf[0] = buf[0] - 'A' + 'a';
	if((int)buf[1] == 39 || (int)buf[1] == 47) //accounts for several words that have special characters in position 2
		Word_shift(buf, 1);
	if(buf[1] >= 'A' && buf[1] <= 'Z')
		buf[1] = buf[1] - 'A' + 'a';
  	checked++;
  	
  	if(!(buf[0] >= 'a' && buf[0] <= 'z')){
  		fprintf(repfp, "%s\n", buf);
		missed++;
		continue;
	}

	if (buf[1] >= gDictionary[gletterRange[(int)buf[0]][1]][1]){
		start1 = gletterRange[(int)buf[0]][1];
		end1 = gletterRange[(int)buf[0] + 1][0];
	}	
	else{
		start1 = gletterRange[(int)buf[0]][0];
		end1 = gletterRange[(int)buf[0]][1];
	}
		
	
  	for(i = start1; i < end1; i++){
  		if(equalStrCase(buf, gDictionary[i]) == 0)
  			break;	
	}

	if(i == end1){		
		int j;
		//Strip end
		while(!(buf[strlen1(buf) - 2] >= 'A' && buf[strlen1(buf) - 2] <= 'Z') && !(buf[strlen1(buf) - 2] >= 'a' && buf[strlen1(buf) - 2] <= 'z') && !(buf[strlen1(buf) - 2] >= '0' && buf[strlen1(buf) - 2] <= '9')){
	  		Word_shift(buf, (strlen1(buf) - 2));
		}

  		//'s
		if(buf[strlen1(buf) - 2] == 's'){
			if((char)buf[strlen1(buf) - 3] == 39){
				Word_shift(buf, (strlen1(buf) - 3));
				Word_shift(buf, (strlen1(buf) - 2));
			}
		}

		for(j = start1; j < end1; j++){
			if(equalStrCase(buf, gDictionary[j]) == 0)
  				break;
		}
		if(j == end1){
				fprintf(repfp, "%s\n", buf);
				missed++;
		}
	}
  }
  fclose(docfp);
  if(repfp != stdout)
  	fclose(repfp);
}
#endif

/////////////////////////////////////////////////////////////
//total time used: =30.140000 seconds with old method
void oldCheck_spelling(char document[] , char report[], int toscreen) { 
 FILE  *docfp = fopen( document, "r") ;
 FILE  *repfp  = NULL;
 
 char buf[MAXSIZE];
  
  if (docfp == NULL) { printf( "can not open %s", document); exit(40); } 
  if ( report != NULL ) { 
  	 if ( (repfp = fopen( report, "w")) == NULL) 
	  { printf( "can not open %s", report); exit(41); } 	 
  } 
  else repfp = stdout; 
  
  int i;
  
  while(!feof(docfp)){
  	fscanf(docfp, "%s", buf);
  	//Strip front
  	while(!(buf[0] >= 'A' && buf[0] <= 'Z') && !(buf[0] >= 'a' && buf[0] <= 'z') && !(buf[0] >= '0' && buf[0] <= '9')){
  		Word_shift(buf, 0);
	}

  	if(buf[0] >= 'A' && buf[0] <= 'Z')
		buf[0] = buf[0] - 'A' + 'a';
	if((int)buf[1] == 39 || (int)buf[1] == 47) //accounts for several words that have special characters in position 2
		Word_shift(buf, 1);
	if(buf[1] >= 'A' && buf[1] <= 'Z')
		buf[1] = buf[1] - 'A' + 'a';
  	checked++;
  	for(i = 0; i < gDictCount; i++){
  		if(equalStrCase(buf, gDictionary[i]) == 0)
  			break;
  		
	}

	if(i == gDictCount){
		int j;
		//Strip end
		while(!(buf[strlen1(buf) - 2] >= 'A' && buf[strlen1(buf) - 2] <= 'Z') && !(buf[strlen1(buf) - 2] >= 'a' && buf[strlen1(buf) - 2] <= 'z') && !(buf[strlen1(buf) - 2] >= '0' && buf[strlen1(buf) - 2] <= '9')){
	  		Word_shift(buf, (strlen1(buf) - 2));
		}

  		//'s
		if(buf[strlen1(buf) - 2] == 's'){
			if((char)buf[strlen1(buf) - 3] == 39){
				Word_shift(buf, (strlen1(buf) - 3));
				Word_shift(buf, (strlen1(buf) - 2));
			}
		}

		for(j = 0; j < gDictCount; j++){
			if(equalStrCase(buf, gDictionary[j]) == 0)
  				break;
		}
		if(j == gDictCount){
			fprintf(repfp, "%s\n", buf);
			missed++;
		}
	}  	
  }
  fclose(docfp);
  if(repfp != stdout)
  	fclose(repfp);
  
  /* read one word from document file, compare that against each 
  word in dictionary (i.e. gDictionary[k] , using a loop to change
  k from 0 to gDictCount-1. If a match found, it is a good word.
  **Otherwise, check for extra characters at the start and end of word.
  some examples are:  
   --hot -hot  peoples' cat's  it's  done. done? wow! wow!!!! "cold" 'day'
   You have to strip non a-z and A-Z chars from the front. Check, after 
   stripping the extra chars from the word from both ends.
  If no match at all, then show that word to report file (or to screen).
  When document file is completely checked, write statistics (how 
  many were checked, how many misspelled, etc..). 
 */
}

void Word_shift(char word[], int pos){
	int i = pos;
	for(; i < strlen1(word); i++){
		word[i] = word[i + 1];
	}
}

