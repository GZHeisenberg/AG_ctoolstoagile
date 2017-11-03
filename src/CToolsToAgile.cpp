
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

#include "CToolsToAgile.h"
#include "AgileMap.h"
#include "wcs.h"
#include "MathUtils.h"

using namespace std;

CToolsToAgile::CToolsToAgile(const char * _imagePath, const char * _newFileName) {
	
	imagePath = _imagePath;
	newFileName = _newFileName;
	read(imagePath);
	

	Euler(m_la2,m_ba2, &ao1, &bo1,1);

	
	cout << "ao1: " << ao1 << " bo1: " << bo1 << endl;

	
	if(mapPathToAgileMap(newFileName)==0) {
		cout << "\nImage successfully written!\n\nThe new image'name is: " << newFileName <<endl;
	}else{
		cout << "\nFile not created!" << endl;
		exit(EXIT_FAILURE);
	}
		
}



int CToolsToAgile::mapPathToAgileMap(const char * _newFileName)
{
	
	string newFileName(_newFileName);
	
	
	
	remove(newFileName.c_str());
	
	return write(newFileName.c_str()); 
		
	
	
}


int CToolsToAgile::read(const char *fileName) {
	m_fileName[0] = 0;

	FitsFile f;
	if (!f.Open(fileName)) {
		cerr << "ERROR " << f.Status() << " opening " << fileName << endl;
		return f.Status();
		}

	strncpy(m_fileName, fileName, 1024);
	m_fileName[1023] = 0;
	int bitpix, naxis;
	long naxes[2] = { 1, 1 };
	if (fits_get_img_param(f, 2, &bitpix, &naxis, naxes, f)) {
		cerr << "Wrong image parameters" << endl;
		return 1;
		}
	if (naxis != 2) {
		cerr << "Error: only 2D images are supported" << endl;
		return 1;
		}

	/// zzz MatD mat(2, naxes[1], naxes[0]);
	MatD mat(naxes[1], naxes[0]);
	rows = naxes[1];
	cols = naxes[0];
	long fpixel[2] = { 1, 1 };
	fits_read_pix(f, TDOUBLE, fpixel, mat.Size(), NULL, mat.Buffer(), NULL, f);
	mat.TransposeTo(*this);
	

	/// Mandatory keywords
	f.ReadKey("CDELT1", &m_xbin);
	f.ReadKey("CRPIX1", &m_x0);
	f.ReadKey("CDELT2", &m_ybin);
	f.ReadKey("CRPIX2", &m_y0);
	f.ReadKey("CRVAL1", &m_la2);
	f.ReadKey("CRVAL2", &m_ba2);
	f.ReadKey("LONPOLE", &m_lonpole);
	/// Optional keywords
	//f.ReadKey("CTYPE1", ctype1, "");
	//f.ReadKey("CTYPE2", ctype2, "");
	f.ReadKey("RADECSYS", radecsys, "");
	f.ReadKey("MINENG", &m_emin, 0);
	f.ReadKey("MAXENG", &m_emax, 99999);
	f.ReadKey("INDEX", &m_mapIndex, -2.1);
	f.ReadKey("SC-Z-LII", &m_lp, double(-999));
	f.ReadKey("SC-Z-BII", &m_bp, double(-999));
	f.ReadKey("SC-LONPL", &m_gp, 0.0);
	f.ReadKey("DATE-OBS", m_dateObs, "");
	f.ReadKey("DATE_OBS", m_dateObs, "");	//added
	f.ReadKey("DATE-END", m_dateEnd, "");
	f.ReadKey("DATE_END", m_dateEnd, "");	//added
	f.ReadKey("TSTART", &m_tstart, 0.0);
	f.ReadKey("TSTOP", &m_tstop, 0.0);
	f.ReadKey("TIME_OBS", m_tobs, "");	//added
	f.ReadKey("TIME_END", m_tend, "");	//added
	f.ReadKey("FOVMIN", &m_fovMin, 0.0);
	f.ReadKey("FOV", &m_fovMax, 0.0);
	f.ReadKey("ALBEDO", &m_albedo, 0.0);
	f.ReadKey("PHASECOD", &m_phaseCode, 0l);
	f.ReadKey("STEP", &m_step, 0.0);
	f.ReadKey("SKYL", m_skyL, "");
	f.ReadKey("SKYH", m_skyH, "");

	if (f.Status())
		cerr << "ERROR " << f.Status() << " reading " << fileName << endl;
	
	return f.Status();
}

int CToolsToAgile::write(const char * fileName) {
	
	FitsFile f;
	if (!f.Create(fileName)) {
		cerr << "ERROR " << f.Status() << " creating " << fileName << endl;
		return f.Status();
		}

	MatD mat;
	TransposeTo(mat);

	int bitpix = DOUBLE_IMG;
	int naxis = 2;
	long naxes[2] = { mat.Cols(), mat.Rows() };
	long fpixel[2] = { 1, 1 };
	fits_create_img(f, bitpix, naxis, naxes, f);
	fits_write_pix(f, TDOUBLE, fpixel, mat.Size(), const_cast<double*>(mat.Buffer()), f);

	//f.WriteKey("CTYPE1", ctype1);
	//f.WriteKey("CTYPE2", ctype2);
	f.WriteKey("CTYPE1", "GLON-ARC");
	f.WriteKey("CTYPE2", "GLAT-ARC");
	f.WriteKey("CRPIX1", m_x0);
	f.WriteKey("CRVAL1", ao1);
	f.WriteKey("CDELT1", m_xbin);
	f.WriteKey("CRPIX2", m_y0);
	f.WriteKey("CRVAL2", bo1);
	f.WriteKey("CDELT2", m_ybin);
	f.WriteKey("LONPOLE", m_lonpole);
	f.WriteKey("MINENG", m_emin);
	f.WriteKey("MAXENG", m_emax);
	//f.WriteKey("CUNIT1", "deg     ");
	//f.WriteKey("CUNIT2", "deg     ");
	f.WriteKey("RADECSYS", radecsys);
	f.WriteKey("INDEX", m_mapIndex);
	f.WriteKey("SC-Z-LII", m_lp);
	f.WriteKey("SC-Z-BII", m_bp);
	f.WriteKey("SC-LONPL", m_gp);

	if (m_dateObs[0])
		f.WriteKey("DATE-OBS", m_dateObs);
	if (m_dateEnd[0])
		f.WriteKey("DATE-END", m_dateEnd);

	f.WriteKey("TSTART", m_tstart);
	f.WriteKey("TIME_OBS", m_tobs);	//added
	f.WriteKey("TIME_END", m_tend);	//added
	f.WriteKey("TSTOP", m_tstop);
	f.WriteKey("FOVMIN", m_fovMin);
	f.WriteKey("FOV", m_fovMax);
	f.WriteKey("ALBEDO", m_albedo);
	f.WriteKey("PHASECOD", m_phaseCode);

	if (m_step)
		f.WriteKey("STEP", m_step);

	if (m_skyL[0])
		f.WriteKey("SKYL", m_skyL);
	if (m_skyH[0])
		f.WriteKey("SKYH", m_skyH);

	if (f.Status())
		cerr << "ERROR " << f.Status() << " writing to " << fileName << endl;
	return f.Status();
}






