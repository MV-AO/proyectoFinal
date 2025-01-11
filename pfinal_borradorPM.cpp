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
	
    ifstream fIn, f; 
    ofstream fOut;
 
    unsigned int x, y, nIntentos = 0;

    Jugador p;
    Vector v; //vector con la info de los jugadores
    unsigned int tam = 0; //numero de elementos rellenados del vector
    string fichNombre;
    
    cout << "Bienvenido al juego del buscaminas. " << endl;
    
    do 
    {
       op = Menu();
       switch (op) 
       { 
    
         case 'a': 
        	   cout << "Nombre del fichero con las posiciones de las minas?: " << endl;
        	   cin >> fichNombre;
        	   cout << endl;
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
       system("CLS");
       MuestraTablero(tab); 
    } 
    while (FinJuego(tab) == false); 
    
  
    if  (MinaAbierta(tab) == true)
       cout << endl << "Fin del Juego. Una mina ha explotado!" << endl;
    else 
       cout << endl << "Fin del Juego. Victoria!" << endl; 

    cout << "Numero de Intentos: " << nIntentos << endl;

    if (TodasCeldasProcesadas(tab) == true ) 
	{ 
		do
		{
	       cout << "Nombre del fichero para leer la informacion jugadores?: " << endl;
	       cin >> fichNombre;
	
	       f.open(fichNombre.c_str());
		   if (!f)
		   {
			  system("CLS");
	          cout << "Error.  No se puede abrir para lectura." << endl;
		   }
	       else 
	       {
	          LeeJugadoresFichero(v, tam, f);
	          cout << "El fichero contiene actualmente " << tam << " jugadores." << endl;
	          f.close();
	       }
	       
		}while(!f);
       
      p = LeeInfoJugador(nIntentos);
       
      if (InsertaJugadorVector(p, v, tam) == false)
         cout << "No suficiente memoria para insertar el jugador." << endl;
      else 
         cout << "Jugador insertado correctamente." << endl;
       
       do
       {
	      cout << "Nombre del fichero para guardar la informacion jugadores?: " << endl; 
	      cin >> fichNombre;
		  fOut.open(fichNombre.c_str());
	      
	      if (fOut.fail())
	      {
			  system("CLS");
	        cout << "Error. No se puede abrir el fichero para escritura." << endl;
		  }
	      else 
	      {
	           EscribeJugadoresFichero(v, tam, fOut);
	           fOut.close();
	      }   
	   }while(fOut.fail());
   }
   
   system("PAUSE");
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

/**
 *
 * Escribe los datos de los jugadpores guardados en un fichero.
 *
 * @param [in] vec vector con los jugadores.
 * @param [in] t numero de vectores realmente utilizados.
 * @param [in] f fichero de salida.
 *
 */
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
/**
 *
 * Muestra el menu y devuelve la opcion seleccionada por el usuario.
 *
 * @return la letra de la opción seleccionada.
 *
 */
char Menu() 
{
	char op;
	do
	{
		cout << "a. Lee las posiciones de las minas desde fichero." << endl;
		cout << "b. Genera aleatoriamente las posiciones de las minas." << endl;
		cout << "c. Descubre celda." << endl;
		cout << "d. Marca celda como una mina. Anyade una bandera." << endl;
		cout << "e. Desmarca celda como una mina." << endl;
		cout << "Escoge una opcion del menu: ";
		cin >>	op;
		cout << endl;
		
		if(op != 'a' && op != 'b' && op != 'c' && op != 'd' && op != 'e')
			cout << "Ingresa otro caracter valido." << endl;
	}
	while(op != 'a' && op != 'b' && op != 'c' && op != 'd' && op != 'e');
	
	return op;
}

/******* Funciones de Inicializacion *******/
/**
 *
 * Inicializa el tablero a partir de la información de un fichero.
 *
 * @param [in] f fichero con la información.
 * @param [out] tab tablero con la información de cada celda rellenado.
 *
 */
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
			tab[i][j].nMinas = NumeroMinasVecinas(tab, i, j);
	
	f.close();
	return;
}
	
/**
 *
 * Inicializa el tablero de forma aleatorio con un maximo de 8 minas.
 *
 * @param [out] tab tablero con la información de cada celda rellenado.
 *
 */
void InicializaAleatoriamente(Tablero tab) 
{
	unsigned int x, y, m;
	srand(time(NULL)); 

	for(unsigned int i = 0; i < FIL; i++)
		for(unsigned int j = 0; j < COL; j++)
		{
			tab[i][j].mina = false;
			tab[i][j].destapada = false;
			tab[i][j].bandera = false;
		}
	
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
		
	for(unsigned int i = 0; i < FIL; i++)
		for(unsigned int j = 0; j < COL; j++)
			tab[i][j].nMinas = NumeroMinasVecinas(tab, i, j);

	return;	
}

/************** Mostrar Tablero *****************/
/**
 *
 * Muestra eltablero por pantalla.
 *
 * @param [in] tab tablero con la información de cada celda rellenado.
 *
 */
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
/**
 *
 * Lee la posicion de la celda que el usuario quiera destapar.
 *
 *@param [out] fil numero de fila en la que se encuentra la celda.
 *@param [out] col numero de columna en la que se encuentra la celda.
 *
 */
void LeeCelda(unsigned int & fil, unsigned int & col)
{	
	do
	{
	   cout << "Coloque la posicion de la celda." << endl << "Fila: ";
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

/**
 *
 * Abre la celda seleccionada por el jugador y sus vecinas si no tienen bombas a su alredador de forma recursiva.
 *
 *@param [in] tab tablero con la información de cada celda rellenado.
 *@param [in] i numero de fila en la que se encuentra la celda de referencia.
 *@param [in] j numero de columna en la que se encuentra la celda de referencia.
 *
 */    
void AbreCelda(Tablero tab, unsigned int i, unsigned int j) 
{
	if(tab[i][j].destapada == false)
    tab[i][j].destapada = true;
	  
    if(tab[i][j].nMinas == 0)
    {
	    for(int a = (int)i-1; a <= (int)i+1; a++)
            for (int b = (int)j-1; b <= (int)j+1; b++)
                if (tab[a][b].destapada == false && a >=0 && a <=7 && b >= 0 && b <= 7)
                    AbreCelda(tab, a, b);
	}
	else
		if(tab[i][j].destapada == false && tab[i][j].mina == false)
	        tab[i][j].destapada = true;
	
	
	return;   
}

/**
 *
 * Comprueba que el juego se haya terminado.
 *
 * @param [in] tab tablero de celdas.
 *
 * @return si se ha acabado o no el juego.
 *
 */
bool FinJuego(const Tablero tab) 
{
    bool finM, finA, end = false;
   
   finM = MinaAbierta(tab);
   finA = TodasCeldasProcesadas(tab);
   
   if(finM == true || finA == true)
   	end = true;
   	
	return end;
}

/**
 *
 * Calcula la cantidad de minas existentes en las celdas vecinas a una determinada.
 *
 *@param [in] tab tablero de celdas.
 *@param [in] i numero de fila en la que se encuentra la celda de referencia.
 *@param [in] j numero de columna en la que se encuentra la celda de referencia.
 *
 * @return la cantidad de minas a su alrededor.
 *
 */
unsigned int NumeroMinasVecinas(const Tablero tab, unsigned int i, unsigned int j) 
{
	unsigned short minas = 0;
	
    for(int a = (int)i-1; a <= (int)i+1; a++)
            for (int b = (int)j-1; b <= (int)j+1; b++)
                if (tab[a][b].mina == true && a >=0 && a <=7 && b >= 0 && b <= 7)
                    minas++;
	
	return minas;
}

/**
 *
 * Comprueba si una mina ha sido destapada.
 *
 * @param [in] tab tablero de celdas.
 *
 * @return si se ha destapado o no una mina.
 *
 */
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

/**
 *
 * Comprueba si todas las celdas se han abierto y se han fichado banderas en donde estan las minas correctamente.
 *
 * @param [in] tab tablero de celdas.
 *
 * @return si todas las celdas se han destapado sin abrir una mina.
 *
 */
bool TodasCeldasProcesadas(const Tablero tab) 
{
    unsigned int i, j, m = 0, mT = 0, n = 0;
    bool all = false;
    
    for(i = 0; i < FIL; i++)
    	for(j = 0; j < COL; j++)
    		if(tab[i][j].mina == true)
    			mT++;
	
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
	
	if(m == mT && n == 0)
		all = true;
	
	return all;
}
