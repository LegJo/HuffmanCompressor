#include "Huffman.h"
#include <time.h>

int InvalidSyntaxe(int systemreturn);
int ExistFile(const char *filename);


int main(int argc,char* argv[]){
    Bool dev = false;
    int systemreturn = 1; 

    if(ExistFile("dev.txt")){
        system("rm dev.txt"); 
    }

    if((strcmp(argv[1], "-D")==1) && ExistFile("tmp.txt")){
        system("rm tmp.txt");
    }

    if (argc == 2){
        if(strcmp(argv[1],"-help") == 0 ){
            systemreturn = system("cat README");
            return systemreturn;
        }
        else {
            return InvalidSyntaxe(systemreturn);
        }
    }
    if((argc != 4 && argc != 5) || ((strcmp(argv[1], "-C")!=0)&&(strcmp(argv[1], "-D")!=0))){
        return InvalidSyntaxe(systemreturn);
    }
    if(ExistFile(argv[2]) != 1)
    {
        puts("Le fichier input n'existe pas");
        return InvalidSyntaxe(systemreturn);
    }
    if(argc == 5){
        if (strcmp(argv[4],"-dev") == 0){
            dev = true;
        }
        else {
            return InvalidSyntaxe(systemreturn);
        }
    }
    
    if (strcmp(argv[1],"-C")==0){
        clock_t debut = clock();
        CryptFile(argv[2],argv[3], dev);
        clock_t fin = clock();
        double tempsExecution = (double)(fin-debut) / CLOCKS_PER_SEC;
        printf("Done (%.2fs).\n\n",tempsExecution);
    }

    if (strcmp(argv[1],"-D")==0){
        clock_t debut = clock();
        DecryptFile(argv[2],argv[3], dev);
        printf("\nDecompressing %s...\n\n",argv[2]);
        clock_t fin = clock();
        double tempsExecution = (double)(fin-debut) / CLOCKS_PER_SEC;
        printf("Done (%.2fs).\n\n",tempsExecution);

    }
    return 0;

}

int InvalidSyntaxe(int systemreturn){ 
        puts("INVALID SYNTAXE");
        systemreturn = system("tail -n +3 README | head -4 ");
        puts("taper ./executable -help pour voir d'avantage");
        return systemreturn;
}

int ExistFile(const char *filename)
{
    FILE *file;
    if ((file = fopen(filename, "r"))){
        fclose(file);
        return 1;
    }
    return 0;
}
