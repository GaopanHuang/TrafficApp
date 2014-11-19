// AutoDetect4Pts.h: interface for the CAutoDetect4Pts class.
//
//////////////////////////////////////////////////////////////////////

#if !defined __AUTODETECT4PTS_H__
#define __AUTODETECT4PTS_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
#include <vector>
using namespace std;

class CAutoDetect4Pts  
{
public:
	CAutoDetect4Pts();
	virtual ~CAutoDetect4Pts();
	bool SetImage(IplImage *inputImage);
	int Detect();
	int GetPts(CvPoint *pt);
	bool ConnectdCorner(IplImage *edge, CvPoint2D32f *pCorner, int &n);


public:
	IplImage *m_Src;
	IplImage *m_SrcGray;
	IplImage *m_Edge;
	CvPoint2D32f m_pCorner[1000];
	int m_Num;
	CvPoint m_AutoPt[4];


};

#endif // !defined(AFX_AUTODETECT4PTS_H__F2E92C45_6412_4E9D_AFCA_9C2042788F93__INCLUDED_)
