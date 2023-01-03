#ifndef USEFULL_H
#define USEFULL_H
#define Default -59
#define MALLOC(x)((x *)malloc(sizeof(x)))

typedef enum Bool{
    false,
    true
}Bool;

int Min(int a, int b) ;

float MinFloat(float a, float b);

int Max(int a, int b);

int Pow(int a, int b);

float PowF(float a, float b);

float MaxF(float a, float b) ;

int NbChiffre(int nb);

int CharToInt(char car);

void CompressionRatio(char* inputfilename,char* outputfilename);

#endif


