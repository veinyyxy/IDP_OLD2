#include "galloc.h"

#define LISTSIZE  100000 
#define LISTSIZEB 900000 



static char *ptrs[LISTSIZE];
static size_t lens[LISTSIZE];
static char cbuf[LISTSIZEB];
static int first = 1;
static int verbo=0;
static int buferr=0;   /* flag error on buffer exceeded */

/*  replacement for malloc */

void *galloc (size_t len,char *ch)
{
	char *mem=NULL,*mmm;
	int i,j;
	size_t llen;

	/* initialize ptrs to null */
	if (first)
	{
		first = 0;
		for (i=0; i<LISTSIZE; i++)
		{
			ptrs[i] = NULL;
		}
	}
	/* allocate the memory */
	llen = len + 8;
	mem = (char *)malloc(llen);
	if (mem==NULL)
	{
		return (NULL);
	}

	if (buferr)
		return(mem);
  /* add 8 characters 'A' to the tail of the allocated memory */
	mmm = mem + len;
	for (i=0; i<8; i++) 
		*(mmm+i) = 'A';
  /* move to the first non-NULL ptr */
	i = 0;                
	while (ptrs[i]!=NULL)
		i++;
	
	if (i>LISTSIZE-2)
	{
	/* if hard-coded limit of ptrs has been exceeded, 
		trip the flag to stop memory tracking */
		if (!buferr)
		{
			printf ("Galloc memory tracking buffers exceeded. \n");
			buferr = 1;
		}
		return(mem);
	}
	ptrs[i] = mem;               /* pointer to allocated memory */
	lens[i] = len;               /* length of allocated memory  */
	for (j=0; j<8; j++)
		cbuf[i*8+j] = ' ';

	j = 0;
	while (j<8 && *(ch+j))
	{
		cbuf[i*8+j] = *(ch+j);      /* tag name of allocated memory   */
		j++;
	}
	return(mem);
}

/* replacement for free */

void gree (char *mem, char *ch)
{
	int i,j,flag;
	size_t len;
	char *mmm;

	/* if we have stopped tracking memory, just free it and return */
	if (buferr) {
		free (mem);
		return;
	}
	/* move through the list of ptrs to the one we're going to free */
	i = 0;
	while (1) {
		if (i>LISTSIZE-2) break;
		if (ptrs[i]==mem) break; 
		i++;
	}
	if (i>LISTSIZE-2) {
		if (verbo) printf ("!*!*!      freeing unallocated space! %s %p\n",ch,mem);
	} 
	else {
		/* reset this pointer to NULL */
		ptrs[i] = NULL;
		/* check if tail of allocated memory still has 8 'A' characters */
		len = lens[i];
		mmm = mem + len;
		flag = 0;
		for (j=0; j<8; j++) if (*(mmm+j)!='A') flag = 1;
		if (flag) {
			if (verbo) {
				printf ("Overlay!!! -->");
				for (j=0; j<8; j++) printf("%c",*(mmm+j));       /* show the overlay */
				printf ("<-- -->");
				for (j=0; j<8; j++) printf("%c",cbuf[i*8+j]);    /* show the galloc tag */
				printf ("<-- %s\n",ch);                           /* show the gree tag */
			}
		}
		if (cbuf[i*8+4]=='?') {
			if (verbo) {
				printf ("Freeing %i %p %s ",i,mem,ch);
				for (j=0; j<8; j++) printf("%c",cbuf[i*8+j]);
				printf ("<--\n");
			}
		}
	}
	free (mem);
}


/* lists currently allocated memory chunks */

void glook(void) {
	int i,j,flag;
	size_t len;
	char *mmm;

	if (buferr) {
		printf ("Mem tracking buffer was exceeded. \n");
	}
	for (i=0; i<LISTSIZE; i++) {
		if (ptrs[i]!=NULL) {
			/* list the index, ptr, length, and name of each allocated memory chunk */
			printf ("pos=%i  ptr=%p  len=%ld  type=",i,ptrs[i],lens[i]);
			for (j=0; j<8; j++) printf("%c",cbuf[i*8+j]);
			len = lens[i];
			mmm = ptrs[i] + len;
			flag = 0;
			for (j=0; j<8; j++) if (*(mmm+j)!='A') flag = 1;
			if (verbo && flag) printf ("   *** Overlay *** ");
			printf ("\n");
		}
	}
	if (buferr) {
		printf ("Mem tracking buffer was exceeded. \n");
	}
}

/* Checks on a particular memory chunk */

void gsee (char *mem) {
	int i,j,flag,len;
	char *mmm;
	if (buferr) {
		printf ("Mem tracking buffer was exceeded. \n");
	}
	i = 0;
	while (1) {
		if (i>LISTSIZE-1) break;
		if (ptrs[i]==mem) break; 
		i++;
	}
	if (i>LISTSIZE-1) {
		if (verbo) printf ("unallocated space! %p\n",mem);
		/*     getchar();              /\* Wait if requested    *\/ */
	} else {
		len = lens[i];
		mmm = mem + len;
		flag = 0;
		if (verbo) printf("pos=%d  ptr=%p  len=%i  tag=",i,mem,len);
		for (j=0; j<8; j++) if (*(mmm+j)!='A') flag = 1;
		if (verbo) {
			for (j=0; j<8; j++) printf("%c",cbuf[i*8+j]);
			if (flag) {
				printf ("   * * * Overlay!!! * * *");
				printf ("-->");
				for (j=0; j<8; j++) printf("%c",*(mmm+j));
				printf ("<--");
			}
			printf("\n");
		}
		/*       getchar();              /\* Wait if requested    *\/ */
	}
	if (buferr) {
		printf ("Mem tracking buffer was exceeded. \n");
	}
}
