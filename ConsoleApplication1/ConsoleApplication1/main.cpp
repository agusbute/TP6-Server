#include "stdafx.h"
#include <SDKDDKVer.h>
#include "server.h"


char * lookForPath(char*);

using namespace std;

int main(int argc, char* argv[])
{
	server sv;
	char * message;
	string path;
	FILE * pFile;
	cout << endl << "Starting handler on port " << PORT << endl;		
	if (sv.startConnection())										//esto devuelve un Bool true si se conectaron y un false si no
	{
		if (*(message = sv.recieve_message()) != NULL)			//aca hay que validar lo que nos manda y verificar si lo que hay es todo lo de la consigna
		{
			path = lookForPath(message);
			const char * cpath = path.c_str();
			pFile = fopen(cpath, "r");
			if (pFile != NULL)
			{

			}
			else
			{
				sv.send_404();
			}
			
		}
	}
	else
	{
		std::cout << "Error while listening." << std::endl;
	}
	//verificacion mensaje
	return 0;
}

char * lookForPath(char * message)
{
	char ret[10000];			//espacio para el path
	int i = 0;
	while ((*message) != '/' && i != 100)
	{
		message++;
		i++;
	}
	if (i == 100)
	{
		return (char*)"NO";
	}
	for (int i = 0; *message != ' '; message++, i++)
	{
		ret[i] = *message;
	}
	ret[strlen(message)] = '\0';
	return ret;
}