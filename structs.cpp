#include "structs.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <cstdio>

using namespace std;

bool ExisteBDD(){
	bool resultado = false;
	ifstream archivo;
	archivo.open("basededatos.dat", ios::binary);
	if (!archivo.fail()){
		if(!archivo.eof()) resultado = true;
	}
	return resultado;
}

void crearBaseDeDatos(PDB database){
	database -> cantidad_usuarios = 0;
	database -> usuarios = new Usuario[100];
	if ((database -> usuarios)==NULL){
		cout<<"Error, no hay memoria suficiente."<<endl;
	}
}
void crearUsuario(PDB database){
	int cont = 0;
	PUsuario user = &(database -> usuarios[database -> cantidad_usuarios]);
	char nombre[11], n[11], contrasena[13];
		bool correcto = false;
	if ((database -> cantidad_usuarios) < 100){
		cout<<"Ingrese su nombre: ";
		gets(nombre);
		while (!correcto){
			if (strlen(nombre) >= 8 && strlen(nombre)<11){
				correcto = true;
				for (unsigned int i = 0; i<sizeof(nombre); i++){
					if(!isalnum(nombre[i])) correcto = false;
				}
				while(correcto && cont < (database -> cantidad_usuarios)){
					// tiraba error querer pasarle asi el nombre de usuario a n, por eso
					// defini a n como *n en vez de n[11], pero supongo que la caca fue no usar
					//strcopy
					strcpy(n, (database -> usuarios[cont].nombre));
					if(strcmp(nombre, n)==0) correcto = false;
					else cont ++;
				}
			}else{
				cout<<"Error. Vuelva a ingresar su nombre: ";
				gets(nombre);
				cout<<endl;
			}
		}
		cout <<"Su nombre es: "<<nombre;
		correcto = false;
		cout<<"Ingrese su contrasena: ";
		gets(contrasena);
		while (!correcto){
			if (strlen(contrasena) > 0 && strlen(contrasena)<13){
				cout <<"Su contrasena es: "<<contrasena;
				correcto = true;
				for (unsigned int i = 0; i<sizeof(contrasena); i++){
					if(!isalnum(contrasena[i])) correcto = false;
				}
			}else{
				cout<<"Error. Vuelva a ingresar su contrasena."<<endl;
				gets(contrasena);
			}
		}
		strcpy((user -> nombre), nombre);
		strcpy((user -> contrasena), contrasena);
		user -> partidasfacil = new Partida[10];
		user -> partidasmedio = new Partida[10];
		user -> partidasdificil = new Partida[10];
		user -> perdidas = 0;
		user -> ganadas = 0;
		user -> abandonos = 0;
		database -> cantidad_usuarios++;
		}else{
			cout<<"Error. Ya se alcanzo la cantidad maxima de usuarios."<<endl;
		}		

}
void GuardarPartida (PPartida match, int dif, int score, char tipo){
	// Cambie la funcion para aprovechar que pasaste todo a punteros,
	// ahora lo que hace es generar una fecha nueva con cada nueva partida
	// y despues le asigna al puntero de fecha de la partida el valor de memoria
	// de la nueva fecha uwu
	Fecha date;
	Pfecha punterodate = &date;
	obtenerFecha(punterodate);
	match -> fecha = punterodate;
	match -> dificultad = dif;
	match -> puntaje = score;
	match -> tipo = tipo;
}
void obtenerFecha(Pfecha date){
	
	int dig0, dig1, dig2, dig3, mes, anio, dia;
	char fecha[25], nombreMes[3];
	time_t current_time;
	current_time=time(NULL);
	ctime(&current_time);
	strcpy(fecha, ctime(&current_time));
	dig0 = (int)fecha[8] - 48;
	dig1 = (int)fecha[9] -48;
	dia = dig0 * 10 + dig1;
	date -> dia = dia;
	for (int i = 0; i<3; i++){
		nombreMes[i] = fecha[i+4];
	}
	mes = QueMesEs(nombreMes);
	date -> mes = mes;
	dig0 = (int)fecha[20] - 48;
	dig1 = (int)fecha[21] -48;
	dig2 = (int)fecha[22] -48;
	dig3 = (int)fecha[23] -48;
	anio = dig0 * 1000 + dig1 * 100 + dig2 * 10 + dig3;
	date -> anyo = anio;
}
	
int QueMesEs(char mes[3]){
	int rdo = -1;
	if (mes[0]=='D'){
		rdo = 12;
	}else{
		if (mes[0]=='N'){
			rdo = 11;
		}else{
			if (mes[0]=='O'){
				rdo = 10;
			}else{
				if (mes[0]=='S'){
					rdo = 9;
				}else{
					if (mes[0]=='F'){
						rdo = 2;
					}else{
						if(mes[0]=='A'){
							if (mes[1]=='p') rdo = 4;
							else rdo = 8;
						}else{
							if(mes[0]=='M'){
								if(mes[2]=='r') rdo = 3;
								else rdo = 5;
							}else{
								if(mes[0]=='J'){
									if(mes[1]=='a') rdo = 1;
									else{
										if (mes[2]=='n') rdo = 6;
										else rdo = 7;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return  rdo;
}

void partidaAUsuario(PPartida match, PUsuario usr){
		if (match -> tipo == 'G') usr -> gan++;
		else if (match -> tipo == 'P') usr -> perd++;
		else usr -> ab++;

		int total = (usr -> gan) + (usr -> perd) + (usr -> ab);

		usr -> ganadas = usr -> gan * 100.0 / total;	
		usr -> perdidas = usr -> perd * 100.0 / total;
		usr -> abandonos = usr -> ab * 100.0 / total;
}



void guardarDB(PDB database){
	// tiene que crear si no lo hay un archivo binario y guardarle la base de datos
	// en caso de que el archivo ya exista se le sobreescribe la base de datos,
	// asi se puede llamar a la misma funcion cada vez que se quiera guardar algo
	ofstream archivo;
	archivo.open("basededatos.dat",ios::binary);
	if(!archivo.fail()){
		archivo.write((char*) database, sizeof(*database));
		archivo.close();
	}else{
		cout<<"Error al abrir el archivo."<<endl;
	}

}

Usuario AbrirUsuario (PDB database){
	Usuario user;
	int QU = database -> cantidad_usuarios, cont = 0, i = 0, longitud;
	char nombre[11], n[11], pwrd[13], p[13];
	bool encontrado = false, coincide = false;
	while(!encontrado){
		cout<<"Ingrese nombre de usuario: ";
		gets(nombre);
		while(!encontrado && cont < QU){
			longitud = strlen(database->usuarios[cont].nombre);
			while (i < longitud){
				n[i]= database -> usuarios[cont].nombre[i];
				i++;
			}
			n[i] = '\0';
			i = 0;
			if (strcmp(nombre, n)==0) encontrado = true;
			else cont++;
		}
	}
	while(!coincide){
		longitud = strlen(database -> usuarios[cont].contrasena);
		while (i < longitud){
			p[i] = database -> usuarios[cont].contrasena[i];
			i++;
		}
		cout<<"Ingrese su contrasena: ";
		gets(pwrd);
		if (strcmp(pwrd, p)==0) coincide = true;
		else cout<<"Contrasena incorrecta, por favor, intente de nuevo."<<endl;
	}	
	if (encontrado && coincide) user = database -> usuarios[cont];
	
	return user;
}

DB AbrirBaseDeDatos(){
	DB database;
	ifstream archivo;
	archivo.open("basededatos.dat",ios::binary);
	if(!archivo.fail()){
		archivo.read((char*) &database, sizeof(database));
		archivo.close();
	}else{
		puts("Error al abrir el archivo.");
	}
	return database;
}



