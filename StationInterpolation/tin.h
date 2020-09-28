#ifndef TIN2_H
#define TIN2_H

#include "dem.h"

//#define USENCC
#ifdef USENCC
namespace NCCGIS{
#endif

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define Is_Flat_Tri(tri) ((fabs(tri->pt[0]->pContour->z - tri->pt[1]->pContour->z) < ZERO)&&(fabs(tri->pt[0]->pContour->z - tri->pt[2]->pContour->z) < ZERO))
#define MinZ(tri) (min(tri->pt[0]->z, min(tri->pt[1]->z, tri->pt[2]->z)))
#define MaxZ(tri) (max(tri->pt[0]->z, max(tri->pt[1]->z, tri->pt[2]->z)))
struct CTContour;
struct CTPoint
{
        long id;
        double x,y;
        CTContour *pContour;
        CTPoint()
        {
                x = y = 0;
                pContour = NULL;
        };
        CTPoint(double xx, double yy)
        {
                x = xx;
                y = yy;
                pContour = NULL;
        };
        bool operator==(CTPoint pt)
        {
                if ((fabs(x - pt.x) < ZERO)&&(fabs(y - pt.y) < ZERO))
                        return true;
                else
                        return false;
        }
};

struct CTContour
{
        long ptnum;				//
        double z;				//
        CTPoint *pts;			//
        CTContour()
        {
                pts = NULL;
        }
};

struct CTPointChain
{
        CTPoint *pt;
        struct CTPointChain *next;
        CTPointChain()
        {
                pt   = NULL;
                next = NULL;
        };
};

struct CTTriangle
{					//the delaunay triangle struct.
        long id;
        CTTriangle *tri[3];				//the three triangles which border the triangle
        CTPoint *pt[3];
        bool flag[3];					//flag the edge is the constrained line.
        struct CTTriangleChain *chain;
        CTTriangle()
        {
                id = -1;
                flag[0] = flag[1] = flag[2] = false;
        };
};
struct CTTriangleChain
{					//
        CTTriangle *tri;
        struct CTTriangleChain *pre;		//
        struct CTTriangleChain *next;		//
        CTTriangleChain()
        {
                tri  = NULL;
                pre  = NULL;
                next = NULL;
        };
};

// the chain of the points
struct CPointChain
{
        CPoint3D *pt;
        struct CPointChain *next;
        CPointChain()
        {
                pt   = NULL;
                next = NULL;
        };
        CPointChain(CPoint3D *ppt)
        {
                pt   = ppt;
                next = NULL;
        };
};


// The chain of the triangles
struct CTriangleChain
{
        BYTE flag;
        CTriangle *tri;
        struct CTriangleChain *next;
        CTriangleChain()
        {
                flag = 0;
                tri  = NULL;
                next = NULL;
        };
        CTriangleChain(CTriangle* ptri)
        {
                flag = 0;
                tri  = ptri;
                next = NULL;
        };
};
/*****************************************************************************/
/* For efficiency, a variety of data structures are allocated in bulk.  The  */
/*   following constants determine how many of each structure is allocated   */
/*   at once.                                                                */
#define TRIPERBLOCK 4092           /* Number of triangles allocated at once. */
#define SHELLEPERBLOCK 508       /* Number of shell edges allocated at once. */
#define POINTPERBLOCK 4092            /* Number of points allocated at once. */
#define VIRUSPERBLOCK 1020   /* Number of virus triangles allocated at once. */
/* Number of encroached segments allocated at once. */
#define BADSEGMENTPERBLOCK 252
/* Number of skinny triangles allocated at once. */
#define BADTRIPERBLOCK 4092
/* Number of splay tree nodes allocated at once. */
#define SPLAYNODEPERBLOCK 508

/* The point marker DEADPOINT is an arbitrary number chosen large enough to  */
/*   (hopefully) not conflict with user boundary markers.  Make sure that it */
/*   is small enough to fit into your machine's integer size.                */

#define DEADPOINT -1073741824

/* The next line is used to outsmart some very stupid compilers.  If your    */
/*   compiler is smarter, feel free to replace the "int" with "void".        */
/*   Not that it matters.                                                    */

//#define VOID int

/* Two constants for algorithms based on random sampling.  Both constants    */
/*   have been chosen empirically to optimize their respective algorithms.   */

/* Used for the point location scheme of Mucke, Saias, and Zhu, to decide    */
/*   how large a random sample of triangles to inspect.                      */
#define SAMPLEFACTOR 11
/* Used in Fortune's sweepline Delaunay algorithm to determine what fraction */
/*   of boundary edges should be maintained in the splay tree for point      */
/*   location on the front.                                                  */
#define SAMPLERATE 10

/* A number that speaks for itself, every kissable digit.                    */

//#define PI 3.141592653589793238462643383279502884197169399375105820974944592308

/* Another fave.                                                             */

#define SQUAREROOTTWO 1.4142135623730950488016887242096980785696718753769480732

/* And here's one for those of you who are intimidated by math.              */

#define ONETHIRD 0.333333333333333333333333333333333333333333333333333333333333


/********* Primitives for triangles                                  *********/
/*                                                                           */
/*                                                                           */

/* decode() converts a pointer to an oriented triangle.  The orientation is  */
/*   extracted from the two least significant bits of the pointer.           */

#define decode(ptr, triedge)                                                  \
  (triedge).orient = (long) ((unsigned long) (ptr) & (unsigned long) 3l);      \
  (triedge).tri = (triangle *)                                                \
                  ((unsigned long) (ptr) ^ (unsigned long) (triedge).orient)

/* encode() compresses an oriented triangle into a single pointer.  It       */
/*   relies on the assumption that all triangles are aligned to four-byte    */
/*   boundaries, so the two least significant bits of (triedge).tri are zero.*/

#define encode(triedge)                                                       \
  (triangle) ((unsigned long) (triedge).tri | (unsigned long) (triedge).orient)

/* The following edge manipulation primitives are all described by Guibas    */
/*   and Stolfi.  However, they use an edge-based data structure, whereas I  */
/*   am using a triangle-based data structure.                               */

/* sym() finds the abutting triangle, on the same edge.  Note that the       */
/*   edge direction is necessarily reversed, because triangle/edge handles   */
/*   are always directed counterclockwise around the triangle.               */

#define sym(triedge1, triedge2)                                               \
  ptr = (triedge1).tri[(triedge1).orient];                                    \
  decode(ptr, triedge2);

#define symself(triedge)                                                      \
  ptr = (triedge).tri[(triedge).orient];                                      \
  decode(ptr, triedge);

/* lnext() finds the next edge (counterclockwise) of a triangle.             */

#define lnext(triedge1, triedge2)                                             \
  (triedge2).tri = (triedge1).tri;                                            \
  (triedge2).orient = plus1mod3[(triedge1).orient]

#define lnextself(triedge)                                                    \
  (triedge).orient = plus1mod3[(triedge).orient]

/* lprev() finds the previous edge (clockwise) of a triangle.                */

#define lprev(triedge1, triedge2)                                             \
  (triedge2).tri = (triedge1).tri;                                            \
  (triedge2).orient = minus1mod3[(triedge1).orient]

#define lprevself(triedge)                                                    \
  (triedge).orient = minus1mod3[(triedge).orient]

/* onext() spins counterclockwise around a point; that is, it finds the next */
/*   edge with the same origin in the counterclockwise direction.  This edge */
/*   will be part of a different triangle.                                   */

#define onext(triedge1, triedge2)                                             \
  lprev(triedge1, triedge2);                                                  \
  symself(triedge2);

#define onextself(triedge)                                                    \
  lprevself(triedge);                                                         \
  symself(triedge);

/* oprev() spins clockwise around a point; that is, it finds the next edge   */
/*   with the same origin in the clockwise direction.  This edge will be     */
/*   part of a different triangle.                                           */

#define oprev(triedge1, triedge2)                                             \
  sym(triedge1, triedge2);                                                    \
  lnextself(triedge2);

#define oprevself(triedge)                                                    \
  symself(triedge);                                                           \
  lnextself(triedge);

/* dnext() spins counterclockwise around a point; that is, it finds the next */
/*   edge with the same destination in the counterclockwise direction.  This */
/*   edge will be part of a different triangle.                              */

#define dnext(triedge1, triedge2)                                             \
  sym(triedge1, triedge2);                                                    \
  lprevself(triedge2);

#define dnextself(triedge)                                                    \
  symself(triedge);                                                           \
  lprevself(triedge);

/* dprev() spins clockwise around a point; that is, it finds the next edge   */
/*   with the same destination in the clockwise direction.  This edge will   */
/*   be part of a different triangle.                                        */

#define dprev(triedge1, triedge2)                                             \
  lnext(triedge1, triedge2);                                                  \
  symself(triedge2);

#define dprevself(triedge)                                                    \
  lnextself(triedge);                                                         \
  symself(triedge);

/* rnext() moves one edge counterclockwise about the adjacent triangle.      */
/*   (It's best understood by reading Guibas and Stolfi.  It involves        */
/*   changing triangles twice.)                                              */

#define rnext(triedge1, triedge2)                                             \
  sym(triedge1, triedge2);                                                    \
  lnextself(triedge2);                                                        \
  symself(triedge2);

#define rnextself(triedge)                                                    \
  symself(triedge);                                                           \
  lnextself(triedge);                                                         \
  symself(triedge);

/* rnext() moves one edge clockwise about the adjacent triangle.             */
/*   (It's best understood by reading Guibas and Stolfi.  It involves        */
/*   changing triangles twice.)                                              */

#define rprev(triedge1, triedge2)                                             \
  sym(triedge1, triedge2);                                                    \
  lprevself(triedge2);                                                        \
  symself(triedge2);

#define rprevself(triedge)                                                    \
  symself(triedge);                                                           \
  lprevself(triedge);                                                         \
  symself(triedge);

/* These primitives determine or set the origin, destination, or apex of a   */
/* triangle.                                                                 */

#define org(triedge, pointptr)                                                \
  pointptr = (point) (triedge).tri[plus1mod3[(triedge).orient] + 3]

#define dest(triedge, pointptr)                                               \
  pointptr = (point) (triedge).tri[minus1mod3[(triedge).orient] + 3]

#define apex(triedge, pointptr)                                               \
  pointptr = (point) (triedge).tri[(triedge).orient + 3]

#define setorg(triedge, pointptr)                                             \
  (triedge).tri[plus1mod3[(triedge).orient] + 3] = (triangle) pointptr

#define setdest(triedge, pointptr)                                            \
  (triedge).tri[minus1mod3[(triedge).orient] + 3] = (triangle) pointptr

#define setapex(triedge, pointptr)                                            \
  (triedge).tri[(triedge).orient + 3] = (triangle) pointptr

#define setvertices2null(triedge)                                             \
  (triedge).tri[3] = (triangle) NULL;                                         \
  (triedge).tri[4] = (triangle) NULL;                                         \
  (triedge).tri[5] = (triangle) NULL;

/* Bond two triangles together.                                              */

#define bond(triedge1, triedge2)                                              \
  (triedge1).tri[(triedge1).orient] = encode(triedge2);                       \
  (triedge2).tri[(triedge2).orient] = encode(triedge1)

/* Dissolve a bond (from one side).  Note that the other triangle will still */
/*   think it's connected to this triangle.  Usually, however, the other     */
/*   triangle is being deleted entirely, or bonded to another triangle, so   */
/*   it doesn't matter.                                                      */

#define dissolve(triedge)                                                     \
  (triedge).tri[(triedge).orient] = (triangle) dummytri

/* Copy a triangle/edge handle.                                              */

#define triedgecopy(triedge1, triedge2)                                       \
  (triedge2).tri = (triedge1).tri;                                            \
  (triedge2).orient = (triedge1).orient

/* Test for equality of triangle/edge handles.                               */

#define triedgeequal(triedge1, triedge2)                                      \
  (((triedge1).tri == (triedge2).tri) &&                                      \
   ((triedge1).orient == (triedge2).orient))

/* Primitives to infect or cure a triangle with the virus.  These rely on    */
/*   the assumption that all shell edges are aligned to four-byte boundaries.*/

#define infect(triedge)                                                       \
  (triedge).tri[6] = (triangle)                                               \
                     ((unsigned long) (triedge).tri[6] | (unsigned long) 2l)

#define uninfect(triedge)                                                     \
  (triedge).tri[6] = (triangle)                                               \
                     ((unsigned long) (triedge).tri[6] & ~ (unsigned long) 2l)

/* Test a triangle for viral infection.                                      */

#define infected(triedge)                                                     \
  (((unsigned long) (triedge).tri[6] & (unsigned long) 2l) != 0)

/* Check or set a triangle's attributes.                                     */

#define elemattribute(triedge, attnum)                                        \
  ((double *) (triedge).tri)[elemattribindex + (attnum)]

#define setelemattribute(triedge, attnum, value)                              \
  ((double *) (triedge).tri)[elemattribindex + (attnum)] = value

/* Check or set a triangle's maximum area bound.                             */

#define areabound(triedge)  ((double *) (triedge).tri)[areaboundindex]

#define setareabound(triedge, value)                                          \
  ((double *) (triedge).tri)[areaboundindex] = value

/********* Primitives for shell edges                                *********/
/*                                                                           */
/*                                                                           */

/* sdecode() converts a pointer to an oriented shell edge.  The orientation  */
/*   is extracted from the least significant bit of the pointer.  The two    */
/*   least significant bits (one for orientation, one for viral infection)   */
/*   are masked out to produce the real pointer.                             */

#define sdecode(sptr, edge)                                                   \
  (edge).shorient = (long) ((unsigned long) (sptr) & (unsigned long) 1l);      \
  (edge).sh = (shelle *)                                                      \
              ((unsigned long) (sptr) & ~ (unsigned long) 3l)

/* sencode() compresses an oriented shell edge into a single pointer.  It    */
/*   relies on the assumption that all shell edges are aligned to two-byte   */
/*   boundaries, so the least significant bit of (edge).sh is zero.          */

#define sencode(edge)                                                         \
  (shelle) ((unsigned long) (edge).sh | (unsigned long) (edge).shorient)

/* ssym() toggles the orientation of a shell edge.                           */

#define ssym(edge1, edge2)                                                    \
  (edge2).sh = (edge1).sh;                                                    \
  (edge2).shorient = 1 - (edge1).shorient

#define ssymself(edge)                                                        \
  (edge).shorient = 1 - (edge).shorient

/* spivot() finds the other shell edge (from the same segment) that shares   */
/*   the same origin.                                                        */

#define spivot(edge1, edge2)                                                  \
  sptr = (edge1).sh[(edge1).shorient];                                        \
  sdecode(sptr, edge2)

#define spivotself(edge)                                                      \
  sptr = (edge).sh[(edge).shorient];                                          \
  sdecode(sptr, edge)

/* snext() finds the next shell edge (from the same segment) in sequence;    */
/*   one whose origin is the input shell edge's destination.                 */

#define snext(edge1, edge2)                                                   \
  sptr = (edge1).sh[1 - (edge1).shorient];                                    \
  sdecode(sptr, edge2)

#define snextself(edge)                                                       \
  sptr = (edge).sh[1 - (edge).shorient];                                      \
  sdecode(sptr, edge)

/* These primitives determine or set the origin or destination of a shell    */
/*   edge.                                                                   */

#define sorg(edge, pointptr)                                                  \
  pointptr = (point) (edge).sh[2 + (edge).shorient]

#define sdest(edge, pointptr)                                                 \
  pointptr = (point) (edge).sh[3 - (edge).shorient]

#define setsorg(edge, pointptr)                                               \
  (edge).sh[2 + (edge).shorient] = (shelle) pointptr

#define setsdest(edge, pointptr)                                              \
  (edge).sh[3 - (edge).shorient] = (shelle) pointptr

/* These primitives read or set a shell marker.  Shell markers are used to   */
/*   hold user boundary information.                                         */

#define mark(edge)  (* (long *) ((edge).sh + 6))

#define setmark(edge, value)                                                  \
  * (long *) ((edge).sh + 6) = value

/* Bond two shell edges together.                                            */

#define sbond(edge1, edge2)                                                   \
  (edge1).sh[(edge1).shorient] = sencode(edge2);                              \
  (edge2).sh[(edge2).shorient] = sencode(edge1)

/* Dissolve a shell edge bond (from one side).  Note that the other shell    */
/*   edge will still think it's connected to this shell edge.                */

#define sdissolve(edge)                                                       \
  (edge).sh[(edge).shorient] = (shelle) dummysh

/* Copy a shell edge.                                                        */

#define shellecopy(edge1, edge2)                                              \
  (edge2).sh = (edge1).sh;                                                    \
  (edge2).shorient = (edge1).shorient

/* Test for equality of shell edges.                                         */

#define shelleequal(edge1, edge2)                                             \
  (((edge1).sh == (edge2).sh) &&                                              \
   ((edge1).shorient == (edge2).shorient))

/********* Primitives for interacting triangles and shell edges      *********/
/*                                                                           */
/*                                                                           */

/* tspivot() finds a shell edge abutting a triangle.                         */

#define tspivot(triedge, edge)                                                \
  sptr = (shelle) (triedge).tri[6 + (triedge).orient];                        \
  sdecode(sptr, edge)

/* stpivot() finds a triangle abutting a shell edge.  It requires that the   */
/*   variable `ptr' of type `triangle' be defined.                           */

#define stpivot(edge, triedge)                                                \
  ptr = (triangle) (edge).sh[4 + (edge).shorient];                            \
  decode(ptr, triedge)

/* Bond a triangle to a shell edge.                                          */

#define tsbond(triedge, edge)                                                 \
  (triedge).tri[6 + (triedge).orient] = (triangle) sencode(edge);             \
  (edge).sh[4 + (edge).shorient] = (shelle) encode(triedge)

/* Dissolve a bond (from the triangle side).                                 */

#define tsdissolve(triedge)                                                   \
  (triedge).tri[6 + (triedge).orient] = (triangle) dummysh

/* Dissolve a bond (from the shell edge side).                               */

#define stdissolve(edge)                                                      \
  (edge).sh[4 + (edge).shorient] = (shelle) dummytri

/********* Primitives for points                                     *********/
/*                                                                           */
/*                                                                           */

#define pointmark(pt)  ((long *) (pt))[pointmarkindex]

#define setpointmark(pt, value)                                               \
  ((long *) (pt))[pointmarkindex] = value

#define point2tri(pt)  ((triangle *) (pt))[point2triindex]

#define setpoint2tri(pt, value)                                               \
  ((triangle *) (pt))[point2triindex] = value

/**                                                                         **/
/**                                                                         **/
/********* Mesh manipulation primitives end here                     *********/


/* Which of the following two methods of finding the absolute values is      */
/*   fastest is compiler-dependent.  A few compilers can inline and optimize */
/*   the fabs() call; but most will incur the overhead of a function call,   */
/*   which is disastrously slow.  A faster way on IEEE machines might be to  */
/*   mask the appropriate bit, but that's difficult to do in C.              */

#define Absolute(a)  ((a) >= 0.0 ? (a) : -(a))
/* #define Absolute(a)  fabs(a) */

/* Many of the operations are broken up into two pieces, a main part that    */
/*   performs an approximate operation, and a "tail" that computes the       */
/*   roundoff error of that operation.                                       */
/*                                                                           */
/* The operations Fast_Two_Sum(), Fast_Two_Diff(), Two_Sum(), Two_Diff(),    */
/*   Split(), and Two_Product() are all implemented as described in the      */
/*   reference.  Each of these macros requires certain variables to be       */
/*   defined in the calling routine.  The variables `bvirt', `c', `abig',    */
/*   `_i', `_j', `_k', `_l', `_m', and `_n' are declared `INEXACT' because   */
/*   they store the result of an operation that may incur roundoff error.    */
/*   The input parameter `x' (or the highest numbered `x_' parameter) must   */
/*   also be declared `INEXACT'.                                             */

#define Fast_Two_Sum_Tail(a, b, x, y) \
  bvirt = x - a; \
  y = b - bvirt

#define Fast_Two_Sum(a, b, x, y) \
  x = (double) (a + b); \
  Fast_Two_Sum_Tail(a, b, x, y)

#define Two_Sum_Tail(a, b, x, y) \
  bvirt = (double) (x - a); \
  avirt = x - bvirt; \
  bround = b - bvirt; \
  around = a - avirt; \
  y = around + bround

#define Two_Sum(a, b, x, y) \
  x = (double) (a + b); \
  Two_Sum_Tail(a, b, x, y)

#define Two_Diff_Tail(a, b, x, y) \
  bvirt = (double) (a - x); \
  avirt = x + bvirt; \
  bround = bvirt - b; \
  around = a - avirt; \
  y = around + bround

#define Two_Diff(a, b, x, y) \
  x = (double) (a - b); \
  Two_Diff_Tail(a, b, x, y)



#define Split(a, ahi, alo) \
  c = (double) (splitter * a); \
  abig = (double) (c - a); \
  ahi = c - abig; \
  alo = a - ahi

#define Two_Product_Tail(a, b, x, y) \
  Split(a, ahi, alo); \
  Split(b, bhi, blo); \
  err1 = x - (ahi * bhi); \
  err2 = err1 - (alo * bhi); \
  err3 = err2 - (ahi * blo); \
  y = (alo * blo) - err3

#define Two_Product(a, b, x, y) \
  x = (double) (a * b); \
  Two_Product_Tail(a, b, x, y)

/* Two_Product_Presplit() is Two_Product() where one of the inputs has       */
/*   already been split.  Avoids redundant splitting.                        */

#define Two_Product_Presplit(a, b, bhi, blo, x, y) \
  x = (double) (a * b); \
  Split(a, ahi, alo); \
  err1 = x - (ahi * bhi); \
  err2 = err1 - (alo * bhi); \
  err3 = err2 - (ahi * blo); \
  y = (alo * blo) - err3

/* Square() can be done more quickly than Two_Product().                     */

#define Square_Tail(a, x, y) \
  Split(a, ahi, alo); \
  err1 = x - (ahi * ahi); \
  err3 = err1 - ((ahi + ahi) * alo); \
  y = (alo * alo) - err3

#define Square(a, x, y) \
  x = (double) (a * a); \
  Square_Tail(a, x, y)

/* Macros for summing expansions of various fixed lengths.  These are all    */
/*   unrolled versions of Expansion_Sum().                                   */

#define Two_One_Sum(a1, a0, b, x2, x1, x0) \
  Two_Sum(a0, b , _i, x0); \
  Two_Sum(a1, _i, x2, x1)

#define Two_One_Diff(a1, a0, b, x2, x1, x0) \
  Two_Diff(a0, b , _i, x0); \
  Two_Sum( a1, _i, x2, x1)

#define Two_Two_Sum(a1, a0, b1, b0, x3, x2, x1, x0) \
  Two_One_Sum(a1, a0, b0, _j, _0, x0); \
  Two_One_Sum(_j, _0, b1, x3, x2, x1)

#define Two_Two_Diff(a1, a0, b1, b0, x3, x2, x1, x0) \
  Two_One_Diff(a1, a0, b0, _j, _0, x0); \
  Two_One_Diff(_j, _0, b1, x3, x2, x1)


/* Labels that signify whether a record consists primarily of pointers or of */
/*   floating-point words.  Used to make decisions about data alignment.     */
enum wordtype {POINTER, FLOATINGPOINT};

/* Labels that signify the result of point location.  The result of a        */
/*   search indicates that the point falls in the interior of a triangle, on */
/*   an edge, on a vertex, or outside the mesh.                              */
enum locateresult {INTRIANGLE, ONEDGE, ONVERTEX, OUTSIDE};

/* Labels that signify the result of site insertion.  The result indicates   */
/*   that the point was inserted with complete success, was inserted but     */
/*   encroaches on a segment, was not inserted because it lies on a segment, */
/*   or was not inserted because another point occupies the same location.   */
enum insertsiteresult {SUCCESSFULPOINT, ENCROACHINGPOINT, VIOLATINGPOINT,
                       DUPLICATEPOINT};

/* Labels that signify the result of direction finding.  The result          */
/*   indicates that a segment connecting the two query points falls within   */
/*   the direction triangle, along the left edge of the direction triangle,  */
/*   or along the right edge of the direction triangle.                      */
enum finddirectionresult {WITHIN, LEFTCOLLINEAR, RIGHTCOLLINEAR};

/* Labels that signify the result of the circumcenter computation routine.   */
/*   The return value indicates which edge of the triangle is shortest.      */
enum circumcenterresult {OPPOSITEORG, OPPOSITEDEST, OPPOSITEAPEX};

/*****************************************************************************/
/*                                                                           */
/*  The basic mesh data structures                                           */
/*                                                                           */
/*  There are three:  points, triangles, and shell edges (abbreviated        */
/*  `shelle').  These three data structures, linked by pointers, comprise    */
/*  the mesh.  A point simply represents a point in space and its properties.*/
/*  A triangle is a triangle.  A shell edge is a special data structure used */
/*  to represent impenetrable segments in the mesh (including the outer      */
/*  boundary, boundaries of holes, and internal boundaries separating two    */
/*  triangulated regions).  Shell edges represent boundaries defined by the  */
/*  user that triangles may not lie across.                                  */
/*                                                                           */
/*  A triangle consists of a list of three vertices, a list of three         */
/*  adjoining triangles, a list of three adjoining shell edges (when shell   */
/*  edges are used), an arbitrary number of optional user-defined floating-  */
/*  point attributes, and an optional area constraint.  The latter is an     */
/*  upper bound on the permissible area of each triangle in a region, used   */
/*  for mesh refinement.                                                     */
/*                                                                           */
/*  For a triangle on a boundary of the mesh, some or all of the neighboring */
/*  triangles may not be present.  For a triangle in the interior of the     */
/*  mesh, often no neighboring shell edges are present.  Such absent         */
/*  triangles and shell edges are never represented by NULL pointers; they   */
/*  are represented by two special records:  `dummytri', the triangle that   */
/*  fills "outer space", and `dummysh', the omnipresent shell edge.          */
/*  `dummytri' and `dummysh' are used for several reasons; for instance,     */
/*  they can be dereferenced and their contents examined without causing the */
/*  memory protection exception that would occur if NULL were dereferenced.  */
/*                                                                           */
/*  However, it is important to understand that a triangle includes other    */
/*  information as well.  The pointers to adjoining vertices, triangles, and */
/*  shell edges are ordered in a way that indicates their geometric relation */
/*  to each other.  Furthermore, each of these pointers contains orientation */
/*  information.  Each pointer to an adjoining triangle indicates which face */
/*  of that triangle is contacted.  Similarly, each pointer to an adjoining  */
/*  shell edge indicates which side of that shell edge is contacted, and how */
/*  the shell edge is oriented relative to the triangle.                     */
/*                                                                           */
/*  Shell edges are found abutting edges of triangles; either sandwiched     */
/*  between two triangles, or resting against one triangle on an exterior    */
/*  boundary or hole boundary.                                               */
/*                                                                           */
/*  A shell edge consists of a list of two vertices, a list of two           */
/*  adjoining shell edges, and a list of two adjoining triangles.  One of    */
/*  the two adjoining triangles may not be present (though there should      */
/*  always be one), and neighboring shell edges might not be present.        */
/*  Shell edges also store a user-defined integer "boundary marker".         */
/*  Typically, this integer is used to indicate what sort of boundary        */
/*  conditions are to be applied at that location in a finite element        */
/*  simulation.                                                              */
/*                                                                           */
/*  Like triangles, shell edges maintain information about the relative      */
/*  orientation of neighboring objects.                                      */
/*                                                                           */
/*  Points are relatively simple.  A point is a list of floating point       */
/*  numbers, starting with the x, and y coordinates, followed by an          */
/*  arbitrary number of optional user-defined floating-point attributes,     */
/*  followed by an integer boundary marker.  During the segment insertion    */
/*  phase, there is also a pointer from each point to a triangle that may    */
/*  contain it.  Each pointer is not always correct, but when one is, it     */
/*  speeds up segment insertion.  These pointers are assigned values once    */
/*  at the beginning of the segment insertion phase, and are not used or     */
/*  updated at any other time.  Edge swapping during segment insertion will  */
/*  render some of them incorrect.  Hence, don't rely upon them for          */
/*  anything.  For the most part, points do not have any information about   */
/*  what triangles or shell edges they are linked to.                        */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/*  Handles                                                                  */
/*                                                                           */
/*  The oriented triangle (`triedge') and oriented shell edge (`edge') data  */
/*  structures defined below do not themselves store any part of the mesh.   */
/*  The mesh itself is made of `triangle's, `shelle's, and `point's.         */
/*                                                                           */
/*  Oriented triangles and oriented shell edges will usually be referred to  */
/*  as "handles".  A handle is essentially a pointer into the mesh; it       */
/*  allows you to "hold" one particular part of the mesh.  Handles are used  */
/*  to specify the regions in which one is traversing and modifying the mesh.*/
/*  A single `triangle' may be held by many handles, or none at all.  (The   */
/*  latter case is not a memory leak, because the triangle is still          */
/*  connected to other triangles in the mesh.)                               */
/*                                                                           */
/*  A `triedge' is a handle that holds a triangle.  It holds a specific side */
/*  of the triangle.  An `edge' is a handle that holds a shell edge.  It     */
/*  holds either the left or right side of the edge.                         */
/*                                                                           */
/*  Navigation about the mesh is accomplished through a set of mesh          */
/*  manipulation primitives, further below.  Many of these primitives take   */
/*  a handle and produce a new handle that holds the mesh near the first     */
/*  handle.  Other primitives take two handles and glue the corresponding    */
/*  parts of the mesh together.  The exact position of the handles is        */
/*  important.  For instance, when two triangles are glued together by the   */
/*  bond() primitive, they are glued by the sides on which the handles lie.  */
/*                                                                           */
/*  Because points have no information about which triangles they are        */
/*  attached to, I commonly represent a point by use of a handle whose       */
/*  origin is the point.  A single handle can simultaneously represent a     */
/*  triangle, an edge, and a point.                                          */
/*                                                                           */
/*****************************************************************************/

/* The triangle data structure.  Each triangle contains three pointers to    */
/*   adjoining triangles, plus three pointers to vertex points, plus three   */
/*   pointers to shell edges (defined below; these pointers are usually      */
/*   `dummysh').  It may or may not also contain user-defined attributes     */
/*   and/or a floating-point "area constraint".  It may also contain extra   */
/*   pointers for nodes, when the user asks for high-order elements.         */
/*   Because the size and structure of a `triangle' is not decided until     */
/*   runtime, I haven't simply defined the type `triangle' to be a struct.   */
typedef double **triangle;			/* Really:  typedef triangle *triangle   */

/* An oriented triangle:  includes a pointer to a triangle and orientation.  */
/*   The orientation denotes an edge of the triangle.  Hence, there are      */
/*   three possible orientations.  By convention, each edge is always        */
/*   directed to point counterclockwise about the corresponding triangle.    */
struct CTriEdge {

  triangle *tri;
  long orient;                                         /* Ranges from 0 to 2. */
};

/* The shell data structure.  Each shell edge contains two pointers to       */
/*   adjoining shell edges, plus two pointers to vertex points, plus two     */
/*   pointers to adjoining triangles, plus one shell marker.                 */
typedef double **shelle;                  /* Really:  typedef shelle *shelle   */

/* An oriented shell edge:  includes a pointer to a shell edge and an        */
/*   orientation.  The orientation denotes a side of the edge.  Hence, there */
/*   are two possible orientations.  By convention, the edge is always       */
/*   directed so that the "side" denoted is the right side of the edge.      */
struct CEdge {
  shelle *sh;
  long shorient;                                       /* Ranges from 0 to 1. */
};

/* The point data structure.  Each point is actually an array of REALs.      */
/*   The number of REALs is unknown until runtime.  An integer boundary      */
/*   marker, and sometimes a pointer to a triangle, is appended after the    */
/*   REALs.                                                                  */
typedef double *point;


/* A type used to allocate memory.  firstblock is the first block of items.  */
/*   nowblock is the block from which items are currently being allocated.   */
/*   nextitem points to the next slab of free memory for an item.            */
/*   deaditemstack is the head of a linked list (stack) of deallocated items */
/*   that can be recycled.  unallocateditems is the number of items that     */
/*   remain to be allocated from nowblock.                                   */
/*                                                                           */
/* Traversal is the process of walking through the entire list of items, and */
/*   is separate from allocation.  Note that a traversal will visit items on */
/*   the "deaditemstack" stack as well as live items.  pathblock points to   */
/*   the block currently being traversed.  pathitem points to the next item  */
/*   to be traversed.  pathitemsleft is the number of items that remain to   */
/*   be traversed in pathblock.                                              */
/*                                                                           */
/* itemwordtype is set to POINTER or FLOATINGPOINT, and is used to suggest   */
/*   what sort of word the record is primarily made up of.  alignbytes       */
/*   determines how new records should be aligned in memory.  itembytes and  */
/*   itemwords are the length of a record in bytes (after rounding up) and   */
/*   words.  itemsperblock is the number of items allocated at once in a     */
/*   single block.  items is the number of currently allocated items.        */
/*   maxitems is the maximum number of items that have been allocated at     */
/*   once; it is the current number of items plus the number of records kept */
/*   on deaditemstack.                                                       */
struct CMemoryPool {
  long **firstblock, **nowblock;
  long *nextitem;
  long *deaditemstack;
  long **pathblock;
  long *pathitem;
  enum wordtype itemwordtype;
  long alignbytes;
  long itembytes, itemwords;
  long itemsperblock;
  long items, maxitems;
  long unallocateditems;
  long pathitemsleft;
};

  //Check if a point lies to the left of a vector p1-p2.
//Cross product will be twice the area of the triangle constructed by
//the three vertices. This method returns the Cross Product of the
//vector p1-p2, p1-p3.
bool IsLeft(CTPoint* pt0, CTPoint* pt1, CTPoint* check);
bool IsLeft(CPoint3D* pt0, CPoint3D* pt1, CPoint3D* pt2);
bool IsLeft(CPoint3D* p1, CPoint3D* p2, double x, double y);
bool IsLeft(CPoint2D* pt0, CPoint2D* pt1, CPoint2D* pt2);
bool IsRight(CPoint2D* pt0, CPoint2D* pt1, CPoint2D* pt2);


class CTIN
{
private:
    struct CMemoryPool triangles;
    struct CMemoryPool shelles;
    struct CMemoryPool points;
    struct CMemoryPool viri;
    struct CMemoryPool badsegments;
    struct CMemoryPool badtriangles;
    struct CMemoryPool splaynodes;
    struct badface *queuefront[64];
    struct badface **queuetail[64];
    double xmin, xmax, ymin, ymax;                              /* x and y bounds. */
    double xminextreme;        /* Nonexistent x value used as a flag in sweepline. */
    long inpoints;                                     /* Number of input points. */
    long inelements;                                /* Number of input triangles. */
    long insegments;                                 /* Number of input segments. */
    long holes;                                         /* Number of input holes. */
    long regions;                                     /* Number of input regions. */
    long edges;                                       /* Number of output edges. */
    long hullsize;                            /* Number of edges of convex hull. */
    long triwords;                                   /* Total words per triangle. */
    long shwords;                                  /* Total words per shell edge. */
    long pointmarkindex;             /* Index to find boundary marker of a point. */
    long point2triindex;         /* Index to find a triangle adjacent to a point. */
    long highorderindex;    /* Index to find extra nodes for high-order elements. */
    long elemattribindex;              /* Index to find attributes of a triangle. */
    long areaboundindex;               /* Index to find area bound of a triangle. */
    long checksegments;           /* Are there segments in the triangulation yet? */
    long samples;                /* Number of random samples for point location. */
    unsigned long randomseed;                     /* Current random number seed. */
    double splitter;       /* Used to split REAL factors for exact multiplication. */
    double epsilon;                             /* Floating-point machine epsilon. */
    double resulterrbound;
    double ccwerrboundA, ccwerrboundB, ccwerrboundC;
    double iccerrboundA, iccerrboundB, iccerrboundC;
    long incirclecount;                   /* Number of incircle tests performed. */
    long counterclockcount;       /* Number of counterclockwise tests performed. */
    long hyperbolacount;        /* Number of right-of-hyperbola tests performed. */
    long circumcentercount;    /* Number of circumcenter calculations performed. */
    long circletopcount;         /* Number of circle top calculations performed. */
    long poly, quality, vararea, fixedarea, convex;
    long edgesout, neighbors;
    long dwyer;
    long splitseg;
    long docheck;

    long useshelles;
    long order;
    long nobisect;
    double minangle, goodangle;
    double maxarea;
    point infpoint1, infpoint2, infpoint3;

    triangle *dummytri;
            triangle *dummytribase;      /* Keep base address so we can free() it later. */

            /* Pointer to the omnipresent shell edge.  Referenced by any triangle or     */
            /*   shell edge that isn't really connected to a shell edge at that          */
            /*   location.                                                               */
            shelle *dummysh;
            shelle *dummyshbase;         /* Keep base address so we can free() it later. */

            /* Pointer to a recently visited triangle.  Improves point location if       */
            /*   proximate points are inserted sequentially.                             */
            struct CTriEdge recenttri;

            /*****************************************************************************/
            /*                                                                           */
            /*  Mesh manipulation primitives.  Each triangle contains three pointers to  */
            /*  other triangles, with orientations.  Each pointer points not to the      */
            /*  first byte of a triangle, but to one of the first three bytes of a       */
            /*  triangle.  It is necessary to extract both the triangle itself and the   */
            /*  orientation.  To save memory, I keep both pieces of information in one   */
            /*  pointer.  To make this possible, I assume that all triangles are aligned */
            /*  to four-byte boundaries.  The `decode' routine below decodes a pointer,  */
            /*  extracting an orientation (in the range 0 to 2) and a pointer to the     */
            /*  beginning of a triangle.  The `encode' routine compresses a pointer to a */
            /*  triangle and an orientation into a single pointer.  My assumptions that  */
            /*  triangles are four-byte-aligned and that the `unsigned long' type is     */
            /*  long enough to hold a pointer are two of the few kludges in this program.*/
            /*                                                                           */
            /*  Shell edges are manipulated similarly.  A pointer to a shell edge        */
            /*  carries both an address and an orientation in the range 0 to 1.          */
            /*                                                                           */
            /*  The other primitives take an oriented triangle or oriented shell edge,   */
            /*  and return an oriented triangle or oriented shell edge or point; or they */
            /*  change the connections in the data structure.                            */
            /*                                                                           */
            /*****************************************************************************/


            /* Fast lookup arrays to speed some of the mesh manipulation primitives.     */

            long plus1mod3[3];
            long minus1mod3[3];

    private:

            /*****************************************************************************/
            /*                                                                           */
            /*  poolinit()   Initialize a pool of memory for allocation of items.        */
            /*****************************************************************************/
            void poolinit(struct CMemoryPool *pool, long bytecount, long itemcount, enum wordtype wtype, long alignment);

            /*****************************************************************************/
            /*                                                                           */
            /*  poolrestart()   Deallocate all items in a pool.                          */
            /*****************************************************************************/
            void poolrestart(struct CMemoryPool *pool);

            /*****************************************************************************/
            /*                                                                           */
            /*  pooldeinit()   Free to the operating system all memory taken by a pool.  */
            /*****************************************************************************/
            void pooldeinit(struct CMemoryPool *pool);

            /*****************************************************************************/
            /*                                                                           */
            /*  poolalloc()   Allocate space for an item.                                */
            /*****************************************************************************/
            long *poolalloc(struct CMemoryPool *pool);

            /*****************************************************************************/
            /*                                                                           */
            /*  pooldealloc()   Deallocate space for an item.                            */
            /*****************************************************************************/
            void pooldealloc(struct CMemoryPool *pool, long *dyingitem);

            /*****************************************************************************/
            /*                                                                           */
            /*  traversalinit()   Prepare to traverse the entire list of items.          */
            /*****************************************************************************/
            void traversalinit(struct CMemoryPool *pool);

            /*****************************************************************************/
            /*                                                                           */
            /*  traverse()   Find the next item in the list.                             */
            /*****************************************************************************/
            long *traverse(struct CMemoryPool *pool);

            /*****************************************************************************/
            /*                                                                           */
            /*  dummyinit()   Initialize the triangle that fills "outer space" and the   */
            /*                omnipresent shell edge.                                    */
            /*****************************************************************************/
            void dummyinit(long trianglewords, long shellewords);

            /*****************************************************************************/
            /*                                                                           */
            /*  initializepointpool()   Calculate the size of the point data structure   */
            /*                          and initialize its memory pool.                  */
            /*****************************************************************************/
            void initializepointpool();

            /*****************************************************************************/
            /*                                                                           */
            /*  initializetrisegpools()   Calculate the sizes of the triangle and shell  */
            /*                            edge data structures and initialize their      */
            /*                            memory pools.                                  */
            /*****************************************************************************/
            void initializetrisegpools();

            /*****************************************************************************/
            /*                                                                           */
            /*  triangledealloc()   Deallocate space for a triangle, marking it dead.    */
            /*****************************************************************************/
            void triangledealloc(triangle *dyingtriangle);

            /*****************************************************************************/
            /*                                                                           */

            /*  triangletraverse()   Traverse the triangles, skipping dead ones.         */
            /*****************************************************************************/
            triangle *triangletraverse();

            /*****************************************************************************/
            /*                                                                           */
            /*  shelledealloc()   Deallocate space for a shell edge, marking it dead.    */
            /*****************************************************************************/
            void shelledealloc(shelle *dyingshelle);

            /*****************************************************************************/
            /*                                                                           */
            /*  shelletraverse()   Traverse the shell edges, skipping dead ones.         */
            /*****************************************************************************/
            shelle *shelletraverse();

            /*****************************************************************************/
            /*                                                                           */
            /*  pointdealloc()   Deallocate space for a point, marking it dead.          */
            /*****************************************************************************/
            void pointdealloc(point dyingpoint);

            /*****************************************************************************/
            /*                                                                           */
            /*  pointtraverse()   Traverse the points, skipping dead ones.               */
            /*****************************************************************************/
            point pointtraverse();

            /*****************************************************************************/
            /*                                                                           */
            /*  getpoint()   Get a specific point, by number, from the list.             */
            /*****************************************************************************/
            point getpoint(long number);

            /*****************************************************************************/
            /*                                                                           */
            /*  triangledeinit()   Free all remaining allocated memory.                  */
            /*****************************************************************************/
            void triangledeinit();

            /*****************************************************************************/
            /*                                                                           */
            /*  maketriangle()   Create a new triangle with orientation zero.            */
            /*****************************************************************************/
            void maketriangle(struct CTriEdge *newtriedge);

            /*****************************************************************************/
            /*                                                                           */
            /*  makeshelle()   Create a new shell edge with orientation zero.            */
            /*****************************************************************************/
            void makeshelle(struct CEdge *newedge);

            /*****************************************************************************/
            /*                                                                           */
            /*  exactinit()   Initialize the variables used for exact arithmetic.        */
            /*****************************************************************************/
            void exactinit();

            /*****************************************************************************/
            /*                                                                           */
            /*  fast_expansion_sum_zeroelim()   Sum two expansions, eliminating zero     */
            /*                                  components from the output expansion.    */
            /*****************************************************************************/
            long fast_expansion_sum_zeroelim(long elen, double *e, long flen, double *f, double *h);

            /*****************************************************************************/
            /*                                                                           */
            /*  scale_expansion_zeroelim()   Multiply an expansion by a scalar,          */
            /*                               eliminating zero components from the        */
            /*                               output expansion.                           */
            /*****************************************************************************/
            long scale_expansion_zeroelim(long elen, double *e, double b, double *h);

            /*****************************************************************************/
            /*                                                                           */
            /*  estimate()   Produce a one-word estimate of an expansion's value.        */
            /*****************************************************************************/
            double estimate(long elen, double *e);

            /*****************************************************************************/
            /*                                                                           */
            /*  counterclockwise()   Return a positive value if the points pa, pb, and   */
            /*                       pc occur in counterclockwise order; a negative      */
            /*                       value if they occur in clockwise order; and zero    */
            /*                       if they are collinear.  The result is also a rough  */
            /*                       approximation of twice the signed area of the       */
            /*                       triangle defined by the three points.               */
            /*****************************************************************************/
            double counterclockwiseadapt(point pa, point pb, point pc, double detsum);
            double counterclockwise(point pa, point pb, point pc);

            /*****************************************************************************/
            /*                                                                           */
            /*  incircle()   Return a positive value if the point pd lies inside the     */
            /*               circle passing through pa, pb, and pc; a negative value if  */
            /*               it lies outside; and zero if the four points are cocircular.*/
            /*               The points pa, pb, and pc must be in counterclockwise       */
            /*               order, or the sign of the result will be reversed.          */
            /*****************************************************************************/
            double incircleadapt(point pa, point pb, point pc, point pd, double permanent);
            double incircle(point pa, point pb, point pc, point pd);

            /*****************************************************************************/
            /*                                                                           */
            /*  triangleinit()   Initialize some variables.                              */
            /*****************************************************************************/
            void triangleinit();

            /*****************************************************************************/
            /*                                                                           */
            /*  randomnation()   Generate a random number between 0 and `choices' - 1.   */
            /*****************************************************************************/
            unsigned long randomnation(unsigned long choices);

            /*****************************************************************************/
            /*                                                                           */
            /*  makepointmap()   Construct a mapping from points to triangles to improve  */
            /*                  the speed of point location for segment insertion.       */
            /*****************************************************************************/
            void makepointmap();

            /*****************************************************************************/
            /*                                                                           */
            /*  preciselocate()   Find a triangle or edge containing a given point.      */
            /*****************************************************************************/
            enum locateresult preciselocate(point searchpoint, struct CTriEdge *searchtri);

            /*****************************************************************************/
            /*                                                                           */
            /*  locate()   Find a triangle or edge containing a given point.             */
            /*****************************************************************************/
            enum locateresult locate(point searchpoint,struct CTriEdge * searchtri);

            /*****************************************************************************/
            /*                                                                           */
            /*  insertshelle()   Create a new shell edge and insert it between two       */
            /*                   triangles.                                              */
            /*****************************************************************************/
            void insertshelle(struct CTriEdge *tri, long shellemark);

            /*****************************************************************************/
            /*                                                                           */
            /*  flip()   Transform two triangles to two different triangles by flipping  */
            /*           an edge within a quadrilateral.                                 */
            /*****************************************************************************/
            void flip(struct CTriEdge *flipedge);

            /*****************************************************************************/
            /*                                                                           */
            /*  insertsite()   Insert a vertex into a Delaunay triangulation,            */
            /*                 performing flips as necessary to maintain the Delaunay    */
            /*                 property.                                                 */
            /*****************************************************************************/
            enum insertsiteresult insertsite(point insertpoint, struct CTriEdge *searchtri,
                            struct CEdge *splitedge, long segmentflaws, long triflaws);

            /*****************************************************************************/
            /*                                                                           */
            /*  triangulatepolygon()   Find the Delaunay triangulation of a polygon that */
            /*                         has a certain "nice" shape.  This includes the    */
            /*                         polygons that result from deletion of a point or  */
            /*                         insertion of a segment.                           */
            /*****************************************************************************/

            void triangulatepolygon(struct CTriEdge *firstedge, struct CTriEdge *lastedge,
                            long edgecount, long doflip, long triflaws);

            /*****************************************************************************/
            /*                                                                           */
            /*  pointsort()   Sort an array of points by x-coordinate, using the         */
            /*                y-coordinate as a secondary key.                           */
            /*****************************************************************************/
            void pointsort(point *sortarray, long arraysize);

            /*****************************************************************************/
            /*                                                                           */
            /*  pointmedian()   An order statistic algorithm, almost.  Shuffles an array */
            /*                  of points so that the first `median' points occur        */
            /*                  lexicographically before the remaining points.           */
            /*****************************************************************************/
            void pointmedian(point *sortarray, long arraysize, long median, long axis);

            /*****************************************************************************/
            /*                                                                           */
            /*  alternateaxes()   Sorts the points as appropriate for the divide-and-    */
            /*                    conquer algorithm with alternating cuts.               */
            /*****************************************************************************/
            void alternateaxes(point *sortarray, long arraysize, long axis);

            /*****************************************************************************/
            /*                                                                           */
            /*  mergehulls()   Merge two adjacent Delaunay triangulations into a         */
            /*                 single Delaunay triangulation.                            */
            /*****************************************************************************/
            void mergehulls(struct CTriEdge *farleft, struct CTriEdge *innerleft,
                            struct CTriEdge *innerright, struct CTriEdge *farright, long axis);

            /*****************************************************************************/
            /*                                                                           */
            /*  divconqrecurse()   Recursively form a Delaunay triangulation by the      */
            /*                     divide-and-conquer method.                            */
            /*****************************************************************************/
            void divconqrecurse(point *sortarray, long vertices, long axis, struct CTriEdge *farleft,
                            struct CTriEdge *farright);

            long removeghosts(struct CTriEdge *startghost);

            /*****************************************************************************/
            /*                                                                           */
            /*  divconqdelaunay()   Form a Delaunay triangulation by the divide-and-     */
            /*                      conquer method.                                      */
            /*****************************************************************************/
            long divconqdelaunay();

            /*****************************************************************************/
            /*                                                                           */
            /*  finddirection()   Find the first triangle on the path from one point     */
            /*                    to another.                                            */
            /*****************************************************************************/
            enum finddirectionresult finddirection(struct CTriEdge *searchtri, point endpoint);

            /*****************************************************************************/
            /*                                                                           */
            /*  segmentintersection()   Find the intersection of an existing segment     */
            /*                          and a segment that is being inserted.  Insert    */
            /*                          a point at the intersection, splitting an        */
            /*                          existing shell edge.                             */
            /*****************************************************************************/
            void segmentintersection(struct CTriEdge *splittri, struct CEdge *splitshelle, point endpoint2);

            /*****************************************************************************/
            /*                                                                           */
            /*  scoutsegment()   Scout the first triangle on the path from one endpoint  */
            /*                   to another, and check for completion (reaching the      */
            /*                   second endpoint), a collinear point, and the            */
            /*                   intersection of two segments.                           */
            /*****************************************************************************/
            long scoutsegment(struct CTriEdge *searchtri, point endpoint2, long newmark);

            /*****************************************************************************/
            /*                                                                           */
            /*  delaunayfixup()   Enforce the Delaunay condition at an edge, fanning out */
            /*                    recursively from an existing point.  Pay special       */
            /*                    attention to stacking inverted triangles.              */
            /*****************************************************************************/
            void delaunayfixup(struct CTriEdge *fixuptri, long leftside);

            /*****************************************************************************/
            /*                                                                           */
            /*  constrainededge()   Force a segment into a constrained Delaunay          */
            /*                      triangulation by deleting the triangles it           */
            /*                      intersects, and triangulating the polygons that      */
            /*                      form on each side of it.                             */
            /*****************************************************************************/
            void constrainededge(struct CTriEdge *starttri, point endpoint2, long newmark);

            /*****************************************************************************/
            /*                                                                           */
            /*  insertsegment()   Insert a PSLG segment into a triangulation.            */
            /*****************************************************************************/
            void insertsegment(point endpoint1, point endpoint2, long newmark);

            /*****************************************************************************/
            /*                                                                           */
            /*  markhull()   Cover the convex hull of a triangulation with shell edges.  */
            /*****************************************************************************/
            void markhull();

    public:

            CTIN();
            virtual ~CTIN();
            //void SavePtTINFile(char* filename);
            /*****************************************************************************/
            /*                                                                           */
            /*  main() or triangulate()   main process.                           */
            /*****************************************************************************/
            bool Delaunay();
            /*bool Delaunay(CBlockPoint* pBlockPoint)
      {
                    lBlockID = pBlockPoint->lBlockID;
                    oBoundBox = pBlockPoint->oBoundBox;
                    lPointCount = pBlockPoint->lPointCount;
                    pPoints = pBlockPoint->pPoints;
                    return Delaunay();
            };

      */


            /*****************************************************************************/
            /*                                                                           */
            /*  set_triangle_flags()   set the triangle flags, indicate the triangle's	 */
            /*						   edge is constrained edge or not.		             */
            /*****************************************************************************/
            void set_triangle_flags(CTTriangle* tri);

            /*****************************************************************************/
            /*                                                                           */
            /*  process_inner_flat_polygon()   process the inner flat triangles.         */
            /*****************************************************************************/

            void process_inner_flat_polygon(CTTriangleChain *flat_poly);
            /*****************************************************************************/
            /*                                                                           */
            /*  process_flat_tri()   process the flat triangles.			             */
            /*****************************************************************************/
            void process_flat_tri();

            /*****************************************************************************/
            /*                                                                           */
            /*  construct_flat_polygon()   construct flat polygon			             */
            /*****************************************************************************/
            void construct_flat_polygon(CTTriangle* flat_tri, CTTriangleChain** flat_polygon);

    public:
            long lBlockID;					//块ID;

            CBoundBox oBoundBox;			//范围；

    //用于由点生成TIN的变量；
            CPoint3D* pPoints;				//输入的点；
            long lPointCount;				//输入的点的个数；

            long lTriangleCount;			//三角形的数目；
            CTriangle* pTriangles;			//三角形数组；

            //放入链表中的点，三角形；
            CTriangleChain* pTriangleChain;
            CPointChain* pPointChain;

            //点、三角形的起始编号；
            long lBaseTriangleNum;
            long lBasePointNum;

    //用于由等高线生成TIN的变量；
    protected:
            CTTriangle* pTTriangles;		//初始构建的三角形数组；
            long lTTriangleNum;				//初始构建三角形的数目；
            CTPoint* pTPoints;				//点的指针，包括点、等高线上的点；
            long lTPointNum;				//第一次输入的点的个数；

            long lInPointCount;				//原始输入点个数；
            CPoint3D* pInPoints;			//输入点；
            long lInLineCount;				//原始输入约束线个数；
            CConstrainLine* pInLines;		//原始输入约束线；
            long lContourCount;				//等高线数；
            CTContour* pContours;			//等高线;

            CTTriangleChain* pTriEndChain;	//三角形链表的尾指针；
            CTPointChain* pPtEndChain;		//点链表的尾指针；

    public:
            CTPointChain* pPtChain;			//点的链表；
            long lTPointCount;				//点数，包括点、等高线上的点；

            CTTriangleChain* pTriChain;		//三角形链表的头指针；
            long lTTriangleCount;			//三角形数；



    //对外接口
    public:
            //process the point delaunay include constrainlines.
            bool Delaunay(CBlockPoint* pBlockPoint, CBlockTIN* pBlockTIN);
            //用于从等高线构建TIN
            bool Delaunay(CBlockContour* pBlockContour, CBlockTIN* pBlockTIN);
};

#ifdef USENCC
}
#endif

#endif // TIN_H