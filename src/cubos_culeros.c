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
#include <limits.h>

char mapa_colores[1024][25];
int num_colores_encontrados = 0;

void mapear_definicion_cubo(int *array_definiciones_cubos,
		char *cadena_definicion_cubos);
int equivalencia_color(char *nombre_color);
size_t join_integers(const int *num, size_t num_len, char *buf,
		size_t buf_len);
void generar_posiciones_pivote(int (*definiciones_cubos)[6],
		int (*posiciones_cubos)[6][6], int num_cubos);
void generar_posiciones_cubo(int (*definiciones_cubos)[6],
		int (*posiciones_pivote)[6][6], int posiciones_cubos[4][6][4][6],
		int num_cubos);
void imprimir_posiciones_cubos(int (*posiciones_cubos)[6][4][6], int num_cubos);
int comparar_cubos(int (*posiciones_cubos)[6][4][6], int num_cubos);
void contador_gauger(int acumuladores[4], int carries[4], int num_acumuladores,
		int tope_contadores);

int contar_diferencias(int posiciones_cubos[4][6][4][6], int indices_pivotes[4],
		int indices_rotaciones[4], int num_cubos);

int main(int argc, char *argv[]) {
	char *buffer;
	char buffershit[512];
	int i;
	int nread;
	int num_de_cubos = 0;
	int num_de_cubos_guardados = 0;
	int anterior_num_de_cubos = 0;
	int num_min_caras_diferentes = 0;
	int definicion_cubo[6];
	int definiciones_cubos[4][6];
	int posiciones_pivote[4][6][6];
	/* 4 cubos x 6 caras pivote x 4 rotaciones x 6 caras */
	int posiciones_cubos[4][6][4][6];
	int conteo_posiciones_comunes_pivote[4][6];
	int indices_maximas_coincidencias_en_cubo[4][6][4];
	int coincidencias_en_cubo[4][6][4][6];
	int lineas_input_usuario = 0;
	int indice_lineas_usuaio = 0;
	char input_usuario[1024][150];

	gets(*(input_usuario + lineas_input_usuario));
	lineas_input_usuario++;
	while (!(strlen(*(input_usuario + lineas_input_usuario - 1)) == 1
			&& *(*(input_usuario + lineas_input_usuario - 1)) == '0')) {
		gets(*(input_usuario + lineas_input_usuario));
		lineas_input_usuario++;
	}

	num_colores_encontrados = 0;

	for (indice_lineas_usuaio = 0; indice_lineas_usuaio < lineas_input_usuario;
			indice_lineas_usuaio++) {
		buffer = *(input_usuario + indice_lineas_usuaio);
		if (*buffer == '0') {
			break;
		}
		num_de_cubos = strtol(buffer, NULL, 10);
		if (num_de_cubos) {
//			printf("numbero de culos %d\n", num_de_cubos);
			if (anterior_num_de_cubos) {
				generar_posiciones_pivote(definiciones_cubos, posiciones_pivote,
						num_de_cubos_guardados);
				generar_posiciones_cubo(definiciones_cubos, posiciones_pivote,
						posiciones_cubos, num_de_cubos_guardados);
				imprimir_posiciones_cubos(posiciones_cubos,
						num_de_cubos_guardados);
				/*
				 comparar_cubos(posiciones_cubos, num_de_cubos_guardados,
				 conteo_posiciones_comunes_pivote,
				 indices_maximas_coincidencias_en_cubo,
				 coincidencias_en_cubo);
				 imprimir_conteo_posiciones_comunes(
				 conteo_posiciones_comunes_pivote,
				 num_de_cubos_guardados);
				 num_min_caras_diferentes =
				 calcular_num_minimo_de_caras_diferentes(
				 conteo_posiciones_comunes_pivote,
				 num_de_cubos_guardados,
				 indices_maximas_coincidencias_en_cubo,
				 coincidencias_en_cubo, posiciones_cubos);
				 printf("%d\n", num_min_caras_diferentes);
				 */

			}
			anterior_num_de_cubos = num_de_cubos;
			num_de_cubos_guardados = 0;
		} else {
//			printf("definicion de culo %s", buffer);
			mapear_definicion_cubo(definicion_cubo, buffer);
			join_integers(definicion_cubo, 6, buffershit, 512);
//			printf("yo se que ya no m kieres %s\n", buffershit);
			memcpy((void *) *(definiciones_cubos + num_de_cubos_guardados),
					(void *) definicion_cubo, 6 * (sizeof(int)));
			num_de_cubos_guardados++;

		}
	}
	generar_posiciones_pivote(definiciones_cubos, posiciones_pivote,
			num_de_cubos_guardados);
	generar_posiciones_cubo(definiciones_cubos, posiciones_pivote,
			posiciones_cubos, num_de_cubos_guardados);
	imprimir_posiciones_cubos(posiciones_cubos, num_de_cubos_guardados);
	/*
	 comparar_cubos(posiciones_cubos, num_de_cubos_guardados,
	 conteo_posiciones_comunes_pivote,
	 indices_maximas_coincidencias_en_cubo, coincidencias_en_cubo);
	 num_min_caras_diferentes = calcular_num_minimo_de_caras_diferentes(
	 conteo_posiciones_comunes_pivote, num_de_cubos_guardados,
	 indices_maximas_coincidencias_en_cubo, coincidencias_en_cubo,
	 posiciones_cubos);
	 printf("%d\n", num_min_caras_diferentes);
	 */

	return EXIT_SUCCESS;
}

void mapear_definicion_cubo(int *array_definiciones_cubos,
		char *cadena_definicion_cubos) {
	char * pch;
	int index = 0;
	int valor_color = 0;
	pch = strtok(cadena_definicion_cubos, " \n");
	while (pch != NULL) {
//		printf("token %s\n", pch);
		if ((valor_color = equivalencia_color(pch)) == -1) {
			strcpy(*(mapa_colores + num_colores_encontrados), pch);
			valor_color = num_colores_encontrados++;
		}

//		printf("valor color %d\n", valor_color);
		*(array_definiciones_cubos + index) = valor_color;
		index++;
		pch = strtok(NULL, " \n");
	}
}

size_t join_integers(const int *num, size_t num_len, char *buf,
		size_t buf_len) {
	size_t i;
	unsigned int written = 0;

	for (i = 0; i < num_len; i++) {
		written += snprintf(buf + written, buf_len - written,
				(i != 0 ? ",\t%u" : "%u"), *(num + i));
		if (written == buf_len)
			break;
	}

	return written;
}

void generar_posiciones_pivote(int (*definiciones_cubos)[6],
		int (*posiciones_pivote)[6][6], int num_cubos) {
	int i = 0;
	int indice_cara_cubo = 0;
	char buffer[512];
//	printf("generando mierdassssssss %d\n", num_cubos);
//	printf("como dona a su agujero %d\n", *((*(definiciones_cubos + 1)) + 0));
	for (i = 0; i < num_cubos; i++) {
//		printf("numero de culo %d", i);
		for (indice_cara_cubo = 0; indice_cara_cubo < 6; indice_cara_cubo++) {
			join_integers(*(definiciones_cubos + i), 6, buffer, 512);
//			printf("cara de culo pivote %d: %s\n", indice_cara_cubo, buffer);
			switch (indice_cara_cubo) {
			case 0:
				*(*(*(posiciones_pivote + i) + indice_cara_cubo) + 0) =
						*(*(definiciones_cubos + i) + 0);
				*(*(*(posiciones_pivote + i) + indice_cara_cubo) + 1) =
						*(*(definiciones_cubos + i) + 2);
				*(*(*(posiciones_pivote + i) + indice_cara_cubo) + 2) =
						*(*(definiciones_cubos + i) + 1);
				*(*(*(posiciones_pivote + i) + indice_cara_cubo) + 3) =
						*(*(definiciones_cubos + i) + 3);
				*(*(*(posiciones_pivote + i) + indice_cara_cubo) + 4) =
						*(*(definiciones_cubos + i) + 4);
				*(*(*(posiciones_pivote + i) + indice_cara_cubo) + 5) =
						*(*(definiciones_cubos + i) + 5);

				break;
			case 1:
				*(*(*(posiciones_pivote + i) + indice_cara_cubo) + 0) =
						*(*(definiciones_cubos + i) + 1);
				*(*(*(posiciones_pivote + i) + indice_cara_cubo) + 1) =
						*(*(definiciones_cubos + i) + 2);
				*(*(*(posiciones_pivote + i) + indice_cara_cubo) + 2) =
						*(*(definiciones_cubos + i) + 5);
				*(*(*(posiciones_pivote + i) + indice_cara_cubo) + 3) =
						*(*(definiciones_cubos + i) + 3);
				*(*(*(posiciones_pivote + i) + indice_cara_cubo) + 4) =
						*(*(definiciones_cubos + i) + 0);
				*(*(*(posiciones_pivote + i) + indice_cara_cubo) + 5) =
						*(*(definiciones_cubos + i) + 4);
				break;
			case 2:
				*(*(*(posiciones_pivote + i) + indice_cara_cubo) + 0) =
						*(*(definiciones_cubos + i) + 2);
				*(*(*(posiciones_pivote + i) + indice_cara_cubo) + 1) =
						*(*(definiciones_cubos + i) + 1);
				*(*(*(posiciones_pivote + i) + indice_cara_cubo) + 2) =
						*(*(definiciones_cubos + i) + 0);
				*(*(*(posiciones_pivote + i) + indice_cara_cubo) + 3) =
						*(*(definiciones_cubos + i) + 4);
				*(*(*(posiciones_pivote + i) + indice_cara_cubo) + 4) =
						*(*(definiciones_cubos + i) + 5);
				*(*(*(posiciones_pivote + i) + indice_cara_cubo) + 5) =
						*(*(definiciones_cubos + i) + 3);
				break;
			case 3:
				*(*(*(posiciones_pivote + i) + indice_cara_cubo) + 0) =
						*(*(definiciones_cubos + i) + 3);
				*(*(*(posiciones_pivote + i) + indice_cara_cubo) + 1) =
						*(*(definiciones_cubos + i) + 4);
				*(*(*(posiciones_pivote + i) + indice_cara_cubo) + 2) =
						*(*(definiciones_cubos + i) + 0);
				*(*(*(posiciones_pivote + i) + indice_cara_cubo) + 3) =
						*(*(definiciones_cubos + i) + 1);
				*(*(*(posiciones_pivote + i) + indice_cara_cubo) + 4) =
						*(*(definiciones_cubos + i) + 5);
				*(*(*(posiciones_pivote + i) + indice_cara_cubo) + 5) =
						*(*(definiciones_cubos + i) + 2);
				break;
			case 4:
				*(*(*(posiciones_pivote + i) + indice_cara_cubo) + 0) =
						*(*(definiciones_cubos + i) + 4);
				*(*(*(posiciones_pivote + i) + indice_cara_cubo) + 1) =
						*(*(definiciones_cubos + i) + 2);
				*(*(*(posiciones_pivote + i) + indice_cara_cubo) + 2) =
						*(*(definiciones_cubos + i) + 0);
				*(*(*(posiciones_pivote + i) + indice_cara_cubo) + 3) =
						*(*(definiciones_cubos + i) + 3);
				*(*(*(posiciones_pivote + i) + indice_cara_cubo) + 4) =
						*(*(definiciones_cubos + i) + 5);
				*(*(*(posiciones_pivote + i) + indice_cara_cubo) + 5) =
						*(*(definiciones_cubos + i) + 1);
				break;
			case 5:
				*(*(*(posiciones_pivote + i) + indice_cara_cubo) + 0) =
						*(*(definiciones_cubos + i) + 5);
				*(*(*(posiciones_pivote + i) + indice_cara_cubo) + 1) =
						*(*(definiciones_cubos + i) + 2);
				*(*(*(posiciones_pivote + i) + indice_cara_cubo) + 2) =
						*(*(definiciones_cubos + i) + 4);
				*(*(*(posiciones_pivote + i) + indice_cara_cubo) + 3) =
						*(*(definiciones_cubos + i) + 3);
				*(*(*(posiciones_pivote + i) + indice_cara_cubo) + 4) =
						*(*(definiciones_cubos + i) + 1);
				*(*(*(posiciones_pivote + i) + indice_cara_cubo) + 5) =
						*(*(definiciones_cubos + i) + 0);
				break;
			default:
				break;
			}
		}
	}
}

void generar_posiciones_cubo(int (*definiciones_cubos)[6],
		int (*posiciones_pivote)[6][6], int posiciones_cubos[4][6][4][6],
		int num_cubos) {
//	int acumuladores[] = { 0, 0, 0, 0 };
//	int carries[] = { 0, 0, 0, 0 };
	int num_cubo = 0;
	int num_pivote = 0;
	int num_cara_lateral = 0;
	int num_rotacion = 0;
	int *cubo_pivote = NULL;
	int *cubo_rotacion = NULL;
	int sequencia_caras_laterales[8];

	for (num_cubo = 0; num_cubo < num_cubos; ++num_cubo) {
		for (num_pivote = 0; num_pivote < 6; ++num_pivote) {
			// Obteniendo el culo pivote
			cubo_pivote = *(*(posiciones_pivote + num_cubo) + num_pivote);
			// Generando la secuencia de caras laterales
			for (num_cara_lateral = 0; num_cara_lateral < 8;
					num_cara_lateral++) {
				*(sequencia_caras_laterales + num_cara_lateral) = *(cubo_pivote
						+ 1 + num_cara_lateral % 4);
			}
			// Generando rotaciones de cubos
			for (num_rotacion = 0; num_rotacion < 4; ++num_rotacion) {
				cubo_rotacion = *(*(*(posiciones_cubos + num_cubo) + num_pivote)
						+ num_rotacion);
				*(cubo_rotacion + 0) = *(cubo_pivote + 0);
				*(cubo_rotacion + 5) = *(cubo_pivote + 5);
				for (num_cara_lateral = 0; num_cara_lateral < 4;
						++num_cara_lateral) {
					*(cubo_rotacion + 1 + num_cara_lateral) =
							*(sequencia_caras_laterales + num_rotacion
									+ num_cara_lateral);
				}
			}

		}
	}

}

void imprimir_posiciones_cubos(int (*posiciones_cubos)[6][4][6], int num_cubos) {
	int indice_cubo = 0;
	int indice_cara_pivote = 0;
	int indice_cara = 0;
	int indice_rotacion = 0;
	char buffershit[512];
	for (indice_cubo = 0; indice_cubo < num_cubos; indice_cubo++) {
		printf("Para el cubo %d:\n", indice_cubo);
		for (indice_cara_pivote = 0; indice_cara_pivote < 6;
				indice_cara_pivote++) {
			printf("\tPara el pivote %d\n", indice_cara_pivote);
			for (indice_rotacion = 0; indice_rotacion < 4; ++indice_rotacion) {
				join_integers(
						*(*(*(posiciones_cubos + indice_cubo)
								+ indice_cara_pivote) + indice_rotacion), 6,
						buffershit, 512);
				printf("\t\tRotacion %d:\t%s\n", indice_rotacion, buffershit);
			}
		}
	}

}

void contador_gauger(int acumuladores[4], int carries[4], int num_acumuladores,
		int tope_contadores) {
	int indice_acumulador = 0;
	for (indice_acumulador = num_acumuladores - 1; indice_acumulador > 0;
			indice_acumulador--) {
		if (indice_acumulador < num_acumuladores
				&& *(carries + indice_acumulador + 1)) {
			if (++*(acumuladores + indice_acumulador) > tope_contadores) {
				*(carries + indice_acumulador) = 1;
				*(acumuladores + indice_acumulador) = 0;
			}
			*(carries + indice_acumulador + 1) = 0;
		}
		// El carry podria estar encendido, y aun asi, aumenta el acumulador
		if (++*(acumuladores + indice_acumulador) > tope_contadores) {
			*(carries + indice_acumulador) = 1;
			*(acumuladores + indice_acumulador) = 0;
		}
	}
}

int contar_diferencias(int posiciones_cubos[4][6][4][6], int indices_pivotes[4],
		int indices_rotaciones[4], int num_cubos) {
	int color = 0;
	int num_diferencias = 0;
	int indice_cara = 0;
	int indice_cubo = 0;
	int color_ocurrencias_maximas = 0;
	int mapa_conteo_colores[1024];
	int *cubo_actual = NULL;

	for (indice_cara = 0; indice_cara < 6; ++indice_cara) {
		// Reset del mapa de colores para cada cara.
		for (color = 0; color < 1024; color++) {
			mapa_conteo_colores[color] = 0;
		}
		for (indice_cubo = 0; indice_cubo < num_cubos; ++indice_cubo) {
			cubo_actual = *(*(*(posiciones_cubos + indice_cubo)
					+ *(indices_pivotes + indice_cubo))
					+ *(indices_rotaciones + indice_cubo));
			color = *(cubo_actual + indice_cara);
			mapa_conteo_colores[color]++;

			if (mapa_conteo_colores[color]
					> mapa_conteo_colores[color_ocurrencias_maximas]) {
				color_ocurrencias_maximas = color;
			}

		}
		num_diferencias += num_cubos
				- mapa_conteo_colores[color_ocurrencias_maximas];
	}

	return num_diferencias;
}

int comparar_cubos(int (*posiciones_cubos)[6][4][6], int num_cubos) {
	int num_diferencias_actual = 0;
	int min_num_diferencias = 0;
	int indice_carries_indices_rotaciones = 0;
	int indices_pivotes[] = { 0, 0, 0, 0 };
	int carries_indices_pivotes[] = { 0, 0, 0, 0 };
	int indices_rotaciones[] = { 0, 0, 0, 0 };
	int carries_indices_rotaciones[] = { 0, 0, 0, 0 };

	min_num_diferencias = INT_MAX;

	while (!*carries_indices_pivotes) {
		while (!*carries_indices_rotaciones) {
			num_diferencias_actual = contar_diferencias(posiciones_cubos,
					indices_pivotes, indices_rotaciones, num_cubos);
			if (num_diferencias_actual < min_num_diferencias) {
				min_num_diferencias = num_diferencias_actual;
			}
		}
		*carries_indices_pivotes = 0;
	}

	return min_num_diferencias;
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
