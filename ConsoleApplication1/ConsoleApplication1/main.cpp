#include "server.h"



int main(int argc, char* argv[])
{
	server sv;
	std::cout << std::endl << "Starting handler on port " << PORT << std::endl;
	sv.startConnection();		//esto devuelve un Bool true si se conectaron y un false si no, hacer if


	sv.send_message();
	Sleep(100); 

	return 0;
}