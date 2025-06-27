//Proyecto Final - Programacion 1
//Mapa de Mexico 
//Brisa Martinez Urrutia, Karla Jimena Rodriguez Lara, Hilda Ximena de Luna Jimenez, Ximena Montserrat Ramos Campos



#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip> // Para std::setw y std::left
#include <limits> // Para numeric_limits
#include <cstring> // Requerido para strlen y strcpy

// --- Prototipos de Funciones ---

// Estructuras, Uniones y Enumeraciones
enum class Clima { PREDOM_CALIDO, PREDOM_SECO, PREDOM_TEMPLADO, INDEFINIDO };

union DatoAdicional {
    int rankingTuristico;
    float indiceDesarrollo;
};

struct Estado {
    char nombre[50];
    char capital[50];
    long poblacion;
    double superficie;
    Clima tipoClima;
    DatoAdicional extra;
    char datoHistorico[256];
};

// Funciones del Menú e Interfaz
void mostrarEncabezado(const std::string& titulo);
void mostrarMenuPrincipal();
void procesarOpcion(char opcion, std::vector<Estado>& estados, bool (*matrizAdyacencia)[32]);
void mostrarAyuda();
void despedida();

// Funciones Principales de la Aplicación
void mostrarClimas(const std::vector<Estado>& estados);
void consultarEstado(const std::vector<Estado>& estados);
void mostrarCordilleras();
void mostrarRiosPrincipales();
void mostrarMatrizAdyacencia(const std::vector<Estado>& estados, bool (*matriz)[32]);

// Funciones de Manejo de Datos y Archivos
void inicializarDatos(std::vector<Estado>& estados, bool (*matrizAdyacencia)[32]);
void guardarDatosBinario(const std::vector<Estado>& estados);
bool cargarDatosBinario(std::vector<Estado>& estados);
void guardarReporteEstado(const Estado& estado);

// Funciones Auxiliares
int obtenerEstadoPorNombre(const std::vector<Estado>& estados, const std::string& nombre);
std::string obtenerNombreClima(Clima c);
void limpiarPantalla();
void pausarPantalla();


// --- Función Principal ---

int main() {
// Definición de vectores y matrices
std::vector<Estado> estados;
bool matrizAdyacencia[32][32] = {false}; // Matriz de adyacencia entre estados

// Cargar datos desde archivo binario o inicializarlos
if (!cargarDatosBinario(estados)) {
  std::cout << "No se encontró archivo 'estados.dat'. Inicializando datos por defecto." << std::endl;
  inicializarDatos(estados, matrizAdyacencia);
  guardarDatosBinario(estados); // Guardar para futuros usos
  pausarPantalla();
}else{
	
// Si se cargan los datos, la matriz de adyacencia aún debe inicializarse.
inicializarDatos(estados, matrizAdyacencia);
}
char opcion;

do {
  limpiarPantalla();
  mostrarMenuPrincipal();
  std::cin >> opcion;
  procesarOpcion(opcion, estados, matrizAdyacencia);
  
}while (opcion != '0');

return 0;

}


// --- Implementación de Funciones ---
void mostrarEncabezado(const std::string& titulo)
 {
   std::cout << "====================================================================\n";
   std::cout << std::setw(40 + titulo.length() / 2) << std::right << titulo << std::endl;
   std::cout << "====================================================================\n\n";
}

void mostrarMenuPrincipal() 
{
   mostrarEncabezado("Mapa Interactivo de la Republica Mexicana");
   std::cout << "Seleccione una opcion:\n\n";
   std::cout << "1. Ver Climas Predominantes por Estado\n";
   std::cout << "2. Consultar Datos de un Estado\n";
   std::cout << "3. Mostrar Principales Cordilleras y Relieves\n";
   std::cout << "4. Mostrar Principales Rios\n";
   std::cout << "5. Ver Matriz de Adyacencia de Estados (Fronteras Terrestres)\n";
   std::cout << "9. Ayuda\n";
   std::cout << "0. Salir\n\n";
   std::cout << "Opcion: ";
}

void procesarOpcion(char opcion, std::vector<Estado>& estados, bool (*matrizAdyacencia)[32]) 
{
   limpiarPantalla();
   switch (opcion) 
   {
     case '1': mostrarClimas(estados); break; 
     case '2': consultarEstado(estados); break;
     case '3': mostrarCordilleras(); break;
     case '4': mostrarRiosPrincipales(); break;
     case '5': mostrarMatrizAdyacencia(estados, matrizAdyacencia); break;
     case '9': mostrarAyuda(); break;
     case '0': despedida(); break;
     default: std::cout << "Opcion no valida. Intente de nuevo." << std::endl; break;
}
   if (opcion != '0') 
   {
     pausarPantalla();
    }
}

//funcion que permite dar ayuda al usuario en caso de no saber como se utiliza 
void mostrarAyuda() 
{
   mostrarEncabezado("Ayuda del Sistema");
   std::cout << "Esta aplicacion le permite explorar datos de Mexico.\n";

   std::cout << " - Opcion 1: Muestra una lista de todos los estados y su clima predominante.\n";
   std::cout << " - Opcion 2: Le pide el nombre de un estado para ver su informacion detallada.\n";
   std::cout << " Tambien le permite guardar esta informacion en 'reporte.txt'.\n";
   std::cout << " - Opcion 3: Lista las cordilleras y sistemas montañosos mas importantes.\n";
   std::cout << " - Opcion 4: Lista los rios mas importantes del pais.\n";
   std::cout << " - Opcion 5: Muestra una matriz indicando qué estados comparten frontera terrestre.\n";
   std::cout << " - Opcion 0: Cierra la aplicacion.\n\n";
   std::cout << "Los datos se guardan en 'estados.dat' para un acceso mas rapido en usos futuros.\n";
}

//funcion para imprimir el mensaje al momento de terminar el programa
void despedida() 
{
   std::cout << "\nGracias por usar el Mapa Interactivo de Mexico. ¡Hasta pronto!\n" << std::endl;
}

//funcion para mostrar los estados junto con su clima
void mostrarClimas(const std::vector<Estado>& estados) 
{
   mostrarEncabezado("Climas Predominantes en Mexico");//encabezado
   std::cout << std::left << std::setw(25) << "Estado" << "Clima Predominante\n";//muestra en pantalla el nombre del estado y el clima que predomina
   std::cout << "------------------------------------------\n";
   
   //recorre cada estado del vector y muestra su nombre junto con su tipo de clima
   for (const auto& estado : estados) 
   {
     std::cout << std::left << std::setw(25) << estado.nombre
     << obtenerNombreClima(estado.tipoClima) << std::endl;
    }
}

//funcion para que el usuario pueda consultar la informacion de un estado
void consultarEstado(const std::vector<Estado>& estados) 
{
    mostrarEncabezado("Consultar Datos de un Estado");//titulo de la opcion
    std::string nombreEstado;
    std::cout << "Escriba el nombre del estado a consultar: ";//pide al usuario ingresar el nombre del estado que desea consultar
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpiar buffer
    std::getline(std::cin, nombreEstado);//captura el nombre del estado

    int indice = obtenerEstadoPorNombre(estados, nombreEstado);//busca el indice del estado en el vector

    if (indice != -1) //si se encuentra el estado
	{
      const Estado* estadoPtr = &estados[indice];//apunta al estado encontrado

      std::cout << "\n--- Mostrando Datos para: " << estadoPtr->nombre << " ---\n";
      std::cout << "Capital: " << estadoPtr->capital << std::endl;
      std::cout << "Poblacion: " << estadoPtr->poblacion << " habitantes." << std::endl;
      std::cout << "Superficie: " << estadoPtr->superficie << " km^2." << std::endl;
      std::cout << "Clima Predominante: " << obtenerNombreClima(estadoPtr->tipoClima) << std::endl;

      //muestra datos adicionales segun el tipo de clima que presente dicho estado
      if (estadoPtr->tipoClima == Clima::PREDOM_CALIDO) //si el clima predominante es calido
	  {
        std::cout << "Ranking Turistico (Sol y Playa): " << estadoPtr->extra.rankingTuristico << "/10" << std::endl;//se muestra el ranking turistico
       } else {
          std::cout << "Indice de Desarrollo (IDH): " << estadoPtr->extra.indiceDesarrollo << std::endl;
       }
       
       //muestra un dato historico del estado utilizando memoria dinamica
       std::cout << "\n--- Dato Historico ---\n";
       char* datoDinamico = new char[strlen(estadoPtr->datoHistorico) + 1];
       strcpy(datoDinamico, estadoPtr->datoHistorico);
       std::cout << datoDinamico << std::endl;
       delete[] datoDinamico;//libera la memoria asignada

       //le permite al usuario guardar el reporte en un archivo de texto
       std::cout << "\n¿Desea guardar un reporte de este estado en 'reporte.txt'? (s/n): ";
       char resp;
       std::cin >> resp;
      if (resp == 's' || resp == 'S') 
	  {
         guardarReporteEstado(*estadoPtr);//funcion externa para guardar el reporte
         std::cout << "Reporte guardado exitosamente.\n";
      }
    } else{
       std::cout << "Estado no encontrado. Verifique el nombre e intente de nuevo.\n";
    }
}

//funcion para mostrar los principales sistemas montañosos
void mostrarCordilleras() 
{
   mostrarEncabezado("Principales Sistemas Montanosos de Mexico");//titulo
   //se crea un vector con los nombres de las principales cordilleras
   std::vector<std::string> cordilleras = {
     "Sierra Madre Occidental", "Sierra Madre Oriental", "Eje Neovolcanico Transversal",
     "Sierra Madre del Sur", "Sierra de Baja California", "Sierra Madre de Chiapas"
   };
   
   //se recorre el vector utilizando el bucle y se imprime cada nombre de la cordillera
   for (const auto& cordillera : cordilleras) 
   {
     std::cout << " - " << cordillera << std::endl;
    }
}

//funcion para mostrar los rios principales y una pequeña descripcion 
void mostrarRiosPrincipales() 
{
   mostrarEncabezado("Principales Rios de Mexico");//titulo
   
   //arreglo con el nombre y la descripcion de cada rio
   const char* rios[5][2]={
     {"Rio Bravo","Desemboca en el Golfo de Mexico. Sirve de frontera con EE.UU."},
     {"Rio Grijalva-Usumacinta", "La cuenca mas grande de Mexico. Desemboca en el Golfo de Mexico."},
     {"Rio Lerma-Santiago", "Nace en el Estado de Mexico y desemboca en el Oceano Pacefico."},
     {"Rio Balsas", "Importante rio del centro-sur que desemboca en el Oceano Pacifico."},
     {"Rio Panuco", "Nace en la altiplanicie mexicana y desemboca en el Golfo de Mexico."}
  };
  
   //recorre el arreglo y muestra el nombre y descripcion del rio 
   for(int i = 0; i < 5; ++i) 
   {
     std::cout << std::left << std::setw(25) << rios[i][0] << ": " << rios[i][1] << std::endl;
   }
}

//Función que muestra la matriz de adyacencia de los estados mexicanos.
//La matriz indica si dos estados comparten una frontera terrestre (1 = sí, 0 = no).
//estados: vector con los nombres de los estados
//matriz: matriz booleana de 32x32 que indica conexiones entre estados
void mostrarMatrizAdyacencia(const std::vector<Estado>& estados, bool (*matriz)[32]) 
{
   mostrarEncabezado("Matriz de Adyacencia (Fronteras Terrestres)");//titulo
   std::cout << "Muestra si un estado (fila) comparte frontera con otro (columna).\n'1' = Si, '0' = No.\n\n";//explicacion para que el usuario pueda entender
   
    //Imprime los índices de columna para la parte superior de la tabla
   std::cout << " ";
   
   for(size_t i = 0; i < estados.size(); ++i) { std::cout << std::setw(3) << i; }//numeros de columna con ancho fijo
   std::cout << "\n----";
   
   for(size_t i = 0; i < estados.size(); ++i) { std::cout << "---"; }
   std::cout << "\n";


       //Recorre las filas de la matriz(cada estado)
      for(size_t i = 0; i < estados.size(); ++i) {
      	
      	//imprime el numero de fila con una linea divisoria
        std::cout << std::setw(3) << i << " |";
        
        
       // Recorre las columnas (con qué otros estados se conecta)
        for(size_t j = 0; j < estados.size(); ++j) {
        std::cout << std::setw(3) << matriz[i][j];//1 si hay frontera,0 si no
       }
       
       // Al final de cada fila, imprime el nombre del estado correspondiente
       std::cout << " | " << estados[i].nombre << "\n";
    
     }
}

// Esta funcion crea un archivo de texto con la info de un estado
// Ideal para imprimir o tener un respaldo
void guardarReporteEstado(const Estado& estado) 
{
   std::ofstream archivo("reporte.txt");//abre archivo de texto
   
   if (archivo.is_open()) 
   {
     archivo << "========================================\n";
     archivo << " REPORTE DEL ESTADO: " << estado.nombre << "\n";
     archivo << "========================================\n\n";
     archivo << "Capital: " << estado.capital << "\n";
     archivo << "Población: " << estado.poblacion << "\n";
     archivo << "Superficie: " << estado.superficie << " km^2\n";
     archivo << "Clima Predominante: " << obtenerNombreClima(estado.tipoClima) << "\n";
     archivo << "Dato Histórico: " << estado.datoHistorico << "\n";
     archivo.close();
    } else {
      std::cerr << "Error: No se pudo abrir el archivo 'reporte.txt' para escritura.\n";
    }
}

// Esta funcion guarda todos los estados en un archivo binario
// para poder leerlos despues sin tener que meterlos a mano otra vez
void guardarDatosBinario(const std::vector<Estado>& estados) 
{
   std::ofstream archivo("estados.dat", std::ios::binary);//abre el archivo en modo binario
   if (archivo.is_open()) 
   {
     for (const auto& estado : estados) {
     archivo.write(reinterpret_cast<const char*>(&estado), sizeof(Estado));//guarda cada estado
    }
     archivo.close();//cierra archivo
     
   }else{
     std::cerr << "Error: No se pudo crear el archivo binario 'estados.dat'.\n";
    }
}


//Esta funcion intenta abrir el archivo binario de los estados
//Si existe, los mete al vector para poder usarlos
bool cargarDatosBinario(std::vector<Estado>& estados) 
{
   std::ifstream archivo("estados.dat", std::ios::binary);//abre archivo binario
   if (!archivo.is_open()) return false;//si no se puede abrir, devuelve false
   Estado temp;
   
   while (archivo.read(reinterpret_cast<char*>(&temp), sizeof(Estado))) //guarda cada estado
   {
     estados.push_back(temp);//agrega cada estado leido al vector
    }
   archivo.close();//si se cargaron estados, devuelve true
   return !estados.empty();
}


//Busca el nombre del estado en el vector y devuelve su posicion
//Si no lo encuentra, regresa -1
int obtenerEstadoPorNombre(const std::vector<Estado>& estados, const std::string& nombre) 
{
   for (size_t i = 0; i < estados.size(); ++i) 
   {
     std::string nombreEstado = estados[i].nombre;
     std::string nombreBusqueda = nombre;
      
      // convierte ambos a minúsculas para que la búsqueda no falle por mayúsculas
      for (char &c : nombreEstado) c = tolower(c);
      for (char &c : nombreBusqueda) c = tolower(c);
     
     if (nombreEstado == nombreBusqueda) 
	 {
       return i;//si lo encuentra,regresa la posición
     }
    }
   return -1;//si no se encuentra
}

// Devuelve el nombre del clima como texto segun el enum
std::string obtenerNombreClima(Clima c) 
{
    switch (c) 
	{
        case Clima::PREDOM_CALIDO: return "Calido / Humedo";
        case Clima::PREDOM_SECO: return "Seco / Desertico";
        case Clima::PREDOM_TEMPLADO: return "Templado";
        default: return "Indefinido";
    }
    
}

// Simula limpiar la pantalla imprimiendo saltos de línea
void limpiarPantalla() {
    std::cout << "\n\n";
}

// Detiene el programa hasta que el usuario presione Enter
void pausarPantalla() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "\nPresione Enter para continuar...";
    std::cin.get();
}


//funcion para inicializar los datos de cada uno de los estados
void inicializarDatos(std::vector<Estado>& estados, bool (*matrizAdyacencia)[32]) 
{

if (estados.empty()) 
{
    Estado temp;

    strcpy(temp.nombre, "Aguascalientes"); strcpy(temp.capital, "Aguascalientes"); temp.poblacion = 1425607; temp.superficie = 5618; temp.tipoClima = Clima::PREDOM_SECO; temp.extra.indiceDesarrollo = 0.798f; strcpy(temp.datoHistorico, "Sede de la Convencion Revolucionaria de 1914."); estados.push_back(temp);
    strcpy(temp.nombre, "Baja California"); strcpy(temp.capital, "Mexicali"); temp.poblacion = 3769020; temp.superficie = 71446; temp.tipoClima = Clima::PREDOM_SECO; temp.extra.indiceDesarrollo = 0.811f; strcpy(temp.datoHistorico, "Estado fronterizo clave para el comercio."); estados.push_back(temp);
    strcpy(temp.nombre, "Baja California Sur"); strcpy(temp.capital, "La Paz"); temp.poblacion = 798447; temp.superficie = 73922; temp.tipoClima = Clima::PREDOM_SECO; temp.extra.indiceDesarrollo = 0.805f; strcpy(temp.datoHistorico, "Famoso por sus destinos turisticos como Los Cabos."); estados.push_back(temp);
    strcpy(temp.nombre, "Campeche"); strcpy(temp.capital, "San Francisco de Campeche"); temp.poblacion = 928363; temp.superficie = 57924; temp.tipoClima = Clima::PREDOM_CALIDO; temp.extra.rankingTuristico = 7; strcpy(temp.datoHistorico, "Ciudad amurallada, patrimonio de la humanidad."); estados.push_back(temp);
    strcpy(temp.nombre, "Chiapas"); strcpy(temp.capital, "Tuxtla Gutierrez"); temp.poblacion = 5543828; temp.superficie = 73289; temp.tipoClima = Clima::PREDOM_CALIDO; temp.extra.rankingTuristico = 6; strcpy(temp.datoHistorico, "Cuna de importantes culturas mayas como Palenque."); estados.push_back(temp);
    strcpy(temp.nombre, "Chihuahua"); strcpy(temp.capital, "Chihuahua"); temp.poblacion = 3741869; temp.superficie = 247455; temp.tipoClima = Clima::PREDOM_SECO; temp.extra.indiceDesarrollo = 0.799f; strcpy(temp.datoHistorico, "El estado mas grande de Mexico, hogar de la Barranca del Cobre."); estados.push_back(temp);
    strcpy(temp.nombre, "Ciudad de Mexico"); strcpy(temp.capital, "Ciudad de Mexico"); temp.poblacion = 9209944; temp.superficie = 1485; temp.tipoClima = Clima::PREDOM_TEMPLADO; temp.extra.indiceDesarrollo = 0.835f; strcpy(temp.datoHistorico, "Antigua Tenochtitlan, capital del Imperio Azteca."); estados.push_back(temp);
    strcpy(temp.nombre, "Coahuila"); strcpy(temp.capital, "Saltillo"); temp.poblacion = 3146771; temp.superficie = 151563; temp.tipoClima = Clima::PREDOM_SECO; temp.extra.indiceDesarrollo = 0.809f; strcpy(temp.datoHistorico, "Cuna de la Revolucion Mexicana con Francisco I. Madero."); estados.push_back(temp);
    strcpy(temp.nombre, "Colima"); strcpy(temp.capital, "Colima"); temp.poblacion = 731391; temp.superficie = 5625; temp.tipoClima = Clima::PREDOM_CALIDO; temp.extra.rankingTuristico = 7; strcpy(temp.datoHistorico, "Conocido por su activo volcan de Fuego."); estados.push_back(temp);
    strcpy(temp.nombre, "Durango"); strcpy(temp.capital, "Victoria de Durango"); temp.poblacion = 1832650; temp.superficie = 123451; temp.tipoClima = Clima::PREDOM_SECO; temp.extra.indiceDesarrollo = 0.769f; strcpy(temp.datoHistorico, "Escenario de muchas peliculas del oeste (westerns)."); estados.push_back(temp);
    strcpy(temp.nombre, "Guanajuato"); strcpy(temp.capital, "Guanajuato"); temp.poblacion = 6166934; temp.superficie = 30608; temp.tipoClima = Clima::PREDOM_TEMPLADO; temp.extra.indiceDesarrollo = 0.767f; strcpy(temp.datoHistorico, "Cuna de la Independencia de Mexico."); estados.push_back(temp);
    strcpy(temp.nombre, "Guerrero"); strcpy(temp.capital, "Chilpancingo de los Bravo"); temp.poblacion = 3540685; temp.superficie = 63621; temp.tipoClima = Clima::PREDOM_CALIDO; temp.extra.rankingTuristico = 8; strcpy(temp.datoHistorico, "Hogar del famoso puerto de Acapulco."); estados.push_back(temp);
    strcpy(temp.nombre, "Hidalgo"); strcpy(temp.capital, "Pachuca de Soto"); temp.poblacion = 3082841; temp.superficie = 20846; temp.tipoClima = Clima::PREDOM_TEMPLADO; temp.extra.indiceDesarrollo = 0.761f; strcpy(temp.datoHistorico, "Importante centro minero durante la colonia."); estados.push_back(temp);
    strcpy(temp.nombre, "Jalisco"); strcpy(temp.capital, "Guadalajara"); temp.poblacion = 8348151; temp.superficie = 78589; temp.tipoClima = Clima::PREDOM_TEMPLADO; temp.extra.indiceDesarrollo = 0.791f; strcpy(temp.datoHistorico, "Cuna del mariachi y el tequila."); estados.push_back(temp);
    strcpy(temp.nombre, "Mexico"); strcpy(temp.capital, "Toluca de Lerdo"); temp.poblacion = 16992418; temp.superficie = 22357; temp.tipoClima = Clima::PREDOM_TEMPLADO; temp.extra.indiceDesarrollo = 0.787f; strcpy(temp.datoHistorico, "El estado mas poblado del pais."); estados.push_back(temp);
    strcpy(temp.nombre, "Michoacan"); strcpy(temp.capital, "Morelia"); temp.poblacion = 4748846; temp.superficie = 58643; temp.tipoClima = Clima::PREDOM_TEMPLADO; temp.extra.indiceDesarrollo = 0.742f; strcpy(temp.datoHistorico, "Hogar de la mariposa Monarca en invierno."); estados.push_back(temp);
    strcpy(temp.nombre, "Morelos"); strcpy(temp.capital, "Cuernavaca"); temp.poblacion = 1971520; temp.superficie = 4893; temp.tipoClima = Clima::PREDOM_CALIDO; temp.extra.rankingTuristico = 7; strcpy(temp.datoHistorico, "Conocido como la 'ciudad de la eterna primavera'."); estados.push_back(temp);
    strcpy(temp.nombre, "Nayarit"); strcpy(temp.capital, "Tepic"); temp.poblacion = 1235456; temp.superficie = 27815; temp.tipoClima = Clima::PREDOM_CALIDO; temp.extra.rankingTuristico = 8; strcpy(temp.datoHistorico, "Importante productor de tabaco y frutas tropicales."); estados.push_back(temp);
    strcpy(temp.nombre, "Nuevo Leon"); strcpy(temp.capital, "Monterrey"); temp.poblacion = 5784442; temp.superficie = 64220; temp.tipoClima = Clima::PREDOM_SECO; temp.extra.indiceDesarrollo = 0.825f; strcpy(temp.datoHistorico, "Uno de los centros industriales mas importantes de Mexico."); estados.push_back(temp);
    strcpy(temp.nombre, "Oaxaca"); strcpy(temp.capital, "Oaxaca de Juarez"); temp.poblacion = 4132148; temp.superficie = 93793; temp.tipoClima = Clima::PREDOM_CALIDO; temp.extra.rankingTuristico = 6; strcpy(temp.datoHistorico, "Conocido por su rica cultura indigena y gastronomia."); estados.push_back(temp);
    strcpy(temp.nombre, "Puebla"); strcpy(temp.capital, "Puebla de Zaragoza"); temp.poblacion = 6583278; temp.superficie = 34290; temp.tipoClima = Clima::PREDOM_TEMPLADO; temp.extra.indiceDesarrollo = 0.760f; strcpy(temp.datoHistorico, "Lugar de la Batalla de Puebla del 5 de mayo."); estados.push_back(temp);
    strcpy(temp.nombre, "Queretaro"); strcpy(temp.capital, "Santiago de Queretaro"); temp.poblacion = 2368467; temp.superficie = 11684; temp.tipoClima = Clima::PREDOM_TEMPLADO; temp.extra.indiceDesarrollo = 0.803f; strcpy(temp.datoHistorico, "Centro de la conspiracion que inicio la Independencia."); estados.push_back(temp);
    strcpy(temp.nombre, "Quintana Roo"); strcpy(temp.capital, "Chetumal"); temp.poblacion = 1857985; temp.superficie = 42361; temp.tipoClima = Clima::PREDOM_CALIDO; temp.extra.rankingTuristico = 10; strcpy(temp.datoHistorico, "Hogar de Cancun, la Riviera Maya y sitios arqueologicos."); estados.push_back(temp);
    strcpy(temp.nombre, "San Luis Potosi"); strcpy(temp.capital, "San Luis Potosi"); temp.poblacion = 2822255; temp.superficie = 60983; temp.tipoClima = Clima::PREDOM_SECO; temp.extra.indiceDesarrollo = 0.771f; strcpy(temp.datoHistorico, "Importante centro minero y logistico."); estados.push_back(temp);
    strcpy(temp.nombre, "Sinaloa"); strcpy(temp.capital, "Culiacan Rosales"); temp.poblacion = 3026943; temp.superficie = 57377; temp.tipoClima = Clima::PREDOM_CALIDO; temp.extra.rankingTuristico = 7; strcpy(temp.datoHistorico, "Uno de los mayores productores agricolas de Mexico."); estados.push_back(temp);
    strcpy(temp.nombre, "Sonora"); strcpy(temp.capital, "Hermosillo"); temp.poblacion = 2944840; temp.superficie = 179503; temp.tipoClima = Clima::PREDOM_SECO; temp.extra.indiceDesarrollo = 0.806f; strcpy(temp.datoHistorico, "Estado desertico con una importante industria ganadera."); estados.push_back(temp);
    strcpy(temp.nombre, "Tabasco"); strcpy(temp.capital, "Villahermosa"); temp.poblacion = 2402598; temp.superficie = 24738; temp.tipoClima = Clima::PREDOM_CALIDO; temp.extra.rankingTuristico = 5; strcpy(temp.datoHistorico, "Corazon de la industria petrolera de Mexico."); estados.push_back(temp);
    strcpy(temp.nombre, "Tamaulipas"); strcpy(temp.capital, "Ciudad Victoria"); temp.poblacion = 3527735; temp.superficie = 80175; temp.tipoClima = Clima::PREDOM_SECO; temp.extra.indiceDesarrollo = 0.793f; strcpy(temp.datoHistorico, "Estado fronterizo con importante actividad portuaria."); estados.push_back(temp);
    strcpy(temp.nombre, "Tlaxcala"); strcpy(temp.capital, "Tlaxcala de Xicohtencatl"); temp.poblacion = 1342977; temp.superficie = 3991; temp.tipoClima = Clima::PREDOM_TEMPLADO; temp.extra.indiceDesarrollo = 0.765f; strcpy(temp.datoHistorico, "Aliados de los espanoles en la conquista de Mexico."); estados.push_back(temp);
    strcpy(temp.nombre, "Veracruz"); strcpy(temp.capital, "Xalapa-Enriquez"); temp.poblacion = 8062579; temp.superficie = 71820; temp.tipoClima = Clima::PREDOM_CALIDO; temp.extra.rankingTuristico = 8; strcpy(temp.datoHistorico, "Principal puerto de Mexico en el Golfo por siglos."); estados.push_back(temp);
    strcpy(temp.nombre, "Yucatan"); strcpy(temp.capital, "Merida"); temp.poblacion = 2320898; temp.superficie = 39612; temp.tipoClima = Clima::PREDOM_CALIDO; temp.extra.rankingTuristico = 9; strcpy(temp.datoHistorico, "Centro de la civilizacion maya, con Chichen Itza."); estados.push_back(temp);
    strcpy(temp.nombre, "Zacatecas"); strcpy(temp.capital, "Zacatecas"); temp.poblacion = 1622138; temp.superficie = 75539; temp.tipoClima = Clima::PREDOM_SECO; temp.extra.indiceDesarrollo = 0.762f; strcpy(temp.datoHistorico, "Ciudad colonial con una rica historia minera de plata."); estados.push_back(temp);
}

// 0: Aguascalientes
    matrizAdyacencia[0][13] = matrizAdyacencia[13][0] = true; // Jalisco
    matrizAdyacencia[0][31] = matrizAdyacencia[31][0] = true; // Zacatecas

// 1: Baja California
    matrizAdyacencia[1][2] = matrizAdyacencia[2][1] = true; // Baja California Sur
    matrizAdyacencia[1][25] = matrizAdyacencia[25][1] = true; // Sonora

// 2: Baja California Sur
// (ya definido con Baja California)

// 3: Campeche
    matrizAdyacencia[3][22] = matrizAdyacencia[22][3] = true; // Quintana Roo
    matrizAdyacencia[3][26] = matrizAdyacencia[26][3] = true; // Tabasco
    matrizAdyacencia[3][30] = matrizAdyacencia[30][3] = true; // Yucatán

// 4: Chiapas
    matrizAdyacencia[4][19] = matrizAdyacencia[19][4] = true; // Oaxaca
    matrizAdyacencia[4][26] = matrizAdyacencia[26][4] = true; // Tabasco
    matrizAdyacencia[4][29] = matrizAdyacencia[29][4] = true; // Veracruz

// 5: Chihuahua
    matrizAdyacencia[5][7] = matrizAdyacencia[7][5] = true; // Coahuila
    matrizAdyacencia[5][9] = matrizAdyacencia[9][5] = true; // Durango
    matrizAdyacencia[5][24] = matrizAdyacencia[24][5] = true; // Sinaloa
    matrizAdyacencia[5][25] = matrizAdyacencia[25][5] = true; // Sonora

// 6: Ciudad de México
    matrizAdyacencia[6][14] = matrizAdyacencia[14][6] = true; // México
    matrizAdyacencia[6][16] = matrizAdyacencia[16][6] = true; // Morelos

// 7: Coahuila
    matrizAdyacencia[7][9] = matrizAdyacencia[9][7] = true; // Durango
    matrizAdyacencia[7][18] = matrizAdyacencia[18][7] = true; // Nuevo León
    matrizAdyacencia[7][23] = matrizAdyacencia[23][7] = true; // San Luis Potosí
    matrizAdyacencia[7][31] = matrizAdyacencia[31][7] = true; // Zacatecas

// 8: Colima
    matrizAdyacencia[8][13] = matrizAdyacencia[13][8] = true; // Jalisco
    matrizAdyacencia[8][15] = matrizAdyacencia[15][8] = true; // Michoacán

// 9: Durango
    matrizAdyacencia[9][13] = matrizAdyacencia[13][9] = true; // Jalisco
    matrizAdyacencia[9][17] = matrizAdyacencia[17][9] = true; // Nayarit
    matrizAdyacencia[9][24] = matrizAdyacencia[24][9] = true; // Sinaloa
    matrizAdyacencia[9][31] = matrizAdyacencia[31][9] = true; // Zacatecas

// 10: Guanajuato
    matrizAdyacencia[10][13] = matrizAdyacencia[13][10] = true; // Jalisco
    matrizAdyacencia[10][15] = matrizAdyacencia[15][10] = true; // Michoacán
    matrizAdyacencia[10][21] = matrizAdyacencia[21][10] = true; // Querétaro
    matrizAdyacencia[10][23] = matrizAdyacencia[23][10] = true; // San Luis Potosí
    matrizAdyacencia[10][31] = matrizAdyacencia[31][10] = true; // Zacatecas

// 11: Guerrero
    matrizAdyacencia[11][14] = matrizAdyacencia[14][11] = true; // México
    matrizAdyacencia[11][15] = matrizAdyacencia[15][11] = true; // Michoacán
    matrizAdyacencia[11][16] = matrizAdyacencia[16][11] = true; // Morelos
    matrizAdyacencia[11][19] = matrizAdyacencia[19][11] = true; // Oaxaca
    matrizAdyacencia[11][20] = matrizAdyacencia[20][11] = true; // Puebla

// 12: Hidalgo
    matrizAdyacencia[12][14] = matrizAdyacencia[14][12] = true; // México
    matrizAdyacencia[12][20] = matrizAdyacencia[20][12] = true; // Puebla
    matrizAdyacencia[12][21] = matrizAdyacencia[21][12] = true; // Querétaro
    matrizAdyacencia[12][23] = matrizAdyacencia[23][12] = true; // San Luis Potosí
    matrizAdyacencia[12][28] = matrizAdyacencia[28][12] = true; // Tlaxcala
    matrizAdyacencia[12][29] = matrizAdyacencia[29][12] = true; // Veracruz

// 13: Jalisco
// (ya definido con Ags, Col, Gto, Dur, Nay, Zac)

// 14: México
    matrizAdyacencia[14][28] = matrizAdyacencia[28][14] = true; // Tlaxcala

// 15: Michoacán
// (ya definido con Col, Gto, Gue, Mex, Jal)

// 16: Morelos
// (ya definido con CDMX, Gue, Mex, Pue)

// 17: Nayarit
    matrizAdyacencia[17][24] = matrizAdyacencia[24][17] = true; // Sinaloa

// 18: Nuevo León
    matrizAdyacencia[18][23] = matrizAdyacencia[23][18] = true; // San Luis Potosí
    matrizAdyacencia[18][27] = matrizAdyacencia[27][18] = true; // Tamaulipas

// 19: Oaxaca
    matrizAdyacencia[19][20] = matrizAdyacencia[20][19] = true; // Puebla
    matrizAdyacencia[19][29] = matrizAdyacencia[29][19] = true; // Veracruz

// 20: Puebla
// (ya definido con Gue, Hid, Mex, Oax, Ver, Tla)

// 21: Querétaro
// (ya definido con Gto, Hid, Mex, SLP)

// 22: Quintana Roo
    matrizAdyacencia[22][30] = matrizAdyacencia[30][22] = true; // Yucatán

// 23: San Luis Potosí
    matrizAdyacencia[23][29] = matrizAdyacencia[29][23] = true; // Veracruz
    matrizAdyacencia[23][13] = matrizAdyacencia[13][23] = true; // Jalisco
    matrizAdyacencia[23][27] = matrizAdyacencia[27][23] = true; // Tamaulipas

// 24: Sinaloa
// (ya definido con Chih, Dur, Nay, Son)

// 25: Sonora
// (ya definido con BC, Chih, Sin)

// 26: Tabasco
// (ya definido con Cam, Chi, Ver)

// 27: Tamaulipas
    matrizAdyacencia[27][29] = matrizAdyacencia[29][27] = true; // Veracruz

// 28: Tlaxcala
    matrizAdyacencia[28][20] = matrizAdyacencia[20][28] = true; // Puebla

// 29: Veracruz
// (ya definido con Chi, Hid, Oax, Pue, SLP, Tab, Tam)

// 30: Yucatán
// (ya definido con Cam, QR)

// 31: Zacatecas
// (ya definido con Ags, Coah, Dur, Gto, Jal, SLP, Nay)
}
