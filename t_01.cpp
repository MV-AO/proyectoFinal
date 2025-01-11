/**
 *
 * @file t-01.cpp
 *
 * @brief 
 *
 * Juego de las minas.
 * 
 *
 * @version 1.0
 * @author Maria Victoria Albino Oviedo
 * @author Paula Pantoja
 * @date 27-12-2024
 *
 * @copyright Universidad de Valencia
 */

#include <iostream>
#include <fstream>
#include <stdlib.h>   
#include <ctime>
#include <string>

using namespace std;

//info del juego
const int MAX_MINAS = 8; 
const int FIL = 8;
const int COL = 8;

//info de los jugadores
/** 
 * 
 * @struct Fecha
 * Registro para guardar la información de una Fecha
 *
 * @var Fecha::dia
 * Almacena el dia
 * @var Fecha::mes
 * Almacena el mes
 * @var Fecha::anyo
 * Almacena el año
 */
struct Fecha 
{
	unsigned short dia, mes anyo;
}

/** 
 * 
 * @struct Jugador
 * Registro para guardar la información de un jugador.
 *
 * @var Jugador::nom
 * Almacena el nombre del jugador.
 * @var Jugador::nac
 * Almacena la fecha de nacimiento del jugador con un formato de dd/mm/aaaa.
 * @var Jugador::jugadas
 * Almacena el numero de jugadas realizadas.
 * @var Jugador::intentos
 * Almacena el numero de intentos disponibles.
 */
struct Jugador
{
	string nombre;
	unsigned int PartidasJugadas;
}

const int MAX_JUGADORES = 100;

/**
*
* @brief Definicion de tipo de dato Vector de tamaño 100 elementos de tipo Jugador.
*
* @typedef Vector
*/
typedef Jugador Vector[MAX_JUGADORES];

/** 
 * 
 * @struct Estado
 * Registro para guardar la información de una celda.
 *
 * @var Estado::nMinas
 * Almacena el numero de bombas vecinas.
 * @var Estado::mina
 * Comprueba si existe una mina en esa celda.
 * @var Estado::destapada
 * Comprueba si la celda está descubierta ya o no.
 * @var Estado::bandera
 * Comprueba si la celda está marcada con una bandera o no.
 */
struct Estado
{
	unsigned int nMinas; // numero de bombas vecinas
	bool mina; //existe una mina en esa celda
	bool destapada; //la celda está descubierta ya o no
	bool bandera; //la celda está marcada con una bandera o no
};

/**
*
* @brief Definicion de tipo de dato Tablero de tamaño 8x8 elementos de tipo Estado.
*
* @typedef Tablero
*/
typedef Estado Tablero[FIL][COL];


char Menu();
{
	char op;
	MenuEscribir();
	cout << "Escoge una opción del menú: ";
	cin >> 	op;
	
	return op;
}
void MenuEscribir()
{
	cout << "a. Lee las posiciones de las minas desde fichero." << endl;
	cout << "b. Genera aleatoriamente las posiciones de las minas." << endl;
	cout << "c. Descubre celda." << endl;
	cout << "d. Marca celda como una mina. Añade una bandera." << endl;
	cout << "e. Desmarca celda como una mina." << endl;
}

void InicializaDesdeFichero(Tablero, ifstream &);
void InicializaAleatoriamente(Tablero);

//funciones para mostrar el tablero actual del juego
void MuestraTablero(const Tablero);
unsigned int NumeroMinasVecinas(const Tablero, unsigned int, unsigned int);

//funciones del juego
void LeeCelda(unsigned int &, unsigned int &);
void AbreCelda(Tablero, unsigned int,unsigned nt); 
bool FinJuego(const Tablero);

//funciones auxiliares
bool MinaAbierta(const Tablero);
bool TodasCeldasProcesadas(const Tablero);

//funciones del jugador
Jugador LeeInfoJugador(unsigned int);
void LeeJugadoresFichero(Vector, unsigned int &, ifstream &);

bool InsertaJugadorVector(Jugador, Vector, unsigned int &);
void EscribeJugadoresFichero(const Vector, unsigned int, ofstream &);
  
int main() 
{   
    char op = Menu();
    Tablero tab;

    ifstream fIn, f; 
    ofstream fOut;
 
    unsigned int x, y, nIntentos = 0;

    Jugador p;
    Vector v; //vector con la info de los jugadores
    unsigned int tam = 0; //numero de elementos rellenados del vector
    string fichNombre;
    
    do 
    {
       op = Menu();
       switch (op) 
       { 
    
         case 'a': 
        	   cout << "Fichero con las posiciones de las minas?: " << endl;
        	   cin >> fichNombre;
			   fIn.open(fichNombre.c_str());
        	   
             if (fIn.fail())
                cout << "Error. Fichero no encontrado." << endl;
          	 else 
             {
            	   InicializaDesdeFichero (tab, fIn); 
                fIn.close();
             }
          	  break;

         case 'b': 
            InicializaAleatoriamente(tab); 
            break; 

         case 'c':
            LeeCelda(x,y);
            AbreCelda(tab, x, y); 
            nIntentos++; 
            break;

         case 'd':
            LeeCelda(x,y);
            tab[x][y].bandera = true;                 
            break;

         case 'e':
            LeeCelda(x,y);
            tab[x][y].bandera = false;               
            break;
       }
       MuestraTablero(tab); 
    } 
    while (FinJuego(tab) == false); 
    
  
    if  (MinaAbierta(tab) == true)
       cout << endl << "Fin Juego. Mina ha explotado!" << endl;
    else 
       cout << endl << "Fin Juego. Victoria!" << endl; 

    cout << "Numero de Intentos: " << nIntentos << endl;

    if (TodasCeldasProcesadas(tab) == true ) 
	{ 
       cout << "Fichero para leer la información jugadores?: " << endl;
       cin >> fichNombre;

       f.open(fichNombre.c_str());
	   if (!f)
          cerr << "Error.  No se puede abrir para lectura." << endl;
       else 
       {
          LeeJugadoresFichero(v, tam, f);
          cout << "El fichero contiene actualmente " << tam << " jugadores." << endl;
          f.close();
       }
       
      p = LeeInfoJugador(nIntentos);
       
      if (InsertaJugadorVector(p, v, tam) == false)
         cout << "No suficiente memoria para insertar el jugador." << endl;
      else 
         cout << "Jugador insertado correctamente." << endl;
       
      cout << "Fichero para guardar la información jugadores?: " << endl; 
      cin >> fichNombre;
	  fOut.open(fichNombre.c_str());
      
      if (fOut.fail())
        cerr << "Error. No se puede abrir el fichero para escritura." << endl;
      else 
      {
           EscribeJugadoresFichero(v, tam, fOut);
           fOut.close();
      }   
   }

   return 0;
}

// Funciones del Jugador
/**
 *
 * Rellena un tipo Jugador con los datos del usuario actual.
 *
 * @param [in] n numero de intentos.
 *
 *@return el jugador con sus datos rellenados.
 */
Jugador LeeInfoJugador(unsigned int n)
{    
	Jugador p;
	
  	cout << "Por favor ingrese sus datos." << endl;
  	cout << "Nombre: " << endl;
  	getline(cin, p.nom);
  	cout << "Fecha de nacimiento: " << endl;
  	cin << p.nac.dia << p.nac.mes << p.nac.anyo;
  	
  	p.jugadas = n;
  	return p;
}   

/**
 *
 * Rellena un vector con los jugadores que han ganado desde un fichero.
 *
 * @param [in] f fichero con la informacion para rellenar el vector.
 * @param [out] vec vector con los jugadores.
 * @param [out] t numero de vectores realmente utilizados.
 *
 */
void LeeJugadoresFichero(Vector vec, unsigned int & t, ifstream & f) 
{
   //Verificar si t siempre es  al llamar la función o empieza siendo > 0.
   
   getline(f, vec[t].nom);
   f >> vec[t].nac.dia;
   f >> vec[t].nac.mes;
   f >> vec[t].nac.anyo;
   f >> vec[t].jugadas;
   
   t++;
   
   while(!f.eof())
   {
   		getline(f, vec[t].nom);
   		f >> vec[t].nac.dia;
  		f >> vec[t].nac.mes;
   		f >> vec[t].nac.anyo;
   		f >> vec[t].jugadas;
   
  		c++;
  		t++;
   }
   
   return;
}

/**
 *
 * Comprueba si es posible rellenar el vector con un jugador más. Si es cierto, realizará esta operación.
 *
 * @param [in] a jugador a agregar.
 * @param [in, out] vec vector con los jugadores.
 * @param [in, out] t numero de vectores realmente utilizados.
 *
 *@return si es posible o no agregar al jugador.
 */
bool InsertaJugadorVector(Jugador a, Vector vec, unsigned int & t) 
{
	bool bien = true;
	t++;
	
	if(t > MAX_JUGADORES)
		bien = false;
	else
	{
		vec[t].nom = a.nom;
   		vec[t].nac.dia = a.nac.dia;
   		vec[t].nac.mes = a.nac.mes;
   		vec[t].nac.anyo = a.nac.anyo;
   		vec[t].jugadas = a.jugadas;
	}
   	
   	return bien;
}

void EscribeJugadoresFichero(const Vector vec, unsigned int t, ofstream & f) 
{
    // completar      
}

/****** Funcion Menu ***********/
char Menu() 
{
 	// completar
}

/******* Funciones de Inicializacion *******/
void InicializaDesdeFichero(Tablero tab, ifstream & f) 
{   
    // completar
}

void InicializaAleatoriamente(Tablero tab) 
{
  // completar
}


/************** Mostrar Tablero *****************/
void MuestraTablero(const Tablero tab) 
{
    // completar  
} 

/*********** Funciones del Juego **********/
void LeeCelda(unsigned int & fil, unsigned int & col)
{
   // completar    
}
            
void AbreCelda(Tablero tab, unsigned int i, unsigned int j) 
{
   // completar    
}

bool FinJuego(const Tablero tab) 
{
   // completar
}

unsigned int NumeroMinasVecinas(const Tablero tab, unsigned int i, unsigned int j) 
{
	// completar
}

bool MinaAbierta(const Tablero tab) 
{    
    // completar
}

bool TodasCeldasProcesadas(const Tablero tab) 
{
    // completar
}
