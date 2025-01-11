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
	unsigned short dia, mes, anyo;
};

struct Jugador
{
   string nombre;
   Fecha nac;
   unsigned int PartidasJugadas;
              
};

const int MAX_JUGADORES = 100;
typedef Jugador Vector[MAX_JUGADORES];

struct Estado
{
	unsigned int nMinas; // numero de bombas vecinas
	bool mina; //existe una mina en esa celda
	bool destapada; //la celda está descubierta ya o no
	bool bandera; //la celda está marcada con una bandera o no
};

typedef Estado Tablero[FIL][COL];


char Menu();
void MenuEscribir();
void InicializaDesdeFichero(Tablero tab, ifstream & f);
void InicializaAleatoriamente(Tablero);
void MuestraTablero(const Tablero);
unsigned int NumeroMinasVecinas(const Tablero, int, int);


//funciones del juego
void LeeCelda(unsigned int & fil, unsigned int & col);
void AbreCelda(Tablero, int, int); 
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
	char op;
	do
	{
		cout << "a. Lee las posiciones de las minas desde fichero." << endl;
		cout << "b. Genera aleatoriamente las posiciones de las minas." << endl;
		cout << "c. Descubre celda." << endl;
		cout << "d. Marca celda como una mina. Añade una bandera." << endl;
		cout << "e. Desmarca celda como una mina." << endl;
		cout << "Escoge una opción del menú: ";
		cin >>	op;
		
		if(op != 'a' && op != 'b' && op != 'c' && op != 'd' && op != 'e')
			cout << "Ingresa otro carácter valido." << endl;
	}
	while(op != 'a' && op != 'b' && op != 'c' && op != 'd' && op != 'e');
	
	return op;
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
	{
		for(j = 0; j < COL; j++)
		{
			tab[i][j].nMinas = NumeroMinasVecinas(tab, i, j);
			cout << tab[i][j].nMinas << " ";
		}
		cout << endl;
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
	for(int i = -1; i < FIL; i++)
	{
		if(i >= 0)
			cout << i+1;
		else
			cout << " ";
		
		for(unsigned int j = 0; j < COL; j++)
		{
			if(i == -1)
				cout << "| " << j + 1 << " ";
			else
			{
				cout << "|";
				if (tab[i][j].destapada == true)
					{
					if (tab[i][j].mina == true)
						cout << " # ";
					else if (tab[i][j].nMinas == 0)
						cout <<  "   ";
					else
						cout << " " << tab[i][j].nMinas << " ";
					}
				else if (tab[i][j].bandera == true)
					cout << " ^ ";
				else
					cout << " . ";
					
			}
		}
				cout << "|";
		
		cout << endl;
		cout << "----------------------------------" << endl;
	}
		
	return;
	
}

/*********** Funciones del Juego **********/
void LeeCelda(unsigned int & fil, unsigned int & col)
{	
	do
	{
	   cout << "Coloque la posición de la celda." << endl << "Fila: ";
	   cin >> fil;
	   cout << "Columna: ";
	   cin >> col;   
	   
	   if(fil < 1 || fil > FIL || col < 1 || col > COL)
	   	cout << "Posicion erronea, introduzca una valida." << endl;
	   	
	}while(fil < 1 || fil > FIL || col < 1 || col > COL);
	
	fil--;
	col--;
	
	return;
}
            
void AbreCelda(Tablero tab, int i, int j) 
{
	if(tab[i][j].destapada == false)
    tab[i][j].destapada = true;
	  
    if(tab[i][j].nMinas == 0)
    {
	    for(int a = i-1; a <= i+1; a++)
            for (int b = j-1; b <= j+1; b++)
                if (tab[a][b].destapada == false && a >=0 && a <=7 && b >= 0 && b <= 7)
                    AbreCelda(tab, a, b);
	}
	else
		if(tab[i][j].destapada == false && tab[i][j].mina == false)
	        tab[i][j].destapada = true;
	
	
	return;   
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

unsigned int NumeroMinasVecinas(const Tablero tab, int i, int j) 
{
	unsigned short minas;
	//if (tab[i][j].mina == true) 
	
    for(int a = i-1; a <= i+1; a++)
            for (int b = j-1; b <= j+1; b++)
           //FALTA LO DE QUE SE CHOQUEN 
                if (tab[a][b].mina == true && a >=0 && a <=7 && b >= 0 && b <= 7)
                    minas++;
	
	return minas;
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
