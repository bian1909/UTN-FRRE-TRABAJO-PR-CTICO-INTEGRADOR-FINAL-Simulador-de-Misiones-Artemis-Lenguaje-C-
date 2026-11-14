#ifndef ARTEMIS_H    
#define ARTEMIS_H  
//proteccion del archivo 
//estas dos lineas evita que el archivo se incluya mas de una vez durante la compilacion 


/* Constantes fisicas (SI) */
#define G_GRAV 6.674e-11          /* constante de gravitacion universal (N*m^2/kg^2) */
#define MASA_TIERRA 5.972e24      /* masa de la Tierra (kg) */
#define RADIO_TIERRA 6371000.0    /* radio medio de la Tierra en metros (m) */
#define MASA_ORION 33000.0        /* masa de la nave en kilogramos(kg), solo en la opcion 1 */
#define ALTITUD_CONSIGNA 192000.0 /* altitud del enunciado (m), solo en la opcion 1 */
#define G_TERRESTRE 9.80          /* para expresar aceleraciones en multiplos de g */ //aceleracion gravitatoria terrestre 
#define PI 3.14159265358979       /* no se usa M_PI porque no existe con -std=c99 */ //numero pi
// se pcupan valores constasntes porque estos valores no deberían cambiar durante la ejecución del programa.


/* Rango de altitud LEO valido (m).
   Por debajo de 160 km el rozamiento atmosferico hace decaer la orbita en
   pocas horas. Por encima de 2000 km ya no se considera orbita baja. */
#define ALTITUD_MIN 160000.0    
#define ALTITUD_MAX 2000000.0  
//Estas dos lineas son constantes que establecen los límites de altitud permitidos para considerar una órbita terrestre baja (LEO).
//sirven para validar que el usuario ingrese una altitud dentro del rango establecido.


/* Rango del pulso del propulsor lateral */
#define ACEL_MIN 0.01   /* m/s^2 */  // aceleracion minima  
#define ACEL_MAX 20.0   /* m/s^2 */  //aceleracion maxima
#define DT_MIN 0.1      /* s */     //duracion minima 
#define DT_MAX 600.0    /* s */     //duracion maxima 
//Estas constantes establecen los valores mínimos y máximos que puede ingresar el usuario para realizar una simulación del pulso del propulsor lateral.
//Esto nos permite trabajar con cantidades definidas sin tener que escribir los números directamente en diferentes partes del código.


//cantidad de registros y analisis 
#define MAX_REGISTROS 50   /* tamano del historial */
#define TOTAL_ANALISIS 4   /* cantidad de analisis de la mision */
//Acá definimos dos constantes relacionadas con el funcionamiento general del programa.
//Esto nos permite trabajar con cantidades definidas sin tener que escribir los números directamente en diferentes partes del código.


/* Resultado de una orbita circular */
typedef struct {
    double altitud;         /* m */     //altitud de la orbita en metros 
    double radio;           /* m, RT + altitud */   //distancia desde el centro terrestre hasta la nave 
    double velocidad;       /* m/s */   //velocidad orbital en m/s
    double periodo;         /* s */     //tiempo que tarda en completar una vuelta 
    double acel_centripeta; /* m/s^2 */     //aceleracion centripeta en m/s²
} Orbita;
// se lo utiliza porque son valores que pueden tener decimales y necesitamos precisión para realizar los cálculos físicos.


/* Resultado del pulso lateral */  //esta estructura almacena los resultados obtenidos al aplicar un pulso lateral a la nave.
typedef struct {
    double aceleracion;     /* m/s^2 */     //aceleracion aplicada al propulsor lateral 
    double duracion;        /* s */     //tiempo durante el cual se aplica el pulso 
    double dv_lateral;      /* m/s: a * dt */      //variacion de la velocidad lateral 
    double desviacion;      /* m: 1/2 * a * dt^2 */     //desplazamiento lateral producido
    double velocidad_final; /* m/s: sqrt(v^2 + dv^2) */     //velocidad resultante despues del pulso 
    double angulo_grados;   /* grados: atan(dv / v) */      //angulo generado por el cambio de velocidad 
    double excentricidad;   /* dv / v */    //valor utilizado para representar la desviacion de la orbitra 
    double var_altitud;     /* m: r * excentricidad */      //variacion estimada de altitud 
} Pulso;


/* Una consulta realizada */
typedef struct {
    int analisis;           /* 1 a 4 */     //indica que analisis se realizo (del 1 a 4)
    double altitud;         /* m */     //almacena la altitud utilizada en esa consulta 
    char resumen[80];       /* texto corto con el resultado principal */    //almacena un texto de hasta 79 caracteres mas el caracter especial \0 que indica el final de la cadena 
} Registro;
//esto permite conservar información de las consultas realizadas para mostrarla posteriormente.


/* Estado de toda la mision */  //Esta estructura representa el estado general de toda la misión.
typedef struct {
    Registro historial[MAX_REGISTROS];  //Es un arreglo de 50 posiciones. Cada posición almacena un elemento de tipo Registro. Es decir, podemos guardar hasta 50 consultas realizadas.
    int cantidad;   //Indica cuántos registros se encuentran actualmente almacenados en el historial.
    int completados[TOTAL_ANALISIS];  /* 0 = pendiente, 1 = hecho */    //Es un arreglo de 4 posiciones que permite saber cuáles de los cuatro análisis ya fueron realizados.
} Mision;


/* Calculos */     //Un prototipo le indica al compilador que existe una función, cómo se llama, qué parámetros recibe y qué tipo de dato devuelve. 
void calcular_orbita(Orbita *o, double altitud);   //void: significa que la función no devuelve ningún valor mediante return. calcular_orbita: nombre de la función. Orbita *o: recibe un puntero a una estructura de tipo Orbita. double altitud: recibe una altitud como parámetro.
void calcular_pulso(const Orbita *o, Pulso *p, double a, double dt);    //const Orbita *o: puntero a una órbita que no debe modificarse mediante ese puntero. Pulso *p: puntero a la estructura donde se guardarán los resultados del pulso. double a: aceleración aplicada. double dt: duración del pulso.


/* Estado de la mision */   
void iniciar_mision(Mision *m);     //Recibe un puntero a la misión y se encarga de inicializarla.
void registrar_consulta(Mision *m, int analisis, double altitud, const char *resumen);      //Permite guardar una consulta en el historial.
int contar_completados(const Mision *m);    //Cuenta cuántos análisis fueron completados.


/* Analisis del menu (opciones 1 a 4) y reporte (opcion 5) */
void analisis_demostracion(Mision *m);  //ejecutar el analisis de demostracion 
void analisis_periodo(Mision *m);   //realizar el analisis del periodo orbital 
void analisis_aceleracion(Mision *m);   //realizar el analisis de aceleracion 
void analisis_pulso(Mision *m);     //realizar el analisis del pulso lateral 
void mostrar_reporte(const Mision *m);      //mostral el reporte de la mision 

#endif
