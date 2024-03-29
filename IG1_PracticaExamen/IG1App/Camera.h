//#pragma once
#ifndef _H_Camera_H_
#define _H_Camera_H_

#include <GL/freeglut.h>
#include <glm.hpp>

#include "Viewport.h"

using namespace glm;
//-------------------------------------------------------------------------

class Camera {
public:
	explicit Camera(Viewport* vp);
	~Camera() {};

	// viewPort
	Viewport const& viewPort() const { return *mViewPort; };

	// view matrix 
	glm::dmat4 const& viewMat() const { return mViewMat; };

	void set2D();
	void set3D();

	//void pitch(GLdouble a);   // rotates a degrees on the X axis
	//void yaw(GLdouble a);     // rotates a degrees on the Y axis
	//void roll(GLdouble a);    // rotates a degrees on the Z axis

	void moveLR(GLdouble cs);	// Left / Right
	void moveFB(GLdouble cs);	// Forward / Backward
	void moveUD(GLdouble cs);	// Up / Down
	void lookLR(GLdouble cs);	//Cambia el punto al que mira la c�mara pero sin mover la camra
	void lookUD(GLdouble cs);	//Cambia el punto al que mira la c�mara pero sin mover la camra

	void changePrj();		    //Toggles between orto and perspective
	void orbit(GLdouble incAng, GLdouble incY = 0);

	// projection matrix
	glm::dmat4 const& projMat() const { return mProjMat; };

	// sets scene visible area size 
	void setSize(GLdouble xw, GLdouble yh);
	// updates the scale factor 
	void setScale(GLdouble s);
	// cenital view
	void setCenital();

	// transfers its viewport, the view matrix and projection matrix to the GPU
	void upload() const { mViewPort->upload();  uploadVM(); uploadPM(); };

protected:

	glm::dvec3 mEye = { 0.0, 0.0, 500.0 };	 // camera's position
	glm::dvec3 mLook = { 0.0, 0.0, 0.0 };	 // target's position
	glm::dvec3 mUp = { 0.0, 1.0, 0.0 };		 // the up vector 
	glm::dvec3 mRight, mUpward, mFront;		 // inicializar (?)

	glm::dmat4 mViewMat;					 // view matrix = inverse of modeling matrix 
	void uploadVM() const;					 // transfers viewMat to the GPU

	glm::dmat4 mProjMat;					 // projection matrix
	void uploadPM() const;					 // transfers projMat to the GPU

	GLdouble mRadio = 1000.0, mAng = -45;

	GLdouble xRight, xLeft, yTop, yBot;      // size of scene visible area
	GLdouble mNearVal = 1, mFarVal = 10000;  // view volume
	GLdouble mScaleFact = 1;				 // scale factor
	bool bOrto = true;						 // orthogonal or perspective projection

	Viewport* mViewPort;					 // the viewport
	
    //Establece la matriz de vista (view mat)
	void setVM(); 
	//Establece la proyeccion (ortogonal o perspectiva)
	void setPM(); 
	void setAxes();
};
//-------------------------------------------------------------------------

#endif //_H_Camera_H_