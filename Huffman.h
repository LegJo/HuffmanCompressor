#ifndef __HUFFMAN__H__
#define __HUFFMAN__H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TreeHuff.h"
#define SIZE 256
#define SIZE_BIN 40


//Prototype's definition for Huffman
void CryptFile(char* inputfilename, char* outputfilename, Bool dev);
void DecryptFile(char* inputfilename,char* outputfilename, Bool dev);


void OccurenceSort(Element* array, char* input); 
void SelectionSort(Element *array, int n); 
void ArrayCreation(Element* array);
PtList ListOfTreeOrdered(Element *array,int size);
PtList ListTreeOrder(PtList li, PtTree tr);
PtTree JoinTree(Element array, PtTree tleft, PtTree tright);
PtTree HuffmanTree(PtList li);
void CodeBin(PtTree tree, char* codebin);
void LengthBin(Element* Tab, int n);
void WriteHead(Element *array, char* outputfilename,int n);
void WritingInFile(Element *FinalTab,Element *array,char* inputfilename,char* outputfilename,int n);
void WritingInBin(char* outputfilename);
char* DecIntToCharBin(int decimal);
Element* ReadHead(FILE* f,int sum);
int sizeTabHead(FILE* file);
void writeInDecrypt(FILE* file, PtTree tree,char* outputfilename);


#endif
