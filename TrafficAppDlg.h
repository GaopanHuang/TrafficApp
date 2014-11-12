// TrafficAppDlg.h : header file
//

#if !defined(AFX_TRAFFICAPPDLG_H__9EF92E30_2BEC_419F_8C45_8C4B6EBA6235__INCLUDED_)
#define AFX_TRAFFICAPPDLG_H__9EF92E30_2BEC_419F_8C45_8C4B6EBA6235__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AutoDetect4Pts.h"
#include "PtsDlg.h"
#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
#include <vector>
using namespace std;

#define DISP_IMG_WIDTH 780
#define DISP_IMG_HEIGHT 550
#define DISP_SHIFT_X 0//该值不为零会出现显示像素与图像像素不对应的问题
#define DISP_SHIFT_Y 0//该值不为零会出现显示像素与图像像素不对应的问题
#define NUM_LEAST_PTS 4
#define ZOOM_IN_WIDTH 62
#define ZOOM_IN_HEIGHT 56
#define ZOOM_IN_TIMES 3
#define DISP_RECT_SHIFT_X 815
#define DISP_RECT_SHIFT_Y 20

#define LengthCalibrationBlock 25//标定方块边长，用于自动标定，手动标定通过手动输入完成
/////////////////////////////////////////////////////////////////////////////
// CTrafficAppDlg dialog

enum LINE_TYPE {straight_line, curve_line};
typedef struct pt_corr
{
	double xs; double ys; double s;
	double xi; double yi; double i;
 }PT_CORR;

class CTrafficAppDlg : public CDialog
{
// Construction
public:
	CTrafficAppDlg(CWnd* pParent = NULL);	// standard constructor
	~CTrafficAppDlg();

// Dialog Data
	//{{AFX_DATA(CTrafficAppDlg)
	enum { IDD = IDD_TRAFFICAPP_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrafficAppDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTrafficAppDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAbout();
	afx_msg void OnRadioLine1();
	afx_msg void OnRadioLine2();
	afx_msg void OnButtonRemove();
	afx_msg void OnButtonSave();
	afx_msg void OnFileOpen();
	afx_msg BOOL OnButtonCalibration();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


public:
	int GetImgRect(IplImage *pSrcImg, IplImage *pImgRect, CPoint pt);
	void ShowImgRect(IplImage *srcImg, int width, int height );
	void ShowImg(IplImage *srcImg, int width, int height );
	void init(CString filename);
	void SaveParaMat();
	void CamputeCameraPara();
	
	int AutoDetectCalibrationPt(IplImage *m_InputImg, CvPoint *Pt);

	IplImage *m_InputImg;
	IplImage *m_SrcImg;
	IplImage *m_DstImg;
	IplImage *m_CurImg;
	IplImage *m_ShowImg;
	IplImage *m_ZoomInImg;
	LINE_TYPE m_LineType;
	BOOL m_IsCalibration;
	BOOL m_IsLBDown;

	BITMAPINFO  m_Bmi;

	unsigned int m_ShowImgWidth;
	unsigned int m_ShowImgHeight;
	double m_RatioX;
	double m_RatioY;

	double m_MRect[9];
	unsigned int m_NumPts;
	vector <PT_CORR> m_ptsCalibrate;
	double m_PtDown[3];
	double m_PtUp[3];
	CvPoint m_PtDownOri;
	CvPoint m_PtUpOri;

	CvPoint m_AutoPtimage[4];

	double m_CurveLength;

	CvFont m_Font;
	CString m_FilePath;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRAFFICAPPDLG_H__9EF92E30_2BEC_419F_8C45_8C4B6EBA6235__INCLUDED_)
