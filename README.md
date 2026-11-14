# Simulador de Misiones Artemis - Escenario 2 (orbita LEO)

## Integrantes

- Godoy Longoni Bianca Antonella
- Striger Killian
- Charra Joaquin
- Amarilla Jair

Comision C. Materia: Algoritmos y Estructuras de Datos, UTN FRRE

## Escenario

Programa en C, por consola, que resuelve la Situacion-Problema 2: insercion y estabilidad en orbita terrestre baja (LEO), MCU y gravitación:

#### Descrición:

Tras apagar sus motores de ascenso, la nave Orion es inyectada en una órbita terrestre baja (LEO) de estacionamiento, donde realiza un Movimiento Circular Uniforme (MCU) a una altitud constante de h = 192,0 km (192.000,0 m) sobre la superficie terrestre. Considere que la masa de la Tierra es MT = 5,972 × 10²⁴ kg, el radio terrestre medio es RT = 6.371,0 km (6.371.000,0 m), la radio orbital total es r = RT + h = 6.563,0 km (6.563.000,0 m), y la constante de gravitación universal es G = 6,674 × 10⁻¹¹ N·m²/kg². La masa total de la nave Orion en órbita es morion = 33.000,0 kg.

> Ejemplos de preguntas de análisis y cálculo analítico que el programa debería responder, a partir de distintos valores de entrada ingresados por el usuario 

1. Demuestre analíticamente que la fuerza de atracción gravitatoria actúa como la fuerza centrípeta del sistema y calcule el módulo de la velocidad orbital v necesaria para mantener dicha órbita circular.
2. ¿Cuántos segundos y minutos le toma a la nave Orion completar una vuelta completa (período orbital T) alrededor de la Tierra?
3. Calcule el módulo de la aceleración centrípeta que experimentan los astronautas dentro de la cápsula a esa altitud.
4. Si durante la órbita la nave enciende un propulsor auxiliar lateral que genera un pulso de aceleración a = 0,50 m/s² constante y perpendicular a su velocidad orbital durante Δt = 10,0 s, describa la trayectoria resultante y calcule la desviación lateral instantánea.


### Regla del juego

1. El usuario hace de controlador de misión y elige desde un menú qué análisis ejecutar.
2. La opción 1 (demostración) usa los datos del enunciado. Las opciones 2, 3 y 4 piden valores al usuario (altitud, y en la opción 4 también la aceleración y la duración del pulso).
3. Cada consulta queda registrada en un historial de hasta 50 consultas.
4. La misión se da por completada cuando se hicieron los 4 análisis al menos una vez Cualquier análisis se puede repetir con otros valores.
5. Al salir con la misión incompleta, el programa pide confirmación.
6. La opción 5 muestra el reporte final de desempeño: tabla de consultas, estado de cada análisis ([HECHO] o [PENDIENTE]), porcentaje de avance y si la misión está completa.

### Rangos validos de ingreso

| Dato | Rango | Justificacion |
|---|---|---|
| Altitud | 160 a 2000 km | Por debajo de 160 km el rozamiento atmosferico hace decaer la orbita en pocas horas. Por encima de 2000 km ya no es orbita baja. |
| Aceleracion del pulso | 0,01 a 20 m/s^2 | Valores razonables para un propulsor auxiliar. |
| Duracion del pulso | 0,1 a 600 s | Pulsos cortos frente al periodo orbital. |

Se acepta coma o punto como separador decimal.

## Funcionamiento

### organización del codigo

...
/
├── main.c           menú principal y ciclo del programa
├── artemis.h        constantes, structs y prototipos
├── artemis.c        cálculos físicos, análisis, registro y reporte
├── validaciones.h   prototipos de lectura segura y de pantalla
├── validaciones.c   lectura segura de datos y formato de pantalla
├── Makefile
├── .gitignore
└── README.md
...

### Módulos
- main.c: contiene main(), el dibujo del menú y el ciclo principal con switch. No tiene cálculos.
- artemis.h / artemis.c: constantes, structs, fórmulas físicas, los 4 análisis, el registro de consultas y el reporte final.
- validaciones.h / validaciones.c: ingreso seguro de datos (fgets con strtol y strtod) y formato de pantalla (títulos, líneas, mensajes de error y aviso).

### Opciones del menu

1. Demostracion paso a paso con los datos del enunciado: Fg = Fc, v = raiz(G*MT/r) y verificacion numerica.
2. Periodo orbital: T = 2*pi*r / v, en segundos y minutos.
3. Aceleracion centripeta: a = v^2 / r, en m/s^2 y en multiplos de g.
4. Pulso lateral: dv = a*dt, d = 1/2*a*dt^2, velocidad final, angulo de desvio, excentricidad y descripcion de la trayectoria. Avisa si dt supera el 5 % del periodo.
5. Reporte final: tabla de consultas, estado de cada analisis y avance.
0. Salir (pide confirmacion si la mision esta incompleta).

A
### Casos de prueba

| Entrada | Resultado esperado | Resultado obtenido |
|---|---|---|
| Opcion 1 | v = 7792,95 m/s, Fg = Fc = 305362,1 N | Coincide |
| Opcion 2, h = 192 | T = 5291,52 s (88,19 min) | Coincide |
| Opcion 3, h = 192 | a = 9,2534 m/s^2 (0,944 g) | Coincide |
| Opcion 4, h = 192, a = 0,5, dt = 10 | dv = 5 m/s, d = 25 m, e = 6,416e-4 | Coincide |
| Letras ("abc", "12abc", "1.2.3") | [ERROR] y vuelve a preguntar | Coincide |
| Solo Enter | [ERROR] y vuelve a preguntar | Coincide |
| nan / inf | [ERROR] y vuelve a preguntar | Coincide |
| Altitud 50 | [ERROR] fuera de rango (160 a 2000) | Coincide |
| Linea de 500 caracteres | [ERROR] sin afectar la lectura siguiente | Coincide |
| `0,5` como decimal | Se interpreta como 0.5 | Coincide |
| Fin de entrada (Ctrl+Z / Ctrl+D) | Aviso y cierre del programa | Coincide |

## Compilacion y ejecucion

Requisito: compilador gcc (en Windows se puede usar MinGW). Todos los archivos deben estar en la misma carpeta.

Compilar y ejecutar con gcc (funciona igual en Linux, macOS y Windows). El comando va en una sola línea:
> gcc -Wall -Wextra -std=c99 -pedantic -o artemis main.c artemis.c validaciones.c -lm

para ejecutar 
>./artemis          # Linux y macOS
>.\artemis.exe      # Windows (PowerShell)

con el makefile: 
>make          # compila el ejecutable artemis
>make run      # compila y ejecuta
>make clean    # borra los objetos y el ejecutable


## Uso de Inteligencia Artificial

Se utilizo Claude (de Anthropic) como asistencia en el desarrollo del trabajo, asi como en su planificación, y tambien para detalles de este README.
