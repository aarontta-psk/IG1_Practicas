#include "Entity.h"

#include <gtc/matrix_transform.hpp>  
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_access.hpp>
#include "IG1App.h"

//-------------------------------------------------------------------------

void Abs_Entity::upload(dmat4 const& modelViewMat) const
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(value_ptr(modelViewMat));  // transfers modelView matrix to the GPU
}
//-------------------------------------------------------------------------

EjesRGB::EjesRGB(GLdouble l) : Abs_Entity()
{
	mMesh = Mesh::createRGBAxes(l);
}
//-------------------------------------------------------------------------

void EjesRGB::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glEnable(GL_COLOR_MATERIAL);
		glLineWidth(2);
		mMesh->render();
		glLineWidth(1);
		glDisable(GL_COLOR_MATERIAL);
	}
}
//-------------------------------------------------------------------------

Poligono::Poligono(GLuint numL, GLdouble rd) : Abs_Entity()
{
	mMesh = Mesh::generaPoligono(numL, rd);
}
//-------------------------------------------------------------------------

void Poligono::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);

		glLineWidth(2);
		glColor3d(mColor.r, mColor.g, mColor.b);
		mMesh->render();

		glColor3d(1.0, 1.0, 1.0);
		glLineWidth(1);
	}
}
//-------------------------------------------------------------------------

Sierpinski::Sierpinski(GLuint numP, GLdouble rd) : Abs_Entity()
{
	mMesh = Mesh::generaSierpinski(numP, rd);
}
//-------------------------------------------------------------------------

void Sierpinski::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glPointSize(2);
		glColor4dv(value_ptr(mColor)); //le pasa el puntero al atributo para no poner [r, g, b]
		mMesh->render();
		glColor4dv(value_ptr(dvec4(1)));
		glPointSize(1);
	}
}
//-------------------------------------------------------------------------

TrianguloRGB::TrianguloRGB(GLdouble rd) : Abs_Entity()
{
	mMesh = Mesh::generaTrianguloRGB(rd);
}
//-------------------------------------------------------------------------

void TrianguloRGB::update() {
	localAngle += 2;
	globalAngle++;

	setModelMat(translate(dmat4(1.0), dvec3(rad * cos(radians(globalAngle)), rad * sin(radians(globalAngle)), 0.0)));
	setModelMat(rotate(mModelMat, -radians(localAngle), dvec3(0.0, 0.0, 1.0)));
}
//-------------------------------------------------------------------------

void TrianguloRGB::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glPolygonMode(GL_BACK, GL_LINE);
		glPolygonMode(GL_FRONT, GL_FILL);
		mMesh->render();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}
//-------------------------------------------------------------------------

RectanguloRGB::RectanguloRGB(GLdouble w, GLdouble h) : Abs_Entity()
{
	mMesh = Mesh::generaRectanguloRGB(w, h);
}
//-------------------------------------------------------------------------

void RectanguloRGB::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glPolygonMode(GL_BACK, GL_LINE);
		glPolygonMode(GL_FRONT, GL_FILL);
		mMesh->render();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}
//-------------------------------------------------------------------------

Estrella3D::Estrella3D(GLdouble re, GLuint np, GLdouble h) : Abs_Entity()
{
	mMesh = Mesh::generaEstrellaTexCor(re, np, h);
}
//-------------------------------------------------------------------------

void Estrella3D::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		mTexture->bind(GL_REPLACE);
		mMesh->render();

		aMat = rotate(aMat, radians(180.0), dvec3(0.0, 1.0, 0.0)); // rotamos la matriz
		upload(aMat);
		mMesh->render();

		mTexture->unbind();
	}
}
//-------------------------------------------------------------------------

void Estrella3D::update() {
	zAngle++;
	yAngle++;

	//primero giramos en la 'y' y luego en la 'z' para obtener la rotacion deseada
	setModelMat(translate(dmat4(1), position));
	setModelMat(rotate(mModelMat, radians(yAngle), dvec3(0.0, 1.0, 0.0)));
	setModelMat(rotate(mModelMat, -radians(zAngle), dvec3(0.0, 0.0, 1.0)));
}
//-------------------------------------------------------------------------

void Estrella3D::setPosition(const dvec3& position)
{
	this->position = position;
}
//-------------------------------------------------------------------------

Caja::Caja(GLdouble ld) : Abs_Entity()
{
	mMesh = Mesh::generaContCuboTexCor(ld);
}
//-------------------------------------------------------------------------

void Caja::setTexture(Texture* const& t, Texture* const& t2)
{
	Abs_Entity::setTexture(t);
	mText2 = t2;
}
//-------------------------------------------------------------------------

void Caja::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);

		glEnable(GL_CULL_FACE);

		//Renderizamos primero las caras traseras para
		//evitar problemas con el blending, es decir, tenemos
		//que renderizar los objetos transparentes de m�s lejanos a m�s cercanos.
		glCullFace(GL_FRONT);
		mText2->bind(GL_REPLACE);
		mMesh->render();
		mText2->unbind();

		glCullFace(GL_BACK);
		mTexture->bind(GL_REPLACE);
		mMesh->render();
		mTexture->unbind();

		glDisable(GL_CULL_FACE);
	}
}
//-------------------------------------------------------------------------

CajaConFondo::CajaConFondo(GLdouble ld) : Caja(ld), ld_(ld), angle(0), position(1), modelMatFloor(1)
{
	meshFloor = Mesh::generaRectanguloTexCor(ld, ld, 1, 1);
}
//-------------------------------------------------------------------------

CajaConFondo::~CajaConFondo()
{
	delete meshFloor;
}
//-------------------------------------------------------------------------

void CajaConFondo::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr && meshFloor != nullptr) {

		glEnable(GL_CULL_FACE);
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication

		//Renderizamos primero las caras traseras para
		//evitar problemas con el blending, es decir, tenemos
		//que renderizar los objetos transparentes de m�s lejanos a m�s cercanos.
		upload(aMat);
		glCullFace(GL_FRONT);
		mText2->bind(GL_REPLACE);
		mMesh->render();
		upload(modelViewMat * modelMatFloor);
		meshFloor->render();
		mText2->unbind();

		upload(aMat);
		glCullFace(GL_BACK);
		mTexture->bind(GL_REPLACE);
		mMesh->render();
		upload(modelViewMat * modelMatFloor);
		meshFloor->render();
		mTexture->unbind();

		glDisable(GL_CULL_FACE);
	}
}
//-------------------------------------------------------------------------

void CajaConFondo::update()
{
	this->translateAll();
	angle++;
	angle = std::fmod(angle, 360.0);
}
//-------------------------------------------------------------------------

void CajaConFondo::setPosition(const dvec3& position)
{
	this->position = position;
}
//-------------------------------------------------------------------------

void CajaConFondo::translateAll()
{
	setModelMat(translate(dmat4(1), this->position));
	setModelMat(rotate(mModelMat, radians(-angle), dvec3(0, 0, 1)));
	modelMatFloor = translate(mModelMat, dvec3(0, -ld_ / 2, 0));
	modelMatFloor = rotate(modelMatFloor, radians(90.0), dvec3(1, 0, 0));
}
//-------------------------------------------------------------------------

Suelo::Suelo(GLdouble w, GLdouble h, GLuint rw, GLuint rh) : Abs_Entity()
{
	mMesh = Mesh::generaRectanguloTexCor(w, h, rw, rh);
}
//-------------------------------------------------------------------------

void Suelo::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		mTexture->setWrap(GL_REPEAT);
		//mTexture->bind(GL_REPLACE);
		mTexture->bind(GL_MODULATE);
		glColor4dv(value_ptr(mColor));
		mMesh->render();
		glColor4dv(value_ptr(dvec4(1)));
		mTexture->unbind();
	}
}
//-------------------------------------------------------------------------

Foto::Foto(GLdouble w, GLdouble h) : Abs_Entity()
{
	mMesh = Mesh::generaRectanguloTexCor(w, h, 1, 1);
}
//-------------------------------------------------------------------------

void Foto::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		mTexture->setWrap(GL_REPEAT);
		mTexture->bind(GL_REPLACE);
		mMesh->render();
		mTexture->unbind();
	}
}
//-------------------------------------------------------------------------

void Foto::update() {
	mTexture->loadColorBuffer(IG1App::s_ig1app.winWidth(), IG1App::s_ig1app.winHeight(), GL_FRONT);
}
//-------------------------------------------------------------------------

Planta::Planta(GLdouble w, GLdouble h)
{
	mMesh = Mesh::generaRectanguloTexCor(w, h, 1, 1);
}
//-------------------------------------------------------------------------

void Planta::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0);

		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		mTexture->setWrap(GL_REPEAT);
		mTexture->bind(GL_REPLACE);
		mMesh->render();

		aMat = rotate(aMat, radians(60.0), dvec3(0.0, 1.0, 0.0)); // rotamos la matriz
		upload(aMat);
		mMesh->render();

		aMat = rotate(aMat, radians(60.0), dvec3(0.0, 1.0, 0.0)); // rotamos la matriz
		upload(aMat);
		mMesh->render();

		mTexture->unbind();

		glDisable(GL_ALPHA_TEST);
	}
}
//-------------------------------------------------------------------------

Hexagono::Hexagono(GLdouble radio)
{
	mMesh = Mesh::generaHexagonoTexCor(radio);
}
//------------------------------------------------------------------------

void Hexagono::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);

		mTexture->bind(GL_MODULATE);
		mMesh->render();
		mTexture->unbind();
	}
}
//------------------------------------------------------------------------

AnilloCuadrado::AnilloCuadrado()
{
	mMesh = IndexMesh::generaAnilloCuadradoIndexado();
}
//------------------------------------------------------------------------

void AnilloCuadrado::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);

		glEnable(GL_COLOR_MATERIAL);

		mMesh->render();

		glDisable(GL_COLOR_MATERIAL);
	}
}
//------------------------------------------------------------------------

CuboIndexado::CuboIndexado(GLdouble l)
{
	mMesh = IndexMesh::generaCuboConTapasIndexado(l);
}
//------------------------------------------------------------------------

void CuboIndexado::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);


		glEnable(GL_COLOR_MATERIAL);
		glColor3d(0.0, 1.0, 0.0);
		mMesh->render();
		glColor3d(1.0, 1.0, 1.0);
		glDisable(GL_COLOR_MATERIAL);
	}
}
//------------------------------------------------------------------------

Grid::Grid(GLdouble lado, GLuint numDivisiones)
{
	mMesh = IndexMesh::generaGrid(lado, numDivisiones);
}
//------------------------------------------------------------------------

void Grid::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);

		if (mTexture == nullptr) {
			glEnable(GL_COLOR_MATERIAL);
			glColor3f(0.0, 0.0, 1.0);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
			mTexture->bind(GL_MODULATE);
		mMesh->render();

		if (mTexture == nullptr) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glColor3f(1.0, 1.0, 1.0);
			glDisable(GL_COLOR_MATERIAL);
		}
		else
			mTexture->unbind();
	}
}
//------------------------------------------------------------------------

Cono::Cono(GLdouble height, GLdouble radius, GLuint n)
{
	// h=altura del cono, r=radio de la base
	// n=n�mero de muestras, m=n�mero de puntos del perfil
	int m = 3;
	dvec3* perfil = new dvec3[m];
	perfil[0] = dvec3(0.5, 0.0, 0.0);
	perfil[1] = dvec3(radius, 0.0, 0.0);
	perfil[2] = dvec3(0.5, height, 0.0);
	this->mMesh = new MbR(m, n, perfil);
	mMesh = MbR::generaIndexMeshByRevolution(m, n, perfil);
}
//------------------------------------------------------------------------

void Cono::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		mMesh->render();
	}
}
//------------------------------------------------------------------------