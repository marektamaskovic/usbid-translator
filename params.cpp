#include "params.hpp"

int parseParams(int argc, char *argv[], params_t &params){
	if(argc == 2 && !((std::string)(argv[1])).compare("--help")){
		params.help_f = true;
	}
	else if((argc == 4 || argc == 5) && !((std::string)(argv[1])).compare("id")){
		try{
			params.id[0] = std::stoul(argv[2], 0, 16);
			params.id[1] = std::stoul(argv[3], 0, 16);
		}
		catch(std::invalid_argument &e){
			wrongParams(e.what());
		}
		catch(std::out_of_range &e){
			wrongParams(e.what());
		}
		if(argc == 5 && (((std::string)(argv[4])).length() > 0) ){
			params.filepath = argv[4];
		}
		params.id_f = true;
	}
	else if((argc == 5 || argc == 6) && !((std::string)(argv[1])).compare("interface")){
		try{
			params.interface[0] = std::stoul(argv[2], 0, 16);
			params.interface[1] = std::stoul(argv[3], 0, 16);
			params.interface[2] = std::stoul(argv[4], 0, 16);
		}
		catch(std::invalid_argument &e){
			wrongParams(e.what());
		}
		catch(std::out_of_range &e){
			wrongParams(e.what());
		}
		if(argc == 6 && (((std::string)(argv[5])).length() > 0) ){
			params.filepath = argv[5];
		}
		params.interface_f = true;
	}
	else{
		wrongParams("");
	}
	return 0;
}

void printParams(const params_t &p){
	std::cout << "id_f: " << p.id_f << std::endl
			  << "interface_f: " << p.interface_f << std::endl;

	for(unsigned i = 0; i < 2; i++)
		std::cout << "id[" << i << "]: " << p.id[i] << std::endl;
	for(unsigned i = 0; i < 3; i++)
		std::cout << "interface[" << i << "]: " << p.interface[i] << std::endl;

	std::cout << p.filepath << std::endl;
}

void printHelp(void){
	std::cout << "USBID-parser help message" << std::endl
			  << "Usage:" << std::endl
			  << "USBID-parser [OPTION] args" << std::endl
			  << "    id\t\tVID PID\t\t " << std::endl
			  << "    interface\tC S P\t" << std::endl;
}

void wrongParams(const std::string &msg){
	std::cerr << "Error:" << "Wrong parameters!" << std::endl;
	if(msg.length()) std::cout << msg << std::endl;
	exit(1);
}
