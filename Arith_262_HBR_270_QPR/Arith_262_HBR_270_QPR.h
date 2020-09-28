#ifndef _ARITH_262_HBR_H_
#define _ARITH_262_HBR_H_
#include "IArith.h"
#include "GDef.h"
#include "GXList.h"
#include "GData.h"
#include "StandardRadarDataX.h"
#include "Product_262_HBR.h"
#include "Product_270_QPR.h"
#include "IBase.h"

struct buffer
{
    void*		p;
    gUint32	size;
    gUint32	w;
    gUint32	h;
    gUint32	reso;

    buffer()
    {
        memset(this, 0x00, sizeof(buffer));
        reso = 1;
    }
    ~buffer()
    {
        free (p);
        memset(this, 0x00, sizeof(buffer));
    }

    gInt32 create(gInt32 w, gInt32 h, gUint32 reso)
    {
        p = new gInt8[w * h * reso * sizeof(gInt8)];

        size = w * h * reso * sizeof(gInt8);

        buffer::w = w;
        buffer::h = h;
        buffer::reso = reso;

        return w*h*reso;
    };

    void setReso(gUint32 r)
    {
        if(r > size) reso = 1;
        else reso = r;
    };

    template<typename T>
    T*	loc(gUint32 col, gUint32 row)
    {
        if((row + col * w)*sizeof(T) > size) return NULL;

        return (T*) ( (T*)p + row*w + col );
    };
    void* loc(gUint32 col, gUint32 row, gUint32 r)
    {
        if(r * row + col * w * r > size) return NULL;

        return  (void*)( (gUint8*)p + r * row + col * w * r );
    };

    gUint32 resize(gUint32 _w, gUint32 _h, gUint32 _reso)
    {
        if(_w * _h * _reso > size)
        {
            p = realloc(p, _w * _h * _reso);
        }

        w = _w;
        h = _w;
        reso = _reso;

        size = w * h * reso;

        return size;
    };

    void destroy()
    {
        if(p) free (p);

        memset(this, 0x00, sizeof(buffer));
    };

};

struct RADIALREF
{
    gUint16	AzimuthAngle;         
    gInt16	DeltaAngel;	
    gInt16  ref[800];
};

typedef struct tagQPRData
{
    gInt16 packet_code;
    gInt16 row_num;
    gInt16 col_num;
    gInt16 row_res;
    gInt16 col_res;
    gInt16 QPR;
}QPRDATA;

typedef struct tagHBRData
{
    gInt16 packet_code;
    gInt16 elevation;
    gUint16 azimuth;
    gInt16 HBR;
}HBRDATA;

typedef struct tagGridData
{
    gUint32 dist_x;
    gUint32 dist_y;
    short   data;
}GRID_DATA;


class Arith_262_HBR_270_QPR : public IArith
{
 public:
  Arith_262_HBR_270_QPR();
  ~Arith_262_HBR_270_QPR();

  SYSMETHOD(Initialize)();
  SYSMETHOD(LoadData)(void* pdata, ReadParam * ReadParameter);
  SYSMETHOD(OutPutData)(void* pdata);
  SYSMETHOD(Execute());
  SYSMETHOD(UnInitialize());

 private:
  GXList<GenerationData >* m_pInputList;
  ReadParam * m_Parameter;
  GXList<GenerationData>* m_pOutputList;
  gInt32 m_bins;
  buffer	m_workBuf;

  //gInt32 DoQRate( gInt32 nVCPElevTiltCount,		// number of VCP tilts
  /*
    gInt32 DoQRate( gInt32 nRay,			//number of radials 
    gInt32 iRefGateLen,		// length of ref gate in meters 
    gInt32 *nRefGateCount,		//array of ref gates number of VCP tilts 
    gInt16  ***parData,		// reflectivity data arrays
    gUint16 **ppAzTiltAngle,		// azimuth angles of VCP tilts
    gInt16 *pTiltElev,		// elevation angles of VCP tilts
    QPRDATA*** pOutputQPR,
    gUint32* OutputQPRSize,
    gUint32* OutputQPRRow,
    gUint32* OutputQPRCol,
    void*** pOutputHBR,
    gUint32* OutputHBRSize,
    gUint32* OutGateCount
    );
  */

    gInt32 DoQRate( gInt32 nRay,			//number of radials 
        gInt32 iRefGateLen,		// length of ref gate in meters 
        gInt32 *nRefGateCount,		//array of ref gates number of VCP tilts 
        /*gInt16  ***parData,*/  // reflectivity data arrays
     StandardRadarData* pstdRData,
		gUint16 **ppAzTiltAngle,		// azimuth angles of VCP tilts
        gInt16 *pTiltElev,		// elevation angles of VCP tilts
        QPRDATA*** pOutputQPR,
        gUint32* OutputQPRSize,
        gUint32* OutputQPRRow,
        gUint32* OutputQPRCol,
        void*** pOutputHBR,
        gUint32* OutputHBRSize,
        gUint32* OutGateCount
        );

    void DoIsolatedCheck(RADIALREF **href, 
        gInt32 nRay,
        gInt32 qr_gates );

    gInt32  IsIsolated( RADIALREF **href,
        gInt32 tilt, 
        gInt32 azLeft,
        gInt32 az, 
        gInt32 azRight,
        gInt32 bin,
        gInt16 gate_zmin);

    void DoOutlierCheck( RADIALREF **href, 
        gInt32 nRay,
        gInt32 qr_gates );

    gInt32  IsSpuriousNoise(RADIALREF **href,
        gInt32 tilt, 
        gInt32 azLeft,
        gInt32 az, 
        gInt32 azRight,
        gInt32 bin,
        gInt16 gate_zmax) ;

    void RemoveNoise(RADIALREF **href,
        gInt32 nRay,
        gInt32 tilt,
        gInt32 az,
        gInt32 Bin);

    void BuildHybridScan( RADIALREF **href,
        RADIALREF *qr_dbz,
        gInt32 nRay,
        gInt32 qr_gates,
        gInt32 qr_gatelen);

    gInt32  ComputeLowestElevLayerArea( RADIALREF **href, 
        gInt32 nRay);

    void DoMedianPolarBuf( RADIALREF *qr_dbz, 
        gInt32 RadialNums, 
        gInt32 Bins) ;

    gInt16 DoMedianWindow( gInt16*Windowbuf, 
        gInt32  nsize);

    bool SaveHBR(RADIALREF *qr_dbz, 
        gInt32 nRay, 
        gInt32 qr_gates, 
        gInt32 qr_gatelen,
        void** pOutput,
        gUint32* OutputSize);

    bool SaveQPR(RADIALREF *qr_dbz, 
        gInt32 nRay, 
        gInt32 qr_gates, 
        gInt32 qr_gatelen,
        QPRDATA***  pOutput,
        gUint32* OutputSize,
        gUint32* OutputRow,
        gUint32* OutputCol);

    GHRESULT ConvertRadialToGrid(GRID_DATA** GridBuf,
        gInt32 w, 
        gInt32 h,
        RADIALREF *qr_dbz,
        gInt32 nRay,
        gInt32 gateSize,
        gInt32 R1, gInt32 R2,
        gInt32 rx, gInt32 ry,
        double beam_width );

    void Smooth(gInt16**Ingest, 
        gInt32 w, 
        gInt32 h, 
        gInt16 InitValue);

    //GHRESULT CalcGridRes(GRID_DATA** pGridData,
    //    gInt32 w, 
    //    gInt32 h);

    GHRESULT SaveQPRToProductData(StandardRadarData* pstdRadarData,
        GRADARDATAHEADER* pRaderHeader,
        QPRDATA** pInput,
        gUint32 inputSize,
        gUint32 row,
        gUint32 col);

    GHRESULT SaveHBRToProductData(StandardRadarData* pstdRadarData,
        GRADARDATAHEADER* pRaderHeader,
        void** pInput,
        gUint32 inputSize,
        gUint32 nRayCnt,
        gUint32 gateCnt);

};

#endif
