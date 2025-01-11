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
struct Fecha 
{
	// completar
};

struct Jugador
{
   //completar
            
};

const int MAX_JUGADORES = 100;
typedef Jugador Vector[MAX_JUGADORES];

struct Estado
{
	unsigned int nMinas; // numero de bombas vecinas
	bool mina; //existe una mina en esa celda
	bool destapada; //la celda est� descubierta ya o no
	bool bandera; //la celda est� marcada con una bandera o no
};

typedef Estado Tablero[FIL][COL];


char Menu();
void MenuEscribir();
//funciones de inicializacion
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
    char op; //opcion del menu
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
       cout << "Fichero para leer la informaci�n jugadores?: " << endl;
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
       
      cout << "Fichero para guardar la informaci�n jugadores?: " << endl; 
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
	Jugador p;
	
	cin.ignore();
  	cout << "Por favor ingrese sus datos." << endl;
  	cout << "Nombre: " << endl;
  	getline(cin, p.nombre);
  	cout << "Fecha de nacimiento." << endl;
  	cout << "Dia: ";
  	cin >> p.nac.dia;
  	cout << "Mes: ";
  	cin >> p.nac.mes;
  	cout << "Anyo: ";
  	cin >> p.nac.anyo;
  	
  	p.PartidasJugadas = n;
  	return p;
}   

void LeeJugadoresFichero(Vector vec, unsigned int &t, ifstream & f) 
{
   getline(f, vec[t].nombre);
   f >> vec[t].nac.dia;
   f >> vec[t].nac.mes;
   f >> vec[t].nac.anyo;
   f >> vec[t].PartidasJugadas;
   f.ignore();
   
   t++;
   
   while(f.good())
   {
   		getline(f, vec[t].nombre);
   		f >> vec[t].nac.dia;
  		f >> vec[t].nac.mes;
   		f >> vec[t].nac.anyo;
   		f >> vec[t].PartidasJugadas;
	    f.ignore();
   
  		t++;
   }

   
   return;
}

bool InsertaJugadorVector(Jugador a, Vector vec, unsigned int & t) 
{
	bool bien = true;
	
	if(t > MAX_JUGADORES)
		bien = false;
	else
	{
		vec[t].nombre = a.nombre;
   		vec[t].nac.dia = a.nac.dia;
   		vec[t].nac.mes = a.nac.mes;
   		vec[t].nac.anyo = a.nac.anyo;
   		vec[t].PartidasJugadas = a.PartidasJugadas;
		
		t++;
	}
   	
   	return bien;  
}

void EscribeJugadoresFichero(const Vector vec, unsigned int t, ofstream & f) 
{
	for(unsigned int i = 0; i < t; i++)
	{
		f << vec[i].nombre << endl;
		f << vec[i].nac.dia << " " << vec[i].nac.mes << " " << vec[i].nac.anyo << endl;
   		f << vec[i].PartidasJugadas << endl;
	}
	
   return;       
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
}

bool FinJuego(const Tablero tab) 
{
   bool finM, finA, end = false;
   
   finM = MinaAbierta(tab);
   finA = TodasCeldasProcesadas(tab);
   
   if(finM == true || finA == true)
   	end = true;
   	
	return end;
}

unsigned int NumeroMinasVecinas(const Tablero tab, unsigned int i, unsigned int j) 
{
	// completar
}

bool MinaAbierta(const Tablero tab) 
{    
    unsigned int i, j;
    bool boom = false;
    
    for(i = 0; i < FIL; i++)
    {
    	for(j = 0; j < COL; j++)
    	{
    		if(tab[i][j].mina == true && tab[i][j].destapada == true)
    			boom = true;
		}
	}
	
	return boom;
}

bool TodasCeldasProcesadas(const Tablero tab) 
{
    unsigned int i, j, m = 0, n = 0;
    bool all = false;
    
    for(i = 0; i < FIL; i++)
    {
    	for(j = 0; j < COL; j++)
    	{
    		if(tab[i][j].mina == true && tab[i][j].bandera == true)
    			m++;
    			
    		if(tab[i][j].mina == false && tab[i][j].destapada == false)
    			n++;
		}
	}
	
	if(m == 8 && n == 0)
		all = true;
	
	return all;
}
