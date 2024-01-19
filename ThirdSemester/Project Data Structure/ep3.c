#include <stdio.h>
#include <stdlib.h>
#define CHAR 100
#define MAXCOLOR 255
#define DIS_FILTER 3


int *load_image_from_file(char *filename, int *w, int *h);
void save_image_to_file(char *filename, int *I, int w, int h);
int *invert_image(int *I, int w, int h);
int *binarize(int *I, int w, int h, int t);
void free_vector(int *v);
int *filter_image(int *I, int w, int h, int d, int tipo);
void add_edge(int *I, int *v, int w, int h, int new_w, int new_h, int d);
void *transfer_values_from_vector(int *I, int *J, int w, int h);
int filter_element(int *I, int *list, int d, int i, int j, int w, int tipo);
void list_bubble_sort(int *v, int s);
int *circumvent_image(int *I, int w, int h, int d);
int *label_components(int *I, int w, int h, int *ncc);
void is_conex(int *I, int *v, int i, int j, int w, int h);




int main(){
    char choice, filename[CHAR], filename_save[CHAR];
    int *original_img, readed_original, *result_image, readed_result, w, h;
    int i, j;

    choice = 'A';
    readed_original= 0; readed_result = 0;

    while (choice != 'q' && choice != 'Q'){

        printf("\n\n-----------------------------------\n");
        printf("Escolha uma opcao\n");

        printf("Q Quit (terminar o programa)\n");
        printf("L Ler um arquivo de imagens\n");
        
        if (readed_original){
            printf("S Salvar a imagem em um arquivo\n");
            printf("M Manter a imagem-entrada anterior\n");
            printf("B Binarizar a imagem-entrada\n");
            printf("C Calcular contorno da imagem-entrada\n");
            printf("F Filtrar a imagem-entrada\n");
            printf("I Inverter a imagem-entrada\n");
            printf("R Rotular a imagem-entrada\n");
        }

        printf("==> ");
        scanf(" %c", &choice);


        if (choice == 'l' || choice == 'L'){

            if(readed_original){
                free_vector(original_img);
                readed_original = 0;
            }

            printf("==> Digite o nome do arquivo de entrada: ");
            scanf("%s", filename);
           
            original_img = load_image_from_file(filename, &w, &h);

            if (original_img == NULL){
                printf("Nao foi possivel abrir o arquivo %s\n\n", filename);
            }
            else {
                readed_original = 1;
                printf("\n** Tamanho da imagem: %d x %d", w, h);
            }

        }


        else if ((choice == 's' || choice == 'S') && readed_original){
            printf("==> Digite o nome do arquivo de saida: ");
            scanf("%s", filename_save);
           
            if (readed_result){
                save_image_to_file(filename_save, result_image, w, h);
            }
            else{
                save_image_to_file(filename_save, original_img, w, h);
            }
            printf("\n** Imagem gravada no arquivo: %s\n", filename_save);
        }


        else if ((choice == 'm' || choice == 'M') && readed_result){
            free_vector(result_image);
            readed_result = 0;            
        }

        else if ((choice == 'b' || choice == 'B') && readed_original){
            printf("==> Digite o limiar: ");
            scanf("%d", &i);

            if (!readed_result) {
                transfer_values_from_vector(original_img, result_image, w, h);
            }

            result_image = binarize(result_image, w, h, i);
            printf("\n** Imagem foi binarizada\n");
            readed_result = 1;
        }


        else if ((choice == 'f' || choice == 'F') && readed_original){
            printf("==> Digite o tipo do filtro: ");
            scanf("%d", &i);

            printf("==> Digite o tamanho do filtro: ");
            scanf("%d", &j);

            if (!readed_result) {
                transfer_values_from_vector(original_img, result_image, w, h);
            }

            result_image = filter_image(result_image, w, h, j, i);
            printf("\n** Imagem foi filtrada\n");
            readed_result = 1;
        }
        
        else if ((choice == 'i' || choice == 'I') && readed_original){
            if (!readed_result) {
                transfer_values_from_vector(original_img, result_image, w, h);
            }

            result_image = invert_image(result_image, w, h);
        
            printf("\n** Imagem foi invertida\n");
            readed_result = 1;
        }
        else if ((choice == 'c' || choice == 'C') && readed_original){
            if (!readed_result) {
                transfer_values_from_vector(original_img, result_image, w, h);
            }
            result_image = circumvent_image(result_image, w, h, DIS_FILTER);
            printf("\n** Contorno foi calculado\n");
            readed_result = 1;
            
        }
        else if ((choice == 'r' || choice == 'R') && readed_original){
            if (!readed_result) {
                transfer_values_from_vector(original_img, result_image, w, h);
            }
            result_image = label_components(result_image, w, h, &i);
        } 
    }
    
    if (readed_original){free_vector(original_img);}
    if (readed_result){free_vector(result_image);}

    return 0;

}


void free_vector(int *v){
    free(v);
    v = NULL;
    return;
}


int *load_image_from_file(char *filename, int *weight, int *height){
    FILE *file;
    char line[CHAR];
    int *v, i, j;

    file = fopen(filename, "r");

    if (file == NULL){return NULL;}
    
    printf("Arquivo %s lido com sucesso\n\n", filename);


    /* Cabecalho do arquivo */
    fscanf(file, "%*[^\n]\n", line);
    fscanf(file, "%d %d\n", weight, height);
    fscanf(file, "%*[^\n]\n", line);


    /* Alocando o vetor */
    v = malloc(sizeof(int) * (*weight) * (*height));

    for (i = 0; i <(*height); i++){
        for(j = 0; j < (*weight); j++){
            fscanf(file, "%d", &(v[i*(*weight) + j]));
        }
    }

    fclose(file);
    return v;
}

 
void save_image_to_file(char *filename, int *I, int w, int h){
    FILE *file;
    int i, j;

    file = fopen(filename, "w");
    
    /* Salvando o cabecalho */
    fprintf(file, "P2\n");
    fprintf(file, "%d %d\n", w, h);
    fprintf(file, "%d\n", MAXCOLOR);
    
    
    /* Salvando o vetor */
    for (i = 0; i < h; i++){
        for (j = 0; j < (w - 1); j ++){
            fprintf(file, "%d ", I[(i*w) + j]);
        }
        fprintf(file, "%d\n", I[(i*w) + j]);
    }

    fclose(file);
    return;
}


int *invert_image(int *I, int w, int h){
    int *v, i, j;

    v = malloc(sizeof(int) * w * h);

    for (i = 0; i < h; i++){
        for (j = 0; j < w; j ++){
            v[(i*w) + j] = MAXCOLOR - I[(i*w) + j];
        }
    }

    free_vector(I);
    return v;
}


int *binarize(int *I, int w, int h, int t){
    int *v, i, j;

    v = malloc(sizeof(int) * w * h);

    for (i = 0; i < h ; i++){
        for (j = 0; j < w; j++){
            if (I[w*i + j] >= t){
                v[w*i + j] = MAXCOLOR;
            }
            else{
                v[w*i + j] = 0;
            }
        }
    }
    free_vector(I);
    return v;
}


int *filter_image(int *I, int w, int h, int d, int tipo){
    int *v, *k, *l;
    int i, j, t, new_h, new_w;
    

    if ((d/2) <= 0 || (tipo > 3) || (tipo < 1) || (d%2) != 1){
        printf("Nao foi possivel filtrar a imagem\n\n");
        return I;
    }


    new_w = w+(d-1);
    new_h = h+(d-1);

    v = malloc(sizeof(int) * new_h * new_w);

    printf("Adicionando borda\n");
    /* Adicionando borda ao vetor v*/
    add_edge(I, v, w, h, new_w, new_h, d);

    printf("Aplicando filtro\n");
    /* Aplicando filtro ao vetor*/
    l = malloc(sizeof(int) * d * d);
    k = malloc(sizeof(int) * h * w);
    t = 0;


    for (i = (d/2); i < new_h-(d/2); i++){
        for(j = (d/2); j < new_w -(d/2); j++){
                k[t] = filter_element(v, l, d, i, j, new_w, tipo);
                t++;
            }
    }

    free_vector(l);
    free_vector(v);
    free_vector(I);
    
    return k;
}


void add_edge(int *I, int *v, int w, int h, int new_w, int new_h, int d){
    int i, j, k;

    for (i = 0; i < h; i++){
        printf("Linha %d\n", i);

        for(j = 0; j < w; j++){
            v[new_w*(i+ (d/2)) + (j + (d/2))] = I[w*i + j];
        }
    }

    // for (i = 0; i < new_h; i++){
    //     for(j = 0; j < (d/2); j++){
    //         v[new_w *(i+(d/2)) + j] = I[w*i];    
    //     }
    // }


    // for (i = 0; i < h; i++){
    //     for(j = (d/2); j < w; j++){
    //         v[new_w * (i+(d/2)) + j] = I[w*i + (j - (d/2))];  
    //     }
    // }

    // for (i = 0; i < h; i++){
    //     for(j = w; j < new_w; j++){
    //         v[new_w * (i+(d/2)) + j] = I[w*(i+1) - 1];
    //     }
    // }

    // for (i = 0; i < h; i++){
    //     printf("Linha %d\n", i);
    //     for(j= 0; j < new_w; j++) {
    //         if (j < (d/2)){
    //         }
    //         else if (j < w){
    //             // v[new_w * (i+(d/2)) + j] = I[w*i + (j - (d/2))];
    //         }
    //         else{
    //             // v[new_w * (i+(d/2)) + j] = I[w*(i+1) - 1];
    //         }
       
    //     }

    // }

       
    //     /* Adicionando os primeiros elementos da linha */
    //     for (j = 0; j < d/2; j++){
    //         v[new_w*i + j] = I[w*(i- (d/2))];
    //     }
        
    //     /* Preenchendo os "meios" da linha */
    //     for (j = d/2; j < w + (d/2); j++){
    //         v[new_w*i + j] = I[w*(i - (d/2)) + (j - (d/2))];
    //     }

    //     /* Adicionando os últimos elementos da linha */
    //     for (j = w + d/2; j <new_w; j++){
    //         v[new_w*i + j] = I[w*(i - (d/2)) + (w-1)];
    //     }
    // }

    printf("Primeira borda feita\n");
    
    
    /* Duplicando a segunda e a penúltima linha, para a primeira e a última linha d/2 vezes*/
    // for(i = 0; i < (d/2); i++){
    //     for(j = 0; j < new_w; j++){
    //         v[i*new_w + j] = v[(d/2)*(new_w)+ j];
    //         v[(new_h)*(w+(d/2)+i) + j] = v[(new_w)*(w+(d/2)-1) + j];
    //     }
    // }

    return;
}


void *transfer_values_from_vector(int *I, int *J, int w, int h){
    int i, j, *v;

    free_vector(J);
    J = malloc(sizeof(int) * w * h);
    for (i = 0; i < h; i++){
        for(j = 0; j < w; j++){
            J[w*i + j] = I[w*i + j];
        }
    }

   
    return ;
}


void list_bubble_sort(int *v, int s){
    int i, j, aux;
    
    for (i = 0; i < (s-1); i++){
        for(j = (i+1); j < s; j++){
            if (v[i] > v[j]){
                aux = v[i];
                v[i] = v[j];
                v[j]= aux;
            }
        }
    }
    return;
}


int filter_element(int *I, int *list, int d, int i, int j,  int w, int tipo){
    int p,  q,  t;
    
    t = 0;
    
    for (p = i-(d/2); p <= i+(d/2); p++){
        for (q = j-(d/2); q <= j+(d/2); q++){
            list[t] = I[p*w + q];
            t++;
        }
    }

    
    list_bubble_sort(list, d*d);

    if (tipo == 1){
        return list[0];
    }
    else if (tipo == 2){
        return list[d/2];
    }
    else {
        return list[(d*d)-1];
    }

}


int *circumvent_image(int *I, int w, int h, int d){
    int *v, *l;
    int i, j;

    
    transfer_values_from_vector(I, l, w, h);
    transfer_values_from_vector(I, v, w, h);
    v = filter_image(v, w, h, d, 1);


    for (i = 0; i < h; i++){
        for (j = 0; j < w; j ++){
            l[w*i + j] = l[w*i + j] - v[w*i + j];
        }
    }

    free_vector(v);
    free_vector(I);
    return l;
}



void is_conex(int *I, int *v, int i, int j, int w, int h){
    int p, q, r, s, t, u;
    p = i-1; q = i+1; r = j-1; s = j+1;
    
    if (i-1 < 0){
        p = 0;
    }
    if (i+1 > w){
        q = i;
    }

    if (j-1 < 0){
        r = 0;
    }
    if (j+1 > w){
        s = j;
    }

    for (t = p; t <= q; t++){
        for(u = r; u <= s; u++){
            if (I[i*w + j] == I[t*w + u] && v[t*w + u] == 0){
                v[t*w + u] = v[i*w + j];
                is_conex(I, v, t, u, w, h);
            }
        }
    }

    return;
    
}




int *label_components(int *I, int w, int h, int *ncc){
    int *v, i, j;

    *ncc = 0;
    v = malloc(sizeof(int) * w * h);


    for (i = 0; i < h; i++){
        for(j = 0; j < w; j++){
           v[i*w + j] = 0;
        }
    }


    for(i = 0; i < h; i++){
        for(j = 0; j < w; j++){
            if ((I[i*w + j] != 0) && (v[i*w + j] == 0)){
                *ncc = *ncc + 1;
                v[i*w + j]= *ncc;
                is_conex(I, v, i, j, w, h);
            }
        }
    }

    if (*ncc <= 127){
        for (i = 0; i < h; i++){
            for(j = 0; j < w; j++){
                v[i*w + j] *= 255/(*ncc);
            }
        }
    }



    free(I);
    return v;
}
