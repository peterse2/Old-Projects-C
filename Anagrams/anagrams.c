/************************************************************************
 * filename: anagrams.c													*
 *																		*
 * Reads from a dictionary of words and builds lists of anagrams		*
 * (see example dictionary files and output produced for each 			*
 * input/dictionary file).												*
 *																		*
 * An anagram is a word formed by rearranging the letters of another	*
 * word such as the word "cinema" formed from the word "iceman".		*
 *																		*
 * Author(s): Caitlin Crowe and Emily Peterson			              	*
 ***********************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_ARRAY_SIZE 1000000
#define MAX_WORD_SIZE 50

// node in a linked list
struct node {
    char *text;
    struct node *next;
};

typedef struct node Node;

// structure used for an array element
//		- head field points to the first element of a linked list
//		- size field stores number of nodes in the linked list
typedef struct {
    int size;
    Node *head;
} AryElement;

/************************************************************************
 * YOU MUST NOT DEFINE ANY GLOBAL VARIABLES (i.e., OUTSIDE FUNCTIONS).  *
 * COMMUNICATION BETWEEN FUNCTIONS MUST HAPPEN ONLY VIA PARAMETERS.     *
 ************************************************************************/

/************************************************************************
 * Function declarations/prototypes										*
 ************************************************************************/

AryElement *buildAnagramArray(char *infile, int *aryLen);

void printAnagramArray(char *outfile, AryElement *ary, int aryLen);

void freeAnagramArray(AryElement *ary, int aryLen);

bool areAnagrams(char *word1, char *word2);

Node *createNode(char *word);


/************************************************************************
 * Main driver of the program.											*
 * The input file is assumed to contain one word (of lower case			*
 * letters only) per line.												*
 * Use the following commands to compile and run the program:			*
 *		$ gcc -Wall -std=c99 -o anagrams anagrams.c						*
 * 		$ ./anagrams  dictionary1.txt  output1.txt						*
 ************************************************************************/
int main(int argc, char *argv[])
{
    AryElement *ary;
    int aryLen;
    
    if (argc != 3) {
        printf("Wrong number of arguments to program.\n");
        printf("Usage: ./anagrams infile outfile\n");
        exit(EXIT_FAILURE);
    }
    
    char *inFile = argv[1];
    char *outFile = argv[2];
    
    ary = buildAnagramArray(inFile,&aryLen);
    
    printAnagramArray(outFile,ary,aryLen);
    
    freeAnagramArray(ary,aryLen);
    
    return EXIT_SUCCESS;
}

/************************************************************************
 * Takes a filename that contains one word (of lower case letters) per	*
 * line, reads the file contents, and builds an array of linked lists	*
 * and returns a pointer to this array. It also sets the aryLen 		*
 * parameter to size/length of the array returned from the function.	*
 ************************************************************************/
AryElement *buildAnagramArray(char *infile, int *aryLen)
{
    AryElement *ary = calloc(INITIAL_ARRAY_SIZE, sizeof(AryElement)); // stores pointer to dynamically allocated array of structures
    char word[MAX_WORD_SIZE]; // stores a word read from the input file
    int curAryLen = 1; // stores current length/size of the array
    int nbrUsedInAry = 0; // stores number of actual entries in the array//# of total nodes
    int i;
    bool added;
    
    
    // prepare the input file for reading
    FILE *fp = fopen(infile,"r");
    if (fp == NULL) {
        fprintf(stderr,"Error opening file %s\n", infile);
        exit(EXIT_FAILURE);
    }
    
    while(fgets(word, MAX_WORD_SIZE, fp) != NULL){
        word[strlen(word)-1] = '\0';
        added = false;
        for(i=0; i<nbrUsedInAry; i++) {
            if(areAnagrams(word, ary[i].head->text) == 1){
                Node *tempNode = ary[i].head;
                while(tempNode->next != NULL){
                    tempNode = tempNode->next;
                }
                tempNode->next = createNode(word);
                ary[i].size++;
                added = true;
            }
        }
        if(added==false){
            if(curAryLen == nbrUsedInAry){
                curAryLen = 2 * curAryLen;
                ary = realloc(ary, curAryLen);
            }
            ary[nbrUsedInAry].size = 0;
            ary[nbrUsedInAry].head = createNode(word);
            nbrUsedInAry++;
        }
    }
    ary = realloc(ary, nbrUsedInAry);
    
    fclose(fp);
    *aryLen = nbrUsedInAry;
    return ary;
}

/************************************************************************
 * Takes a filename used for output, a pointer to the array, and size	*
 * of the array, and prints	the list of anagrams (see sample output) 	*
 * to the file specified.												*
 ************************************************************************/
void printAnagramArray(char *outfile, AryElement *ary, int aryLen)
{
    FILE *fp = fopen(outfile, "w");
    if (fp == NULL) {
        fprintf(stderr,"Error opening file %s\n", outfile);
        exit(EXIT_FAILURE);
    }
    int i;
    for(i=0; i<aryLen; i++){
        Node *tempNode = ary[i].head;
        if(tempNode->next != NULL){
            fprintf(fp, "%s ", tempNode->text);
            while(tempNode->next != NULL){
                tempNode = tempNode->next;
                fprintf(fp, "%s ", tempNode->text);
                
            }
            fprintf(fp, "\n");
        }
    }
    fclose(fp);
}

/************************************************************************
 * Releases memory allocated for the array and all the linked lists.	*
 * This involves releasing the memory allocated for each Node of each	*
 * linked list and the array itself. Before freeing up memory of a Node *
 * object, make sure to release the memory allocated for the 			*
 * "text" field of that node first.										*
 ************************************************************************/
void freeAnagramArray(AryElement *ary, int aryLen)
{
    Node *ptr, *tmp;
    int i;
    for (i = 0; i < aryLen; i++) {
        if (ary[i].size > 0) {
            ptr = ary[i].head;
            while (ptr != NULL) {
                free(ptr->text);
                tmp = ptr->next;
                free(ptr);
                ptr = tmp;
            }
        }
    }
    free(ary);
}

/************************************************************************
 * Allocates memory for a Node object and initializes the "text" field	*
 * with the input string/word and the "next" field to NULL. Returns a	*
 * pointer to the Node object created.									*
 ************************************************************************/
Node *createNode(char *word)
{
    Node *node = malloc(sizeof(struct node));
    char *temp = malloc(sizeof(strlen(word)+1));
    strcpy(temp, word);
    
    node->text = temp;
    node->next = NULL;
    
    return node;
}

/************************************************************************
 * Returns true if the input strings are anagrams, false otherwise.		*
 * Assumes the words contain only lower case letters.					*
 ************************************************************************/
bool areAnagrams(char *word1, char *word2)
{
    int len1 = 0, len2 = 0, i, x1 = 0, x2 = 0, z;
    len1 = strlen(word1);
    len2 = strlen(word2);
    
    if(len1 == len2){ //if the two words are not the same length, then they cannot be anagrams
        for(i=0; i<len1; i++){
            z = word1[i] * word1[i];
            x1 = z + x1; //get ascii of the letters and add them together
        }
        for(i=0; i<len2; i++){
            z = word2[i] * word2[i];
            x2 = z + x2; //get ascii of the letters and add them together
        }
        if(x1 == x2){ //if the ascii values are the same, and the word length is the same, the words are anagrams
            return 1;
        }
    }
    
    return 0;
}
