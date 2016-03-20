#pragma once

#include "engine/render/renderer.h"
#include "cube.h"

/**
  * On utilise des chunks pour que si on modifie juste un cube, on ait pas
  * besoin de recharger toute la carte dans le buffer, mais juste le chunk en question
  */
class NYChunk
{
	public :

		static const int CHUNK_SIZE = 16; ///< Taille d'un chunk en nombre de cubes (n*n*n)
		NYCube _Cubes[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE]; ///< Cubes contenus dans le chunk

		GLuint _BufWorld; ///< Identifiant du VBO pour le monde
		
		static float _WorldVert[CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE*3*4*6]; ///< Buffer pour les sommets
		static float _WorldCols[CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE*3*4*6]; ///< Buffer pour les couleurs
		static float _WorldNorm[CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE*3*4*6]; ///< Buffer pour les normales
		static float _WorldUV[CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE * 2 * 4 * 6];

		static const int SIZE_VERTICE = 3 * sizeof(float); ///< Taille en octets d'un vertex dans le VBO
		static const int SIZE_COLOR = 3 * sizeof(float);  ///< Taille d'une couleur dans le VBO
		static const int SIZE_NORMAL = 3 * sizeof(float);  ///< Taille d'une normale dans le VBO
		static const int SIZE_UV = 2 * sizeof(float);

		int _NbVertices; ///< Nombre de vertices dans le VBO (on ne met que les faces visibles)

		NYChunk * Voisins[6];
		
		NYChunk()
		{
			_NbVertices = 0;
			_BufWorld = 0;
			memset(Voisins,0x00,sizeof(void*) * 6);
		}

		void setVoisins(NYChunk * xprev, NYChunk * xnext,NYChunk * yprev,NYChunk * ynext,NYChunk * zprev,NYChunk * znext)
		{
			Voisins[0] = xprev;
			Voisins[1] = xnext;
			Voisins[2] = yprev;
			Voisins[3] = ynext;
			Voisins[4] = zprev;
			Voisins[5] = znext;
		}

		/**
		  * Raz de l'état des cubes (a draw = false)
		  */
		void reset(void)
		{
			for(int x=0;x<CHUNK_SIZE;x++)
				for(int y=0;y<CHUNK_SIZE;y++)
					for(int z=0;z<CHUNK_SIZE;z++)
					{
						_Cubes[x][y][z]._Draw = true;
						_Cubes[x][y][z]._Type = CUBE_AIR;
					}
		}

		//Penser à appeler add_world_to_vbo(); a la fin du init_world()

		//Ajout d'un chunk au VBO
		void toVbo(void)
		{
			//On utilise les buffers temporaires pour préparer nos datas
			float * ptVert = _WorldVert;
			float * ptCols = _WorldCols;
			float * ptNorm = _WorldNorm;
			float * ptUV = _WorldUV;

			_NbVertices = 0;

			//On parcourt tous nos cubes
			for (int x = 0; x<CHUNK_SIZE; x++)
			{
				for (int y = 0; y<CHUNK_SIZE; y++)
				{
					for (int z = 0; z<CHUNK_SIZE; z++)
					{
						if (_Cubes[x][y][z]._Draw && _Cubes[x][y][z]._Type != CUBE_AIR)
						{
							float color[3];

							switch (_Cubes[x][y][z]._Type)
							{
							case CUBE_TERRE:
								color[0] = 101.0f / 255.0f;
								color[1] = 74.0f / 255.0f;
								color[2] = 0.0f / 255.0f;
								break;
							case CUBE_HERBE:
								color[0] = 1.0f / 255.0f;
								color[1] = 112.0f / 255.0f;
								color[2] = 12.0f / 255.0f;
								break;
							case CUBE_EAU:
								color[0] = 0.0f / 255.0f;
								color[1] = 48.0f / 255.0f;
								color[2] = 255.0f / 255.0f;
								break;
							}

							//Position du cube (coin bas gauche face avant)
							float xPos = x*(float)NYCube::CUBE_SIZE;
							float yPos = y*(float)NYCube::CUBE_SIZE;
							float zPos = z*(float)NYCube::CUBE_SIZE;

							NYCube * cubeXPrev = NULL;
							NYCube * cubeXNext = NULL;
							NYCube * cubeYPrev = NULL;
							NYCube * cubeYNext = NULL;
							NYCube * cubeZPrev = NULL;
							NYCube * cubeZNext = NULL;

							get_surrounding_cubes(
								x, y, z,
								&cubeXPrev, &cubeXNext,
								&cubeYPrev, &cubeYNext,
								&cubeZPrev, &cubeZNext);

							//Premier QUAD (le z-)
							if (cubeZPrev == NULL || cubeZPrev->isSolid() == false)
							{
								*ptVert = xPos; ptVert++;
								*ptVert = yPos; ptVert++;
								*ptVert = zPos; ptVert++; _NbVertices++;

								*ptUV = 0.0f; ptUV++;
								*ptUV = 0.0f; ptUV++;

								*ptCols = color[0] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[1] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[2] + (rand() % 100) / 2000.0f; ptCols++;

								*ptNorm = 0; ptNorm++;
								*ptNorm = 0; ptNorm++;
								*ptNorm = -1; ptNorm++;




								*ptVert = xPos + NYCube::CUBE_SIZE; ptVert++;
								*ptVert = yPos; ptVert++;
								*ptVert = zPos; ptVert++; _NbVertices++;

								*ptUV = 1.0f; ptUV++;
								*ptUV = 0.0f; ptUV++;

								*ptCols = color[0] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[1] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[2] + (rand() % 100) / 2000.0f; ptCols++;

								*ptNorm = 0; ptNorm++;
								*ptNorm = 0; ptNorm++;
								*ptNorm = -1; ptNorm++;




								*ptVert = xPos + NYCube::CUBE_SIZE; ptVert++;
								*ptVert = yPos + NYCube::CUBE_SIZE; ptVert++;
								*ptVert = zPos; ptVert++; _NbVertices++;

								*ptUV = 1.0f; ptUV++;
								*ptUV = 1.0f; ptUV++;

								*ptCols = color[0] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[1] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[2] + (rand() % 100) / 2000.0f; ptCols++;

								*ptNorm = 0; ptNorm++;
								*ptNorm = 0; ptNorm++;
								*ptNorm = -1; ptNorm++;



								*ptVert = xPos; ptVert++;
								*ptVert = yPos + NYCube::CUBE_SIZE; ptVert++;
								*ptVert = zPos; ptVert++; _NbVertices++;

								*ptUV = 0.0f; ptUV++;
								*ptUV = 1.0f; ptUV++;

								*ptCols = color[0] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[1] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[2] + (rand() % 100) / 2000.0f; ptCols++;

								*ptNorm = 0; ptNorm++;
								*ptNorm = 0; ptNorm++;
								*ptNorm = -1; ptNorm++;



							}



							//Second QUAD (x+)
							if (cubeXNext == NULL || cubeXNext->isSolid() == false)
							{
								*ptVert = xPos + NYCube::CUBE_SIZE; ptVert++;
								*ptVert = yPos; ptVert++;
								*ptVert = zPos; ptVert++; _NbVertices++;

								*ptUV = 0.0f; ptUV++;
								*ptUV = 0.0f; ptUV++;

								*ptCols = color[0] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[1] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[2] + (rand() % 100) / 2000.0f; ptCols++;

								*ptNorm = 1; ptNorm++;
								*ptNorm = 0; ptNorm++;
								*ptNorm = 0; ptNorm++;



								*ptVert = xPos + NYCube::CUBE_SIZE; ptVert++;
								*ptVert = yPos + NYCube::CUBE_SIZE; ptVert++;
								*ptVert = zPos; ptVert++; _NbVertices++;

								*ptUV = 1.0f; ptUV++;
								*ptUV = 0.0f; ptUV++;

								*ptCols = color[0] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[1] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[2] + (rand() % 100) / 2000.0f; ptCols++;

								*ptNorm = 1; ptNorm++;
								*ptNorm = 0; ptNorm++;
								*ptNorm = 0; ptNorm++;



								*ptVert = xPos + NYCube::CUBE_SIZE; ptVert++;
								*ptVert = yPos + NYCube::CUBE_SIZE; ptVert++;
								*ptVert = zPos + NYCube::CUBE_SIZE; ptVert++; _NbVertices++;

								*ptUV = 1.0f; ptUV++;
								*ptUV = 1.0f; ptUV++;

								*ptCols = color[0] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[1] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[2] + (rand() % 100) / 2000.0f; ptCols++;

								*ptNorm = 1; ptNorm++;
								*ptNorm = 0; ptNorm++;
								*ptNorm = 0; ptNorm++;




								*ptVert = xPos + NYCube::CUBE_SIZE; ptVert++;
								*ptVert = yPos; ptVert++;
								*ptVert = zPos + NYCube::CUBE_SIZE; ptVert++; _NbVertices++;

								*ptUV = 0.0f; ptUV++;
								*ptUV = 1.0f; ptUV++;

								*ptCols = color[0] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[1] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[2] + (rand() % 100) / 2000.0f; ptCols++;

								*ptNorm = 1; ptNorm++;
								*ptNorm = 0; ptNorm++;
								*ptNorm = 0; ptNorm++;
							}

							//Troisieme QUAD (x-)
							if (cubeXPrev == NULL || cubeXPrev->isSolid() == false)
							{
								*ptVert = xPos; ptVert++;
								*ptVert = yPos; ptVert++;
								*ptVert = zPos; ptVert++; _NbVertices++;

								*ptUV = 0.0f; ptUV++;
								*ptUV = 0.0f; ptUV++;

								*ptCols = color[0] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[1] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[2] + (rand() % 100) / 2000.0f; ptCols++;

								*ptNorm = -1; ptNorm++;
								*ptNorm = 0; ptNorm++;
								*ptNorm = 0; ptNorm++;



								*ptVert = xPos; ptVert++;
								*ptVert = yPos; ptVert++;
								*ptVert = zPos + NYCube::CUBE_SIZE; ptVert++; _NbVertices++;

								*ptUV = 0.0f; ptUV++;
								*ptUV = 1.0f; ptUV++;

								*ptCols = color[0] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[1] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[2] + (rand() % 100) / 2000.0f; ptCols++;

								*ptNorm = -1; ptNorm++;
								*ptNorm = 0; ptNorm++;
								*ptNorm = 0; ptNorm++;



								*ptVert = xPos; ptVert++;
								*ptVert = yPos + NYCube::CUBE_SIZE; ptVert++;
								*ptVert = zPos + NYCube::CUBE_SIZE; ptVert++; _NbVertices++;

								*ptUV = 1.0f; ptUV++;
								*ptUV = 1.0f; ptUV++;

								*ptCols = color[0] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[1] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[2] + (rand() % 100) / 2000.0f; ptCols++;

								*ptNorm = -1; ptNorm++;
								*ptNorm = 0; ptNorm++;
								*ptNorm = 0; ptNorm++;





								*ptVert = xPos; ptVert++;
								*ptVert = yPos + NYCube::CUBE_SIZE; ptVert++;
								*ptVert = zPos; ptVert++; _NbVertices++;

								*ptUV = 1.0f; ptUV++;
								*ptUV = 0.0f; ptUV++;

								*ptCols = color[0] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[1] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[2] + (rand() % 100) / 2000.0f; ptCols++;

								*ptNorm = -1; ptNorm++;
								*ptNorm = 0; ptNorm++;
								*ptNorm = 0; ptNorm++;
							}


							//Quatrieme QUAD (y+)
							if (cubeYNext == NULL || cubeYNext->isSolid() == false)
							{
								*ptVert = xPos; ptVert++;
								*ptVert = yPos + NYCube::CUBE_SIZE; ptVert++;
								*ptVert = zPos; ptVert++; _NbVertices++;

								*ptUV = 0.0f; ptUV++;
								*ptUV = 0.0f; ptUV++;

								*ptCols = color[0] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[1] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[2] + (rand() % 100) / 2000.0f; ptCols++;

								*ptNorm = 0; ptNorm++;
								*ptNorm = 1; ptNorm++;
								*ptNorm = 0; ptNorm++;



								*ptVert = xPos; ptVert++;
								*ptVert = yPos + NYCube::CUBE_SIZE; ptVert++;
								*ptVert = zPos + NYCube::CUBE_SIZE; ptVert++; _NbVertices++;

								*ptUV = 0.0f; ptUV++;
								*ptUV = 1.0f; ptUV++;

								*ptCols = color[0] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[1] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[2] + (rand() % 100) / 2000.0f; ptCols++;

								*ptNorm = 0; ptNorm++;
								*ptNorm = 1; ptNorm++;
								*ptNorm = 0; ptNorm++;



								*ptVert = xPos + NYCube::CUBE_SIZE; ptVert++;
								*ptVert = yPos + NYCube::CUBE_SIZE; ptVert++;
								*ptVert = zPos + NYCube::CUBE_SIZE; ptVert++; _NbVertices++;

								*ptUV = 1.0f; ptUV++;
								*ptUV = 1.0f; ptUV++;

								*ptCols = color[0] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[1] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[2] + (rand() % 100) / 2000.0f; ptCols++;

								*ptNorm = 0; ptNorm++;
								*ptNorm = 1; ptNorm++;
								*ptNorm = 0; ptNorm++;





								*ptVert = xPos + NYCube::CUBE_SIZE; ptVert++;
								*ptVert = yPos + NYCube::CUBE_SIZE; ptVert++;
								*ptVert = zPos; ptVert++; _NbVertices++;

								*ptUV = 1.0f; ptUV++;
								*ptUV = 0.0f; ptUV++;

								*ptCols = color[0] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[1] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[2] + (rand() % 100) / 2000.0f; ptCols++;

								*ptNorm = 0; ptNorm++;
								*ptNorm = 1; ptNorm++;
								*ptNorm = 0; ptNorm++;
							}

							//Cinquieme QUAD (y-)
							if (cubeYPrev == NULL || cubeYPrev->isSolid() == false)
							{
								*ptVert = xPos; ptVert++;
								*ptVert = yPos; ptVert++;
								*ptVert = zPos; ptVert++; _NbVertices++;

								*ptUV = 0.0f; ptUV++;
								*ptUV = 0.0f; ptUV++;

								*ptCols = color[0] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[1] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[2] + (rand() % 100) / 2000.0f; ptCols++;

								*ptNorm = 0; ptNorm++;
								*ptNorm = -1; ptNorm++;
								*ptNorm = 0; ptNorm++;



								*ptVert = xPos + NYCube::CUBE_SIZE; ptVert++;
								*ptVert = yPos; ptVert++;
								*ptVert = zPos; ptVert++; _NbVertices++;

								*ptUV = 1.0f; ptUV++;
								*ptUV = 0.0f; ptUV++;

								*ptCols = color[0] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[1] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[2] + (rand() % 100) / 2000.0f; ptCols++;

								*ptNorm = 0; ptNorm++;
								*ptNorm = -1; ptNorm++;
								*ptNorm = 0; ptNorm++;



								*ptVert = xPos + NYCube::CUBE_SIZE; ptVert++;
								*ptVert = yPos; ptVert++;
								*ptVert = zPos + NYCube::CUBE_SIZE; ptVert++; _NbVertices++;

								*ptUV = 1.0f; ptUV++;
								*ptUV = 1.0f; ptUV++;

								*ptCols = color[0] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[1] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[2] + (rand() % 100) / 2000.0f; ptCols++;

								*ptNorm = 0; ptNorm++;
								*ptNorm = -1; ptNorm++;
								*ptNorm = 0; ptNorm++;




								*ptVert = xPos; ptVert++;
								*ptVert = yPos; ptVert++;
								*ptVert = zPos + NYCube::CUBE_SIZE; ptVert++; _NbVertices++;

								*ptUV = 0.0f; ptUV++;
								*ptUV = 1.0f; ptUV++;

								*ptCols = color[0] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[1] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[2] + (rand() % 100) / 2000.0f; ptCols++;

								*ptNorm = 0; ptNorm++;
								*ptNorm = -1; ptNorm++;
								*ptNorm = 0; ptNorm++;
							}

							//Sixieme QUAD (z+)
							if (cubeZNext == NULL || cubeZNext->isSolid() == false)
							{
								*ptVert = xPos; ptVert++;
								*ptVert = yPos; ptVert++;
								*ptVert = zPos + NYCube::CUBE_SIZE; ptVert++; _NbVertices++;

								*ptUV = 0.0f; ptUV++;
								*ptUV = 0.0f; ptUV++;

								*ptCols = color[0] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[1] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[2] + (rand() % 100) / 2000.0f; ptCols++;

								*ptNorm = 0; ptNorm++;
								*ptNorm = 0; ptNorm++;
								*ptNorm = 1; ptNorm++;



								*ptVert = xPos + NYCube::CUBE_SIZE; ptVert++;
								*ptVert = yPos; ptVert++;
								*ptVert = zPos + NYCube::CUBE_SIZE; ptVert++; _NbVertices++;

								*ptUV = 1.0f; ptUV++;
								*ptUV = 0.0f; ptUV++;

								*ptCols = color[0] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[1] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[2] + (rand() % 100) / 2000.0f; ptCols++;

								*ptNorm = 0; ptNorm++;
								*ptNorm = 0; ptNorm++;
								*ptNorm = 1; ptNorm++;



								*ptVert = xPos + NYCube::CUBE_SIZE; ptVert++;
								*ptVert = yPos + NYCube::CUBE_SIZE; ptVert++;
								*ptVert = zPos + NYCube::CUBE_SIZE; ptVert++; _NbVertices++;

								*ptUV = 1.0f; ptUV++;
								*ptUV = 1.0f; ptUV++;

								*ptCols = color[0] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[1] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[2] + (rand() % 100) / 2000.0f; ptCols++;

								*ptNorm = 0; ptNorm++;
								*ptNorm = 0; ptNorm++;
								*ptNorm = 1; ptNorm++;



								*ptVert = xPos; ptVert++;
								*ptVert = yPos + NYCube::CUBE_SIZE; ptVert++;
								*ptVert = zPos + NYCube::CUBE_SIZE; ptVert++; _NbVertices++;

								*ptUV = 0.0f; ptUV++;
								*ptUV = 1.0f; ptUV++;

								*ptCols = color[0] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[1] + (rand() % 100) / 2000.0f; ptCols++;
								*ptCols = color[2] + (rand() % 100) / 2000.0f; ptCols++;

								*ptNorm = 0; ptNorm++;
								*ptNorm = 0; ptNorm++;
								*ptNorm = 1; ptNorm++;
							}
						}
					}
				}
			}

			if (_BufWorld != 0)
				glDeleteBuffers(1, &_BufWorld);

			glGenBuffers(1, &_BufWorld);

			glBindBuffer(GL_ARRAY_BUFFER, _BufWorld);

			glBufferData(GL_ARRAY_BUFFER,
				_NbVertices * SIZE_VERTICE +
				_NbVertices * SIZE_COLOR +
				_NbVertices * SIZE_NORMAL +
				_NbVertices * SIZE_UV,
				NULL,
				GL_STATIC_DRAW);

			GLenum error = glGetError();

			glBufferSubData(GL_ARRAY_BUFFER,
				0,
				_NbVertices * SIZE_VERTICE,
				_WorldVert);

			error = glGetError();

			glBufferSubData(GL_ARRAY_BUFFER,
				_NbVertices * SIZE_VERTICE,
				_NbVertices * SIZE_COLOR,
				_WorldCols);

			error = glGetError();

			glBufferSubData(GL_ARRAY_BUFFER,
				_NbVertices * SIZE_VERTICE +
				_NbVertices * SIZE_COLOR,
				_NbVertices * SIZE_NORMAL,
				_WorldNorm);

			glBufferSubData(GL_ARRAY_BUFFER,
				_NbVertices * SIZE_VERTICE +
				_NbVertices * SIZE_COLOR +
				_NbVertices * SIZE_NORMAL,
				_NbVertices * SIZE_UV,
				_WorldUV);

			error = glGetError();

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void render(void)
		{
			glEnable(GL_COLOR_MATERIAL);
			glEnable(GL_LIGHTING);

			//On bind le buuffer
			glBindBuffer(GL_ARRAY_BUFFER, _BufWorld);
			NYRenderer::checkGlError("glBindBuffer");

			//On active les datas que contiennent le VBO
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			//On place les pointeurs sur les datas, aux bons offsets
			glVertexPointer(3, GL_FLOAT, 0, (void*)(0));
			glColorPointer(3, GL_FLOAT, 0, (void*)(_NbVertices*SIZE_VERTICE));
			glNormalPointer(GL_FLOAT, 0, (void*)(_NbVertices*SIZE_VERTICE + _NbVertices*SIZE_COLOR));
			glTexCoordPointer(2, GL_FLOAT, 0, (void*)(_NbVertices*SIZE_VERTICE + _NbVertices*SIZE_COLOR + _NbVertices*SIZE_NORMAL));

			//On demande le dessin
			//glDrawArrays(GL_TRIANGLES, 0, _NbVertices);
			glDrawArrays(GL_QUADS, 0, _NbVertices);

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_COLOR_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);

			glDisable(GL_COLOR_MATERIAL);
			glDisable(GL_LIGHTING);
		}

		void get_surrounding_cubes(int x, int y, int z, NYCube ** cubeXPrev, NYCube ** cubeXNext,
			NYCube ** cubeYPrev, NYCube ** cubeYNext,
			NYCube ** cubeZPrev, NYCube ** cubeZNext)
		{

			*cubeXPrev = NULL;
			*cubeXNext = NULL;
			*cubeYPrev = NULL;
			*cubeYNext = NULL;
			*cubeZPrev = NULL;
			*cubeZNext = NULL;

			if (x == 0 && Voisins[0] != NULL)
				*cubeXPrev = &(Voisins[0]->_Cubes[CHUNK_SIZE - 1][y][z]);
			else if (x > 0)
				*cubeXPrev = &(_Cubes[x - 1][y][z]);

			if (x == CHUNK_SIZE - 1 && Voisins[1] != NULL)
				*cubeXNext = &(Voisins[1]->_Cubes[0][y][z]);
			else if (x < CHUNK_SIZE - 1)
				*cubeXNext = &(_Cubes[x + 1][y][z]);

			if (y == 0 && Voisins[2] != NULL)
				*cubeYPrev = &(Voisins[2]->_Cubes[x][CHUNK_SIZE - 1][z]);
			else if (y > 0)
				*cubeYPrev = &(_Cubes[x][y - 1][z]);

			if (y == CHUNK_SIZE - 1 && Voisins[3] != NULL)
				*cubeYNext = &(Voisins[3]->_Cubes[x][0][z]);
			else if (y < CHUNK_SIZE - 1)
				*cubeYNext = &(_Cubes[x][y + 1][z]);

			if (z == 0 && Voisins[4] != NULL)
				*cubeZPrev = &(Voisins[4]->_Cubes[x][y][CHUNK_SIZE - 1]);
			else if (z > 0)
				*cubeZPrev = &(_Cubes[x][y][z - 1]);

			if (z == CHUNK_SIZE - 1 && Voisins[5] != NULL)
				*cubeZNext = &(Voisins[5]->_Cubes[x][y][0]);
			else if (z < CHUNK_SIZE - 1)
				*cubeZNext = &(_Cubes[x][y][z + 1]);
		}

		/**
		  * On verifie si le cube peut être vu
		  */
		bool test_hidden(int x, int y, int z)
		{
			NYCube * cubeXPrev = NULL; 
			NYCube * cubeXNext = NULL; 
			NYCube * cubeYPrev = NULL; 
			NYCube * cubeYNext = NULL; 
			NYCube * cubeZPrev = NULL; 
			NYCube * cubeZNext = NULL; 

			get_surrounding_cubes(
				x, y, z,
				&cubeXPrev, &cubeXNext,
				&cubeYPrev, &cubeYNext,
				&cubeZPrev, &cubeZNext);

			if( cubeXPrev == NULL || cubeXNext == NULL ||
				cubeYPrev == NULL || cubeYNext == NULL ||
				cubeZPrev == NULL || cubeZNext == NULL )
				return false;

			if( cubeXPrev->isSolid() == true && //droite
				cubeXNext->isSolid() == true && //gauche
				cubeYPrev->isSolid() == true && //haut
				cubeYNext->isSolid() == true && //bas
				cubeZPrev->isSolid() == true && //devant
				cubeZNext->isSolid() == true )  //derriere
				return true;
			return false;
		}

		void disableHiddenCubes(void)
		{
			for(int x=0;x<CHUNK_SIZE;x++)
				for(int y=0;y<CHUNK_SIZE;y++)
					for(int z=0;z<CHUNK_SIZE;z++)
					{
						_Cubes[x][y][z]._Draw = true;
						if(test_hidden(x,y,z))
							_Cubes[x][y][z]._Draw = false;
					}
		}


};