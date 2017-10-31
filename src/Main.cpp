
/*
 * Copyright (c) 2017
 *     Leonardo Baroncelli, Giancarlo Zollino
 *
 * Any information contained in this software
 * is property of the AGILE TEAM and is strictly
 * private and confidential.
 * 
 * https://github.com/Leofaber/MapConverter
*/

#include <iostream>
#include <fstream>
#include <string.h>
#include <PilParams.h>

#include "CToolToAgile.h"

using namespace std;


const PilDescription paramsDescr[] = {
    { PilString, "fitsfilePath", "Input fits file name to convert" },
    { PilNone, "", "" }
};


int main(int argc, char *argv[]) {
	cout << "############## AG_ctooltoagile ##############" << endl;
	
	if(argc > 2) {
		cout << "\nOnly one argument expected\n" << endl;		
		exit (EXIT_FAILURE);
	}
	
	PilParams params(paramsDescr);
    if (!params.Load(argc, argv))
        return EXIT_FAILURE;
	
	
	// PARAMETRO OBBLIGATORI
	
	const char *fitsfilePath = params["fitsfilePath"];
	
	
	cout << "\nInput fitsfile path: " << fitsfilePath << endl;
	
	
	CToolToAgile ctoolAgile(fitsfilePath);
	
	
	
	cout << "\n############## AG_ctooltoagile end ##############" << endl;
}
