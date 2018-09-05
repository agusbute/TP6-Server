#include "server.h"


int main(int argc, char* argv[])
{
	server sv;
	char * message;
	std::cout << std::endl << "Starting handler on port " << PORT << std::endl;		
	if (sv.startConnection())	//esto devuelve un Bool true si se conectaron y un false si no
	{
		message = sv.recieve_message();		//aca hay que validar lo que nos manda y verificar si lo que hay es todo lo de la consigna
	}
	else
	{
		std::cout << "Error while listening." << std::endl;
	}
	sv.send_message();
	Sleep(100); 

	return 0;
}