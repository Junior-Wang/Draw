
// DrawTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DrawTest.h"
#include "DrawTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDrawTestDlg dialog



CDrawTestDlg::CDrawTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDrawTestDlg::IDD, pParent)
	, m_bShowImg(false)
	, m_bDragging(false)
	, m_nClickNum(0)
	, m_bRightTime(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_HCross = AfxGetApp()->LoadStandardCursor(IDC_CROSS);
}

void CDrawTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SHOW_PICTURE, m_PictureControl);
}

BEGIN_MESSAGE_MAP(CDrawTestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_SELIMG, &CDrawTestDlg::OnChangeSelimg)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
//	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// CDrawTestDlg message handlers

BOOL CDrawTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDrawTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDrawTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDrawTestDlg::OnChangeSelimg()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString filename;
	GetDlgItemText(IDC_SELIMG, filename);

	if (m_bShowImg)
	{
		m_0Img.Destroy();
	}
	if (S_OK!=m_0Img.Load(filename))
	{
		AfxMessageBox(_T("图像加载出错"));
	}
	ATL::CImage dstimg;
	CDC* pDC = GetDlgItem(IDC_SHOW_PICTURE)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(IDC_SHOW_PICTURE)->GetClientRect(&rect);
	pDC->SetStretchBltMode(COLORONCOLOR);
	m_0Img.Draw(hDC, rect);
	ReleaseDC(pDC);

	m_bShowImg = true;
}


void CDrawTestDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//先获得控件相对于窗口的位置
	CRect re;
	m_PictureControl.GetClientRect(&re);
	CRect reBeforeMap = re;
	m_PictureControl.MapWindowPoints((CWnd *)this, re);
	//不在图片控件内
	if (point.x<re.left || point.y<re.top 
		|| point.x>re.right || point.y>re.bottom) 
		return;
	//没有加载图像
	if (!m_bShowImg)
		return;
	m_nClickNum = m_nClickNum % 4;
	if (0==m_nClickNum)
	{
		m_bDragging = true;
		CDC* pDC = GetDlgItem(IDC_SHOW_PICTURE)->GetDC();
		pDC->SetStretchBltMode(COLORONCOLOR);
		m_0Img.Draw(pDC->GetSafeHdc(), reBeforeMap);
		ReleaseDC(pDC);

		m_CutImg4Points.clear();
		m_bRightTime = false;
	}
	m_CutImg4Points.push_back(point);
	SetCapture();//捕捉鼠标，充分控制鼠标
	m_prevPoint = point;
	m_origPoint = point;
	SetCursor(m_HCross);
	ClientToScreen(&re);//转换为屏幕坐标
	ClipCursor(&re);//限定光标在指定矩形
	m_nClickNum++;
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CDrawTestDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//CClientDC aDC(this);
	//CPen pen(PS_SOLID, 1, RGB(255, 0, 0));
	//aDC.SelectObject(&pen);
	if (!m_bDragging)
	{
		return;
	}
	if (m_nClickNum>3)
	{
		m_bDragging = false;
		ReleaseCapture();//结束捕捉鼠标
		ClipCursor(NULL);
	}
	CRect re;
	m_PictureControl.GetClientRect(&re);
	m_PictureControl.MapWindowPoints((CWnd *)this, re);
	CString info;
	info.Format(_T("(%d, %d)"), (point.x - re.left)*m_0Img.GetWidth() / re.Width(), 
		(point.y - re.top)*m_0Img.GetHeight() / re.Height()  );
	SetDlgItemText(IDC_STATIC, info);
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CDrawTestDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (!m_bDragging)
	{
		return;
	}
	SetCursor(m_HCross);
	CClientDC aDC(this);
	aDC.SetROP2(R2_NOT);//逆转当前屏幕颜色绘图
	//CPen pen(PS_SOLID, 1, RGB(0, 255, 0));
	//aDC.SelectObject(&pen);
	aDC.MoveTo(m_origPoint);
	aDC.LineTo(m_prevPoint);
	aDC.MoveTo(m_origPoint);
	aDC.LineTo(point);
	if (m_nClickNum>1)
	{	
		if ( m_bRightTime )
		{
			aDC.MoveTo(m_CutImg4Points.at(0));
			aDC.LineTo(m_prevPoint);
		}
		m_bRightTime = true;
		aDC.MoveTo(m_CutImg4Points.at(0));
		aDC.LineTo(point);
	}
	m_prevPoint = point;
	CDialogEx::OnMouseMove(nFlags, point);
}