#include "chunk.h"

float NYChunk::_WorldVert[CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE*3*4*6];
float NYChunk:: _WorldCols[CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE*3*4*6];
float  NYChunk::_WorldNorm[CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE*3*4*6];
float  NYChunk::_WorldUV[CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE * 2 * 4 * 6];
float  NYChunk::_WorldAttr[CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE * 1 * 4 * 6];
	