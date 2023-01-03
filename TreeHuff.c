#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Usefull.h"
#include "TreeHuff.h"

PtTree NewTree(Element elmt) {
	PtTree a;
	if((a = MALLOC(Tree)) == NULL){
		fprintf(stderr, "ERREUR ALLOCATION MEMOIRE");
		exit(1);
	}
	a->elmt = elmt ;
	a->tleft = NULL;
	a->tright = NULL;
	//a->eqlbr = 0;
	return a;
}

int TreeEmpty(PtTree arb) {
	return ( arb == NULL);
}

int FileVide(PtrFile f) {
	return ( f == NULL);
}

int IsLeaf(PtTree arb) {
	if(TreeEmpty(arb)){
		return -1;
	}
	else if(TreeEmpty(arb->tleft) && TreeEmpty(arb->tright)){
		return 1;
	}
	else {
		return 0;
	}
}


PtTree FreeTree(PtTree arb){
	if(!TreeEmpty(arb)){
		if(!TreeEmpty(arb->tleft))
			arb->tleft=FreeTree(arb->tleft);
		if(!TreeEmpty(arb->tright))
			arb->tright=FreeTree(arb->tright);
		free(arb);
	}
	return NULL;
}
 
void FreeCodeBinTab(Element* Tab, int n){
	for(int i =0; i<n;i++){
		free(Tab[i].codebin);
	}
	return;
}

		
PtrFile EnfilerArb(PtrFile f, PtTree arb){
	if(FileVide(f)){
		PtrFile f;
		if((f = MALLOC(File)) == NULL){
			fprintf(stderr, "ERREUR ALLOCATION MEMOIRE");
			exit(1);
		}
		f->elmt=arb;
		f->next=NULL;
		return f;
	}
	else {
		f->next = EnfilerArb(f->next, arb);
		return f;
	}
}


PtrFile Defiler(PtrFile f) {
	if(!FileVide(f)){
		PtrFile tmp=f->next;
		free(f);
		return tmp;
	}
	else {
		return NULL;
	}
}

PtTree TeteFile(PtrFile f) {
	if (FileVide(f)){
		return NULL;
	}
	else {
		return f->elmt;
	 }
 }



int HeightTree(PtTree arb) {
	if (TreeEmpty(arb)) {
		return -1;
	}
	else {
		return (1 + Max(HeightTree(arb->tleft) , HeightTree(arb->tright)));
	}
}

int SizeTreeLeaf(PtTree arb) { //NbDeNoeuds aussi

	if(TreeEmpty(arb)){
		return 0;
	}
	else if(IsLeaf(arb)){
		return 1;
	}
	else {
		return ( SizeTreeLeaf(arb->tleft) + SizeTreeLeaf(arb->tright));
	}
}



void ClearNode(PtTree arb){
	if(TreeEmpty(arb)){
		return;
	}
	else{
		ClearNode(arb->tleft);
		ClearNode(arb->tright);
		free(arb);
	}
	
}


int NbFeuille(PtTree arb) {
	if(TreeEmpty(arb)) {
		return 0;
	}
	else if(IsLeaf(arb)) { 
		return 1;
	}
	else {
		return (NbFeuille(arb->tleft) + NbFeuille(arb->tright));
	}
}







void AffTab(Element *Tab, int n){
	int i;
	for(i=0 ; i<n ; i++){
		if(Tab[i].occur != 0){
			if ((int)Tab[i].letter != 10 && (int)Tab[i].letter != 13){ // 13 = code ascii du retour chariot et 10 = code ascii de Fin de ligne LF\n
				printf("|[%c]:[%d]:[%s]:[%d]|", Tab[i].letter, Tab[i].occur, Tab[i].codebin, Tab[i].length);
				printf("\n"); 
			}
			else { 
				
				if ((int)Tab[i].letter == 10){
					printf("|[LF]:[%d]:[%s]:[%d]|", Tab[i].occur, Tab[i].codebin, Tab[i].length);
				}
				else{
					printf("|[\\n]:[%d]:[%s]:[%d]|", Tab[i].occur, Tab[i].codebin, Tab[i].length);
				}
				printf("\n");
				
			}
			
		} 
	}
}


Element* TreeToTab(Element *Tab, PtTree arb, int* indice){
	if (!TreeEmpty(arb)){
		if(IsLeaf(arb)){
			Tab[*indice] = arb->elmt ; 
		}
		if(!TreeEmpty(arb->tleft) && IsLeaf(arb->tleft)){
			*indice = (*indice)+1; //printf(" %d ", *indice);
			TreeToTab(Tab, arb->tleft, indice);
		}
		else {
			TreeToTab(Tab, arb->tleft, indice);
		}	
		if(!TreeEmpty(arb->tright)&& IsLeaf(arb->tright)){
			*indice = (*indice)+1; //printf(" %d ", *indice);
			TreeToTab(Tab, arb->tright, indice);
		}
		else {
			TreeToTab(Tab, arb->tright, indice);
		}
	}
	return Tab;
} 


void AffListofTree(PtList list){
	if(list == NULL){
		return;
	}
	else{
		AffTree(list->tree,0);
		printf("\n-----------------------------------------\n");
		AffListofTree(list->next);
	}
}

PtList RemoveFirst(PtList li){
	if(li==NULL){
		return NULL;
	}
	else{
		PtList tmp = li;
		li = li->next;
		free(tmp);
		return li;
	}
}

void FreeList(PtList li){
	if(li==NULL){
		return;
	}
	else{
		free(li);
		FreeList(li->next);
		return;
	}
}

void AffTree(PtTree arb, int prof)
{
	if(!TreeEmpty(arb) && arb->elmt.occur >0){
   	    int i;
    	for (i=0; i < prof-1; i++)
   		 {
   	    	 printf("|   ");
   		 }

		if(prof != 0){
			printf("|___");
 
		}
		
		if ( (int) arb->elmt.letter == 10 ){
   			printf("[LF][%d][%s]\n", arb->elmt.occur,arb->elmt.codebin);
		}
		else if ( (int) arb->elmt.letter == 13 ){
   			printf("[\\n][%d][%s]\n", arb->elmt.occur,arb->elmt.codebin);
		}
		else
		{
			printf("[%c][%d][%s]\n", arb->elmt.letter , arb->elmt.occur , arb->elmt.codebin);
		}
   		if (!TreeEmpty(arb->tleft)){
			AffTree(arb->tleft, prof + 1);
		}
		if(!TreeEmpty(arb->tright)){
			AffTree(arb->tright, prof + 1);
		}
	}
}


//fonction d'ecriture du fichier dev.txt __________________________________________________________

void FileDev(PtTree tree, Element *Tab, int sizetab){ 
	FILE *filedev = fopen("dev.txt", "w+");
	AffTabInFile(Tab, sizetab, filedev);
	fprintf(filedev, "\n________________________________________________________________________________\n\n");
    AffTreeInFile(tree, 0, filedev);
	fclose(filedev);
}

void AffTreeInFile(PtTree arb, int prof, FILE* filedev)
{
	
	if(!TreeEmpty(arb) && arb->elmt.occur >0){
   	    int i;
    	for (i=0; i < prof-1; i++)
   		 {
   	    	 fprintf(filedev, "|   ");
   		 }

		if(prof != 0){
			fprintf(filedev, "|___");
 
		}
		
		if ( (int) arb->elmt.letter == 10 ){
   			fprintf(filedev, "[LF][%d][%s]\n", arb->elmt.occur,arb->elmt.codebin);
		}
		else if ( (int) arb->elmt.letter == 13 ){
   			fprintf(filedev, "[\\n][%d][%s]\n", arb->elmt.occur,arb->elmt.codebin);
		}
		else
		{
			fprintf(filedev, "[%c][%d][%s]\n", arb->elmt.letter , arb->elmt.occur , arb->elmt.codebin);
		}
   		if (!TreeEmpty(arb->tleft)){
			AffTreeInFile(arb->tleft, prof + 1, filedev);
		}
		if(!TreeEmpty(arb->tright)){
			AffTreeInFile(arb->tright, prof + 1, filedev);
		}
	}
}

void AffTabInFile(Element *Tab, int n, FILE* filedev){
	
	int i;
	for(i=0 ; i<n ; i++){
		if(Tab[i].occur != 0){
			if ((int)Tab[i].letter != 10 && (int)Tab[i].letter != 13){ // 13 = code ascii du retour chariot et 10 = code ascii de Fin de ligne LF\n
				fprintf(filedev, "|[%c]:[%d]:[%s]:[%d]|", Tab[i].letter, Tab[i].occur, Tab[i].codebin, Tab[i].length);
				fprintf(filedev, "\n"); 
			}
			else { 
				
				if ((int)Tab[i].letter == 10){
					fprintf(filedev, "|[LF]:[%d]:[%s]:[%d]|", Tab[i].occur, Tab[i].codebin, Tab[i].length);
				}
				else{
					fprintf(filedev, "|[\\n]:[%d]:[%s]:[%d]|", Tab[i].occur, Tab[i].codebin, Tab[i].length);
				}
				fprintf(filedev, "\n");
				
			}
			
		} 
	}
}