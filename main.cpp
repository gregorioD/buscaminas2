#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <ctime>
#include <stdio.h>  
#include <stdlib.h>   
#include <time.h>
#include "structs.cpp"

using namespace std;

int bAdyacentes(int x, int y, int tlx, int tly, bool Bombas[16][30]);
bool acciones(int x, int y, char z, char Pantalla[16][30], bool Bombas[16][30], int tlX, int tlY);
void descubrir(int x, int y, bool Bombas[16][30], char Pantalla[16][30], int tlX, int tlY);
void mostrarMatriz(int tlx, int tly, char Pantalla[16][30]);
void ponerBombas(int tlx, int tly, int QBombas, bool Bombas[16][30]);
int jugar(int dificultad, char &caso);
bool ganaste(int tlX, int tlY, char Pantalla[16][30], bool Bombas[16][30]);
int menu();
bool confirmacion();
void Grupo();
bool entradaPorTeclado(int dificultad, char &x, char &y, char &z, int tlX, int tlY);
void mostrarBombas(int tlX,int tlY, bool Bombas[16][30]);
void mensajeGanaste(int tiempo);
void mensajePerdiste(int tlX, int tlY, bool Bombas[16][30]);
int menuUsuarios();

int main(){
	system("color 10");
	bool bandera = true;
	int dificultad, resultado;
	char entrada;
	int puntos_jugar;
	
	
	system("mode con: cols=73 lines=30");
	do{
		system("CLS");
		cout <<"    Bienvenido, ingrese 0 para continuar al menu principal" << endl;
		cout<<"    ____  __  _______ _________    __  ________   _____   _____"<<endl;
		cout<<"   / __ )/ / / / ___// ____/   |  /  |/  /  _/ | / /   | / ___/"<<endl;
		cout<<"  / __  / / / /\\__ \\/ /   / /| | / /|_/ // //  |/ / /| | \\__ \\ "<<endl;
		cout<<" / /_/ / /_/ /___/ / /___/ ___ |/ /  / // // /|  / ___ |___/ / "<<endl;
		cout<<"/_____/\\____//____/\\____/_/  |_/_/  /_/___/_/ |_/_/  |_/____/  "<<endl;
		cout<<endl;
		cout<<"      Desarrolladores: Franco Cosolito, Gregorio Diaz          " << endl;
		cout<<"                 version 2.0.0 - 2020                          "<<endl;
		system("pause");
		system("CLS");
		do {
			//Menú de usuasrios y eso. -1 quiere salir, 0 cuando pusiste 4 veces mal, 1 cuando pone bien.
			resultado = menuUsuarios();
			/*
			if (resultado == 1) {
				do {
					system("CLS");
					dificultad = menu();
					if (dificultad < 4 && dificultad>0) {
						puntos_jugar = jugar(dificultad, );
					}
				} while (dificultad != 0);
			}
			else {
				*/
				if (resultado == -1) {
					bandera = false;
				}
			//}
		} while (resultado != 0 && bandera);
	} while (bandera);
}
	
int menuUsuarios(){
	int opcion, devolver, devaluacion_noche, puntos_jugar, dificultad;
	char caso;
	bool no_termino = true;
	DB database;
	Partida match;
	Usuario user;
	if (!ExisteBDD()){
		crearBaseDeDatos(&database);
	}
	while(no_termino){
		cout<<"Ingrese una opcion y pulse enter:"<<endl;
		cout<<"1: Ingresar"<<endl;
		cout<<"2: Crear usuario"<<endl;
		cout<<"3: Ver puntajes"<<endl;
		cout<<"4: Salir del juego"<<endl;
		
		cin>>opcion;
		if (opcion<1 || opcion>4){
			cout<<"La opcion ingresada no es valida, ingrese un valor entre 1 y 4:"<<endl;
			
		}else{
			no_termino = false;
			system("CLS");
			devaluacion_noche = opcion;
			
		}
	}
	switch(devaluacion_noche){
		case 1:
			database = AbrirBaseDeDatos();
			user = AbrirUsuario(&database);
			dificultad = menu();
			do {
				system("CLS");
				dificultad = menu();
				if (dificultad < 4 && dificultad>0) {
					puntos_jugar = jugar(dificultad, caso);
					if(caso == 1){
						GuardarPartida(&match, dificultad, puntos_jugar, caso);
						partidaAUsuario(&match, &user);
					}else{
						GuardarPartida(&match, dificultad, 0, caso);
						partidaAUsuario(&match, &user);
					}
				}
			} while (dificultad != 0);
			break;
		case 2:
			database = AbrirBaseDeDatos();
			crearUsuario(&database);
			
			break;
		case 3:
			database = AbrirBaseDeDatos();
			break;
		case 4:
			devolver = -1;
			break;
	}
	
	guardarDB(&database);
	return devolver;
}
		
int bAdyacentes(int x, int y, int tlx, int tly, bool Bombas[16][30]){
	//las coordenadas de un punto
	//devuelve un entero con la cantidad de bombas adyacentes
	int resultado = 0;
	//la entrada al parametro es de acciones y el usuario ingresa a partir de 1
	--x;
	--y;
	
	if (y < tly-1 && y > 0 && x < tlx-1 && x > 0){
		resultado = (Bombas[y][x - 1] + Bombas[y][x + 1] + Bombas[y + 1][x - 1] + Bombas[y + 1][x] + Bombas[y + 1][x + 1] +
			Bombas[y - 1][x - 1] + Bombas[y - 1][x] + Bombas[y - 1][x + 1]);
	}
	//centro arriba
	if ( y == 0 && x < tlx-1 && x > 0 ){
		resultado = Bombas[y][x - 1] + Bombas[y][x + 1] + Bombas[y + 1][x - 1] + Bombas[y + 1][x] + Bombas[y + 1][x + 1];
	}
	//centro abajo
	if ( y == tly-1 && x < tlx-1 && x > 0 ){
		resultado = Bombas[y][x - 1] + Bombas[y][x + 1] + Bombas[y - 1][x - 1] + Bombas[y - 1][x] + Bombas[y + 1][x - 1];
	}
	//centro, tope y fondo izquierdo
	if (x==0){
		if (y==0){
			resultado = Bombas[y][x + 1] + Bombas[y + 1][x + 1] + Bombas[y + 1][x];
		}else if (y==tly-1){
			resultado = Bombas[y-1][x] + Bombas[y - 1][x + 1] + Bombas[y][x + 1];
		}else{
			resultado = Bombas[y - 1][x] + Bombas[y + 1][x] + Bombas[y][x + 1] + Bombas[y - 1][x + 1] + Bombas[y + 1][x + 1];
		}
	}
	//centro, tope y fondo derecho
	if (x==tlx-1){
		if (y==0){
			resultado = Bombas[y][x - 1] + Bombas[y + 1][x - 1] + Bombas[y + 1][x];
		}else if (y==tly-1){
			resultado = Bombas[y - 1][x - 1] + Bombas[y][x - 1] + Bombas[y - 1][x];
		}else{
			resultado = Bombas[y - 1][x] + Bombas[y + 1][x] + Bombas[y][x - 1] + Bombas[y - 1][x - 1] + Bombas[y + 1][x - 1];
		}
	}
	
	
	
	return resultado;
}
bool acciones(int x, int y, char z, char Pantalla[16][30], bool Bombas[16][30], int tlX, int tlY){
	// a y b coordenadas. z es la accion que quiero realizar. 
	// matPant es la matriz que se muestra y matBomb es la matriz de las bombas.
	bool devolver = true;
	if (z=='D'){
		if (Bombas[y-1][x-1]==true){
			devolver = false;
		}
		else{
			if(Pantalla[y-1][x-1]=='?'){
				descubrir(x, y, Bombas, Pantalla, tlX, tlY );
			}else{
				cout << "Error, no se puede descubrir una celda previamente descubierta, o marcada." << endl;
				system("pause");
				system("CLS");
			}
		}
	}
	else{
		if (z=='M'){
			if (Pantalla[y-1][x-1]=='?'){
				Pantalla[y-1][x-1] = 'B';
			}
			else{
				if (Pantalla[y-1][x-1] == 'B'){
					Pantalla[y-1][x-1] = '?';
				}
				else{
					cout << "Error, no se puede marcar una celda previamente descubierta."<<endl;
					system("pause");
					system("CLS");
				}
			}
		}
	}
	return devolver;
}
				
void descubrir(int x, int y, bool Bombas[16][30], char Pantalla[16][30], int tlX, int tlY){
	// descubrir en la matriz por pantalla la posicion (a; b)
	Pantalla[y-1][x-1] = (char)(bAdyacentes(x, y, tlX, tlY, Bombas) + 48);
}
					
void mostrarMatriz(int tlX,int tlY, char Pantalla[16][30]){
	char caracter_referencia = 'a';
	system("CLS");
	cout<<"   ";
	for (int i=0; i<tlX; i++){
		if (caracter_referencia == '{'){
			caracter_referencia = '1';
		}
		cout<<caracter_referencia<<' ';
		caracter_referencia++;
	}
	cout<<"\n"<<endl;
	caracter_referencia = 'a';
	
	for (int i=0; i<tlY; i++){
		cout<<caracter_referencia<<"  ";
		for (int j=0; j<tlX; j++){
			cout<<Pantalla[i][j]<<' ';
		}
		cout<<endl;
		caracter_referencia++;
	}
	cout<<"\n";
	
}
						
						
void ponerBombas(int tlx, int tly, int QBombas, bool  Bombas[16][30]){
	srand (time(NULL));     
	int posicionX = rand()% tlx, posicionY = rand()% tly, contador = 0;
	while (contador < QBombas){
		if(!Bombas[posicionY][posicionX]){
			Bombas[posicionY][posicionX] = true;
			contador++;
		}
		posicionX = rand()% tlx, posicionY = rand()% tly;
	}
}
							
							
int jugar (int dificultad, char &caso){
	char Pantalla[16][30], accion, coordX, coordY;
	bool Bombas[16][30];
	int tlX, tlY, CuantasBombas, tiempoinicial = time(NULL), tiempofinal = -1;
	bool NoPerdio = true, Gano = false, SeQueda = true, devuelve = false;
	if (dificultad==1){
		tlX = 8;
		tlY = 8;
		CuantasBombas = 10;
	}else{
		if (dificultad==2){
			tlX=16;
			tlY=16;
			CuantasBombas = 40;
		}else{
			if (dificultad==3){
				tlX=30;
				tlY=16;
				CuantasBombas = 99;
			}
		}
	}
	for (int i = 0; i < tlY; i++){
		for (int j = 0; j < tlX; j++){
			Bombas[i][j] = false;
			Pantalla[i][j] = '?';
		}
	}
	
	ponerBombas(tlX, tlY, CuantasBombas, Bombas);
	
	while (NoPerdio && !Gano && SeQueda){
		mostrarMatriz(tlX, tlY, Pantalla);
		SeQueda = entradaPorTeclado(dificultad, coordX, coordY, accion, tlX, tlY);
		if (SeQueda) {
			if (dificultad != 3 || !(coordX > 48 && coordX < 53)) {
				NoPerdio = acciones((int)coordX - 96, (int)coordY - 96, accion, Pantalla, Bombas, tlX, tlY);
				Gano = ganaste(tlX, tlY, Pantalla, Bombas);
			}
			else {
				NoPerdio = acciones((int)coordX - 22, (int)coordY - 96, accion, Pantalla, Bombas, tlX, tlY);
				Gano = ganaste(tlX, tlY, Pantalla, Bombas);
			}
			if (Gano) {
				tiempofinal = time(NULL) - tiempoinicial;
				mensajeGanaste(tiempofinal);
				caso = 'G';
			}
			if (!NoPerdio) {
				mensajePerdiste(tlX, tlY, Bombas);
				caso = 'P';
			}
		}else {
			caso = 'A';
			cin.ignore(100000, '\n');
		}
	}
	return tiempofinal;
}
								
bool ganaste (int tlX, int tlY, char Pantalla[16][30], bool Bombas[16][30]){
	bool estanDescubiertos = true;
	int i = 0, j = 0;
	while (estanDescubiertos && i<tlX){
		j = 0;
		while (estanDescubiertos && j<tlY){
			if (!Bombas[i][j]){
				if (Pantalla[i][j]=='?'|| Pantalla[i][j]=='B'){
					estanDescubiertos = false;
				}
			}
			j++;
		}
		i++;
	}
	return estanDescubiertos;
}
									
int menu(){
	// toma una entrada de un caracter y si es un valor entre 0 y 3 devuelve este valor como entero
	char opcion = 0;
	bool termino = false;
	bool confirmar_salir = false;
	int resultado = 0;
	
	while(!termino){
		cout<<"Ingrese una opcion y presione enter para continuar:"<<endl;
		cout << "1 : Dificultad facil" << endl << "2 : Dificultad intermedia" << endl<< "3 : Dificultad dificil"<<endl;
		cout<<"0 : Salir del juego"<<endl;
		cin >> opcion;
		if ((int)opcion < 48 || (int)opcion > 51){
			system("CLS");
			cout << "La opcion ingresada no es valida, ingrese un caracter entre 0 y 3:" << endl;	
		}else if((int)opcion < 52 && (int)opcion > 48){
			resultado = (int)opcion - 48;
			termino = true;
		}else{
			confirmar_salir = confirmacion();
			if (confirmar_salir){
				resultado = 0;
				termino = true;
			}
		}
		cin.ignore(EOF);
		cin.clear();
	}
	
	return resultado;
}
										
bool confirmacion(){
	// devuelve true si el usuario quiere salir y false si no quiere
	char rta = 0;
	bool devolver = false, conf = true;
	system("CLS");
	cout<<"¿Esta seguro que desea cerrar su sesion?"<<endl;
	cout<<"Ingrese 's' para salir o 'n' para permanecer en el juego:"<<endl;
	while (conf){
		cin>>rta;
		if (rta !='s' && rta !='n'){
			system("CLS");
			cout<<"La opcion no es valida, ingrese 's' para salir o 'n' para permanecer en el juego."<<endl;
			
		}else if(rta == 's'){
			devolver = true;
			conf = false;
			system("CLS");
			cout<<"    ___    ____  ________  _____"<<endl;
			cout<<"   /   |  / __ \\/  _/ __ \\/ ___/"<<endl;
			cout<<"  / /| | / / / // // / / /\\__ \\ "<<endl;
			cout<<" / ___ |/ /_/ // // /_/ /___/ / "<<endl;
			cout<<"/_/  |_/_____/___/\\____//____/  "<<endl;
		}else{
			system("CLS");
			devolver = false;
			conf = false;
		}
	}
	return devolver;
}
											
bool entradaPorTeclado (int dificultad, char &x, char &y, char &z, int tlX, int tlY){
	bool seVa = false, termino = false;
	while (!seVa && !termino){
		switch (dificultad){
		case 1:
		case 2:
			cout <<"Ingrese su coordenada en X. Si desea salir presione 0"<<endl;
			cin >> x;
			if ((x<'a'||x>(char)(tlY+96))&& x!='0'){
				cout << "Error en el ingreso, el proceso de carga vuelve a empezar"<<endl;
				system("pause");
			}else{
				if (x=='0'){
					seVa = true;
				}else{
					cout <<"Ingrese su coordenada en Y. Si desea salir presione 0"<<endl;
					cin >> y;
					if ((y<'a'||y>(char)(tlY+96))&& y!='0'){
						cout << "Error en el ingreso, el proceso de carga vuelve a empezar"<<endl;
						system("pause");
					}else{
						if (y=='0'){
							seVa = true;
						}else{
							cout <<"Ingrese la accion que desea realizar. M para marcar o desmarcar, y D para descubrir. Si desea salir presione 0"<<endl;
							cin >> z;
							if ((z!='M'&&z!='D')&&z!='0'){
								cout << "Error en el ingreso, el proceso de carga vuelve a empezar"<<endl;
								system("pause");
							}else{
								if (z=='0'){
									seVa = true;
								}else{
									termino = true;
								}	
							}
						}
					}
				}
			}
			break;
		case 3:
			cout <<"Ingrese su coordenada en X. Si desea salir presione 0"<<endl;
			cin >> x;
			system ("CLS");
			if ((x<'a'||x>(char)(tlY+96))&&(x<'0'||x>'5')){
				cout << "Error en el ingreso, el proceso de carga vuelve a empezar"<<endl;
				system("pause");
			}else{
				if (x=='0'){
					seVa = true;
				}else{
					cout <<"Ingrese su coordenada en Y. Si desea salir presione 0"<<endl;
					cin >> y;
					if ((y<'a'||y>(char)(tlY+96))&& y!='0'){
						cout << "Error en el ingreso, el proceso de carga vuelve a empezar"<<endl;
						system("pause");
					}else{
						if (y=='0'){
							seVa = true;
						}else{
							cout <<"Ingrese la accion que desea realizar. M para marcar o desmarcar, y D para descubrir. Si desea salir presione 0"<<endl;
							cin >> z;
							if ((z!='M'&&z!='D')&&z!='0'){
								cout << "Error en el ingreso, el proceso de carga vuelve a empezar"<<endl;
								system("pause");
							}else{
								if (z=='0'){
									seVa = true;
								}else{
									termino = true;
								}	
							}
						}
					}
				}
			}
			break;
		}
	}
	return !seVa;
}
												
void mostrarBombas(int tlX,int tlY, bool Bombas[16][30]){
	for (int i=0; i<tlY; i++){
		cout<<"  ";
		for (int j=0; j<tlX; j++){
			cout<<Bombas[i][j]<<' ';
		}
		cout<<endl;
	}
}
													
void mensajeGanaste(int tiempo){
	system("CLS");
	cout<<"    ______     ___      _     __          __          __"<<endl;
	cout<<"   / ____/__  / (_)____(_)___/ /___ _____/ /__  _____/ /"<<endl;
	cout<<"  / /_  / _ \\/ / / ___/ / __  / __ `/ __  / _ \\/ ___/ / "<<endl;
	cout<<" / __/ /  __/ / / /__/ / /_/ / /_/ / /_/ /  __(__  )_/  "<<endl;
	cout<<"/_/    \\___/_/_/\\___/_/\\__,_/\\__,_/\\__,_/\\___/____(_)"<<endl;
	cout<<endl;
	cout<<"   _________    _   _____   _________________"<<endl;
	cout<<"  / ____/   |  / | / /   | / ___/_  __/ ____/"<<endl;
	cout<<" / / __/ /| | /  |/ / /| | \\__ \\ / / / __/   "<<endl;
	cout<<"/ /_/ / ___ |/ /|  / ___ |___/ // / / /___   "<<endl;
	cout<<"\\____/_/  |_/_/ |_/_/  |_/____//_/ /_____/   "<<endl;
	cout<<""<<endl;
	
	cout<<"Tiempo de la partida: "<<tiempo/60<<" minutos, "<<tiempo%60<<" segundos."<<endl;
	cout<<"Vuelve al menu principal"<<endl;
	system("pause");
}
														
														
void mensajePerdiste(int tlx, int tly, bool Bombas[16][30]){
	system("CLS");
	cout<<"    ____  __________  ____  _____________________"<<endl;
	cout<<"   / __ \\/ ____/ __ \\/ __ \\/  _/ ___/_  __/ ____/"<<endl;
	cout<<"  / /_/ / __/ / /_/ / / / // / \\__ \\ / / / __/   "<<endl;
	cout<<" / ____/ /___/ _, _/ /_/ // / ___/ // / / /___   "<<endl;
	cout<<"/_/   /_____/_/ |_/_____/___//____//_/ /_____/   "<<endl;
	cout<<""<<endl;
	mostrarBombas(tlx, tly, Bombas);
	cout<<"Vuelve al menu principal"<<endl;
	system ("pause");
}

