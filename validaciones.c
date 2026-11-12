#include <stdio.h>                       // Biblioteca estándar de entrada/salida (printf, fgets, getchar).
#include <stdlib.h>                      // Biblioteca estándar para conversiones y control (exit, strtol, strtod, system).
#include <string.h>                      // Biblioteca para manipulación de cadenas (strchr, strspn).
#include <ctype.h>                       // Biblioteca para clasificación de caracteres (isspace).
#include <math.h>                        // Biblioteca matemática (isfinite).
#include "validaciones.h"                // Incluye el archivo de cabecera propio con las declaraciones.

#define TAM_BUFFER 100                   // Define el tamaño máximo del búfer  (100 caracteres).
#define ANCHO 62                         // Define el ancho fijo predeterminado para las líneas de la interfaz.

/* Lee una linea con fgets. Si la linea es mas larga que el buffer, descarta
   el resto. Si se termina la entrada (EOF) avisa y cierra el programa, asi no hay bucle infinito. */
static void leer_linea(char *buffer, int tamano) // Función estática auxiliar para leer líneas de forma segura.
{
    int c;                               // Variable para almacenar caracteres cuando limpio el búfer.

    if (fgets(buffer, tamano, stdin) == NULL) { // Intenta leer una línea desde la entrada estándar; si falla (EOF)
        printf("\n");                    // Imprime un salto de línea para prolijidad.
        mostrar_aviso("Fin de la entrada. Se cierra el programa."); // Muestra aviso de cierre.
        exit(0);                         // Termina la ejecución del programa de forma segura.
    }
    if (strchr(buffer, '\n') == NULL) {  // Si el salto de línea no está en el búfer (la entrada era más larga)
        while ((c = getchar()) != '\n' && c != EOF) { // bucle para descartar  caracteres sobrantes
            /* descarta el resto de la linea */ // hasta el final de línea o fin de archivo.
        }
    }
}

/* Devuelve 1 si el texto tiene solo espacios (o esta vacio) */
static int es_vacia(const char *texto)   // Función auxiliar para comprobar si una cadena está vacía o solo contiene espacios.
{
    int i;                               // Variable de iteración para el bucle.

    for (i = 0; texto[i] != '\0'; i++) { // Recorre cada carácter del texto hasta el carácter nulo '\0'.
        if (!isspace((unsigned char)texto[i])) { // Si encuentra un carácter que NO sea un espacio en blanco...
            return 0;                    // retorna 0 (no está vacía).
        }
    }
    return 1;                            // Si recorrió todo sin hallar caracteres válidos, retorna 1 (está vacía).
}

/* Reemplaza la coma por punto para trabajar con decimales */
static void coma_a_punto(char *texto)    // Función auxiliar pública  para pasar de comas decimales a puntos.
{
    int i;                               // Variable de iteración.

    for (i = 0; texto[i] != '\0'; i++) { // Recorre toda la cadena de texto.
        if (texto[i] == ',') {           // Si encuentra una coma...
            texto[i] = '.';              // la reemplaza por un punto (formato estándar de conversión).
        }
    }
}

/* Lee un entero entre min y max. Repitiendo hasta que sea valido. */
int leer_entero(const char *mensaje, int min, int max) // Función pública para ingresar un número entero validado en un rango.
{
    char buffer[TAM_BUFFER];             // Arreglo para almacenar la entrada como texto.
    char texto_error[100];               // Arreglo para armar dinámicamente mensajes de error.
    char *fin;                           // Puntero auxiliar para verificar residuos en la conversión numérica.
    long numero;                         // Variable donde se almacena el número convertido (tipo long).

    while (1) {                          // Bucle infinito que se repetirá hasta obtener un valor válido.
        printf("%s", mensaje);           // Muestra el mensaje o instrucción solicitada al usuario.
        leer_linea(buffer, TAM_BUFFER);  // Lee de forma segura la línea ingresada en el búfer.

        if (es_vacia(buffer)) {          // Valida si el usuario no ingresó nada o solo presionó Enter.
            mostrar_error("No ingreso ningun valor."); // Muestra mensaje de error correspondiente.
            continue;                    // Salta a la siguiente iteración del bucle para pedir el dato de nuevo.
        }
        numero = strtol(buffer, &fin, 10); // Convierte el texto del búfer a número entero largo en base 10.
        /* despues del numero solo pueden quedar espacios */
        if (!es_vacia(fin)) {            // Si después del número quedaron caracteres no numéricos (ej. "3abc")...
            mostrar_error("Ingrese un numero entero valido (ejemplo: 3)."); // ...muestra error de formato.
            continue;                    // Vuelve a pedir el valor.
        }
        if (numero < min || numero > max) { // Valida si el número está fuera de los límites permitidos.
            sprintf(texto_error, "El valor debe estar entre %d y %d.", min, max); // Crea el mensaje con los límites exactos.
            mostrar_error(texto_error);  // Muestra el error de rango.
            continue;                    // Vuelve a pedir el valor.
        }
        return (int)numero;              // Si pasó todas las validaciones, convierte a int y retorna el número seguro.
    }
}

/* Lee un numero real entre min y max. Acepta coma o punto decimal. */
double leer_double(const char *mensaje, double min, double max) // Función pública para leer números decimales con rangos.
{
    char buffer[TAM_BUFFER];             // Búfer local para capturar la entrada en texto.
    char texto_error[100];               // Búfer local para armar mensajes de error de rango.
    char *fin;                           // Puntero auxiliar para detectar caracteres inválidos tras la conversión.
    double numero;                       // Variable donde se almacena el número decimal convertido.

    while (1) {                          // Bucle de validación continua.
        printf("%s", mensaje);           // Imprime la directiva o mensaje en pantalla.
        leer_linea(buffer, TAM_BUFFER);  // Captura la línea de texto de manera segura.

        if (es_vacia(buffer)) {          // Comprueba si la entrada está vacía.
            mostrar_error("No ingreso ningun valor."); // Notifica el error.
            continue;                    // Repite la iteración.
        }
        coma_a_punto(buffer);            // Normaliza comas a puntos para aceptar formato latinoamericano.
        numero = strtod(buffer, &fin);   // Convierte la cadena de texto a un número de tipo double.
        /* strtod acepta "nan" e "inf", por eso se pregunta isfinite */
        if (!es_vacia(fin) || !isfinite(numero)) { // Si sobraron caracteres inválidos o el resultado no es un número finito...
            mostrar_error("Ingrese un numero valido (ejemplo: 192 o 192.5)."); // ...muestra error de formato.
            continue;                    // Repite la iteración.
        }
        if (numero < min || numero > max) { // Valida si el número real está fuera del rango permitido.
            sprintf(texto_error, "El valor debe estar entre %g y %g.", min, max); // Construye el mensaje con los límites permitidos.
            mostrar_error(texto_error);  // Imprime el error de rango.
            continue;                    // Repite la iteración.
        }
        return numero;                   // Retorna el número decimal validado.
    }
}

/* Pregunta s/n. Devuelve 1 si es 's' y 0 si es 'n'. */
int leer_si_no(const char *mensaje)      // Función para capturar respuestas afirmativas o negativas de forma robusta.
{
    char buffer[TAM_BUFFER];             // Búfer para capturar la respuesta del usuario.
    char letra;                          // Variable para almacenar el carácter de respuesta relevante.

    while (1) {                          // Bucle infinito hasta obtener una respuesta correcta ('s' o 'n').
        printf("%s", mensaje);           // Muestra la pregunta en pantalla.
        leer_linea(buffer, TAM_BUFFER);  // Lee la entrada de manera segura.

        if (es_vacia(buffer)) {          // Valida si la entrada está vacía.
            mostrar_error("Responda con s o n."); // Muestra error.
            continue;                    // Vuelve a pedir.
        }
        /* se toma el primer caracter que no es espacio */
        letra = buffer[strspn(buffer, " \t")]; // Salta espacios/tabulaciones iniciales y toma el primer carácter válido.
        if (!es_vacia(buffer + strspn(buffer, " \t") + 1)) { // Si después de ese carácter hay más texto sobrante...
            mostrar_error("Responda con s o n."); // ...considera que no es una respuesta limpia y da error.
            continue;                    // Vuelve a pedir.
        }
        if (letra == 's' || letra == 'S') { // Si el usuario ingresó 's' o 'S'...
            return 1;                    // ...retorna 1 (verdadero / sí).
        }
        if (letra == 'n' || letra == 'N') { // Si el usuario ingresó 'n' o 'N'...
            return 0;                    // ...retorna 0 (falso / no).
        }
        mostrar_error("Responda con s o n."); // Si ingresó cualquier otra letra, muestra error.
    }
}

/* Imprime una linea de caracteres repetidos del ancho de la pantalla */
static void imprimir_repetido(char caracter) // Función auxiliar para imprimir un caracter repetido hasta completar `ANCHO`.
{
    int i;                               // Variable contadora para el bucle.

    for (i = 0; i < ANCHO; i++) {        // Bucle que se repite tantas veces como indique la macro `ANCHO` (62).
        putchar(caracter);               // Imprime en consola el carácter especificado en cada iteración.
    }
    putchar('\n');                       // Imprime un salto de línea al finalizar la secuencia.
}

/* Titulo entre dos lineas */
void mostrar_titulo(const char *texto)   // Función para mostrar un título decorado con líneas divisorias arriba y abajo.
{
    mostrar_linea();                     // Imprime la línea divisoria superior.
    printf("  %s\n", texto);             // Imprime el texto del título con un pequeño margen izquierdo.
    mostrar_linea();                     // Imprime la línea divisoria inferior.
}

/* Linea separadora de 62 guiones */
void mostrar_linea(void)                 // Función pública para mostrar una línea simple de guiones.
{
    imprimir_repetido('-');              // Llama a la función auxiliar pasando el carácter guion '-'.
}

/* Linea separadora de 62 signos igual (para el encabezado del menu) */
void mostrar_linea_doble(void)           // Función  para mostrar una línea doble para menús.
{
    imprimir_repetido('=');              // Llama a la función auxiliar pasando el carácter igual '='.
}

void mostrar_error(const char *texto)    // Función para imprimir un mensaje formateado como error.
{
    printf("[ERROR] %s\n", texto);       // Imprime la `[ERROR]` seguida del texto recibido.
}

void mostrar_aviso(const char *texto)    // Función para imprimir un mensaje formateado como aviso.
{
    printf("[AVISO] %s\n", texto);       // Imprime `[AVISO]` seguida del texto recibido.
}

/* Muestra un dato alineado: etiqueta, valor y unidad */
void mostrar_dato(const char *etiqueta, double valor, const char *unidad) // Muestra una línea organizada con formato tabular.
{
    printf("  %-30s %14.2f %s\n", etiqueta, valor, unidad); // Imprime etiqueta alineada a la izq (30 espacios), valor decimal (14 espacios, 2 decimales) y unidad.
}

void pausar(void)                        // Función para pausar la ejecución del programa.
{
    char buffer[TAM_BUFFER];             // Búfer temporal local.

    printf("\nPresione Enter para continuar..."); // Muestra el mensaje de pausa al usuario.
    leer_linea(buffer, TAM_BUFFER);      // Espera a que el usuario presione Enter de forma segura.
}

void limpiar_pantalla(void)              // Función independiente del sistema operativo para limpiar la consola.
{
#ifdef _WIN32                            // Directiva de compilación: si el sistema operativo es Windows.
    system("cls");                       // ejecuta el comando "cls" de la consola de Windows.
#else                                    // De lo contrario (sistemas Unix/Linux/macOS).
    system("clear");                     // ejecuta el comando "clear" de la terminal.
#endif                                   // Fin de la directiva condicional.
}