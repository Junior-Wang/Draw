
// DrawTestDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include <vector>

// CDrawTestDlg dialog
class CDrawTestDlg : public CDialogEx
{
// Construction
public:
	CDrawTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_DRAWTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	bool m_bShowImg;
	ATL::CImage m_0Img;

	int m_nClickNum;
	std::vector<CPoint> m_CutImg4Points;
	CPoint m_prevPoint;
	CPoint m_origPoint;
	HCURSOR m_HCross;
	bool m_bDragging;
	bool m_bRightTime;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_PictureControl;
	afx_msg void OnChangeSelimg();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
//	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};
