x|#include <stdio.h>    //Incluye la biblioteca estándar de entrada y salida.
#include <math.h>   //Incluye funciones matemáticas.
#include <string.h>     //Incluye funciones para trabajar con cadenas de caracteres.
#include "artemis.h"    //Incluye nuestro archivo de cabecera.
#include "validaciones.h"   //Incluye otro archivo de cabecera que contiene las declaraciones relacionadas con las validaciones y funciones auxiliares de pantalla.

/* Nombres de los 4 analisis, para el reporte */   //Esta parte crea un arreglo que almacena los nombres de los cuatro análisis.
static const char *nombres_analisis[TOTAL_ANALISIS] = {
    "Gravedad como fuerza centripeta y velocidad orbital",
    "Periodo orbital",
    "Aceleracion centripeta de los astronautas",
    "Pulso del propulsor lateral"
};

/* ---------------- Calculos ---------------- */

/* Calcula velocidad, periodo y aceleracion centripeta de una orbita circular.
   Fg = Fc  ->  G*MT*m/r^2 = m*v^2/r  ->  v = raiz(G*MT/r) */
void calcular_orbita(Orbita *o, double altitud)  //void: no devuelve un resultado mediante return. Orbita *o: recibe un puntero a una estructura Orbita. double altitud: recibe la altitud ingresada o establecida por el programa.
{
    o->altitud = altitud;       //guarda la altitud recibida dentro del campo altitud de ka estructura 
    o->radio = RADIO_TIERRA + altitud;      //calcula el radio orbital
    o->velocidad = sqrt(G_GRAV * MASA_TIERRA / o->radio);       //calcula la velocidad orbital
    /* T = longitud de la circunferencia / velocidad */ 
    o->periodo = 2.0 * PI * o->radio / o->velocidad;    //calcula el periodo orbital, es decer, cuanto tarda la nave en completar una vuelta 
    /* a_c = v^2 / r, igual a G*MT/r^2 */
    o->acel_centripeta = o->velocidad * o->velocidad / o->radio;    //calcula la aceleracion centripeta 
}

/* Calcula el efecto de un pulso lateral (perpendicular a la velocidad).
   Como parte del reposo lateral, dv = a*dt y d = 1/2*a*dt^2. */
void calcular_pulso(const Orbita *o, Pulso *p, double a, double dt)     //esta funcion calcula que sucede cuando la nave recibe un impulso lateral
{
    p->aceleracion = a;    //guarda la aceleracion
    p->duracion = dt;      //guarda cuanto dura el pulso
    p->dv_lateral = a * dt;     //calcula cuanto cambia la velocidad lateral 
    p->desviacion = 0.5 * a * dt * dt;      //calcula el desplezamiento lateral producido durante el impulso 
    /* la velocidad lateral es perpendicular a v, se suman como catetos */
    p->velocidad_final = sqrt(o->velocidad * o->velocidad       //calculka la velocidad resultante 
                              + p->dv_lateral * p->dv_lateral);
    p->angulo_grados = atan(p->dv_lateral / o->velocidad) * 180.0 / PI;     //calcula el angulo de desviacion 
    p->excentricidad = p->dv_lateral / o->velocidad;        //calcula la excentricidad utilizada por el modelo del programa
    p->var_altitud = o->radio * p->excentricidad;       //calcula la variacion estimada de altitud multiplicando el radio orbital por la excentricidad
}

/* ---------------- Estado de la mision ---------------- */

void iniciar_mision(Mision *m)      //recibe un puntero a la estructuda Mision. Su objetivo es dejar la mision preparada para comenzar 
{
    int i;      //declara una variable entera que utilizaremos como contador del ciclo for 

    m->cantidad = 0;        //inicializa la cantidad de consultas realizadas en cero 
    for (i = 0; i < TOTAL_ANALISIS; i++) {      //es un ciclo que se repite cuantro veces
        m->completados[i] = 0;      //en cada vuelta del ciclo establece que el analisis correspondiente todavia esta pendiente 
    }
}

/* Guarda una consulta en el historial y marca el analisis como hecho */
void registrar_consulta(Mision *m, int analisis, double altitud, const char *resumen)
{
    Registro *r;        //declara un puntero a una estrutura Registro. Este puntero nos permitira acceder al registro donde vamos a guardar la consulta  

    m->completados[analisis - 1] = 1;       //marca el analisis como realizado. Se resta -1 porq el usuario identifica los analisis con numeros del 1 al 4, pero los arreglos comienzan en la posicion 0.
    if (m->cantidad >= MAX_REGISTROS) {     //comprueba si el historial ya alcanzo su capacidad maxima 
        mostrar_aviso("Historial lleno: la consulta no se guarda en el reporte.");      //si el historial esta lleno, muestra un mensaje de advertencia 
        return;     //finaliza la funcion inmediatamente 
    }
    r = &m->historial[m->cantidad];     //hacemos que el pountero r apunte a la proxima posicion libre del historial
    r->analisis = analisis;     //guarda el numero del analisis en el registro 
    r->altitud = altitud;       //guarad la altitud utilizada 
    strncpy(r->resumen, resumen, sizeof(r->resumen) - 1);   //copia el texto recibido en el campo resumen 
    r->resumen[sizeof(r->resumen) - 1] = '\0';      //nos aseguramos de que el ultimo lugar del arreglo contenga el caracter nulo \0
    m->cantidad++;      //incrementa la cantidad de consultas almacenadas 
}

int contar_completados(const Mision *m)     //esta funcion cuneta cuantos analisis se completaron
{
    int i;      //declara el contador del ciclo
    int total = 0;     //inicializa un acumulador en cero 

    for (i = 0; i < TOTAL_ANALISIS; i++) {      //recorre las cuatro posiciones del arreglo completados 
        total += m->completados[i];     //suma el contenido de cada posicion al acumulador 
    }
    return total;       //devuelve el resultado 
}

/* ---------------- Auxiliares de pantalla ---------------- */

/* Pide la altitud en km y la devuelve en metros */
static double pedir_altitud(void)       
{
    double km;      //declara una variable para guardar la altitud ingresada en kilometros 

    km = leer_double("Altitud de la orbita en km (160 a 2000, ejemplo 192): ",     
                     ALTITUD_MIN / 1000.0, ALTITUD_MAX / 1000.0);       //llama a leer_double(), que solicita un numero decimal y valida que este dentro de los limites establecidos 
    return km * 1000.0;     //convierte los kilometros a metros y devuelve el resultado 
}

/* Imprime un paso de la demostracion: titulo y formula */
static void mostrar_paso(const char *titulo, const char *formula)
{
    printf("\n%s\n", titulo);       //muestra el titulo
    printf("    %s\n", formula);      //muestra la formula, dejando cuatro espacios al principio para darle formato visual 
}

/* ---------------- Opcion 1: demostracion ---------------- */

/* Imprime los datos del enunciado (paso 1) */
static void paso_datos(double radio)    //es una funcion que recibe el radio arbital 
{
    printf("\nPaso 1: Datos\n");        //muestra el titulo del primer paso 
    printf("    MT = %.3e kg\n", MASA_TIERRA);      //muestra la masa de la tierra 
    printf("    G  = %.3e N*m^2/kg^2\n", G_GRAV);   //muestra la constante de gravitacion universal 
    printf("    RT = %.1f m\n", RADIO_TIERRA);      //muestra el radio terrestre 
    printf("    h  = %.1f m\n", ALTITUD_CONSIGNA);  //muestra la altitud establecida en el enunciado 
    printf("    r  = RT + h = %.1f m\n", radio);    //muestra el radio orbital que recibimos como parametro 
    printf("    m  = %.1f kg\n", MASA_ORION);       //muestra la masa de la nave orion
}

/* Pasos 2 a 5: fuerza gravitatoria, condicion del MCU, igualacion y despeje */
static void pasos_teoria(double radio, double fg)     //esta funcion muestra los pasos teoricos de la demostracion
{
    mostrar_paso("Paso 2: Fuerza gravitatoria", "Fg = G*MT*m / r^2");   //utiliza mostrar_paso() para mostrar el paso 2 y la fomrula de la feurza gravitatoria 
    printf("    Fg = %.3e * %.3e * %.1f / (%.1f)^2\n",      //muestra la formula reemplazando las variables por los valores numericos 
           G_GRAV, MASA_TIERRA, MASA_ORION, radio);
    printf("    Fg = %.2f N\n", fg);        //muestra el resultado de la fuerza gravitatoria, que ya fue calculado y recibido mediante fg.

    mostrar_paso("Paso 3: Condicion del MCU",       //explican que para mantener una trayectoria circular necesitamos una fuerza centripeta.
                 "Para describir una circunferencia hace falta Fc = m*v^2 / r");

    mostrar_paso("Paso 4: Igualacion",      //como el modelo considera que la gravedad es la unica fuerza que actua
                 "Si la unica fuerza es la gravedad: Fg = Fc");
    printf("    G*MT*m / r^2 = m*v^2 / r\n");   //muestra la igualdad desarrollada 

    mostrar_paso("Paso 5: Despeje",     //se explica que se simplifica la masa de la nave y un factor del radio 
                 "Se simplifica m (no influye) y un factor r:");
    printf("    v^2 = G*MT / r   ->   v = raiz(G*MT / r)\n");   //muestra el resultado del despeje
}

/* Pasos 6 y 7: calculo de v y verificacion Fg = Fc */
static void pasos_calculo(const Orbita *o, double fg)   //esta funcion calcula y muestra la velocidad orbital y despues verifica si la feurza gravitatoria coincide con la centripeta
{
    double fc;    //declara una variable para almacenar la feurza centripeta 
    double diferencia;    //declara una variable para almacenar la diferencia relativa entre ambas fuerzas

    printf("\nPaso 6: Calculo de la velocidad orbital\n");      //muestra el titulo del paso 6
    printf("    v = raiz(%.3e * %.3e / %.1f)\n", G_GRAV, MASA_TIERRA, o->radio);    //muestra la formula de la velocidad orbital con los valores numericos 
    printf("    v = %.2f m/s\n", o->velocidad);     //muestra la velocidad orbital en metros por segundo
    printf("    v = %.4f km/s\n", o->velocidad / 1000.0);   //convierte la velocidad a kilometros por segundo dividiendo por 1000
    printf("    v = %.2f km/h\n", o->velocidad * 3.6);      //convierte la velocidad a kilometros por hora multiplicando por 3,6

    /* se compara con tolerancia, nunca con == entre doubles */
    fc = MASA_ORION * o->velocidad * o->velocidad / o->radio;   //calcula la fuerza centripeta 
    diferencia = fabs(fg - fc) / fg;    //calcula la diferencia relativa entre la feurza gravitatoria y la centripeta
    printf("\nPaso 7: Verificacion\n");     //muestra el titulo del paso 7
    printf("    Fc = m*v^2 / r = %.2f N\n", fc);     //muestran los valores de las dos fuerzas para poder compararlas. linea 156 y 157
    printf("    Fg             = %.2f N\n", fg);        
    printf("    Diferencia relativa = %.3e\n", diferencia);     //muestra la diferencia relativa en notacion cientifica 
    if (diferencia < 1e-9) {     //comprueba si la diferencia es menos que 0,000000001
        printf("    Se verifica: Fg = Fc\n");       //si la diferencia es suficientemente pequeña, muestra que se verifica la igualdad. linea 160 y 161
    } else {
        mostrar_aviso("Fg y Fc no coinciden.");     //en caso contrario, muestra un aviso indicando que las feurzas no coinciden. linea 162 y 163
    }
}

/* Opcion 1: demostracion analitica paso a paso con los datos del enunciado */
void analisis_demostracion(Mision *m)     //Recibe la mision para poder registrar la consulta al terminar 
{
    Orbita o;       //almacenara los resultados orbitales 
    double fg;      //almacenara la fuerza gravitatoria 
    char resumen[80];       //almacenara un resumen del analisis

    calcular_orbita(&o, ALTITUD_CONSIGNA);      //llama a la funcion que ya estudiamos 
    fg = G_GRAV * MASA_TIERRA * MASA_ORION / (o.radio * o.radio);       //calcula la fuerza gravitatoria 

    mostrar_titulo("DEMOSTRACION: GRAVEDAD COMO FUERZA CENTRIPETA");    //muestra el titulo de la demostracion 
    paso_datos(o.radio);    //llama a paso_datos() para mostrar los datos iniciales 
    pasos_teoria(o.radio, fg);      //llama a pasos_teoria() para mostrar el desarrollo teorico 
    pasos_calculo(&o, fg);      //llama a pasos_calculo() para calcular y verificar las fuerzas 
    printf("\nPaso 8: Conclusion\n");     //muestran la conclusion de la demostracion. lineas 180, 181 y 182
    printf("    La gravedad cumple el rol de fuerza centripeta. La velocidad\n");   
    printf("    orbital no depende de la masa de la nave, solo de r.\n");
    mostrar_linea();    //muestra una linea divisoria en pantalla 

    sprintf(resumen, "v = %.2f m/s, Fg = Fc = %.1f N", o.velocidad, fg);     //guarda un resumen con la velocidad orbital y la fuerza gravitatoria 
    registrar_consulta(m, 1, ALTITUD_CONSIGNA, resumen);    //registra la consulta en el historial de la mision 
}

/* ---------------- Opcion 2: periodo ---------------- */

void analisis_periodo(Mision *m)    //funcion que recibe la mision para registrar la consulta 
{
    Orbita o;     //delcara una estructura para guardar los calculo orbitales 
    char resumen[80];       //delcara una cadena para guardar el resumen del resultado 

    calcular_orbita(&o, pedir_altitud());   //primero se ejecuta pedir_altitud(), que solicita al usuario y devuelve el resultado en metros. Despues se utiliza ese valor como segundo parametro de calcular_orbita(). La funcoin completa la estrutura "o" con los resultados.

    mostrar_titulo("PERIODO ORBITAL");      //muestra el titulo del analisis
    mostrar_dato("Altitud ingresada", o.altitud / 1000.0, "km");    //muestra la altitud ingresada, pero la divide por 1000 para expresarla en kilometros 
    mostrar_dato("Radio orbital r", o.radio / 1000.0, "km");    //muestra el radio orbital en kilometros 
    mostrar_dato("Velocidad orbital v", o.velocidad, "m/s");    //muestra la velocidad orbital en metros por segundos
    mostrar_dato("Periodo T", o.periodo, "s");    //muestra el periodo orbital en segundos 
    mostrar_dato("Periodo T", o.periodo / 60.0, "min");     //muestra nuevamente el periodo, pero convertido a minutos 
    mostrar_linea();      //muestra una linea divisoria 

    sprintf(resumen, "T = %.2f s (%.2f min)", o.periodo, o.periodo / 60.0);     //genera un resumen del periodo en segundos y minutos utilizando sprintf()
    registrar_consulta(m, 2, o.altitud, resumen);       //regustra la consulta inidcando que corresponde al analisis numero 2
}

/* ---------------- Opcion 3: aceleracion centripeta ---------------- */

void analisis_aceleracion(Mision *m)      //recibe la mision para registrar el analisis 
{
    Orbita o;   //delcara la estructura Orbita y la cadena resumen. Linea 214 y 215
    char resumen[80];

    calcular_orbita(&o, pedir_altitud());     //solicita la altitud y calcula los datos orbitales mediante calcular_orbita()

    mostrar_titulo("ACELERACION CENTRIPETA");      //muestra el titulo 
    mostrar_dato("Altitud ingresada", o.altitud / 1000.0, "km");    //muestra la altitud en kilometros 
    mostrar_dato("Velocidad orbital v", o.velocidad, "m/s");       //muestra la velocidad orbital 
    mostrar_dato("Aceleracion centripeta", o.acel_centripeta, "m/s^2");     //muestra la aceleracion centripeta calculada 
    mostrar_dato("En multiplos de g", o.acel_centripeta / G_TERRESTRE, "g");    //expresa la aceleracion como multiple de la gravedad terrestre 
    mostrar_linea();    //muestra una linea divisoria 
    printf("  Los astronautas estan en caida libre continua: por eso\n");       //muestran una explicacion sobre la sensacion de ingravidez de los astronautas. Linea 225, 226 y 227
    printf("  sienten ingravidez aunque la gravedad a esa altura sea\n");       
    printf("  casi la de la superficie.\n");
    mostrar_linea();    //muestra otra linea divisoria 

    sprintf(resumen, "a_c = %.4f m/s^2 (%.3f g)", o.acel_centripeta,       //generan el resumen del analisis con la aceleracion en m/s² y en multiplos de g. Linea 230 y 231
            o.acel_centripeta / G_TERRESTRE);
    registrar_consulta(m, 3, o.altitud, resumen);      //registra la consulta como analisis numero 3
}

/* ---------------- Opcion 4: pulso lateral ---------------- */

/* Describe la trayectoria resultante del pulso */
static void describir_trayectoria(const Pulso *p)
{
    printf("  Trayectoria resultante:\n");     //muestra el titulo de la trayectoria resultante 
    printf("  - Si el pulso fuera radial (dentro del plano orbital), la\n");    //muestran que sucederia si el impulso fuera radial, es decir, dentro del plano orbital. Linea 241, 242, 243 y 244
    printf("    orbita pasa de circulo a elipse muy poco excentrica. La\n");
    printf("    nave oscilaria alrededor de la altitud original en +-%.2f km.\n",
           p->var_altitud / 1000.0);
    printf("  - Si el pulso fuera normal al plano orbital, la orbita sigue\n");   //explica que sucederia si el impulso fuera perpendicular al plano orbital. Linea 245, 246 y 247
    printf("    siendo circular pero su plano se inclina %.4f grados.\n",
           p->angulo_grados);
    printf("  - En ambos casos el efecto es pequeno frente a la velocidad\n");     //finalizan la explicacion indicando que el efecto del impulso es pequeño comparado con la velocidad orbital. Linea 248 y 249
    printf("    orbital.\n");
}

void analisis_pulso(Mision *m)     //Es la funcion principal de la opcion 4. Recibe la mision para guardar la consulta 
{
    Orbita o;   //almacena los datos de la orbita 
    Pulso p;    //almacena los resultados del impulso 
    double a;   //guarda la aceleracion ingresada
    double dt;  //guarda la duracion del impulso 
    char resumen[80];   //almacena el resumen 

    calcular_orbita(&o, pedir_altitud());   //primero solicita la altitud y calcula todos los datos orbitales 
    a = leer_double("Aceleracion del pulso en m/s^2 (0.01 a 20, ejemplo 0.5): ",    //solicta al usuario la aceleracion del impulso. Linea 261 y 262
                    ACEL_MIN, ACEL_MAX);
    dt = leer_double("Duracion del pulso en s (0.1 a 600, ejemplo 10): ",   //solicita la duracion del impulso. Linea 263 y 264
                     DT_MIN, DT_MAX);
    calcular_pulso(&o, &p, a, dt);      //llama a la funcion que estudiamos anteoriormente

    mostrar_titulo("PULSO DEL PROPULSOR LATERAL");     //muestra el titulo del analisis 
    mostrar_dato("Velocidad orbital v", o.velocidad, "m/s");    //muestra la velocidad orbital
    mostrar_dato("Velocidad lateral dv = a*dt", p.dv_lateral, "m/s");   //muestra la variacion de velocidad lateral 
    mostrar_dato("Desviacion lateral d", p.desviacion, "m");    //muestra el desplazamiento lateral 
    mostrar_dato("Velocidad final", p.velocidad_final, "m/s");  //muestra la velocidad final 
    mostrar_dato("Angulo de desvio", p.angulo_grados, "grados");    //muestra el angulo de desviacion 
    printf("  %-30s %14.3e\n", "Excentricidad e", p.excentricidad);     //muestra la excentricidad
    mostrar_dato("Variacion de altitud +-r*e", p.var_altitud / 1000.0, "km");   //muestra la variacion estimada de altitud en kilometros 
    mostrar_linea();    //muestra una linea divisoria 
    describir_trayectoria(&p);      //llama a la funcion auxiliar que explica que sucede con la trayectoria 
    mostrar_linea();    //muestra otra linea divisoria 

    /* si el pulso dura mas del 5% del periodo ya no es corto */
    if (dt > 0.05 * o.periodo) {    //esta condicion verifica si la duracion del impulso supera el 5% del perioso orbital. Linea 280, 281 y 282
        mostrar_aviso("El pulso supera el 5% del periodo: la aproximacion pierde validez.");
    }

    sprintf(resumen, "d = %.2f m, dv = %.2f m/s", p.desviacion, p.dv_lateral);  //genera un resumen del desplazamiento lateral y la variacion de velocidad 
    registrar_consulta(m, 4, o.altitud, resumen);       //registra la consulta como analisis numero 4 
}

/* ---------------- Opcion 5: reporte final ---------------- */

void mostrar_reporte(const Mision *m)   //declara la funcion que muestra el reporte 
{
    int i;  //delcara el contador que utilizaremos en lso ciclos 
    int hechos = contar_completados(m);     //llama a la funcion contar_completados()

    mostrar_titulo("REPORTE FINAL DE LA MISION");      //muestra el titulo del reporte final
    printf("  Consultas realizadas: %d\n\n", m->cantidad);      //muestra cuantas consultas se almacenaron en el historial 
    for (i = 0; i < m->cantidad; i++) {     //este ciclo recorre todos los registros almacenados. Lineas 297, 298, 299 y 300 
        printf("  %2d. Analisis %d | %7.2f km | %s\n", i + 1,
               m->historial[i].analisis, m->historial[i].altitud / 1000.0,
               m->historial[i].resumen);
    }
    mostrar_linea();    //muestra una linea divisoria 
    for (i = 0; i < TOTAL_ANALISIS; i++) {      //este ciclo reocrre los cuatros analisis. Lineas 303, 304, 305 y 306
        printf("  %d. %-50s %s\n", i + 1, nombres_analisis[i],
               m->completados[i] ? "[HECHO]" : "[PENDIENTE]");
    }
    mostrar_linea();       //muestra una linea divisoria 
    printf("  Avance: %d%%\n", hechos * 100 / TOTAL_ANALISIS);  //calcula el porcentaje de analisis completados 
    if (hechos == TOTAL_ANALISIS) {         //compara la cnatidad de analisis completados con la cantidad total de analisis. Lineas 309, 310, 311, 312 y 313
        printf("  MISION COMPLETA: orbita verificada\n");
    } else {
        printf("  MISION INCOMPLETA: faltan analisis\n");
    }
    mostrar_linea();    //muestra una linea divisoria y finaliza la funcion
}
