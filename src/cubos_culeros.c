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
int equivalencia_color(char *nombre_color);
size_t join_integers(const unsigned int *num, size_t num_len, char *buf,
		size_t buf_len);
void generar_posiciones_pivote(int (*definiciones_cubos)[6],
		int (*posiciones_cubos)[6][6], int num_cubos);
void generar_posiciones_cubo(int (*definiciones_cubos)[6],
		int (*posiciones_pivote)[6][6], int posiciones_cubos[4][6][4][6],
		int num_cubos);
void imprimir_posiciones_cubos(int (*posiciones_cubos)[6][4][6], int num_cubos);
/*
 void comparar_cubos(int (*posiciones_cubos)[6][4][6], int num_cubos,
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
 int coincidencias_en_cubo[4][6][4][6], int posiciones_cubos[4][6][4][6]);
 */

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

size_t join_integers(const unsigned int *num, size_t num_len, char *buf,
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
//	printf("comparando culo %s con %s\n", buffershit, buffershit1);

	for (indice_cara_culo_en_el_que_se_busca = 0;
			indice_cara_culo_en_el_que_se_busca < 8;
			indice_cara_culo_en_el_que_se_busca++) {
		*(sequencia_caras_laterales + indice_cara_culo_en_el_que_se_busca) =
				*(posicion_culo_en_el_que_se_busca + 1
						+ indice_cara_culo_en_el_que_se_busca % 4);
	}
// Comparacion de cubos iniciando por cada cara lateral
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
// Checando la cara de arriba
	if (*posicion_culo_en_conteo == *posicion_culo_en_el_que_se_busca) {
		num_coincidencias++;
	}
// Checando la cara de abajo
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
							posicion_cubo_en_el_que_se_busca_actual, 0);
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

// Sacando el pivote con mayor numero de coincidencias
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
		// Inicializando los cubos a comparar
		*(cubos_a_comparar + indice_cubo) = NULL;

	}

// Arreglar las caras laterales de los cubos
	posicion_cubo_pivote = *(*(posiciones_cubos
			+ indice_cubo_maximas_coincidencias)
			+ indice_posicion_maximas_coincidencias);
	maximas_coincidencias = 0;
	cubos_a_comparar[indice_cubo_maximas_coincidencias] = posicion_cubo_pivote;
	join_integers(posicion_cubo_pivote, 6, pasitotuntun, 512);
	printf("si bailas de aki, paya %s\n", pasitotuntun);
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
		printf("si bailas de aya, paca %s\n", pasitotuntun);

	}
// Haciendo el conteo por columna
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
