#include "MarchingCubesDataInterface.h"
#include "MarchingCubes.h"
#include <osg/vec3>

int CubeEdgeFlags[256]=
{
	0x000, 0x109, 0x203, 0x30a, 0x406, 0x50f, 0x605, 0x70c, 0x80c, 0x905, 0xa0f, 0xb06, 0xc0a, 0xd03, 0xe09, 0xf00, 
	0x190, 0x099, 0x393, 0x29a, 0x596, 0x49f, 0x795, 0x69c, 0x99c, 0x895, 0xb9f, 0xa96, 0xd9a, 0xc93, 0xf99, 0xe90, 
	0x230, 0x339, 0x033, 0x13a, 0x636, 0x73f, 0x435, 0x53c, 0xa3c, 0xb35, 0x83f, 0x936, 0xe3a, 0xf33, 0xc39, 0xd30, 
	0x3a0, 0x2a9, 0x1a3, 0x0aa, 0x7a6, 0x6af, 0x5a5, 0x4ac, 0xbac, 0xaa5, 0x9af, 0x8a6, 0xfaa, 0xea3, 0xda9, 0xca0, 
	0x460, 0x569, 0x663, 0x76a, 0x066, 0x16f, 0x265, 0x36c, 0xc6c, 0xd65, 0xe6f, 0xf66, 0x86a, 0x963, 0xa69, 0xb60, 
	0x5f0, 0x4f9, 0x7f3, 0x6fa, 0x1f6, 0x0ff, 0x3f5, 0x2fc, 0xdfc, 0xcf5, 0xfff, 0xef6, 0x9fa, 0x8f3, 0xbf9, 0xaf0, 
	0x650, 0x759, 0x453, 0x55a, 0x256, 0x35f, 0x055, 0x15c, 0xe5c, 0xf55, 0xc5f, 0xd56, 0xa5a, 0xb53, 0x859, 0x950, 
	0x7c0, 0x6c9, 0x5c3, 0x4ca, 0x3c6, 0x2cf, 0x1c5, 0x0cc, 0xfcc, 0xec5, 0xdcf, 0xcc6, 0xbca, 0xac3, 0x9c9, 0x8c0, 
	0x8c0, 0x9c9, 0xac3, 0xbca, 0xcc6, 0xdcf, 0xec5, 0xfcc, 0x0cc, 0x1c5, 0x2cf, 0x3c6, 0x4ca, 0x5c3, 0x6c9, 0x7c0, 
	0x950, 0x859, 0xb53, 0xa5a, 0xd56, 0xc5f, 0xf55, 0xe5c, 0x15c, 0x055, 0x35f, 0x256, 0x55a, 0x453, 0x759, 0x650, 
	0xaf0, 0xbf9, 0x8f3, 0x9fa, 0xef6, 0xfff, 0xcf5, 0xdfc, 0x2fc, 0x3f5, 0x0ff, 0x1f6, 0x6fa, 0x7f3, 0x4f9, 0x5f0, 
	0xb60, 0xa69, 0x963, 0x86a, 0xf66, 0xe6f, 0xd65, 0xc6c, 0x36c, 0x265, 0x16f, 0x066, 0x76a, 0x663, 0x569, 0x460, 
	0xca0, 0xda9, 0xea3, 0xfaa, 0x8a6, 0x9af, 0xaa5, 0xbac, 0x4ac, 0x5a5, 0x6af, 0x7a6, 0x0aa, 0x1a3, 0x2a9, 0x3a0, 
	0xd30, 0xc39, 0xf33, 0xe3a, 0x936, 0x83f, 0xb35, 0xa3c, 0x53c, 0x435, 0x73f, 0x636, 0x13a, 0x033, 0x339, 0x230, 
	0xe90, 0xf99, 0xc93, 0xd9a, 0xa96, 0xb9f, 0x895, 0x99c, 0x69c, 0x795, 0x49f, 0x596, 0x29a, 0x393, 0x099, 0x190, 
	0xf00, 0xe09, 0xd03, 0xc0a, 0xb06, 0xa0f, 0x905, 0x80c, 0x70c, 0x605, 0x50f, 0x406, 0x30a, 0x203, 0x109, 0x000
};

int iTriangleConnectionTable[256][16] =  
{
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, 250},
	{9, 2, 10, 0, 2, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{2, 8, 3, 2, 10, 8, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1},
	{3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 11, 2, 8, 11, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 9, 0, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 11, 2, 1, 9, 11, 9, 8, 11, -1, -1, -1, -1, -1, -1, -1},
	{3, 10, 1, 11, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 10, 1, 0, 8, 10, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1},
	{3, 9, 0, 3, 11, 9, 11, 10, 9, -1, -1, -1, -1, -1, -1, -1},
	{9, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 3, 0, 7, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 1, 9, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, 237},
	{4, 1, 9, 4, 7, 1, 7, 3, 1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 4, 7, 3, 0, 4, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1},
	{9, 2, 10, 9, 0, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
	{2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, -1, -1, -1, -1},
	{8, 4, 7, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, 231},
	{11, 4, 7, 11, 2, 4, 2, 0, 4, -1, -1, -1, -1, -1, -1, -1},
	{9, 0, 1, 8, 4, 7, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
	{4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, -1, -1, -1, -1},
	{3, 10, 1, 3, 11, 10, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1},
	{1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, -1, -1, -1, -1},
	{4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, -1, -1, -1, -1},
	{4, 7, 11, 4, 11, 9, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1},
	{9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 5, 4, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 5, 4, 1, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{8, 5, 4, 8, 3, 5, 3, 1, 5, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 0, 8, 1, 2, 10, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
	{5, 2, 10, 5, 4, 2, 4, 0, 2, -1, -1, -1, -1, -1, -1, -1},
	{2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, -1, -1, -1, -1},
	{9, 5, 4, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 11, 2, 0, 8, 11, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
	{0, 5, 4, 0, 1, 5, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1}, 
	{2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, -1, -1, -1, -1},
	{10, 3, 11, 10, 1, 3, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1},
	{4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, -1, -1, -1, -1}, 
	{5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, -1, -1, -1, -1},
	{5, 4, 8, 5, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1},
	{9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 3, 0, 9, 5, 3, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1},
	{0, 7, 8, 0, 1, 7, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1},
	{1, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 7, 8, 9, 5, 7, 10, 1, 2, -1, -1, -1, -1, -1, -1, -1},
	{10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, -1, -1, -1, -1},
	{8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, -1, -1, -1, -1},
	{2, 10, 5, 2, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1},
	{7, 9, 5, 7, 8, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1},
	{9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, -1, -1, -1, -1},
	{2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, -1, -1, -1, -1},
	{11, 2, 1, 11, 1, 7, 7, 1, 5, -1, -1, -1, -1, -1, -1, -1},
	{9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, -1, -1, -1, -1},
	{5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0, -1},
	{11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0, -1}, 
	{11, 10, 5, 7, 11, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 
	{9, 0, 1, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, 189},
	{1, 8, 3, 1, 9, 8, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
	{1, 6, 5, 2, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 6, 5, 1, 2, 6, 3, 0, 8, -1, -1, -1, -1, -1, -1, -1},
	{9, 6, 5, 9, 0, 6, 0, 2, 6, -1, -1, -1, -1, -1, -1, -1},
	{5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, -1, -1, -1, -1},
	{2, 3, 11, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{11, 0, 8, 11, 2, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
	{0, 1, 9, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
	{5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, -1, -1, -1, -1},
	{6, 3, 11, 6, 5, 3, 5, 1, 3, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, -1, -1, -1, -1},
	{3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, -1, -1, -1, -1},
	{6, 5, 9, 6, 9, 11, 11, 9, 8, -1, -1, -1, -1, -1, -1, -1},
	{5, 10, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, 175},
	{4, 3, 0, 4, 7, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1, -1},
	{1, 9, 0, 5, 10, 6, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
	{10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, -1, -1, -1, -1},
	{6, 1, 2, 6, 5, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, -1, -1, -1, -1},
	{8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, -1, -1, -1, -1},
	{7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9, -1},
	{3, 11, 2, 7, 8, 4, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
	{5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, -1, -1, -1, -1},
	{0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1},
	{9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6, -1},
	{8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, -1, -1, -1, -1},
	{5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11, -1},
	{0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7, -1},
	{6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, -1, -1, -1, -1},
	{10, 4, 9, 6, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 10, 6, 4, 9, 10, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1},
	{10, 0, 1, 10, 6, 0, 6, 4, 0, -1, -1, -1, -1, -1, -1, -1},
	{8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, -1, -1, -1, -1},
	{1, 4, 9, 1, 2, 4, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1},
	{3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, -1, -1, -1, -1},
	{0, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{8, 3, 2, 8, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1},
	{10, 4, 9, 10, 6, 4, 11, 2, 3, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6, -1, -1, -1, -1},
	{3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10, -1, -1, -1, -1},
	{6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1, -1},
	{9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3, -1, -1, -1, -1},
	{8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1, -1},
	{3, 11, 6, 3, 6, 0, 0, 6, 4, -1, -1, -1, -1, -1, -1, -1},
	{6, 4, 8, 11, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{7, 10, 6, 7, 8, 10, 8, 9, 10, -1, -1, -1, -1, -1, -1, -1},
	{0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10, -1, -1, -1, -1},
	{10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0, -1, -1, -1, -1},
	{10, 6, 7, 10, 7, 1, 1, 7, 3, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7, -1, -1, -1, -1},
	{2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9, -1},
	{7, 8, 0, 7, 0, 6, 6, 0, 2, -1, -1, -1, -1, -1, -1, -1},
	{7, 3, 2, 6, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7, -1, -1, -1, -1},
	{2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7, -1},
	{1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11, -1},
	{11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1, -1, -1, -1, -1},
	{8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6, -1},
	{0, 9, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0, -1, -1, -1, -1},
	{7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 0, 8, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 1, 9, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{8, 1, 9, 8, 3, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
	{10, 1, 2, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, 3, 0, 8, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
	{2, 9, 0, 2, 10, 9, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
	{6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8, -1, -1, -1, -1},
	{7, 2, 3, 6, 2, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{7, 0, 8, 7, 6, 0, 6, 2, 0, -1, -1, -1, -1, -1, -1, -1},
	{2, 7, 6, 2, 3, 7, 0, 1, 9, -1, -1, -1, -1, -1, -1, -1},
	{1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6, -1, -1, -1, -1},
	{10, 7, 6, 10, 1, 7, 1, 3, 7, -1, -1, -1, -1, -1, -1, -1},
	{10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8, -1, -1, -1, -1},
	{0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7, -1, -1, -1, -1},
	{7, 6, 10, 7, 10, 8, 8, 10, 9, -1, -1, -1, -1, -1, -1, -1},
	{6, 8, 4, 11, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 6, 11, 3, 0, 6, 0, 4, 6, -1, -1, -1, -1, -1, -1, -1},
	{8, 6, 11, 8, 4, 6, 9, 0, 1, -1, -1, -1, -1, -1, -1, -1},
	{9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6, -1, -1, -1, -1},
	{6, 8, 4, 6, 11, 8, 2, 10, 1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6, -1, -1, -1, -1},
	{4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9, -1, -1, -1, -1},
	{10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3, -1},
	{8, 2, 3, 8, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1},
	{0, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8, -1, -1, -1, -1},
	{1, 9, 4, 1, 4, 2, 2, 4, 6, -1, -1, -1, -1, -1, -1, -1},
	{8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1, -1, -1, -1, -1},
	{10, 1, 0, 10, 0, 6, 6, 0, 4, -1, -1, -1, -1, -1, -1, -1},
	{4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3, -1},
	{10, 9, 4, 6, 10, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 9, 5, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, 4, 9, 5, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
	{5, 0, 1, 5, 4, 0, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
	{11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5, -1, -1, -1, -1},
	{9, 5, 4, 10, 1, 2, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
	{6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5, -1, -1, -1, -1},
	{7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2, -1, -1, -1, -1},
	{3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6, -1},
	{7, 2, 3, 7, 6, 2, 5, 4, 9, -1, -1, -1, -1, -1, -1, -1},
	{9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7, -1, -1, -1, -1},
	{3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0, -1, -1, -1, -1},
	{6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8, -1},
	{9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7, -1, -1, -1, -1},
	{1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4, -1},
	{4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10, -1},
	{7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10, -1, -1, -1, 80},
	{6, 9, 5, 6, 11, 9, 11, 8, 9, -1, -1, -1, -1, -1, -1, -1},
	{3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5, -1, -1, -1, -1},
	{0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11, -1, -1, -1, -1},
	{6, 11, 3, 6, 3, 5, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6, -1, -1, -1, -1},
	{0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10, -1},
	{11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5, -1},
	{6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3, -1, -1, -1, -1},
	{5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2, -1, -1, -1, -1},
	{9, 5, 6, 9, 6, 0, 0, 6, 2, -1, -1, -1, -1, -1, -1, -1},
	{1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8, -1},
	{1, 5, 6, 2, 1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6, -1},
	{10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0, -1, -1, -1, 66},
	{0, 3, 8, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{11, 5, 10, 7, 5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{11, 5, 10, 11, 7, 5, 8, 3, 0, -1, -1, -1, -1, -1, -1, -1},
	{5, 11, 7, 5, 10, 11, 1, 9, 0, -1, -1, -1, -1, -1, -1, -1},
	{10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1, -1, -1, -1, -1},
	{11, 1, 2, 11, 7, 1, 7, 5, 1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11, -1, -1, -1, -1},
	{9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7, -1, -1, -1, -1},
	{7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2, -1},
	{2, 5, 10, 2, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1},
	{8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5, -1, -1, -1, -1},
	{9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2, -1, -1, -1, -1},
	{9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2, -1},
	{1, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 7, 0, 7, 1, 1, 7, 5, -1, -1, -1, -1, -1, -1, -1},
	{9, 0, 3, 9, 3, 5, 5, 3, 7, -1, -1, -1, -1, -1, -1, -1},
	{9, 8, 7, 5, 9, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{5, 8, 4, 5, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1},
	{5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0, -1, -1, -1, -1},
	{0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5, -1, -1, -1, -1},
	{10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4, -1},
	{2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8, -1, -1, -1, -1},
	{0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11, -1},
	{0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5, -1},
	{9, 4, 5, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4, -1, -1, -1, -1},
	{5, 10, 2, 5, 2, 4, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1},
	{3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9, -1},
	{5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2, -1, -1, -1, -1},
	{8, 4, 5, 8, 5, 3, 3, 5, 1, -1, -1, -1, -1, -1, -1, -1},
	{0, 4, 5, 1, 0, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5, -1, -1, -1, -1},
	{9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 11, 7, 4, 9, 11, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11, -1, -1, -1, -1},
	{1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11, -1, -1, -1, -1},
	{3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4, -1},
	{4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2, -1, -1, -1, -1},
	{9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3, -1},
	{11, 7, 4, 11, 4, 2, 2, 4, 0, -1, -1, -1, -1, -1, -1, -1},
	{11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4, -1, -1, -1, 24},
	{2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9, -1, -1, -1, -1},
	{9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7, -1},
	{3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10, -1},
	{1, 10, 2, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 9, 1, 4, 1, 7, 7, 1, 3, -1, -1, -1, -1, -1, -1, -1},
	{4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1, -1, -1, -1, 18},
	{4, 0, 3, 7, 4, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 0, 9, 3, 9, 11, 11, 9, 10, -1, -1, -1, -1, -1, -1, -1},
	{0, 1, 10, 0, 10, 8, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1},
	{3, 1, 10, 11, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 11, 1, 11, 9, 9, 11, 8, -1, -1, -1, -1, -1, -1, -1},
	{3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9, -1, -1, -1, -1},
	{0, 2, 11, 8, 0, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{2, 3, 8, 2, 8, 10, 10, 8, 9, -1, -1, -1, -1, -1, -1, -1},
	{9, 10, 2, 0, 9, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8, -1, -1, -1, 5},
	{1, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 3, 8, 9, 1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
};

MarchingCubes::MarchingCubes(MarchingCubesDataInterface* pCubeDataBuffer) : m_CubeDataBuffer(pCubeDataBuffer)
{
}


MarchingCubes::~MarchingCubes(void)
{
}

#define Algorithm1 0
#define Algorithm2 0
#define Algorithm3 0
#define Algorithm4 1
#define DebugNormal 0
#define NormalLength 1500

void MarchingCubes::MarchingCubesMain(int m_dIsoValue)
{
	if(!m_CubeDataBuffer)
		return;

	m_CubeDataBuffer->MarchingCubesStart();
	int iCount = 0;
	//std::map< int, std::pair<v3Point, v3Point> > IntMapToEdge;
	//std::map<int, v3Point> PointOnEdge;
	double gradFactorX(0.3), gradFactorY(0.3), gradFactorZ(0.3);

	int iTableID = -1;
	int iValue = (int)m_dIsoValue;
	unsigned char ucIndex = 0;
	//osg::Vec3 factor(1.0/(2.0*gradFactorX), 1.0/(2.0*gradFactorY), 1.0/(2.0*gradFactorZ));
	int iCubeSize = m_CubeDataBuffer->GetSize();

	for(int iCubeNum = 0; iCubeNum < iCubeSize; iCubeNum+=8)
	{
		CubeEdge IntMapToEdge[12] = {0};
		v3Point PointOnEdge[12] = {0};
		v3Point NormalOnEdge[12] = {0};
		/*IntMapToEdge.clear();
		PointOnEdge.clear();*/

		unsigned int iIndex = 0;
		//创始化边数据
		InitEdgeMap(IntMapToEdge, iCubeNum);
		//创建顶点索引
		for(int iPI = 0; iPI < 8; iPI++)
		{
			int iI = iCubeNum/* * 8*/ + iPI;
			v3Point v3p;// = m_CubeDataBuffer->GetCubePoint(iI);
			m_CubeDataBuffer->GetCubePoint(iI, v3p.x, v3p.y, v3p.z, v3p.value);
			if(v3p.value >= m_dIsoValue)
				iIndex |= 1<<iPI;
		}

		iTableID = CubeEdgeFlags[iIndex];
		if(!iTableID) continue;

		for(int ie = 0; ie < 12; ie++)
		{
			if(iTableID & 1<< ie)
			{
				v3Point v3Result, v3Normal;
				ComputeValueOnEdge(IntMapToEdge, ie, m_dIsoValue, v3Result);
				PointOnEdge[ie] = v3Result;
				
#if Algorithm2
				/*v3Normal.x = v3Result.x * factor.x();
				v3Normal.y = v3Result.y * factor.y();
				v3Normal.z = v3Result.z * factor.z();*/
				vNormalizeVector(v3Normal, v3Result);
				
				/*v3Normal.x /= sqrt(v3Normal.x * v3Normal.x + v3Normal.y * v3Normal.y + v3Normal.z * v3Normal.z);
				v3Normal.y /= sqrt(v3Normal.x * v3Normal.x + v3Normal.y * v3Normal.y + v3Normal.z * v3Normal.z);
				v3Normal.z /= sqrt(v3Normal.x * v3Normal.x + v3Normal.y * v3Normal.y + v3Normal.z * v3Normal.z);*/

				NormalOnEdge[ie] = v3Normal;
#endif
				//PointOnEdge.insert(std::pair<int, v3Point>(ie, v3Result));
			}
		}

		for (int n=0; iTriangleConnectionTable[iIndex][n] != -1; n+=3)
		{
#if 0
			if(iCount < iPrePointCount)
			{
				iCount++;
				break;
			}
			if(m_TriArray->size() > iVertexCount) return;
#endif
			double dr(0), dg(0), db(0), da(0);
			float iValue = PointOnEdge[iTriangleConnectionTable[iIndex][n]].value;
			//查找等值面三角型顶点

			osg::Vec3 v3triPint1, v3triPint2, v3triPint3;

			v3triPint1.set(PointOnEdge[iTriangleConnectionTable[iIndex][n+2]].x
				, PointOnEdge[iTriangleConnectionTable[iIndex][n+2]].y
				, PointOnEdge[iTriangleConnectionTable[iIndex][n+2]].z);

			v3triPint2.set(PointOnEdge[iTriangleConnectionTable[iIndex][n+1]].x
				, PointOnEdge[iTriangleConnectionTable[iIndex][n+1]].y
				, PointOnEdge[iTriangleConnectionTable[iIndex][n+1]].z);

			v3triPint3.set(PointOnEdge[iTriangleConnectionTable[iIndex][n+0]].x
				, PointOnEdge[iTriangleConnectionTable[iIndex][n+0]].y
				, PointOnEdge[iTriangleConnectionTable[iIndex][n+0]].z);

			/*v3triPint1.x = PointOnEdge[iTriangleConnectionTable[iIndex][n+2]].x;
			v3triPint1.y = PointOnEdge[iTriangleConnectionTable[iIndex][n+2]].y;
			v3triPint1.z = PointOnEdge[iTriangleConnectionTable[iIndex][n+2]].z;

			v3triPint2.x = PointOnEdge[iTriangleConnectionTable[iIndex][n+1]].x;
			v3triPint2.y = PointOnEdge[iTriangleConnectionTable[iIndex][n+1]].y;
			v3triPint2.z = PointOnEdge[iTriangleConnectionTable[iIndex][n+1]].z;

			v3triPint3.x = PointOnEdge[iTriangleConnectionTable[iIndex][n+0]].x;
			v3triPint3.y = PointOnEdge[iTriangleConnectionTable[iIndex][n+0]].y;
			v3triPint3.z = PointOnEdge[iTriangleConnectionTable[iIndex][n+0]].z;*/


			/*m_CubeDataBuffer->OutVertexData(0, PointOnEdge[iTriangleConnectionTable[iIndex][n+2]].x
			, PointOnEdge[iTriangleConnectionTable[iIndex][n+2]].y
			, PointOnEdge[iTriangleConnectionTable[iIndex][n+2]].z);

			m_CubeDataBuffer->OutVertexData(1, PointOnEdge[iTriangleConnectionTable[iIndex][n+1]].x
			, PointOnEdge[iTriangleConnectionTable[iIndex][n+1]].y
			, PointOnEdge[iTriangleConnectionTable[iIndex][n+1]].z);

			m_CubeDataBuffer->OutVertexData(2, PointOnEdge[iTriangleConnectionTable[iIndex][n]].x
			, PointOnEdge[iTriangleConnectionTable[iIndex][n]].y
			, PointOnEdge[iTriangleConnectionTable[iIndex][n]].z);*/

			m_CubeDataBuffer->OutVertexData(0, PointOnEdge[iTriangleConnectionTable[iIndex][n]].x
				, PointOnEdge[iTriangleConnectionTable[iIndex][n]].y
				, PointOnEdge[iTriangleConnectionTable[iIndex][n]].z);

			m_CubeDataBuffer->OutVertexData(1, PointOnEdge[iTriangleConnectionTable[iIndex][n+1]].x
				, PointOnEdge[iTriangleConnectionTable[iIndex][n+1]].y
				, PointOnEdge[iTriangleConnectionTable[iIndex][n+1]].z);

			m_CubeDataBuffer->OutVertexData(2, PointOnEdge[iTriangleConnectionTable[iIndex][n+2]].x
				, PointOnEdge[iTriangleConnectionTable[iIndex][n+2]].y
				, PointOnEdge[iTriangleConnectionTable[iIndex][n+2]].z);

			m_CubeDataBuffer->GetColorFromValue(iValue, dr, dg, db, da);

			m_CubeDataBuffer->OutColorData(0, dr, dg, db, da);

			m_CubeDataBuffer->OutColorData(1, dr, dg, db, da);

			m_CubeDataBuffer->OutColorData(2, dr, dg, db, da);
			//////////////////////////////////////////////////////////////////////////
#if Algorithm2
			v3Normal1.set(NormalOnEdge[iTriangleConnectionTable[iIndex][n+2]].x
				, NormalOnEdge[iTriangleConnectionTable[iIndex][n+2]].y
				, NormalOnEdge[iTriangleConnectionTable[iIndex][n+2]].z);

			v3Normal2.set(NormalOnEdge[iTriangleConnectionTable[iIndex][n+1]].x
				, NormalOnEdge[iTriangleConnectionTable[iIndex][n+1]].y
				, NormalOnEdge[iTriangleConnectionTable[iIndex][n+1]].z);

			v3Normal3.set(NormalOnEdge[iTriangleConnectionTable[iIndex][n]].x
				, NormalOnEdge[iTriangleConnectionTable[iIndex][n]].y
				, NormalOnEdge[iTriangleConnectionTable[iIndex][n]].z);
#endif
			
#if Algorithm1
			//三角型法线
			osg::Vec3 v3Test1 = v3triPint2 - v3triPint1;
			osg::Vec3 v3Test2 = v3triPint3 - v3triPint2;

			osg::Vec3 ZeroVector(0, 0, 0);
			if(! (v3Test1.x()*v3Test2.y() - v3Test2.x() * v3Test1.y() > 0.0)) 
			{
				m_TriArray->push_back(v3triPint1);
				m_TriArray->push_back(v3triPint3);
				m_TriArray->push_back(v3triPint2);

				v3Normal = (v3triPint1 - v3triPint3)^(v3triPint2 - v3triPint3);
				v3Normal = ZeroVector - v3Normal;
				v3Normal.normalize();
				
				colorArray->push_back(*v4color);
				colorArray->push_back(*v4color);
				colorArray->push_back(*v4color);

				m_TriNomArray->push_back(v3Normal);
				m_TriNomArray->push_back(v3Normal);
				m_TriNomArray->push_back(v3Normal);
			}
			else
			{
				m_TriArray->push_back(v3triPint1);
				m_TriArray->push_back(v3triPint2);
				m_TriArray->push_back(v3triPint3);

				v3Normal = (v3triPint1 - v3triPint2)^(v3triPint3 - v3triPint2);
				//v3Normal = ZeroVector - v3Normal;
				v3Normal.normalize();

				colorArray->push_back(*v4color);
				colorArray->push_back(*v4color);
				colorArray->push_back(*v4color);

				m_TriNomArray->push_back(v3Normal);
				m_TriNomArray->push_back(v3Normal);
				m_TriNomArray->push_back(v3Normal);
			}
#endif

#if Algorithm2
			if(!((v3Test1.x()*v3Test2.y() - v3Test2.x() * v3Test1.y()) > 0.0)) 
			{
				m_TriArray->push_back(v3triPint1);
				m_TriArray->push_back(v3triPint3);
				m_TriArray->push_back(v3triPint2);

				m_TriNomArray->push_back(v3Normal1);
				m_TriNomArray->push_back(v3Normal3);
				m_TriNomArray->push_back(v3Normal2);
			}
			else
			{
				m_TriArray->push_back(v3triPint1);
				m_TriArray->push_back(v3triPint2);
				m_TriArray->push_back(v3triPint3);

				m_TriNomArray->push_back(v3Normal1);
				m_TriNomArray->push_back(v3Normal2);
				m_TriNomArray->push_back(v3Normal3);
			}

			
#endif
			
#if Algorithm3
			double dS = 0;
			double dx1 = fabs(v3triPint1.x() - v3triPint2.x());
			double dx2 = fabs(v3triPint1.x() - v3triPint3.x());
			double dy1 = fabs(v3triPint1.y() - v3triPint2.y());
			double dy2 = fabs(v3triPint1.y() - v3triPint3.y());
			double dz1 = fabs(v3triPint1.z() - v3triPint2.z());
			double dz2 = fabs(v3triPint1.z() - v3triPint3.z());


			if(dx1 > 0.0000001 && dx2 > 0.0000001 && dy1 > 0.0000001 && dy2 > 0.0000001)
			{
				dS = ((v3triPint1.x()*v3triPint2.y()-v3triPint2.x()*v3triPint1.y())
					+(v3triPint2.x()*v3triPint3.y()-v3triPint3.x()*v3triPint2.y())
					+(v3triPint3.x()*v3triPint1.y()-v3triPint1.x()*v3triPint3.y()))/2.0;
			}
			else if(dx1 > 0.0000001 && dx2 > 0.0000001 && dz1 > 0.0000001 && dz2 > 0.0000001)
			{
				dS = ((v3triPint1.x()*v3triPint2.z()-v3triPint2.x()*v3triPint1.z())
					+(v3triPint2.x()*v3triPint3.z()-v3triPint3.x()*v3triPint2.z())
					+(v3triPint3.x()*v3triPint1.z()-v3triPint1.x()*v3triPint3.z()))/2.0;
			}
			else
			{
				dS = ((v3triPint1.y()*v3triPint2.z()-v3triPint2.y()*v3triPint1.z())
					+(v3triPint2.y()*v3triPint3.z()-v3triPint3.y()*v3triPint2.z())
					+(v3triPint3.y()*v3triPint1.z()-v3triPint1.y()*v3triPint3.z()))/2.0;
			}

			if(dS > 0.0)
			{
				m_CubeDataBuffer->OutNormalData(0, v3triPint1.x(), v3triPint1.y(), v3triPint1.z());
				m_CubeDataBuffer->OutNormalData(1, v3triPint2.x(), v3triPint2.y(), v3triPint2.z());
				m_CubeDataBuffer->OutNormalData(2, v3triPint3.x(), v3triPint3.y(), v3triPint3.z());
			}
			else
			{
				m_CubeDataBuffer->OutNormalData(0, v3triPint1.x(), v3triPint1.y(), v3triPint1.z());
				m_CubeDataBuffer->OutNormalData(1, v3triPint3.x(), v3triPint3.y(), v3triPint3.z());
				m_CubeDataBuffer->OutNormalData(2, v3triPint2.x(), v3triPint2.y(), v3triPint2.z());
			}
#endif

#if Algorithm4
			osg::Vec3 normal = (v3triPint2-v3triPint1)^(v3triPint3-v3triPint1);
			m_CubeDataBuffer->OutNormalData(0, normal.x(), normal.y(), normal.z());
			m_CubeDataBuffer->OutNormalData(1, normal.x(), normal.y(), normal.z());
			m_CubeDataBuffer->OutNormalData(2, normal.x(), normal.y(), normal.z());
#endif
		}

	}

	m_CubeDataBuffer->MarchingCubesFinish();
}

void MarchingCubes::MarchingCubesMain( float m_dIsoValue )
{
	if(!m_CubeDataBuffer)
		return;

	m_CubeDataBuffer->MarchingCubesStart();
	int iCount = 0;
	//std::map< int, std::pair<v3Point, v3Point> > IntMapToEdge;
	//std::map<int, v3Point> PointOnEdge;
	double gradFactorX(0.3), gradFactorY(0.3), gradFactorZ(0.3);

	int iTableID = -1;
	//int iValue = (int)m_dIsoValue;
	unsigned char ucIndex = 0;
	//osg::Vec3 factor(1.0/(2.0*gradFactorX), 1.0/(2.0*gradFactorY), 1.0/(2.0*gradFactorZ));
	int iCubeSize = m_CubeDataBuffer->GetSize();

	for(int iCubeNum = 0; iCubeNum < iCubeSize; iCubeNum+=8)
	{
		CubeEdge IntMapToEdge[12] = {0};
		v3Point PointOnEdge[12] = {0};
		v3Point NormalOnEdge[12] = {0};
		/*IntMapToEdge.clear();
		PointOnEdge.clear();*/

		unsigned int iIndex = 0;
		//创始化边数据
		InitEdgeMap(IntMapToEdge, iCubeNum);
		//创建顶点索引
		for(int iPI = 0; iPI < 8; iPI++)
		{
			int iI = iCubeNum/* * 8*/ + iPI;
			v3Point v3p;// = m_CubeDataBuffer->GetCubePoint(iI);
			m_CubeDataBuffer->GetCubePoint(iI, v3p.x, v3p.y, v3p.z, v3p.value);
			if(v3p.value - m_dIsoValue >= 0)
				iIndex |= 1<<iPI;
		}

		iTableID = CubeEdgeFlags[iIndex];
		if(!iTableID) continue;

		for(int ie = 0; ie < 12; ie++)
		{
			if(iTableID & 1<< ie)
			{
				v3Point v3Result, v3Normal;
				ComputeValueOnEdge(IntMapToEdge, ie, m_dIsoValue, v3Result);
				PointOnEdge[ie] = v3Result;
			}
		}

		for (int n=0; iTriangleConnectionTable[iIndex][n] != -1; n+=3)
		{

			double dr(0), dg(0), db(0), da(0);
			float iValue = PointOnEdge[iTriangleConnectionTable[iIndex][n]].value;
			//查找等值面三角型顶点

			osg::Vec3 v3triPint1, v3triPint2, v3triPint3;

			v3triPint1.set(PointOnEdge[iTriangleConnectionTable[iIndex][n+2]].x
				, PointOnEdge[iTriangleConnectionTable[iIndex][n+2]].y
				, PointOnEdge[iTriangleConnectionTable[iIndex][n+2]].z);

			v3triPint2.set(PointOnEdge[iTriangleConnectionTable[iIndex][n+1]].x
				, PointOnEdge[iTriangleConnectionTable[iIndex][n+1]].y
				, PointOnEdge[iTriangleConnectionTable[iIndex][n+1]].z);

			v3triPint3.set(PointOnEdge[iTriangleConnectionTable[iIndex][n+0]].x
				, PointOnEdge[iTriangleConnectionTable[iIndex][n+0]].y
				, PointOnEdge[iTriangleConnectionTable[iIndex][n+0]].z);

			m_CubeDataBuffer->OutVertexData(0, PointOnEdge[iTriangleConnectionTable[iIndex][n]].x
				, PointOnEdge[iTriangleConnectionTable[iIndex][n]].y
				, PointOnEdge[iTriangleConnectionTable[iIndex][n]].z);

			m_CubeDataBuffer->OutVertexData(1, PointOnEdge[iTriangleConnectionTable[iIndex][n+1]].x
				, PointOnEdge[iTriangleConnectionTable[iIndex][n+1]].y
				, PointOnEdge[iTriangleConnectionTable[iIndex][n+1]].z);

			m_CubeDataBuffer->OutVertexData(2, PointOnEdge[iTriangleConnectionTable[iIndex][n+2]].x
				, PointOnEdge[iTriangleConnectionTable[iIndex][n+2]].y
				, PointOnEdge[iTriangleConnectionTable[iIndex][n+2]].z);

			m_CubeDataBuffer->GetColorFromValue(iValue, dr, dg, db, da);

			m_CubeDataBuffer->OutColorData(0, dr, dg, db, da);

			m_CubeDataBuffer->OutColorData(1, dr, dg, db, da);

			m_CubeDataBuffer->OutColorData(2, dr, dg, db, da);
			//////////////////////////////////////////////////////////////////////////

#if Algorithm3
			double dS = 0;
			double dx1 = fabs(v3triPint1.x() - v3triPint2.x());
			double dx2 = fabs(v3triPint1.x() - v3triPint3.x());
			double dy1 = fabs(v3triPint1.y() - v3triPint2.y());
			double dy2 = fabs(v3triPint1.y() - v3triPint3.y());
			double dz1 = fabs(v3triPint1.z() - v3triPint2.z());
			double dz2 = fabs(v3triPint1.z() - v3triPint3.z());


			if(dx1 > 0.0000001 && dx2 > 0.0000001 && dy1 > 0.0000001 && dy2 > 0.0000001)
			{
				dS = ((v3triPint1.x()*v3triPint2.y()-v3triPint2.x()*v3triPint1.y())
					+(v3triPint2.x()*v3triPint3.y()-v3triPint3.x()*v3triPint2.y())
					+(v3triPint3.x()*v3triPint1.y()-v3triPint1.x()*v3triPint3.y()))/2.0;
			}
			else if(dx1 > 0.0000001 && dx2 > 0.0000001 && dz1 > 0.0000001 && dz2 > 0.0000001)
			{
				dS = ((v3triPint1.x()*v3triPint2.z()-v3triPint2.x()*v3triPint1.z())
					+(v3triPint2.x()*v3triPint3.z()-v3triPint3.x()*v3triPint2.z())
					+(v3triPint3.x()*v3triPint1.z()-v3triPint1.x()*v3triPint3.z()))/2.0;
			}
			else
			{
				dS = ((v3triPint1.y()*v3triPint2.z()-v3triPint2.y()*v3triPint1.z())
					+(v3triPint2.y()*v3triPint3.z()-v3triPint3.y()*v3triPint2.z())
					+(v3triPint3.y()*v3triPint1.z()-v3triPint1.y()*v3triPint3.z()))/2.0;
			}

			if(dS > 0.0)
			{
				m_CubeDataBuffer->OutNormalData(0, v3triPint1.x(), v3triPint1.y(), v3triPint1.z());
				m_CubeDataBuffer->OutNormalData(1, v3triPint2.x(), v3triPint2.y(), v3triPint2.z());
				m_CubeDataBuffer->OutNormalData(2, v3triPint3.x(), v3triPint3.y(), v3triPint3.z());
			}
			else
			{
				m_CubeDataBuffer->OutNormalData(0, v3triPint1.x(), v3triPint1.y(), v3triPint1.z());
				m_CubeDataBuffer->OutNormalData(1, v3triPint3.x(), v3triPint3.y(), v3triPint3.z());
				m_CubeDataBuffer->OutNormalData(2, v3triPint2.x(), v3triPint2.y(), v3triPint2.z());
			}
#endif
		}

	}

	m_CubeDataBuffer->MarchingCubesFinish();
}

void MarchingCubes::InitEdgeMap( CubeEdge* pCubeEdge, int iCubeID )
{
	m_CubeDataBuffer->GetCubePoint(iCubeID + 0, (pCubeEdge + 0)->start.x, (pCubeEdge + 0)->start.y, (pCubeEdge + 0)->start.z, (pCubeEdge + 0)->start.value);
	m_CubeDataBuffer->GetCubePoint(iCubeID + 1, (pCubeEdge + 0)->end.x, (pCubeEdge + 0)->end.y, (pCubeEdge + 0)->end.z, (pCubeEdge + 0)->end.value);

	m_CubeDataBuffer->GetCubePoint(iCubeID + 1, (pCubeEdge + 1)->start.x, (pCubeEdge + 1)->start.y, (pCubeEdge + 1)->start.z, (pCubeEdge + 1)->start.value);
	m_CubeDataBuffer->GetCubePoint(iCubeID + 2, (pCubeEdge + 1)->end.x, (pCubeEdge + 1)->end.y, (pCubeEdge + 1)->end.z, (pCubeEdge + 1)->end.value);

	m_CubeDataBuffer->GetCubePoint(iCubeID + 2, (pCubeEdge + 2)->start.x, (pCubeEdge + 2)->start.y, (pCubeEdge + 2)->start.z, (pCubeEdge + 2)->start.value);
	m_CubeDataBuffer->GetCubePoint(iCubeID + 3, (pCubeEdge + 2)->end.x, (pCubeEdge + 2)->end.y, (pCubeEdge + 2)->end.z, (pCubeEdge + 2)->end.value);

	m_CubeDataBuffer->GetCubePoint(iCubeID + 3, (pCubeEdge + 3)->start.x, (pCubeEdge + 3)->start.y, (pCubeEdge + 3)->start.z, (pCubeEdge + 3)->start.value);
	m_CubeDataBuffer->GetCubePoint(iCubeID + 0, (pCubeEdge + 3)->end.x, (pCubeEdge + 3)->end.y, (pCubeEdge + 3)->end.z, (pCubeEdge + 3)->end.value);

	m_CubeDataBuffer->GetCubePoint(iCubeID + 4, (pCubeEdge + 4)->start.x, (pCubeEdge + 4)->start.y, (pCubeEdge + 4)->start.z, (pCubeEdge + 4)->start.value);
	m_CubeDataBuffer->GetCubePoint(iCubeID + 5, (pCubeEdge + 4)->end.x, (pCubeEdge + 4)->end.y, (pCubeEdge + 4)->end.z, (pCubeEdge + 4)->end.value);

	m_CubeDataBuffer->GetCubePoint(iCubeID + 5, (pCubeEdge + 5)->start.x, (pCubeEdge + 5)->start.y, (pCubeEdge + 5)->start.z, (pCubeEdge + 5)->start.value);
	m_CubeDataBuffer->GetCubePoint(iCubeID + 6, (pCubeEdge + 5)->end.x, (pCubeEdge + 5)->end.y, (pCubeEdge + 5)->end.z, (pCubeEdge + 5)->end.value);

	m_CubeDataBuffer->GetCubePoint(iCubeID + 6, (pCubeEdge + 6)->start.x, (pCubeEdge + 6)->start.y, (pCubeEdge + 6)->start.z, (pCubeEdge + 6)->start.value);
	m_CubeDataBuffer->GetCubePoint(iCubeID + 7, (pCubeEdge + 6)->end.x, (pCubeEdge + 6)->end.y, (pCubeEdge + 6)->end.z, (pCubeEdge + 6)->end.value);

	m_CubeDataBuffer->GetCubePoint(iCubeID + 7, (pCubeEdge + 7)->start.x, (pCubeEdge + 7)->start.y, (pCubeEdge + 7)->start.z, (pCubeEdge + 7)->start.value);
	m_CubeDataBuffer->GetCubePoint(iCubeID + 4, (pCubeEdge + 7)->end.x, (pCubeEdge + 7)->end.y, (pCubeEdge + 7)->end.z, (pCubeEdge + 7)->end.value);

	m_CubeDataBuffer->GetCubePoint(iCubeID + 0, (pCubeEdge + 8)->start.x, (pCubeEdge + 8)->start.y, (pCubeEdge + 8)->start.z, (pCubeEdge + 8)->start.value);
	m_CubeDataBuffer->GetCubePoint(iCubeID + 4, (pCubeEdge + 8)->end.x, (pCubeEdge + 8)->end.y, (pCubeEdge + 8)->end.z, (pCubeEdge + 8)->end.value);

	m_CubeDataBuffer->GetCubePoint(iCubeID + 1, (pCubeEdge + 9)->start.x, (pCubeEdge + 9)->start.y, (pCubeEdge + 9)->start.z, (pCubeEdge + 9)->start.value);
	m_CubeDataBuffer->GetCubePoint(iCubeID + 5, (pCubeEdge + 9)->end.x, (pCubeEdge + 9)->end.y, (pCubeEdge + 9)->end.z, (pCubeEdge + 9)->end.value);

	m_CubeDataBuffer->GetCubePoint(iCubeID + 2, (pCubeEdge + 10)->start.x, (pCubeEdge + 10)->start.y, (pCubeEdge + 10)->start.z, (pCubeEdge + 10)->start.value);
	m_CubeDataBuffer->GetCubePoint(iCubeID + 6, (pCubeEdge + 10)->end.x, (pCubeEdge + 10)->end.y, (pCubeEdge + 10)->end.z, (pCubeEdge + 10)->end.value);

	m_CubeDataBuffer->GetCubePoint(iCubeID + 3, (pCubeEdge + 11)->start.x, (pCubeEdge + 11)->start.y, (pCubeEdge + 11)->start.z, (pCubeEdge + 11)->start.value);
	m_CubeDataBuffer->GetCubePoint(iCubeID + 7, (pCubeEdge + 11)->end.x, (pCubeEdge + 11)->end.y, (pCubeEdge + 11)->end.z, (pCubeEdge + 11)->end.value);
}

void MarchingCubes::ComputeValueOnEdge( CubeEdge* pCubeEdge, int iEdgeID, int ivalue, v3Point& result )
{
	osg::Vec3 v1, v2, p;
	v3Point startPoint, endPoint;
	double fac = 0;
	//std::map< int, std::pair<v3Point, v3Point> >::iterator findedResult = buff.find(iEdgeID);

	if(pCubeEdge)
	{
		startPoint = (pCubeEdge + iEdgeID)->start;
		endPoint = (pCubeEdge + iEdgeID)->end;

		v1.set(startPoint.x, startPoint.y, startPoint.z);
		v2.set(endPoint.x, endPoint.y, endPoint.z);

		if(startPoint.value == endPoint.value)
		{
			result = startPoint;
		}
		else
		{
			p = v1 + (v2 - v1)/(endPoint.value - startPoint.value)*(ivalue - startPoint.value);

			result.x = p.x();
			result.y = p.y();
			result.z = p.z();
			result.value = ivalue;
		}
	}
}

//compute energy
float MarchingCubes::computeENG(float x,float y,float z)
{
	float R1(200/*6378000*/);
	float R2(300);

	v3Point ball1;
	ball1.x = 0;
	ball1.y = 0;
	ball1.z = 0;
	v3Point ball2;
	ball2.x = 2000000;
	ball2.y = 0;
	ball2.z = 0;

	float r1 = sqrtf(pow(x-ball1.x,2)+pow(y-ball1.y,2)+pow(z-ball1.z,2));
	float r2 = sqrtf(pow(x-ball2.x,2)+pow(y-ball2.y,2)+pow(z-ball2.z,2));
	float energy1 = 0;
	if(r1 < R1)
		energy1 = pow(1-pow(r1/R1,2),2);
	float energy2 = 0;
	if(r2 < R2)
		energy2 = pow(1-pow(r2/R2,2),2);
	return energy1+energy2;
}

void MarchingCubes::vNormalizeVector(v3Point &rfVectorResult, v3Point &rfVectorSource)
{
	float fOldLength;
	float fScale;
	fOldLength = sqrtf( (rfVectorSource.x * rfVectorSource.x) +
		(rfVectorSource.y * rfVectorSource.y) +
		(rfVectorSource.z * rfVectorSource.z) );
	if(fOldLength == 0.0)
	{
		rfVectorResult.x = rfVectorSource.x;
		rfVectorResult.y = rfVectorSource.y;
		rfVectorResult.z = rfVectorSource.z;
	}
	else
	{
		fScale = 1.0/fOldLength;
		rfVectorResult.x = rfVectorSource.x*fScale;
		rfVectorResult.y = rfVectorSource.y*fScale;
		rfVectorResult.z = rfVectorSource.z*fScale;
	}
}

//vGetNormal() finds the gradient of the scalar field at a point
//This gradient can be used as a very accurate vertex normal for lighting calculations
void MarchingCubes::vGetNormal(v3Point& rfNormal, float fX, float fY, float fZ)
{
	rfNormal.x = computeENG(fX-1.0, fY, fZ) - computeENG(fX+1.0, fY, fZ);
	rfNormal.y = computeENG(fX, fY-1.0, fZ) - computeENG(fX, fY+1.0, fZ);
	rfNormal.z = computeENG(fX, fY, fZ-1.0) - computeENG(fX, fY, fZ+1.0);
	vNormalizeVector(rfNormal, rfNormal);
}