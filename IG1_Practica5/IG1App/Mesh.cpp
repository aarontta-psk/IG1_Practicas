#include "Mesh.h"
#include "CheckML.h"
#include <fstream>
using namespace std;
using namespace glm;

//-------------------------------------------------------------------------

void Mesh::draw() const
{
	glDrawArrays(mPrimitive, 0, size()); // primitive graphic, first index and number of elements to be rendered
}
//-------------------------------------------------------------------------

void Mesh::render() const
{
	if (vVertices.size() > 0) { // transfer data
								// transfer the coordinates of the vertices
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_DOUBLE, 0, vVertices.data());   // number of coordinates per vertex, type of each coordinate, stride, pointer 
		if (vColors.size() > 0) {    // transfer colors
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(4, GL_DOUBLE, 0, vColors.data());  // components number (rgba=4), type of each component, stride, pointer  
		}
		if (vTexCoords.size() > 0) { // transfer texture
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_DOUBLE, 0, vTexCoords.data());
		}
		if (vNormals.size() > 0) {
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(GL_DOUBLE, 0, vNormals.data());
		}

		draw();

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
	}
}
//-------------------------------------------------------------------------

Mesh* Mesh::createRGBAxes(GLdouble l)
{
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_LINES;

	mesh->mNumVertices = 6;
	mesh->vVertices.reserve(mesh->mNumVertices);

	// X axis vertices
	mesh->vVertices.emplace_back(0.0, 0.0, 0.0);
	mesh->vVertices.emplace_back(l, 0.0, 0.0);
	// Y axis vertices
	mesh->vVertices.emplace_back(0, 0.0, 0.0);
	mesh->vVertices.emplace_back(0.0, l, 0.0);
	// Z axis vertices
	mesh->vVertices.emplace_back(0.0, 0.0, 0.0);
	mesh->vVertices.emplace_back(0.0, 0.0, l);

	mesh->vColors.reserve(mesh->mNumVertices);
	// X axis color: red  (Alpha = 1 : fully opaque)
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	// Y axis color: green
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	// Z axis color: blue
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);

	return mesh;
}
//-------------------------------------------------------------------------

Mesh* Mesh::generaPoligono(GLuint numL, GLdouble rd) {
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_LINE_LOOP;

	mesh->mNumVertices = numL;
	mesh->vVertices.reserve(mesh->mNumVertices);
	GLdouble angle = 90; //angulo

	for (GLuint counter = 0; counter != numL; counter++) {
		mesh->vVertices.emplace_back(rd * cos(radians(angle)), rd * sin(radians(angle)), 0.0);
		angle += 360.0 / numL; // incrementamos el angulo del siguiente vertice
	}

	return mesh;
}
//-------------------------------------------------------------------------

Mesh* Mesh::generaSierpinski(GLuint numP, GLdouble rd) {
	Mesh* triangle = generaPoligono(3, rd);
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_POINTS;

	mesh->mNumVertices = numP;
	mesh->vVertices.reserve(mesh->mNumVertices);

	mesh->vVertices.emplace_back(triangle->vVertices[rand() % 3]);
	glm::dvec3 pAux;

	for (GLuint counter = 1; counter != numP; counter++) {
		pAux = triangle->vVertices[rand() % 3];
		mesh->vVertices.emplace_back((mesh->vVertices.back() + pAux) / 2.0);
	}

	delete triangle; triangle = nullptr;

	return mesh;
}
//-------------------------------------------------------------------------

Mesh* Mesh::generaTrianguloRGB(GLdouble rd) {
	Mesh* mesh = generaPoligono(3, rd);

	mesh->mPrimitive = GL_TRIANGLES;

	mesh->vColors.reserve(mesh->mNumVertices);
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);

	return mesh;
}
//-------------------------------------------------------------------------

Mesh* Mesh::generaRectangulo(GLdouble w, GLdouble h) {
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_TRIANGLE_STRIP;

	mesh->mNumVertices = 4;
	mesh->vVertices.reserve(mesh->mNumVertices);
	mesh->vVertices.emplace_back(-w / 2.0, h / 2.0, 0.0);  // v0 arriba izquierda
	mesh->vVertices.emplace_back(-w / 2.0, -h / 2.0, 0.0); // v1 abajo izquierda
	mesh->vVertices.emplace_back(w / 2.0, h / 2.0, 0.0);   // v2 arriba derecha
	mesh->vVertices.emplace_back(w / 2.0, -h / 2.0, 0.0);  // v3 abajo derecha

	return mesh;
}
//-------------------------------------------------------------------------

Mesh* Mesh::generaRectanguloRGB(GLdouble w, GLdouble h) {
	Mesh* mesh = generaRectangulo(w, h);

	mesh->vColors.reserve(mesh->mNumVertices);
	mesh->vColors.emplace_back(1.0, 0.0, 0.75, 1.0); // rosa
	mesh->vColors.emplace_back(0.5, 1.0, 0.25, 1.0); // azul claro
	mesh->vColors.emplace_back(0.25, 0.5, 1.0, 1.0); // verde muy claro
	mesh->vColors.emplace_back(1.0, 0.8, 0.0, 1.0);  // dorado

	return mesh;
}
//-------------------------------------------------------------------------

Mesh* Mesh::generaEstrella3D(GLdouble re, GLuint np, GLdouble h) {
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_TRIANGLE_FAN;

	mesh->mNumVertices = 2 * np + 2;
	mesh->vVertices.reserve(mesh->mNumVertices);

	mesh->vVertices.emplace_back(0.0, 0.0, 0.0);

	GLdouble angle = 90.0;

	for (int i = 0; i < mesh->mNumVertices - 1; i++) {
		if (i % 2 == 0)
			mesh->vVertices.emplace_back(re * cos(radians(angle)), re * sin(radians(angle)), h);
		else
			mesh->vVertices.emplace_back(re / 2 * cos(radians(angle)), re / 2 * sin(radians(angle)), h);

		angle += 360.0 / (np * 2.0);
	}

	return mesh;
}
//-------------------------------------------------------------------------

Mesh* Mesh::generaContCubo(GLdouble ld) {
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_TRIANGLE_STRIP;

	mesh->mNumVertices = 10;
	mesh->vVertices.reserve(mesh->mNumVertices);

	GLdouble x, y, z;

	for (int i = 0; i < mesh->mNumVertices; i++) {
		if (i > 1 && i < 6) x = ld / 2;
		else x = -ld / 2;

		if (i % 2 == 0) y = ld / 2;
		else y = -ld / 2;

		if (i > 3 && i < 8) z = -ld / 2;
		else z = ld / 2;

		mesh->vVertices.emplace_back(x, y, z);
	}

	return mesh;
}
//-------------------------------------------------------------------------

Mesh* Mesh::generaRectanguloTexCor(GLdouble w, GLdouble h, GLuint rw, GLuint rh) {
	Mesh* mesh = generaRectangulo(w, h);

	mesh->vTexCoords.reserve(mesh->mNumVertices);
	mesh->vTexCoords.emplace_back(0.0, rh);  // v0
	mesh->vTexCoords.emplace_back(0.0, 0.0); // v1
	mesh->vTexCoords.emplace_back(rw, rh);   // v2
	mesh->vTexCoords.emplace_back(rw, 0.0);  // v3

	return mesh;
}
//-------------------------------------------------------------------------

Mesh* Mesh::generaEstrellaTexCor(GLdouble re, GLuint np, GLdouble h) {
	Mesh* mesh = generaEstrella3D(re, np, h);

	mesh->vTexCoords.reserve(mesh->mNumVertices);
	mesh->vTexCoords.emplace_back(0.5, 0.5);

	GLdouble angle = 90.0;

	for (int i = 0; i < mesh->mNumVertices - 1; i++) {
		mesh->vTexCoords.emplace_back(0.5 + 0.5 * cos(radians(angle)), 0.5 + 0.5 * sin(radians(angle)));
		angle += 360.0 / (np * 2.0);
	}

	return mesh;
}
//-------------------------------------------------------------------------

Mesh* Mesh::generaContCuboTexCor(GLdouble nl) {
	Mesh* mesh = generaContCubo(nl);

	mesh->vTexCoords.reserve(mesh->mNumVertices);

	for (int i = 0; i < mesh->mNumVertices; i++) {
		mesh->vTexCoords.emplace_back(GLdouble(i / 2), i % 2 == 0 ? 1.0 : 0.0);
	}

	return mesh;
}
//-------------------------------------------------------------------------

Mesh* Mesh::generaHexagonoTexCor(GLdouble radio)
{
	Mesh* auxMesh = Mesh::generaPoligono(6, radio);
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_TRIANGLE_FAN;
	mesh->mNumVertices = 8;
	mesh->vVertices.reserve(mesh->mNumVertices);
	mesh->vVertices.emplace_back(0, 0, 0);

	for (int i = 5; i >= 0; i--) mesh->vVertices.emplace_back(auxMesh->vVertices[i]);

	mesh->vVertices.emplace_back(auxMesh->vVertices[5]);

	mesh->vTexCoords.reserve(mesh->mNumVertices);
	mesh->vTexCoords.emplace_back(0.5, 0.5);

	GLdouble angle = 90.0;

	for (int i = 0; i < mesh->mNumVertices - 1; i++) {
		mesh->vTexCoords.emplace_back(0.5 + 0.5 * cos(radians(angle)), 0.5 + 0.5 * sin(radians(angle)));
		angle += 360.0 / 6;
	}

	delete auxMesh;
	auxMesh = nullptr;
	return mesh;
}
//-------------------------------------------------------------------------

IndexMesh* IndexMesh::generaGrid(GLdouble lado, GLuint numDiv)
{
	// Grid cuadrado de lado*lado, centrado en el plano Y=0,
	// dividido en numDiv*numDiv celdas (cada celda son 2 tri�ngulos)
	IndexMesh* indexMesh = new IndexMesh();
	GLdouble incr = lado / numDiv; // incremento para la coordenada x, y la c. z
	GLuint numFC = numDiv + 1;	   // n�mero de v�rtices por filas y columnas

	// Generaci�n de v�rtices
	indexMesh->mNumVertices = numFC * numFC;
	indexMesh->vVertices.reserve(indexMesh->mNumVertices);

	for (int i = 0; i < indexMesh->mNumVertices; i++)
		indexMesh->vVertices.emplace_back(dvec3());

	double z = -lado / 2;
	double x = -lado / 2;
	for (int f = 0; f < numFC; f++) {
		for (int c = 0; c < numFC; c++) {
			int indice = f * numFC + c;
			indexMesh->vVertices[indice] = dvec3(x + c * incr, 0, z + f * incr);
		}
	}

	// generacion de texturas
	indexMesh->vTexCoords.reserve(indexMesh->mNumVertices);

	for (int i = 0; i < indexMesh->mNumVertices; i++)
		indexMesh->vTexCoords.emplace_back(dvec2());

	GLuint s = 0;
	GLuint t = 1;
	for (int f = 0; f < numFC; f++) {
		for (int c = 0; c < numFC; c++) {
			int indice = f * numFC + c;
			indexMesh->vTexCoords[indice] = dvec2(s + (double)c / numDiv, t - (double)f / numDiv);
		}
	}

	// Generaci�n de �ndices
	indexMesh->nNumIndices = numDiv * numDiv * 6;
	indexMesh->vIndices = new GLuint[indexMesh->nNumIndices];

	GLuint i = 0;
	for (int h = 0; h < numDiv; h++) {
		for (int k = 0; k < numDiv; k++) {
			int iv = h * numFC + k;
			indexMesh->vIndices[i++] = iv;
			indexMesh->vIndices[i++] = iv + numFC;
			indexMesh->vIndices[i++] = iv + 1;

			indexMesh->vIndices[i++] = iv + 1;
			indexMesh->vIndices[i++] = iv + numFC;
			indexMesh->vIndices[i++] = iv + numFC + 1;
		}
	}

	// normales
	indexMesh->buildNormalVectors();
	return indexMesh;
}
//-------------------------------------------------------------------------

void IndexMesh::render() const
{
	if (vVertices.size() > 0) { // transfer data
								// transfer the coordinates of the vertices
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_DOUBLE, 0, vVertices.data());   // number of coordinates per vertex, type of each coordinate, stride, pointer 
		if (vColors.size() > 0) {    // transfer colors
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(4, GL_DOUBLE, 0, vColors.data());  // components number (rgba=4), type of each component, stride, pointer  
		}
		if (vTexCoords.size() > 0) { // transfer texture
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_DOUBLE, 0, vTexCoords.data());
		}
		if (vNormals.size() > 0) {
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(GL_DOUBLE, 0, vNormals.data());
		}
		if (vIndices != nullptr) { // transfer texture
			glEnableClientState(GL_INDEX_ARRAY);
			glIndexPointer(GL_UNSIGNED_INT, 0, vIndices);
		}

		draw();

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_INDEX_ARRAY);
	}
}
//-------------------------------------------------------------------------

void IndexMesh::draw() const
{
	glDrawElements(mPrimitive, nNumIndices, GL_UNSIGNED_INT, vIndices);
}
//-------------------------------------------------------------------------

void IndexMesh::buildNormalVectors()
{
	vNormals.reserve(mNumVertices);

	for (int i = 0; i < mNumVertices; i++)
		vNormals.emplace_back(0, 0, 0);

	for (int i = 0; i < nNumIndices; i += 3)
	{
		dvec3 a, b, c, n;
		a = vVertices[vIndices[i]];
		b = vVertices[vIndices[i + 1]];
		c = vVertices[vIndices[i + 2]];
		n = cross(b - a, c - a);

		vNormals[vIndices[i]] += n;
		vNormals[vIndices[i + 1]] += n;
		vNormals[vIndices[i + 2]] += n;
	}

	for (int i = 0; i < mNumVertices; i++)
		vNormals[i] = normalize(vNormals[i]);
}
//-------------------------------------------------------------------------

IndexMesh* IndexMesh::generaAnilloCuadradoIndexado()
{
	IndexMesh* iMesh = new IndexMesh();

	iMesh->mPrimitive = GL_TRIANGLE_STRIP;

	iMesh->mNumVertices = 8;
	iMesh->vVertices.reserve(iMesh->mNumVertices);

	iMesh->vVertices.emplace_back(30.0, 30.0, 0.0);
	iMesh->vVertices.emplace_back(10.0, 10.0, 0.0);
	iMesh->vVertices.emplace_back(70.0, 30.0, 0.0);
	iMesh->vVertices.emplace_back(90.0, 10.0, 0.0);
	iMesh->vVertices.emplace_back(70.0, 70.0, 0.0);
	iMesh->vVertices.emplace_back(90.0, 90.0, 0.0);
	iMesh->vVertices.emplace_back(30.0, 70.0, 0.0);
	iMesh->vVertices.emplace_back(10.0, 90.0, 0.0);

	iMesh->vColors.reserve(iMesh->mNumVertices);

	iMesh->vColors.emplace_back(0.0, 0.0, 0.0, 1.0);
	iMesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	iMesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	iMesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
	iMesh->vColors.emplace_back(1.0, 1.0, 0.0, 1.0);
	iMesh->vColors.emplace_back(1.0, 0.0, 1.0, 1.0);
	iMesh->vColors.emplace_back(0.0, 1.0, 1.0, 1.0);
	iMesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);

	iMesh->nNumIndices = 10;
	iMesh->vIndices = new GLuint[iMesh->nNumIndices];
	for (int i = 0; i < iMesh->nNumIndices; i++)
		iMesh->vIndices[i] = i % 8;

	//Normales
	iMesh->vNormals.reserve(iMesh->mNumVertices);

	for (int i = 0; i < iMesh->mNumVertices; i++)
		iMesh->vNormals.emplace_back(0, 0, 0);

	dvec3 a, b, c, n;
	a = iMesh->vVertices[iMesh->vIndices[0]];
	b = iMesh->vVertices[iMesh->vIndices[1]];
	c = iMesh->vVertices[iMesh->vIndices[2]];

	n = cross(b - a, c - a);

	for (int i = 0; i < iMesh->mNumVertices; i++)
		iMesh->vNormals[i] = normalize(n);

	return iMesh;
}
//-------------------------------------------------------------------------

IndexMesh* IndexMesh::generaCuboConTapasIndexado(GLdouble l)
{
	IndexMesh* iMesh = new IndexMesh();

	iMesh->mPrimitive = GL_TRIANGLES;

	iMesh->mNumVertices = 8;
	iMesh->vVertices.reserve(iMesh->mNumVertices);

	GLdouble x, y, z;

	for (int i = 0; i < iMesh->mNumVertices; i++) {
		if (i > 1 && i < 6) x = l / 2;
		else x = -l / 2;

		if (i % 2 == 0) y = l / 2;
		else y = -l / 2;

		if (i > 3 && i < 8) z = -l / 2;
		else z = l / 2;

		iMesh->vVertices.emplace_back(x, y, z);
	}

	//Indices
	iMesh->nNumIndices = 36;
	iMesh->vIndices = new GLuint[iMesh->nNumIndices]{ 0,3,2,	  //Cara delantera
									 0,1,3,
									 2,3,4,	  //Cara derecha
									 4,3,5,
									 4,5,6,	  //Cara trasera
									 6,5,7,
									 6,7,1,	  //Cara izquierda
									 1,0,6,
									 6,0,4,	  //Cara arriba
									 4,0,2,
									 7,1,5,	  //Cara abajo
									 5,1,3 };


	//Normales
	iMesh->buildNormalVectors();

	return iMesh;
}
//-------------------------------------------------------------------------

MbR::MbR(int m, int n, glm::dvec3* perfil)
{
	this->m = m;
	this->n = n;
	this->perfil = perfil;
}
//-------------------------------------------------------------------------

MbR* MbR::generaIndexMeshByRevolution(int mm, int nn, glm::dvec3* perfil)
{
	MbR* mesh = new MbR(mm, nn, perfil);

	// Definir la primitiva como GL_TRIANGLES
	mesh->mPrimitive = GL_TRIANGLES;
	// Definir el n�mero de v�rtices como nn*mm
	mesh->mNumVertices = nn * mm;
	// Reservamos espacio
	mesh->vVertices.reserve(mesh->mNumVertices);

	for (int i = 0; i < mesh->mNumVertices; i++)
		mesh->vVertices.emplace_back(0, 0, 0);

	for (int i = 0; i < nn; i++)
	{
		// Generar la muestra i-�sima de v�rtices
		GLdouble theta = i * 360 / nn;
		GLdouble c = cos(radians(theta));
		GLdouble s = sin(radians(theta));

		// R_y(theta) es la matriz de rotaci�n alrededor del eje Y
		for (int j = 0; j < mm; j++) {
			int indice = i * mm + j;
			GLdouble x = c * perfil[j].x + s * perfil[j].z;
			GLdouble z = -s * perfil[j].x + c * perfil[j].z;
			mesh->vVertices[indice] = dvec3(x, perfil[j].y, z);
		}
	}

	//Determinar indices caras cuadrangulares
	mesh->nNumIndices = 6 * nn * mm - 6 * nn;
	mesh->vIndices = new GLuint[mesh->nNumIndices];
	// El contador i recorre las muestras alrededor del eje Y
	int indiceMayor = 0;
	for (int i = 0; i < nn; i++)
	{
		// El contador j recorre los v�rtices del perfil,
		// de abajo arriba. Las caras cuadrangulares resultan
		// al unir la muestra i-�sima con la (i+1)%nn-�sima
		for (int j = 0; j < mm - 1; j++)
		{
			// El contador indice sirve para llevar cuenta
			// de los �ndices generados hasta ahora. Se recorre
			// la cara desde la esquina inferior izquierda
			int indice = i * mm + j;
			// Los cuatro �ndices son entonces:
			//indice, (indice + mm) % (nn * mm), (indice + mm + 1) % (nn * mm), indice + 1
			mesh->vIndices[indiceMayor] = indice;
			indiceMayor++;
			mesh->vIndices[indiceMayor] = (indice + mm) % (nn * mm);
			indiceMayor++;
			mesh->vIndices[indiceMayor] = (indice + mm + 1) % (nn * mm);
			indiceMayor++;

			mesh->vIndices[indiceMayor] = (indice + mm + 1) % (nn * mm);
			indiceMayor++;
			mesh->vIndices[indiceMayor] = indice + 1;
			indiceMayor++;
			mesh->vIndices[indiceMayor] = indice;
			indiceMayor++;
		}
	}

	// Vectores normales
	mesh->buildNormalVectors();
	return mesh;
}
//-------------------------------------------------------------------------
