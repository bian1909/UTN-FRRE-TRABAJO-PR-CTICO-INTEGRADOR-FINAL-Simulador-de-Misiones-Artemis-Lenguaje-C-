# Define el compilador de C a utilizar (en este caso, GCC)
CC = gcc
# Banderas (flags) de compilación: 
# -Wall y -Wextra muestran advertencias de código.
# -std=c99 obliga a usar el estándar C99. 
# -pedantic asegura un cumplimiento estricto del estándar.
CFLAGS = -Wall -Wextra -std=c99 -pedantic
# Librerías a enlazar en el paso final. "-lm" vincula la librería matemática de C
LIBS = -lm
# Variable que agrupa todos los archivos objeto (.o) necesarios para el programa
OBJETOS = main.o artemis.o validaciones.o
# Objetivo por defecto. Al ejecutar "make" en la terminal, se construirá "artemis"
all: artemis
# Regla para crear el ejecutable "artemis". Depende de los archivos en $(OBJETOS)
artemis: $(OBJETOS)
	# Comando: Llama al compilador con las banderas, nombra la salida "artemis" (-o), 
	# e incluye los objetos y la librería matemática
	$(CC) $(CFLAGS) -o artemis $(OBJETOS) $(LIBS)
# Regla para compilar main.o. Se recompilará si main.c, artemis.h o validaciones.h cambian
main.o: main.c artemis.h validaciones.h
	# La bandera "-c" le dice a GCC que solo compile el código, sin enlazarlo todavía
	$(CC) $(CFLAGS) -c main.c
# Reglas similares para compilar los demás archivos objeto individuales
artemis.o: artemis.c artemis.h validaciones.h
	$(CC) $(CFLAGS) -c artemis.c

validaciones.o: validaciones.c validaciones.h
	$(CC) $(CFLAGS) -c validaciones.c
# Objetivo útil para desarrollo: ejecuta "make all" para asegurar que esté compilado 
# y luego ejecuta el programa creado automáticamente
run: all
	./artemis
# Objetivo de mantenimiento para borrar (rm -f) los archivos generados durante la compilación, 
# manteniendo el proyecto limpio de binarios (.o y ejecutables de Linux/Windows)
clean:
	rm -f $(OBJETOS) artemis artemis.exe
