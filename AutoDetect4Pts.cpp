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
	m_SrcGray=NULL;
	m_Edge=NULL;
}

CAutoDetect4Pts::~CAutoDetect4Pts()
{
	if(m_Src!=NULL)
	{
		cvReleaseImage(&m_Src);
		m_Src=NULL;
	}
	if(m_SrcGray!=NULL)
	{
		cvReleaseImage(&m_SrcGray);
		m_SrcGray=NULL;
	}
	if(m_Edge!=NULL)
	{
		cvReleaseImage(&m_Edge);
		m_Edge=NULL;
	}
	cvDestroyAllWindows();
}
int CAutoDetect4Pts::Detect()
{
	if (m_SrcGray == NULL)
		return 0;
// 	cvNamedWindow("src");
// 	cvNamedWindow("edge");

	cvZero(m_Edge);
	cvCanny(m_SrcGray, m_Edge, 230, 250);
	cvDilate(m_Edge, m_Edge);

	IplImage *eig_image = cvCreateImage( cvSize(m_SrcGray->width,m_SrcGray->height),
		IPL_DEPTH_32F, 1);
	IplImage *temp_image = cvCreateImage( cvSize(m_SrcGray->width,m_SrcGray->height),
		IPL_DEPTH_32F, 1);

	int numcorners = 500;
	cvGoodFeaturesToTrack(m_SrcGray,eig_image,temp_image, m_pCorner, &numcorners, 0.1, 50);


	m_Num = 0;
	if (ConnectdCorner(m_Edge, m_pCorner, numcorners))
	{
		m_Num = numcorners;
		double len = 0, max_len = 0;
		int flag[4] = {0,1,2,3};
		int i, j;
		for (i = 0; i < numcorners; i ++)
		{
			for (j = i+1; j < numcorners; j ++)
			{
				len = (m_pCorner[i].x-m_pCorner[j].x)*(m_pCorner[i].x-m_pCorner[j].x) +
					(m_pCorner[i].y-m_pCorner[j].y)*(m_pCorner[i].y-m_pCorner[j].y);
				if (len > max_len)
				{
					flag[0] = i;
					flag[2] = j;
					max_len = len;
 				}
			}
		}

		switch (flag[0])
		{
		case 0:
			switch (flag[2])
			{
			case 1:
				flag[1] = 2; flag[3] = 3;
				break;
			case 2:
				flag[1] = 1; flag[3] = 3;
				break;
			case 3:
				flag[1] = 1; flag[3] = 2;
				break;				
			}
			break;
		case 1:
			switch (flag[2])
			{
			case 2:
				flag[1] = 0; flag[3] = 3;
				break;
			case 3:
				flag[1] = 0; flag[3] = 2;
				break;
			}
			break;
		case 2:
			switch (flag[2])
			{
			case 3:
				flag[1] = 0; flag[3] = 1;
				break;
			}
			break;
		}

		for (i = 0; i < m_Num; i ++)
		{
			m_AutoPt[i] = cvPoint((int)m_pCorner[flag[i]].x, (int)m_pCorner[flag[i]].y);
		}
	}

// 	for (int i = 0; i < numcorners; i ++)
// 	{
// 		if (m_Edge->imageData[int(m_pCorner[i].y)* m_Edge->widthStep + int(m_pCorner[i]. x)] != 0 )
// 		{
// 			cvCircle(m_Src, cvPoint(m_pCorner[i].x, m_pCorner[i].y), 3, cvScalar(0, 0, 255));
// 		}
// 	}
// 	cvShowImage("edge", m_Edge);
// 	cvShowImage("src", m_Src);
// 	cvWaitKey(0);
// 	
	cvReleaseImage(&eig_image);
	cvReleaseImage(&temp_image);

	return m_Num;
}

bool CAutoDetect4Pts::ConnectdCorner(IplImage *edge, CvPoint2D32f *pCorner, int &n)
{
	CvPoint2D32f *tmpcorner = new CvPoint2D32f[n];
	int *pflag = new int[n];
	memset(pflag, 0, sizeof(int)*n);
	int m = 0;
	for (int i  = 0; i < n; i ++)//挨个判断
	{
		if (pflag[i] != 0)//之前判断过，换下一个
			continue;

		m = 0;
		pflag [i] = 1;
		tmpcorner[m] = pCorner[i];
		cvFloodFill(m_Edge, cvPoint(pCorner[i].x, pCorner[i].y), cvScalar(100,100,100));
		for (int j  = i+1; j < n; j ++)
		{
			if (edge->imageData[int(pCorner[j].y)* m_Edge->widthStep + int(pCorner[j]. x)] == 100)
			{
				m ++;
				pflag[j] = 1;
				tmpcorner[m] = pCorner[j];
			}
		}
		if (m == 3)
			break;
		cvFloodFill(m_Edge, cvPoint(pCorner[i].x, pCorner[i].y), cvScalar(255,255,255));

	}
	if (m == 3)
	{
		for (int i  = 0; i < 4; i ++)
		{
			pCorner[i] = tmpcorner[i];
		}
		delete []tmpcorner;
		n = 4;
		return true;
	}
	else
	{
		delete []tmpcorner;
		return false;
	}
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
		IPL_DEPTH_8U, 3);
	m_SrcGray = cvCreateImage( cvSize(inputImage->width,inputImage->height),
		IPL_DEPTH_8U, 1);
	m_Edge = cvCreateImage( cvSize(inputImage->width,inputImage->height),
		IPL_DEPTH_8U, 1);

	cvCvtColor( inputImage, m_SrcGray, CV_BGR2GRAY);
	cvCopy( inputImage, m_Src);
	return true;
}

