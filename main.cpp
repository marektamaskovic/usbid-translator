#include <iostream>
#include <string>

#include "params.hpp"
#include "USBIDs.hpp"


int main(int argc, char *argv[]){

	params_t params;
	parseParams(argc, argv, params);

	if(params.help_f){
		printHelp();
		return 0;
	}

	try{
		class USBIDs usb(params.filepath);
		if(params.id_f){
			std::cout << usb.idToString(params.id[0], params.id[1]) << std::endl;
			return 0;
		}
		else if(params.interface_f){
			std::cout << usb.interfaceToString(
									params.interface[0],
									params.interface[1],
									params.interface[2]
								 ) << std::endl;

			return 0;
		}
		else{
			std::cerr << "Unknown error occurred!";
			return 1;
		}
	}
	catch(const syntax_error &e){
		std::cerr << e.what() << std::endl;
		return 1;
	}


	return 0;
}

/* vim: set ts=4 sw=4 tw=0 noet : */
