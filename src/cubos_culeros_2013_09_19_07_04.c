/*
 ============================================================================
 Name        : cubos_culeros.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

void mapear_definicion_cubo(int *array_definiciones_cubos,
		char *cadena_definicion_cubos);
size_t join_integers(const unsigned int *num, size_t num_len, char *buf,
		size_t buf_len);
void generar_posiciones_cubos(int (*definiciones_cubos)[6],
		int (*posiciones_cubos)[6][6], int num_cubos);
void imprimir_posiciones_cubos(int (*posiciones_cubos)[6][6], int num_cubos);
void comparar_cubos(int (*posiciones_cubos)[6][6], int num_cubos,
		int (*conteo_posiciones_comunes_pivote)[6],
		int (*indices_maximas_coincidencias_en_cubo)[6][4],
		int coincidencias_en_cubo[4][6][4][6]);
int contar_coincidencias_posiciones(int *posicion_culo_en_conteo,
		int *posicion_culo_en_el_que_se_busca);
void imprimir_conteo_posiciones_comunes(
		int conteo_posiciones_comunes_pivote[4][6], int num_cubos);
int calcular_num_minimo_de_caras_diferentes(
		int conteo_posiciones_comunes_pivote[4][6], int num_cubos,
		int (*indices_maximas_coincidencias_en_cubo)[6][4],
		int coincidencias_en_cubo[4][6][4][6]);

int main(int argc, char *argv[]) {
	FILE *fr;
	char buffer[512];
	char buffershit[512];
	int i;
	int nread;
	int num_de_cubos = 0;
	int num_de_cubos_guardados = 0;
	int anterior_num_de_cubos = 0;
	int num_min_caras_diferentes = 0;
	int definicion_cubo[6];
	int definiciones_cubos[4][6];
	/* 4 cubos x 6 caras pivote x 6 caras */
	int posiciones_cubos[4][6][6];
	int conteo_posiciones_comunes_pivote[4][6];
	int indices_maximas_coincidencias_en_cubo[4][6][4];
	int coincidencias_en_cubo[4][6][4][6];

	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */

	if (argc != 2) /* argc should be 2 for correct execution */
	{
		/* We print argv[0] assuming it is the program name */
		printf("usage: %s filename\n", argv[0]);
		return EXIT_FAILURE;
	}
	fr = fopen(argv[1], "rt");

	while (fgets(buffer, 80, fr) != NULL ) {
		num_de_cubos = strtol(buffer, NULL, 10);
		if (num_de_cubos) {
			printf("numbero de culos %d\n", num_de_cubos);
			if (anterior_num_de_cubos) {
				generar_posiciones_cubos(definiciones_cubos, posiciones_cubos,
						num_de_cubos_guardados);
				imprimir_posiciones_cubos(posiciones_cubos,
						num_de_cubos_guardados);
				comparar_cubos(posiciones_cubos, num_de_cubos_guardados,
						conteo_posiciones_comunes_pivote,
						indices_maximas_coincidencias_en_cubo,
						coincidencias_en_cubo);
				/*
				 imprimir_conteo_posiciones_comunes(
				 conteo_posiciones_comunes_pivote,
				 num_de_cubos_guardados);
				 */
				num_min_caras_diferentes =
						calcular_num_minimo_de_caras_diferentes(
								conteo_posiciones_comunes_pivote,
								num_de_cubos_guardados,
								indices_maximas_coincidencias_en_cubo,
								coincidencias_en_cubo);
				printf("caras %d\n", num_min_caras_diferentes);

			}
			anterior_num_de_cubos = num_de_cubos;
			num_de_cubos_guardados = 0;
		} else {
			printf("definicion de culo %s", buffer);
			mapear_definicion_cubo(definicion_cubo, buffer);
			join_integers(definicion_cubo, 6, buffershit, 512);
			printf("yo se que ya no m kieres %s\n", buffershit);
			memcpy((void * )*(definiciones_cubos + num_de_cubos_guardados),
					(void * )definicion_cubo, 6 * (sizeof(int)));
			num_de_cubos_guardados++;

		}
	}
	generar_posiciones_cubos(definiciones_cubos, posiciones_cubos,
			num_de_cubos_guardados);
	imprimir_posiciones_cubos(posiciones_cubos, num_de_cubos_guardados);
	comparar_cubos(posiciones_cubos, num_de_cubos_guardados,
			conteo_posiciones_comunes_pivote,
			indices_maximas_coincidencias_en_cubo, coincidencias_en_cubo);
	num_min_caras_diferentes = calcular_num_minimo_de_caras_diferentes(
			conteo_posiciones_comunes_pivote, num_de_cubos_guardados,
			indices_maximas_coincidencias_en_cubo, coincidencias_en_cubo);
	printf("caras %d\n", num_min_caras_diferentes);
	fclose(fr);

	return EXIT_SUCCESS;
}

void mapear_definicion_cubo(int *array_definiciones_cubos,
		char *cadena_definicion_cubos) {
	char * pch;
	int index = 0;
	int valor_color = 0;
	pch = strtok(cadena_definicion_cubos, " \n");
	while (pch != NULL ) {
		valor_color = 0;
//		printf("token %s\n", pch);
		if (!strcmp("aquamarine", pch)) {
			valor_color = 1;
		}
		if (!strcmp("blond", pch)) {
			valor_color = 2;
		}
		if (!strcmp("blue", pch)) {
			valor_color = 3;
		}
		if (!strcmp("chrome-green", pch)) {
			valor_color = 4;
		}
		if (!strcmp("chrome-yellow", pch)) {
			valor_color = 5;
		}
		if (!strcmp("cream", pch)) {
			valor_color = 6;
		}
		if (!strcmp("cyan", pch)) {
			valor_color = 7;
		}
		if (!strcmp("emerald-green", pch)) {
			valor_color = 8;
		}
		if (!strcmp("gray", pch)) {
			valor_color = 9;
		}
		if (!strcmp("green", pch)) {
			valor_color = 10;
		}
		if (!strcmp("indigo", pch)) {
			valor_color = 11;
		}
		if (!strcmp("lemon", pch)) {
			valor_color = 12;
		}
		if (!strcmp("magenta", pch)) {
			valor_color = 13;
		}
		if (!strcmp("olive", pch)) {
			valor_color = 14;
		}
		if (!strcmp("pink", pch)) {
			valor_color = 15;
		}
		if (!strcmp("purple", pch)) {
			valor_color = 16;
		}
		if (!strcmp("red", pch)) {
			valor_color = 17;
		}
		if (!strcmp("salmon", pch)) {
			valor_color = 18;
		}
		if (!strcmp("scarlet", pch)) {
			valor_color = 19;
		}
		if (!strcmp("sea-green", pch)) {
			valor_color = 20;
		}
		if (!strcmp("sky-blue", pch)) {
			valor_color = 21;
		}
		if (!strcmp("turquoise-blue", pch)) {
			valor_color = 22;
		}
		if (!strcmp("vermilion", pch)) {
			valor_color = 23;
		}
		if (!strcmp("vilidian", pch)) {
			valor_color = 24;
		}
		if (!strcmp("violet", pch)) {
			valor_color = 25;
		}
		if (!strcmp("wine-red", pch)) {
			valor_color = 26;
		}
		if (!strcmp("yellow", pch)) {
			valor_color = 27;
		}
//		printf("valor color %d\n", valor_color);
		*(array_definiciones_cubos + index) = valor_color;
		index++;
		pch = strtok(NULL, " \n");
	}
}

size_t join_integers(const unsigned int *num, size_t num_len, char *buf,
		size_t buf_len) {
	size_t i;
	unsigned int written = 0;

	for (i = 0; i < num_len; i++) {
		written +=
				snprintf(buf + written, buf_len - written, (i != 0 ? ", %u" : "%u"),
						*(num + i));
		if (written == buf_len)
			break;
	}

	return written;
}

void generar_posiciones_cubos(int (*definiciones_cubos)[6],
		int (*posiciones_cubos)[6][6], int num_cubos) {
	int i = 0;
	int indice_cara_cubo = 0;
	char buffer[512];
	printf("generando mierdassssssss %d\n", num_cubos);
//	printf("como dona a su agujero %d\n", *((*(definiciones_cubos + 1)) + 0));
	for (i = 0; i < num_cubos; i++) {
//		printf("numero de culo %d", i);
		for (indice_cara_cubo = 0; indice_cara_cubo < 6; indice_cara_cubo++) {
			join_integers(*(definiciones_cubos + i), 6, buffer, 512);
//			printf("cara de culo pivote %d: %s\n", indice_cara_cubo, buffer);
			switch (indice_cara_cubo) {
			case 0:
				*(*(*(posiciones_cubos + i) + indice_cara_cubo) + 0) =
						*(*(definiciones_cubos + i) + 0);
				*(*(*(posiciones_cubos + i) + indice_cara_cubo) + 1) =
						*(*(definiciones_cubos + i) + 2);
				*(*(*(posiciones_cubos + i) + indice_cara_cubo) + 2) =
						*(*(definiciones_cubos + i) + 1);
				*(*(*(posiciones_cubos + i) + indice_cara_cubo) + 3) =
						*(*(definiciones_cubos + i) + 3);
				*(*(*(posiciones_cubos + i) + indice_cara_cubo) + 4) =
						*(*(definiciones_cubos + i) + 4);
				*(*(*(posiciones_cubos + i) + indice_cara_cubo) + 5) =
						*(*(definiciones_cubos + i) + 5);

				break;
			case 1:
				*(*(*(posiciones_cubos + i) + indice_cara_cubo) + 0) =
						*(*(definiciones_cubos + i) + 1);
				*(*(*(posiciones_cubos + i) + indice_cara_cubo) + 1) =
						*(*(definiciones_cubos + i) + 2);
				*(*(*(posiciones_cubos + i) + indice_cara_cubo) + 2) =
						*(*(definiciones_cubos + i) + 5);
				*(*(*(posiciones_cubos + i) + indice_cara_cubo) + 3) =
						*(*(definiciones_cubos + i) + 3);
				*(*(*(posiciones_cubos + i) + indice_cara_cubo) + 4) =
						*(*(definiciones_cubos + i) + 0);
				*(*(*(posiciones_cubos + i) + indice_cara_cubo) + 5) =
						*(*(definiciones_cubos + i) + 4);
				break;
			case 2:
				*(*(*(posiciones_cubos + i) + indice_cara_cubo) + 0) =
						*(*(definiciones_cubos + i) + 2);
				*(*(*(posiciones_cubos + i) + indice_cara_cubo) + 1) =
						*(*(definiciones_cubos + i) + 1);
				*(*(*(posiciones_cubos + i) + indice_cara_cubo) + 2) =
						*(*(definiciones_cubos + i) + 0);
				*(*(*(posiciones_cubos + i) + indice_cara_cubo) + 3) =
						*(*(definiciones_cubos + i) + 4);
				*(*(*(posiciones_cubos + i) + indice_cara_cubo) + 4) =
						*(*(definiciones_cubos + i) + 5);
				*(*(*(posiciones_cubos + i) + indice_cara_cubo) + 5) =
						*(*(definiciones_cubos + i) + 3);
				break;
			case 3:
				*(*(*(posiciones_cubos + i) + indice_cara_cubo) + 0) =
						*(*(definiciones_cubos + i) + 3);
				*(*(*(posiciones_cubos + i) + indice_cara_cubo) + 1) =
						*(*(definiciones_cubos + i) + 4);
				*(*(*(posiciones_cubos + i) + indice_cara_cubo) + 2) =
						*(*(definiciones_cubos + i) + 0);
				*(*(*(posiciones_cubos + i) + indice_cara_cubo) + 3) =
						*(*(definiciones_cubos + i) + 1);
				*(*(*(posiciones_cubos + i) + indice_cara_cubo) + 4) =
						*(*(definiciones_cubos + i) + 5);
				*(*(*(posiciones_cubos + i) + indice_cara_cubo) + 5) =
						*(*(definiciones_cubos + i) + 2);
				break;
			case 4:
				*(*(*(posiciones_cubos + i) + indice_cara_cubo) + 0) =
						*(*(definiciones_cubos + i) + 4);
				*(*(*(posiciones_cubos + i) + indice_cara_cubo) + 1) =
						*(*(definiciones_cubos + i) + 2);
				*(*(*(posiciones_cubos + i) + indice_cara_cubo) + 2) =
						*(*(definiciones_cubos + i) + 0);
				*(*(*(posiciones_cubos + i) + indice_cara_cubo) + 3) =
						*(*(definiciones_cubos + i) + 3);
				*(*(*(posiciones_cubos + i) + indice_cara_cubo) + 4) =
						*(*(definiciones_cubos + i) + 5);
				*(*(*(posiciones_cubos + i) + indice_cara_cubo) + 5) =
						*(*(definiciones_cubos + i) + 1);
				break;
			case 5:
				*(*(*(posiciones_cubos + i) + indice_cara_cubo) + 0) =
						*(*(definiciones_cubos + i) + 5);
				*(*(*(posiciones_cubos + i) + indice_cara_cubo) + 1) =
						*(*(definiciones_cubos + i) + 2);
				*(*(*(posiciones_cubos + i) + indice_cara_cubo) + 2) =
						*(*(definiciones_cubos + i) + 4);
				*(*(*(posiciones_cubos + i) + indice_cara_cubo) + 3) =
						*(*(definiciones_cubos + i) + 3);
				*(*(*(posiciones_cubos + i) + indice_cara_cubo) + 4) =
						*(*(definiciones_cubos + i) + 1);
				*(*(*(posiciones_cubos + i) + indice_cara_cubo) + 5) =
						*(*(definiciones_cubos + i) + 0);
				break;
			default:
				break;
			}
		}
	}
}

void imprimir_posiciones_cubos(int (*posiciones_cubos)[6][6], int num_cubos) {
	int indice_cubo = 0;
	int indice_cara_pivote = 0;
	int indice_cara = 0;
	char buffershit[512];
	for (indice_cubo = 0; indice_cubo < num_cubos; indice_cubo++) {
		printf("Para el cubo %d:\n", indice_cubo);
		for (indice_cara_pivote = 0; indice_cara_pivote < 6;
				indice_cara_pivote++) {
			join_integers(
					*(*(posiciones_cubos + indice_cubo) + indice_cara_pivote),
					6, buffershit, 512);
			printf("\tCara pivote %d: %s\n", indice_cara_pivote, buffershit);
		}
	}

}

void comparar_cubos(int (*posiciones_cubos)[6][6], int num_cubos,
		int (*conteo_posiciones_comunes_pivote)[6],
		int (*indices_maximas_coincidencias_en_cubo)[6][4],
		int coincidencias_en_cubo[4][6][4][6]) {
	int indice_cubo_en_conteo = 0;
	int indice_cubo_en_el_que_se_busca = 0;
	int indice_posicion_cubo_en_conteo = 0;
	int indice_posicion_cubo_en_el_que_se_busca = 0;
	int num_coincidencias = 0;
	int max_coincidencias_en_cubo_actual;
	int indice_max_coincidencias_en_cubo_actual = 0;
	int (*cubo_en_conteo_actual)[6] = NULL;
	int (*cubo_en_el_que_se_busca_actual)[6] = NULL;
	int *posicion_cubo_en_conteo_actual = NULL;
	int *posicion_cubo_en_el_que_se_busca_actual = NULL;

//	printf("shit %d\n", num_cubos);
	// Inicializando conteo de coincidencias
	for (indice_cubo_en_conteo = 0; indice_cubo_en_conteo < num_cubos;
			indice_cubo_en_conteo++) {
//		printf("WTF!!!\n");
		for (indice_posicion_cubo_en_conteo = 0;
				indice_posicion_cubo_en_conteo < 6;
				indice_posicion_cubo_en_conteo++) {
			*(*(conteo_posiciones_comunes_pivote + indice_cubo_en_conteo)
					+ indice_posicion_cubo_en_conteo) = 0;
		}
	}

	// Cubo para el cual se estan contando coincidencias con las otras caras de los otros cubos
	for (indice_cubo_en_conteo = 0; indice_cubo_en_conteo < num_cubos;
			indice_cubo_en_conteo++) {
		cubo_en_conteo_actual = *(posiciones_cubos + indice_cubo_en_conteo);
//		printf("no no no el coco no\n");
		// Cara del culo del que se estan contando coincidencias
		for (indice_posicion_cubo_en_conteo = 0;
				indice_posicion_cubo_en_conteo < 6;
				indice_posicion_cubo_en_conteo++) {
			posicion_cubo_en_conteo_actual = *(cubo_en_conteo_actual
					+ indice_posicion_cubo_en_conteo);
			// Cubo en el que se buscan coincidencias
			for (indice_cubo_en_el_que_se_busca = 0;
					indice_cubo_en_el_que_se_busca < num_cubos;
					indice_cubo_en_el_que_se_busca++) {
				// No comparar posiciones dentro de un mismo culo
				if (indice_cubo_en_conteo == indice_cubo_en_el_que_se_busca) {
					continue;
				}
				cubo_en_el_que_se_busca_actual = *(posiciones_cubos
						+ indice_cubo_en_el_que_se_busca);
				// Inicializar el maximo numero de coincidencias dentro del cubo actual
				max_coincidencias_en_cubo_actual = 0;
				// Cara del cubo en el que se buscan coincidencias
				for (indice_posicion_cubo_en_el_que_se_busca = 0;
						indice_posicion_cubo_en_el_que_se_busca < 6;
						indice_posicion_cubo_en_el_que_se_busca++) {
					posicion_cubo_en_el_que_se_busca_actual =
							*(cubo_en_el_que_se_busca_actual
									+ indice_posicion_cubo_en_el_que_se_busca);
					/*
					 printf("Comparando %d:%d con %d:%d\n",
					 indice_cubo_en_conteo,
					 indice_posicion_cubo_en_conteo,
					 indice_cubo_en_el_que_se_busca,
					 indice_posicion_cubo_en_el_que_se_busca);*/
					// Comparacion de cubos, conteo de coincidencias
					num_coincidencias = contar_coincidencias_posiciones(
							posicion_cubo_en_conteo_actual,
							posicion_cubo_en_el_que_se_busca_actual);
					// Añadir el conteo de coincidencias actual al global de cada posicion con todas las demas posiciones en otros cubos
					*(*(conteo_posiciones_comunes_pivote + indice_cubo_en_conteo)
							+ indice_posicion_cubo_en_conteo) +=
							num_coincidencias;
					// Si el numero de coincidencias en la posicion del cubo que se busca actual es mayor al maximo registrado, actualizar dicho maximo y el indice donde se
					// encontro ese maximo
					/*
					 printf(
					 "max coincidencias %d contra num de coincidencias actuales %d\n",
					 max_coincidencias_en_cubo_actual,
					 num_coincidencias);
					 */
					if (max_coincidencias_en_cubo_actual < num_coincidencias) {
						max_coincidencias_en_cubo_actual = num_coincidencias;
						indice_max_coincidencias_en_cubo_actual =
								indice_posicion_cubo_en_el_que_se_busca;
					}
					// Guardar las coincidencias de esta comparacion
					*(*(*(*(coincidencias_en_cubo + indice_cubo_en_conteo)
							+ indice_posicion_cubo_en_conteo)
							+ indice_cubo_en_el_que_se_busca)
							+ indice_posicion_cubo_en_el_que_se_busca) =
							num_coincidencias;
				}
				// Guardar el indice donde se encontro el maximo de coincidencias para el cubo en que se busca actualmente
				*(*(*(indices_maximas_coincidencias_en_cubo
						+ indice_cubo_en_conteo)
						+ indice_posicion_cubo_en_conteo)
						+ indice_cubo_en_el_que_se_busca) =
						indice_max_coincidencias_en_cubo_actual;
				/*
				 printf(
				 "para %d:%d:%d se guardo indice de max num d coincidencias %d con %d\n",
				 indice_cubo_en_conteo, indice_posicion_cubo_en_conteo,
				 indice_cubo_en_el_que_se_busca,
				 indice_max_coincidencias_en_cubo_actual,
				 max_coincidencias_en_cubo_actual);
				 */
			}
		}
	}
}

int contar_coincidencias_posiciones(int *posicion_culo_en_conteo,
		int *posicion_culo_en_el_que_se_busca) {
	int num_coincidencias = 0;
	int indice_cara_culo_en_conteo = 0;
	int indice_cara_culo_en_el_que_se_busca = 0;
	int indice_cara_lateral_inicial = 0;
	int coincidencias_caras_laterales = 0;
	int sequencia_caras_laterales[8];
	char buffershit[512];
	char buffershit1[512];
	join_integers(posicion_culo_en_conteo, 6, buffershit, 512);
	join_integers(posicion_culo_en_el_que_se_busca, 6, buffershit1, 512);
//	printf("comparando culo %s con %s\n", buffershit, buffershit1);

	for (indice_cara_culo_en_el_que_se_busca = 0;
			indice_cara_culo_en_el_que_se_busca < 8;
			indice_cara_culo_en_el_que_se_busca++) {
		*(sequencia_caras_laterales + indice_cara_culo_en_el_que_se_busca) =
				*(posicion_culo_en_el_que_se_busca + 1
						+ indice_cara_culo_en_el_que_se_busca % 4);
	}
	for (indice_cara_lateral_inicial = 0; indice_cara_lateral_inicial < 4;
			indice_cara_lateral_inicial++) {
		coincidencias_caras_laterales = 0;
		for (indice_cara_culo_en_conteo = 0; indice_cara_culo_en_conteo < 4;
				indice_cara_culo_en_conteo++) {
			if (*(sequencia_caras_laterales + indice_cara_lateral_inicial
					+ indice_cara_culo_en_conteo)
					== *(posicion_culo_en_conteo + 1
							+ indice_cara_culo_en_conteo)) {
				coincidencias_caras_laterales++;
			}
		}
		if (coincidencias_caras_laterales > num_coincidencias) {
			num_coincidencias = coincidencias_caras_laterales;
		}
	}
	if (*posicion_culo_en_conteo == *posicion_culo_en_el_que_se_busca) {
		num_coincidencias++;
	}
	if (*(posicion_culo_en_conteo + 5)
			== *(posicion_culo_en_el_que_se_busca + 5)) {
		num_coincidencias++;
	}

	return num_coincidencias;
}

void imprimir_conteo_posiciones_comunes(
		int conteo_posiciones_comunes_pivote[4][6], int num_cubos) {
	int indice_cubo = 0;
	int indice_posicion_cubo = 0;
	for (indice_cubo = 0; indice_cubo < num_cubos; indice_cubo++) {
		printf("PAra el cubo %d:\n", indice_cubo);
		for (indice_posicion_cubo = 0; indice_posicion_cubo < 6;
				indice_posicion_cubo++) {
			printf("\tEn la posicion %d: %d\n", indice_posicion_cubo,
					*(*(conteo_posiciones_comunes_pivote + indice_cubo)
							+ indice_posicion_cubo));
		}
	}
}

int calcular_num_minimo_de_caras_diferentes(
		int conteo_posiciones_comunes_pivote[4][6], int num_cubos,
		int (*indices_maximas_coincidencias_en_cubo)[6][4],
		int coincidencias_en_cubo[4][6][4][6]) {
	int indice_cubo = 0;
	int indice_posicion_cubo = 0;
	int indice_maximas_coincidencias = 0;
	int maximas_coincidencias = 0;
	int indice_posicion_maximas_coincidencias = 0;
	int indice_cubo_maximas_coincidencias = 0;

	for (indice_cubo = 0; indice_cubo < num_cubos; indice_cubo++) {
		for (indice_posicion_cubo = 0; indice_posicion_cubo < 6;
				indice_posicion_cubo++) {
			if (*(*(conteo_posiciones_comunes_pivote + indice_cubo)
					+ indice_posicion_cubo) > maximas_coincidencias) {
				maximas_coincidencias = *(*(conteo_posiciones_comunes_pivote
						+ indice_cubo) + indice_posicion_cubo);
				indice_cubo_maximas_coincidencias = indice_cubo;
				indice_posicion_maximas_coincidencias = indice_posicion_cubo;
			}
		}
	}

	maximas_coincidencias = 0;
	for (indice_cubo = 0; indice_cubo < num_cubos; indice_cubo++) {
		if (indice_cubo == indice_cubo_maximas_coincidencias) {
			continue;
		}
		indice_maximas_coincidencias =
				*(*(*(indices_maximas_coincidencias_en_cubo
						+ indice_cubo_maximas_coincidencias)
						+ indice_posicion_maximas_coincidencias) + indice_cubo);
		maximas_coincidencias += *(*(*(*(coincidencias_en_cubo
				+ indice_cubo_maximas_coincidencias)
				+ indice_posicion_maximas_coincidencias) + indice_cubo)
				+ indice_maximas_coincidencias);
		printf("comparacion %d:%d-%d:%d es %d\n",
				indice_cubo_maximas_coincidencias,
				indice_posicion_maximas_coincidencias, indice_cubo,
				indice_maximas_coincidencias, maximas_coincidencias);
		printf("puta mierda de %d:%d-%d %d\n",
				indice_cubo_maximas_coincidencias,
				indice_posicion_maximas_coincidencias, indice_cubo,
				maximas_coincidencias);

	}
	return (num_cubos - 1) * 6 - maximas_coincidencias;
}
