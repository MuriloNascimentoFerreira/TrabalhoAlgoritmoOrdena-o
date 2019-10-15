#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct{
	int id;
	int numeracao;
	char marca[50];
	char modelo[50];
	char cor[20];
}Calcado;

typedef struct{ 
		char marcaSort[50];
}Marca;
		
typedef struct{
	char modeloSort[50];
}Modelo;

typedef struct{    
		char corSort[20];
}Cor;

void gerarCalcados(FILE *arq_ca,FILE *arq_ma,FILE *arq_mo,FILE *arq_co){
	int n = 0;
	printf("Quantos registro deseja gerar?");
	scanf(" %d",&n);
	Calcado* calcado = malloc(sizeof(Calcado));
	arq_ca = fopen("arquivo_calcados","ab");
	for (int i = 0; i < n; i++)
	{
		//////////ID///////////
		calcado->id = 1+rand()% 1000000;
		///////////NUMERAÇÃO///////////
		calcado->numeracao = 1+rand()% 50;
		////////Marca///////
		int x = rand()% 15;
		Marca* vetorMarca[14];
		arq_ma = fopen("arquivo_marca","rb");
		for (int j = 0; j < 15; j++)
		{
			vetorMarca[j] = malloc(sizeof(Marca));
			fread(vetorMarca[j]->marcaSort,sizeof(Marca),1,arq_ma);
		}
		fclose(arq_ma);
		strcpy(calcado->marca,vetorMarca[x]->marcaSort);

		/////////MODELO///////////
		x = rand()% 7;
		Modelo* vetorModelo[6];
		arq_mo = fopen("arquivo_modelo","rb");
		for (int j = 0; j < 7; j++)
		{
			vetorModelo[j] = malloc(sizeof(Modelo));
			fread(vetorModelo[j]->modeloSort,sizeof(Modelo),1,arq_mo);
		}
		fclose(arq_mo);
		strcpy(calcado->modelo,vetorModelo[x]->modeloSort);
		
		///////////COR////////////	
		x = rand()% 7;
		Cor* vetorCor[6];
		arq_co = fopen("arquivo_cor","rb");
		for (int j = 0; j < 7; j++)
		{
			vetorCor[j] = malloc(sizeof(Cor));
			fread(vetorCor[j]->corSort,sizeof(Cor),1,arq_co);
		}
		fclose(arq_co);
		strcpy(calcado->cor,vetorCor[x]->corSort);
		////////////////////////////////////////
		fwrite(calcado,sizeof(Calcado),1,arq_ca); 
	}
	printf("%d Registro(s) de calçado(s) gerado(s) com sucesso!\n",n);
	fclose(arq_ca);
	
}

void imprimirCalcados(Calcado *vetor[],int n){
	
	printf("\n--------------ORDENADO---------------\n");
	for (int i = 0; i < n; i++)
	{
		printf("ID: %d\n",vetor[i]->id);
		printf("Tamanho: %d\n",vetor[i]->numeracao);
		printf("Marca: %s\n",vetor[i]->marca);
		printf("Modelo: %s\n",vetor[i]->modelo);
		printf("Cor: %s\n",vetor[i]->cor);
		printf("\n");
	}
	printf("---------------------------------------\n");
}

int contarCalcados(FILE* arq_c){
		int calcados = 0;
		Calcado* novo = malloc(sizeof(Calcado));
		arq_c = fopen("arquivo_calcados","rb");
		if (arq_c == NULL){
			printf("Nenhum registro encontrado!\n");
			return 0;
		}
			
		while(fread(novo,sizeof(Calcado),1,arq_c)){
			calcados++;
		}
		fclose(arq_c);
		return calcados;
}
	
void ordenaShell(FILE *arq_c,double *tempoShel){
	///////Carrega dados do arquivo para um vetor /////////
	int tamanhoVetor = contarCalcados(arq_c);
	Calcado *vetorCalcados[tamanhoVetor];
	arq_c = fopen("arquivo_calcados","rb");
	int k = 0;
	for (k = 0; k < tamanhoVetor; k++)
	{
		vetorCalcados[k] = malloc(sizeof(Calcado));
		fread(vetorCalcados[k],sizeof(Calcado),1,arq_c);
	}
	
	if(tamanhoVetor == 0){
		return;
	}
	fclose(arq_c);	
	///////////////////////////////////////////////
	/////////////Ordenação Shellsort//////////////
	clock_t t0, tf;
	t0 = clock();
	Calcado *temporario;
	temporario = malloc(sizeof(Calcado));
	int i , j;
    int h = 1;
    while(h < tamanhoVetor) {
        h = 3*h+1;
    }
    while ( h > 1) {
        h = h/3;
        for(i = h; i < tamanhoVetor; i++) {
            temporario = vetorCalcados[i];
            j = i;
            while (j >= h && temporario->id < vetorCalcados[j - h]->id){ 
                vetorCalcados[j] = vetorCalcados[j - h];
                j = j - h;
            }
            vetorCalcados[j] = temporario;
        }
    }	
    tf = clock();
	*tempoShel = (( (double) (tf - t0) ) / (((double)CLOCKS_PER_SEC)/1000));
	
	imprimirCalcados(vetorCalcados,tamanhoVetor);	
	
}

void trocarPosicoes(Calcado *vetorCalcados[], int i, int j){ 
	Calcado *temporario = vetorCalcados[i];
	vetorCalcados[i] = vetorCalcados[j];
	vetorCalcados[j] = temporario;
}

void refazerHeap(Calcado *vetorCalcados[], int n){ 
	int pai, filhoEsquerdo, filhoDireito, filhoMaior;	
	pai = 0;	
	while(pai <= (n / 2 - 1)){	
		filhoEsquerdo = 2 * pai + 1;
		filhoDireito = 2 * pai + 2;
		
		filhoMaior = filhoEsquerdo;
		if(filhoDireito < n && vetorCalcados[filhoDireito]->id > vetorCalcados[filhoEsquerdo]->id){
			filhoMaior = filhoDireito;
		}
		
		if(vetorCalcados[filhoMaior]->id > vetorCalcados[pai]->id){
			trocarPosicoes(vetorCalcados, pai, filhoMaior);
			pai = filhoMaior;			
		}else{
			break;
		}
	}
}

void extrairRaizHeap(Calcado *vetorCalcados[], int n){ 
	trocarPosicoes(vetorCalcados, 0, n - 1);	
}

void construirHeap(Calcado *vetorCalcados[],int n){ 
	int i, pai, filho;	
	for(i = 0; i < (n - 1); i++){		
		filho = i + 1;
		pai = (filho - 1) / 2;
		while(filho > 0 && vetorCalcados[pai]->id < vetorCalcados[filho]->id){
			trocarPosicoes(vetorCalcados, pai, filho);
			filho = pai;
			pai = (filho - 1) / 2;
		}		
	}
}

void ordenaHeap(FILE *arq_c,double *tempoHeap){
	///////Carrega dados do arquivo para um vetor ////////
	int tamanhoVetor = contarCalcados(arq_c);
	if(tamanhoVetor == 0)
		return;
	Calcado *vetorCalcados[tamanhoVetor];
	arq_c = fopen("arquivo_calcados","rb");
	for (int i = 0; i < tamanhoVetor; i++)
	{
		vetorCalcados[i] = malloc(sizeof(Calcado));
		fread(vetorCalcados[i],sizeof(Calcado),1,arq_c);
	}
	fclose(arq_c);
	/////////////////////////////////////////////////
	///////////Ordenação helpsort////////////
	
	///Medindo o tempo de execução
	clock_t t0, tf;
	t0 = clock();///inicio da execução
	//////////////////////////////
	
	int tamanhoVetorTemp = tamanhoVetor;
	construirHeap(vetorCalcados,tamanhoVetor);
	while(tamanhoVetorTemp > 1){							
		extrairRaizHeap(vetorCalcados,tamanhoVetorTemp);
		tamanhoVetorTemp--;			
		refazerHeap(vetorCalcados,tamanhoVetorTemp);		
	}	
	
	tf = clock();//Fim da execução
	*tempoHeap = (( (double) (tf - t0) ) / (((double)CLOCKS_PER_SEC)/1000));///Armazena o tempo
	imprimirCalcados(vetorCalcados,tamanhoVetor);
}

void particao(int esquerda, int direita, int *i, int *j,Calcado *vetorCalcados[]){ 
	Calcado *pivo;
	pivo = malloc(sizeof(Calcado));
	Calcado *temporario;
	temporario = malloc(sizeof(Calcado));
	*i = esquerda;  
	*j = direita;
	pivo = vetorCalcados[(*i + *j)/2];
	do{ 
		while(pivo->id > vetorCalcados[*i]->id)
			(*i)++;				
		while(pivo->id < vetorCalcados[*j]->id) 
			(*j)--;		
		if(*i <= *j) { 
			temporario = vetorCalcados[*i]; 
			vetorCalcados[*i] = vetorCalcados[*j]; 
			vetorCalcados[*j] = temporario;
			(*i)++; 
			(*j)--;
		}		
		
    }while(*i <= *j);
}

void ordena(int esquerda,int direita,Calcado *vetorCalcados[]){ 
	int i, j;
	particao(esquerda, direita, &i, &j,vetorCalcados);
	if(esquerda < j) 
		ordena(esquerda, j, vetorCalcados);
	if(i < direita) 
		ordena(i, direita, vetorCalcados);
}

void ordenaQuick(FILE *arq_c,double *tempoQuick){
	//////Carrega dados do arquivo para um vetor //////
	int tamanhoVetor = contarCalcados(arq_c);
	if(tamanhoVetor == 0)
		return;
	
	Calcado *vetorCalcados[tamanhoVetor];
	arq_c = fopen("arquivo_calcados","rb");
	for (int i = 0; i < tamanhoVetor; i++)
	{
		vetorCalcados[i] = malloc(sizeof(Calcado));
		fread(vetorCalcados[i],sizeof(Calcado),1,arq_c);
	}
	fclose(arq_c);
	/////////////////////////////////////////////////
	
	/////////Ordenação quicksort////////////////////
	
	///Medindo o tempo de execução
	clock_t t0, tf;
	t0 = clock();///inicio da execução
	//////////////////////////////
	
	ordena(0,tamanhoVetor-1,vetorCalcados);
	
	tf = clock();//Fim da execução
	*tempoQuick = (( (double) (tf - t0) ) / (((double)CLOCKS_PER_SEC)/1000));///Armazena o tempo
	imprimirCalcados(vetorCalcados,tamanhoVetor);
	
}

int opc(){
		int n = 0; 
		printf("\n---------------MENU DE OPÇÕES-----------------\n");
		printf("1-Ordenar por Shellsort\n");
		printf("2-Ordenar por Heapsort\n");
		printf("3-Ordenar por Quicksort\n");
		printf("4-Gerar registros de calçado\n");
		printf("5-Tempo para execução de cada algoritmo de ordenação\n");
		printf("0-Sair\n");
		printf("-----------------------------------------------\n");
		scanf(" %d",&n);
		return n;
}

int main()
{
	srand(time(NULL));
	FILE *arq_calcados = NULL,*arq_modelos = NULL,*arq_cores = NULL,*arq_marcas = NULL;
	double tempoShel = 0,tempoHeap = 0,tempoQuick = 0;
	
	while(1){
			switch(opc()){
					case 1:ordenaShell(arq_calcados,&tempoShel);
							break;
							
					case 2:ordenaHeap(arq_calcados,&tempoHeap);
							break;
					
					case 3:ordenaQuick(arq_calcados,&tempoQuick);
							break;
									
					case 4:gerarCalcados(arq_calcados,arq_marcas,arq_modelos,arq_cores);
							break;
					case 5:printf("Tempo Shellsort: %f\nTempo HeapSort: %f\nTempo Quicksort %f\n",tempoShel,tempoHeap,tempoQuick);
							break;
							
					case 0: return 0;
			}
	}
	
	return 0;
}

