#include "stdafx.h"
#include <SDKDDKVer.h>
#include "server.h"


#define CR 13
#define LF 10

int main(int argc, char* argv[])
{
	server sv;
	char * message;
	std::cout << std::endl << "Starting handler on port " << PORT << std::endl;		
	if (sv.startConnection())										//esto devuelve un Bool true si se conectaron y un false si no
	{
		if (*(message = sv.recieve_message()) != NULL)			//aca hay que validar lo que nos manda y verificar si lo que hay es todo lo de la consigna
		{

		}
	}
	else
	{
		std::cout << "Error while listening." << std::endl;
	}
	//verificacion mensaje
	sv.send_message();

	return 0;
}