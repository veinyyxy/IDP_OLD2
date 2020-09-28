#include "tin.h"

//#define USENCC
#ifdef USENCC
namespace NCCGIS{
#endif

CTIN::CTIN()
{
    //* Fast lookup arrays to speed some of the mesh manipulation primitives.
            plus1mod3[0] = 1;
            plus1mod3[1] = 2;
            plus1mod3[2] = 0;
            minus1mod3[0] = 2;
            minus1mod3[1] = 0;
            minus1mod3[2] = 1;

            //the params used by tin created from points and lines.
            pPoints = NULL;
            pTriangles = NULL;

            pPointChain = NULL;
            pTriangleChain = NULL;

            //点、三角形的起始编号；
            lBaseTriangleNum = 0;
            lBasePointNum = 0;

            //the params used by tin created from points, contours, lines.
            pInPoints = NULL;
            pTPoints = NULL;
            pPtChain = NULL;

            pContours = NULL;

            pInLines = NULL;

            pTriChain = NULL;
            pTTriangles = NULL;
}
CTIN::~CTIN()
{
    //the memory used by TIN created from points and lines.
            if (pPoints)
            {
                    free(pPoints);
                    pPoints = NULL;
            }

            if (pTriangles)
            {
                    free(pTriangles);
                    pTriangles = NULL;
            }


            if (pTriangleChain){
                    CTriangleChain* pChain = pTriangleChain;
                    while(pTriangleChain){
                            pChain = pTriangleChain;
                            pTriangleChain = pTriangleChain->next;
                            free(pChain->tri);
							pChain->tri = NULL;
                            free(pChain);
							pChain = NULL;
                    }
            }
            if (pPointChain){
                    CPointChain* pTempPtChain = pPointChain;
                    while(pPointChain){
                            pTempPtChain = pPointChain;
                            pPointChain = pPointChain->next;
                            free(pTempPtChain->pt);
							pTempPtChain->pt = NULL;
                            free(pTempPtChain);
							pTempPtChain = NULL;
                    }
            }
}
void CTIN::poolinit(struct CMemoryPool *pool, long bytecount, long itemcount, enum wordtype wtype, long alignment)
{
        long  wordsize;

        /* Initialize values in the pool. */
        pool->itemwordtype = wtype;
        wordsize = (pool->itemwordtype == POINTER) ? sizeof(long *) : sizeof(double);
        /* Find the proper alignment, which must be at least as large as:   */
        /*   - The parameter `alignment'.                                   */
        /*   - The primary word type, to avoid unaligned accesses.          */
        /*   - sizeof(long *), so the stack of dead items can be maintained */
        /*       without unaligned accesses.                                */
        if (alignment > wordsize) {
                pool->alignbytes = alignment;
        } else {
                pool->alignbytes = wordsize;
        }
        if (sizeof(long *) > pool->alignbytes) {
                pool->alignbytes = sizeof(long *);
        }
        pool->itemwords = ((bytecount + pool->alignbytes - 1) / pool->alignbytes)
                * (pool->alignbytes / wordsize);
        pool->itembytes = pool->itemwords * wordsize;
        pool->itemsperblock = itemcount;

        /* Allocate a block of items.  Space for `itemsperblock' items and one    */
        /*   pointer (to point to the next block) are allocated, as well as space */
        /*   to ensure alignment of the items.                                    */
        pool->firstblock = (long **) malloc(pool->itemsperblock * pool->itembytes
                + sizeof(long *) + pool->alignbytes);
        if (pool->firstblock == (long **) NULL) {
                printf("Error:  Out of memory.\n");
                exit(1);
        }
        /* Set the next block pointer to NULL. */
        *(pool->firstblock) = (long *) NULL;
        poolrestart(pool);
}

/*****************************************************************************/
/*                                                                           */
/*  poolrestart()   Deallocate all items in a pool.                          */
/*                                                                           */
/*  The pool is returned to its starting state, except that no memory is     */
/*  freed to the operating system.  Rather, the previously allocated blocks  */
/*  are ready to be reused.                                                  */
/*                                                                           */
/*****************************************************************************/

void CTIN::poolrestart(struct CMemoryPool *pool)
{
        unsigned long alignptr;

        pool->items = 0;
        pool->maxitems = 0;

        /* Set the currently active block. */
        pool->nowblock = pool->firstblock;
        /* Find the first item in the pool.  Increment by the size of (long *). */
        alignptr = (unsigned long) (pool->nowblock + 1);
        /* Align the item on an `alignbytes'-byte boundary. */
        pool->nextitem = (long *)
                (alignptr + (unsigned long) pool->alignbytes
                - (alignptr % (unsigned long) pool->alignbytes));
        /* There are lots of unallocated items left in this block. */
        pool->unallocateditems = pool->itemsperblock;
        /* The stack of deallocated items is empty. */
        pool->deaditemstack = (long *) NULL;
}

/*****************************************************************************/
/*                                                                           */
/*  pooldeinit()   Free to the operating system all memory taken by a pool.  */
/*                                                                           */
/*****************************************************************************/


void CTIN::pooldeinit(struct CMemoryPool *pool)
{
        while (pool->firstblock != (long **) NULL) {
                pool->nowblock = (long **) *(pool->firstblock);
                free(pool->firstblock);
                pool->firstblock = pool->nowblock;
        }
}

/*****************************************************************************/
/*                                                                           */
/*  poolalloc()   Allocate space for an item.                                */
/*                                                                           */
/*****************************************************************************/

long *CTIN::poolalloc(struct CMemoryPool *pool)
{
        long *newitem;
        long **newblock;
        unsigned long alignptr;

        /* First check the linked list of dead items.  If the list is not   */
        /*   empty, allocate an item from the list rather than a fresh one. */
        if (pool->deaditemstack != (long *) NULL) {
                newitem = pool->deaditemstack;               /* Take first item in list. */
                pool->deaditemstack = * (long **) pool->deaditemstack;
        } else {
                /* Check if there are any free items left in the current block. */
                if (pool->unallocateditems == 0) {
                        /* Check if another block must be allocated. */
                        if (*(pool->nowblock) == (long *) NULL) {
                                /* Allocate a new block of items, pointed to by the previous block. */
                                newblock = (long **) malloc(pool->itemsperblock * pool->itembytes
                                        + sizeof(long *) + pool->alignbytes);
                                if (newblock == (long **) NULL) {
                                        printf("Error:  Out of memory.\n");
                                        exit(1);
                                }
                                *(pool->nowblock) = (long *) newblock;
                                /* The next block pointer is NULL. */
                                *newblock = (long *) NULL;
                        }
                        /* Move to the new block. */
                        pool->nowblock = (long **) *(pool->nowblock);
                        /* Find the first item in the block.    */
                        /*   Increment by the size of (long *). */
                        alignptr = (unsigned long) (pool->nowblock + 1);
                        /* Align the item on an `alignbytes'-byte boundary. */
                        pool->nextitem = (long *)
                                (alignptr + (unsigned long) pool->alignbytes
                                - (alignptr % (unsigned long) pool->alignbytes));
                        /* There are lots of unallocated items left in this block. */
                        pool->unallocateditems = pool->itemsperblock;
                }
                /* Allocate a new item. */
                newitem = pool->nextitem;
                /* Advance `nextitem' pointer to next free item in block. */
                if (pool->itemwordtype == POINTER) {
                        pool->nextitem = (long *) ((long **) pool->nextitem + pool->itemwords);
                } else {
                        pool->nextitem = (long *) ((double *) pool->nextitem + pool->itemwords);
                }
                pool->unallocateditems--;
                pool->maxitems++;
        }
        pool->items++;
        return newitem;
}

/*****************************************************************************/
/*                                                                           */
/*  pooldealloc()   Deallocate space for an item.                            */
/*                                                                           */
/*  The deallocated space is stored in a queue for later reuse.              */
/*                                                                           */
/*****************************************************************************/

void CTIN::pooldealloc(struct CMemoryPool *pool, long *dyingitem)
{
        /* Push freshly killed item onto stack. */
        *((long **) dyingitem) = pool->deaditemstack;
        pool->deaditemstack = dyingitem;
        pool->items--;
}

/*****************************************************************************/
/*                                                                           */
/*  traversalinit()   Prepare to traverse the entire list of items.          */
/*                                                                           */
/*  This routine is used in conjunction with traverse().                     */
/*                                                                           */
/*****************************************************************************/

void CTIN::traversalinit(struct CMemoryPool *pool)
{
        unsigned long alignptr;

        /* Begin the traversal in the first block. */
        pool->pathblock = pool->firstblock;
        /* Find the first item in the block.  Increment by the size of (long *). */
        alignptr = (unsigned long) (pool->pathblock + 1);
        /* Align with item on an `alignbytes'-byte boundary. */
        pool->pathitem = (long *)
                (alignptr + (unsigned long) pool->alignbytes
                - (alignptr % (unsigned long) pool->alignbytes));
        /* Set the number of items left in the current block. */
        pool->pathitemsleft = pool->itemsperblock;
}

/*****************************************************************************/
/*                                                                           */
/*  traverse()   Find the next item in the list.                             */
/*                                                                           */
/*  This routine is used in conjunction with traversalinit().  Be forewarned */
/*  that this routine successively returns all items in the list, including  */
/*  deallocated ones on the deaditemqueue.  It's up to you to figure out     */
/*  which ones are actually dead.  Why?  I don't want to allocate extra      */
/*  space just to demarcate dead items.  It can usually be done more         */
/*  space-efficiently by a routine that knows something about the structure  */
/*  of the item.                                                             */
/*                                                                           */
/*****************************************************************************/

long *CTIN::traverse(struct CMemoryPool *pool)
{
        long *newitem;
        unsigned long alignptr;

        /* Stop upon exhausting the list of items. */
        if (pool->pathitem == pool->nextitem) {
                return (long *) NULL;
        }
        /* Check whether any untraversed items remain in the current block. */
        if (pool->pathitemsleft == 0) {
                /* Find the next block. */
                pool->pathblock = (long **) *(pool->pathblock);
                /* Find the first item in the block.  Increment by the size of (long *). */
                alignptr = (unsigned long) (pool->pathblock + 1);
                /* Align with item on an `alignbytes'-byte boundary. */
                pool->pathitem = (long *)
                        (alignptr + (unsigned long) pool->alignbytes
                        - (alignptr % (unsigned long) pool->alignbytes));
                /* Set the number of items left in the current block. */
                pool->pathitemsleft = pool->itemsperblock;
        }
        newitem = pool->pathitem;
        /* Find the next item in the block. */
        if (pool->itemwordtype == POINTER) {
                pool->pathitem = (long *) ((long **) pool->pathitem + pool->itemwords);
        } else {
                pool->pathitem = (long *) ((double *) pool->pathitem + pool->itemwords);
        }
        pool->pathitemsleft--;
        return newitem;
}

/*****************************************************************************/
/*                                                                           */
/*  dummyinit()   Initialize the triangle that fills "outer space" and the   */
/*                omnipresent shell edge.                                    */
/*                                                                           */
/*  The triangle that fills "outer space", called `dummytri', is pointed to  */
/*  by every triangle and shell edge on a boundary (be it outer or inner) of */
/*  the triangulation.  Also, `dummytri' points to one of the triangles on   */
/*  the convex hull (until the holes and concavities are carved), making it  */
/*  possible to find a starting triangle for point location.                 */
/*                                                                           */
/*  The omnipresent shell edge, `dummysh', is pointed to by every triangle   */
/*  or shell edge that doesn't have a full complement of real shell edges    */
/*  to point to.                                                             */
/*                                                                           */
/*****************************************************************************/

void CTIN::dummyinit(long trianglewords, long shellewords)
{
        unsigned long alignptr;

        /* `triwords' and `shwords' are used by the mesh manipulation primitives */
        /*   to extract orientations of triangles and shell edges from pointers. */
        triwords = trianglewords;       /* Initialize `triwords' once and for all. */
        shwords = shellewords;           /* Initialize `shwords' once and for all. */

        /* Set up `dummytri', the `triangle' that occupies "outer space". */
        dummytribase = (triangle *) malloc(triwords * sizeof(triangle)
                + triangles.alignbytes);
        if (dummytribase == (triangle *) NULL) {
                printf("Error:  Out of memory.\n");
                exit(1);
        }
        /* Align `dummytri' on a `triangles.alignbytes'-byte boundary. */
        alignptr = (unsigned long) dummytribase;
        dummytri = (triangle *)
                (alignptr + (unsigned long) triangles.alignbytes
                - (alignptr % (unsigned long) triangles.alignbytes));
        /* Initialize the three adjoining triangles to be "outer space".  These  */
        /*   will eventually be changed by various bonding operations, but their */
        /*   values don't really matter, as long as they can legally be          */
        /*   dereferenced.                                                       */
        dummytri[0] = (triangle) dummytri;
        dummytri[1] = (triangle) dummytri;
        dummytri[2] = (triangle) dummytri;
        /* Three NULL vertex points. */
        dummytri[3] = (triangle) NULL;
        dummytri[4] = (triangle) NULL;
        dummytri[5] = (triangle) NULL;

        if (useshelles) {
                /* Set up `dummysh', the omnipresent "shell edge" pointed to by any      */
                /*   triangle side or shell edge end that isn't attached to a real shell */
                /*   edge.                                                               */
                dummyshbase = (shelle *) malloc(shwords * sizeof(shelle)
                        + shelles.alignbytes);
                if (dummyshbase == (shelle *) NULL) {
                        printf("Error:  Out of memory.\n");
                        exit(1);
                }
                /* Align `dummysh' on a `shelles.alignbytes'-byte boundary. */
                alignptr = (unsigned long) dummyshbase;
                dummysh = (shelle *)
                        (alignptr + (unsigned long) shelles.alignbytes
                        - (alignptr % (unsigned long) shelles.alignbytes));
                /* Initialize the two adjoining shell edges to be the omnipresent shell */
                /*   edge.  These will eventually be changed by various bonding         */
                /*   operations, but their values don't really matter, as long as they  */
                /*   can legally be dereferenced.                                       */
                dummysh[0] = (shelle) dummysh;
                dummysh[1] = (shelle) dummysh;
                /* Two NULL vertex points. */
                dummysh[2] = (shelle) NULL;
                dummysh[3] = (shelle) NULL;
                /* Initialize the two adjoining triangles to be "outer space". */
                dummysh[4] = (shelle) dummytri;
                dummysh[5] = (shelle) dummytri;
                /* Set the boundary marker to zero. */
                * (long *) (dummysh + 6) = 0;

                /* Initialize the three adjoining shell edges of `dummytri' to be */
                /*   the omnipresent shell edge.                                  */
                dummytri[6] = (triangle) dummysh;
                dummytri[7] = (triangle) dummysh;
                dummytri[8] = (triangle) dummysh;
        }
}

/*****************************************************************************/
/*                                                                           */
/*  initializepointpool()   Calculate the size of the point data structure   */
/*                          and initialize its memory pool.                  */
/*                                                                           */
/*  This routine also computes the `pointmarkindex' and `point2triindex'     */
/*  indices used to find values within each point.                           */
/*                                                                           */
/*****************************************************************************/

void CTIN::initializepointpool()
{
        long  pointsize;

        /* The index within each point at which the boundary marker is found.  */
        /*   Ensure the point marker is aligned to a sizeof(long )-byte address. */
        pointmarkindex = (2 * sizeof(double) + sizeof(long ) - 1)
                / sizeof(long );
        pointsize = (pointmarkindex + 1) * sizeof(long );
        if (poly) {
                /* The index within each point at which a triangle pointer is found.   */
                /*   Ensure the pointer is aligned to a sizeof(triangle)-byte address. */
                point2triindex = (pointsize + sizeof(triangle) - 1) / sizeof(triangle);
                pointsize = (point2triindex + 1) * sizeof(triangle);
        }
        /* Initialize the pool of points. */
        poolinit(&points, pointsize, POINTPERBLOCK,
                (sizeof(double) >= sizeof(triangle)) ? FLOATINGPOINT : POINTER, 0);
}

/*****************************************************************************/
/*                                                                           */
/*  initializetrisegpools()   Calculate the sizes of the triangle and shell  */
/*                            edge data structures and initialize their      */
/*                            memory pools.                                  */
/*                                                                           */
/*  This routine also computes the `highorderindex', `elemattribindex', and  */
/*  `areaboundindex' indices used to find values within each triangle.       */
/*                                                                           */
/*****************************************************************************/

void CTIN::initializetrisegpools()
{
        long  trisize;

        /* The index within each triangle at which the extra nodes (above three)  */
        /*   associated with high order elements are found.  There are three      */
        /*   pointers to other triangles, three pointers to corners, and possibly */
        /*   three pointers to shell edges before the extra nodes.                */
        highorderindex = 6 + (useshelles * 3);
        /* The number of bytes occupied by a triangle. */
        trisize = ((order + 1) * (order + 2) / 2 + (highorderindex - 3)) *
                sizeof(triangle);
        /* The index within each triangle at which its attributes are found, */
        /*   where the index is measured in doubles.                           */
        elemattribindex = (trisize + sizeof(double) - 1) / sizeof(double);
        /* The index within each triangle at which the maximum area constraint  */
        /*   is found, where the index is measured in doubles.  Note that if the  */
        /*   `regionattrib' flag is set, an additional attribute will be added. */
        areaboundindex = elemattribindex;
        /* If triangle attributes or an area bound are needed, increase the number */
        /*   of bytes occupied by a triangle.                                      */
        if (vararea) {
                trisize = (areaboundindex + 1) * sizeof(double);
        }
        /* If a Voronoi diagram or triangle neighbor graph is requested, make    */
        /*   sure there's room to store an integer index in each triangle.  This */
        /*   integer index can occupy the same space as the shell edges or       */
        /*   attributes or area constraint or extra nodes.                       */
        if ((neighbors) &&
                (trisize < 6 * sizeof(triangle) + sizeof(long ))) {
                trisize = 6 * sizeof(triangle) + sizeof(long );
        }
        /* Having determined the memory size of a triangle, initialize the pool. */
        poolinit(&triangles, trisize, TRIPERBLOCK, POINTER, 4);

        if (useshelles) {
                /* Initialize the pool of shell edges. */
                poolinit(&shelles, 6 * sizeof(triangle) + sizeof(long ), SHELLEPERBLOCK,
                        POINTER, 4);

                /* Initialize the "outer space" triangle and omnipresent shell edge. */
                dummyinit(triangles.itemwords, shelles.itemwords);
        } else {
                /* Initialize the "outer space" triangle. */
                dummyinit(triangles.itemwords, 0);

        }
}

/*****************************************************************************/
/*                                                                           */
/*  triangledealloc()   Deallocate space for a triangle, marking it dead.    */
/*                                                                           */
/*****************************************************************************/

void CTIN::triangledealloc(triangle *dyingtriangle)
{
        /* Set triangle's vertices to NULL.  This makes it possible to        */
        /*   detect dead triangles when traversing the list of all triangles. */
        dyingtriangle[3] = (triangle) NULL;
        dyingtriangle[4] = (triangle) NULL;
        dyingtriangle[5] = (triangle) NULL;
        pooldealloc(&triangles, (long *) dyingtriangle);
}

/*****************************************************************************/
/*                                                                           */
/*  triangletraverse()   Traverse the triangles, skipping dead ones.         */
/*                                                                           */
/*****************************************************************************/

triangle *CTIN::triangletraverse()
{
        triangle *newtriangle;

        do {
                newtriangle = (triangle *) traverse(&triangles);
                if (newtriangle == (triangle *) NULL) {
                        return (triangle *) NULL;
                }
        } while (newtriangle[3] == (triangle) NULL);            /* Skip dead ones. */
        return newtriangle;
}

/*****************************************************************************/
/*                                                                           */
/*  shelledealloc()   Deallocate space for a shell edge, marking it dead.    */
/*                                                                           */
/*****************************************************************************/

void CTIN::shelledealloc(shelle *dyingshelle)
{
        /* Set shell edge's vertices to NULL.  This makes it possible to */
        /*   detect dead shells when traversing the list of all shells.  */
        dyingshelle[2] = (shelle) NULL;
        dyingshelle[3] = (shelle) NULL;
        pooldealloc(&shelles, (long *) dyingshelle);
}

/*****************************************************************************/
/*                                                                           */
/*  shelletraverse()   Traverse the shell edges, skipping dead ones.         */
/*                                                                           */
/*****************************************************************************/

shelle *CTIN::shelletraverse()
{
        shelle *newshelle;

        do {
                newshelle = (shelle *) traverse(&shelles);
                if (newshelle == (shelle *) NULL) {
                        return (shelle *) NULL;
                }
        } while (newshelle[2] == (shelle) NULL);                /* Skip dead ones. */
        return newshelle;
}

/*****************************************************************************/
/*                                                                           */
/*  pointdealloc()   Deallocate space for a point, marking it dead.          */
/*                                                                           */
/*****************************************************************************/

void CTIN::pointdealloc(point dyingpoint)
{
        /* Mark the point as dead.  This makes it possible to detect dead points */
        /*   when traversing the list of all points.                             */
        setpointmark(dyingpoint, DEADPOINT);
        pooldealloc(&points, (long *) dyingpoint);
}

/*****************************************************************************/
/*                                                                           */
/*  pointtraverse()   Traverse the points, skipping dead ones.               */
/*                                                                           */
/*****************************************************************************/

point CTIN::pointtraverse()
{
        point newpoint;

        do {
                newpoint = (point) traverse(&points);
                if (newpoint == (point) NULL) {
                        return (point) NULL;
                }
        } while (pointmark(newpoint) == DEADPOINT);             /* Skip dead ones. */
        return newpoint;
}


/*****************************************************************************/
/*                                                                           */
/*  getpoint()   Get a specific point, by number, from the list.             */
/*                                                                           */
/*  The first point is number 'firstnumber' = 0.                                 */
/*                                                                           */
/*  Note that this takes O(n) time (with a small constant, if POINTPERBLOCK  */
/*  is large).  I don't care to take the trouble to make it work in constant */
/*  time.                                                                    */
/*                                                                           */
/*****************************************************************************/

point CTIN::getpoint(long number)
{
        long **getblock;
        point foundpoint;
        unsigned long alignptr;
        long current;

        getblock = points.firstblock;
        current = 0;
        /* Find the right block. */
        while (current + points.itemsperblock <= number) {
                getblock = (long **) *getblock;
                current += points.itemsperblock;
        }
        /* Now find the right point. */
        alignptr = (unsigned long) (getblock + 1);
        foundpoint = (point) (alignptr + (unsigned long) points.alignbytes
                - (alignptr % (unsigned long) points.alignbytes));
        while (current < number) {
                foundpoint += points.itemwords;
                current++;
        }
        return foundpoint;
}

/*****************************************************************************/
/*                                                                           */
/*  triangledeinit()   Free all remaining allocated memory.                  */
/*                                                                           */
/*****************************************************************************/

void CTIN::triangledeinit()
{
        pooldeinit(&triangles);
        free(dummytribase);
		dummytribase = NULL;
        if (useshelles) {
                pooldeinit(&shelles);
                free(dummyshbase);
				dummyshbase = NULL;
        }
        pooldeinit(&points);
}

/**                                                                         **/
/**                                                                         **/
/********* Memory management routines end here                       *********/

/********* Constructors begin here                                   *********/
/**                                                                         **/
/**                                                                         **/

/*****************************************************************************/
/*                                                                           */
/*  maketriangle()   Create a new triangle with orientation zero.            */
/*                                                                           */
/*****************************************************************************/

void CTIN::maketriangle(struct CTriEdge *newtriedge)
{
        newtriedge->tri = (triangle *) poolalloc(&triangles);
        /* Initialize the three adjoining triangles to be "outer space". */
        newtriedge->tri[0] = (triangle) dummytri;
        newtriedge->tri[1] = (triangle) dummytri;
        newtriedge->tri[2] = (triangle) dummytri;
        /* Three NULL vertex points. */
        newtriedge->tri[3] = (triangle) NULL;
        newtriedge->tri[4] = (triangle) NULL;
        newtriedge->tri[5] = (triangle) NULL;
        /* Initialize the three adjoining shell edges to be the omnipresent */
        /*   shell edge.                                                    */
        if (useshelles) {
                newtriedge->tri[6] = (triangle) dummysh;
                newtriedge->tri[7] = (triangle) dummysh;
                newtriedge->tri[8] = (triangle) dummysh;
        }
        if (vararea) {
                setareabound(*newtriedge, -1.0);
        }

        newtriedge->orient = 0;
}

/*****************************************************************************/
/*                                                                           */
/*  makeshelle()   Create a new shell edge with orientation zero.            */
/*                                                                           */
/*****************************************************************************/

void CTIN::makeshelle(struct CEdge *newedge)
{
        newedge->sh = (shelle *) poolalloc(&shelles);
        /* Initialize the two adjoining shell edges to be the omnipresent */
        /*   shell edge.                                                  */
        newedge->sh[0] = (shelle) dummysh;
        newedge->sh[1] = (shelle) dummysh;
        /* Two NULL vertex points. */
        newedge->sh[2] = (shelle) NULL;
        newedge->sh[3] = (shelle) NULL;
        /* Initialize the two adjoining triangles to be "outer space". */
        newedge->sh[4] = (shelle) dummytri;
        newedge->sh[5] = (shelle) dummytri;
        /* Set the boundary marker to zero. */
        setmark(*newedge, 0);

        newedge->shorient = 0;
}

/**                                                                         **/
/**                                                                         **/
/********* Constructors end here                                     *********/

/********* Determinant evaluation routines begin here                *********/
/**                                                                         **/
/**                                                                         **/

/* The adaptive exact arithmetic geometric predicates implemented herein are */
/*   described in detail in my Technical Report CMU-CS-96-140.  The complete */
/*   reference is given in the header.                                       */


/*****************************************************************************/
/*                                                                           */
/*  exactinit()   Initialize the variables used for exact arithmetic.        */
/*                                                                           */
/*  `epsilon' is the largest power of two such that 1.0 + epsilon = 1.0 in   */
/*  floating-point arithmetic.  `epsilon' bounds the relative roundoff       */
/*  error.  It is used for floating-point error analysis.                    */
/*                                                                           */
/*  `splitter' is used to split floating-point numbers into two half-        */
/*  length significands for exact multiplication.                            */
/*                                                                           */
/*  I imagine that a highly optimizing compiler might be too smart for its   */
/*  own good, and somehow cause this routine to fail, if it pretends that    */
/*  floating-point arithmetic is too much like real arithmetic.              */
/*                                                                           */
/*  Don't change this routine unless you fully understand it.                */
/*                                                                           */
/*****************************************************************************/

void CTIN::exactinit()
{
        double half;
        double check, lastcheck;
        long  every_other;

        every_other = 1;
        half = 0.5;
        epsilon = 1.0;
        splitter = 1.0;
        check = 1.0;
        /* Repeatedly divide `epsilon' by two until it is too small to add to      */
        /*   one without causing roundoff.  (Also check if the sum is equal to     */
        /*   the previous sum, for machines that round up instead of using exact   */
        /*   rounding.  Not that these routines will work on such machines anyway. */
        do {
                lastcheck = check;
                epsilon *= half;
                if (every_other)
                {
                        splitter *= 2.0;
                }
                every_other = !every_other;
                check = 1.0 + epsilon;
        } while ((check != 1.0) && (check != lastcheck));
        splitter += 1.0;
        //  if (verbose > 1) {
        //    printf("Floating point roundoff is of magnitude %.17g\n", epsilon);
        //    printf("Floating point splitter is %.17g\n", splitter);
        //  }
        /* Error bounds for orientation and incircle tests. */
        resulterrbound = (3.0 + 8.0 * epsilon) * epsilon;
        ccwerrboundA = (3.0 + 16.0 * epsilon) * epsilon;
        ccwerrboundB = (2.0 + 12.0 * epsilon) * epsilon;
        ccwerrboundC = (9.0 + 64.0 * epsilon) * epsilon * epsilon;
        iccerrboundA = (10.0 + 96.0 * epsilon) * epsilon;
        iccerrboundB = (4.0 + 48.0 * epsilon) * epsilon;
        iccerrboundC = (44.0 + 576.0 * epsilon) * epsilon * epsilon;
}

/*****************************************************************************/
/*                                                                           */
/*  fast_expansion_sum_zeroelim()   Sum two expansions, eliminating zero     */
/*                                  components from the output expansion.    */
/*                                                                           */
/*  Sets h = e + f.  See my Robust Predicates paper for details.             */
/*                                                                           */
/*  If round-to-even is used (as with IEEE 754), maintains the strongly      */
/*  nonoverlapping property.  (That is, if e is strongly nonoverlapping, h   */
/*  will be also.)  Does NOT maintain the nonoverlapping or nonadjacent      */
/*  properties.                                                              */
/*                                                                           */
/*****************************************************************************/

long  CTIN::fast_expansion_sum_zeroelim(long  elen, double *e, long  flen, double *f, double *h)  /* h cannot be e or f. */
{
        double Q;
        double Qnew;
        double hh;
        double bvirt;
        double avirt, bround, around;
        long  eindex, findex, hindex;
        double enow, fnow;

        enow = e[0];
        fnow = f[0];
        eindex = findex = 0;
        if ((fnow > enow) == (fnow > -enow)) {
                Q = enow;
                enow = e[++eindex];
        } else {
                Q = fnow;
                fnow = f[++findex];
        }
        hindex = 0;
        if ((eindex < elen) && (findex < flen)) {
                if ((fnow > enow) == (fnow > -enow)) {
                        Fast_Two_Sum(enow, Q, Qnew, hh);
                        enow = e[++eindex];
                } else {
                        Fast_Two_Sum(fnow, Q, Qnew, hh);
                        fnow = f[++findex];
                }
                Q = Qnew;
                if (hh != 0.0) {


                        h[hindex++] = hh;
                }
                while ((eindex < elen) && (findex < flen)) {
                        if ((fnow > enow) == (fnow > -enow)) {
                                Two_Sum(Q, enow, Qnew, hh);
                                enow = e[++eindex];
                        } else {
                                Two_Sum(Q, fnow, Qnew, hh);
                                fnow = f[++findex];
                        }
                        Q = Qnew;
                        if (hh != 0.0) {
                                h[hindex++] = hh;
                        }
                }
        }
        while (eindex < elen) {
                Two_Sum(Q, enow, Qnew, hh);
                enow = e[++eindex];
                Q = Qnew;
                if (hh != 0.0) {
                        h[hindex++] = hh;
                }
        }
        while (findex < flen) {
                Two_Sum(Q, fnow, Qnew, hh);
                fnow = f[++findex];
                Q = Qnew;
                if (hh != 0.0) {
                        h[hindex++] = hh;
                }
        }
        if ((Q != 0.0) || (hindex == 0)) {
                h[hindex++] = Q;
        }
        return hindex;
}

/*****************************************************************************/
/*                                                                           */
/*  scale_expansion_zeroelim()   Multiply an expansion by a scalar,          */
/*                               eliminating zero components from the        */
/*                               output expansion.                           */
/*                                                                           */
/*  Sets h = be.  See my Robust Predicates paper for details.                */
/*                                                                           */
/*  Maintains the nonoverlapping property.  If round-to-even is used (as     */
/*  with IEEE 754), maintains the strongly nonoverlapping and nonadjacent    */
/*  properties as well.  (That is, if e has one of these properties, so      */
/*  will h.)                                                                 */
/*                                                                           */
/*****************************************************************************/

long  CTIN::scale_expansion_zeroelim(long  elen, double *e, double b, double *h)   /* e and h cannot be the same. */
{
        double Q, sum;
        double hh;
        double product1;
        double product0;
        long  eindex, hindex;
        double enow;
        double bvirt;
        double avirt, bround, around;
        double c;
        double abig;
        double ahi, alo, bhi, blo;
        double err1, err2, err3;

        Split(b, bhi, blo);
        Two_Product_Presplit(e[0], b, bhi, blo, Q, hh);
        hindex = 0;
        if (hh != 0) {
                h[hindex++] = hh;
        }
        for (eindex = 1; eindex < elen; eindex++) {
                enow = e[eindex];
                Two_Product_Presplit(enow, b, bhi, blo, product1, product0);

                Two_Sum(Q, product0, sum, hh);
                if (hh != 0) {
                        h[hindex++] = hh;
                }
                Fast_Two_Sum(product1, sum, Q, hh);
                if (hh != 0) {
                        h[hindex++] = hh;
                }
        }
        if ((Q != 0.0) || (hindex == 0)) {
                h[hindex++] = Q;
        }
        return hindex;
}

/*****************************************************************************/
/*                                                                           */
/*  estimate()   Produce a one-word estimate of an expansion's value.        */
/*                                                                           */
/*  See my Robust Predicates paper for details.                              */
/*                                                                           */
/*****************************************************************************/

double CTIN::estimate(long  elen, double *e)
{
        double Q;
        long  eindex;

        Q = e[0];
        for (eindex = 1; eindex < elen; eindex++) {
                Q += e[eindex];
        }
        return Q;
}

/*****************************************************************************/
/*                                                                           */
/*  counterclockwise()   Return a positive value if the points pa, pb, and   */
/*                       pc occur in counterclockwise order; a negative      */
/*                       value if they occur in clockwise order; and zero    */
/*                       if they are collinear.  The result is also a rough  */
/*                       approximation of twice the signed area of the       */
/*                       triangle defined by the three points.               */
/*                                                                           */
/*  Uses exact arithmetic if necessary to ensure a correct answer.  The      */
/*  result returned is the determinant of a matrix.  This determinant is     */
/*  computed adaptively, in the sense that exact arithmetic is used only to  */
/*  the degree it is needed to ensure that the returned value has the        */
/*  correct sign.  Hence, this function is usually quite fast, but will run  */
/*  more slowly when the input points are collinear or nearly so.            */
/*                                                                           */
/*  See my Robust Predicates paper for details.                              */
/*                                                                           */
/*****************************************************************************/

double CTIN::counterclockwiseadapt(point pa, point pb, point pc, double detsum)
{
        double acx, acy, bcx, bcy;
        double acxtail, acytail, bcxtail, bcytail;
        double detleft, detright;
        double detlefttail, detrighttail;
        double det, errbound;
        double B[4], C1[8], C2[12], D[16];
        double B3;
        long  C1length, C2length, Dlength;
        double u[4];
        double u3;
        double s1, t1;
        double s0, t0;

        double bvirt;
        double avirt, bround, around;
        double c;
        double abig;
        double ahi, alo, bhi, blo;
        double err1, err2, err3;
        double _i, _j;
        double _0;

        acx = (double) (pa[0] - pc[0]);
        bcx = (double) (pb[0] - pc[0]);
        acy = (double) (pa[1] - pc[1]);
        bcy = (double) (pb[1] - pc[1]);

        Two_Product(acx, bcy, detleft, detlefttail);
        Two_Product(acy, bcx, detright, detrighttail);

        Two_Two_Diff(detleft, detlefttail, detright, detrighttail,
                B3, B[2], B[1], B[0]);
        B[3] = B3;

        det = estimate(4, B);
        errbound = ccwerrboundB * detsum;
        if ((det >= errbound) || (-det >= errbound)) {
                return det;
        }

        Two_Diff_Tail(pa[0], pc[0], acx, acxtail);
        Two_Diff_Tail(pb[0], pc[0], bcx, bcxtail);
        Two_Diff_Tail(pa[1], pc[1], acy, acytail);
        Two_Diff_Tail(pb[1], pc[1], bcy, bcytail);

        if ((acxtail == 0.0) && (acytail == 0.0)
                && (bcxtail == 0.0) && (bcytail == 0.0)) {
                return det;
        }

        errbound = ccwerrboundC * detsum + resulterrbound * Absolute(det);
        det += (acx * bcytail + bcy * acxtail)
                - (acy * bcxtail + bcx * acytail);
        if ((det >= errbound) || (-det >= errbound)) {
                return det;
        }

        Two_Product(acxtail, bcy, s1, s0);
        Two_Product(acytail, bcx, t1, t0);
        Two_Two_Diff(s1, s0, t1, t0, u3, u[2], u[1], u[0]);
        u[3] = u3;
        C1length = fast_expansion_sum_zeroelim(4, B, 4, u, C1);

        Two_Product(acx, bcytail, s1, s0);
        Two_Product(acy, bcxtail, t1, t0);
        Two_Two_Diff(s1, s0, t1, t0, u3, u[2], u[1], u[0]);
        u[3] = u3;
        C2length = fast_expansion_sum_zeroelim(C1length, C1, 4, u, C2);

        Two_Product(acxtail, bcytail, s1, s0);
        Two_Product(acytail, bcxtail, t1, t0);
        Two_Two_Diff(s1, s0, t1, t0, u3, u[2], u[1], u[0]);
        u[3] = u3;
        Dlength = fast_expansion_sum_zeroelim(C2length, C2, 4, u, D);

        return(D[Dlength - 1]);
}

double CTIN::counterclockwise(point pa, point pb, point pc)
{
        double detleft, detright, det;
        double detsum, errbound;

        counterclockcount++;

        detleft = (pa[0] - pc[0]) * (pb[1] - pc[1]);
        detright = (pa[1] - pc[1]) * (pb[0] - pc[0]);
        det = detleft - detright;

        if (detleft > 0.0) {
                if (detright <= 0.0) {
                        return det;
                } else {
                        detsum = detleft + detright;
                }
        } else if (detleft < 0.0) {
                if (detright >= 0.0) {
                        return det;
                } else {
                        detsum = -detleft - detright;
                }
        } else {
                return det;
        }

        errbound = ccwerrboundA * detsum;
        if ((det >= errbound) || (-det >= errbound)) {
                return det;
        }

        return counterclockwiseadapt(pa, pb, pc, detsum);
}

/*****************************************************************************/
/*                                                                           */
/*  incircle()   Return a positive value if the point pd lies inside the     */
/*               circle passing through pa, pb, and pc; a negative value if  */
/*               it lies outside; and zero if the four points are cocircular.*/
/*               The points pa, pb, and pc must be in counterclockwise       */
/*               order, or the sign of the result will be reversed.          */
/*                                                                           */
/*  Uses exact arithmetic if necessary to ensure a correct answer.  The      */
/*  result returned is the determinant of a matrix.  This determinant is     */
/*  computed adaptively, in the sense that exact arithmetic is used only to  */
/*  the degree it is needed to ensure that the returned value has the        */
/*  correct sign.  Hence, this function is usually quite fast, but will run  */
/*  more slowly when the input points are cocircular or nearly so.           */
/*                                                                           */
/*  See my Robust Predicates paper for details.                              */
/*                                                                           */
/*****************************************************************************/

double CTIN::incircleadapt(point pa, point pb, point pc, point pd, double permanent)
{
        double adx, bdx, cdx, ady, bdy, cdy;
        double det, errbound;

        double bdxcdy1, cdxbdy1, cdxady1, adxcdy1, adxbdy1, bdxady1;
        double bdxcdy0, cdxbdy0, cdxady0, adxcdy0, adxbdy0, bdxady0;
        double bc[4], ca[4], ab[4];
        double bc3, ca3, ab3;
        double axbc[8], axxbc[16], aybc[8], ayybc[16], adet[32];
        long  axbclen, axxbclen, aybclen, ayybclen, alen;
        double bxca[8], bxxca[16], byca[8], byyca[16], bdet[32];
        long  bxcalen, bxxcalen, bycalen, byycalen, blen;
        double cxab[8], cxxab[16], cyab[8], cyyab[16], cdet[32];
        long  cxablen, cxxablen, cyablen, cyyablen, clen;
        double abdet[64];
        long  ablen;
        double fin1[1152], fin2[1152];
        double *finnow, *finother, *finswap;
        long  finlength;

        double adxtail, bdxtail, cdxtail, adytail, bdytail, cdytail;
        double adxadx1, adyady1, bdxbdx1, bdybdy1, cdxcdx1, cdycdy1;
        double adxadx0, adyady0, bdxbdx0, bdybdy0, cdxcdx0, cdycdy0;
        double aa[4], bb[4], cc[4];
        double aa3, bb3, cc3;
        double ti1, tj1;
        double ti0, tj0;
        double u[4], v[4];
        double u3, v3;
        double temp8[8], temp16a[16], temp16b[16], temp16c[16];
        double temp32a[32], temp32b[32], temp48[48], temp64[64];
        long  temp8len, temp16alen, temp16blen, temp16clen;
        long  temp32alen, temp32blen, temp48len, temp64len;
        double axtbb[8], axtcc[8], aytbb[8], aytcc[8];
        long  axtbblen, axtcclen, aytbblen, aytcclen;
        double bxtaa[8], bxtcc[8], bytaa[8], bytcc[8];
        long  bxtaalen, bxtcclen, bytaalen, bytcclen;
        double cxtaa[8], cxtbb[8], cytaa[8], cytbb[8];
        long  cxtaalen, cxtbblen, cytaalen, cytbblen;
        double axtbc[8], aytbc[8], bxtca[8], bytca[8], cxtab[8], cytab[8];
        long  axtbclen, aytbclen, bxtcalen, bytcalen, cxtablen, cytablen;
        double axtbct[16], aytbct[16], bxtcat[16], bytcat[16], cxtabt[16], cytabt[16];
        long  axtbctlen, aytbctlen, bxtcatlen, bytcatlen, cxtabtlen, cytabtlen;
        double axtbctt[8], aytbctt[8], bxtcatt[8];
        double bytcatt[8], cxtabtt[8], cytabtt[8];
        long  axtbcttlen, aytbcttlen, bxtcattlen, bytcattlen, cxtabttlen, cytabttlen;
        double abt[8], bct[8], cat[8];
        long  abtlen, bctlen, catlen;
        double abtt[4], bctt[4], catt[4];
        long  abttlen, bcttlen, cattlen;
        double abtt3, bctt3, catt3;
        double negate;

        double bvirt;
        double avirt, bround, around;
        double c;
        double abig;
        double ahi, alo, bhi, blo;
        double err1, err2, err3;
        double _i, _j;
        double _0;

        adx = (double) (pa[0] - pd[0]);
        bdx = (double) (pb[0] - pd[0]);
        cdx = (double) (pc[0] - pd[0]);
        ady = (double) (pa[1] - pd[1]);
        bdy = (double) (pb[1] - pd[1]);
        cdy = (double) (pc[1] - pd[1]);

        Two_Product(bdx, cdy, bdxcdy1, bdxcdy0);
        Two_Product(cdx, bdy, cdxbdy1, cdxbdy0);
        Two_Two_Diff(bdxcdy1, bdxcdy0, cdxbdy1, cdxbdy0, bc3, bc[2], bc[1], bc[0]);
        bc[3] = bc3;
        axbclen = scale_expansion_zeroelim(4, bc, adx, axbc);
        axxbclen = scale_expansion_zeroelim(axbclen, axbc, adx, axxbc);
        aybclen = scale_expansion_zeroelim(4, bc, ady, aybc);
        ayybclen = scale_expansion_zeroelim(aybclen, aybc, ady, ayybc);
        alen = fast_expansion_sum_zeroelim(axxbclen, axxbc, ayybclen, ayybc, adet);

        Two_Product(cdx, ady, cdxady1, cdxady0);
        Two_Product(adx, cdy, adxcdy1, adxcdy0);
        Two_Two_Diff(cdxady1, cdxady0, adxcdy1, adxcdy0, ca3, ca[2], ca[1], ca[0]);
        ca[3] = ca3;
        bxcalen = scale_expansion_zeroelim(4, ca, bdx, bxca);
        bxxcalen = scale_expansion_zeroelim(bxcalen, bxca, bdx, bxxca);
        bycalen = scale_expansion_zeroelim(4, ca, bdy, byca);
        byycalen = scale_expansion_zeroelim(bycalen, byca, bdy, byyca);
        blen = fast_expansion_sum_zeroelim(bxxcalen, bxxca, byycalen, byyca, bdet);

        Two_Product(adx, bdy, adxbdy1, adxbdy0);
        Two_Product(bdx, ady, bdxady1, bdxady0);
        Two_Two_Diff(adxbdy1, adxbdy0, bdxady1, bdxady0, ab3, ab[2], ab[1], ab[0]);
        ab[3] = ab3;
        cxablen = scale_expansion_zeroelim(4, ab, cdx, cxab);
        cxxablen = scale_expansion_zeroelim(cxablen, cxab, cdx, cxxab);
        cyablen = scale_expansion_zeroelim(4, ab, cdy, cyab);
        cyyablen = scale_expansion_zeroelim(cyablen, cyab, cdy, cyyab);
        clen = fast_expansion_sum_zeroelim(cxxablen, cxxab, cyyablen, cyyab, cdet);

        ablen = fast_expansion_sum_zeroelim(alen, adet, blen, bdet, abdet);
        finlength = fast_expansion_sum_zeroelim(ablen, abdet, clen, cdet, fin1);

        det = estimate(finlength, fin1);
        errbound = iccerrboundB * permanent;
        if ((det >= errbound) || (-det >= errbound)) {
                return det;
        }

        Two_Diff_Tail(pa[0], pd[0], adx, adxtail);
        Two_Diff_Tail(pa[1], pd[1], ady, adytail);
        Two_Diff_Tail(pb[0], pd[0], bdx, bdxtail);
        Two_Diff_Tail(pb[1], pd[1], bdy, bdytail);
        Two_Diff_Tail(pc[0], pd[0], cdx, cdxtail);
        Two_Diff_Tail(pc[1], pd[1], cdy, cdytail);
        if ((adxtail == 0.0) && (bdxtail == 0.0) && (cdxtail == 0.0)
                && (adytail == 0.0) && (bdytail == 0.0) && (cdytail == 0.0)) {
                return det;
        }

        errbound = iccerrboundC * permanent + resulterrbound * Absolute(det);
        det += ((adx * adx + ady * ady) * ((bdx * cdytail + cdy * bdxtail)
                - (bdy * cdxtail + cdx * bdytail))
                + 2.0 * (adx * adxtail + ady * adytail) * (bdx * cdy - bdy * cdx))
                + ((bdx * bdx + bdy * bdy) * ((cdx * adytail + ady * cdxtail)
                - (cdy * adxtail + adx * cdytail))
                + 2.0 * (bdx * bdxtail + bdy * bdytail) * (cdx * ady - cdy * adx))
                + ((cdx * cdx + cdy * cdy) * ((adx * bdytail + bdy * adxtail)
                - (ady * bdxtail + bdx * adytail))
                + 2.0 * (cdx * cdxtail + cdy * cdytail) * (adx * bdy - ady * bdx));
        if ((det >= errbound) || (-det >= errbound)) {
                return det;
        }

        finnow = fin1;
        finother = fin2;

        if ((bdxtail != 0.0) || (bdytail != 0.0)
                || (cdxtail != 0.0) || (cdytail != 0.0)) {
                Square(adx, adxadx1, adxadx0);
                Square(ady, adyady1, adyady0);
                Two_Two_Sum(adxadx1, adxadx0, adyady1, adyady0, aa3, aa[2], aa[1], aa[0]);
                aa[3] = aa3;
        }
        if ((cdxtail != 0.0) || (cdytail != 0.0)
                || (adxtail != 0.0) || (adytail != 0.0)) {
                Square(bdx, bdxbdx1, bdxbdx0);
                Square(bdy, bdybdy1, bdybdy0);
                Two_Two_Sum(bdxbdx1, bdxbdx0, bdybdy1, bdybdy0, bb3, bb[2], bb[1], bb[0]);
                bb[3] = bb3;
        }
        if ((adxtail != 0.0) || (adytail != 0.0)
                || (bdxtail != 0.0) || (bdytail != 0.0)) {
                Square(cdx, cdxcdx1, cdxcdx0);
                Square(cdy, cdycdy1, cdycdy0);
                Two_Two_Sum(cdxcdx1, cdxcdx0, cdycdy1, cdycdy0, cc3, cc[2], cc[1], cc[0]);
                cc[3] = cc3;
        }

        if (adxtail != 0.0) {
                axtbclen = scale_expansion_zeroelim(4, bc, adxtail, axtbc);
                temp16alen = scale_expansion_zeroelim(axtbclen, axtbc, 2.0 * adx,
                        temp16a);

                axtcclen = scale_expansion_zeroelim(4, cc, adxtail, axtcc);
                temp16blen = scale_expansion_zeroelim(axtcclen, axtcc, bdy, temp16b);

                axtbblen = scale_expansion_zeroelim(4, bb, adxtail, axtbb);
                temp16clen = scale_expansion_zeroelim(axtbblen, axtbb, -cdy, temp16c);

                temp32alen = fast_expansion_sum_zeroelim(temp16alen, temp16a,
                        temp16blen, temp16b, temp32a);
                temp48len = fast_expansion_sum_zeroelim(temp16clen, temp16c,
                        temp32alen, temp32a, temp48);
                finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp48len,
                        temp48, finother);
                finswap = finnow; finnow = finother; finother = finswap;
        }
        if (adytail != 0.0) {
                aytbclen = scale_expansion_zeroelim(4, bc, adytail, aytbc);
                temp16alen = scale_expansion_zeroelim(aytbclen, aytbc, 2.0 * ady,
                        temp16a);

                aytbblen = scale_expansion_zeroelim(4, bb, adytail, aytbb);
                temp16blen = scale_expansion_zeroelim(aytbblen, aytbb, cdx, temp16b);

                aytcclen = scale_expansion_zeroelim(4, cc, adytail, aytcc);
                temp16clen = scale_expansion_zeroelim(aytcclen, aytcc, -bdx, temp16c);

                temp32alen = fast_expansion_sum_zeroelim(temp16alen, temp16a,
                        temp16blen, temp16b, temp32a);
                temp48len = fast_expansion_sum_zeroelim(temp16clen, temp16c,
                        temp32alen, temp32a, temp48);
                finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp48len,
                        temp48, finother);
                finswap = finnow; finnow = finother; finother = finswap;
        }
        if (bdxtail != 0.0) {
                bxtcalen = scale_expansion_zeroelim(4, ca, bdxtail, bxtca);
                temp16alen = scale_expansion_zeroelim(bxtcalen, bxtca, 2.0 * bdx,
                        temp16a);

                bxtaalen = scale_expansion_zeroelim(4, aa, bdxtail, bxtaa);
                temp16blen = scale_expansion_zeroelim(bxtaalen, bxtaa, cdy, temp16b);

                bxtcclen = scale_expansion_zeroelim(4, cc, bdxtail, bxtcc);
                temp16clen = scale_expansion_zeroelim(bxtcclen, bxtcc, -ady, temp16c);

                temp32alen = fast_expansion_sum_zeroelim(temp16alen, temp16a,
                        temp16blen, temp16b, temp32a);
                temp48len = fast_expansion_sum_zeroelim(temp16clen, temp16c,
                        temp32alen, temp32a, temp48);
                finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp48len,
                        temp48, finother);
                finswap = finnow; finnow = finother; finother = finswap;
        }
        if (bdytail != 0.0) {
                bytcalen = scale_expansion_zeroelim(4, ca, bdytail, bytca);
                temp16alen = scale_expansion_zeroelim(bytcalen, bytca, 2.0 * bdy,
                        temp16a);

                bytcclen = scale_expansion_zeroelim(4, cc, bdytail, bytcc);
                temp16blen = scale_expansion_zeroelim(bytcclen, bytcc, adx, temp16b);

                bytaalen = scale_expansion_zeroelim(4, aa, bdytail, bytaa);
                temp16clen = scale_expansion_zeroelim(bytaalen, bytaa, -cdx, temp16c);

                temp32alen = fast_expansion_sum_zeroelim(temp16alen, temp16a,
                        temp16blen, temp16b, temp32a);
                temp48len = fast_expansion_sum_zeroelim(temp16clen, temp16c,
                        temp32alen, temp32a, temp48);
                finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp48len,
                        temp48, finother);
                finswap = finnow; finnow = finother; finother = finswap;
        }
        if (cdxtail != 0.0) {
                cxtablen = scale_expansion_zeroelim(4, ab, cdxtail, cxtab);
                temp16alen = scale_expansion_zeroelim(cxtablen, cxtab, 2.0 * cdx,
                        temp16a);

                cxtbblen = scale_expansion_zeroelim(4, bb, cdxtail, cxtbb);
                temp16blen = scale_expansion_zeroelim(cxtbblen, cxtbb, ady, temp16b);

                cxtaalen = scale_expansion_zeroelim(4, aa, cdxtail, cxtaa);
                temp16clen = scale_expansion_zeroelim(cxtaalen, cxtaa, -bdy, temp16c);

                temp32alen = fast_expansion_sum_zeroelim(temp16alen, temp16a,
                        temp16blen, temp16b, temp32a);
                temp48len = fast_expansion_sum_zeroelim(temp16clen, temp16c,
                        temp32alen, temp32a, temp48);
                finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp48len,
                        temp48, finother);
                finswap = finnow; finnow = finother; finother = finswap;
        }
        if (cdytail != 0.0) {
                cytablen = scale_expansion_zeroelim(4, ab, cdytail, cytab);
                temp16alen = scale_expansion_zeroelim(cytablen, cytab, 2.0 * cdy,
                        temp16a);

                cytaalen = scale_expansion_zeroelim(4, aa, cdytail, cytaa);
                temp16blen = scale_expansion_zeroelim(cytaalen, cytaa, bdx, temp16b);

                cytbblen = scale_expansion_zeroelim(4, bb, cdytail, cytbb);
                temp16clen = scale_expansion_zeroelim(cytbblen, cytbb, -adx, temp16c);

                temp32alen = fast_expansion_sum_zeroelim(temp16alen, temp16a,
                        temp16blen, temp16b, temp32a);
                temp48len = fast_expansion_sum_zeroelim(temp16clen, temp16c,
                        temp32alen, temp32a, temp48);
                finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp48len,
                        temp48, finother);
                finswap = finnow; finnow = finother; finother = finswap;
        }

        if ((adxtail != 0.0) || (adytail != 0.0)) {
                if ((bdxtail != 0.0) || (bdytail != 0.0)
                        || (cdxtail != 0.0) || (cdytail != 0.0)) {
                        Two_Product(bdxtail, cdy, ti1, ti0);
                        Two_Product(bdx, cdytail, tj1, tj0);
                        Two_Two_Sum(ti1, ti0, tj1, tj0, u3, u[2], u[1], u[0]);
                        u[3] = u3;
                        negate = -bdy;
                        Two_Product(cdxtail, negate, ti1, ti0);
                        negate = -bdytail;
                        Two_Product(cdx, negate, tj1, tj0);
                        Two_Two_Sum(ti1, ti0, tj1, tj0, v3, v[2], v[1], v[0]);
                        v[3] = v3;
                        bctlen = fast_expansion_sum_zeroelim(4, u, 4, v, bct);

                        Two_Product(bdxtail, cdytail, ti1, ti0);
                        Two_Product(cdxtail, bdytail, tj1, tj0);
                        Two_Two_Diff(ti1, ti0, tj1, tj0, bctt3, bctt[2], bctt[1], bctt[0]);
                        bctt[3] = bctt3;
                        bcttlen = 4;
                } else {
                        bct[0] = 0.0;
                        bctlen = 1;
                        bctt[0] = 0.0;
                        bcttlen = 1;
                }

                if (adxtail != 0.0) {
                        temp16alen = scale_expansion_zeroelim(axtbclen, axtbc, adxtail, temp16a);
                        axtbctlen = scale_expansion_zeroelim(bctlen, bct, adxtail, axtbct);
                        temp32alen = scale_expansion_zeroelim(axtbctlen, axtbct, 2.0 * adx,
                                temp32a);
                        temp48len = fast_expansion_sum_zeroelim(temp16alen, temp16a,
                                temp32alen, temp32a, temp48);
                        finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp48len,
                                temp48, finother);
                        finswap = finnow; finnow = finother; finother = finswap;
                        if (bdytail != 0.0) {
                                temp8len = scale_expansion_zeroelim(4, cc, adxtail, temp8);
                                temp16alen = scale_expansion_zeroelim(temp8len, temp8, bdytail,
                                        temp16a);
                                finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp16alen,
                                        temp16a, finother);
                                finswap = finnow; finnow = finother; finother = finswap;
                        }
                        if (cdytail != 0.0) {
                                temp8len = scale_expansion_zeroelim(4, bb, -adxtail, temp8);
                                temp16alen = scale_expansion_zeroelim(temp8len, temp8, cdytail,
                                        temp16a);
                                finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp16alen,
                                        temp16a, finother);
                                finswap = finnow; finnow = finother; finother = finswap;
                        }

                        temp32alen = scale_expansion_zeroelim(axtbctlen, axtbct, adxtail,
                                temp32a);
                        axtbcttlen = scale_expansion_zeroelim(bcttlen, bctt, adxtail, axtbctt);
                        temp16alen = scale_expansion_zeroelim(axtbcttlen, axtbctt, 2.0 * adx,
                                temp16a);
                        temp16blen = scale_expansion_zeroelim(axtbcttlen, axtbctt, adxtail,
                                temp16b);
                        temp32blen = fast_expansion_sum_zeroelim(temp16alen, temp16a,
                                temp16blen, temp16b, temp32b);
                        temp64len = fast_expansion_sum_zeroelim(temp32alen, temp32a,
                                temp32blen, temp32b, temp64);
                        finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp64len,
                                temp64, finother);
                        finswap = finnow; finnow = finother; finother = finswap;
                }
                if (adytail != 0.0) {
                        temp16alen = scale_expansion_zeroelim(aytbclen, aytbc, adytail, temp16a);
                        aytbctlen = scale_expansion_zeroelim(bctlen, bct, adytail, aytbct);
                        temp32alen = scale_expansion_zeroelim(aytbctlen, aytbct, 2.0 * ady,
                                temp32a);
                        temp48len = fast_expansion_sum_zeroelim(temp16alen, temp16a,
                                temp32alen, temp32a, temp48);
                        finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp48len,
                                temp48, finother);
                        finswap = finnow; finnow = finother; finother = finswap;


                        temp32alen = scale_expansion_zeroelim(aytbctlen, aytbct, adytail,
                                temp32a);
                        aytbcttlen = scale_expansion_zeroelim(bcttlen, bctt, adytail, aytbctt);
                        temp16alen = scale_expansion_zeroelim(aytbcttlen, aytbctt, 2.0 * ady,
                                temp16a);
                        temp16blen = scale_expansion_zeroelim(aytbcttlen, aytbctt, adytail,
                                temp16b);
                        temp32blen = fast_expansion_sum_zeroelim(temp16alen, temp16a,
                                temp16blen, temp16b, temp32b);
                        temp64len = fast_expansion_sum_zeroelim(temp32alen, temp32a,
                                temp32blen, temp32b, temp64);
                        finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp64len,
                                temp64, finother);
                        finswap = finnow; finnow = finother; finother = finswap;
                }
        }
        if ((bdxtail != 0.0) || (bdytail != 0.0)) {
                if ((cdxtail != 0.0) || (cdytail != 0.0)
                        || (adxtail != 0.0) || (adytail != 0.0)) {
                        Two_Product(cdxtail, ady, ti1, ti0);
                        Two_Product(cdx, adytail, tj1, tj0);
                        Two_Two_Sum(ti1, ti0, tj1, tj0, u3, u[2], u[1], u[0]);
                        u[3] = u3;
                        negate = -cdy;
                        Two_Product(adxtail, negate, ti1, ti0);
                        negate = -cdytail;
                        Two_Product(adx, negate, tj1, tj0);
                        Two_Two_Sum(ti1, ti0, tj1, tj0, v3, v[2], v[1], v[0]);
                        v[3] = v3;
                        catlen = fast_expansion_sum_zeroelim(4, u, 4, v, cat);

                        Two_Product(cdxtail, adytail, ti1, ti0);
                        Two_Product(adxtail, cdytail, tj1, tj0);
                        Two_Two_Diff(ti1, ti0, tj1, tj0, catt3, catt[2], catt[1], catt[0]);
                        catt[3] = catt3;
                        cattlen = 4;
                } else {
                        cat[0] = 0.0;
                        catlen = 1;
                        catt[0] = 0.0;
                        cattlen = 1;
                }

                if (bdxtail != 0.0) {
                        temp16alen = scale_expansion_zeroelim(bxtcalen, bxtca, bdxtail, temp16a);
                        bxtcatlen = scale_expansion_zeroelim(catlen, cat, bdxtail, bxtcat);
                        temp32alen = scale_expansion_zeroelim(bxtcatlen, bxtcat, 2.0 * bdx,
                                temp32a);
                        temp48len = fast_expansion_sum_zeroelim(temp16alen, temp16a,
                                temp32alen, temp32a, temp48);
                        finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp48len,
                                temp48, finother);
                        finswap = finnow; finnow = finother; finother = finswap;
                        if (cdytail != 0.0) {
                                temp8len = scale_expansion_zeroelim(4, aa, bdxtail, temp8);
                                temp16alen = scale_expansion_zeroelim(temp8len, temp8, cdytail,
                                        temp16a);
                                finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp16alen,
                                        temp16a, finother);
                                finswap = finnow; finnow = finother; finother = finswap;
                        }
                        if (adytail != 0.0) {
                                temp8len = scale_expansion_zeroelim(4, cc, -bdxtail, temp8);
                                temp16alen = scale_expansion_zeroelim(temp8len, temp8, adytail,
                                        temp16a);
                                finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp16alen,
                                        temp16a, finother);
                                finswap = finnow; finnow = finother; finother = finswap;
                        }

                        temp32alen = scale_expansion_zeroelim(bxtcatlen, bxtcat, bdxtail,
                                temp32a);
                        bxtcattlen = scale_expansion_zeroelim(cattlen, catt, bdxtail, bxtcatt);
                        temp16alen = scale_expansion_zeroelim(bxtcattlen, bxtcatt, 2.0 * bdx,
                                temp16a);
                        temp16blen = scale_expansion_zeroelim(bxtcattlen, bxtcatt, bdxtail,
                                temp16b);
                        temp32blen = fast_expansion_sum_zeroelim(temp16alen, temp16a,
                                temp16blen, temp16b, temp32b);
                        temp64len = fast_expansion_sum_zeroelim(temp32alen, temp32a,
                                temp32blen, temp32b, temp64);
                        finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp64len,
                                temp64, finother);
                        finswap = finnow; finnow = finother; finother = finswap;
                }
                if (bdytail != 0.0) {
                        temp16alen = scale_expansion_zeroelim(bytcalen, bytca, bdytail, temp16a);
                        bytcatlen = scale_expansion_zeroelim(catlen, cat, bdytail, bytcat);
                        temp32alen = scale_expansion_zeroelim(bytcatlen, bytcat, 2.0 * bdy,
                                temp32a);
                        temp48len = fast_expansion_sum_zeroelim(temp16alen, temp16a,
                                temp32alen, temp32a, temp48);
                        finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp48len,
                                temp48, finother);
                        finswap = finnow; finnow = finother; finother = finswap;


                        temp32alen = scale_expansion_zeroelim(bytcatlen, bytcat, bdytail,
                                temp32a);
                        bytcattlen = scale_expansion_zeroelim(cattlen, catt, bdytail, bytcatt);
                        temp16alen = scale_expansion_zeroelim(bytcattlen, bytcatt, 2.0 * bdy,
                                temp16a);
                        temp16blen = scale_expansion_zeroelim(bytcattlen, bytcatt, bdytail,
                                temp16b);
                        temp32blen = fast_expansion_sum_zeroelim(temp16alen, temp16a,
                                temp16blen, temp16b, temp32b);
                        temp64len = fast_expansion_sum_zeroelim(temp32alen, temp32a,
                                temp32blen, temp32b, temp64);
                        finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp64len,
                                temp64, finother);
                        finswap = finnow; finnow = finother; finother = finswap;
                }
        }
        if ((cdxtail != 0.0) || (cdytail != 0.0)) {
                if ((adxtail != 0.0) || (adytail != 0.0)
                        || (bdxtail != 0.0) || (bdytail != 0.0)) {
                        Two_Product(adxtail, bdy, ti1, ti0);
                        Two_Product(adx, bdytail, tj1, tj0);
                        Two_Two_Sum(ti1, ti0, tj1, tj0, u3, u[2], u[1], u[0]);
                        u[3] = u3;
                        negate = -ady;
                        Two_Product(bdxtail, negate, ti1, ti0);
                        negate = -adytail;
                        Two_Product(bdx, negate, tj1, tj0);
                        Two_Two_Sum(ti1, ti0, tj1, tj0, v3, v[2], v[1], v[0]);
                        v[3] = v3;
                        abtlen = fast_expansion_sum_zeroelim(4, u, 4, v, abt);

                        Two_Product(adxtail, bdytail, ti1, ti0);
                        Two_Product(bdxtail, adytail, tj1, tj0);
                        Two_Two_Diff(ti1, ti0, tj1, tj0, abtt3, abtt[2], abtt[1], abtt[0]);
                        abtt[3] = abtt3;
                        abttlen = 4;
                } else {
                        abt[0] = 0.0;
                        abtlen = 1;
                        abtt[0] = 0.0;
                        abttlen = 1;
                }

                if (cdxtail != 0.0) {
                        temp16alen = scale_expansion_zeroelim(cxtablen, cxtab, cdxtail, temp16a);
                        cxtabtlen = scale_expansion_zeroelim(abtlen, abt, cdxtail, cxtabt);
                        temp32alen = scale_expansion_zeroelim(cxtabtlen, cxtabt, 2.0 * cdx,
                                temp32a);
                        temp48len = fast_expansion_sum_zeroelim(temp16alen, temp16a,
                                temp32alen, temp32a, temp48);
                        finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp48len,
                                temp48, finother);
                        finswap = finnow; finnow = finother; finother = finswap;
                        if (adytail != 0.0) {
                                temp8len = scale_expansion_zeroelim(4, bb, cdxtail, temp8);
                                temp16alen = scale_expansion_zeroelim(temp8len, temp8, adytail,
                                        temp16a);
                                finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp16alen,
                                        temp16a, finother);
                                finswap = finnow; finnow = finother; finother = finswap;
                        }
                        if (bdytail != 0.0) {
                                temp8len = scale_expansion_zeroelim(4, aa, -cdxtail, temp8);
                                temp16alen = scale_expansion_zeroelim(temp8len, temp8, bdytail,
                                        temp16a);
                                finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp16alen,
                                        temp16a, finother);
                                finswap = finnow; finnow = finother; finother = finswap;
                        }

                        temp32alen = scale_expansion_zeroelim(cxtabtlen, cxtabt, cdxtail,
                                temp32a);
                        cxtabttlen = scale_expansion_zeroelim(abttlen, abtt, cdxtail, cxtabtt);
                        temp16alen = scale_expansion_zeroelim(cxtabttlen, cxtabtt, 2.0 * cdx,
                                temp16a);
                        temp16blen = scale_expansion_zeroelim(cxtabttlen, cxtabtt, cdxtail,
                                temp16b);
                        temp32blen = fast_expansion_sum_zeroelim(temp16alen, temp16a,
                                temp16blen, temp16b, temp32b);
                        temp64len = fast_expansion_sum_zeroelim(temp32alen, temp32a,
                                temp32blen, temp32b, temp64);
                        finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp64len,
                                temp64, finother);
                        finswap = finnow; finnow = finother; finother = finswap;
                }
                if (cdytail != 0.0) {
                        temp16alen = scale_expansion_zeroelim(cytablen, cytab, cdytail, temp16a);
                        cytabtlen = scale_expansion_zeroelim(abtlen, abt, cdytail, cytabt);
                        temp32alen = scale_expansion_zeroelim(cytabtlen, cytabt, 2.0 * cdy,
                                temp32a);
                        temp48len = fast_expansion_sum_zeroelim(temp16alen, temp16a,
                                temp32alen, temp32a, temp48);

                        finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp48len,
                                temp48, finother);
                        finswap = finnow; finnow = finother; finother = finswap;


                        temp32alen = scale_expansion_zeroelim(cytabtlen, cytabt, cdytail,
                                temp32a);
                        cytabttlen = scale_expansion_zeroelim(abttlen, abtt, cdytail, cytabtt);
                        temp16alen = scale_expansion_zeroelim(cytabttlen, cytabtt, 2.0 * cdy,
                                temp16a);
                        temp16blen = scale_expansion_zeroelim(cytabttlen, cytabtt, cdytail,
                                temp16b);
                        temp32blen = fast_expansion_sum_zeroelim(temp16alen, temp16a,
                                temp16blen, temp16b, temp32b);
                        temp64len = fast_expansion_sum_zeroelim(temp32alen, temp32a,
                                temp32blen, temp32b, temp64);
                        finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp64len,
                                temp64, finother);
                        finswap = finnow; finnow = finother; finother = finswap;
                }
        }

        return finnow[finlength - 1];
}

double CTIN::incircle(point pa, point pb, point pc, point pd)
{
        double adx, bdx, cdx, ady, bdy, cdy;
        double bdxcdy, cdxbdy, cdxady, adxcdy, adxbdy, bdxady;
        double alift, blift, clift;
        double det;
        double permanent, errbound;

        incirclecount++;

        adx = pa[0] - pd[0];
        bdx = pb[0] - pd[0];
        cdx = pc[0] - pd[0];
        ady = pa[1] - pd[1];
        bdy = pb[1] - pd[1];
        cdy = pc[1] - pd[1];

        bdxcdy = bdx * cdy;
        cdxbdy = cdx * bdy;
        alift = adx * adx + ady * ady;

        cdxady = cdx * ady;
        adxcdy = adx * cdy;
        blift = bdx * bdx + bdy * bdy;

        adxbdy = adx * bdy;
        bdxady = bdx * ady;
        clift = cdx * cdx + cdy * cdy;

        det = alift * (bdxcdy - cdxbdy)
                + blift * (cdxady - adxcdy)
                + clift * (adxbdy - bdxady);

        permanent = (Absolute(bdxcdy) + Absolute(cdxbdy)) * alift
                + (Absolute(cdxady) + Absolute(adxcdy)) * blift
                + (Absolute(adxbdy) + Absolute(bdxady)) * clift;
        errbound = iccerrboundA * permanent;
        if ((det > errbound) || (-det > errbound)) {
                return det;
        }

        return incircleadapt(pa, pb, pc, pd, permanent);
}

/**                                                                         **/
/**                                                                         **/
/********* Determinant evaluation routines end here                  *********/

/*****************************************************************************/
/*                                                                           */
/*  triangleinit()   Initialize some variables.                              */
/*                                                                           */
/*****************************************************************************/

void CTIN::triangleinit()
{
        points.maxitems = triangles.maxitems = shelles.maxitems = viri.maxitems =
                badsegments.maxitems = badtriangles.maxitems = splaynodes.maxitems = 0l;
        points.itembytes = triangles.itembytes = shelles.itembytes = viri.itembytes =
                badsegments.itembytes = badtriangles.itembytes = splaynodes.itembytes = 0;
        recenttri.tri = (triangle *) NULL;    /* No triangle has been visited yet. */
        samples = 1;            /* CTPoint location should take at least one sample. */
        checksegments = 0;      /* There are no segments in the triangulation yet. */
        incirclecount = counterclockcount = hyperbolacount = 0;
        circumcentercount = circletopcount = 0;
        randomseed = 1;

        exactinit();                     /* Initialize exact arithmetic constants. */
}

/*****************************************************************************/
/*                                                                           */
/*  randomnation()   Generate a random number between 0 and `choices' - 1.   */
/*                                                                           */
/*  This is a simple linear congruential random number generator.  Hence, it */
/*  is a bad random number generator, but good enough for most randomized    */
/*  geometric algorithms.                                                    */
/*                                                                           */
/*****************************************************************************/

unsigned long CTIN::randomnation(unsigned long  choices)
{
        randomseed = (randomseed * 1366l + 150889l) % 714025l;
        return randomseed / (714025l / choices + 1);
}


/********* CTPoint location routines begin here                        *********/
/**                                                                         **/
/**                                                                         **/

/*****************************************************************************/
/*                                                                           */
/*  makepointmap()   Construct a mapping from points to triangles to improve  */
/*                  the speed of point location for segment insertion.       */
/*                                                                           */
/*  Traverses all the triangles, and provides each corner of each triangle   */
/*  with a pointer to that triangle.  Of course, pointers will be            */
/*  overwritten by other pointers because (almost) each point is a corner    */
/*  of several triangles, but in the end every point will point to some      */
/*  triangle that contains it.                                               */

/*                                                                           */
/*****************************************************************************/

void CTIN::makepointmap()
{
        struct CTriEdge triangleloop;
        point triorg;

        //  if (verbose) {    printf("    Constructing mapping from points to triangles.\n");  }
        traversalinit(&triangles);
        triangleloop.tri = triangletraverse();
        while (triangleloop.tri != (triangle *) NULL)
        {
                /* Check all three points of the triangle. */
                for (triangleloop.orient = 0; triangleloop.orient < 3;	triangleloop.orient++)
                {
                        org(triangleloop, triorg);
                        setpoint2tri(triorg, encode(triangleloop));
                }
                triangleloop.tri = triangletraverse();
        }
}

/*****************************************************************************/
/*                                                                           */
/*  preciselocate()   Find a triangle or edge containing a given point.      */
/*                                                                           */
/*  Begins its search from `searchtri'.  It is important that `searchtri'    */
/*  be a handle with the property that `searchpoint' is strictly to the left */
/*  of the edge denoted by `searchtri', or is collinear with that edge and   */
/*  does not intersect that edge.  (In particular, `searchpoint' should not  */
/*  be the origin or destination of that edge.)                              */
/*                                                                           */

/*  These conditions are imposed because preciselocate() is normally used in */
/*  one of two situations:                                                   */
/*                                                                           */
/*  (1)  To try to find the location to insert a new point.  Normally, we    */
/*       know an edge that the point is strictly to the left of.  In the     */
/*       incremental Delaunay algorithm, that edge is a bounding box edge.   */
/*       In Ruppert's Delaunay refinement algorithm for quality meshing,     */
/*       that edge is the shortest edge of the triangle whose circumcenter   */
/*       is being inserted.                                                  */
/*                                                                           */
/*  (2)  To try to find an existing point.  In this case, any edge on the    */
/*       convex hull is a good starting edge.  The possibility that the      */
/*       vertex one seeks is an endpoint of the starting edge must be        */
/*       screened out before preciselocate() is called.                      */
/*                                                                           */
/*  On completion, `searchtri' is a triangle that contains `searchpoint'.    */
/*                                                                           */
/*  This implementation differs from that given by Guibas and Stolfi.  It    */
/*  walks from triangle to triangle, crossing an edge only if `searchpoint'  */
/*  is on the other side of the line containing that edge.  After entering   */
/*  a triangle, there are two edges by which one can leave that triangle.    */
/*  If both edges are valid (`searchpoint' is on the other side of both      */
/*  edges), one of the two is chosen by drawing a line perpendicular to      */
/*  the entry edge (whose endpoints are `forg' and `fdest') passing through  */
/*  `fapex'.  Depending on which side of this perpendicular `searchpoint'    */
/*  falls on, an exit edge is chosen.                                        */
/*                                                                           */
/*  This implementation is empirically faster than the Guibas and Stolfi     */
/*  point location routine (which I originally used), which tends to spiral  */
/*  in toward its target.                                                    */
/*                                                                           */
/*  Returns ONVERTEX if the point lies on an existing vertex.  `searchtri'   */
/*  is a handle whose origin is the existing vertex.                         */
/*                                                                           */
/*  Returns ONEDGE if the point lies on a mesh edge.  `searchtri' is a       */
/*  handle whose primary edge is the edge on which the point lies.           */
/*                                                                           */
/*  Returns INTRIANGLE if the point lies strictly within a triangle.         */
/*  `searchtri' is a handle on the triangle that contains the point.         */
/*                                                                           */
/*  Returns OUTSIDE if the point lies outside the mesh.  `searchtri' is a    */
/*  handle whose primary edge the point is to the right of.  This might      */
/*  occur when the circumcenter of a triangle falls just slightly outside    */
/*  the mesh due to floating-point roundoff error.  It also occurs when      */
/*  seeking a hole or region point that a foolish user has placed outside    */
/*  the mesh.                                                                */
/*                                                                           */
/*  WARNING:  This routine is designed for convex triangulations, and will   */
/*  not generally work after the holes and concavities have been carved.     */
/*  However, it can still be used to find the circumcenter of a triangle, as */
/*  long as the search is begun from the triangle in question.               */
/*                                                                           */
/*****************************************************************************/

enum locateresult CTIN::preciselocate(point searchpoint, struct CTriEdge *searchtri)
{
        struct CTriEdge backtracktri;
        point forg, fdest, fapex;
        point swappoint;
        double orgorient, destorient;
        long  moveleft;
        triangle ptr;                         /* Temporary variable used by sym(). */

        //  if (verbose > 2) {    printf("  Searching for point (%.12g, %.12g).\n", searchpoint[0], searchpoint[1]);  }
        /* Where are we? */
        org(*searchtri, forg);
        dest(*searchtri, fdest);
        apex(*searchtri, fapex);
        while (1) {
                //    if (verbose > 2) {
                //		printf("    At (%.12g, %.12g) (%.12g, %.12g) (%.12g, %.12g)\n",
                //            forg[0], forg[1], fdest[0], fdest[1], fapex[0], fapex[1]);
                //    }
                /* Check whether the apex is the point we seek. */
                if ((fapex[0] == searchpoint[0]) && (fapex[1] == searchpoint[1])) {
                        lprevself(*searchtri);
                        return ONVERTEX;
                }
                /* Does the point lie on the other side of the line defined by the */
                /*   triangle edge opposite the triangle's destination?            */
                destorient = counterclockwise(forg, fapex, searchpoint);
                /* Does the point lie on the other side of the line defined by the */
                /*   triangle edge opposite the triangle's origin?                 */
                orgorient = counterclockwise(fapex, fdest, searchpoint);
                if (destorient > 0.0) {
                        if (orgorient > 0.0) {
                                /* Move left if the inner product of (fapex - searchpoint) and  */
                                /*   (fdest - forg) is positive.  This is equivalent to drawing */
                                /*   a line perpendicular to the line (forg, fdest) passing     */
                                /*   through `fapex', and determining which side of this line   */
                                /*   `searchpoint' falls on.                                    */
                                moveleft = (fapex[0] - searchpoint[0]) * (fdest[0] - forg[0]) +
                                        (fapex[1] - searchpoint[1]) * (fdest[1] - forg[1]) > 0.0;
                        } else {
                                moveleft = 1;
                        }
                } else {
                        if (orgorient > 0.0) {
                                moveleft = 0;
                        } else {
                                /* The point we seek must be on the boundary of or inside this */
                                /*   triangle.                                                 */
                                if (destorient == 0.0) {
                                        lprevself(*searchtri);
                                        return ONEDGE;
                                }
                                if (orgorient == 0.0) {
                                        lnextself(*searchtri);
                                        return ONEDGE;
                                }
                                return INTRIANGLE;
                        }
                }

                /* Move to another triangle.  Leave a trace `backtracktri' in case */
                /*   floating-point roundoff or some such bogey causes us to walk  */
                /*   off a boundary of the triangulation.  We can just bounce off  */
                /*   the boundary as if it were an elastic band.                   */
                if (moveleft) {
                        lprev(*searchtri, backtracktri);
                        fdest = fapex;
                } else {
                        lnext(*searchtri, backtracktri);
                        forg = fapex;
                }
                sym(backtracktri, *searchtri);

                /* Check for walking off the edge. */
                if (searchtri->tri == dummytri) {
                        /* Turn around. */
                        triedgecopy(backtracktri, *searchtri);
                        swappoint = forg;
                        forg = fdest;
                        fdest = swappoint;
                        apex(*searchtri, fapex);
                        /* Check if the point really is beyond the triangulation boundary. */
                        destorient = counterclockwise(forg, fapex, searchpoint);
                        orgorient = counterclockwise(fapex, fdest, searchpoint);
                        if ((orgorient < 0.0) && (destorient < 0.0)) {
                                return OUTSIDE;
                        }
                } else {
                        apex(*searchtri, fapex);
                }
        }
}

/*****************************************************************************/
/*                                                                           */
/*  locate()   Find a triangle or edge containing a given point.             */
/*                                                                           */
/*  Searching begins from one of:  the input `searchtri', a recently         */
/*  encountered triangle `recenttri', or from a triangle chosen from a       */
/*  random sample.  The choice is made by determining which triangle's       */
/*  origin is closest to the point we are searcing for.  Normally,           */
/*  `searchtri' should be a handle on the convex hull of the triangulation.  */
/*                                                                           */
/*  Details on the random sampling method can be found in the Mucke, Saias,  */
/*  and Zhu paper cited in the header of this code.                          */
/*                                                                           */
/*  On completion, `searchtri' is a triangle that contains `searchpoint'.    */
/*                                                                           */
/*  Returns ONVERTEX if the point lies on an existing vertex.  `searchtri'   */
/*  is a handle whose origin is the existing vertex.                         */
/*                                                                           */
/*  Returns ONEDGE if the point lies on a mesh edge.  `searchtri' is a       */
/*  handle whose primary edge is the edge on which the point lies.           */
/*                                                                           */
/*  Returns INTRIANGLE if the point lies strictly within a triangle.         */
/*  `searchtri' is a handle on the triangle that contains the point.         */
/*                                                                           */
/*  Returns OUTSIDE if the point lies outside the mesh.  `searchtri' is a    */
/*  handle whose primary edge the point is to the right of.  This might      */
/*  occur when the circumcenter of a triangle falls just slightly outside    */
/*  the mesh due to floating-point roundoff error.  It also occurs when      */
/*  seeking a hole or region point that a foolish user has placed outside    */
/*  the mesh.                                                                */
/*                                                                           */
/*  WARNING:  This routine is designed for convex triangulations, and will   */
/*  not generally work after the holes and concavities have been carved.     */
/*                                                                           */
/*****************************************************************************/

enum locateresult CTIN::locate(point searchpoint,struct CTriEdge * searchtri)
{
        long **sampleblock;
        triangle *firsttri;
        struct CTriEdge sampletri;
        point torg, tdest;
        unsigned long alignptr;
        double searchdist, dist;
        double ahead;
        long sampleblocks, samplesperblock, samplenum;
        long triblocks;
        long i, j;
        triangle ptr;                         /* Temporary variable used by sym(). */

        //  if (verbose > 2) {
        //    printf("  Randomly sampling for a triangle near point (%.12g, %.12g).\n",
        //           searchpoint[0], searchpoint[1]);
        //  }
        /* Record the distance from the suggested starting triangle to the */
        /*   point we seek.                                                */
        org(*searchtri, torg);
        searchdist = (searchpoint[0] - torg[0]) * (searchpoint[0] - torg[0])
                + (searchpoint[1] - torg[1]) * (searchpoint[1] - torg[1]);
        //  if (verbose > 2) {
        //    printf("    Boundary triangle has origin (%.12g, %.12g).\n",
        //           torg[0], torg[1]);
        //  }

        /* If a recently encountered triangle has been recorded and has not been */
        /*   deallocated, test it as a good starting point.                      */
        if (recenttri.tri != (triangle *) NULL) {
                if (recenttri.tri[3] != (triangle) NULL) {
                        org(recenttri, torg);
                        if ((torg[0] == searchpoint[0]) && (torg[1] == searchpoint[1])) {
                                triedgecopy(recenttri, *searchtri);
                                return ONVERTEX;
                        }
                        dist = (searchpoint[0] - torg[0]) * (searchpoint[0] - torg[0])
                                + (searchpoint[1] - torg[1]) * (searchpoint[1] - torg[1]);
                        if (dist < searchdist) {
                                triedgecopy(recenttri, *searchtri);
                                searchdist = dist;
                                /*        if (verbose > 2) {
                                printf("    Choosing recent triangle with origin (%.12g, %.12g).\n",
                                torg[0], torg[1]);
                                }
                                */
                        }
                }
        }

        /* The number of random samples taken is proportional to the cube root of */
        /*   the number of triangles in the mesh.  The next bit of code assumes   */
        /*   that the number of triangles increases monotonically.                */
        while (SAMPLEFACTOR * samples * samples * samples < triangles.items) {
                samples++;
        }
        triblocks = (triangles.maxitems + TRIPERBLOCK - 1) / TRIPERBLOCK;
        samplesperblock = 1 + (samples / triblocks);
        sampleblocks = samples / samplesperblock;
        sampleblock = triangles.firstblock;
        sampletri.orient = 0;

        for (i = 0; i < sampleblocks; i++) {
                alignptr = (unsigned long) (sampleblock + 1);
                firsttri = (triangle *) (alignptr + (unsigned long) triangles.alignbytes
                        - (alignptr % (unsigned long) triangles.alignbytes));
                for (j = 0; j < samplesperblock; j++) {
                        if (i == triblocks - 1) {
                                samplenum = randomnation((long )
                                        (triangles.maxitems - (i * TRIPERBLOCK)));
                        } else {
                                samplenum = randomnation(TRIPERBLOCK);
                        }
                        sampletri.tri = (triangle *)
                                (firsttri + (samplenum * triangles.itemwords));
                        if (sampletri.tri[3] != (triangle) NULL) {
                                org(sampletri, torg);
                                dist = (searchpoint[0] - torg[0]) * (searchpoint[0] - torg[0])
                                        + (searchpoint[1] - torg[1]) * (searchpoint[1] - torg[1]);
                                if (dist < searchdist) {
                                        triedgecopy(sampletri, *searchtri);
                                        searchdist = dist;
                                        /*          if (verbose > 2) {
                                        printf("    Choosing triangle with origin (%.12g, %.12g).\n",
                                        torg[0], torg[1]);
                                        }
                                        */
                                }
                        }
                }
                sampleblock = (long **) *sampleblock;
        }
        /* Where are we? */
        org(*searchtri, torg);
        dest(*searchtri, tdest);
        /* Check the starting triangle's vertices. */
        if ((torg[0] == searchpoint[0]) && (torg[1] == searchpoint[1])) {
                return ONVERTEX;
        }
        if ((tdest[0] == searchpoint[0]) && (tdest[1] == searchpoint[1])) {
                lnextself(*searchtri);
                return ONVERTEX;
        }
        /* Orient `searchtri' to fit the preconditions of calling preciselocate(). */
        ahead = counterclockwise(torg, tdest, searchpoint);
        if (ahead < 0.0) {
                /* Turn around so that `searchpoint' is to the left of the */
                /*   edge specified by `searchtri'.                        */
                symself(*searchtri);
        } else if (ahead == 0.0) {
                /* Check if `searchpoint' is between `torg' and `tdest'. */
                if (((torg[0] < searchpoint[0]) == (searchpoint[0] < tdest[0]))
                        && ((torg[1] < searchpoint[1]) == (searchpoint[1] < tdest[1]))) {
                        return ONEDGE;
                }
        }
        return preciselocate(searchpoint, searchtri);

}

/**                                                                         **/
/**                                                                         **/
/********* CTPoint location routines end here                          *********/

/********* Mesh transformation routines begin here                   *********/
/**                                                                         **/
/**                                                                         **/

/*****************************************************************************/
/*                                                                           */
/*  insertshelle()   Create a new shell edge and insert it between two       */
/*                   triangles.                                              */
/*                                                                           */
/*  The new shell edge is inserted at the edge described by the handle       */
/*  `tri'.  Its vertices are properly initialized.  The marker `shellemark'  */
/*  is applied to the shell edge and, if appropriate, its vertices.          */
/*                                                                           */
/*****************************************************************************/

//tri;          /* Edge at which to insert the new shell edge. */
//shellemark;                            /* Marker for the new shell edge. */
void CTIN::insertshelle(struct CTriEdge *tri, long  shellemark)
{
        struct CTriEdge oppotri;
        struct CEdge newshelle;
        point triorg, tridest;
        triangle ptr;                         /* Temporary variable used by sym(). */
        shelle sptr;                      /* Temporary variable used by tspivot(). */

        /* Mark points if possible. */
        org(*tri, triorg);
        dest(*tri, tridest);
        if (pointmark(triorg) == 0) {
                setpointmark(triorg, shellemark);
        }
        if (pointmark(tridest) == 0) {
                setpointmark(tridest, shellemark);
        }
        /* Check if there's already a shell edge here. */
        tspivot(*tri, newshelle);
        if (newshelle.sh == dummysh) {
                /* Make new shell edge and initialize its vertices. */
                makeshelle(&newshelle);
                setsorg(newshelle, tridest);
                setsdest(newshelle, triorg);
                /* Bond new shell edge to the two triangles it is sandwiched between. */
                /*   Note that the facing triangle `oppotri' might be equal to        */
                /*   `dummytri' (outer space), but the new shell edge is bonded to it */
                /*   all the same.                                                    */
                tsbond(*tri, newshelle);
                sym(*tri, oppotri);
                ssymself(newshelle);
                tsbond(oppotri, newshelle);
                setmark(newshelle, shellemark);
                //    if (verbose > 2) {
                //      printf("  Inserting new ");
                //      printshelle(&newshelle);
                //    }
        } else {
                if (mark(newshelle) == 0) {
                        setmark(newshelle, shellemark);

                }
        }
}

/*****************************************************************************/
/*                                                                           */
/*  Terminology                                                              */
/*                                                                           */
/*  A "local transformation" replaces a small set of triangles with another  */
/*  set of triangles.  This may or may not involve inserting or deleting a   */
/*  point.                                                                   */
/*                                                                           */
/*  The term "casing" is used to describe the set of triangles that are      */
/*  attached to the triangles being transformed, but are not transformed     */
/*  themselves.  Think of the casing as a fixed hollow structure inside      */
/*  which all the action happens.  A "casing" is only defined relative to    */
/*  a single transformation; each occurrence of a transformation will        */
/*  involve a different casing.                                              */
/*                                                                           */
/*  A "shell" is similar to a "casing".  The term "shell" describes the set  */
/*  of shell edges (if any) that are attached to the triangles being         */
/*  transformed.  However, I sometimes use "shell" to refer to a single      */
/*  shell edge, so don't get confused.                                       */
/*                                                                           */
/*****************************************************************************/


/*****************************************************************************/
/*                                                                           */
/*  flip()   Transform two triangles to two different triangles by flipping  */
/*           an edge within a quadrilateral.                                 */
/*                                                                           */
/*  Imagine the original triangles, abc and bad, oriented so that the        */
/*  shared edge ab lies in a horizontal plane, with the point b on the left  */
/*  and the point a on the right.  The point c lies below the edge, and the  */
/*  point d lies above the edge.  The `flipedge' handle holds the edge ab    */
/*  of triangle abc, and is directed left, from vertex a to vertex b.        */
/*                                                                           */
/*  The triangles abc and bad are deleted and replaced by the triangles cdb  */
/*  and dca.  The triangles that represent abc and bad are NOT deallocated;  */
/*  they are reused for dca and cdb, respectively.  Hence, any handles that  */
/*  may have held the original triangles are still valid, although not       */
/*  directed as they were before.                                            */
/*                                                                           */
/*  Upon completion of this routine, the `flipedge' handle holds the edge    */
/*  dc of triangle dca, and is directed down, from vertex d to vertex c.     */
/*  (Hence, the two triangles have rotated counterclockwise.)                */
/*                                                                           */
/*  WARNING:  This transformation is geometrically valid only if the         */
/*  quadrilateral adbc is convex.  Furthermore, this transformation is       */
/*  valid only if there is not a shell edge between the triangles abc and    */
/*  bad.  This routine does not check either of these preconditions, and     */
/*  it is the responsibility of the calling routine to ensure that they are  */
/*  met.  If they are not, the streets shall be filled with wailing and      */
/*  gnashing of teeth.                                                       */
/*                                                                           */
/*****************************************************************************/

void CTIN::flip(struct CTriEdge *flipedge)			/* Handle for the triangle abc. */

{
        struct CTriEdge botleft, botright;
        struct CTriEdge topleft, topright;
        struct CTriEdge top;
        struct CTriEdge botlcasing, botrcasing;
        struct CTriEdge toplcasing, toprcasing;
        struct CEdge botlshelle, botrshelle;
        struct CEdge toplshelle, toprshelle;
        point leftpoint, rightpoint, botpoint;
        point farpoint;
        triangle ptr;                         /* Temporary variable used by sym(). */
        shelle sptr;                      /* Temporary variable used by tspivot(). */

        /* Identify the vertices of the quadrilateral. */
        org(*flipedge, rightpoint);
        dest(*flipedge, leftpoint);
        apex(*flipedge, botpoint);
        sym(*flipedge, top);

        apex(top, farpoint);

        /* Identify the casing of the quadrilateral. */
        lprev(top, topleft);
        sym(topleft, toplcasing);
        lnext(top, topright);
        sym(topright, toprcasing);
        lnext(*flipedge, botleft);
        sym(botleft, botlcasing);
        lprev(*flipedge, botright);
        sym(botright, botrcasing);
        /* Rotate the quadrilateral one-quarter turn counterclockwise. */
        bond(topleft, botlcasing);
        bond(botleft, botrcasing);
        bond(botright, toprcasing);
        bond(topright, toplcasing);

        if (checksegments) {
                /* Check for shell edges and rebond them to the quadrilateral. */
                tspivot(topleft, toplshelle);
                tspivot(botleft, botlshelle);
                tspivot(botright, botrshelle);
                tspivot(topright, toprshelle);
                if (toplshelle.sh == dummysh) {
                        tsdissolve(topright);
                } else {
                        tsbond(topright, toplshelle);
                }
                if (botlshelle.sh == dummysh) {
                        tsdissolve(topleft);
                } else {
                        tsbond(topleft, botlshelle);

                }
                if (botrshelle.sh == dummysh) {
                        tsdissolve(botleft);
                } else {
                        tsbond(botleft, botrshelle);
                }
                if (toprshelle.sh == dummysh) {
                        tsdissolve(botright);
                } else {
                        tsbond(botright, toprshelle);
                }
        }

        /* New point assignments for the rotated quadrilateral. */
        setorg(*flipedge, farpoint);
        setdest(*flipedge, botpoint);
        setapex(*flipedge, rightpoint);
        setorg(top, botpoint);
        setdest(top, farpoint);
        setapex(top, leftpoint);
        //  if (verbose > 2) {
        //    printf("  Edge flip results in left ");
        //    lnextself(topleft);
        //    printtriangle(&topleft);
        //    printf("  and right ");
        //    printtriangle(flipedge);
        //  }
}

/*****************************************************************************/
/*                                                                           */
/*  insertsite()   Insert a vertex into a Delaunay triangulation,            */
/*                 performing flips as necessary to maintain the Delaunay    */
/*                 property.                                                 */
/*                                                                           */
/*  The point `insertpoint' is located.  If `searchtri.tri' is not NULL,     */
/*  the search for the containing triangle begins from `searchtri'.  If      */
/*  `searchtri.tri' is NULL, a full point location procedure is called.      */
/*  If `insertpoint' is found inside a triangle, the triangle is split into  */
/*  three; if `insertpoint' lies on an edge, the edge is split in two,       */
/*  thereby splitting the two adjacent triangles into four.  Edge flips are  */
/*  used to restore the Delaunay property.  If `insertpoint' lies on an      */
/*  existing vertex, no action is taken, and the value DUPLICATEPOINT is     */
/*  returned.  On return, `searchtri' is set to a handle whose origin is the */
/*  existing vertex.                                                         */
/*                                                                           */
/*  Normally, the parameter `splitedge' is set to NULL, implying that no     */
/*  segment should be split.  In this case, if `insertpoint' is found to     */
/*  lie on a segment, no action is taken, and the value VIOLATINGPOINT is    */
/*  returned.  On return, `searchtri' is set to a handle whose primary edge  */
/*  is the violated segment.                                                 */
/*                                                                           */
/*  If the calling routine wishes to split a segment by inserting a point in */
/*  it, the parameter `splitedge' should be that segment.  In this case,     */
/*  `searchtri' MUST be the triangle handle reached by pivoting from that    */
/*  segment; no point location is done.                                      */
/*                                                                           */
/*  `segmentflaws' and `triflaws' are flags that indicate whether or not     */
/*  there should be checks for the creation of encroached segments or bad    */
/*  quality faces.  If a newly inserted point encroaches upon segments,      */
/*  these segments are added to the list of segments to be split if          */
/*  `segmentflaws' is set.  If bad triangles are created, these are added    */
/*  to the queue if `triflaws' is set.                                       */
/*                                                                           */
/*  If a duplicate point or violated segment does not prevent the point      */
/*  from being inserted, the return value will be ENCROACHINGPOINT if the    */
/*  point encroaches upon a segment (and checking is enabled), or            */
/*  SUCCESSFULPOINT otherwise.  In either case, `searchtri' is set to a      */
/*  handle whose origin is the newly inserted vertex.                        */
/*                                                                           */
/*  insertsite() does not use flip() for reasons of speed; some              */
/*  information can be reused from edge flip to edge flip, like the          */
/*  locations of shell edges.                                                */
/*                                                                           */
/*****************************************************************************/

enum insertsiteresult CTIN::insertsite(point insertpoint, struct CTriEdge *searchtri, struct CEdge *splitedge,
                                                                           long  segmentflaws, long  triflaws)
{
        struct CTriEdge horiz;
        struct CTriEdge top;
        struct CTriEdge botleft, botright;
        struct CTriEdge topleft, topright;
        struct CTriEdge newbotleft, newbotright;
        struct CTriEdge newtopright;
        struct CTriEdge botlcasing, botrcasing;
        struct CTriEdge toplcasing, toprcasing;
        struct CTriEdge testtri;
        struct CEdge botlshelle, botrshelle;
        struct CEdge toplshelle, toprshelle;
        struct CEdge brokenshelle;
        struct CEdge checkshelle;
        struct CEdge rightedge;
        struct CEdge newedge;
        struct CEdge *encroached;
        point first;
        point leftpoint, rightpoint, botpoint, toppoint, farpoint;
        double area;
        enum insertsiteresult success;
        enum locateresult intersect;
        long  doflip;
        long  mirrorflag;
        triangle ptr;                         /* Temporary variable used by sym(). */
        shelle sptr;         /* Temporary variable used by spivot() and tspivot(). */

        //  if (verbose > 1) {    printf("  Inserting (%.12g, %.12g).\n", insertpoint[0], insertpoint[1]);  }
        if (splitedge == (struct CEdge *) NULL) {
                /* Find the location of the point to be inserted.  Check if a good */
                /*   starting triangle has already been provided by the caller.    */
                if (searchtri->tri == (triangle *) NULL) {
                        /* Find a boundary triangle. */
                        horiz.tri = dummytri;
                        horiz.orient = 0;
                        symself(horiz);
                        /* Search for a triangle containing `insertpoint'. */
                        intersect = locate(insertpoint, &horiz);
                } else {
                        /* Start searching from the triangle provided by the caller. */
                        triedgecopy(*searchtri, horiz);
                        intersect = preciselocate(insertpoint, &horiz);
                }
        } else {
                /* The calling routine provides the edge in which the point is inserted. */
                triedgecopy(*searchtri, horiz);

                intersect = ONEDGE;
        }
        if (intersect == ONVERTEX) {
                /* There's already a vertex there.  Return in `searchtri' a triangle */
                /*   whose origin is the existing vertex.                            */
                triedgecopy(horiz, *searchtri);
                triedgecopy(horiz, recenttri);
                return DUPLICATEPOINT;
        }
        if ((intersect == ONEDGE) || (intersect == OUTSIDE)) {
                /* The vertex falls on an edge or boundary. */
                if (checksegments && (splitedge == (struct CEdge *) NULL)) {
                        /* Check whether the vertex falls on a shell edge. */
                        tspivot(horiz, brokenshelle);
                        if (brokenshelle.sh != dummysh) {
                                /* The vertex falls on a shell edge. */
                                if (segmentflaws) {
                                        if (nobisect == 0) {
                                                /* Add the shell edge to the list of encroached segments. */
                                                encroached = (struct CEdge *) poolalloc(&badsegments);
                                                shellecopy(brokenshelle, *encroached);
                                        } else if ((nobisect == 1) && (intersect == ONEDGE)) {
                                                /* This segment may be split only if it is an internal boundary. */
                                                sym(horiz, testtri);
                                                if (testtri.tri != dummytri) {
                                                        /* Add the shell edge to the list of encroached segments. */
                                                        encroached = (struct CEdge *) poolalloc(&badsegments);
                                                        shellecopy(brokenshelle, *encroached);
                                                }
                                        }
                                }
                                /* Return a handle whose primary edge contains the point, */
                                /*   which has not been inserted.                         */
                                triedgecopy(horiz, *searchtri);
                                triedgecopy(horiz, recenttri);
                                return VIOLATINGPOINT;
                        }
                }
                /* Insert the point on an edge, dividing one triangle into two (if */
                /*   the edge lies on a boundary) or two triangles into four.      */
                lprev(horiz, botright);
                sym(botright, botrcasing);
                sym(horiz, topright);
                /* Is there a second triangle?  (Or does this edge lie on a boundary?) */
                mirrorflag = topright.tri != dummytri;
                if (mirrorflag) {
                        lnextself(topright);
                        sym(topright, toprcasing);
                        maketriangle(&newtopright);
                } else {
                        /* Splitting the boundary edge increases the number of boundary edges. */
                        hullsize++;
                }
                maketriangle(&newbotright);

                /* Set the vertices of changed and new triangles. */
                org(horiz, rightpoint);
                dest(horiz, leftpoint);
                apex(horiz, botpoint);
                setorg(newbotright, botpoint);
                setdest(newbotright, rightpoint);
                setapex(newbotright, insertpoint);
                setorg(horiz, insertpoint);
                if (vararea) {
                        /* Set the area constraint of a new triangle. */
                        setareabound(newbotright, areabound(botright));
                }
                if (mirrorflag) {
                        dest(topright, toppoint);
                        setorg(newtopright, rightpoint);
                        setdest(newtopright, toppoint);
                        setapex(newtopright, insertpoint);
                        setorg(topright, insertpoint);
                        if (vararea) {
                                /* Set the area constraint of another new triangle. */
                                setareabound(newtopright, areabound(topright));
                        }
                }

                /* There may be shell edges that need to be bonded */
                /*   to the new triangle(s).                       */
                if (checksegments) {
                        tspivot(botright, botrshelle);
                        if (botrshelle.sh != dummysh) {
                                tsdissolve(botright);
                                tsbond(newbotright, botrshelle);
                        }
                        if (mirrorflag) {
                                tspivot(topright, toprshelle);
                                if (toprshelle.sh != dummysh) {
                                        tsdissolve(topright);
                                        tsbond(newtopright, toprshelle);
                                }
                        }
                }

                /* Bond the new triangle(s) to the surrounding triangles. */
                bond(newbotright, botrcasing);
                lprevself(newbotright);
                bond(newbotright, botright);
                lprevself(newbotright);
                if (mirrorflag) {
                        bond(newtopright, toprcasing);
                        lnextself(newtopright);
                        bond(newtopright, topright);
                        lnextself(newtopright);
                        bond(newtopright, newbotright);
                }

                if (splitedge != (struct CEdge *) NULL) {
                        /* Split the shell edge into two. */
                        setsdest(*splitedge, insertpoint);
                        ssymself(*splitedge);
                        spivot(*splitedge, rightedge);
                        insertshelle(&newbotright, mark(*splitedge));
                        tspivot(newbotright, newedge);
                        sbond(*splitedge, newedge);
                        ssymself(newedge);
                        sbond(newedge, rightedge);
                        ssymself(*splitedge);
                }


                /*    if (verbose > 2) {
                printf("  Updating bottom left ");
                printtriangle(&botright);
                if (mirrorflag) {
        printf("  Updating top left ");
        printtriangle(&topright);
        printf("  Creating top right ");
        printtriangle(&newtopright);
                }
                printf("  Creating bottom right ");
                printtriangle(&newbotright);
                }
                */
                /* Position `horiz' on the first edge to check for */
                /*   the Delaunay property.                        */
                lnextself(horiz);
  } else {
          /* Insert the point in a triangle, splitting it into three. */
          lnext(horiz, botleft);
          lprev(horiz, botright);
          sym(botleft, botlcasing);
          sym(botright, botrcasing);
          maketriangle(&newbotleft);
          maketriangle(&newbotright);

          /* Set the vertices of changed and new triangles. */
          org(horiz, rightpoint);
          dest(horiz, leftpoint);
          apex(horiz, botpoint);
          setorg(newbotleft, leftpoint);
          setdest(newbotleft, botpoint);
          setapex(newbotleft, insertpoint);
          setorg(newbotright, botpoint);
          setdest(newbotright, rightpoint);
          setapex(newbotright, insertpoint);
          setapex(horiz, insertpoint);
          if (vararea) {
                  /* Set the area constraint of the new triangles. */
                  area = areabound(horiz);
                  setareabound(newbotleft, area);
                  setareabound(newbotright, area);
          }

          /* There may be shell edges that need to be bonded */
          /*   to the new triangles.                         */
          if (checksegments) {
                  tspivot(botleft, botlshelle);
                  if (botlshelle.sh != dummysh) {
                          tsdissolve(botleft);
                          tsbond(newbotleft, botlshelle);
                  }
                  tspivot(botright, botrshelle);
                  if (botrshelle.sh != dummysh) {
                          tsdissolve(botright);
                          tsbond(newbotright, botrshelle);
                  }
          }

          /* Bond the new triangles to the surrounding triangles. */
          bond(newbotleft, botlcasing);
          bond(newbotright, botrcasing);
          lnextself(newbotleft);
          lprevself(newbotright);
          bond(newbotleft, newbotright);
          lnextself(newbotleft);
          bond(botleft, newbotleft);
          lprevself(newbotright);
          bond(botright, newbotright);

          /*    if (verbose > 2) {
      printf("  Updating top ");
      printtriangle(&horiz);
      printf("  Creating left ");
      printtriangle(&newbotleft);
      printf("  Creating right ");
      printtriangle(&newbotright);
          }
          */  }

          /* The insertion is successful by default, unless an encroached */
          /*   edge is found.                                             */
          success = SUCCESSFULPOINT;
          /* Circle around the newly inserted vertex, checking each edge opposite */

          /*   it for the Delaunay property.  Non-Delaunay edges are flipped.     */
          /*   `horiz' is always the edge being checked.  `first' marks where to  */
          /*   stop circling.                                                     */
          org(horiz, first);
          rightpoint = first;
          dest(horiz, leftpoint);
          /* Circle until finished. */
          while (1) {
                  /* By default, the edge will be flipped. */
                  doflip = 1;
                  if (checksegments) {
                          /* Check for a segment, which cannot be flipped. */
                          tspivot(horiz, checkshelle);
                          if (checkshelle.sh != dummysh) {
                                  /* The edge is a segment and cannot be flipped. */
                                  doflip = 0;
                          }
                  }
                  if (doflip) {
                          /* Check if the edge is a boundary edge. */
                          sym(horiz, top);
                          if (top.tri == dummytri) {
                                  /* The edge is a boundary edge and cannot be flipped. */
                                  doflip = 0;
                          } else {
                                  /* Find the point on the other side of the edge. */
                                  apex(top, farpoint);
                                  /* In the incremental Delaunay triangulation algorithm, any of    */
                                  /*   `leftpoint', `rightpoint', and `farpoint' could be vertices  */
                                  /*   of the triangular bounding box.  These vertices must be      */
                                  /*   treated as if they are infinitely distant, even though their */
                                  /*   "coordinates" are not.                                       */
                                  if ((leftpoint == infpoint1) || (leftpoint == infpoint2)
                                          || (leftpoint == infpoint3)) {
                                          /* `leftpoint' is infinitely distant.  Check the convexity of */
                                          /*   the boundary of the triangulation.  'farpoint' might be  */
                                          /*   infinite as well, but trust me, this same condition      */
                                          /*   should be applied.                                       */
                                          doflip = counterclockwise(insertpoint, rightpoint, farpoint) > 0.0;
                                  } else if ((rightpoint == infpoint1) || (rightpoint == infpoint2)
                                          || (rightpoint == infpoint3)) {
                                          /* `rightpoint' is infinitely distant.  Check the convexity of */
                                          /*   the boundary of the triangulation.  'farpoint' might be  */
                                          /*   infinite as well, but trust me, this same condition      */
                                          /*   should be applied.                                       */
                                          doflip = counterclockwise(farpoint, leftpoint, insertpoint) > 0.0;
                                  } else if ((farpoint == infpoint1) || (farpoint == infpoint2)
                                          || (farpoint == infpoint3)) {
                                          /* `farpoint' is infinitely distant and cannot be inside */
                                          /*   the circumcircle of the triangle `horiz'.           */
                                          doflip = 0;
                                  } else {
                                          /* Test whether the edge is locally Delaunay. */
                                          doflip = incircle(leftpoint, insertpoint, rightpoint, farpoint)
                                                  > 0.0;
                                  }
                                  if (doflip) {
                                          /* We made it!  Flip the edge `horiz' by rotating its containing */
                                          /*   quadrilateral (the two triangles adjacent to `horiz').      */
                                          /* Identify the casing of the quadrilateral. */
                                          lprev(top, topleft);
                                          sym(topleft, toplcasing);
                                          lnext(top, topright);
                                          sym(topright, toprcasing);
                                          lnext(horiz, botleft);
                                          sym(botleft, botlcasing);
                                          lprev(horiz, botright);
                                          sym(botright, botrcasing);
                                          /* Rotate the quadrilateral one-quarter turn counterclockwise. */
                                          bond(topleft, botlcasing);
                                          bond(botleft, botrcasing);
                                          bond(botright, toprcasing);
                                          bond(topright, toplcasing);
                                          if (checksegments) {
                                                  /* Check for shell edges and rebond them to the quadrilateral. */
                                                  tspivot(topleft, toplshelle);
                                                  tspivot(botleft, botlshelle);
                                                  tspivot(botright, botrshelle);
                                                  tspivot(topright, toprshelle);
                                                  if (toplshelle.sh == dummysh) {
                                                          tsdissolve(topright);
                                                  } else {
                                                          tsbond(topright, toplshelle);
                                                  }
                                                  if (botlshelle.sh == dummysh) {
                                                          tsdissolve(topleft);
                                                  } else {
                                                          tsbond(topleft, botlshelle);
                                                  }
                                                  if (botrshelle.sh == dummysh) {
                                                          tsdissolve(botleft);
                                                  } else {
                                                          tsbond(botleft, botrshelle);
                                                  }
                                                  if (toprshelle.sh == dummysh) {
                                                          tsdissolve(botright);
                                                  } else {
                                                          tsbond(botright, toprshelle);
                                                  }
                                          }
                                          /* New point assignments for the rotated quadrilateral. */
                                          setorg(horiz, farpoint);
                                          setdest(horiz, insertpoint);
                                          setapex(horiz, rightpoint);
                                          setorg(top, insertpoint);
                                          setdest(top, farpoint);
                                          setapex(top, leftpoint);
                                          if (vararea) {
                                                  if ((areabound(top) <= 0.0) || (areabound(horiz) <= 0.0)) {
                                                          area = -1.0;
                                                  } else {
                                                          /* Take the average of the two triangles' area constraints.    */
                                                          /*   This prevents small area constraints from migrating a     */
                                                          /*   long, long way from their original location due to flips. */
                                                          area = 0.5 * (areabound(top) + areabound(horiz));
                                                  }
                                                  setareabound(top, area);
                                                  setareabound(horiz, area);
                                          }

                                          /*          if (verbose > 2) {
                                          printf("  Edge flip results in left ");
                                          lnextself(topleft);
                                          printtriangle(&topleft);
                                          printf("  and right ");
                                          printtriangle(&horiz);
                                          }
                                          */          /* On the next iterations, consider the two edges that were  */
                                          /*   exposed (this is, are now visible to the newly inserted */
                                          /*   point) by the edge flip.                                */
                                          lprevself(horiz);
                                          leftpoint = farpoint;
                                  }
      }
    }
    if (!doflip) {
                /* The handle `horiz' is accepted as locally Delaunay. */
                /* Look for the next edge around the newly inserted point. */

                lnextself(horiz);
                sym(horiz, testtri);
                /* Check for finishing a complete revolution about the new point, or */
                /*   falling off the edge of the triangulation.  The latter will     */
                /*   happen when a point is inserted at a boundary.                  */
                if ((leftpoint == first) || (testtri.tri == dummytri)) {
                        /* We're done.  Return a triangle whose origin is the new point. */
                        lnext(horiz, *searchtri);
                        lnext(horiz, recenttri);
                        return success;
                }
                /* Finish finding the next edge around the newly inserted point. */
                lnext(testtri, horiz);
                rightpoint = leftpoint;
                dest(horiz, leftpoint);
    }
  }
}

/*****************************************************************************/
/*                                                                           */
/*  triangulatepolygon()   Find the Delaunay triangulation of a polygon that */
/*                         has a certain "nice" shape.  This includes the    */
/*                         polygons that result from deletion of a point or  */
/*                         insertion of a segment.                           */
/*                                                                           */
/*  This is a conceptually difficult routine.  The starting assumption is    */
/*  that we have a polygon with n sides.  n - 1 of these sides are currently */
/*  represented as edges in the mesh.  One side, called the "base", need not */
/*  be.                                                                      */
/*                                                                           */
/*  Inside the polygon is a structure I call a "fan", consisting of n - 1    */
/*  triangles that share a common origin.  For each of these triangles, the  */
/*  edge opposite the origin is one of the sides of the polygon.  The        */
/*  primary edge of each triangle is the edge directed from the origin to    */
/*  the destination; note that this is not the same edge that is a side of   */
/*  the polygon.  `firstedge' is the primary edge of the first triangle.     */
/*  From there, the triangles follow in counterclockwise order about the     */
/*  polygon, until `lastedge', the primary edge of the last triangle.        */
/*  `firstedge' and `lastedge' are probably connected to other triangles     */
/*  beyond the extremes of the fan, but their identity is not important, as  */
/*  long as the fan remains connected to them.                               */
/*                                                                           */
/*  Imagine the polygon oriented so that its base is at the bottom.  This    */
/*  puts `firstedge' on the far right, and `lastedge' on the far left.       */
/*  The right vertex of the base is the destination of `firstedge', and the  */
/*  left vertex of the base is the apex of `lastedge'.                       */
/*                                                                           */
/*  The challenge now is to find the right sequence of edge flips to         */
/*  transform the fan into a Delaunay triangulation of the polygon.  Each    */
/*  edge flip effectively removes one triangle from the fan, committing it   */
/*  to the polygon.  The resulting polygon has one fewer edge.  If `doflip'  */
/*  is set, the final flip will be performed, resulting in a fan of one      */
/*  (useless?) triangle.  If `doflip' is not set, the final flip is not      */
/*  performed, resulting in a fan of two triangles, and an unfinished        */
/*  triangular polygon that is not yet filled out with a single triangle.    */
/*  On completion of the routine, `lastedge' is the last remaining triangle, */
/*  or the leftmost of the last two.                                         */
/*                                                                           */
/*  Although the flips are performed in the order described above, the       */
/*  decisions about what flips to perform are made in precisely the reverse  */
/*  order.  The recursive triangulatepolygon() procedure makes a decision,   */
/*  uses up to two recursive calls to triangulate the "subproblems"          */

/*  (polygons with fewer edges), and then performs an edge flip.             */
/*                                                                           */
/*  The "decision" it makes is which vertex of the polygon should be         */
/*  connected to the base.  This decision is made by testing every possible  */
/*  vertex.  Once the best vertex is found, the two edges that connect this  */
/*  vertex to the base become the bases for two smaller polygons.  These     */
/*  are triangulated recursively.  Unfortunately, this approach can take     */
/*  O(n^2) time not only in the worst case, but in many common cases.  It's  */
/*  rarely a big deal for point deletion, where n is rarely larger than ten, */
/*  but it could be a big deal for segment insertion, especially if there's  */
/*  a lot of long segments that each cut many triangles.  I ought to code    */
/*  a faster algorithm some time.                                            */
/*                                                                           */
/*  The `edgecount' parameter is the number of sides of the polygon,         */
/*  including its base.  `triflaws' is a flag that determines whether the    */
/*  new triangles should be tested for quality, and enqueued if they are     */
/*  bad.                                                                     */
/*                                                                           */
/*****************************************************************************/

void CTIN::triangulatepolygon(struct CTriEdge *firstedge, struct CTriEdge *lastedge, long  edgecount, long  doflip, long  triflaws)
{
        struct CTriEdge testtri;
        struct CTriEdge besttri;
        struct CTriEdge tempedge;
        point leftbasepoint, rightbasepoint;
        point testpoint;
        point bestpoint;
        long  bestnumber;
        long  i;
        triangle ptr;   /* Temporary variable used by sym(), onext(), and oprev(). */

        /* Identify the base vertices. */
        apex(*lastedge, leftbasepoint);
        dest(*firstedge, rightbasepoint);
        /*  if (verbose > 2) {
    printf("  Triangulating interior polygon at edge\n");
    printf("    (%.12g, %.12g) (%.12g, %.12g)\n", leftbasepoint[0],
        leftbasepoint[1], rightbasepoint[0], rightbasepoint[1]);
        }
        */
        /* Find the best vertex to connect the base to. */
        onext(*firstedge, besttri);

        dest(besttri, bestpoint);
        triedgecopy(besttri, testtri);
        bestnumber = 1;
        for (i = 2; i <= edgecount - 2; i++) {
                onextself(testtri);
                dest(testtri, testpoint);
                /* Is this a better vertex? */
                if (incircle(leftbasepoint, rightbasepoint, bestpoint, testpoint) > 0.0) {
                        triedgecopy(testtri, besttri);
                        bestpoint = testpoint;
                        bestnumber = i;
                }
        }
        //  if (verbose > 2) {    printf("    Connecting edge to (%.12g, %.12g)\n", bestpoint[0], bestpoint[1]);  }
        if (bestnumber > 1) {
                /* Recursively triangulate the smaller polygon on the right. */
                oprev(besttri, tempedge);
                triangulatepolygon(firstedge, &tempedge, bestnumber + 1, 1, triflaws);
        }
        if (bestnumber < edgecount - 2) {
                /* Recursively triangulate the smaller polygon on the left. */
                sym(besttri, tempedge);
                triangulatepolygon(&besttri, lastedge, edgecount - bestnumber, 1,
                        triflaws);
                /* Find `besttri' again; it may have been lost to edge flips. */

                sym(tempedge, besttri);
        }
        if (doflip) {
                /* Do one final edge flip. */
                flip(&besttri);
        }
        /* Return the base triangle. */
        triedgecopy(besttri, *lastedge);
}


/**                                                                         **/
/**                                                                         **/
/********* Mesh transformation routines end here                     *********/

/********* Divide-and-conquer Delaunay triangulation begins here     *********/
/**                                                                         **/
/**                                                                         **/

/*****************************************************************************/
/*                                                                           */
/*  The divide-and-conquer bounding box                                      */
/*                                                                           */
/*  I originally implemented the divide-and-conquer and incremental Delaunay */
/*  triangulations using the edge-based data structure presented by Guibas   */
/*  and Stolfi.  Switching to a triangle-based data structure doubled the    */
/*  speed.  However, I had to think of a few extra tricks to maintain the    */
/*  elegance of the original algorithms.                                     */
/*                                                                           */
/*  The "bounding box" used by my variant of the divide-and-conquer          */
/*  algorithm uses one triangle for each edge of the convex hull of the      */
/*  triangulation.  These bounding triangles all share a common apical       */
/*  vertex, which is represented by NULL and which represents nothing.       */
/*  The bounding triangles are linked in a circular fan about this NULL      */
/*  vertex, and the edges on the convex hull of the triangulation appear     */
/*  opposite the NULL vertex.  You might find it easiest to imagine that     */
/*  the NULL vertex is a point in 3D space behind the center of the          */
/*  triangulation, and that the bounding triangles form a sort of cone.      */
/*                                                                           */
/*  This bounding box makes it easy to represent degenerate cases.  For      */
/*  instance, the triangulation of two vertices is a single edge.  This edge */
/*  is represented by two bounding box triangles, one on each "side" of the  */
/*  edge.  These triangles are also linked together in a fan about the NULL  */
/*  vertex.                                                                  */
/*                                                                           */
/*  The bounding box also makes it easy to traverse the convex hull, as the  */
/*  divide-and-conquer algorithm needs to do.                                */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/*  pointsort()   Sort an array of points by x-coordinate, using the         */
/*                y-coordinate as a secondary key.                           */
/*                                                                           */
/*  Uses quicksort.  Randomized O(n log n) time.  No, I did not make any of  */
/*  the usual quicksort mistakes.                                            */
/*                                                                           */
/*****************************************************************************/

void CTIN::pointsort(point *sortarray, long  arraysize)
{
        long  left, right;
        long  pivot;
        double pivotx, pivoty;
        point temp;

        if (arraysize == 2) {
                /* Recursive base case. */
                if ((sortarray[0][0] > sortarray[1][0]) ||
                        ((sortarray[0][0] == sortarray[1][0]) &&
                        (sortarray[0][1] > sortarray[1][1]))) {
                        temp = sortarray[1];
                        sortarray[1] = sortarray[0];
                        sortarray[0] = temp;
                }
                return;
        }
        /* Choose a random pivot to split the array. */
        pivot = (long ) randomnation(arraysize);
        pivotx = sortarray[pivot][0];
        pivoty = sortarray[pivot][1];
        /* Split the array. */
        left = -1;
        right = arraysize;
        while (left < right) {
                /* Search for a point whose x-coordinate is too large for the left. */
                do {
                        left++;
                } while ((left <= right) && ((sortarray[left][0] < pivotx) ||
                        ((sortarray[left][0] == pivotx) &&
                        (sortarray[left][1] < pivoty))));
                /* Search for a point whose x-coordinate is too small for the right. */
                do {
                        right--;
                } while ((left <= right) && ((sortarray[right][0] > pivotx) ||
                        ((sortarray[right][0] == pivotx) &&
                        (sortarray[right][1] > pivoty))));
                if (left < right) {
                        /* Swap the left and right points. */
                        temp = sortarray[left];
                        sortarray[left] = sortarray[right];
                        sortarray[right] = temp;
                }
        }
        if (left > 1) {
                /* Recursively sort the left subset. */
                pointsort(sortarray, left);
        }
        if (right < arraysize - 2) {
                /* Recursively sort the right subset. */
                pointsort(&sortarray[right + 1], arraysize - right - 1);
        }
}

/*****************************************************************************/
/*                                                                           */
/*  pointmedian()   An order statistic algorithm, almost.  Shuffles an array */
/*                  of points so that the first `median' points occur        */
/*                  lexicographically before the remaining points.           */
/*                                                                           */
/*  Uses the x-coordinate as the primary key if axis == 0; the y-coordinate  */
/*  if axis == 1.  Very similar to the pointsort() procedure, but runs in    */
/*  randomized linear time.                                                  */
/*                                                                           */
/*****************************************************************************/

void CTIN::pointmedian(point *sortarray, long  arraysize, long  median, long  axis)
{
        long  left, right;
        long  pivot;
        double pivot1, pivot2;
        point temp;

        if (arraysize == 2) {
                /* Recursive base case. */
                if ((sortarray[0][axis] > sortarray[1][axis]) ||
                        ((sortarray[0][axis] == sortarray[1][axis]) &&
                        (sortarray[0][1 - axis] > sortarray[1][1 - axis]))) {
                        temp = sortarray[1];
                        sortarray[1] = sortarray[0];
                        sortarray[0] = temp;
                }
                return;
        }
        /* Choose a random pivot to split the array. */
        pivot = (long ) randomnation(arraysize);
        pivot1 = sortarray[pivot][axis];
        pivot2 = sortarray[pivot][1 - axis];
        /* Split the array. */
        left = -1;
        right = arraysize;
        while (left < right) {
                /* Search for a point whose x-coordinate is too large for the left. */
                do {
                        left++;
                } while ((left <= right) && ((sortarray[left][axis] < pivot1) ||
                        ((sortarray[left][axis] == pivot1) &&
                        (sortarray[left][1 - axis] < pivot2))));
                /* Search for a point whose x-coordinate is too small for the right. */
                do {
                        right--;
                } while ((left <= right) && ((sortarray[right][axis] > pivot1) ||
                        ((sortarray[right][axis] == pivot1) &&
                        (sortarray[right][1 - axis] > pivot2))));
                if (left < right) {
                        /* Swap the left and right points. */
                        temp = sortarray[left];
                        sortarray[left] = sortarray[right];
                        sortarray[right] = temp;
                }
        }
        /* Unlike in pointsort(), at most one of the following */
        /*   conditionals is true.                             */
        if (left > median) {
                /* Recursively shuffle the left subset. */
                pointmedian(sortarray, left, median, axis);
        }

        if (right < median - 1) {
                /* Recursively shuffle the right subset. */
                pointmedian(&sortarray[right + 1], arraysize - right - 1,
                        median - right - 1, axis);
        }
}

/*****************************************************************************/
/*                                                                           */
/*  alternateaxes()   Sorts the points as appropriate for the divide-and-    */
/*                    conquer algorithm with alternating cuts.               */
/*                                                                           */
/*  itions by x-coordinate if axis == 0; by y-coordinate if axis == 1.   */
/*  For the base case, subsets containing only two or three points are       */
/*  always sorted by x-coordinate.                                           */
/*                                                                           */
/*****************************************************************************/

void CTIN::alternateaxes(point *sortarray, long  arraysize, long  axis)
{
        long  divider;

        divider = arraysize >> 1;
        if (arraysize <= 3) {
                /* Recursive base case:  subsets of two or three points will be      */
                /*   handled specially, and should always be sorted by x-coordinate. */
                axis = 0;
        }
        /* ition with a horizontal or vertical cut. */
        pointmedian(sortarray, arraysize, divider, axis);
        /* Recursively partition the subsets with a cross cut. */
        if (arraysize - divider >= 2) {
                if (divider >= 2) {
                        alternateaxes(sortarray, divider, 1 - axis);
                }
                alternateaxes(&sortarray[divider], arraysize - divider, 1 - axis);
        }
}

/*****************************************************************************/
/*                                                                           */
/*  mergehulls()   Merge two adjacent Delaunay triangulations into a         */
/*                 single Delaunay triangulation.                            */
/*                                                                           */
/*  This is similar to the algorithm given by Guibas and Stolfi, but uses    */
/*  a triangle-based, rather than edge-based, data structure.                */
/*                                                                           */
/*  The algorithm walks up the gap between the two triangulations, knitting  */
/*  them together.  As they are merged, some of their bounding triangles     */
/*  are converted into real triangles of the triangulation.  The procedure   */
/*  pulls each hull's bounding triangles apart, then knits them together     */
/*  like the teeth of two gears.  The Delaunay property determines, at each  */
/*  step, whether the next "tooth" is a bounding triangle of the left hull   */
/*  or the right.  When a bounding triangle becomes real, its apex is        */
/*  changed from NULL to a real point.                                       */
/*                                                                           */
/*  Only two new triangles need to be allocated.  These become new bounding  */
/*  triangles at the top and bottom of the seam.  They are used to connect   */
/*  the remaining bounding triangles (those that have not been converted     */
/*  into real triangles) into a single fan.                                  */
/*                                                                           */
/*  On entry, `farleft' and `innerleft' are bounding triangles of the left   */
/*  triangulation.  The origin of `farleft' is the leftmost vertex, and      */
/*  the destination of `innerleft' is the rightmost vertex of the            */
/*  triangulation.  Similarly, `innerright' and `farright' are bounding      */
/*  triangles of the right triangulation.  The origin of `innerright' and    */
/*  destination of `farright' are the leftmost and rightmost vertices.       */
/*                                                                           */
/*  On completion, the origin of `farleft' is the leftmost vertex of the     */
/*  merged triangulation, and the destination of `farright' is the rightmost */
/*  vertex.                                                                  */
/*                                                                           */
/*****************************************************************************/

void CTIN::mergehulls(struct CTriEdge *farleft, struct CTriEdge *innerleft, struct CTriEdge *innerright, struct CTriEdge *farright, long  axis)
{
        struct CTriEdge leftcand, rightcand;
        struct CTriEdge baseedge;
        struct CTriEdge nextedge;
        struct CTriEdge sidecasing, topcasing, outercasing;
        struct CTriEdge checkedge;
        point innerleftdest;
        point innerrightorg;
        point innerleftapex, innerrightapex;
        point farleftpt, farrightpt;
        point farleftapex, farrightapex;
        point lowerleft, lowerright;
        point upperleft, upperright;
        point nextapex;
        point checkvertex;
        long  changemade;
        long  badedge;
        long  leftfinished, rightfinished;
        triangle ptr;                         /* Temporary variable used by sym(). */

        dest(*innerleft, innerleftdest);
        apex(*innerleft, innerleftapex);
        org(*innerright, innerrightorg);
        apex(*innerright, innerrightapex);
        /* Special treatment for horizontal cuts. */
        if (dwyer && (axis == 1)) {
                org(*farleft, farleftpt);
                apex(*farleft, farleftapex);
                dest(*farright, farrightpt);
                apex(*farright, farrightapex);
                /* The pointers to the extremal points are shifted to point to the */
                /*   topmost and bottommost point of each hull, rather than the    */
                /*   leftmost and rightmost points.                                */
                while (farleftapex[1] < farleftpt[1]) {
                        lnextself(*farleft);
                        symself(*farleft);
                        farleftpt = farleftapex;
                        apex(*farleft, farleftapex);
                }
                sym(*innerleft, checkedge);
                apex(checkedge, checkvertex);
                while (checkvertex[1] > innerleftdest[1]) {
                        lnext(checkedge, *innerleft);
                        innerleftapex = innerleftdest;
                        innerleftdest = checkvertex;
                        sym(*innerleft, checkedge);
                        apex(checkedge, checkvertex);
                }
                while (innerrightapex[1] < innerrightorg[1]) {
                        lnextself(*innerright);
                        symself(*innerright);
                        innerrightorg = innerrightapex;
                        apex(*innerright, innerrightapex);
                }
                sym(*farright, checkedge);
                apex(checkedge, checkvertex);
                while (checkvertex[1] > farrightpt[1]) {
                        lnext(checkedge, *farright);
                        farrightapex = farrightpt;
                        farrightpt = checkvertex;
                        sym(*farright, checkedge);
                        apex(checkedge, checkvertex);
                }
        }
        /* Find a line tangent to and below both hulls. */
        do {
                changemade = 0;
                /* Make innerleftdest the "bottommost" point of the left hull. */
                if (counterclockwise(innerleftdest, innerleftapex, innerrightorg) > 0.0) {
                        lprevself(*innerleft);
                        symself(*innerleft);
                        innerleftdest = innerleftapex;
                        apex(*innerleft, innerleftapex);
                        changemade = 1;
                }
                /* Make innerrightorg the "bottommost" point of the right hull. */
                if (counterclockwise(innerrightapex, innerrightorg, innerleftdest) > 0.0) {
                        lnextself(*innerright);
                        symself(*innerright);
                        innerrightorg = innerrightapex;
                        apex(*innerright, innerrightapex);
                        changemade = 1;
                }
        } while (changemade);
        /* Find the two candidates to be the next "gear tooth". */
        sym(*innerleft, leftcand);
        sym(*innerright, rightcand);
        /* Create the bottom new bounding triangle. */
        maketriangle(&baseedge);
        /* Connect it to the bounding boxes of the left and right triangulations. */
        bond(baseedge, *innerleft);
        lnextself(baseedge);
        bond(baseedge, *innerright);
        lnextself(baseedge);
        setorg(baseedge, innerrightorg);
        setdest(baseedge, innerleftdest);
        /* Apex is intentionally left NULL. */
        /*  if (verbose > 2) {
    printf("  Creating base bounding ");
    printtriangle(&baseedge);
        }
        */
        /* Fix the extreme triangles if necessary. */
        org(*farleft, farleftpt);
        if (innerleftdest == farleftpt) {
                lnext(baseedge, *farleft);
        }
        dest(*farright, farrightpt);
        if (innerrightorg == farrightpt) {
                lprev(baseedge, *farright);
        }
        /* The vertices of the current knitting edge. */
        lowerleft = innerleftdest;
        lowerright = innerrightorg;
        /* The candidate vertices for knitting. */
        apex(leftcand, upperleft);
        apex(rightcand, upperright);
        /* Walk up the gap between the two triangulations, knitting them together. */
        while (1) {
                /* Have we reached the top?  (This isn't quite the right question,       */
                /*   because even though the left triangulation might seem finished now, */
                /*   moving up on the right triangulation might reveal a new point of    */
                /*   the left triangulation.  And vice-versa.)                           */
                leftfinished = counterclockwise(upperleft, lowerleft, lowerright) <= 0.0;
                rightfinished = counterclockwise(upperright, lowerleft, lowerright) <= 0.0;
                if (leftfinished && rightfinished) {
                        /* Create the top new bounding triangle. */
                        maketriangle(&nextedge);
                        setorg(nextedge, lowerleft);
                        setdest(nextedge, lowerright);
                        /* Apex is intentionally left NULL. */
                        /* Connect it to the bounding boxes of the two triangulations. */
                        bond(nextedge, baseedge);
                        lnextself(nextedge);
                        bond(nextedge, rightcand);
                        lnextself(nextedge);
                        bond(nextedge, leftcand);
                        /*      if (verbose > 2) {
                        printf("  Creating top bounding ");
                        printtriangle(&baseedge);
                        }
                        */
                        /* Special treatment for horizontal cuts. */
                        if (dwyer && (axis == 1)) {
                                org(*farleft, farleftpt);
                                apex(*farleft, farleftapex);
                                dest(*farright, farrightpt);
                                apex(*farright, farrightapex);
                                sym(*farleft, checkedge);
                                apex(checkedge, checkvertex);
                                /* The pointers to the extremal points are restored to the leftmost */
                                /*   and rightmost points (rather than topmost and bottommost).     */
                                while (checkvertex[0] < farleftpt[0]) {
                                        lprev(checkedge, *farleft);
                                        farleftapex = farleftpt;
                                        farleftpt = checkvertex;
                                        sym(*farleft, checkedge);
                                        apex(checkedge, checkvertex);
                                }
                                while (farrightapex[0] > farrightpt[0]) {
                                        lprevself(*farright);
                                        symself(*farright);
                                        farrightpt = farrightapex;
                                        apex(*farright, farrightapex);
                                }
                        }
                        return;
                }
                /* Consider eliminating edges from the left triangulation. */
                if (!leftfinished) {
                        /* What vertex would be exposed if an edge were deleted? */
                        lprev(leftcand, nextedge);
                        symself(nextedge);
                        apex(nextedge, nextapex);
                        /* If nextapex is NULL, then no vertex would be exposed; the */
                        /*   triangulation would have been eaten right through.      */
                        if (nextapex != (point) NULL) {
                                /* Check whether the edge is Delaunay. */
                                badedge = incircle(lowerleft, lowerright, upperleft, nextapex) > 0.0;
                                while (badedge) {
                                        /* Eliminate the edge with an edge flip.  As a result, the    */
                                        /*   left triangulation will have one more boundary triangle. */
                                        lnextself(nextedge);
                                        sym(nextedge, topcasing);
                                        lnextself(nextedge);
                                        sym(nextedge, sidecasing);
                                        bond(nextedge, topcasing);
                                        bond(leftcand, sidecasing);
                                        lnextself(leftcand);
                                        sym(leftcand, outercasing);
                                        lprevself(nextedge);
                                        bond(nextedge, outercasing);
                                        /* Correct the vertices to reflect the edge flip. */
                                        setorg(leftcand, lowerleft);
                                        setdest(leftcand, NULL);
                                        setapex(leftcand, nextapex);
                                        setorg(nextedge, NULL);
                                        setdest(nextedge, upperleft);
                                        setapex(nextedge, nextapex);
                                        /* Consider the newly exposed vertex. */
                                        upperleft = nextapex;
                                        /* What vertex would be exposed if another edge were deleted? */
                                        triedgecopy(sidecasing, nextedge);
                                        apex(nextedge, nextapex);
                                        if (nextapex != (point) NULL) {
                                                /* Check whether the edge is Delaunay. */
                                                badedge = incircle(lowerleft, lowerright, upperleft, nextapex)
                                                        > 0.0;
                                        } else {
                                                /* Avoid eating right through the triangulation. */
                                                badedge = 0;
                                        }
                                }
                        }
                }
                /* Consider eliminating edges from the right triangulation. */
                if (!rightfinished) {
                        /* What vertex would be exposed if an edge were deleted? */
                        lnext(rightcand, nextedge);
                        symself(nextedge);
                        apex(nextedge, nextapex);
                        /* If nextapex is NULL, then no vertex would be exposed; the */
                        /*   triangulation would have been eaten right through.      */
                        if (nextapex != (point) NULL) {
                                /* Check whether the edge is Delaunay. */
                                badedge = incircle(lowerleft, lowerright, upperright, nextapex) > 0.0;
                                while (badedge) {
                                        /* Eliminate the edge with an edge flip.  As a result, the     */
                                        /*   right triangulation will have one more boundary triangle. */
                                        lprevself(nextedge);
                                        sym(nextedge, topcasing);
                                        lprevself(nextedge);
                                        sym(nextedge, sidecasing);
                                        bond(nextedge, topcasing);
                                        bond(rightcand, sidecasing);
                                        lprevself(rightcand);
                                        sym(rightcand, outercasing);
                                        lnextself(nextedge);
                                        bond(nextedge, outercasing);
                                        /* Correct the vertices to reflect the edge flip. */
                                        setorg(rightcand, NULL);
                                        setdest(rightcand, lowerright);
                                        setapex(rightcand, nextapex);
                                        setorg(nextedge, upperright);
                                        setdest(nextedge, NULL);
                                        setapex(nextedge, nextapex);
                                        /* Consider the newly exposed vertex. */
                                        upperright = nextapex;
                                        /* What vertex would be exposed if another edge were deleted? */
                                        triedgecopy(sidecasing, nextedge);
                                        apex(nextedge, nextapex);
                                        if (nextapex != (point) NULL) {
                                                /* Check whether the edge is Delaunay. */
                                                badedge = incircle(lowerleft, lowerright, upperright, nextapex)
                                                        > 0.0;
                                        } else {
                                                /* Avoid eating right through the triangulation. */
                                                badedge = 0;
                                        }
                                }
                        }
                }
                if (leftfinished || (!rightfinished &&
                        (incircle(upperleft, lowerleft, lowerright, upperright) > 0.0))) {
                        /* Knit the triangulations, adding an edge from `lowerleft' */
                        /*   to `upperright'.                                       */
                        bond(baseedge, rightcand);
                        lprev(rightcand, baseedge);
                        setdest(baseedge, lowerleft);
                        lowerright = upperright;
                        sym(baseedge, rightcand);
                        apex(rightcand, upperright);
                } else {
                        /* Knit the triangulations, adding an edge from `upperleft' */
                        /*   to `lowerright'.                                       */
                        bond(baseedge, leftcand);
                        lnext(leftcand, baseedge);
                        setorg(baseedge, lowerright);
                        lowerleft = upperleft;
                        sym(baseedge, leftcand);
                        apex(leftcand, upperleft);
                }
                /*    if (verbose > 2) {
                printf("  Connecting ");
                printtriangle(&baseedge);
                }
                */
  }
}

/*****************************************************************************/
/*                                                                           */
/*  divconqrecurse()   Recursively form a Delaunay triangulation by the      */
/*                     divide-and-conquer method.                            */
/*                                                                           */
/*  Recursively breaks down the problem into smaller pieces, which are       */
/*  knitted together by mergehulls().  The base cases (problems of two or    */
/*  three points) are handled specially here.                                */
/*                                                                           */
/*  On completion, `farleft' and `farright' are bounding triangles such that */
/*  the origin of `farleft' is the leftmost vertex (breaking ties by         */
/*  choosing the highest leftmost vertex), and the destination of            */
/*  `farright' is the rightmost vertex (breaking ties by choosing the        */
/*  lowest rightmost vertex).                                                */
/*                                                                           */
/*****************************************************************************/

void CTIN::divconqrecurse(point *sortarray, long  vertices, long  axis, struct CTriEdge *farleft, struct CTriEdge *farright)
{
        struct CTriEdge midtri, tri1, tri2, tri3;
        struct CTriEdge innerleft, innerright;
        double area;
        long  divider;

        //  if (verbose > 2) {    printf("  Triangulating %d points.\n", vertices);  }
        if (vertices == 2) {
                /* The triangulation of two vertices is an edge.  An edge is */
                /*   represented by two bounding triangles.                  */
                maketriangle(farleft);
                setorg(*farleft, sortarray[0]);
                setdest(*farleft, sortarray[1]);
                /* The apex is intentionally left NULL. */
                maketriangle(farright);
                setorg(*farright, sortarray[1]);
                setdest(*farright, sortarray[0]);
                /* The apex is intentionally left NULL. */
                bond(*farleft, *farright);
                lprevself(*farleft);
                lnextself(*farright);
                bond(*farleft, *farright);
                lprevself(*farleft);
                lnextself(*farright);
                bond(*farleft, *farright);
                /*    if (verbose > 2) {
                printf("  Creating ");
                printtriangle(farleft);
                printf("  Creating ");
                printtriangle(farright);
                }
                */
                /* Ensure that the origin of `farleft' is sortarray[0]. */
                lprev(*farright, *farleft);
                return;

        } else if (vertices == 3) {
                /* The triangulation of three vertices is either a triangle (with */

                /*   three bounding triangles) or two edges (with four bounding   */
                /*   triangles).  In either case, four triangles are created.     */
                maketriangle(&midtri);
                maketriangle(&tri1);
                maketriangle(&tri2);
                maketriangle(&tri3);
                area = counterclockwise(sortarray[0], sortarray[1], sortarray[2]);
                if (area == 0.0) {
                        /* Three collinear points; the triangulation is two edges. */
                        setorg(midtri, sortarray[0]);
                        setdest(midtri, sortarray[1]);
                        setorg(tri1, sortarray[1]);
                        setdest(tri1, sortarray[0]);
                        setorg(tri2, sortarray[2]);
                        setdest(tri2, sortarray[1]);
                        setorg(tri3, sortarray[1]);
                        setdest(tri3, sortarray[2]);
                        /* All apices are intentionally left NULL. */
                        bond(midtri, tri1);
                        bond(tri2, tri3);
                        lnextself(midtri);
                        lprevself(tri1);
                        lnextself(tri2);
                        lprevself(tri3);
                        bond(midtri, tri3);
                        bond(tri1, tri2);
                        lnextself(midtri);
                        lprevself(tri1);
                        lnextself(tri2);
                        lprevself(tri3);
                        bond(midtri, tri1);
                        bond(tri2, tri3);

                        /* Ensure that the origin of `farleft' is sortarray[0]. */
                        triedgecopy(tri1, *farleft);
                        /* Ensure that the destination of `farright' is sortarray[2]. */
                        triedgecopy(tri2, *farright);
                } else {
                        /* The three points are not collinear; the triangulation is one */
                        /*   triangle, namely `midtri'.                                 */
                        setorg(midtri, sortarray[0]);
                        setdest(tri1, sortarray[0]);
                        setorg(tri3, sortarray[0]);
                        /* Apices of tri1, tri2, and tri3 are left NULL. */
                        if (area > 0.0) {
                                /* The vertices are in counterclockwise order. */
                                setdest(midtri, sortarray[1]);
                                setorg(tri1, sortarray[1]);
                                setdest(tri2, sortarray[1]);
                                setapex(midtri, sortarray[2]);
                                setorg(tri2, sortarray[2]);
                                setdest(tri3, sortarray[2]);
                        } else {
                                /* The vertices are in clockwise order. */
                                setdest(midtri, sortarray[2]);
                                setorg(tri1, sortarray[2]);
                                setdest(tri2, sortarray[2]);
                                setapex(midtri, sortarray[1]);
                                setorg(tri2, sortarray[1]);

                                setdest(tri3, sortarray[1]);
                        }
                        /* The topology does not depend on how the vertices are ordered. */
                        bond(midtri, tri1);
                        lnextself(midtri);
                        bond(midtri, tri2);
                        lnextself(midtri);
                        bond(midtri, tri3);
                        lprevself(tri1);
                        lnextself(tri2);
                        bond(tri1, tri2);
                        lprevself(tri1);
                        lprevself(tri3);
                        bond(tri1, tri3);
                        lnextself(tri2);
                        lprevself(tri3);
                        bond(tri2, tri3);
                        /* Ensure that the origin of `farleft' is sortarray[0]. */
                        triedgecopy(tri1, *farleft);
                        /* Ensure that the destination of `farright' is sortarray[2]. */
                        if (area > 0.0) {
                                triedgecopy(tri2, *farright);
                        } else {
                                lnext(*farleft, *farright);
                        }
                }
                /*    if (verbose > 2) {
                printf("  Creating ");
                printtriangle(&midtri);
                printf("  Creating ");
                printtriangle(&tri1);
                printf("  Creating ");
                printtriangle(&tri2);
                printf("  Creating ");
                printtriangle(&tri3);
                }
                */
                return;
        } else {
                /* Split the vertices in half. */
                divider = vertices >> 1;
                /* Recursively triangulate each half. */
                divconqrecurse(sortarray, divider, 1 - axis, farleft, &innerleft);
                divconqrecurse(&sortarray[divider], vertices - divider, 1 - axis,
                        &innerright, farright);
                        /*    if (verbose > 1) {
                        printf("  Joining triangulations with %d and %d vertices.\n", divider,
                        vertices - divider);
                        }
                */
                /* Merge the two triangulations into one. */
                mergehulls(farleft, &innerleft, &innerright, farright, axis);
        }
}

long CTIN::removeghosts(struct CTriEdge *startghost)
{
        struct CTriEdge searchedge;
        struct CTriEdge dissolveedge;
        struct CTriEdge deadtri;
        point markorg;
        long hullsize;
        triangle ptr;                         /* Temporary variable used by sym(). */

        //  if (verbose) {    printf("  Removing ghost triangles.\n");  }
        /* Find an edge on the convex hull to start point location from. */
        lprev(*startghost, searchedge);
        symself(searchedge);
        dummytri[0] = encode(searchedge);
        /* Remove the bounding box and count the convex hull edges. */
        triedgecopy(*startghost, dissolveedge);
        hullsize = 0;
        do {
                hullsize++;
                lnext(dissolveedge, deadtri);
                lprevself(dissolveedge);
                symself(dissolveedge);
                /* If no PSLG is involved, set the boundary markers of all the points */
                /*   on the convex hull.  If a PSLG is used, this step is done later. */
                if (!poly)
                {
                        /* Watch out for the case where all the input points are collinear. */
                        if (dissolveedge.tri != dummytri)
                        {
                                org(dissolveedge, markorg);
                                if (pointmark(markorg) == 0)
                                {
                                        setpointmark(markorg, 1);
                                }
                        }
                }
                /* Remove a bounding triangle from a convex hull triangle. */
                dissolve(dissolveedge);
                /* Find the next bounding triangle. */
                sym(deadtri, dissolveedge);
                /* Delete the bounding triangle. */
                triangledealloc(deadtri.tri);
        } while (!triedgeequal(dissolveedge, *startghost));
        return hullsize;
}

/*****************************************************************************/
/*                                                                           */
/*  divconqdelaunay()   Form a Delaunay triangulation by the divide-and-     */
/*                      conquer method.                                      */
/*                                                                           */
/*  Sorts the points, calls a recursive procedure to triangulate them, and   */
/*  removes the bounding box, setting boundary markers as appropriate.       */
/*                                                                           */
/*****************************************************************************/

long CTIN::divconqdelaunay()
{
        point *sortarray;
        struct CTriEdge hullleft, hullright;
        long  divider;
        long  i;

        /* Allocate an array of pointers to points for sorting. */
        sortarray = (point *) malloc(inpoints * sizeof(point));
        if (sortarray == (point *) NULL) {
                printf("Error:  Out of memory.\n");
                exit(1);
        }
        traversalinit(&points);
        for (i = 0; i < inpoints; i++) {
                sortarray[i] = pointtraverse();
        }
        /* Sort the points. */
        pointsort(sortarray, inpoints);
        i = inpoints;
        if (dwyer) {
                /* Re-sort the array of points to accommodate alternating cuts. */
                divider = i >> 1;
                if (i - divider >= 2) {
                        if (divider >= 2) {
                                alternateaxes(sortarray, divider, 1);
                        }
                        alternateaxes(&sortarray[divider], i - divider, 1);
                }
        }
        /* Form the Delaunay triangulation. */
        divconqrecurse(sortarray, i, 0, &hullleft, &hullright);
        free(sortarray);
		sortarray = NULL;

        return removeghosts(&hullleft);
}

/**                                                                         **/
/**                                                                         **/
/********* Divide-and-conquer Delaunay triangulation ends here       *********/


/********* Segment (shell edge) insertion begins here                *********/
/**                                                                         **/
/**                                                                         **/

/*****************************************************************************/
/*                                                                           */
/*  finddirection()   Find the first triangle on the path from one point     */
/*                    to another.                                            */
/*                                                                           */
/*  Finds the triangle that intersects a line segment drawn from the         */
/*  origin of `searchtri' to the point `endpoint', and returns the result    */
/*  in `searchtri'.  The origin of `searchtri' does not change, even though  */
/*  the triangle returned may differ from the one passed in.  This routine   */
/*  is used to find the direction to move in to get from one point to        */
/*  another.                                                                 */
/*                                                                           */
/*  The return value notes whether the destination or apex of the found      */
/*  triangle is collinear with the two points in question.                   */
/*                                                                           */
/*****************************************************************************/

enum finddirectionresult CTIN::finddirection(struct CTriEdge *searchtri, point endpoint)
{
        struct CTriEdge checktri;
        point startpoint;
        point leftpoint, rightpoint;
        double leftccw, rightccw;
        long  leftflag, rightflag;
        triangle ptr;           /* Temporary variable used by onext() and oprev(). */

        org(*searchtri, startpoint);
        dest(*searchtri, rightpoint);
        apex(*searchtri, leftpoint);
        /* Is `endpoint' to the left? */
        leftccw = counterclockwise(endpoint, startpoint, leftpoint);
        leftflag = leftccw > 0.0;
        /* Is `endpoint' to the right? */
        rightccw = counterclockwise(startpoint, endpoint, rightpoint);
        rightflag = rightccw > 0.0;
        if (leftflag && rightflag) {

                /* `searchtri' faces directly away from `endpoint'.  We could go */
                /*   left or right.  Ask whether it's a triangle or a boundary   */
                /*   on the left.                                                */
                onext(*searchtri, checktri);
                if (checktri.tri == dummytri) {
                        leftflag = 0;
                } else {
                        rightflag = 0;
                }
        }
        while (leftflag) {
                /* Turn left until satisfied. */
                onextself(*searchtri);
                if (searchtri->tri == dummytri) {
                        printf("Internal error in finddirection():  Unable to find a\n");
                        printf("  triangle leading from (%.12g, %.12g) to", startpoint[0],
                                startpoint[1]);
                        printf("  (%.12g, %.12g).\n", endpoint[0], endpoint[1]);
                }
                apex(*searchtri, leftpoint);
                rightccw = leftccw;
                leftccw = counterclockwise(endpoint, startpoint, leftpoint);
                leftflag = leftccw > 0.0;
        }
        while (rightflag) {
                /* Turn right until satisfied. */
                oprevself(*searchtri);
                if (searchtri->tri == dummytri) {

                        printf("Internal error in finddirection():  Unable to find a\n");
                        printf("  triangle leading from (%.12g, %.12g) to", startpoint[0],
                                startpoint[1]);
                        printf("  (%.12g, %.12g).\n", endpoint[0], endpoint[1]);
                }
                dest(*searchtri, rightpoint);
                leftccw = rightccw;
                rightccw = counterclockwise(startpoint, endpoint, rightpoint);
                rightflag = rightccw > 0.0;
        }
        if (leftccw == 0.0) {
                return LEFTCOLLINEAR;
        } else if (rightccw == 0.0) {

                return RIGHTCOLLINEAR;
        } else {
                return WITHIN;
        }
}

/*****************************************************************************/
/*                                                                           */
/*  segmentintersection()   Find the intersection of an existing segment     */
/*                          and a segment that is being inserted.  Insert    */
/*                          a point at the intersection, splitting an        */
/*                          existing shell edge.                             */
/*                                                                           */
/*  The segment being inserted connects the apex of splittri to endpoint2.   */
/*  splitshelle is the shell edge being split, and MUST be opposite          */
/*  splittri.  Hence, the edge being split connects the origin and           */
/*  destination of splittri.                                                 */
/*                                                                           */
/*  On completion, splittri is a handle having the newly inserted            */
/*  intersection point as its origin, and endpoint1 as its destination.      */
/*                                                                           */
/*****************************************************************************/

void CTIN::segmentintersection(struct CTriEdge *splittri, struct CEdge *splitshelle, point endpoint2)
{
        point endpoint1;
        point torg, tdest;
        point leftpoint, rightpoint;
        point newpoint;
        enum insertsiteresult success;
        enum finddirectionresult collinear;
        double ex, ey;
        double tx, ty;
        double etx, ety;
        double split, denom;
        long  i;
        triangle ptr;                       /* Temporary variable used by onext(). */

        /* Find the other three segment endpoints. */
        apex(*splittri, endpoint1);
        org(*splittri, torg);
        dest(*splittri, tdest);
        /* Segment intersection formulae; see the Antonio reference. */
        tx = tdest[0] - torg[0];
        ty = tdest[1] - torg[1];
        ex = endpoint2[0] - endpoint1[0];
        ey = endpoint2[1] - endpoint1[1];
        etx = torg[0] - endpoint2[0];
        ety = torg[1] - endpoint2[1];
        denom = ty * ex - tx * ey;
        if (denom == 0.0) {
                printf("Internal error in segmentintersection():");
                printf("  Attempt to find intersection of parallel segments.\n");
        }
        split = (ey * etx - ex * ety) / denom;
        /* Create the new point. */
        newpoint = (point) poolalloc(&points);
        /* Interpolate its coordinate and attributes. */
        for (i = 0; i < 2; i++) {
                newpoint[i] = torg[i] + split * (tdest[i] - torg[i]);
        }
        setpointmark(newpoint, mark(*splitshelle));
        /*  if (verbose > 1) {
    printf(
    "  Splitting edge (%.12g, %.12g) (%.12g, %.12g) at (%.12g, %.12g).\n",
        torg[0], torg[1], tdest[0], tdest[1], newpoint[0], newpoint[1]);
        }
        */
        /* Insert the intersection point.  This should always succeed. */
        success = insertsite(newpoint, splittri, splitshelle, 0, 0);
        if (success != SUCCESSFULPOINT) {

                printf("Internal error in segmentintersection():\n");
                printf("  Failure to split a segment.\n");
        }

        /* Inserting the point may have caused edge flips.  We wish to rediscover */
        /*   the edge connecting endpoint1 to the new intersection point.         */
        collinear = finddirection(splittri, endpoint1);
        dest(*splittri, rightpoint);
        apex(*splittri, leftpoint);
        if ((leftpoint[0] == endpoint1[0]) && (leftpoint[1] == endpoint1[1])) {
                onextself(*splittri);
        } else if ((rightpoint[0] != endpoint1[0]) ||
                (rightpoint[1] != endpoint1[1])) {
                printf("Internal error in segmentintersection():\n");
                printf("  Topological inconsistency after splitting a segment.\n");
        }
        /* `splittri' should have destination endpoint1. */
}

/*****************************************************************************/
/*                                                                           */
/*  scoutsegment()   Scout the first triangle on the path from one endpoint  */
/*                   to another, and check for completion (reaching the      */
/*                   second endpoint), a collinear point, and the            */
/*                   intersection of two segments.                           */
/*                                                                           */
/*  Returns one if the entire segment is successfully inserted, and zero if  */
/*  the job must be finished by conformingedge() or constrainededge().       */
/*                                                                           */
/*  If the first triangle on the path has the second endpoint as its         */
/*  destination or apex, a shell edge is inserted and the job is done.       */
/*                                                                           */
/*  If the first triangle on the path has a destination or apex that lies on */
/*  the segment, a shell edge is inserted connecting the first endpoint to   */
/*  the collinear point, and the search is continued from the collinear      */
/*  point.                                                                   */
/*                                                                           */
/*  If the first triangle on the path has a shell edge opposite its origin,  */
/*  then there is a segment that intersects the segment being inserted.      */
/*  Their intersection point is inserted, splitting the shell edge.          */
/*                                                                           */
/*  Otherwise, return zero.                                                  */
/*                                                                           */
/*****************************************************************************/

long  CTIN::scoutsegment(struct CTriEdge *searchtri, point endpoint2, long  newmark)
{
        struct CTriEdge crosstri;
        struct CEdge crossedge;
        point leftpoint, rightpoint;
        point endpoint1;
        enum finddirectionresult collinear;
        shelle sptr;                      /* Temporary variable used by tspivot(). */

        collinear = finddirection(searchtri, endpoint2);
        dest(*searchtri, rightpoint);
        apex(*searchtri, leftpoint);
        if (((leftpoint[0] == endpoint2[0]) && (leftpoint[1] == endpoint2[1])) ||
                ((rightpoint[0] == endpoint2[0]) && (rightpoint[1] == endpoint2[1]))) {
                /* The segment is already an edge in the mesh. */
                if ((leftpoint[0] == endpoint2[0]) && (leftpoint[1] == endpoint2[1])) {
                        lprevself(*searchtri);
                }
                /* Insert a shell edge, if there isn't already one there. */
                insertshelle(searchtri, newmark);
                return 1;
        } else if (collinear == LEFTCOLLINEAR) {
                /* We've collided with a point between the segment's endpoints. */
                /* Make the collinear point be the triangle's origin. */
                lprevself(*searchtri);
                insertshelle(searchtri, newmark);
                /* Insert the remainder of the segment. */
                return scoutsegment(searchtri, endpoint2, newmark);
        } else if (collinear == RIGHTCOLLINEAR) {
                /* We've collided with a point between the segment's endpoints. */
                insertshelle(searchtri, newmark);
                /* Make the collinear point be the triangle's origin. */
                lnextself(*searchtri);
                /* Insert the remainder of the segment. */
                return scoutsegment(searchtri, endpoint2, newmark);
        } else {
                lnext(*searchtri, crosstri);
                tspivot(crosstri, crossedge);
                /* Check for a crossing segment. */
                if (crossedge.sh == dummysh) {
                        return 0;
                } else {
                        org(*searchtri, endpoint1);
                        /* Insert a point at the intersection. */
                        segmentintersection(&crosstri, &crossedge, endpoint2);
                        triedgecopy(crosstri, *searchtri);
                        insertshelle(searchtri, newmark);
                        /* Insert the remainder of the segment. */
                        return scoutsegment(searchtri, endpoint2, newmark);
                }
        }
}




/*****************************************************************************/
/*                                                                           */
/*  delaunayfixup()   Enforce the Delaunay condition at an edge, fanning out */
/*                    recursively from an existing point.  Pay special       */
/*                    attention to stacking inverted triangles.              */
/*                                                                           */
/*  This is a support routine for inserting segments into a constrained      */
/*  Delaunay triangulation.                                                  */
/*                                                                           */
/*  The origin of fixuptri is treated as if it has just been inserted, and   */
/*  the local Delaunay condition needs to be enforced.  It is only enforced  */
/*  in one sector, however, that being the angular range defined by          */
/*  fixuptri.                                                                */
/*                                                                           */
/*  This routine also needs to make decisions regarding the "stacking" of    */
/*  triangles.  (Read the description of constrainededge() below before      */
/*  reading on here, so you understand the algorithm.)  If the position of   */
/*  the new point (the origin of fixuptri) indicates that the vertex before  */
/*  it on the polygon is a reflex vertex, then "stack" the triangle by       */
/*  doing nothing.  (fixuptri is an inverted triangle, which is how stacked  */
/*  triangles are identified.)                                               */
/*                                                                           */
/*  Otherwise, check whether the vertex before that was a reflex vertex.     */
/*  If so, perform an edge flip, thereby eliminating an inverted triangle    */
/*  (popping it off the stack).  The edge flip may result in the creation    */
/*  of a new inverted triangle, depending on whether or not the new vertex   */
/*  is visible to the vertex three edges behind on the polygon.              */
/*                                                                           */
/*  If neither of the two vertices behind the new vertex are reflex          */
/*  vertices, fixuptri and fartri, the triangle opposite it, are not         */
/*  inverted; hence, ensure that the edge between them is locally Delaunay.  */
/*                                                                           */
/*  `leftside' indicates whether or not fixuptri is to the left of the       */
/*  segment being inserted.  (Imagine that the segment is pointing up from   */
/*  endpoint1 to endpoint2.)                                                 */
/*                                                                           */
/*****************************************************************************/

void CTIN::delaunayfixup(struct CTriEdge *fixuptri, long  leftside)
{
        struct CTriEdge neartri;
        struct CTriEdge fartri;
        struct CEdge faredge;
        point nearpoint, leftpoint, rightpoint, farpoint;
        triangle ptr;                         /* Temporary variable used by sym(). */
        shelle sptr;                      /* Temporary variable used by tspivot(). */

        lnext(*fixuptri, neartri);
        sym(neartri, fartri);
        /* Check if the edge opposite the origin of fixuptri can be flipped. */
        if (fartri.tri == dummytri) {
                return;
        }
        tspivot(neartri, faredge);
        if (faredge.sh != dummysh) {
                return;
        }
        /* Find all the relevant vertices. */
        apex(neartri, nearpoint);
        org(neartri, leftpoint);
        dest(neartri, rightpoint);
        apex(fartri, farpoint);
        /* Check whether the previous polygon vertex is a reflex vertex. */
        if (leftside) {
                if (counterclockwise(nearpoint, leftpoint, farpoint) <= 0.0) {
                        /* leftpoint is a reflex vertex too.  Nothing can */
                        /*   be done until a convex section is found.     */
                        return;
                }
        } else {
                if (counterclockwise(farpoint, rightpoint, nearpoint) <= 0.0) {
                        /* rightpoint is a reflex vertex too.  Nothing can */
                        /*   be done until a convex section is found.      */
                        return;
                }
        }
        if (counterclockwise(rightpoint, leftpoint, farpoint) > 0.0) {
                /* fartri is not an inverted triangle, and farpoint is not a reflex */
                /*   vertex.  As there are no reflex vertices, fixuptri isn't an    */
                /*   inverted triangle, either.  Hence, test the edge between the   */
                /*   triangles to ensure it is locally Delaunay.                    */
                if (incircle(leftpoint, farpoint, rightpoint, nearpoint) <= 0.0) {
                        return;
                }
                /* Not locally Delaunay; go on to an edge flip. */
        }        /* else fartri is inverted; remove it from the stack by flipping. */
        flip(&neartri);
        lprevself(*fixuptri);    /* Restore the origin of fixuptri after the flip. */
        /* Recursively process the two triangles that result from the flip. */
        delaunayfixup(fixuptri, leftside);
        delaunayfixup(&fartri, leftside);
}

/*****************************************************************************/
/*                                                                           */
/*  constrainededge()   Force a segment into a constrained Delaunay          */
/*                      triangulation by deleting the triangles it           */
/*                      intersects, and triangulating the polygons that      */
/*                      form on each side of it.                             */
/*                                                                           */
/*  Generates a single edge connecting `endpoint1' to `endpoint2'.  The      */
/*  triangle `starttri' has `endpoint1' as its origin.  `newmark' is the     */
/*  boundary marker of the segment.                                          */
/*                                                                           */
/*  To insert a segment, every triangle whose interior intersects the        */
/*  segment is deleted.  The union of these deleted triangles is a polygon   */
/*  (which is not necessarily monotone, but is close enough), which is       */
/*  divided into two polygons by the new segment.  This routine's task is    */
/*  to generate the Delaunay triangulation of these two polygons.            */
/*                                                                           */
/*  You might think of this routine's behavior as a two-step process.  The   */
/*  first step is to walk from endpoint1 to endpoint2, flipping each edge    */
/*  encountered.  This step creates a fan of edges connected to endpoint1,   */
/*  including the desired edge to endpoint2.  The second step enforces the   */
/*  Delaunay condition on each side of the segment in an incremental manner: */
/*  proceeding along the polygon from endpoint1 to endpoint2 (this is done   */
/*  independently on each side of the segment), each vertex is "enforced"    */
/*  as if it had just been inserted, but affecting only the previous         */
/*  vertices.  The result is the same as if the vertices had been inserted   */
/*  in the order they appear on the polygon, so the result is Delaunay.      */
/*                                                                           */
/*  In truth, constrainededge() interleaves these two steps.  The procedure  */
/*  walks from endpoint1 to endpoint2, and each time an edge is encountered  */
/*  and flipped, the newly exposed vertex (at the far end of the flipped     */
/*  edge) is "enforced" upon the previously flipped edges, usually affecting */
/*  only one side of the polygon (depending upon which side of the segment   */
/*  the vertex falls on).                                                    */
/*                                                                           */
/*  The algorithm is complicated by the need to handle polygons that are not */
/*  convex.  Although the polygon is not necessarily monotone, it can be     */
/*  triangulated in a manner similar to the stack-based algorithms for       */
/*  monotone polygons.  For each reflex vertex (local concavity) of the      */
/*  polygon, there will be an inverted triangle formed by one of the edge    */
/*  flips.  (An inverted triangle is one with negative area - that is, its   */
/*  vertices are arranged in clockwise order - and is best thought of as a   */
/*  wrinkle in the fabric of the mesh.)  Each inverted triangle can be       */
/*  thought of as a reflex vertex pushed on the stack, waiting to be fixed   */
/*  later.                                                                   */
/*                                                                           */
/*  A reflex vertex is popped from the stack when a vertex is inserted that  */
/*  is visible to the reflex vertex.  (However, if the vertex behind the     */
/*  reflex vertex is not visible to the reflex vertex, a new inverted        */
/*  triangle will take its place on the stack.)  These details are handled   */
/*  by the delaunayfixup() routine above.                                    */
/*                                                                           */
/*****************************************************************************/

void CTIN::constrainededge(struct CTriEdge *starttri, point endpoint2, long  newmark)
{
        struct CTriEdge fixuptri, fixuptri2;
        struct CEdge fixupedge;
        point endpoint1;
        point farpoint;
        double area;
        long  collision;
        long  done;
        triangle ptr;             /* Temporary variable used by sym() and oprev(). */
        shelle sptr;                      /* Temporary variable used by tspivot(). */

        org(*starttri, endpoint1);
        lnext(*starttri, fixuptri);
        flip(&fixuptri);
        /* `collision' indicates whether we have found a point directly */
        /*   between endpoint1 and endpoint2.                           */
        collision = 0;
        done = 0;
        do {
                org(fixuptri, farpoint);
                /* `farpoint' is the extreme point of the polygon we are "digging" */
                /*   to get from endpoint1 to endpoint2.                           */
                if ((farpoint[0] == endpoint2[0]) && (farpoint[1] == endpoint2[1])) {
                        oprev(fixuptri, fixuptri2);
                        /* Enforce the Delaunay condition around endpoint2. */
                        delaunayfixup(&fixuptri, 0);
                        delaunayfixup(&fixuptri2, 1);
                        done = 1;
                } else {
                        /* Check whether farpoint is to the left or right of the segment */
                        /*   being inserted, to decide which edge of fixuptri to dig     */
                        /*   through next.                                               */
                        area = counterclockwise(endpoint1, endpoint2, farpoint);
                        if (area == 0.0) {
                                /* We've collided with a point between endpoint1 and endpoint2. */
                                collision = 1;
                                oprev(fixuptri, fixuptri2);
                                /* Enforce the Delaunay condition around farpoint. */
                                delaunayfixup(&fixuptri, 0);
                                delaunayfixup(&fixuptri2, 1);
                                done = 1;
                        } else {
                                if (area > 0.0) {         /* farpoint is to the left of the segment. */
                                        oprev(fixuptri, fixuptri2);
                                        /* Enforce the Delaunay condition around farpoint, on the */
                                        /*   left side of the segment only.                       */
                                        delaunayfixup(&fixuptri2, 1);
                                        /* Flip the edge that crosses the segment.  After the edge is */
                                        /*   flipped, one of its endpoints is the fan vertex, and the */
                                        /*   destination of fixuptri is the fan vertex.               */
                                        lprevself(fixuptri);
                                } else {                 /* farpoint is to the right of the segment. */
                                        delaunayfixup(&fixuptri, 0);
                                        /* Flip the edge that crosses the segment.  After the edge is */
                                        /*   flipped, one of its endpoints is the fan vertex, and the */
                                        /*   destination of fixuptri is the fan vertex.               */
                                        oprevself(fixuptri);
                                }
                                /* Check for two intersecting segments. */
                                tspivot(fixuptri, fixupedge);
                                if (fixupedge.sh == dummysh) {
                                        flip(&fixuptri);   /* May create an inverted triangle on the left. */
                                } else {
                                        /* We've collided with a segment between endpoint1 and endpoint2. */
                                        collision = 1;
                                        /* Insert a point at the intersection. */

                                        segmentintersection(&fixuptri, &fixupedge, endpoint2);
                                        done = 1;
                                }
                        }
                }
        } while (!done);
        /* Insert a shell edge to make the segment permanent. */
        insertshelle(&fixuptri, newmark);
        /* If there was a collision with an interceding vertex, install another */
        /*   segment connecting that vertex with endpoint2.                     */
        if (collision) {
                /* Insert the remainder of the segment. */
                if (!scoutsegment(&fixuptri, endpoint2, newmark)) {
                        constrainededge(&fixuptri, endpoint2, newmark);
                }
        }
}

/*****************************************************************************/
/*                                                                           */
/*  insertsegment()   Insert a PSLG segment into a triangulation.            */
/*                                                                           */
/*****************************************************************************/

void CTIN::insertsegment(point endpoint1, point endpoint2, long  newmark)
{
        struct CTriEdge searchtri1, searchtri2;
        triangle encodedtri;
        point checkpoint;
        triangle ptr;                         /* Temporary variable used by sym(). */

                                                                                  /*  if (verbose > 1) {
                                                                                  printf("  Connecting (%.12g, %.12g) to (%.12g, %.12g).\n",
                                                                                  endpoint1[0], endpoint1[1], endpoint2[0], endpoint2[1]);
                                                                                  }
        */

        /* Find a triangle whose origin is the segment's first endpoint. */
        checkpoint = (point) NULL;
        encodedtri = point2tri(endpoint1);
        if (encodedtri != (triangle) NULL) {
                decode(encodedtri, searchtri1);
                org(searchtri1, checkpoint);
        }
        if (checkpoint != endpoint1) {
                /* Find a boundary triangle to search from. */
                searchtri1.tri = dummytri;
                searchtri1.orient = 0;
                symself(searchtri1);
                /* Search for the segment's first endpoint by point location. */
                if (locate(endpoint1, &searchtri1) != ONVERTEX) {
                        printf(
                                "Internal error in insertsegment():  Unable to locate PSLG point\n");
                        printf("  (%.12g, %.12g) in triangulation.\n",
                                endpoint1[0], endpoint1[1]);
                }
        }
        /* Remember this triangle to improve subsequent point location. */
        triedgecopy(searchtri1, recenttri);
        /* Scout the beginnings of a path from the first endpoint */
        /*   toward the second.                                   */
        if (scoutsegment(&searchtri1, endpoint2, newmark)) {
                /* The segment was easily inserted. */
                return;
        }
        /* The first endpoint may have changed if a collision with an intervening */
        /*   vertex on the segment occurred.                                      */
        org(searchtri1, endpoint1);

        /* Find a triangle whose origin is the segment's second endpoint. */
        checkpoint = (point) NULL;
        encodedtri = point2tri(endpoint2);
        if (encodedtri != (triangle) NULL) {
                decode(encodedtri, searchtri2);
                org(searchtri2, checkpoint);
        }
        if (checkpoint != endpoint2) {
                /* Find a boundary triangle to search from. */
                searchtri2.tri = dummytri;
                searchtri2.orient = 0;
                symself(searchtri2);
                /* Search for the segment's second endpoint by point location. */
                if (locate(endpoint2, &searchtri2) != ONVERTEX) {
                        printf(
                                "Internal error in insertsegment():  Unable to locate PSLG point\n");
                        printf("  (%.12g, %.12g) in triangulation.\n",
                                endpoint2[0], endpoint2[1]);
                }
        }
        /* Remember this triangle to improve subsequent point location. */
        triedgecopy(searchtri2, recenttri);
        /* Scout the beginnings of a path from the second endpoint */
        /*   toward the first.                                     */
        if (scoutsegment(&searchtri2, endpoint1, newmark)) {
                /* The segment was easily inserted. */
                return;
        }
        /* The second endpoint may have changed if a collision with an intervening */
        /*   vertex on the segment occurred.                                       */
        org(searchtri2, endpoint2);


    /* Insert the segment directly into the triangulation. */
    constrainededge(&searchtri1, endpoint2, newmark);
}

/*****************************************************************************/
/*                                                                           */
/*  markhull()   Cover the convex hull of a triangulation with shell edges.  */
/*                                                                           */
/*****************************************************************************/

void CTIN::markhull()
{
        struct CTriEdge hulltri;
        struct CTriEdge nexttri;
        struct CTriEdge starttri;
        triangle ptr;             /* Temporary variable used by sym() and oprev(). */

        /* Find a triangle handle on the hull. */
        hulltri.tri = dummytri;
        hulltri.orient = 0;
        symself(hulltri);
        /* Remember where we started so we know when to stop. */
        triedgecopy(hulltri, starttri);
        /* Go once counterclockwise around the convex hull. */
        do {
                /* Create a shell edge if there isn't already one here. */
                insertshelle(&hulltri, 1);
                /* To find the next hull edge, go clockwise around the next vertex. */
                lnextself(hulltri);
                oprev(hulltri, nexttri);
                while (nexttri.tri != dummytri) {
                        triedgecopy(nexttri, hulltri);
                        oprev(hulltri, nexttri);
                }
        } while (!triedgeequal(hulltri, starttri));
}

/**                                                                         **/
/**                                                                         **/
/********* Segment (shell edge) insertion ends here                  *********/

/*void CTIN::SavePtTINFile(char* filename)
{
FILE* file;

  if ((file = fopen(filename, "w")) == NULL){
                printf("Cann't Create file %s\n", filename);
                exit(1);
                }
                fprintf(file, "%ld %ld\n", lPointCount, lTriangleCount);
                fprintf(file, "%lf %lf %lf %lf %lf %lf\n", oBoundBox.xmin, oBoundBox.ymin, oBoundBox.zmin,
                oBoundBox.xmax, oBoundBox.ymax, oBoundBox.zmax);
                long i;
                for (i = 0; i < lPointCount; i++)
                fprintf(file, "%ld %lf %lf %lf\n", pPoints[i].id, pPoints[i].x, pPoints[i].y, pPoints[i].z);

                  CTriangle* tri;
                  for (i=0; i < lTriangleCount; i++)
                  {
                  tri = &pTriangles[i];
                  fprintf(file, "%ld %ld %ld %ld",tri->id, tri->pt[0]->id, tri->pt[1]->id, tri->pt[2]->id);
                  if (tri->tri[0] != NULL)
                  fprintf(file, " %ld",tri->tri[0]->id);
                  else
                  fprintf(file, " -1");
                  if (tri->tri[1] != NULL)
                  fprintf(file, " %ld",tri->tri[1]->id);
                  else
                  fprintf(file, " -1");
                  if (tri->tri[2] != NULL)
                  fprintf(file, " %ld\n",tri->tri[2]->id);
                  else
                  fprintf(file, " -1\n");
                  }
                  fclose(file);
                  }
*/
/*****************************************************************************/
/*                                                                           */
/*  main() or triangulate()   Gosh, do everything.                           */
/*                                                                           */
/*  The sequence is roughly as follows.  Many of these steps can be skipped, */
/*  depending on the command line switches.                                  */
/*                                                                           */
/*  - Initialize constants and parse the command line.                       */
/*  - Read the points from a file and either                                 */
/*    - triangulate them (no -r), or                                         */
/*  - Insert the PSLG segments (-p), and possibly segments on the convex     */
/*      hull (-c).                                                           */
/*  - Read the holes (-p), regional attributes (-pA), and regional area      */
/*      constraints (-pa).  Carve the holes and concavities, and spread the  */
/*      regional attributes and area constraints.                            */
/*  - Enforce the constraints on minimum angle (-q) and maximum area (-a).   */
/*      Also enforce the conforming Delaunay property (-q and -a).           */
/*  - Compute the number of edges in the resulting mesh.                     */
/*  - Promote the mesh's linear triangles to higher order elements (-o).     */
/*  - Write the output files and print the statistics.                       */
/*  - Check the consistency and Delaunay property of the mesh (-C).          */
/*                                                                           */
/*****************************************************************************/

bool CTIN::Delaunay()
{
        quality = vararea = fixedarea = 0;
        dwyer = 1;
        splitseg = 0;
        docheck = 0;
        nobisect = 0;
        order = 1;

        poly = 1;
        convex = 1;
        edgesout = 1;
        neighbors = 1;
        useshelles = poly || quality || convex;

        //************************************************************************************
        //begin transfernodes(in->pointlist, in->pointmarkerlist, in->numberofpoints);
        //*************************************************************************************

        point pointloop;
        long  i;
        long  coordindex;
        long  attribindex;

        inpoints = lPointCount;//numberofpoints;
        if (inpoints < 3) {
                printf("Error:  Input must have at least three input points.\n");
                return false;
        }

        initializepointpool();

        /* Read the points. */
        coordindex = 0;
        attribindex = 0;
        for (i = 0; i < inpoints; i++) {
                pointloop = (point) poolalloc(&points);
                /* Read the point coordinates. */
                pointloop[0] = pPoints[i].x;
                pointloop[1] = pPoints[i].y;
        }
        xmin = oBoundBox.xmin;
        xmax = oBoundBox.xmax;
        ymin = oBoundBox.ymin;
        ymax = oBoundBox.ymax;

        //************************************************************************************
        //end transfernodes(in->pointlist, in->pointmarkerlist, in->numberofpoints);
        //*************************************************************************************

        //hullsize = delaunay();                          /* Triangulate the points. */
        initializetrisegpools();
        //hullsize = divconqdelaunay();

        point *sortarray;
        struct CTriEdge hullleft, hullright;
        long  divider;

        /* Allocate an array of pointers to points for sorting. */
        sortarray = (point *) malloc(inpoints * sizeof(point));
        if (sortarray == (point *) NULL) {
                printf("Error:  Out of memory.\n");
                return false;
        }
        traversalinit(&points);
        for (i = 0; i < inpoints; i++) {
                sortarray[i] = pointtraverse();
        }
        /* Sort the points. */
        pointsort(sortarray, inpoints);
        i = inpoints;

        if (dwyer) {
                /* Re-sort the array of points to accommodate alternating cuts. */
                divider = i >> 1;
                if (i - divider >= 2) {
                        if (divider >= 2) {
                                alternateaxes(sortarray, divider, 1);
                        }
                        alternateaxes(&sortarray[divider], i - divider, 1);
                }
        }
        /* Form the Delaunay triangulation. */
        divconqrecurse(sortarray, i, 0, &hullleft, &hullright);
        free(sortarray);
		sortarray = NULL;

        hullsize = removeghosts(&hullleft);

        //writenodes();
        long  pointnumber;
        traversalinit(&points);
        pointloop = pointtraverse();
        pointnumber = 0;
        while (pointloop != (point) NULL) {
                setpointmark(pointloop, pointnumber);
                pointloop = pointtraverse();
                pointnumber++;
        }

        //	writeelements();	//writeneighbors();
        long  neighbor1, neighbor2, neighbor3;
        triangle ptr;                         /* Temporary variable used by sym(). */
        struct CTriEdge triangleloop;
        point pt1, pt2, pt3;
        struct CTriEdge trisym;
        long  elementnumber;

        pTriangles = (CTriangle *)calloc(triangles.items, sizeof(CTriangle));
        lTriangleCount = triangles.items;

        traversalinit(&triangles);
        triangleloop.tri = triangletraverse();
        triangleloop.orient = 0;
        elementnumber = 0;
        while (triangleloop.tri != (triangle *) NULL) {
                * (long *) (triangleloop.tri + 6) = elementnumber;
                triangleloop.tri = triangletraverse();
                elementnumber++;
        }
        * (long *) (dummytri + 6) = -1;

        traversalinit(&triangles);
        triangleloop.tri = triangletraverse();
        triangleloop.orient = 0;
        elementnumber = 0;
        while (triangleloop.tri != (triangle *) NULL) {
                //the three points.
                org(triangleloop, pt1);
                dest(triangleloop, pt2);
                apex(triangleloop, pt3);
                pTriangles[elementnumber].pt[0] = &pPoints[pointmark(pt1)];
                pTriangles[elementnumber].pt[1] = &pPoints[pointmark(pt2)];
                pTriangles[elementnumber].pt[2] = &pPoints[pointmark(pt3)];

                //the nearby triangles.
                triangleloop.orient = 1;
                sym(triangleloop, trisym);
                neighbor1 = * (long *) (trisym.tri + 6);
                triangleloop.orient = 2;
                sym(triangleloop, trisym);
                neighbor2 = * (long *) (trisym.tri + 6);
                triangleloop.orient = 0;
                sym(triangleloop, trisym);
                neighbor3 = * (long *) (trisym.tri + 6);

                if (neighbor1 != -1)
                        pTriangles[elementnumber].tri[0] = &pTriangles[neighbor1];
                else
                        pTriangles[elementnumber].tri[0] = NULL;

                if (neighbor2 != -1)
                        pTriangles[elementnumber].tri[1] = &pTriangles[neighbor2];
                else
                        pTriangles[elementnumber].tri[1] = NULL;
                if (neighbor3 != -1)
                        pTriangles[elementnumber].tri[2] = &pTriangles[neighbor3];
				else
					pTriangles[elementnumber].tri[2] = NULL;
				pTriangles[elementnumber].id = elementnumber;

				triangleloop.tri = triangletraverse();
				elementnumber++;
        }
        triangledeinit();
        return true;
}



//用于从等高线构建TIN
bool CTIN::Delaunay(CBlockContour* pBlockContour, CBlockTIN* pBlockTIN)
{
        oBoundBox = pBlockContour->oBoundBox;
        //
        CPoint3D *pInPoints = pBlockContour->pPoints;

        lInLineCount = pBlockContour->lConstrainLineCount;
        pInLines = pBlockContour->pConstrainLines;

        lContourCount = pBlockContour->lContourCount;
        pContours = (CTContour*)calloc(lContourCount, sizeof(CTContour));


        lTPointCount = pBlockContour->lPointCount; //
        long lTLineCount = lInLineCount;				//
        long i, j;
        CTContour* pCurContour;			//等高线;
        for (i = 0; i < lContourCount; i++)
        {
                pCurContour = &pContours[i];
                pCurContour->z = pBlockContour->pContours[i].z;
                pCurContour->ptnum = pBlockContour->pContours[i].nPointCount;
                pCurContour->pts = (CTPoint*)calloc(pCurContour->ptnum, sizeof(CTPoint));
                for (j = 0; j < pCurContour->ptnum; j++)
                {
                        pCurContour->pts[j].x = pBlockContour->pContours[i].pPoints[j].x;
                        pCurContour->pts[j].y = pBlockContour->pContours[i].pPoints[j].y;
                        pCurContour->pts[j].pContour = pCurContour;
                        pCurContour->pts[j].id = lTPointCount;
                        lTPointCount++;
                }

                free(pBlockContour->pContours[i].pPoints);
                pBlockContour->pContours[i].pPoints = NULL;


                if ((pCurContour->ptnum > 1)&&(pCurContour->pts[pCurContour->ptnum - 1] == pCurContour->pts[0])){
                        lTPointCount --;
                        pCurContour->pts[pCurContour->ptnum - 1] = pCurContour->pts[0];
                }

                lTLineCount += pCurContour->ptnum - 1;
        }

        free(pBlockContour->pContours);
        pBlockContour->pContours = NULL;

        pTPoints = (CTPoint*)calloc(lTPointCount, sizeof(CTPoint));
        for (i = 0; i < pBlockContour->lPointCount; i++){
                pTPoints[i].x = pInPoints[i].x;
                pTPoints[i].y = pInPoints[i].y;
                pTPoints[i].id = i;
                pTPoints[i].pContour = (CTContour*)calloc(1, sizeof(CTContour));
                pTPoints[i].pContour->z = pInPoints[i].z;
                pTPoints[i].pContour->ptnum = 1;
        }
        free(pBlockContour->pPoints);
        pBlockContour->pPoints = NULL;

        CConstrainLine* pTLines = (CConstrainLine*)calloc(lTLineCount, sizeof(CConstrainLine));
        memcpy(pTLines, pInLines, sizeof(CConstrainLine)*lInLineCount);
        free(pInLines);
		pInLines = NULL;
        pBlockContour->pConstrainLines = NULL;


        lTLineCount = lInLineCount;
        lTPointCount = pBlockContour->lPointCount;
        for (i = 0; i<lContourCount; i++)
        {
                for (long j = 0; j<pContours[i].ptnum; j++)
                {
                        if (j > 0)
                        {
                                pTLines[lTLineCount].frompt = pContours[i].pts[j - 1].id;
                                pTLines[lTLineCount].endpt = pContours[i].pts[j].id;
                        }
                }
                //把等高线的点复制；//
                if ((pContours[i].pts[pContours[i].ptnum - 1] == pContours[i].pts[0])&&(pContours[i].ptnum > 1)){
                        memcpy(pTPoints + lTPointCount, pContours[i].pts, sizeof(CTPoint) * (pContours[i].ptnum - 1));
                        lTPointCount += pContours[i].ptnum - 1;
                }
                else{
                        memcpy(pTPoints + lTPointCount, pContours[i].pts, sizeof(CTPoint) * pContours[i].ptnum);
                        lTPointCount += pContours[i].ptnum;
                }
        }

        triangleinit();
        quality = vararea = fixedarea = 0;
        dwyer = 1;
        splitseg = 0;
        docheck = 0;
        nobisect = 0;
        order = 1;

        poly = 1;
        convex = 1;
        edgesout = 1;
        neighbors = 1;
        useshelles = poly || quality || convex;

        inpoints = 0;//numberofpoints;
        initializepointpool();

        point pointloop;
        pointloop = (point) poolalloc(&points);
        pointloop[0] = pTPoints[0].x;
        pointloop[1] = pTPoints[0].y;

        for (i = 1; i < lTPointCount; i++)
        {
                pointloop = (point) poolalloc(&points);
                // Read the point coordinates.
                pointloop[0] = pTPoints[i].x;
                pointloop[1] = pTPoints[i].y;
        }
        //
        lTPointNum = lTPointCount;

        xmin = oBoundBox.xmin;
        xmax = oBoundBox.xmax;
        ymin = oBoundBox.ymin;
        ymax = oBoundBox.ymax;


        //* Triangulate the points.
        initializetrisegpools();

        point *sortarray;
        struct CTriEdge hullleft, hullright;
        long  divider;

        inpoints = lTPointNum;
        //* Allocate an array of pointers to points for sorting. /
        sortarray = (point *) malloc(inpoints * sizeof(point));
        if (sortarray == (point *) NULL) {
                printf("Error:  Out of memory.\n");
                return false;
        }
        traversalinit(&points);
        for (i = 0; i < inpoints; i++) {
                sortarray[i] = pointtraverse();
        }
        //* Sort the points.
        pointsort(sortarray, inpoints);
        i = inpoints;

        if (dwyer) {
                //* Re-sort the array of points to accommodate alternating cuts.
                divider = i >> 1;
                if (i - divider >= 2) {
                        if (divider >= 2) {
                                alternateaxes(sortarray, divider, 1);
                        }
                        alternateaxes(&sortarray[divider], i - divider, 1);
                }
        }
        // Form the Delaunay triangulation.
        divconqrecurse(sortarray, i, 0, &hullleft, &hullright);
        free(sortarray);
		sortarray = NULL;

        hullsize = removeghosts(&hullleft);

        if (useshelles) {
                checksegments = 1;
                // Insert PSLG segments and/or convex hull segments.
                //insegments = formskeleton(in->segmentlist, in->segmentmarkerlist,	in->numberofsegments);
                long  index;
                point endpoint1, endpoint2;
                long  boundmarker;
                long  i;

                if (poly)
                {
                        //    if (!quiet) {      printf("Inserting segments into Delaunay triangulation.\n");    }
                        insegments = lTLineCount;
                        index = 0;
                        // If segments are to be inserted, compute a mapping from points to triangles.
                        if (insegments > 0)
                        {
                                //if (verbose) {        printf("  Inserting PSLG segments.\n");      }
                                makepointmap();
                        }

                        boundmarker = 0;
                        //* Read and insert the segments.
                        for (i = 0; i < insegments; i++)
                        {
                                endpoint1 = getpoint(pTLines[i].frompt);
                                endpoint2 = getpoint(pTLines[i].endpt);
                                insertsegment(endpoint1, endpoint2, boundmarker);
                        }

                        if (convex || !poly)
                        {
                                // Enclose the convex hull with shell edges.
                                //if (verbose) {printf("  Enclosing convex hull with segments.\n");    }
                                markhull();
                        }
                }
        }

        //释放用于约束线.
        free(pTLines);
		pTLines = NULL;

        //writenodes();
        long  pointnumber;
        traversalinit(&points);
        pointloop = pointtraverse();
        pointnumber = 0;
        while (pointloop != (point) NULL)
        {
                setpointmark(pointloop, pointnumber);
                pointloop = pointtraverse();
                pointnumber++;
        }

        //	writeelements();	//writeneighbors();
        long  neighbor1, neighbor2, neighbor3;
        triangle ptr;                         //* Temporary variable used by sym().
        struct CTriEdge triangleloop;
        point pt1, pt2, pt3;
        struct CTriEdge trisym;
        long  elementnumber;

        lTTriangleCount = triangles.items;
        pTTriangles = (CTTriangle *)calloc(lTTriangleCount, sizeof(CTTriangle));

        traversalinit(&triangles);
        triangleloop.tri = triangletraverse();
        triangleloop.orient = 0;
        elementnumber = 0;
        while (triangleloop.tri != (triangle *) NULL)
        {
                * (long *) (triangleloop.tri + 6) = elementnumber;
                triangleloop.tri = triangletraverse();
                elementnumber++;
        }
        * (long *) (dummytri + 6) = -1;

        traversalinit(&triangles);
        triangleloop.tri = triangletraverse();
        triangleloop.orient = 0;
        elementnumber = 0;
        while (triangleloop.tri != (triangle *) NULL)
        {
                //the three points.
                org(triangleloop, pt1);
                dest(triangleloop, pt2);
                apex(triangleloop, pt3);
                pTTriangles[elementnumber].pt[0] = &pTPoints[pointmark(pt1)];
                pTTriangles[elementnumber].pt[1] = &pTPoints[pointmark(pt2)];
                pTTriangles[elementnumber].pt[2] = &pTPoints[pointmark(pt3)];

                //the nearby triangles.
                triangleloop.orient = 1;
                sym(triangleloop, trisym);
                neighbor1 = * (long *) (trisym.tri + 6);
                triangleloop.orient = 2;
                sym(triangleloop, trisym);
                neighbor2 = * (long *) (trisym.tri + 6);
                triangleloop.orient = 0;
                sym(triangleloop, trisym);
                neighbor3 = * (long *) (trisym.tri + 6);

                if (neighbor1 != -1)
                        pTTriangles[elementnumber].tri[0] = &pTTriangles[neighbor1];
                else
                        pTTriangles[elementnumber].tri[0] = NULL;
                if (neighbor2 != -1)
                        pTTriangles[elementnumber].tri[1] = &pTTriangles[neighbor2];
                else
                        pTTriangles[elementnumber].tri[1] = NULL;
                if (neighbor3 != -1)
                        pTTriangles[elementnumber].tri[2] = &pTTriangles[neighbor3];
                else
                        pTTriangles[elementnumber].tri[2] = NULL;
                pTTriangles[elementnumber].id = elementnumber;

                triangleloop.tri = triangletraverse();
                elementnumber++;
        }
        triangledeinit();

        //add the triangles to the chain.
        CTTriangleChain *pThisChain, *pPreChain;
        pTriChain = (CTTriangleChain*)calloc(1, sizeof(CTTriangleChain));
        pThisChain = pTriChain;
        pThisChain->tri = &pTTriangles[0];
        pThisChain->tri->chain = pThisChain;
        pThisChain->pre = NULL;
        pThisChain->next = NULL;
        pPreChain = pThisChain;
        set_triangle_flags(&pTTriangles[0]);

        //other trianglecount-1
        for (i=1; i<lTTriangleCount; i++)
        {
                pThisChain = (CTTriangleChain*)calloc(1, sizeof(CTTriangleChain));
                pThisChain->tri = &pTTriangles[i];
                pThisChain->tri->chain = pThisChain;
                pThisChain->pre = pPreChain;
                pThisChain->next = NULL;
                pPreChain->next = pThisChain;
                pPreChain = pThisChain;
                set_triangle_flags(&pTTriangles[i]);
        }
        lTTriangleNum = lTTriangleCount;
        pTriEndChain = pThisChain;		//

        process_flat_tri();

        //
        pBlockTIN->oBoundBox = oBoundBox;
        pBlockTIN->lPointCount = lTPointCount;
        pBlockTIN->pPoints = (CPoint3D*)calloc(lTPointCount, sizeof(CPoint3D));

        CTPoint* pt;
        for (i=0; i<lTPointNum; i++)
        {
                pBlockTIN->pPoints[i] = CPoint3D(pTPoints[i].id, pTPoints[i].x, pTPoints[i].y, pTPoints[i].pContour->z);
        }
        if (pPtChain){
                CTPointChain* ptChain = pPtChain;
                while(ptChain)
                {
                        pt = ptChain->pt;
                        pBlockTIN->pPoints[pt->id] = CPoint3D(pt->id, pt->x, pt->y, pt->pContour->z);
                        free(ptChain->pt->pContour);
						ptChain->pt->pContour = NULL;
                        ptChain = ptChain->next;
                }
        }

        pBlockTIN->lTriangleCount = lTTriangleCount;
        pBlockTIN->pTriangles = (CIOTriangle*)calloc(lTTriangleCount, sizeof(CIOTriangle));
        CTTriangleChain* pChain = pTriChain;
        CTTriangle* tri;
        CIOTriangle *pIOTri;
        while(pChain)
        {
                tri = pChain->tri;
                pChain = pChain->next;

                pIOTri = &pBlockTIN->pTriangles[tri->id];
                pIOTri->id = tri->id;
                pIOTri->pt[0] = tri->pt[0]->id;
                pIOTri->pt[1] = tri->pt[1]->id;
                pIOTri->pt[2] = tri->pt[2]->id;
                if (tri->tri[0] != NULL)
                        pIOTri->tri[0] = tri->tri[0]->id;
                if (tri->tri[1] != NULL)
                        pIOTri->tri[1] = tri->tri[1]->id;
                if (tri->tri[2] != NULL)
                        pIOTri->tri[2] = tri->tri[2]->id;
        }

        if (pPtChain)
        {
                CTPointChain* ptchain;
                while(pPtChain!= NULL)
                {
                        ptchain = pPtChain;
                        pPtChain = pPtChain->next;
                        free(ptchain->pt);
						ptchain->pt  = NULL;
                        free(ptchain);
						ptchain = NULL;
                }
                free(pPtChain);
				pPtChain = NULL;
        }
        if (pTPoints)
        {
                for (i=0; i<pBlockContour->lPointCount; i++)
                      {  free(pTPoints[i].pContour);
						pTPoints[i].pContour = NULL;
				}
                free(pTPoints);
                pTPoints = NULL;
        }


        if (pTriChain)
        {
                CTTriangleChain* pChain;
                while(pTriChain != NULL)
                {
                        pChain = pTriChain;
                        pTriChain = pTriChain->next;
                        if (pChain->tri->id >= lTTriangleNum)
                        {
							free(pChain->tri);
							pChain->tri = NULL;
						}
								
                        free(pChain);
						pChain = NULL;
                };
                free(pTriChain);
				pTriChain = NULL;
        }
        if (pTTriangles)
                {
					free(pTTriangles);
					pTTriangles = NULL;
		}
        if (pContours)
        {
                for (long i=0; i<lContourCount; i++)
                {
                        free(pContours[i].pts);
						pContours[i].pts = NULL;
                }
                free(pContours);
				pContours = NULL;
        }
        return true;
}

//delete the outside flat triangles.
void CTIN::process_flat_tri()
{
        CTTriangleChain* pChain = pTriChain;
        CTTriangleChain *pFlatChain;
        CTTriangleChain *pTempChain;
        CTTriangle* tri, *ntri;
        bool bHaveMoved;
        while(pChain != NULL){
                bHaveMoved = false;
                tri = pChain->tri;
                if (tri->id >= lTTriangleNum)		//如果是新添加的三角形，则对其不处理.
                        break;
                if (Is_Flat_Tri(tri)){			//
                        construct_flat_polygon(tri, &pFlatChain);
                        CTTriangleChain *thisChain = pFlatChain;
                        bool bVerge = false;
                        while(thisChain != NULL)
                        {
                                tri = thisChain->tri;
                                if (((tri->tri[0] == NULL)&&(!tri->flag[0]))
                                        ||((tri->tri[1] == NULL)&&(!tri->flag[1]))
                                        ||((tri->tri[2] == NULL)&&(!tri->flag[2])))	//
                                {
                                        bVerge = true;
                                        break;
                                }
                                else
                                        thisChain = thisChain->next;
                        }
                        if (bVerge){		//
                                thisChain = pFlatChain;
                                while(pFlatChain){
                                        //
                                        tri = thisChain->tri;
                                        for (long i=0; i<3; i++){
                                                ntri = tri->tri[i];
                                                if (ntri != NULL){		//与该三角形相邻的三角形的相应三角形为 NULL.
                                                        if (ntri->tri[0] == tri)
                                                                ntri->tri[0] = NULL;
                                                        else if (ntri->tri[1] == tri)
                                                                ntri->tri[1] = NULL;
                                                        else// if (ntri->tri[0] == tri)
                                                                ntri->tri[2] = NULL;
                                                }
                                        }
                                        //把该链从链表中移出.
                                        pTempChain = tri->chain;
                                        if (tri->chain->pre)
                                                tri->chain->pre->next = tri->chain->next;
                                        if (tri->chain->next)
                                                tri->chain->next->pre = tri->chain->pre;
                                        if (pTempChain == pChain){			//
                                                pChain = pChain->next;
                                                bHaveMoved = true;
                                        }
                                        if (tri->id >= lTTriangleNum)				//
                                        {
											free(tri);
											tri = NULL;
										}
										
										if(pTempChain == pTriChain)//增加一个判断 吴恩平 
										{
											free(pTempChain);
											pTempChain = NULL;
											pTriChain = NULL;
											
										}
										else
										{
											free(pTempChain);
											pTempChain = NULL;
										}

                                        //
                                        pFlatChain = pFlatChain->next;
										if(thisChain == pTriChain)//增加一个判断 吴恩平 
										{
											free(thisChain);
											thisChain = pFlatChain;
											pTriChain = NULL;
										}
										else
										{
											free(thisChain);
											thisChain = pFlatChain;
										}
                                }
                        }
                        else{
                                process_inner_flat_polygon(pFlatChain);
                        }

                }
                if (!bHaveMoved)
                        pChain = pChain->next;
        }

        //对链进行重新排序，使由于原来删除的多边形而导致的三角形id
        long lThisTriNum = 0;
        bool bFind = false;
        pChain = pTriChain;
			while(pChain){
				tri = pChain->tri;
				if(tri !=NULL)
				{
					if ((!bFind)&&(tri->id >= lTTriangleNum))
					{//
						lTTriangleNum = lThisTriNum;
						bFind = true;
					}
					tri->id = lThisTriNum;

					lThisTriNum++;
					pChain = pChain->next;
				}
			}

        lTTriangleCount = lThisTriNum;

}

//construct flat polygon
void CTIN::construct_flat_polygon(CTTriangle* flat_tri, CTTriangleChain** flat_polygon)
{
        CTTriangleChain* flat_poly = NULL;// = (CTTriangleChain*)malloc(sizeof(CTTriangleChain));	//
        CTTriangleChain* tempChain = (CTTriangleChain*)malloc(sizeof(CTTriangleChain));	//
        tempChain->pre = tempChain->next = NULL;
        tempChain->tri = flat_tri;				//把初始三角形存入没有处理的三角形链表;
        CTTriangleChain* temp;					//
        CTTriangle* curr_tri;						//
        CTTriangle* temp_tri;
        while(tempChain != NULL){
                curr_tri = tempChain->tri;
                for (long i=0; i<3; i++){
                        temp_tri = curr_tri->tri[i];
                        if ((!curr_tri->flag[i])&&(temp_tri != NULL)&&(Is_Flat_Tri(temp_tri)))
                        {
                                temp = tempChain;			//检查是否有已经在三角形链表中;
                                bool bSame = false;
                                while(temp != NULL)
                                {
                                        if (temp->tri == temp_tri)		//
                                        {
                                                bSame = true;
                                                break;
                                        }
                                        else
                                                temp = temp->next;
                                }
                                if (!bSame)							//
                                {
                                        temp = flat_poly;
                                        while(temp != NULL){			//
                                                if (temp->tri == temp_tri){
                                                        bSame = true;
                                                        break;
                                                }
                                                else
                                                        temp = temp->next;
                                        }
                                }
                                if (!bSame){							//
                                        temp = (CTTriangleChain*)malloc(sizeof(CTTriangleChain));
                                        temp->tri = temp_tri;				//
                                        temp->pre = tempChain;
                                        temp->next = tempChain->next;
                                        tempChain->next = temp;
                                }
                        }
                }
                //
                temp = tempChain;
                tempChain = tempChain->next;
                if (tempChain)
                        tempChain->pre = NULL;

                temp->next = flat_poly;
                if (flat_poly)
                        flat_poly->pre = temp;
                flat_poly = temp;
                flat_poly->pre = NULL;
        }
        *flat_polygon = flat_poly;
}

//set the triangle flags.
void CTIN::set_triangle_flags(CTTriangle* tri)
{
        long ii, iii;
        CTContour* cont1;
        for (long i=0; i<3; i++){
                ii = (i + 1)%3;
                iii = (i + 2)%3;

                if (tri->pt[i]->pContour == tri->pt[ii]->pContour){
                        if (labs(tri->pt[i]->id - tri->pt[ii]->id) == 1)
                                tri->flag[iii] = true;
                        else{
                                cont1 = tri->pt[i]->pContour;
                                if ((cont1->ptnum > 2)&&(cont1->pts[0] == cont1->pts[cont1->ptnum - 1])&&
                                        (((cont1->pts[0].id  == tri->pt[i]->id)&&(cont1->pts[cont1->ptnum - 2].id == tri->pt[ii]->id))||
                                        ((cont1->pts[0].id == tri->pt[ii]->id)&&(cont1->pts[cont1->ptnum - 2].id == tri->pt[i]->id))))
                                        tri->flag[iii] = true;
                                else
                                        for (long j=0; j<lInLineCount; j++)
                                                if (((tri->pt[i]->id == pInLines[j].frompt)&&(tri->pt[ii]->id == pInLines[j].endpt))||
                                                        ((tri->pt[ii]->id == pInLines[j].frompt)&&(tri->pt[i]->id == pInLines[j].endpt))){
                                                        tri->flag[iii] = true;
                                                        break;
                                                }
                        }
                }
        }

}

void CTIN::process_inner_flat_polygon(CTTriangleChain *flat_poly)
{
        CTTriangleChain *thisChain, *pTempChain;
        CTTriangle* tri, *ntri;

        //
        CTPoint *pt, *pt1, *pt2, *pt3;
        CTTriangle *tri1, *ntri1;
        double dA, dB, dC, dD;
        long triindex, ntriindex;
        thisChain = flat_poly;
        long i;
        while(thisChain != NULL){
                bool bFindThisTri = false;
                pTempChain = thisChain;
                while(pTempChain != NULL){
                        tri = pTempChain->tri;
                        for ( i=0; i<3; i++){
                                ntri = tri->tri[i];
                                if ((ntri)&&(!tri->flag[i])&&(!Is_Flat_Tri(ntri))){
                                        bFindThisTri = true;
                                        break;
                                }
                        }
                        if (bFindThisTri)
                                break;
                        else
                                pTempChain = pTempChain->next;
                }
                if (pTempChain == NULL){
                        break;
                }
                if ((pTempChain->pre == NULL)&&(pTempChain->next == NULL)){
                        free(thisChain);
                        thisChain = NULL;
                }
                else{
                        if (pTempChain->pre){
                                pTempChain->pre->next = pTempChain->next;
                        }
                        if (pTempChain->next){
                                pTempChain->next->pre = pTempChain->pre;
                        }
                        if (pTempChain == thisChain){
                                if (pTempChain->next)
                                        thisChain = thisChain->next;
                                else
                                        thisChain = thisChain->pre;
                        }
                        free(pTempChain);
						pTempChain = NULL;
                }

                for (i = 0; i < 3; i++){
                        if (tri->tri[i] == ntri)
                                triindex = i;
                        if (ntri->tri[i] == tri)
                                ntriindex = i;
                }

                //
                long triindex1 = (triindex + 1)%3;
                long triindex2 =  (triindex + 2)%3;
                long ntriindex1 = (ntriindex + 1)%3;
                long ntriindex2 = (ntriindex + 2)%3;

                pt = ntri->pt[ntriindex];
                pt1 = ntri->pt[ntriindex1];
                pt2 =  ntri->pt[ntriindex2];
                pt3 = tri->pt[triindex];

                if (!IsLeft(pt, pt1, pt3)){//Angle(pt1, pt3, pt) >= PI){	//
                        CTPoint* newpt = new CTPoint();		//create a new point.
                        CTContour* newcont = new CTContour();
                        newpt->pContour = newcont;
                        newpt->id = lTPointCount;
                        lTPointCount ++;
                        newpt->x = (pt1->x + pt2->x)/2.0;
                        newpt->y = (pt1->y + pt2->y)/2.0;
                        newpt->pContour->z = (pt1->pContour->z + pt2->pContour->z)/2.0;
                        //calculate the z value of the new point.
                        dA = (pt->y - pt3->y) * (pt2->pContour->z- pt3->pContour->z) - (pt2->y - pt3->y) * (pt->pContour->z- pt3->pContour->z);
                        dB = (pt2->x - pt3->x) * (pt->pContour->z- pt3->pContour->z) - (pt->x - pt3->x) * (pt2->pContour->z- pt3->pContour->z);
                        dC = (pt->x - pt3->x) * (pt2->y- pt3->y) - (pt2->x - pt3->x) * (pt->y- pt3->y);
                        dD = -(dA * pt3->x + dB * pt3->y + dC *pt3->pContour->z);
                        newpt->pContour->z = (newpt->pContour->z -(dA * newpt->x + dB * newpt->y + dD) / dC)/2.0 ;

                        CTPointChain* newptchain = (CTPointChain*)calloc(1, sizeof(CTPointChain));
                        newptchain->pt = newpt;
                        if (pPtChain == NULL){
                                pPtChain = pPtEndChain = newptchain;
                        }
                        else{
                                pPtEndChain->next = newptchain;
                                pPtEndChain = pPtEndChain->next;
                        }

                        CTTriangle* newtri = (CTTriangle*)calloc(1, sizeof(CTTriangle));
                        CTTriangle* nnewtri = (CTTriangle*)calloc(1, sizeof(CTTriangle));
                        memcpy(newtri, tri, sizeof(CTTriangle));
                        memcpy(nnewtri, ntri, sizeof(CTTriangle));
                        newtri->id = lTTriangleCount; lTTriangleCount++;
                        nnewtri->id = lTTriangleCount; lTTriangleCount++;

                        tri->pt[triindex2] = newpt;
                        newtri->pt[triindex1] = newpt;

                        ntri->pt[ntriindex1] = newpt;
                        nnewtri->pt[ntriindex2] = newpt;

                        tri1 = tri->tri[triindex1];
                        if (tri1)							//is not null;
                                for (i=0; i<3; i++)
                                        if (tri1->tri[i] == tri){
                                                tri1->tri[i] = newtri;
                                                break;}
                                        newtri->tri[triindex1] = tri1;
                                        newtri->tri[triindex] = nnewtri;
                                        newtri->tri[triindex2] = tri;
                                        newtri->flag[triindex2] = false;
                                        tri->tri[triindex1] = newtri;
                                        tri->flag[triindex1] = false;

                                        ntri1 = ntri->tri[ntriindex2];
                                        if (ntri1)						//is not null.
                                                for (i=0; i<3; i++)
                                                        if (ntri1->tri[i] == ntri){
                                                                ntri1->tri[i] = nnewtri;
                                                                break;}
                                                        nnewtri->tri[ntriindex] = newtri;
                                                        nnewtri->tri[ntriindex1] = ntri;
                                                        nnewtri->tri[ntriindex2] = ntri1;
                                                        nnewtri->flag[ntriindex1] = false;
                                                        ntri->tri[ntriindex2] = nnewtri;
                                                        ntri->flag[ntriindex2] = false;

                                                        //add the two new triangle to the triangle chain.
                                                        CTTriangleChain* newchain = (CTTriangleChain*)calloc(1, sizeof(CTTriangleChain));
                                                        newchain->tri = newtri;
                                                        newtri->chain = newchain;
                                                        newchain->pre = pTriEndChain;
                                                        pTriEndChain->next = newchain;
                                                        pTriEndChain = pTriEndChain->next;

                                                        newchain = (CTTriangleChain*)calloc(1, sizeof(CTTriangleChain));
                                                        newchain->tri = nnewtri;
                                                        nnewtri->chain = newchain;
                                                        newchain->pre = pTriEndChain;
                                                        pTriEndChain->next = newchain;
                                                        pTriEndChain = pTriEndChain->next;

                }
                else if (IsLeft(pt, pt2, pt3)){//(Angle(pt2, pt, pt3) >= PI){//
                        CTPoint* newpt = (CTPoint*)calloc(1, sizeof(CTPoint));		//create a new point.
                        CTContour* newcont = (CTContour*)calloc(1 ,sizeof(CTContour));
                        newpt->pContour = newcont;
                        newpt->id = lTPointCount;
                        lTPointCount ++;
                        newpt->x = (pt1->x + pt2->x)/2.0;
                        newpt->y = (pt1->y + pt2->y)/2.0;
                        newpt->pContour->z = (pt1->pContour->z + pt2->pContour->z)/2.0;
                        //calculate the z value of the new point.
                        dA = (pt->y - pt3->y) * (pt1->pContour->z- pt3->pContour->z) - (pt1->y - pt3->y) * (pt->pContour->z- pt3->pContour->z);
                        dB = (pt1->x - pt3->x) * (pt->pContour->z- pt3->pContour->z) - (pt->x - pt3->x) * (pt1->pContour->z- pt3->pContour->z);
                        dC = (pt->x - pt3->x) * (pt1->y- pt3->y) - (pt1->x - pt3->x) * (pt->y- pt3->y);
                        dD = -(dA * pt3->x + dB * pt3->y + dC *pt3->pContour->z);
                        newpt->pContour->z = (newpt->pContour->z -(dA * newpt->x + dB * newpt->y + dD) / dC)/2.0 ;

                        CTPointChain* newptchain = (CTPointChain*)calloc(1, sizeof(CTPointChain));
                        newptchain->pt = newpt;
                        if (pPtChain == NULL){
                                pPtChain = pPtEndChain = newptchain;
                        }
                        else{
                                pPtEndChain->next = newptchain;
                                pPtEndChain = pPtEndChain->next;
                        }

                        CTTriangle* newtri = (CTTriangle*)calloc(1, sizeof(CTTriangle));
                        CTTriangle* nnewtri = (CTTriangle*)calloc(1, sizeof(CTTriangle));
                        memcpy(newtri, tri, sizeof(CTTriangle));
                        memcpy(nnewtri, ntri, sizeof(CTTriangle));
                        newtri->id = lTTriangleCount; lTTriangleCount++;
                        nnewtri->id = lTTriangleCount; lTTriangleCount++;

                        tri->pt[triindex1] = newpt;
                        newtri->pt[triindex2] = newpt;

                        ntri->pt[ntriindex2] = newpt;
                        nnewtri->pt[ntriindex1] = newpt;

                        tri1 = tri->tri[triindex2];
                        if (tri1)							//is not null;
                                for (i=0; i<3; i++)
                                        if (tri1->tri[i] == tri){
                                                tri1->tri[i] = newtri;
                                                break;}
                                        newtri->tri[triindex2] = tri1;
                                        newtri->tri[triindex] = nnewtri;
                                        newtri->tri[triindex1] = tri;
                                        newtri->flag[triindex1] = false;
                                        tri->tri[triindex2] = newtri;
                                        tri->flag[triindex2] = false;

                                        ntri1 = ntri->tri[ntriindex1];
                                        if (ntri1)						//is not null.
                                                for (i=0; i<3; i++)
                                                        if (ntri1->tri[i] == ntri){
                                                                ntri1->tri[i] = nnewtri;
                                                                break;}
                                                        nnewtri->tri[ntriindex] = newtri;
                                                        nnewtri->tri[ntriindex2] = ntri;
                                                        nnewtri->tri[ntriindex1] = ntri1;
                                                        nnewtri->flag[ntriindex2] = false;
                                                        ntri->tri[ntriindex1] = nnewtri;
                                                        ntri->flag[ntriindex1] = false;

                                                        //add the two new triangle to the triangle chain.
                                                        CTTriangleChain* newchain = (CTTriangleChain*)calloc(1, sizeof(CTTriangleChain));
                                                        newchain->tri = newtri;
                                                        newtri->chain = newchain;
                                                        newchain->pre = pTriEndChain;
                                                        pTriEndChain->next = newchain;
                                                        pTriEndChain = pTriEndChain->next;

                                                        newchain = (CTTriangleChain*)calloc(1, sizeof(CTTriangleChain));
                                                        newchain->tri = nnewtri;
                                                        nnewtri->chain = newchain;
                                                        newchain->pre = pTriEndChain;
                                                        pTriEndChain->next = newchain;
                                                        pTriEndChain = pTriEndChain->next;
                }
                else{			//形成凸四边形，交换对角线；//特征边在triindex 的下一位，ntri

                        ntri->pt[ntriindex2] = pt3;
                        tri1 = tri->tri[triindex1];
                        ntri->tri[ntriindex] = tri1;
                        ntri1 = ntri->tri[ntriindex1];
                        ntri->tri[ntriindex1] = tri;
                        ntri->flag[ntriindex] = tri->flag[triindex1];

                        //tri1
                        if (tri1)
                                for(i=0; i<3; i++)
                                        if (tri1->tri[i] == tri){
                                                tri1->tri[i] = ntri;
                                                break;}

                                        if (ntri1)
                                                for(i=0; i<3; i++)
                                                        if (ntri1->tri[i] == ntri){
                                                                ntri1->tri[i] = tri;
                                                                break;}

                                                        tri->pt[triindex2] = pt;
                                                        tri->tri[triindex1] = ntri;
                                                        tri->tri[triindex] = ntri1;
                                                        tri->flag[triindex] = ntri->flag[ntriindex1];
                                                        ntri->flag[ntriindex1] = false;
                                                        tri->flag[triindex1] = false;
                }
        }
}

//process the point delaunay include constrainlines.
bool CTIN::Delaunay(CBlockPoint* pBlockPoint, CBlockTIN* pBlockTIN)
{
        triangleinit();

        quality = vararea = fixedarea = 0;
        dwyer = 1;
        splitseg = 0;
        docheck = 0;
        nobisect = 0;
        order = 1;

        poly = 1;
        convex = 1;
        edgesout = 1;
        neighbors = 1;
        useshelles = poly || quality || convex;

        //************************************************************************************
        //begin transfernodes(in->pointlist, in->pointmarkerlist, in->numberofpoints);
        //*************************************************************************************

        point pointloop;
        long  i;

        long lPointCount = pBlockPoint->lPointCount;
        CPoint3D* pPoints = pBlockPoint->pPoints;
        long lLineCount = pBlockPoint->lConstrainLineCount;
        CConstrainLine* pLines = pBlockPoint->pConstrainLines;

        inpoints = lPointCount;//numberofpoints;
        if (inpoints < 3) {
                printf("Error:  Input must have at least three input points.\n");
                exit(1);
        }

        initializepointpool();

        /* Read the points. */
        for (i = 0; i < inpoints; i++) {
                pointloop = (point) poolalloc(&points);
                /* Read the point coordinates. */
                pointloop[0] = pPoints[i].x;
                pointloop[1] = pPoints[i].y;
        }
        xmin = pBlockPoint->oBoundBox.xmin;
        xmax = pBlockPoint->oBoundBox.xmax;
        ymin = pBlockPoint->oBoundBox.ymin;
        ymax = pBlockPoint->oBoundBox.ymax;

        //************************************************************************************
        //end transfernodes(in->pointlist, in->pointmarkerlist, in->numberofpoints);
        //*************************************************************************************

        //hullsize = delaunay();                          /* Triangulate the points. */
        initializetrisegpools();
        //hullsize = divconqdelaunay();

        point *sortarray;
        struct CTriEdge hullleft, hullright;
        long  divider;

        /* Allocate an array of pointers to points for sorting. */
        sortarray = (point *) malloc(inpoints * sizeof(point));
        if (sortarray == (point *) NULL) {
                printf("Error:  Out of memory.\n");
                return false;
        }
        traversalinit(&points);
        for (i = 0; i < inpoints; i++) {
                sortarray[i] = pointtraverse();
        }
        /* Sort the points. */
        pointsort(sortarray, inpoints);
        i = inpoints;

        if (dwyer) {
                /* Re-sort the array of points to accommodate alternating cuts. */
                divider = i >> 1;
                if (i - divider >= 2) {
                        if (divider >= 2) {
                                alternateaxes(sortarray, divider, 1);
                        }
                        alternateaxes(&sortarray[divider], i - divider, 1);
                }
        }
        /* Form the Delaunay triangulation. */
        divconqrecurse(sortarray, i, 0, &hullleft, &hullright);
        free(sortarray);
		sortarray = NULL;

        hullsize = removeghosts(&hullleft);

        if (useshelles) {
                checksegments = 1;
                // Insert PSLG segments and/or convex hull segments.
                //insegments = formskeleton(in->segmentlist, in->segmentmarkerlist,	in->numberofsegments);
                long  index;
                point endpoint1, endpoint2;
                long  boundmarker;
                long  i;

                if (poly) {
                        //    if (!quiet) {      printf("Inserting segments into Delaunay triangulation.\n");    }
                        insegments = lLineCount;
                        index = 0;
                        // If segments are to be inserted, compute a mapping from points to triangles.
                        if (insegments > 0) {
                                //if (verbose) {        printf("  Inserting PSLG segments.\n");      }
                                makepointmap();
                        }

                        boundmarker = 0;
                        /* Read and insert the segments. */
                        for (i = 0; i < insegments; i++) {
                                endpoint1 = getpoint(pLines[i].frompt);
                                endpoint2 = getpoint(pLines[i].endpt);
                                insertsegment(endpoint1, endpoint2, boundmarker);
                        }

                        if (convex || !poly) {
                                // Enclose the convex hull with shell edges.
                                //if (verbose) {printf("  Enclosing convex hull with segments.\n");    }
                                markhull();
                        }
                }
        }


        //write nodes;
        long  pointnumber;
        traversalinit(&points);
        pointloop = pointtraverse();
        pointnumber = 0;
        while (pointloop != (point) NULL) {
                setpointmark(pointloop, pointnumber);
                pointloop = pointtraverse();
                pointnumber++;
        }

        //	writeelements;	//writeneighbors;
        long  neighbor1, neighbor2, neighbor3;
        triangle ptr;                         /* Temporary variable used by sym(). */
        struct CTriEdge triangleloop;
        point pt1, pt2, pt3;
        struct CTriEdge trisym;
        long  elementnumber;

        long lTriangleCount = triangles.items;
        CIOTriangle* pTriangles = (CIOTriangle *)calloc(lTriangleCount, sizeof(CIOTriangle));

        traversalinit(&triangles);
        triangleloop.tri = triangletraverse();
        triangleloop.orient = 0;
        elementnumber = 0;
        while (triangleloop.tri != (triangle *) NULL) {
                * (long *) (triangleloop.tri + 6) = elementnumber;
                triangleloop.tri = triangletraverse();
                elementnumber++;
        }
        * (long *) (dummytri + 6) = -1;

        traversalinit(&triangles);
        triangleloop.tri = triangletraverse();
        triangleloop.orient = 0;
        elementnumber = 0;
        while (triangleloop.tri != (triangle *) NULL) {
                //the three points.
                org(triangleloop, pt1);
                dest(triangleloop, pt2);
                apex(triangleloop, pt3);
                pTriangles[elementnumber].pt[0] = pointmark(pt1);
                pTriangles[elementnumber].pt[1] = pointmark(pt2);
                pTriangles[elementnumber].pt[2] = pointmark(pt3);

                //the nearby triangles.
                triangleloop.orient = 1;
                sym(triangleloop, trisym);
                neighbor1 = * (long *) (trisym.tri + 6);
                triangleloop.orient = 2;
                sym(triangleloop, trisym);
                neighbor2 = * (long *) (trisym.tri + 6);
                triangleloop.orient = 0;
                sym(triangleloop, trisym);
                neighbor3 = * (long *) (trisym.tri + 6);

                pTriangles[elementnumber].tri[0] = neighbor1;
                pTriangles[elementnumber].tri[1] = neighbor2;
                pTriangles[elementnumber].tri[2] = neighbor3;
                pTriangles[elementnumber].id = elementnumber;

                triangleloop.tri = triangletraverse();
                elementnumber++;
        }
        triangledeinit();

        pBlockTIN->lBlockID = 0;
        pBlockTIN->oBoundBox = pBlockPoint->oBoundBox;
        pBlockTIN->lPointCount = lPointCount;
        pBlockTIN->lTriangleCount = lTriangleCount;
        pBlockTIN->pPoints = pPoints;
        pBlockTIN->pTriangles = pTriangles;
        pBlockPoint->pPoints = NULL;
        return true;
}
//Check if a point lies to the left of a vector p1-p2.
//Cross product will be twice the area of the triangle constructed by
//the three vertices. This method returns the Cross Product of the
//vector p1-p2, p1-p3.
bool IsLeft(CTPoint* pt0, CTPoint* pt1, CTPoint* check)
{
        return (pt1->x - pt0->x) * (check->y - pt0->y) - (pt1->y - pt0->y) * (check->x - pt0->x) > 0;
}

bool IsLeft(CPoint3D* pt0, CPoint3D* pt1, CPoint3D* pt2)
{
        return (pt1->x - pt0->x) * (pt2->y - pt0->y) - (pt1->y - pt0->y) * (pt2->x - pt0->x) > 0;
}

bool IsLeft(CPoint3D* p1, CPoint3D* p2, double x, double y)
{
        return (p2->x - p1->x)*(y - p1->y) - (p2->y - p1->y)*(x - p1->x) > 0;
}

//判断第三点pt2
bool IsLeft(CPoint2D* pt0, CPoint2D* pt1, CPoint2D* pt2)
{
        return (pt1->x - pt0->x) * (pt2->y - pt0->y) - (pt1->y - pt0->y) * (pt2->x - pt0->x) > 0;
};
//判断第三点pt2
bool IsRight(CPoint2D* pt0, CPoint2D* pt1, CPoint2D* pt2)
{
        return (pt1->x - pt0->x) * (pt2->y - pt0->y) - (pt1->y - pt0->y) * (pt2->x - pt0->x) < 0;
};

#ifdef USENCC
}
#endif