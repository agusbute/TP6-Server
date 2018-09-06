#include "server.h"

bool verify_received_msg(const char * msg, char * path, char * filename);
int main(int argc, char* argv[])
{
	char * path = (char *)calloc(30, 1);
	char * filename = (char *)calloc(30, 1);
	
	server sv;
	char * message;
	char * filename;
	std::cout << std::endl << "Starting handler on port " << PORT << std::endl;
	if (sv.startConnection())	//esto devuelve un Bool true si se conectaron y un false si no
	{
		message = sv.recieve_message();		//aca hay que validar lo que nos manda y verificar si lo que hay es todo lo de la consigna
		verify_received_msg(message, path, filename);
	}
	else
	{
		std::cout << "Error while listening." << std::endl;
	}
	sv.send_message();
	Sleep(100);
	free(path);
	free(filename);
	return 0;
}

//Devuelve un bool indicando si msg tiene el formato correcto.
//Si tiene formato correcto copia el path en el puntero a char indicado y realiza lo mismo con el filename.
bool verify_received_msg(const char * msg, char * path1, char * filename1)
{

	/*
	Hice pruebas recibindo
	const char * s = "GET";
	const char * s1 = "GET /hola/chau HTTP/1.1 \r\nHost:127.0.0.1 \r\n\r\n";
	const char * y = "GET /hola/chau2 HTTP/1.1 \r\nHost:127.0.0.1 \r\n\r\n";
	const char * s2 = "GET // HTTP/1.1 \r\nHost:127.0.0.1 \r\n\r\n";
	*/
	const char * str1 = "GET /";
	const char * str2 = "HTTP/1.1 \r\n";
	const char * str3 = "Host:127.0.0.1 \r\n";
	const char * str4 = "\r\n";
	char * path = (char *)calloc(30, 1);
	char * filename = (char *)calloc(30, 1);

	unsigned int i = 0;
	unsigned int j = 0;
	while ((msg[i] == str1[i]) && (i < strlen(str1)))
	{
		i++;
	}
	if (i == strlen(str1))
	{
		while ((msg[i] != '/') && (i<(strlen(str1) + 30)))
		{
			path[j++] = msg[i++];
		}
		path[j] = 0;
		j = 0;

		if ((msg[i] == '/') && (strlen(path)))
		{
			i++;
			while ((msg[i] != ' ') && (i<strlen(str1) + 30 + strlen(path) + 30))
			{
				filename[j++] = msg[i++];
			}
			filename[j] = 0;
			j = 0;
			if ((msg[i] == ' ') && (strlen(path)))
			{
				i++;
				while ((msg[i] == str2[j]) && (j < strlen(str2)))
				{
					i++; j++;
				}
				if (j == strlen(str2))
				{
					j = 0;
					while ((msg[i] == str3[j]) && (j < strlen(str3)))
					{
						i++; j++;
					}
					if (j == strlen(str3))
					{
						j = 0;
						while ((msg[i] == str4[j]) && (j < strlen(str4)))
						{
							i++; j++;
						}
						if (j == strlen(str4))
						{
							strcpy_s(path1, 30, path);
							strcpy_s(filename1, 30, filename);
							free(path);
							free(filename);
							return true;

						}
					}
				}
			}
		}
	}
	free(path);
	free(filename);
	return false;
}
