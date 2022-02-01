#include<stdio.h>
#include<string.h>
#include <stdlib.h>

// changed it up a bit. I realized it could be more structured outside of main
// I like my main to be clean and I feel like this gives it a little
// bit cleaner look
struct keyWord{

    char word[20];
    int count;
};

// compares the token with the keyword at word.
void compare(struct keyWord keyWords[], int length, char *tok){

    for(int i = 0; i < length; i++){
        if(strcmp(tok, keyWords[i].word) == 0){
            keyWords[i].count++;
        }
        
    }
}

// copies the key to the arugment
void copy(struct keyWord key[], int length, char *arg[]){

    for(int i = 0; i < length; i++){
        strcpy(key[i].word, arg[i + 1]);
        key[i].count = 0;
    }
}

// created a function for display words. mainly because
// i feel that this one could be used again in the future
void display_words(struct keyWord key[], int length){

    for( int i = 0; i < length; i++){

        printf("\nWord: %s Count: %d\n", key[i].word, key[i].count);
    }
    
}


int main(int argc, char *argv[]){

    int count;

    // makes sure there are inputs given
    if(argc == 1){

        printf("No commands given \n");
    }
    else{

        // begins at count = 1 to not print a.out
        printf("Words inputted:");
        for(count = 1; count < argc; count++){

            printf("\nargv[%d]:%s\n", count, argv[count]);
        }
    }


    // stores a spot for the a.out so we take one away
    int kwInput = argc - 1;
    
    // dynamically allocates the size for the keywords inputted
    struct keyWord *keyWords = malloc(kwInput * sizeof(struct keyWord));

    // iterates and copies the word at i in keywords to the argv[i+1]
    // we add one because of the - 1 in the original problem


    copy(keyWords, kwInput, argv);

    // allocates size for each line
    char *line = malloc(100 * sizeof(char));

    // stores the maximum size of any type of variable
    size_t len;


    // makes sure there are still lines in the input
    // does this with the & symbol getting the actual value
    
    while(getline(&line, &len, stdin) != -1){
        
        // allocates that the whitespace and new lines are accounted for in the files.
        char *hold  = strtok(line, " ");
        
        while(hold){
            
            compare(keyWords,kwInput, hold);
            // resets the token value back to null after a line is done
            hold = strtok(NULL, " ");
        }   
    }
    // prints for the length of the input the key words that one is looking for and their count    
    display_words(keyWords, kwInput);
}


