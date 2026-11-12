#ifndef VALIDACIONES_H
#define VALIDACIONES_H

/* Ingreso seguro de datos */
int leer_entero(const char *mensaje, int min, int max);
double leer_double(const char *mensaje, double min, double max);
int leer_si_no(const char *mensaje);   /* devuelve 1 si 's', 0 si 'n' */

/* Interfaz de pantalla */
void mostrar_titulo(const char *texto);
void mostrar_linea(void);
void mostrar_linea_doble(void);
void mostrar_error(const char *texto);   /* imprime "[ERROR] ..." */
void mostrar_aviso(const char *texto);   /* imprime "[AVISO] ..." */
void mostrar_dato(const char *etiqueta, double valor, const char *unidad);
void pausar(void);                       /* "Presione Enter para continuar..." */
void limpiar_pantalla(void);

#endif
