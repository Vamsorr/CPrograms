
/*
Assignment: pa01 - Encrypting a plaintext file using the Vigenere cipher
|
| Author: Samuel Voor
| Language: c, c++, Java, go, python
|
| To Compile: javac pa01.java
| gcc -o pa01 pa01.c
| g++ -o pa01 pa01.cpp
| go build pa01.go
| python pa01.py
|
| To Execute: java -> java pa01 kX.txt pX.txt
| or c++ -> ./pa01 kX.txt pX.txt
| or c -> ./pa01 kX.txt pX.txt
| or go -> ./pa01 kX.txt pX.txt
| or python -> python pa01.py kX.txt pX.txt
| where kX.txt is the keytext file
| and pX.txt is plaintext file
|
| Note: All input files are simple 8 bit ASCII input
|
| Class: CIS3360 - Security in Computing - Fall 2022
| Instructor: McAlpin
| Due Date: 10/20/2022
*/


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define alphabet_len 26
#define buffsize 512

int main(int argc, char *argv[]) {
  
  int i;
  // open up key file
  FILE *key_file = fopen(argv[1], "r");
  // open up plaintext file
  FILE *plaintext_file = fopen(argv[2], "r");
  

  // buffer string to hold in the key
  char *buffsize_key = (char *)calloc(buffsize, sizeof(char));
  // buffer string to hold the plaintext
  char *buffsize_plaintext = (char *)calloc(buffsize, sizeof(char));
  

  // read in key file
  fgets(buffsize_key, buffsize - 1, key_file);
  // read in plaintext file
  fgets(buffsize_plaintext, buffsize - 1, plaintext_file);
  
  
  //variable holding length of the key
  int key_len = strlen(buffsize_key);
  //variable holding in the length of the plaintext
  int plaintext_len = strlen(buffsize_plaintext);

  //plaintext filter which removes all non-alphabetical values
  for(i = 0; i < plaintext_len; i ++){

    if(!isalpha(buffsize_plaintext[i])){
      buffsize_plaintext[i] = buffsize_plaintext[i + 1];
      plaintext_len --;
    }
  }
  //plaintext filter which converts all uppercase values to lowercase
  for(i = 0; i < plaintext_len; i ++){
    if(buffsize_plaintext[i] <= 90)
      buffsize_plaintext[i] + 32;
  }

  //key filter which removes all non-alphabetical values
    for(i = 0; i < key_len; i ++){

    if(!isalpha(buffsize_key[i])){

      buffsize_key[i] = buffsize_key[i + 1];
      key_len --;
    }
  }

  //key filter which converts all uppercase to lower case
  for(i = 0; i < key_len; i++){
    if(buffsize_key[i] <= 90)
      buffsize_key[i] + 32;
  }
  
    
    //declaring ciphertext
    char *ciphertext = (char *)calloc(plaintext_len, sizeof(char));
  
    //applying vigenere cipher to the plaintext
  for (i = 0; i < plaintext_len; i++) 
ciphertext[i] = (((buffsize_plaintext[i] + buffsize_key[i]) % 26) + 97);
  
  //printing ciphertext
  for(i = 0; i < 80; i++)
    printf("%c", ciphertext[i]);
    printf("\n");

  for(i = 80; i < 160; i ++)
    printf("%c", ciphertext[i]);
    printf("\n");

  for(i = 160; i < 240; i++)
    printf("%c", ciphertext[i]);
    printf("\n");

  for(i = 240; i < 320; i++)
    printf("%c", ciphertext[i]);
    printf("\n");

  for(i = 320; i < 400; i++)
    printf("%c", ciphertext[i]);
    printf("\n");

  for(i = 400; i < 480; i++)
    printf("%c", ciphertext[i]);
    printf("\n");

  for(i = 480; i < buffsize; i++)
    printf("%c", ciphertext[i]);
    printf("\n");
    
   

  // closing input files
  fclose(key_file);
  fclose(plaintext_file);

  
  //freeing arrays
  for(i = 0; i < plaintext_len; i++)
  free(ciphertext[i]);

  for(i = 0; i < key_len; i++)
  free(buffsize_key[i]);

  for(i = 0; i < plaintext_len; i++)
  free(buffsize_plaintext[i]);

  
  return 0;
}

/*
I Samuel Voor 3971335 affirm that this program is
| entirely my own work and that I have neither developed my code together with
| any another person, nor copied any code from any other person, nor permitted
| my code to be copied or otherwise used by any other person, nor have I
| copied, modified, or otherwise used programs created by others. I acknowledge
| that any violation of the above terms will be treated as academic dishonesty.
*/