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

char mapa_colores[1024][25];
int num_colores_encontrados = 0;

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
		int *posicion_culo_en_el_que_se_busca, char acomodar_caras_laterales);
void imprimir_conteo_posiciones_comunes(
		int conteo_posiciones_comunes_pivote[4][6], int num_cubos);
int calcular_num_minimo_de_caras_diferentes(
		int conteo_posiciones_comunes_pivote[4][6], int num_cubos,
		int (*indices_maximas_coincidencias_en_cubo)[6][4],
		int coincidencias_en_cubo[4][6][4][6], int posiciones_cubos[4][6][6]);
int equivalencia_color(char *nombre_color);

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
	sleep(10);
	return 0;

	if (argc != 2) /* argc should be 2 for correct execution */
	{
		/* We print argv[0] assuming it is the program name */
		printf("usage: %s filename\n", argv[0]);
		return EXIT_FAILURE;
	}
	fr = fopen(argv[1], "rt");

	num_colores_encontrados = 0;

	while (fgets(buffer, 80, fr) != NULL ) {
		if (*buffer == '0') {
			break;
		}
		num_de_cubos = strtol(buffer, NULL, 10);
		if (num_de_cubos) {
			if (anterior_num_de_cubos) {
				generar_posiciones_cubos(definiciones_cubos, posiciones_cubos,
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
								coincidencias_en_cubo, posiciones_cubos);
				printf("%d\n", num_min_caras_diferentes);

			}
			anterior_num_de_cubos = num_de_cubos;
			num_de_cubos_guardados = 0;
		} else {
			mapear_definicion_cubo(definicion_cubo, buffer);
			join_integers(definicion_cubo, 6, buffershit, 512);
			/*
			 printf("yo se que ya no m kieres %s\n", buffershit);
			 */
			memcpy((void * )*(definiciones_cubos + num_de_cubos_guardados),
					(void * )definicion_cubo, 6 * (sizeof(int)));
			num_de_cubos_guardados++;

		}
	}
	generar_posiciones_cubos(definiciones_cubos, posiciones_cubos,
			num_de_cubos_guardados);
	comparar_cubos(posiciones_cubos, num_de_cubos_guardados,
			conteo_posiciones_comunes_pivote,
			indices_maximas_coincidencias_en_cubo, coincidencias_en_cubo);
	num_min_caras_diferentes = calcular_num_minimo_de_caras_diferentes(
			conteo_posiciones_comunes_pivote, num_de_cubos_guardados,
			indices_maximas_coincidencias_en_cubo, coincidencias_en_cubo,
			posiciones_cubos);
	printf("%d\n", num_min_caras_diferentes);
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
		if ((valor_color = equivalencia_color(pch)) == -1) {
			strcpy(*(mapa_colores + num_colores_encontrados), pch);
			valor_color = num_colores_encontrados++;
		}

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
				snprintf(buf + written, buf_len - written, (i != 0 ? ",\t%u" : "%u"),
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
	for (i = 0; i < num_cubos; i++) {
		for (indice_cara_cubo = 0; indice_cara_cubo < 6; indice_cara_cubo++) {
			join_integers(*(definiciones_cubos + i), 6, buffer, 512);
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

	for (indice_cubo_en_conteo = 0; indice_cubo_en_conteo < num_cubos;
			indice_cubo_en_conteo++) {
		for (indice_posicion_cubo_en_conteo = 0;
				indice_posicion_cubo_en_conteo < 6;
				indice_posicion_cubo_en_conteo++) {
			*(*(conteo_posiciones_comunes_pivote + indice_cubo_en_conteo)
					+ indice_posicion_cubo_en_conteo) = 0;
		}
	}

	for (indice_cubo_en_conteo = 0; indice_cubo_en_conteo < num_cubos;
			indice_cubo_en_conteo++) {
		cubo_en_conteo_actual = *(posiciones_cubos + indice_cubo_en_conteo);
		for (indice_posicion_cubo_en_conteo = 0;
				indice_posicion_cubo_en_conteo < 6;
				indice_posicion_cubo_en_conteo++) {
			posicion_cubo_en_conteo_actual = *(cubo_en_conteo_actual
					+ indice_posicion_cubo_en_conteo);
			for (indice_cubo_en_el_que_se_busca = 0;
					indice_cubo_en_el_que_se_busca < num_cubos;
					indice_cubo_en_el_que_se_busca++) {
				if (indice_cubo_en_conteo == indice_cubo_en_el_que_se_busca) {
					continue;
				}
				cubo_en_el_que_se_busca_actual = *(posiciones_cubos
						+ indice_cubo_en_el_que_se_busca);
				max_coincidencias_en_cubo_actual = 0;
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
					num_coincidencias = contar_coincidencias_posiciones(
							posicion_cubo_en_conteo_actual,
							posicion_cubo_en_el_que_se_busca_actual, 0);
					*(*(conteo_posiciones_comunes_pivote + indice_cubo_en_conteo)
							+ indice_posicion_cubo_en_conteo) +=
							num_coincidencias;
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
					*(*(*(*(coincidencias_en_cubo + indice_cubo_en_conteo)
							+ indice_posicion_cubo_en_conteo)
							+ indice_cubo_en_el_que_se_busca)
							+ indice_posicion_cubo_en_el_que_se_busca) =
							num_coincidencias;
				}
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
		int *posicion_culo_en_el_que_se_busca, char acomodar_caras_laterales) {
	int num_coincidencias = 0;
	int indice_cara_culo_en_conteo = 0;
	int indice_cara_culo_en_el_que_se_busca = 0;
	int indice_cara_lateral_inicial = 0;
	int coincidencias_caras_laterales = 0;
	int indice_maximas_coincidencias = 0;
	int sequencia_caras_laterales[8];
	char buffershit[512];
	char buffershit1[512];
	join_integers(posicion_culo_en_conteo, 6, buffershit, 512);
	join_integers(posicion_culo_en_el_que_se_busca, 6, buffershit1, 512);
	/*
	 printf("comparando culo %s con %s\n", buffershit, buffershit1);
	 */
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
			indice_maximas_coincidencias = indice_cara_lateral_inicial;
		}
	}
	if (*posicion_culo_en_conteo == *posicion_culo_en_el_que_se_busca) {
		num_coincidencias++;
	}
	if (*(posicion_culo_en_conteo + 5)
			== *(posicion_culo_en_el_que_se_busca + 5)) {
		num_coincidencias++;
	}

	if (acomodar_caras_laterales) {
		for (indice_cara_culo_en_conteo = 0; indice_cara_culo_en_conteo < 4;
				indice_cara_culo_en_conteo++) {
			*(posicion_culo_en_el_que_se_busca + 1 + indice_cara_culo_en_conteo) =
					*(sequencia_caras_laterales + indice_maximas_coincidencias
							+ indice_cara_culo_en_conteo);
		}
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
		int coincidencias_en_cubo[4][6][4][6], int posiciones_cubos[4][6][6]) {
	int indice_cubo = 0;
	int indice_posicion_cubo = 0;
	int indice_maximas_coincidencias = 0;
	int maximas_coincidencias = 0;
	int indice_posicion_maximas_coincidencias = 0;
	int indice_cubo_maximas_coincidencias = 0;
	int color = 0;
	int color_ocurrencias_maximas = 0;
	int caras_a_cambiar = 0;
	int *posicion_cubo_pivote;
	int *posicion_cubo_a_arreglar_caras_laterales;
	int *cubos_a_comparar[4];
	int mapa_conteo_colores[1024];
	char pasitotuntun[512];

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
		*(cubos_a_comparar + indice_cubo) = NULL;

	}

	posicion_cubo_pivote = *(*(posiciones_cubos
			+ indice_cubo_maximas_coincidencias)
			+ indice_posicion_maximas_coincidencias);
	maximas_coincidencias = 0;
	cubos_a_comparar[indice_cubo_maximas_coincidencias] = posicion_cubo_pivote;
	join_integers(posicion_cubo_pivote, 6, pasitotuntun, 512);
	/*
	 printf("si bailas de aki, paya %s\n", pasitotuntun);
	 */
	for (indice_cubo = 0; indice_cubo < num_cubos; indice_cubo++) {
		if (indice_cubo == indice_cubo_maximas_coincidencias) {
			continue;
		}

		indice_maximas_coincidencias =
				*(*(*(indices_maximas_coincidencias_en_cubo
						+ indice_cubo_maximas_coincidencias)
						+ indice_posicion_maximas_coincidencias) + indice_cubo);

		posicion_cubo_a_arreglar_caras_laterales = *(*(posiciones_cubos
				+ indice_cubo) + indice_maximas_coincidencias);
		cubos_a_comparar[indice_cubo] =
				posicion_cubo_a_arreglar_caras_laterales;
		contar_coincidencias_posiciones(posicion_cubo_pivote,
				posicion_cubo_a_arreglar_caras_laterales, 1);
		join_integers(posicion_cubo_a_arreglar_caras_laterales, 6, pasitotuntun,
				512);
		/*
		 printf("si bailas de aya, paca %s\n", pasitotuntun);
		 */

	}
	for (indice_posicion_cubo = 0; indice_posicion_cubo < 6;
			indice_posicion_cubo++) {
		for (color = 0; color < 1024; color++) {
			mapa_conteo_colores[color] = 0;
		}
		for (indice_cubo = 0; indice_cubo < num_cubos; indice_cubo++) {
			color = *(*(cubos_a_comparar + indice_cubo) + indice_posicion_cubo);
			mapa_conteo_colores[color] += 1;

			if (mapa_conteo_colores[color]
					> mapa_conteo_colores[color_ocurrencias_maximas]) {
				color_ocurrencias_maximas = color;
			}
		}
		caras_a_cambiar += num_cubos
				- mapa_conteo_colores[color_ocurrencias_maximas];

	}

	return caras_a_cambiar;
}

int equivalencia_color(char *nombre_color) {
	int indice_color = 0;
	for (indice_color = 0; indice_color < num_colores_encontrados;
			indice_color++) {
		if (!strcmp(*(mapa_colores + indice_color), nombre_color)) {
			return indice_color;
		}
	}
	return -1;
}
