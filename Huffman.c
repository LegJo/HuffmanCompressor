#include "Usefull.h"
#include "TreeHuff.h"
#include "Huffman.h"


//fonction d'encodage d'un fichier
void CryptFile(char* inputfilename,char* outputfilename, Bool dev){    
    printf("\nCompressing %s...\n",inputfilename);
    Element array[SIZE];                                               // Creation du tableau de caracteres
    OccurenceSort(array, inputfilename);                               //recuperation du nombre d'occurences de caractere du fichier 
    //AffTab(array,SIZE);
    PtList list = ListOfTreeOrdered(array,SIZE);                       //creation d'un liste d'arbre ayant pour racine les ELements de chaque caracteres present dans le fichier ainsi que leur occurences
    //AffListofTree(list);
    PtTree FinalTree = HuffmanTree(list);                              //obtention de l'arbre Huffman grace a la List
    char* codebin = (char*)malloc(SIZE_BIN*sizeof(char));
    *codebin = '\0';
    CodeBin(FinalTree, codebin);                                       //ecriture du code binaire d'encoding de chaque caractere de l'arbre     

    int sizetab = SizeTreeLeaf(FinalTree);
    Element* FinalTab = (Element*)(malloc((sizetab)*sizeof(Element)));
    int* indice = (int*)malloc(sizeof(int));
    *indice = -1;
    FinalTab = TreeToTab(FinalTab,FinalTree, indice);                  //Mise en tableau des feuilles de l'arbre pour une meillleur gestion et lisibilite
    SelectionSort(FinalTab, sizetab);                                  //trie de ce tableau
    LengthBin(FinalTab, sizetab);                                      //ecriture des longueurs des code binaires de chaque caractere du tableau        
    if(dev == true){
        FileDev(FinalTree, FinalTab, sizetab);                         //ecriture du fichier dev.txt si argv[5] est "-dev"
    }
    WritingInFile(FinalTab,array,inputfilename, outputfilename, sizetab);   //ecodage dans le fichier output
    CompressionRatio(inputfilename,outputfilename);                         //calcule du ration entre les taille du fichier input et output

    if(dev == false){
        system("rm tmp.txt");
    }
    FreeTree(FinalTree);
    FreeCodeBinTab(array,SIZE); 
    FreeCodeBinTab(FinalTab,sizetab);
    free(indice);
    free(codebin);
    return;
}

//fonction de décodage d'un fichier
void DecryptFile(char* inputfilename,char* outputfilename, Bool dev)
{
    int sizeTab;
    FILE *file = fopen(inputfilename,"rb");                // On ouvre le fichier compressé.
    sizeTab = sizeTabHead(file);                           // On récupère la taille necessaire pour le tableau(soit le nombre de lettre)
    Element *TabDecom = ReadHead(file,sizeTab);            // On récupère le tableau.
    PtList list =  ListOfTreeOrdered(TabDecom,sizeTab);    // On créer ici une liste d'arbre avec les elements du tableau
    PtTree tree = HuffmanTree(list);                       // On recréer l'arbre de compression via la liste.
    if(dev == true){
        FileDev(tree, TabDecom, sizeTab);             //ecriture du fichier dev.txt si argv[5] est "-dev"
    }
    writeInDecrypt(file,tree,outputfilename);              // On ecrit dans le fichier outpur de decompression a l'aide de l'arbre.
    fclose(file);
    FreeCodeBinTab(TabDecom, sizeTab);
    free(TabDecom);
    FreeTree(tree);
}


void SelectionSort(Element *array, int n ){                 // Cette fonction permet de trier un tableau d'Element selon l'occurence des caractères
    int i,j;
    int min;
    Element tmp;
    for(i=0;i<n;i++)
    {
        min = i;                                                                            
        for(j=i+1;j<n;j++)                                               
        {
            if(array[j].occur < array[min].occur)
                min = j;
        }
        tmp = array[i];                                      
        array[i] = array[min];
        array[min] = tmp;
    }
}

void OccurenceSort(Element* array, char* input ){
    FILE *file = fopen(input,"r+");                                       // On ouvre le fichier a compresser
    ArrayCreation(array);                                                 // On créer le tableau qui contient tous les caractère ascii (de 0 a 255)

    int letter = 0;
    while(letter != EOF)                                                  // On parcourt tous le fichier
    { 
        letter = fgetc(file);
        if((letter<0 && letter != EOF)|| (letter>256 && letter != EOF))   // Sécurité
        {
            letter = 63;
        }
        array[letter].occur++;                                            // On ajoute 1 a l'occurrence de la lettre obtenu
    }    
    fclose(file);
    SelectionSort(array, SIZE);                                           // On trie le tableau obtenue
}

void ArrayCreation(Element* array){                                       // Cette fonction créer notre tableau de base
    for(int i = 0; i<SIZE ; i++)
    {
        array[i].letter = (unsigned char)i; 
        array[i].occur = 0;
        array[i].codebin =(char*) malloc(SIZE_BIN*sizeof(char));
        *array[i].codebin = '\0';
        array[i].length = -1;

    }
}



PtList ListOfTreeOrdered(Element *array,int size)                         // Cette fonction va créer une liste (en ordre croissant) d'arbre. Chaque arbre aura comme racine une lettre et un occurrence de notre tableau
{
    PtList listOfTree = NULL;
    for(int i=0; i<size;i++)                                              
        if(array[i].occur != 0){
            listOfTree = ListTreeOrder(listOfTree,NewTree(array[i]));     // On ajoute de facon trié un nouvel arbre
        }
    return listOfTree;
}


PtList ListTreeOrder(PtList li, PtTree tr)                                // Cette fonction ajout un élément dans un liste deja triée. 
{
    List *element = malloc(sizeof(*element));

    if(element == NULL)
    {
        printf("ERROR_ALLOCATION");
        exit(EXIT_FAILURE);
    }

    element->tree = tr;
    element->next = NULL;

    if(li == NULL)
        return element;
    if(li->next == NULL)
    {
        if(li->tree->elmt.occur < tr->elmt.occur)
        {
            li->next = element;
            return li;
        }
        else
        {
            element->next = li;
            return element;
        }
    }
    List *tmp = li;
    while(tmp->next != NULL && tmp->next->tree->elmt.occur < tr->elmt.occur) // On trie par rapport au nombre d'occurence donc on doit atteindre l'occurence
        tmp = tmp->next;
    if(tmp->next != NULL)
        element->next = tmp->next;                                        
    tmp->next = element;
    return li;
}


PtTree JoinTree(Element array, PtTree tleft, PtTree tright){              // Cette fonction fusionne des arbre. On envoit le fils gauche et droit(des arbres) qu'on ajoute a une nouvelle racine
    PtTree tr = NewTree(array);
    tr->tleft = tleft; 
    tr->tright = tright;
    return tr;
}

PtTree HuffmanTree(PtList li){                                            // On créer l'arbre de Huffman
	Element elmt;                                                         // On initialise un element temporaire
	elmt.occur = 0;
	elmt.letter = '\0';
    elmt.codebin =(char*) malloc(SIZE_BIN*sizeof(char));
    *elmt.codebin = '\0';
    elmt.length = -1;

	while(li->next != NULL){                                              // On continue jusqu'a ce qu'il reste que 1 seul arbre => notre arbre Huffman
		elmt.occur = li->tree->elmt.occur + li->next->tree->elmt.occur ;  // Comme notre list est triée, les deux premiers elements sont les plus petits. On ajout donc la somme de leur occurrence a notre element temporaire
		PtTree tmp = JoinTree(elmt, li->tree, li->next->tree);            // On fusionne les deux premier arbre de la liste avec comme racine notre element qui contient la somme de leur occurrence 
        li = RemoveFirst(RemoveFirst(li));                                // Suppression des 2 1er elements de la liste
        li=ListTreeOrder(li,tmp);                                         // On ajoute l'arbre créé dans la liste.
	}
    return li->tree;                                                      // On retourne l'arbre final
} 


void CodeBin(PtTree tree, char* codebin){                                //Fonction qui va créer et assigner un code binaire à chaque caractere en fonction du nombre d'occurence
    char* tmp=(char*) malloc(SIZE_BIN*sizeof(char));
    *tmp= '\0';
    if(TreeEmpty(tree)){                                                //condition d'arret de la recurrence
        return ;
        
    }
    else if(IsLeaf(tree)){                                              //si l'arbre est une feuille alors il contient un caractere, assignation de codebin en tant que son code binaire
        
        strcpy(tree->elmt.codebin,codebin);                             
        return ;
    }
    else {
        
        strcpy(tree->elmt.codebin,"N");                     //si l'arbre n'est pas une feuille alors on lui met un codebin par default et on transmet à son fils droit et gauche le codebin, concatener avec 0 ou 1.
        strcpy(tmp,codebin);
        CodeBin(tree->tleft, strcat(codebin,"1"));
        strcpy(codebin,tmp);
        CodeBin(tree->tright, strcat(codebin,"0"));
    }
    free(tmp);
}

void LengthBin(Element* Tab, int n){
    for(int i=0; i<n; i++){
        Tab[i].length = strlen(Tab[i].codebin);
    }
    return;
}


void WriteHead(Element *array, char* outputfilename, int n){           //Fonction d'écriture de l'en-tête dans le fichier output
    FILE *result = fopen(outputfilename,"w+");                         // ouverture du fichier qui doit contenir la compression
    fprintf(result, "%d ", n);                                         // On écrit le nombre de lettre necessaire
    for (int i=0 ; i<SIZE; i++){                                       // On écrit tous les caractère sauf ceux qui on 0 occurrence
        if(array[i].occur != 0){
            fprintf(result,"%d ",array[i].letter);
            fprintf(result, "%d ", array[i].occur);
        }
    }
    fprintf(result,"$");                                  // Sécurité pour bien séparer
    fclose(result);                                       // On ferme le resultat
}

void WritingInFile(Element *FinalTab,Element *array,char* inputfilename,char* outputfilename,int n) //fonction d'ecriture en ASCII de l'encodage dans le fichier tmp.txt               
{
    FILE *tmp = fopen("tmp.txt","w+");                    // Ouverture du fichier qui contiendra les valeurs de chaque caractère (mais en texte, pas binaire)
    FILE *read = fopen(inputfilename,"r");                // Ouverture du fichier a compresser.
    
    int car = 0;
    int i;
    char bin0 = '0' ;
    char bin1 = '1' ;

    while(car != EOF)                                      // Condition pour aller jusqu'à la fin du texte.
    {
       
        car = fgetc(read);                                 // On recupère les caractères du fichier
        i = n-1;                                           // On initialise i à n-1 donc la fin du tableau(la ou les occurrences sont les plus grandes) pour gagner un peu de temps.
        if((car<0 && car != EOF)||(car>256 && car != EOF)) // Sécurité
            car = 63;
        while(FinalTab[i].letter != car && i>=0)           // Sécurité           
            i--;
        if(i<0 || i>n-1)
            continue;
        
       
        for(int j = 0;j<FinalTab[i].length;j++)            // Cette boucle permet d'ecrire dans le fichier tmp. Si a = 001 alors on écrit 001 en texte.
        {                                                  // Pour ca on va prendre 001 et le décomposer en 0  0  1 et regarder chaque terme.
            if(FinalTab[i].length <= j){                   // Sécurité
                exit(-1);
            }
            else if(*(FinalTab[i].codebin+j) == bin0){     // Si 0 on écrit 0
                    fwrite(&bin0,1,1,tmp);
            }
            else{
                    fwrite(&bin1,1,1,tmp);                 // Si 1 on écrit 1
            }
            
        }
        
        
    }
    fclose(tmp);                                           // fermeture de tmp   
    WriteHead(array, outputfilename, n);                   // Appel de la fonction permettant d'ecrire l'entete du fichier binaire qui contiendra les infos pour decompresser/desencoder. 
    WritingInBin(outputfilename);                          // Appel de la fonction qui ecrit en binaire en regardant tmp (après l'entete).
    fclose(read);
    return;
}


void WritingInBin(char* outputfilename)                     //Fichier de conversion du fichier tmp.txt en binaire Dans le fichier output
{
    FILE *result = fopen(outputfilename,"ab");             // On réouvre resultat mais ici avec ab pour ecrire en binaire après l'entete.
    FILE *tmpFile = fopen("tmp.txt","r+");                 // Ouverture du fichier tmp 
    char mot[8];                                           // Variable pour récuperer des paquet de 8 lettre 
    char tmp = '\0';
    int sum = 0;
    int i;
    while(tmp != EOF)                                      // On regarde tous le fichier
    {
        sum = 0;
        for(i=0;i<8;i++)                                   // recuperation de 8 char dans mot
        {

            tmp = getc(tmpFile);                           // On récupère le caractère.                  
            if(tmp == EOF)                                 // comblage du dernier paquet de 8 si <8 par des 0
            {
                mot[i] = '0';                              
            }
            else
            {
                if(tmp == 49)                              // Si on a 1 alors on ajoute 1 à mot sinon 0
                    mot[i] = '1';
                else
                    mot[i] = '0';
            }
        }

        
        for(int j=0;j<8;j++)                               //transformation du mot en int (decimal). 
        {
            if(*(mot+j) == '1')
                sum += Pow(2,7-j);                         
        }
        fwrite(&sum,1,1,result);                           // Une fois l'entier recuperé on l'envoit sur 1 octet dans le fichier compressé. 
    }
    fclose(tmpFile);
    fclose(result);
}

char* DecIntToCharBin(int decimal){                        // Cette fonction permet de transformer un int en sont equivalent binaire(mais en char) donc si on envoit 13 on obtient un mot qui vaut 00001101
    char* bin=(char*)malloc(8*sizeof(char));               
    *(bin+8) ='\0';
    for(int i=7; i>=0; i--){
        if(decimal % 2 == 0)
            *(bin+i)= '0';
        else 
            *(bin+i)= '1';
        decimal = decimal / 2;
    }
    return bin;
}

Element* ReadHead(FILE *f,int sum)                         // Cette fonction lit l'entete du fichier binaire, afin de récuperer le tableau de caracteres et leur occurrence.
{
    int sum2;
    unsigned char tmp;
    Element *TabDecom = (Element*)(malloc((sum)*sizeof(Element))); // Allocation du tableau de taille sum, lu en entete
    for(int i=0;i<sum;i++){                                      //lecture de l'entete et recuperation du tableau de char et d'occurence                      
        for(int j=0;j<2;j++){                              
            fread(&tmp,1,1,f);                             
            sum2 = 0;                                     
            while(tmp != ' '){                                  //lecture de l'ascii du caractère puis du nb d'occurence  
                if(tmp != ' ')                             
                    sum2 = sum2*10 + CharToInt(tmp);            
                fread(&tmp,1,1,f);                         
            }
            if(j==0)                                       // Dans notre entete on commence par la lettre puis son nombre d'occurrence. Donc la premiere boucle correspond a la lettre et la seconde (le else) à l'occurrence.
                TabDecom[i].letter = sum2;
            else{
                TabDecom[i].occur = sum2;
                TabDecom[i].codebin = (char*) malloc(SIZE_BIN*sizeof(char));
                *TabDecom[i].codebin = '\0';
                TabDecom[i].length = -1;
            }
        }
    }
    return TabDecom;
}



int sizeTabHead(FILE* file)                                // Cette fonction lit le nombre de caractère necessaire (qui est écrit dans l'entete)
{                                                          // Pour comprendre la méthode voir commentaire fonction ReadHead, c'est exactement la même
    char tmp;
    int sum = 0;
    while(tmp != ' '){
        fread(&tmp,1,1,file);
        if(tmp != ' ')
            sum = sum*10 + CharToInt(tmp);
    }
    return sum;
}

void writeInDecrypt(FILE* file, PtTree tree,char* outputfilename)  // fonction d'criture du texte décoder dans le fichier input crypté vers le fichier ouput
{
    FILE* Result = fopen(outputfilename,"w+");             // On ouvre le fichier qui sera décompressé.
    int tmp;
    char *recupChar;
    int total = tree->elmt.occur;
    PtTree tmpTree = tree;
    tmp = getc(file);                                      // Ce getc recupère $ qui est notre sécurité pour séparer entete et texte (elle permet de mieux savoir ou se situe une erreur)
    while(tmp != EOF && total > 0)                                      // On lit le fichier entier
    {
        tmp = getc(file);                                  // On recupère un entier ici qui correspond au caractère. par exemple si on lit 0 alors on récupère 48
        recupChar = DecIntToCharBin(tmp);                  // On transforme cette entier en char. Par exemple on envoit 13 et on récupère mot = 00001101
        for(int i=0;i<8;i++)                               // On lit le mot obtenu précédemment.
        {
            if(recupChar[i] == '1')                        // Si le i-ième caractère du mot est un 1 alors on va a gauche dans notre arbre
                tmpTree = tmpTree->tleft;
            if(recupChar[i] == '0')                        // Si le i-ième caractère du mot est un 0 alors on va a droite dans notre arbre
                tmpTree = tmpTree->tright;
            if(!tmpTree->tleft && !tmpTree->tright)        // Si on atteint une feuille alors on écrit le caractère contenu dans celle-ci et on retourne au début de l'abre
            {
                fwrite(&tmpTree->elmt.letter,1,1,Result);
                total--;
                tmpTree = tree;
            }
        }


    }
    fclose(Result);
}

