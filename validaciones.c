#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "validaciones.h"

#define TAM_BUFFER 100
#define ANCHO 62

/* Lee una linea con fgets. Si la linea es mas larga que el buffer, descarta
   el resto para que no contamine la lectura siguiente. Si se termina la
   entrada (EOF) avisa y cierra el programa, asi nunca hay bucle infinito. */
static void leer_linea(char *buffer, int tamano)
{
    int c;

    if (fgets(buffer, tamano, stdin) == NULL) {
        printf("\n");
        mostrar_aviso("Fin de la entrada. Se cierra el programa.");
        exit(0);
    }
    if (strchr(buffer, '\n') == NULL) {
        while ((c = getchar()) != '\n' && c != EOF) {
            /* descarta el resto de la linea */
        }
    }
}

/* Devuelve 1 si el texto tiene solo espacios (o esta vacio) */
static int es_vacia(const char *texto)
{
    int i;

    for (i = 0; texto[i] != '\0'; i++) {
        if (!isspace((unsigned char)texto[i])) {
            return 0;
        }
    }
    return 1;
}

/* Reemplaza la coma por punto para aceptar decimales como 0,5 */
static void coma_a_punto(char *texto)
{
    int i;

    for (i = 0; texto[i] != '\0'; i++) {
        if (texto[i] == ',') {
            texto[i] = '.';
        }
    }
}

/* Lee un entero entre min y max. Repite hasta que sea valido. */
int leer_entero(const char *mensaje, int min, int max)
{
    char buffer[TAM_BUFFER];
    char texto_error[100];
    char *fin;
    long numero;

    while (1) {
        printf("%s", mensaje);
        leer_linea(buffer, TAM_BUFFER);

        if (es_vacia(buffer)) {
            mostrar_error("No ingreso ningun valor.");
            continue;
        }
        numero = strtol(buffer, &fin, 10);
        /* despues del numero solo pueden quedar espacios */
        if (!es_vacia(fin)) {
            mostrar_error("Ingrese un numero entero valido (ejemplo: 3).");
            continue;
        }
        if (numero < min || numero > max) {
            sprintf(texto_error, "El valor debe estar entre %d y %d.", min, max);
            mostrar_error(texto_error);
            continue;
        }
        return (int)numero;
    }
}

/* Lee un numero real entre min y max. Acepta coma o punto decimal. */
double leer_double(const char *mensaje, double min, double max)
{
    char buffer[TAM_BUFFER];
    char texto_error[100];
    char *fin;
    double numero;

    while (1) {
        printf("%s", mensaje);
        leer_linea(buffer, TAM_BUFFER);

        if (es_vacia(buffer)) {
            mostrar_error("No ingreso ningun valor.");
            continue;
        }
        coma_a_punto(buffer);
        numero = strtod(buffer, &fin);
        /* strtod acepta "nan" e "inf", por eso se pregunta isfinite */
        if (!es_vacia(fin) || !isfinite(numero)) {
            mostrar_error("Ingrese un numero valido (ejemplo: 192 o 192.5).");
            continue;
        }
        if (numero < min || numero > max) {
            sprintf(texto_error, "El valor debe estar entre %g y %g.", min, max);
            mostrar_error(texto_error);
            continue;
        }
        return numero;
    }
}

/* Pregunta s/n. Devuelve 1 si es 's' y 0 si es 'n'. */
int leer_si_no(const char *mensaje)
{
    char buffer[TAM_BUFFER];
    char letra;

    while (1) {
        printf("%s", mensaje);
        leer_linea(buffer, TAM_BUFFER);

        if (es_vacia(buffer)) {
            mostrar_error("Responda con s o n.");
            continue;
        }
        /* se toma el primer caracter que no es espacio */
        letra = buffer[strspn(buffer, " \t")];
        if (!es_vacia(buffer + strspn(buffer, " \t") + 1)) {
            mostrar_error("Responda con s o n.");
            continue;
        }
        if (letra == 's' || letra == 'S') {
            return 1;
        }
        if (letra == 'n' || letra == 'N') {
            return 0;
        }
        mostrar_error("Responda con s o n.");
    }
}

/* Imprime una linea de caracteres repetidos del ancho de la pantalla */
static void imprimir_repetido(char caracter)
{
    int i;

    for (i = 0; i < ANCHO; i++) {
        putchar(caracter);
    }
    putchar('\n');
}

/* Titulo entre dos lineas */
void mostrar_titulo(const char *texto)
{
    mostrar_linea();
    printf("  %s\n", texto);
    mostrar_linea();
}

/* Linea separadora de 62 guiones */
void mostrar_linea(void)
{
    imprimir_repetido('-');
}

/* Linea separadora de 62 signos igual (para el encabezado del menu) */
void mostrar_linea_doble(void)
{
    imprimir_repetido('=');
}

void mostrar_error(const char *texto)
{
    printf("[ERROR] %s\n", texto);
}

void mostrar_aviso(const char *texto)
{
    printf("[AVISO] %s\n", texto);
}

/* Muestra un dato alineado: etiqueta, valor y unidad */
void mostrar_dato(const char *etiqueta, double valor, const char *unidad)
{
    printf("  %-30s %14.2f %s\n", etiqueta, valor, unidad);
}

void pausar(void)
{
    char buffer[TAM_BUFFER];

    printf("\nPresione Enter para continuar...");
    leer_linea(buffer, TAM_BUFFER);
}

void limpiar_pantalla(void)
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
