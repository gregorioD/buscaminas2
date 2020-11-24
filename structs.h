struct Fecha{
	int dia, mes, anyo;
};

typedef struct Fecha* Pfecha;

struct Partida{
	// la dificultad es un numero entre 1 y 3 inclusive
	// el tipo es G, P o A, ganada perdida o abandonada
	
	int dificultad, puntaje;
	char tipo;
	Pfecha fecha;
};

typedef struct Partida* PPartida;

struct Usuario{
	int gan, perd, ab;
	char nombre[11], contrasena[13];
	double perdidas, ganadas, abandonos;
	PPartida partidasfacil, partidasmedio, partidasdificil;
	// un array dinamico puede venir bien para almacenar los objetos de tipo Partida, 
	//es decir con la 
	// informacion de las partidas de cada dificultad para cada usuario. 
	//Seria un arreglo con 3 espacios
	// y en cada uno se almacenaria un arreglo dinamico.
};

typedef struct Usuario* PUsuario;

struct DB{
	PUsuario usuarios;
	int cantidad_usuarios;
};

typedef struct DB* PDB;

bool ExisteBDD();
void crearBaseDeDatos(PDB database);
void crearUsuario(PDB database);
void GuardarPartida (PPartida match, int dif, int score, char tipo);
void obtenerFecha(Pfecha date);
int QueMesEs(char mes[3]);
void partidaAUsuario(PPartida match, PUsuario usr);
void guardarDB(PDB database);
Usuario AbrirUsuario (PDB database);
DB AbrirBaseDeDatos();


/*
void nuevaPartida(Usuario usr);
Usuario nuevoUsuario(char * nomb, char * contra);
bool correcto(char nombre[11], char contra[13]);
*/
