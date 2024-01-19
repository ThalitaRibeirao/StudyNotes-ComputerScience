#include <stdio.h>
#include <stdlib.h>


#define MAX 255
#define CHAR 100


int *load_image_from_file(char *filename, int *w, int *h);
void save_image_to_file(char *filename, int *I, int *w, int *h);



int main(){
    char escolha='A';
    FILE *f;
    int *imagem, *imagem_anterior;
    int *lido = 0;


    while (escolha != 'Q'){
        printf("-----------------------------------\n");
        printf("Escolha uma opcao\n");
        
        printf("Q Quit (terminar o programa)\n");
        printf("L Ler um arquivo de imagens\n");

        if (*lido){
            printf("S Salvar a imagem em um arquivo\n");
            printf("M Manter a imagem-entrada anterior\n");
            printf("B Binarizar a imagem-entrada\n");
            printf("C Calcular contorno da imagem-entrada\n");
            printf("F Filtrar a imagem-entrada\n");
            printf("I Inverter a imagem-entrada\n");
            printf("R Rotular a imagem-entrada\n");
        }

        printf("==> ");
        scanf("%c", &escolha);
    
    }

    return 0;
}


// int *load_image_from_file(char *filename, int *w, int *h){
//     FILE *file;
//     char line [CHAR];
//     int i, j, *v, t = 0;

//     v = malloc(sizeof(int) * (*w) * (*h));

//     fscanf(file, "%[^\n]\n", line);
//     fscanf(file, "%d %d", w, h);
//     fscanf(file, "%[^\n]\n", line);


//     for (i = 0; i < *w; i++){
//         for(j = 0; j < *h; j++){
//             fscanf(file, "%d", v[t]);
//             t++;
//         }
//         fscanf(file,"\n");
//     }

//     return v;
// }

// void save_image_to_file(char *filename, int *I, int *w, int *h){
//     FILE *file;
//     int i, j;


//     file = fopen(filename,"w");
    
//     for (i = 0; i < *w; i++){
//         for (j = 0; j < *h; j ++){
//             fprintf(file, "%d", I[i*(*w) + j]);

//         }
//         fprintf(file, "\n");
//     }

    
// }