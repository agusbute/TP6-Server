#include "stdafx.h"
#include <SDKDDKVer.h>
#include "server.h"

#define MAX_FOR_MSGS	50

using namespace std;
bool partially_compare_string(char * p, const char * str, unsigned int * i);
bool verify_received_msg(char * msg, char * path1);

int main(int argc, char* argv[])
{
	server sv;
	char * path = (char *)calloc(MAX_FOR_MSGS + 1, 1);
	char * message;										
	FILE * pFile;
	std::cout << std::endl << "Web server has been started on port " << PORT << std::endl;
	if (sv.startConnection())										//esto devuelve un Bool true si se conectaron y un false si no
	{
		message = sv.recieve_message();		

		if (verify_received_msg(message, path))			//aca hay que validar lo que nos manda y verificar si lo que hay es todo lo de la consigna
		{
			pFile = fopen(path, "a+");
			if (pFile != NULL)
			{
				sv.send_message(pFile,path);
				fclose(pFile);
			}
			else
			{
				sv.send_404();
			}
		}
	}
	else
	{
		std::cout << "Error while connecting." << std::endl;
	}
	return 0;
}

/*verify_received_msg*/
//Devuelve un bool indicando si msg tiene el formato correcto.
//Si tiene formato correcto copia el path en el puntero a char indicado.
bool verify_received_msg(char * msg, char * path)
{
	/*Strings a comparar en distintas partes*/
	const char * str1 = "GET /";
	const char * str2 = "HTTP/1.1\r\nHost: 127.0.0.1\r\n";
	const char * str3 = "HTTP/1.1\n\rHost: 127.0.0.1\n\r";
	const char * str4 = "HTTP/1.1\r\nHost: localhost\r\n";
	const char * str5 = "HTTP/1.1\n\rHost: localhost\n\r";
	unsigned int i = 0;
	unsigned int j = 0;

	if (partially_compare_string(msg, str1, &i))						//Compara primera parte
	{
		while ((msg[i] != ' ') && (i < (strlen(str1) + MAX_FOR_MSGS)))	//Mientras no haya ' ' se considera que es el path.
		{																//El path podra tener como maximo MAX_FOR_MSGS caracteres.
			path[j++] = msg[i++];										//Guarda el path
		}
		path[j] = 0;													//Anade terminador al path.
		j = 0;
		if ((msg[i] == ' ') && (strlen(path)))							//Identifica si se llego al espacio o hubo error por el tamano del path.
		{
			i++;														//Me muevo al siguiente caracter
			if ((partially_compare_string(msg, str2, &i)) ||			//Comparo el mensage con los strings definidos.
				(partially_compare_string(msg, str3, &i)) ||
				(partially_compare_string(msg, str4, &i)) ||
				(partially_compare_string(msg, str5, &i)))
				return true;											//Si alguno es igual, el formato es correcto.
		}
	}
	return false;
}

/*partially_compare_string*/
 /*
 * Compara dos strings hasta que encuentre el terminador str.
 * La diferencia con cmp_str es que no le importa si en un string no se encontro el terminador.
 * i es un puntero a un valor que indique desde que elemento luego de lo que apunte el puntero p se considerara el string.
 * Si devuelve true, el valor del contenido de i aumentara dependiendo del largo de str.
 */
bool partially_compare_string(char * p, const char * str, unsigned int * i)
{
	unsigned int iValue = *i;										//Guardo con que valor se paso a i.
	unsigned int j = 0;
	unsigned int length = strlen(str);
	while ((p[(iValue)] == str[j]) && (j < length))					//Mientras los strings sean iguales y no se haya llegado al terminador de str.
	{
		iValue++;
		j++;
	}
	if (j == length)												//Identifica si se llego al terminador de str o si algun caracter fue distinto.
	{
		*i = iValue;												//Si los strings son parcialmente iguales actualiza i.
		return true;
	}
	return false;
}