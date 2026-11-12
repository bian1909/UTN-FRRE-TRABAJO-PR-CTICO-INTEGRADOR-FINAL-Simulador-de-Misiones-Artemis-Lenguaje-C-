#ifndef VALIDACIONES_H                    // Preprocesador: si VALIDACIONES_H no está definido...
#define VALIDACIONES_H                    // se define para evitar inclusiones múltiples del archivo.

/* Ingreso seguro de datos */
int leer_entero(const char *mensaje, int min, int max);       // Declara la función para leer enteros dentro de un rango.
double leer_double(const char *mensaje, double min, double max);// Declara la función para leer decimales dentro de un rango.
int leer_si_no(const char *mensaje);   /* devuelve 1 si es 's' y 0 si es 'n' */ // Declara la función para preguntas de sí/no.

/* Interfaz de pantalla */
void mostrar_titulo(const char *texto);                      // Declara la función para imprimir un título formateado.
void mostrar_linea(void);                                    // Declara la función para mostrar una línea simple de guiones.
void mostrar_linea_doble(void);                              // Declara la función para mostrar una línea doble de iguales.
void mostrar_error(const char *texto);   /* imprime "[ERROR] ..." */ // Declara la función para mostrar mensajes de error.
void mostrar_aviso(const char *texto);   /* imprime "[AVISO] ..." */ // Declara la función para mostrar mensajes de advertencia.
void mostrar_dato(const char *etiqueta, double valor, const char *unidad); // Declara la función para mostrar datos tabulados.
void pausar(void);                       /* "Presione Enter para continuar" */ // Pausa la ejecución hasta presionar Enter.
void limpiar_pantalla(void);                                 // Limpia la consola en base a el sistema operativo.

#endif                                   // Fin de la directiva condicional del preprocesador.