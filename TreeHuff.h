#ifndef TREEHUFF_H
#define TREEHUFF_H
#include <stdio.h>
#include <stdlib.h>
#include "Usefull.h"



typedef struct Element{
	unsigned char letter; //caractere
	int occur; 			  //occurence du caractere
	char* codebin;		  //code binaire dans lequel le caractere sera encoder ou decoder
	int length;			  //longueur du char* codebin
}Element;


typedef struct Tree
{
    Element elmt;
    struct Tree *tleft;
    struct Tree *tright;
}Tree,*PtTree;

typedef struct List
{
    PtTree tree;
    struct List *next;
}List,*PtList;

typedef struct Cellule {
	PtTree elmt;
	struct Cellule *next;
}File;

typedef File *PtrFile;	



PtTree NewTree(Element elmt) ;

int TreeEmpty(PtTree arb) ;

int IsLeaf(PtTree arb);

PtTree FreeTree(PtTree arb);

void FreeCodeBinTab(Element* Tab, int n);

int HeightTree(PtTree arb);

int SizeTreeLeaf(PtTree arb) ;

void AffTab(Element *Tab, int n);

Element* TreeToTab(Element *Tab, PtTree arb, int* indice);

PtList RemoveFirst(PtList li);

void FreeList(PtList li);

void AffListofTree(PtList list);

void AffTree(PtTree arb, int prof);

void FileDev(PtTree tree, Element *Tab, int sizetab);

void AffTreeInFile(PtTree arb, int prof, FILE *filedev);

void AffTabInFile(Element *Tab, int n, FILE* filedev);



#endif
