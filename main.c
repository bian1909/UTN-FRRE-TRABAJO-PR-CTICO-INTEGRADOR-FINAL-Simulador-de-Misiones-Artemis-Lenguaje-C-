#include <stdio.h> // libreria basica
#include "artemis.h" // traigo la cabecera propia del proyecto
#include "validaciones.h" //lo mismo con la cabecera de validaciones, para poderla ocupar

/* funcion del menu principal */
static void mostrar_menu(const Mision *m) // funcion que imprime por pantalla el menu
{
    mostrar_linea_doble(); // para armar el marquito
    printf("      SIMULADOR DE MISIONES ARTEMIS - ORBITA TERRESTRE BAJA\n"); // titulo
    mostrar_linea_doble(); // para armar el marquito
    printf("  Nave Orion | Escenario 2: MCU y Gravitacion\n\n"); // subtitulo, espacio blanco abajo con "\n\n"
    //muestra las opciones que puede seleccionar el usuario, segun su interes
    printf("  1. Demostracion: gravedad como fuerza centripeta y velocidad orbital\n"); // muestra la demostración analitica paso a paso de que la fuerza de atracción gravitatoria actúa como la fuerza centrípeta del sistema + calculo del modulo dela velocidad orbital
    printf("  2. Periodo orbital\n"); // Cuántos segundos y minutos le toma a la nave Orion completar una vuelta completa 
    printf("  3. Aceleracion centripeta de los astronautas\n"); // Calcule el módulo de la aceleración centrípeta 
    printf("  4. Pulso del propulsor lateral\n"); // trayectoria resultante y la desviación lateral instantánea.
    printf("  5. Reporte final de la mision\n"); // reporte final segun las consultas
    printf("  0. Salir\n");
    mostrar_linea(); // estetico
    printf("  Analisis completados: %d de %d\n", contar_completados(m), TOTAL_ANALISIS);
    //imprime cuántos análisis están hechos
    mostrar_linea();
}

int main(void) // funcion principal, donde empieza a ejecutarse el programa
{
    Mision mision; // declara la variable que guarda todo el estado de la partida:
                   // historial, cantidad de consultas y análisis completados [0101] = 2 pendientes
    int opcion; // en donde se guarda la opción del usuario

    iniciar_mision(&mision); // inicializa la mision, sin consultas, [0000]

    do { //post test, esto se ejecuta al menos una vez
        limpiar_pantalla(); // se limpia la pantalla
        mostrar_menu(&mision); //se muestra el menu, se le pasa la direccion de la var mision
        opcion = leer_entero("Elija una opcion: ", 0, 5); // permite elegir la opcion y el rango de estas, otro valor seria invalido

        switch (opcion) { // segun la opción ingresada, busca el case
        case 1:
            analisis_demostracion(&mision); // demostración analitica, guarda la consulta y marca el analisis uno como hecho
            pausar(); // enter para continuar, espera el input
            break; //sale del switch
        case 2:
            analisis_periodo(&mision); // calcula y muestra el período orbital, y registra la consulta.
            pausar();
            break;
        case 3:
            analisis_aceleracion(&mision); // calcula y muestra la aceleración centrípeta, registra la consulta
            pausar();
            break;
        case 4:
            analisis_pulso(&mision); //pide los datos del pulso lateral, calcula, muestra y registra la consulta
            pausar();
            break;
        case 5:
            mostrar_reporte(&mision); // muestra el reporte final con el historial, el estado de cada análisis y el avance
            pausar();
            break;
        case 0: // para salir
            /* si falta algun analisis se pide confirmacion */
            if (contar_completados(&mision) < TOTAL_ANALISIS) {
            //pregunta si hay menos análisis hechos que el total; si ya están los 4, no entra y opcion sigue valiendo 0.
                if (!leer_si_no("La mision esta incompleta. Seguro que desea salir? (s/n): ")) {
                //pregunta si de verdad quiere salir; leer_si_no devuelve 1 si contesta s y 0 si contesta n
                // el ! invierte ese valor, entonces la condición se cumple cuando contesta no.
                    opcion = -1;   /* se queda en el menu */
                }
            }
            break;
        }
    } while (opcion != 0);
    //cierra el do y repite el ciclo mientras opcion sea distinta de 0. 
    //Con las opciones 1 a 5 y con el -1 vuelve al menú

    printf("\nFin de la mision. Hasta la proxima, controlador!\n");
    return 0;
}
