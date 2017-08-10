#include <iostream>
#include <string>

#include "params.hpp"
#include "USBIDs.hpp"


int main(int argc, char *argv[]){

	std::ios::sync_with_stdio(true);

	std::istream *input = &(std::cin);
	std::ifstream tmp;
	params_t params;
	parseParams(argc, argv, params);

	if(params.filepath.length() != 0){
		tmp.open(params.filepath, std::ifstream::in);

		if(tmp.is_open()){
			input = &tmp;
		}
	}

	if(params.help_f){
		printHelp();
		return 0;
	}

	try{
		class USBIDs usb(input);
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
