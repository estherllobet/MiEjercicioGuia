#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

<<<<<<< HEAD
int es_palindromo(const char *nombre) {
	int longitud = strlen(nombre);
	for (int i = 0; i < longitud / 2; i++) {
		if (nombre[i] != nombre[longitud - i - 1]) {
			return 0; // No es pali�ndromo
		}
	}
	return 1; // Es pali�ndromo
}

=======
>>>>>>> a4fa14dea76c39cee6836acdf7760c2e708ffd92
int main(int argc, char *argv[])
{
	int sock_conn, sock_listen, ret;
	struct sockaddr_in serv_adr;
	char peticion[512];
	char respuesta[512];
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
	// escucharemos en el port 9050
	serv_adr.sin_port = htons(9080);
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
		printf ("Error al bind");
	//La cola de peticiones pendientes no podr? ser superior a 4
	if (listen(sock_listen, 4) < 0)
		printf("Error en el Listen");
	int i;
	// Atenderemos solo 10 peticione
	for(i=0;i<10;i++){
		printf ("Escuchando\n");
		
		sock_conn = accept(sock_listen, NULL, NULL);
		printf ("He recibido conexion\n");
		//sock_conn es el socket que usaremos para este cliente
		
		//bucle de atencion al cliente
		int terminar = 0;
		while (terminar == 0)
		{			
			// Ahora recibimos su peticion
			ret=read(sock_conn,peticion, sizeof(peticion));
			printf ("Recibida una peticion\n");
			// Tenemos que a?adirle la marca de fin de string 
			// para que no escriba lo que hay despues en el buffer
			peticion[ret]='\0';
			
			//Escribimos la peticion en la consola
			
			printf ("La peticion es: %s\n",peticion);
			char *p = strtok(peticion, "/");
			int codigo =  atoi (p);
			
			char nombre[20];
			
			if (codigo != 0)
			{	
				p = strtok( NULL, "/");
				strcpy (nombre, p);
				printf ("Codigo: %d, Nombre: %s\n", codigo, nombre);
			}
			
			if (codigo == 0)
				terminar = 1;
			else if (codigo ==1) //piden la longitd del nombre
				sprintf (respuesta,"%d",strlen (nombre));
			else if (codigo == 2)
				// quieren saber si el nombre es bonito
				if((nombre[0]=='M') || (nombre[0]=='S'))
				strcpy (respuesta,"SI");
				else
					strcpy (respuesta,"NO");
<<<<<<< HEAD
			else if (codigo == 3) { // Verificar si es pali�ndromo
				if (es_palindromo(nombre)) 
				{
					strcpy(respuesta, "SI");
				} 
				else 
				{
					strcpy(respuesta, "NO");
				}
			}
			else //decir si es alto
=======
				else //decir si es alto
>>>>>>> a4fa14dea76c39cee6836acdf7760c2e708ffd92
				{
					p = strtok(NULL, "/");
					float altura =  atof (p);
					if (altura > 1.70)
						sprintf (respuesta, "%s: eres alto", nombre);
					else
						sprintf (respuesta, "%s: eres bajo", nombre);
				}
			
			if(codigo!=0)
			{
				printf("Respuesta: %s\n", respuesta);
					// Enviamos la respuesta
				write (sock_conn,respuesta, strlen(respuesta));
			}
		}	
		// Se acabo el servicio para este cliente
		close(sock_conn); 
	}
}

