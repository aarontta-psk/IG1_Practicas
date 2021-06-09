//#pragma once
#ifndef _H_Material_H_
#define _H_Material_H_

#include <GL/freeglut.h>
#include <glm.hpp>

//-------------------------------------------------------------------------

class Material {
protected:
	// Coeficientes de reflexi�n
	glm::fvec4 ambient = { 0.2, 0.2, 0.2, 1.0 };  //Luz que alcanza una superficie aunque no est� expuesta a la fuente de luz
	glm::fvec4 diffuse = { 0.8, 0.8, 0.8, 1.0 };  //Responsable del color de la superficie
	glm::fvec4 specular = { 0.0, 0.0, 0.0, 1.0 }; //Luz que refleja la superficie
	GLfloat expF = 0; // Exponente para la reflexi�n especular
	GLuint face = GL_FRONT_AND_BACK;
	GLuint sh = GL_SMOOTH; // Tipo de matizado

public:
	Material() {};
	virtual ~Material() {};
	virtual void upload();
	void setCopper();
};
//-------------------------------------------------------------------------

#endif //_H_Material_H_