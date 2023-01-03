#include <stdio.h>
#include <stdlib.h>
#include "Usefull.h"


int Min(int a, int b) {
	if (a >= b ) {
		return b ;
	}
	else {
		return a ; 
	}
}


float MinFloat(float a, float b) {
	if (a >= b ) {
		return b ;
	}
	else {
		return a ; 
	}
}

int Max(int a, int b) {
	if (a >= b ) {
		return b ;
	}
	else {
		return a ; 
	}
}
 
 int Pow(int a, int b){
	int c=1;
	for(int i=0; i<b; i++){
		c= c*a;
	}
	return c;
}

float PowF(float a, float b){
	float c=1;
	for(int i=0; i<b; i++){
		c= c*a;
	}
	return c;
}

float MaxF(float a, float b) {
	if (a >= b ) {
		return a ;
	}
	else {
		return b ; 
	}
}


int NbChiffre(int nb) {
    int chiffre = 1;
    int bool = 0;
    while(!bool){
        if(nb>-10 && nb<10){
            bool = 1;
        }
        else{
            nb = nb/10;
            chiffre++;
        }
    }
    return chiffre;
}

int CharToInt(char car){
    return (int) car - '0';
}

void CompressionRatio(char* inputfilename,char* outputfilename)
{
	FILE *input = fopen(inputfilename,"r");
	FILE *output = fopen(outputfilename,"r");
	float tailleF1 = 0;
	float tailleF2 = 0;
	float ratio = 0;
	while(getc(input) != EOF)
		tailleF1++;
	while(getc(output) != EOF)
		tailleF2++;
	
	ratio = (100*tailleF2) / tailleF1;
	printf("\n%.0fkb(%s) compressed to %.0fkb(%s) (%.2f%%).\n\n",(tailleF1/1000),inputfilename,(tailleF2/1000),outputfilename,ratio);
}