
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

#include "CToolsToAgile.h"

using namespace std;


const PilDescription paramsDescr[] = {
    { PilString, "fitsfilePath", "Input fits file name to convert" },
    { PilString, "newFitsFileName", "Insert new name for the output fits file"},
    { PilNone, "", "" }
};


int main(int argc, char *argv[]) {
	cout << "############## AG_ctooltoagile ##############" << endl;
	
	PilParams params(paramsDescr);
    if (!params.Load(argc, argv))
        return EXIT_FAILURE;
	
	
	// PARAMETRI OBBLIGATORI
	
	const char *fitsfilePath = params["fitsfilePath"];
	const char *newFitsFileName = params["newFitsFileName"];
	
	
	cout << "\nInput fitsfile path: " << fitsfilePath << endl;
	
	
	CToolsToAgile ctoolAgile(fitsfilePath, newFitsFileName);
	
	
	
	cout << "\n############## AG_ctooltoagile end ##############" << endl;
}
