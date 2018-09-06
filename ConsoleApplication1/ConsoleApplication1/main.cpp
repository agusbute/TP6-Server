#include "stdafx.h"
#include <SDKDDKVer.h>
#include "server.h"

#define MAX_FOR_MSGS	50

using namespace std;

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


//Devuelve un bool indicando si msg tiene el formato correcto.
//Si tiene formato correcto copia el path en el puntero a char indicado.
bool verify_received_msg(char * msg, char * path)
{
	const char * str1 = "GET /";
	const char * str2 = "HTTP/1.1\r\nHost: 127.0.0.1\r\n";
	const char * str3 = "HTTP/1.1\n\rHost: 127.0.0.1\n\r";
	const char * str4 = "HTTP/1.1\r\nHost: localhost\r\n";
	const char * str5 = "HTTP/1.1\n\rHost: localhost\n\r";
	unsigned int i = 0;
	unsigned int j = 0;
	unsigned int partialI;

	while ((msg[i] == str1[i]) && (i < strlen(str1)))
	{
		i++;
	}
	if (i == strlen(str1))
	{
		while ((msg[i] != ' ') && (i<(strlen(str1) + +MAX_FOR_MSGS)))
		{
			path[j++] = msg[i++];
		}
		path[j] = 0;
		j = 0;

		if ((msg[i] == ' ') && (strlen(path)))
		{
			i++;
			partialI = i;
			while ((msg[i] == str2[j]) && (j < strlen(str2)))
			{
				i++; j++;
			}
			if (j == strlen(str2))
			{
				return true;
			}
			else
			{
				i = partialI;
				j = 0;
				while ((msg[i] == str3[j]) && (j < strlen(str3)))
				{
					i++; j++;
				}
				if (j == strlen(str3))
				{
					return true;
				}
				else
				{
					i = partialI;
					j = 0;
					while ((msg[i] == str4[j]) && (j < strlen(str4)))
					{
						i++; j++;
					}
					if (j == strlen(str4))
					{
						return true;
					}
					else
					{
						i = partialI;
						j = 0;
						while ((msg[i] == str5[j]) && (j < strlen(str5)))
						{
							i++; j++;
						}
						if (j == strlen(str5))
						{
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}