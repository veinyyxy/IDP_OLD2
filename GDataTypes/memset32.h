#ifndef MEMSET32_H
#define MEMSET32_H

#if defined(WIN32)||defined(WIN64)
static inline void memset32(void * s, unsigned long c, size_t count)
{
	size_t count0=count%64;
	size_t count1=count-count0;
	size_t count2=count0%8;
	size_t count3=count0-count2;
	size_t count4=count2%4;
	size_t count5=count2-count4;
	__asm
	{
			movq		mm0, c
			punpckldq	mm0, mm0
			mov			edi, s

			movq         mm1, mm0
			movq         mm2, mm0
			movq         mm3, mm0
			movq         mm4, mm0
			movq         mm5, mm0
			movq         mm6, mm0
			movq         mm7, mm0

			mov		ecx, count1
			lea     edi, [edi + ecx]
			neg		ecx
			jz		loopend

loopwrite:
			movntq      [edi + ecx     ], mm0
			movntq      [edi + ecx + 8 ], mm1
			movntq      [edi + ecx + 16], mm2
			movntq      [edi + ecx + 24], mm3
			movntq      [edi + ecx + 32], mm4
			movntq      [edi + ecx + 40], mm5
			movntq      [edi + ecx + 48], mm6
			movntq      [edi + ecx + 56], mm7

			add ecx, 64
			jnz loopwrite
loopend:
			mov			ecx, count3
			lea			edi, [edi + ecx]
			neg			ecx
			jz	set64bitend

loop64bit:
			movntq		[edi+ecx],mm0
			add	ecx,8
			jnz	loop64bit
set64bitend:

			mov			ecx, count5
			jz			byte4end
			lea			edi,[edi+ecx]
			neg			ecx
			
			mov			eax,c
			mov			[edi+ecx],eax

byte4end:
			mov			eax,c
			mov			ecx, count4
			jz			allend
			lea			edi,[edi+ecx]
			neg			ecx
byte1loop:
			mov			[edi+ecx],al

			add			ecx,1
			jz			allend

			shr			eax,1
			shr			eax,1
			shr			eax,1
			shr			eax,1
			shr			eax,1
			shr			eax,1
			shr			eax,1
			shr			eax,1

			jmp			byte1loop



allend:
			emms
	}

}
static inline void gWmemset(void * startp,gWchar value16,size_t count_dword)
{
	wmemset((wchar_t*) startp,value16,count_dword);
}
#else
static inline void * memset32(void * s, unsigned long c, size_t count)
{
	int d0, d1;
	__asm__ __volatile__(
		"rep ; stosl\n\t"
		"testb $2,%b3\n\t"
		"je 1f\n\t"
		"stosw\n"
		"1:\ttestb $1,%b3\n\t"
		"je 2f\n\t"
		"stosb\n"
		"2:"
		:"=&c" (d0), "=&D" (d1)
		:"a" (c), "q" (count), "0" (count/4), "1" ((long) s)
		:"memory");
	return (s);
}
static inline void gWmemset(void * startp,gWchar value16,size_t count_dword)		//example: sizeof(gInt16)*(number of word16)/2
{
	memset32(startp,(gInt32)((value16<<16)|(value16)),count_dword*2);
}
#endif

#endif //MEMSET32_H
