#include "cabecera.h"

int main(){
	lecturaArchivo(); 
	return 0;
}

void lecturaArchivo(){ //Se lee el archivo y se guardan las variables
	FILE * archivoEntrada;
	archivoEntrada = fopen("entrada.txt", "r");
	char linea[64];
	int i, j, cantidadNinos;
	fgets(linea, 64, archivoEntrada); //Se obtiene la primera linea, cantidad de niños
	cantidadNinos = atoi(linea); //Se guarda en una variable
	int matriz[cantidadNinos+2][cantidadNinos+2]; //Se crea una matriz, cada posicion equivale a la distancia
	for(i = 0; i < cantidadNinos+2; i++){ //Se inicia con ceros
		for(j = 0; j < cantidadNinos+2; j++){
			matriz[i][j] = 0;
		}
	}
	while(!feof(archivoEntrada)){ //Mientras no se llegue al final del archivo
		char * auxiliar = (char*)malloc(sizeof(char)*64); //Cada variable guarda los valores de una linea
		char * auxiliarDos = (char*)malloc(sizeof(char)*64);
		char * auxiliarTres = (char*)malloc(sizeof(char)*64);
		int aux, auxDos, auxTres;
		fgets(linea, 64 ,archivoEntrada); //Se obtiene la linea
		auxiliar = strtok(linea, " \n"); //Se guarda el primer valor
		auxiliarDos = strtok(NULL, " \n"); //Se guarda el segundo valor
		auxiliarTres = strtok(NULL, " \n"); //Se guarda el tercer valor
		aux = atoi(auxiliar); //Se transformas a enteros y se guardan
		auxDos = atoi(auxiliarDos);
		auxTres = atoi(auxiliarTres);
		if(aux == 21){ //Si es 21
			matriz[cantidadNinos+1][auxDos] = auxTres; //Se guarda en la ultima posicion en x 
		}
		else if(auxDos == 21){ //Si el segundo es 21
			matriz[aux][cantidadNinos+1] = auxTres; //Se guarda en al ultima posicion en y
		}
		else{
			matriz[aux][auxDos] = auxTres; //Se guarda el valor
		}
	}
	busquedaCamino(cantidadNinos, matriz); //Se llama a busquedaCamino pasandole la cantidad de niños y la matriz
}

void busquedaCamino(int ninos, int matriz[ninos+2][ninos+2]){ //funcion que lista las combinaciones
	int combinacion[ninos]; //Lista de combinaciones
	int aux;
	Camino * listaCombinaciones; //Lista de los tramos
	listaCombinaciones = (Camino*)malloc(sizeof(Camino)*512);
	int i, j, k, l, contador; //Variables auxiliares para ciclos
	contador = 0;
	k = 0;
	for(i = 0; i < ninos; i++){
		combinacion[i] = i+1; //Se guardan las posibles combinaciones ordenadas
	}
	k = k + matriz[0][combinacion[0]]; //Se suma el tramo de 0 a 1
	k = k + matriz[combinacion[ninos-1]][4]; //Se suma el tramo del ultimo a 21
	for(i = 0; i < ninos-1; i++){
		k = k + matriz[combinacion[i]][combinacion[i+1]]; //Se sumas los tramos del medio
	}
	for(l = 0; l < ninos; l++){
		listaCombinaciones[contador].matriz[l] = combinacion[l]; //Se guardan los valores en la matriz de la estructura
	}
	listaCombinaciones[contador].resultado = k; //Se guarda la suma de los tramos
	contador++;
	k = 0;
		for(j = 1; j <= ninos; j++){ //EN estos ciclos se cambian la combinaciones
			for(i = 0; i < ninos-1; i++){
				aux = combinacion[i]; //Se guarda
				combinacion[i] = combinacion[i+1]; //Se cambia
				combinacion[i+1] = aux; //Se coloca el guardado en el otro
				k = k + matriz[0][combinacion[0]]; //Se suman el primer y ultimo tramo
				k = k + matriz[combinacion[ninos-1]][4];
				for(l = 0; l < ninos-1; l++){
					k = k + matriz[combinacion[l]][combinacion[l+1]]; //Se suman los tramos del medio
				}
				for(l = 0; l < ninos; l++){
					listaCombinaciones[contador].matriz[l] = combinacion[l]; //Se guarda la combinacion
				}
				listaCombinaciones[contador].resultado = k; //Se guarda el valor total de los tramos
				contador++;
				k = 0;
			}
		}
		busquedaMejor(listaCombinaciones, contador, ninos); //Se llama a busquedaMejor
}

void busquedaMejor(Camino * c, int cantidadCaminos, int cantidadNinos){ //Esta funcion busca el tramos mas corto
		int i;
		Camino mejorCamino; //Valor auxiliar que guarda la mejor
		mejorCamino = c[0]; //Se asume que la mejor es el primero
		for(i = 1; i < cantidadCaminos; i++){
			if(c[i].resultado < mejorCamino.resultado){ //Si es menor
				mejorCamino = c[i]; //Se guarda en la auxiliar
			}
		}
		free(c); //Se libera la memoria
		imprimirArchivo(mejorCamino, cantidadNinos); //Se llama a la funcion imprimirArchivo
}

void imprimirArchivo(Camino final, int ninos){ //Funcion que muestra el resultado en un archivo de salida
	FILE * archivoSalida;
	archivoSalida = fopen("salida.txt", "w"); //Se abre en modo escritura
	fprintf(archivoSalida, "%s", "0 -"); //Se coloca la posicion inicial
	int i;
	for(i = 0; i < ninos; i++){
		fprintf(archivoSalida, " %d -", final.matriz[i]); //Se colocan las combinaciones
	}
	fprintf(archivoSalida, "%s\n", " 21"); //Se coloca la posicion final
	fprintf(archivoSalida, "%s", "Duracion: ");
	fprintf(archivoSalida, "%d ", final.resultado); //Se coloca el resultado
	fprintf(archivoSalida, "%s", "minutos");
}