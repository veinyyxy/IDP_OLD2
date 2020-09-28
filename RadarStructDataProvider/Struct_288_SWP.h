#ifndef STRUCT_288_SWP_H
#define STRUCT_288_SWP_H

typedef struct  
{
	gInt16 LenOfVILBox;
	gInt16 LenOfSWPBox;
	gInt16 Width;
	gInt16 Height;
}SWPHead;

typedef struct  
{
	gInt16 x;
	gInt16 y;
	gInt16 Probability;
}SWPStruct;

#endif