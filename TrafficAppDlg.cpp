// TrafficAppDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TrafficApp.h"
#include "TrafficAppDlg.h"

#define debugfile "D:\\projects\\GitHub\\TrafficApp\\test\\1small.jpg"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrafficAppDlg dialog

CTrafficAppDlg::CTrafficAppDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTrafficAppDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTrafficAppDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);


	m_InputImg = NULL;
	m_SrcImg = NULL;
	m_DstImg = NULL;
	m_CurImg = NULL;
	m_ShowImg = NULL;
	m_ZoomInImg = NULL;
	m_IsCalibration = FALSE;
	m_IsLBDown = FALSE;
	m_LineType = straight_line;
	m_CurveLength = 0.0;
	m_NumPts = 0;
	memset(m_MRect, 0, 9*sizeof(double));

	m_ShowImgWidth = 0;
	m_ShowImgHeight = 0;
	m_RatioX = 0;
	m_RatioY = 0;

	
	m_ptsCalibrate.clear();
	cvInitFont(&m_Font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5);

	m_FilePath.Format("");
}

CTrafficAppDlg::~CTrafficAppDlg()
{
	if(m_InputImg!=NULL)
	{
		cvReleaseImage(&m_InputImg);
		m_InputImg=NULL;
	}
	if(m_ZoomInImg!=NULL)
	{
		cvReleaseImage(&m_ZoomInImg);
		m_ZoomInImg=NULL;
	}
	if(m_SrcImg!=NULL)
	{
		cvReleaseImage(&m_SrcImg);
		m_SrcImg=NULL;
	}
	if(m_DstImg!=NULL)
	{
		cvReleaseImage(&m_DstImg);
		m_DstImg=NULL;
	}
	if(m_CurImg!=NULL)
	{
		cvReleaseImage(&m_CurImg);
		m_CurImg=NULL;
	}
	if(m_ShowImg!=NULL)
	{
		cvReleaseImage(&m_ShowImg);
		m_ShowImg=NULL;
	}
}


void CTrafficAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTrafficAppDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTrafficAppDlg, CDialog)
	//{{AFX_MSG_MAP(CTrafficAppDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_ABOUT, OnAbout)
	ON_BN_CLICKED(IDC_RADIO_LINE1, OnRadioLine1)
	ON_BN_CLICKED(IDC_RADIO_LINE2, OnRadioLine2)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_LAST, OnButtonRemove)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_BN_CLICKED(IDC_BUTTON_INIT, OnButtonCalibration)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrafficAppDlg message handlers

BOOL CTrafficAppDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_ZoomInImg = cvCreateImage(cvSize(ZOOM_IN_WIDTH*ZOOM_IN_TIMES, ZOOM_IN_HEIGHT*ZOOM_IN_TIMES),
		IPL_DEPTH_8U, 3);
	GetDlgItem(IDC_EDIT_TIPS)->SetWindowText("请选择图片！");
	((CButton *)GetDlgItem(IDC_RADIO_LINE1))->SetCheck(TRUE);
	
	m_Bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_Bmi.bmiHeader.biPlanes = 1;
	m_Bmi.bmiHeader.biBitCount = 24;
	m_Bmi.bmiHeader.biCompression = BI_RGB;
	m_Bmi.bmiHeader.biSizeImage = 0;
	m_Bmi.bmiHeader.biXPelsPerMeter = 0;
	m_Bmi.bmiHeader.biYPelsPerMeter = 0;
	m_Bmi.bmiHeader.biClrUsed = 0;
	m_Bmi.bmiHeader.biClrImportant = 0;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTrafficAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTrafficAppDlg::OnPaint() 
{
	if (m_DstImg)
		ShowImg(m_DstImg, m_DstImg->width, m_DstImg->height);
	else if (m_SrcImg)
		ShowImg(m_SrcImg, m_SrcImg->width, m_SrcImg->height);
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTrafficAppDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTrafficAppDlg::OnAbout() 
{
	// TODO: Add your command handler code here
	CAboutDlg dlg;
	dlg.DoModal();	
}

void CTrafficAppDlg::OnRadioLine1() 
{
	// TODO: Add your control notification handler code here
	m_LineType = straight_line;
}

void CTrafficAppDlg::OnRadioLine2() 
{
	// TODO: Add your control notification handler code here
	m_LineType = curve_line;
}

void CTrafficAppDlg::OnButtonRemove() 
{
	// TODO: Add your control notification handler code here
	if (m_IsCalibration && m_ShowImgWidth > 0)
	{
		cvCopy(m_SrcImg, m_DstImg);
		cvCopy(m_SrcImg, m_CurImg);
		ShowImg(m_SrcImg, m_SrcImg->width, m_SrcImg->height);
	}
}

void CTrafficAppDlg::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
	if (m_IsCalibration && m_DstImg != NULL)
	{
		CString FilePath;
		FilePath = m_FilePath + "_dst1.jpg";
		int i = 2;
		while(GetFileAttributes(FilePath) != 0xFFFFFFFF)
		{
			FilePath.Format("_dst%d.jpg", i);
			FilePath = m_FilePath + FilePath;
			i ++;
		}
		cvSaveImage(FilePath.GetBuffer(0), m_DstImg);
	}
}

void CTrafficAppDlg::OnFileOpen() 
{
	// TODO: Add your command handler code here
 	CString FilePath;
	CFileDialog dlg(TRUE, "*.jpg, *.bmp, *.tif", NULL, NULL, "图片文件(*.jpg, *.bmp, *.tif)|*.jpg|*.bmp|*.tif||");
	
	dlg.m_ofn.lpstrTitle =_T("打开文件");
	
	if(dlg.DoModal() == IDOK)	FilePath=dlg.GetPathName();	
	else	return;
	
	if(m_InputImg!=NULL)
	{
		cvReleaseImage(&m_InputImg);
		m_InputImg=NULL;
	}
		

//	FilePath.Format("%s", debugfile);
	m_InputImg=cvLoadImage(FilePath);
	if(!m_InputImg)
	{
		MessageBox("无法打开图片！");
		return;
	}

	m_FilePath = FilePath.Left(FilePath.GetLength()-4);

	float a = float(m_InputImg->width)/float(m_InputImg->height);
	m_ShowImgHeight = DISP_IMG_HEIGHT>m_InputImg->height ? m_InputImg->height:DISP_IMG_HEIGHT;
	m_ShowImgWidth = int(m_ShowImgHeight*a);
	if (m_ShowImgWidth > DISP_IMG_WIDTH)
	{
		m_ShowImgWidth = DISP_IMG_WIDTH;
		m_ShowImgHeight = int (float(m_ShowImgWidth)/a);
	}

	if(m_SrcImg!=NULL)
	{
		cvReleaseImage(&m_SrcImg);
		m_SrcImg=NULL;
	}
	m_SrcImg = cvCreateImage(cvSize(m_ShowImgWidth, m_ShowImgHeight), m_InputImg->depth, m_InputImg->nChannels);
	cvResize(m_InputImg, m_SrcImg);

	m_RatioX = (double)m_InputImg->width/(double)m_SrcImg->width;
	m_RatioY = (double)m_InputImg->height/(double)m_SrcImg->height;

	init(m_FilePath);
}

void CTrafficAppDlg::init(CString filename)
{
	m_NumPts = 0;
	m_ptsCalibrate.clear();
	memset(m_MRect, 0, 9*sizeof(double));
	//查看之前是否已经标定
	CString ParaPath;
	ParaPath = filename +"_Calibration.dat";
	FILE* fp = fopen(ParaPath,"r");
	
	if (fp != NULL)
	{
		//如果存在标定文件，则直接读取到数组M中，并提示已初始化
		for(int i=0;i<9;i++)
		{
			fscanf(fp,"%lf  ", &m_MRect[i]);
		}		
		fclose(fp);
	
		GetDlgItem(IDC_EDIT_TIPS)->SetWindowText("该图片已初始化，可选择距离\r\n测量方式进行距离测量！");
		GetDlgItem(IDC_BUTTON_INIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_INIT)->SetWindowText("初始化完成！");

		m_IsCalibration = TRUE;
	}
	else
	{
		m_NumPts = AutoDetectCalibrationPt(m_InputImg, m_AutoPtimage);
		if ( m_NumPts == 4 )//自动标定成功
		{
			PT_CORR temp = {0};
			m_ptsCalibrate.clear();
			temp.xi = m_AutoPtimage[0].x;
			temp.yi = m_AutoPtimage[0].y;
			temp.i = 1;
			temp.xs = 100;
			temp.ys = 100;
			temp.s = 1;
			m_ptsCalibrate.push_back(temp);

			temp.xi = m_AutoPtimage[1].x;
			temp.yi = m_AutoPtimage[1].y;
			temp.i = 1;
			temp.xs = 100;
			temp.ys = LengthCalibrationBlock+100;
			temp.s = 1;
			m_ptsCalibrate.push_back(temp);

			temp.xi = m_AutoPtimage[2].x;
			temp.yi = m_AutoPtimage[2].y;
			temp.i = 1;
			temp.xs = LengthCalibrationBlock+100;
			temp.ys = LengthCalibrationBlock+100;
			temp.s = 1;
			m_ptsCalibrate.push_back(temp);

			temp.xi = m_AutoPtimage[3].x;
			temp.yi = m_AutoPtimage[3].y;
			temp.i = 1;
			temp.xs = LengthCalibrationBlock+100;
			temp.ys = 100;
			temp.s = 1;
			m_ptsCalibrate.push_back(temp);

			CamputeCameraPara();
			SaveParaMat();

			GetDlgItem(IDC_EDIT_TIPS)->SetWindowText("该图片已完成自动标定，可选择距离\r\n测量方式进行距离测量！");
			GetDlgItem(IDC_BUTTON_INIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_INIT)->SetWindowText("初始化自动完成！");
			
			m_IsCalibration = TRUE;
		}
		else
		{
			//如果不存在标定文件，则提示需要初始化
			GetDlgItem(IDC_EDIT_TIPS)->SetWindowText("该图片未能自动标定，请手动标定。先对应坐标\r\n完成初始化，再测量距离！");
			GetDlgItem(IDC_BUTTON_INIT)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_INIT)->SetWindowText("坐标对应后，点击开始初始化");
			
			m_IsCalibration = FALSE;
		}
	}

	if(m_DstImg!=NULL)
	{
		cvReleaseImage(&m_DstImg);
		m_DstImg=NULL;
	}
	if(m_CurImg!=NULL)
	{
		cvReleaseImage(&m_CurImg);
		m_CurImg=NULL;
	}

	m_DstImg = cvCreateImage(cvGetSize(m_SrcImg), m_SrcImg->depth, m_SrcImg->nChannels);
	m_CurImg = cvCreateImage(cvGetSize(m_SrcImg), m_SrcImg->depth, m_SrcImg->nChannels);
	cvCopy(m_SrcImg, m_DstImg);
	cvCopy(m_SrcImg, m_CurImg);
	ShowImg(m_CurImg, m_CurImg->width, m_CurImg->height);

}

//自动检测标定块的四个角
int CTrafficAppDlg::AutoDetectCalibrationPt(IplImage *m_InputImg, CvPoint *Pt)
{
	CAutoDetect4Pts autodetect;
	
	if (!autodetect.SetImage(m_InputImg))
		return 0;

	autodetect.Detect();
	return autodetect.GetPts(Pt);
}

void CTrafficAppDlg::ShowImg(IplImage *srcImg, int width, int height )
{
	if( m_ShowImg == NULL )
		m_ShowImg = cvCreateImage( cvSize(srcImg->width,srcImg->height),
		IPL_DEPTH_8U, srcImg->nChannels );
	else if (m_ShowImg->width != srcImg->width)
	{
		cvReleaseImage(&m_ShowImg);
		m_ShowImg = cvCreateImage( cvSize(srcImg->width,srcImg->height),
			IPL_DEPTH_8U, srcImg->nChannels );
	}
	if( srcImg->origin == IPL_ORIGIN_TL )
		cvFlip( srcImg, m_ShowImg, 0 );
	else
		cvCopy( srcImg, m_ShowImg);
	
	m_Bmi.bmiHeader.biWidth = width;
	m_Bmi.bmiHeader.biHeight = height;
	
	CClientDC dc(this);

	float a = float(width)/float(height);
	m_ShowImgHeight = DISP_IMG_HEIGHT>height ? height:DISP_IMG_HEIGHT;
	m_ShowImgWidth = int(m_ShowImgHeight*a);
	if (m_ShowImgWidth > DISP_IMG_WIDTH)
	{
		m_ShowImgWidth = DISP_IMG_WIDTH;
		m_ShowImgHeight = int (float(m_ShowImgWidth)/a);
	}
	
	StretchDIBits(dc.m_hDC, DISP_SHIFT_X, DISP_SHIFT_Y, m_ShowImgWidth+DISP_SHIFT_X, m_ShowImgHeight+DISP_SHIFT_Y,
		0 ,0, width, height,(unsigned char *)m_ShowImg->imageData,&m_Bmi, DIB_RGB_COLORS, SRCCOPY);
}

BOOL CTrafficAppDlg::OnButtonCalibration() 
{
	// TODO: Add your control notification handler code here
	if (m_ShowImgHeight == 0)
		return 0;

	UpdateData(TRUE);
	if (m_IsCalibration)//已经初始化
	{
		GetDlgItem(IDC_EDIT_TIPS)->SetWindowText("该图片已初始化，可选择距离\r\n测量方式进行距离测量！");
		GetDlgItem(IDC_BUTTON_INIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_INIT)->SetWindowText("初始化完成！");
		return TRUE;
	}
	else if (m_NumPts < NUM_LEAST_PTS )
	{
		CString text;
		text.Format("已对应%d个点，请至少选择4个对应点(注意：点不能在同一直线上)！", m_NumPts);
		MessageBox(text.GetBuffer(0));
		return FALSE;
	}
	else{
		CamputeCameraPara();
		SaveParaMat();

		GetDlgItem(IDC_EDIT_TIPS)->SetWindowText("该图片已初始化，可选择距离\r\n测量方式进行距离测量！");
		GetDlgItem(IDC_BUTTON_INIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_INIT)->SetWindowText("初始化完成！");
		m_IsCalibration = TRUE;
		return TRUE;
	}
}

void CTrafficAppDlg::CamputeCameraPara()
{
	CvMat* src1 = cvCreateMat( m_NumPts*2, 8, CV_64FC1);
	CvMat* src2 = cvCreateMat( m_NumPts*2, 1,  CV_64FC1);
	CvMat* dst = cvCreateMat( 8, 1,  CV_64FC1);

	//赋值过程，建立求解方程
	cvZero(src1);
	cvZero(src2);
	cvZero(dst);
	vector<PT_CORR>::iterator iter;
	int i = 0;
	for (i = 0, iter = m_ptsCalibrate.begin(); iter!=m_ptsCalibrate.end(); iter++, i+=2)
	{
		cvmSet(src1, i, 0, (iter)->xi*(iter)->ys);
		cvmSet(src1, i, 1, (iter)->yi*(iter)->ys);
		cvmSet(src1, i, 2, (iter)->i*(iter)->ys);
		cvmSet(src1, i, 3, -(iter)->xi*(iter)->xs);
		cvmSet(src1, i, 4, -(iter)->yi*(iter)->xs);
		cvmSet(src1, i, 5, -(iter)->i*(iter)->xs);

		cvmSet(src1, i+1, 3, (iter)->xi*(iter)->s);
		cvmSet(src1, i+1, 4, (iter)->yi*(iter)->s);
		cvmSet(src1, i+1, 5, (iter)->i*(iter)->s);
		cvmSet(src1, i+1, 6, -(iter)->xi*(iter)->ys);
		cvmSet(src1, i+1, 7, -(iter)->yi*(iter)->ys);

		cvmSet(src2, i+1, 0, (iter)->i*(iter)->ys);
	}

	int abc = cvSolve(src1, src2, dst, CV_SVD);

	//将求解的M矩阵赋值给m_MRect;
	memcpy(m_MRect,dst->data.db,sizeof(double)*8);
	m_MRect[8] = 1.0;

	cvReleaseMat(&src1);
	cvReleaseMat(&src2);
	cvReleaseMat(&dst);
}

void CTrafficAppDlg::SaveParaMat()
{
	CString ParaPath;
	ParaPath = m_FilePath +"_Calibration.dat";
	FILE* fp = fopen(ParaPath,"w");
	
	for(int i=0;i<9;i++)
	{
		fprintf(fp,"%.8lf  ", m_MRect[i]);
	}
	
	fclose(fp);
}

void CTrafficAppDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_SrcImg == NULL
		|| point.x < DISP_SHIFT_X || point.x > m_ShowImgWidth+DISP_SHIFT_X
		|| point.y < DISP_SHIFT_Y || point.y > m_ShowImgHeight+DISP_SHIFT_Y)
		return;

	if (m_IsCalibration)
	{
		m_PtDownOri.x = point.x-DISP_SHIFT_X;
		m_PtDownOri.y = point.y-DISP_SHIFT_Y;
		
		m_PtDown[0] = m_MRect[0]*(point.x*m_RatioX) + m_MRect[1]*(point.y*m_RatioY) + m_MRect[2];
		m_PtDown[1] = m_MRect[3]*(point.x*m_RatioX) + m_MRect[4]*(point.y*m_RatioY) + m_MRect[5];
		m_PtDown[2] = m_MRect[6]*(point.x*m_RatioX) + m_MRect[7]*(point.y*m_RatioY) + m_MRect[8];
		
		if (m_PtDown[2]>0.0001 || m_PtDown[2] < -0.0001)
		{
			m_PtDown[0] /= m_PtDown[2];
			m_PtDown[1] /= m_PtDown[2];
		}
		else
		{
			m_PtDown[0] = 10000000.0;
			m_PtDown[1] = 10000000.0;
		}
		
		if (m_LineType == curve_line)
		{
			m_CurveLength = 0.0;
		}
		
		m_IsLBDown = TRUE;
	}	
	CDialog::OnLButtonDown(nFlags, point);
}

void CTrafficAppDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_SrcImg == NULL
		|| point.x < DISP_SHIFT_X || point.x > m_ShowImgWidth+DISP_SHIFT_X
		|| point.y < DISP_SHIFT_Y || point.y > m_ShowImgHeight+DISP_SHIFT_Y)
		return;

	if (m_IsCalibration)
	{
		m_PtUpOri.x = point.x-DISP_SHIFT_X;
		m_PtUpOri.y = point.y-DISP_SHIFT_Y;

		m_PtUp[0] = m_MRect[0]*(point.x*m_RatioX) + m_MRect[1]*(point.y*m_RatioY) + m_MRect[2];
		m_PtUp[1] = m_MRect[3]*(point.x*m_RatioX) + m_MRect[4]*(point.y*m_RatioY) + m_MRect[5];
		m_PtUp[2] = m_MRect[6]*(point.x*m_RatioX) + m_MRect[7]*(point.y*m_RatioY) + m_MRect[8];

		if (m_PtUp[2]>0.0001 || m_PtUp[2] < -0.0001)
		{
			m_PtUp[0] /= m_PtUp[2];
			m_PtUp[1] /= m_PtUp[2];
		}
		else
		{
			m_PtUp[0] = 10000000.0;
			m_PtUp[1] = 10000000.0;
		}
		
		if (m_LineType == straight_line)
		{
			
			double distance = sqrt((m_PtUp[0]-m_PtDown[0])*(m_PtUp[0]-m_PtDown[0])
				+(m_PtUp[1]-m_PtDown[1])*(m_PtUp[1]-m_PtDown[1]));
			
			cvCopy(m_DstImg, m_CurImg);
			CString text;
			text.Format("%.3lfm", distance);
			cvLine(m_CurImg, m_PtDownOri, m_PtUpOri, cvScalar(0, 255, 0));
			cvPutText( m_CurImg, text.GetBuffer(0), m_PtUpOri, &m_Font, cvScalar(255, 0 ,0));
			ShowImg(m_CurImg, m_CurImg->width, m_CurImg->height);
			cvCopy(m_CurImg, m_DstImg);
		}
		else if (m_LineType == curve_line)
		{
			m_CurveLength += sqrt((m_PtUp[0]-m_PtDown[0])*(m_PtUp[0]-m_PtDown[0])
				+(m_PtUp[1]-m_PtDown[1])*(m_PtUp[1]-m_PtDown[1]));
			
			cvCopy(m_DstImg, m_CurImg);
			CString text;
			text.Format("%.3lfcm", m_CurveLength);
			cvLine(m_CurImg, m_PtDownOri, m_PtUpOri, cvScalar(0, 255, 0));
			cvPutText( m_CurImg, text.GetBuffer(0), m_PtUpOri, &m_Font, cvScalar(255, 0 ,0));
			ShowImg(m_CurImg, m_CurImg->width, m_CurImg->height);
			cvCopy(m_CurImg, m_DstImg);
		}

		m_IsLBDown = FALSE;
	}
	else
	{
		CPtsDlg dlg;
		dlg.m_EditImgx = point.x-DISP_SHIFT_X;
		dlg.m_EditImgy = point.y-DISP_SHIFT_Y;
		UpdateData(TRUE);

		if (dlg.DoModal() == IDOK)
		{
			UpdateData(FALSE);
			PT_CORR temp = {0};
			temp.xi = dlg.m_EditImgx*m_RatioX;
			temp.yi = dlg.m_EditImgy*m_RatioY;
			temp.i = 1;
			temp.xs = dlg.m_EditSpax;
			temp.ys = dlg.m_EditSpay;
			temp.s = 1;
			m_ptsCalibrate.push_back(temp);
			m_NumPts ++;
		}
	}

	CDialog::OnLButtonUp(nFlags, point);
}

void CTrafficAppDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_SrcImg == NULL
		|| point.x < DISP_SHIFT_X || point.x > m_ShowImgWidth+DISP_SHIFT_X
		|| point.y < DISP_SHIFT_Y || point.y > m_ShowImgHeight+DISP_SHIFT_Y)
		return;

	GetImgRect(m_InputImg, m_ZoomInImg, point);
	ShowImgRect(m_ZoomInImg, m_ZoomInImg->width, m_ZoomInImg->height);

	if (m_IsCalibration && m_IsLBDown)
	{
		if (m_LineType == straight_line)
		{
			cvCopy(m_DstImg, m_CurImg);
			cvLine(m_CurImg, m_PtDownOri, cvPoint(point.x, point.y), cvScalar(0, 255, 0));
			ShowImg(m_CurImg, m_CurImg->width, m_CurImg->height);
		}
		else if (m_LineType == curve_line)
		{
			m_PtUpOri.x = point.x-DISP_SHIFT_X;
			m_PtUpOri.y = point.y-DISP_SHIFT_Y;
			m_PtUp[0] = m_MRect[0]*point.x*m_RatioX + m_MRect[1]*point.y*m_RatioY + m_MRect[2];
			m_PtUp[1] = m_MRect[3]*point.x*m_RatioX + m_MRect[4]*point.y*m_RatioY + m_MRect[5];
			m_PtUp[2] = m_MRect[6]*point.x*m_RatioX + m_MRect[7]*point.y*m_RatioY + m_MRect[8];			
			if (m_PtUp[2]>0.0001 || m_PtUp[2] < -0.0001)
			{
				m_PtUp[0] /= m_PtUp[2];
				m_PtUp[1] /= m_PtUp[2];
			}
			else
			{
				m_PtUp[0] = 10000000.0;
				m_PtUp[1] = 10000000.0;
			}
			
			m_CurveLength += sqrt((m_PtUp[0]-m_PtDown[0])*(m_PtUp[0]-m_PtDown[0])
				+(m_PtUp[1]-m_PtDown[1])*(m_PtUp[1]-m_PtDown[1]));

			cvCopy(m_DstImg, m_CurImg);
			cvLine(m_CurImg, m_PtDownOri, m_PtUpOri, cvScalar(0, 255, 0));
			ShowImg(m_CurImg, m_CurImg->width, m_CurImg->height);
			cvCopy(m_CurImg, m_DstImg);

			m_PtDownOri = m_PtUpOri;
			m_PtDown[0] = m_PtUp[0];
			m_PtDown[1] = m_PtUp[1];
			m_PtDown[2] = m_PtUp[2];
		}
	}
	CDialog::OnMouseMove(nFlags, point);
}

int CTrafficAppDlg::GetImgRect(IplImage *pSrcImg, IplImage *pImgRect, CPoint pt)
{
	BYTE *pdst_buf = (BYTE *)pImgRect->imageData;
	BYTE *psrc_buf = (BYTE *)pSrcImg->imageData;
	pt.x = pt.x * m_RatioX;
	pt.y = pt.y * m_RatioY;

	int offset_x = 0, center_x = 0;
	int offset_y = 0, center_y = 0;
	int i, j, k, l;
	if ((pt.x - ZOOM_IN_WIDTH/2) < 0)
	{
		offset_x = 0;
		center_x = pt.x*ZOOM_IN_TIMES;
	}
	else if ((pt.x + ZOOM_IN_WIDTH/2) > pSrcImg->width)
	{
		offset_x = pSrcImg->width - ZOOM_IN_WIDTH;
		center_x = (ZOOM_IN_WIDTH - pSrcImg->width + pt.x) * ZOOM_IN_TIMES;
	}
	else
	{
		offset_x = pt.x - ZOOM_IN_WIDTH/2;
		center_x = ZOOM_IN_WIDTH*ZOOM_IN_TIMES/2;
	}

	if ((pt.y - ZOOM_IN_HEIGHT/2) < 0)
	{
		offset_y = 0;
		center_y = pt.y*ZOOM_IN_TIMES;
	}
	else if ((pt.y + ZOOM_IN_HEIGHT/2) > pSrcImg->height)
	{
		offset_y = pSrcImg->height - ZOOM_IN_HEIGHT;
		center_y = (ZOOM_IN_HEIGHT - pSrcImg->height + pt.y)*ZOOM_IN_TIMES;
	}
	else
	{
		offset_y = pt.y - ZOOM_IN_HEIGHT/2;
		center_y = ZOOM_IN_HEIGHT*ZOOM_IN_TIMES/2;
	}
	
	for (i = 0; i < ZOOM_IN_HEIGHT; i ++)
	{
		for (l = 0; l < ZOOM_IN_TIMES; l ++)
		{
			for (j  = 0; j < ZOOM_IN_WIDTH; j ++)
			{
				for (k = 0; k < ZOOM_IN_TIMES; k ++)
				{
					pdst_buf [(i*ZOOM_IN_TIMES +l) * pImgRect->widthStep + (j*ZOOM_IN_TIMES +k)*3] =
						(BYTE) psrc_buf[(offset_y +i) * pSrcImg->widthStep + (j +offset_x)*3];
					pdst_buf [(i*ZOOM_IN_TIMES +l) * pImgRect->widthStep + (j*ZOOM_IN_TIMES +k)*3 +1] =
						(BYTE) psrc_buf[(offset_y +i) * pSrcImg->widthStep + (j +offset_x)*3 +1];
					pdst_buf [(i*ZOOM_IN_TIMES +l) * pImgRect->widthStep + (j*ZOOM_IN_TIMES +k)*3 + 2] =
						(BYTE) psrc_buf[(offset_y +i) * pSrcImg->widthStep + (j +offset_x)*3 + 2];
				}
			}
		}

	}

	//画个十字架
	for (i = -5; i < 5; i ++)
	{
		if (center_y+i >= pImgRect->height || center_y+i < 0)
			continue;

		pdst_buf [(center_y+i) * pImgRect->widthStep + (center_x)*3] = 255;	
		pdst_buf [(center_y+i) * pImgRect->widthStep + (center_x)*3+1] = 255;	
		pdst_buf [(center_y+i) * pImgRect->widthStep + (center_x)*3+2] = 255;	
	}
	for (j = -5; j < 5; j ++)
	{
		if (center_x+j >= pImgRect->width || center_x+j < 0)
			continue;

		pdst_buf [(center_y) * pImgRect->widthStep + (center_x+j)*3] = 255;	
		pdst_buf [(center_y) * pImgRect->widthStep + (center_x+j)*3+1] = 255;	
		pdst_buf [(center_y) * pImgRect->widthStep + (center_x+j)*3+2] = 255;	
	}
	
	return 0;
}

void CTrafficAppDlg::ShowImgRect(IplImage *srcImg, int width, int height )
{
	if( srcImg->origin == IPL_ORIGIN_TL )
		cvFlip( srcImg, srcImg, 0 );
	
	m_Bmi.bmiHeader.biWidth = width;
	m_Bmi.bmiHeader.biHeight = height;
	
	CClientDC dc(this);
	
	StretchDIBits(dc.m_hDC, DISP_RECT_SHIFT_X, DISP_RECT_SHIFT_Y, ZOOM_IN_WIDTH*ZOOM_IN_TIMES+DISP_SHIFT_X,
		ZOOM_IN_HEIGHT*ZOOM_IN_TIMES+DISP_SHIFT_Y, 0 ,0, width, 
		height, (unsigned char *)srcImg->imageData,&m_Bmi, DIB_RGB_COLORS, SRCCOPY);
}
