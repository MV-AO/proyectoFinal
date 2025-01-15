/**
 *
 * @file pfinal_borradorPM.cpp
 *
 * @brief 
 *
 * Juego de las minas.
 * 
 *
 * @version 1.0
 * @author Maria Victoria Albino Oviedo
 * @author Paula Pantoja
 * @date 11-01-2025
 *
 * @copyright Universidad de Valencia
 */

#include <iostream>
#include <fstream>
#include <stdlib.h>   
#include <ctime>

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
	unsigned short dia, mes, anyo;
};

/** 
 * 
 * @struct Jugador
 * Registro para guardar la información de un jugador.
 *
 * @var Jugador::nombre
 * Almacena el nombre del jugador.
 * @var Jugador::nac
 * Almacena la fecha de nacimiento del jugador con un formato de dd/mm/aaaa.
 * @var Jugador::PartidasJugadas
 * Almacena el numero de PartidasJugadas realizadas.
 * @var Jugador::intentos
 * Almacena el numero de intentos disponibles.
 */
struct Jugador
{
   string nombre;
   Fecha nac;
   unsigned int PartidasJugadas;
              
};

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
void MenuEscribir();
void InicializaDesdeFichero(Tablero tab, ifstream & f);
void InicializaAleatoriamente(Tablero);
void MuestraTablero(const Tablero);
unsigned int NumeroMinasVecinas(const Tablero, unsigned int, unsigned int);


//funciones del juego
void LeeCelda(unsigned int & fil, unsigned int & col);
void AbreCelda(Tablero, unsigned int, unsigned int); 
bool FinJuego(const Tablero);

//funciones auxiliares
bool MinaAbierta(const Tablero tab);
bool TodasCeldasProcesadas(const Tablero tab);

//funciones del jugador
Jugador LeeInfoJugador(unsigned int);
void LeeJugadoresFichero(Vector, unsigned int &, ifstream &);

bool InsertaJugadorVector(Jugador, Vector, unsigned int &);
void EscribeJugadoresFichero(const Vector, unsigned int, ofstream &);
  
int main() 
{   
    char op; //opcion del menu
    Tablero tab;
	
	char Menu(); // llamar funcion de menu
	
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
Jugador LeeInfoJugador(unsigned int n)
{      
  //completar
}   

void LeeJugadoresFichero(Vector vec, unsigned int &t, ifstream & f) 
{
   // completar  
}

bool InsertaJugadorVector(Jugador a, Vector vec, unsigned int & t) 
{
   // completar  
}

void EscribeJugadoresFichero(const Vector vec, unsigned int t, ofstream & f) 
{
    // completar      
}

/****** Funcion Menu ***********/
char Menu() 
{
	char op;
	do
	{
		MenuEscribir();
		cout << "Escoge una opción del menú: ";
		cin >>	op;
		if(op != 'a' && op != 'b' && op != 'c' && op != 'd' && op != 'e')
			cout << "Ingresa otro carácter valido." << endl;
	}
	while(op != 'a' && op != 'b' && op != 'c' && op != 'd' && op != 'e');
		
	
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

/******* Funciones de Inicializacion *******/
void InicializaDesdeFichero(Tablero tab, ifstream & f) 
{
	unsigned int i, j;
	for(i = 0; i < FIL; i++)
		for(j = 0; j < COL; j++)
		{
			tab[i][j].mina = false;
			tab[i][j].destapada = false;
			tab[i][j].bandera = false;
		}
	do
	{
		f >> i;
		f >> j;
		tab[i][j].mina = true;
	}
	while(!f.eof());
	
	for(i = 0; i < FIL; i++)
		for(j = 0; j < COL; j++)
		{
			tab[i][j].nMinas = NumeroMinasVecinas(tab, i, j);
		}
	
	f.close();
	return;
}
	

void InicializaAleatoriamente(Tablero tab) 
{
	unsigned int x, y;
	srand(time(NULL)); 
    for(unsigned int i = 0; i < MAX_MINAS; i++)
    {
		do
   		{
	        x = rand() % FIL;      
	        y = rand() % COL;
		}
		while(tab[x][y].mina == true);
	tab[x][y].mina = true;
	}
	return;	
}


void MuestraTablero(const Tablero tab) 
{
	for(unsigned int i = 0; i < FIL; i++)
		for(unsigned int j = 0; j < COL; j++)
	{
			if (tab[i][j].destapada == true)
				{
				if (tab[i][j].mina == true)
					cout << "#";
				else if (tab[i][j].nMinas == 0)
					cout <<  " ";
				else
					cout << tab[i][j].nMinas;
				}
			else if (tab[i][j].bandera == true)
				cout << "^";
			else
				cout << "·";
	}
	return;
	
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
	unsigned short minas;
	if (tab[i][j].mina == true) 
	    for(unsigned int a = i-1; a < i+1; a++)
	            for (unsigned int b = j-1; b < j+1; b++)
	           //FALTA LO DE QUE SE CHOQUEN 
	                if (tab[a][b].mina == true)
	                    minas++;
	return minas;

}

bool MinaAbierta(const Tablero tab) 
{    
    // completar
}

bool TodasCeldasProcesadas(const Tablero tab) 
{
    // completar
}
