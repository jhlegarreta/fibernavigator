/*
 * Anatomy.h
 *
 *  Created on: 07.07.2008
 *      Author: ralph
 */

#ifndef ANATOMY_H_
#define ANATOMY_H_

#include "datasetInfo.h"
#include "surface.h"


class Anatomy: public DatasetInfo {

public:
	Anatomy(DatasetHelper*);
	virtual ~Anatomy();

	bool load(wxString filename);
	void draw() {};
	void generateTexture();
	void generateGeometry() {};
	void initializeBuffer() {};

	void cutSurface();

	wxUint8* getByteDataset();

	//bool isInsideTriangle(double, double, double, double, double, double, double, double);
	//void cutRestOfLine(int, int, int);

private:
	wxUint8 *m_byteDataset;
	wxUint16 *m_shortDataset;
	wxUint8 *m_rgbDataset;
	float *m_floatDataset;
};

#endif /* ANATOMY_H_ */
