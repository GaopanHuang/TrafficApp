// AutoDetect4Pts.cpp: implementation of the CAutoDetect4Pts class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TrafficApp.h"
#include "AutoDetect4Pts.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAutoDetect4Pts::CAutoDetect4Pts()
{
	m_Src=NULL;
	cvNamedWindow("test");
}

CAutoDetect4Pts::~CAutoDetect4Pts()
{
	if(m_Src!=NULL)
	{
		cvReleaseImage(&m_Src);
		m_Src=NULL;
	}
}
int CAutoDetect4Pts::Detect()
{
	int num = 0;

	return num;
}

int CAutoDetect4Pts::GetPts(CvPoint *pt)
{
	for (int i = 0; i < m_Num; i ++)
		pt[i] = m_AutoPt[i];

	return i;
}


bool CAutoDetect4Pts::SetImage(IplImage *inputImage)
{
	if (inputImage == NULL)
		return false;

	m_Src = cvCreateImage( cvSize(inputImage->width,inputImage->height),
		IPL_DEPTH_8U, inputImage->nChannels );

	cvCopy( inputImage, m_Src);
	return true;
}

