#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <pthread.h>

//funcion que comprueba si es palindromo 
int es_palindromo(const char *nombre) 
{
	int longitud = strlen(nombre);
	for (int i = 0; i < longitud / 2; i++) {
		if (nombre[i] != nombre[longitud - i - 1]) {
			return 0; // No es pali�ndromo
		}
	}
	return 1; // Es pali�ndromo
}

//funcion que devuelve el nombre en mayusculas
void convertir_a_mayusculas(char *nombre) 
{
	for (int i = 0; nombre[i]; i++) {
		nombre[i] = toupper(nombre[i]);
	}
}


int contador;
//estructura necesaria para acceso excluyente:
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


void *AtenderCliente (void *socket)
{
	int sock_conn;
	int *s;
	s = (int *) socket;
	sock_conn= *s;
	
	char peticion[512];
	char respuesta[512];
	int ret;
	
	int terminar =0;
	// Entramos en un bucle para atender todas las peticiones de este cliente
	//hasta que se desconecte
	while (terminar ==0)
	{
		// Ahora recibimos la petici?n
		ret=read(sock_conn,peticion, sizeof(peticion));
		printf ("Recibido\n");
		
		// Tenemos que a?adirle la marca de fin de string 
		// para que no escriba lo que hay despues en el buffer
		peticion[ret]='\0';
		
		
		printf ("Peticion: %s\n",peticion);
		
		// vamos a ver que quieren
		char *p = strtok( peticion, "/");
		int codigo =  atoi (p);
		// Ya tenemos el c?digo de la petici?n
		char nombre[20];
		
		if ((codigo !=0)&&(codigo !=6))
		{
			p = strtok( NULL, "/");
			strcpy (nombre, p);
			// Ya tenemos el nombre
			printf ("Codigo: %d, Nombre: %s\n", codigo, nombre);
		}
		
		if (codigo ==0) //peticion de desconexion
			terminar=1;
		else if (codigo ==1) //piden la longitd del nombre
			sprintf (respuesta,"%d", strlen(nombre));
		else if (codigo ==2)
			// quieren saber si el nombre es bonito
			if((nombre[0]=='M') || (nombre[0]=='S'))
			strcpy (respuesta,"SI");
			else
				strcpy (respuesta,"NO");
		else if (codigo == 3) { // Verificar si es palindromo
			if (es_palindromo(nombre)) 
			{
				strcpy(respuesta, "SI");
			} 
			else 
			{
				strcpy(respuesta, "NO");
			}
		}
		
		else if (codigo == 4) { // devolver nombre en mayusculas
			convertir_a_mayusculas(nombre);
			sprintf(respuesta, "%s", nombre);
		}
		else if (codigo == 5) //decir si es alto
		{
			p = strtok(NULL, "/");
			float altura =  atof (p);
			if (altura > 1.70)
				sprintf (respuesta, "%s: eres alto", nombre);
			else
				sprintf (respuesta, "%s: eres bajo", nombre);
		}
		else if (codigo == 6) //numero de servicios realizados
		{
			sprintf (respuesta,"%d", contador);
		}
			
		if(codigo!=0)
		{
			printf("Respuesta: %s\n", respuesta);
			// Enviamos la respuesta
			write (sock_conn,respuesta, strlen(respuesta));
		}
		if ((codigo == 1)||(codigo == 2)||(codigo == 3)||(codigo == 4)||(codigo == 5))
		{
			pthread_mutex_lock(&mutex); //no me interrumpas ahora
			contador = contador+1;
			pthread_mutex_unlock(&mutex); //ya puedes interrumpirme
		}
	}
	// Se acabo el servicio para este cliente
	close(sock_conn); 
}
int main(int argc, char *argv[])
{
	
	int sock_conn, sock_listen;
	struct sockaddr_in serv_adr;
	// INICIALITZACIONS
	// Obrim el socket
	if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		printf("Error creant socket");
	// Fem el bind al port
	
	
	memset(&serv_adr, 0, sizeof(serv_adr));// inicialitza a zero serv_addr
	serv_adr.sin_family = AF_INET;
	
	// asocia el socket a cualquiera de las IP de la m?quina. 
	//htonl formatea el numero que recibe al formato necesario
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	// establecemos el puerto de escucha
	serv_adr.sin_port = htons(9080);
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
		printf ("Error al bind");
	
	if (listen(sock_listen, 3) < 0)
		printf("Error en el Listen");
	
	contador = 0;
	int i;
	int sockets[100];
	pthread_t thread;
	i = 0;
	// Bucle infinito
	for (;;){
		printf ("Escuchando\n");
		
		sock_conn = accept(sock_listen, NULL, NULL);
		printf ("He recibido conexion\n");
		
		sockets[i] =sock_conn;
		//sock_conn es el socket que usaremos para este cliente
		
		//crear thread y decirle lo que tiene que hacer
		pthread_create (&thread, NULL, AtenderCliente, &sockets[i]);
		i++;
	}
	
}
