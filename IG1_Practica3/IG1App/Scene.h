//#pragma once
#ifndef _H_Scene_H_
#define _H_Scene_H_

#include <GL/freeglut.h>
#include <glm.hpp>

#include "Camera.h"
#include "Entity.h"
#include "Texture.h"

#include <vector>

using namespace std;

//-------------------------------------------------------------------------

class Scene
{
public:
	Scene() {};
	~Scene() { free(); resetGL(); };

	Scene(const Scene& s) = delete;				// no copy constructor
	Scene& operator=(const Scene& s) = delete;  // no copy assignment

	void init(int mId = 0);
	void update();
	void render(Camera const& cam) const;
	void changeScene(int const id);

protected:
	void free();
	void setGL();
	void resetGL();

	vector<Abs_Entity*> gObjectsOpaque	;  // Entities (graphic objects) of the scene
	vector<Abs_Entity*> gObjectsTrans;	   // Entities (graphic objects) translucents of the scene
	vector<Texture*> gTextures;			   // Textures for the gObjs

	static const int NUM_TEXTURES = 5;
	static const pair<std::string, int> bmps[NUM_TEXTURES];
	void loadTexture();
	int mId = 1;
};
//-------------------------------------------------------------------------

#endif //_H_Scene_H_

