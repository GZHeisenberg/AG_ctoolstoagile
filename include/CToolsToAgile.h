/*
 * Copyright (c) 2017
 *     Leonardo Baroncelli, Giancarlo Zollino,
 *
 * Any information contained in this software
 * is property of the AGILE TEAM and is strictly
 * private and confidential.
 * 
 * https://github.com/Leofaber/MapConverter
*/


#include "FitsUtils.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>

#include "ArrayOf.h"
#include "AgileMap.h"
#include "wcs.h"


class CToolsToAgile : public AgileMap
{
	public:

		// Constructor
		CToolsToAgile(const char * imagePath, const char * newFileName);
		
		// Convert fits image in readble fits image	
		int mapPathToAgileMap(const char * newFileName); 


			
	private:
		const char * imagePath;
		const char * newFileName;
		double ** image;
		int rows;
		int cols;
		int read(const char* fileName);
		int write(const char* fileName);	// const
		
		//New CRVAL values
		double ao1, bo1;
		
		/// Data
		/// Mandatory for this map to make sense
		/// CTYPE1 = GLON-ARC
		/// CTYPE2 = GLAT-ARC
		//char ctype1[32];	///CTYPE1
		//char ctype2[32];	///CTYPE2
		double m_xbin;		/// CDELT1
		double m_ybin;		/// CDELT2
		double m_x0;		/// CRPIX1
		double m_y0;		/// CRPIX2
		double m_la2;		/// CRVAL1
		double m_ba2;		/// CRVAL2

		char radecsys[32];	///RADECSYS
		/// Specific data for Agile maps
		double m_lonpole;	/// LONPOLE
		double m_emin;		/// MINENG
		double m_emax;		/// MAXENG
		double m_lp;		/// SC-Z-LII
		double m_bp;		/// SC-Z-BII
		double m_gp;		/// SC-LONPL
		double m_mapIndex;/// INDEX
		double m_fovMin;	/// FOVMIN
		double m_fovMax;	/// FOV
		double m_albedo;	/// ALBEDO
		long   m_phaseCode;/// PHASECOD
		double m_step;		/// STEP (exposure maps only)
		char m_dateObs[32];	/// DATE-OBS
		char m_dateEnd[32];	/// DATE-END
		double m_tstart;    /// TSTART
		char m_tobs[32];	   ///OBSERVATION START TIME 
		double m_tstop;     /// TSTOP
		char m_tend[32];	   /// OBSERVATION STOP TIME
		char m_skyL[1024];  /// SKYL
		char m_skyH[1024];  /// SKYH
		char m_fileName[1024];



};
		
