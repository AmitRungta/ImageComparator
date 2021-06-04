// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface
// (the "Fluent UI") and is provided only as referential material to supplement the
// Microsoft Foundation Classes Reference and related electronic documentation
// included with the MFC C++ library software.
// License terms to copy, use or distribute the Fluent UI are available separately.
// To learn more about our Fluent UI licensing program, please visit
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// ImageComparatorView.h : interface of the CImageComparatorView class
//

#pragma once


class CImageComparatorView : public CScrollView
{
protected: // create from serialization only
	CImageComparatorView() noexcept;
	DECLARE_DYNCREATE(CImageComparatorView)

// Attributes
public:
	CImageComparatorDoc* GetDocument() const;

	// Operations
protected:
	int i_ZoomIndex;
	CPoint      cl_LastProcessedPoint;       // the last mouse pt in the stroke in progress

public :
	void ResyncScrollSizes();       // ensure scroll info is up-to-date
	void fnZoomIn();
	void fnZoomOut();
	float fnGetZoomFactor() const;

	void fnUpdateStatusText();
	void fnDrawImage(CDC* pDC, int iaIndex);



// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);

protected:

// Implementation
public:
	virtual ~CImageComparatorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif




// Generated message map functions
protected:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileopen1();
	afx_msg void OnFileopen2();
	afx_msg void OnThresholdlimitIncrease();
	afx_msg void OnUpdateThresholdlimitIncrease(CCmdUI* pCmdUI);
	afx_msg void OnThresholdlimitDecrease();
	afx_msg void OnUpdateThresholdlimitDecrease(CCmdUI* pCmdUI);


	afx_msg void OnZoomIn();
	afx_msg void OnUpdateZoomIn(CCmdUI* pCmdUI);
	afx_msg void OnZoomOut();
	afx_msg void OnUpdateZoomOut(CCmdUI* pCmdUI);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in ImageComparatorView.cpp
inline CImageComparatorDoc* CImageComparatorView::GetDocument() const
   { return reinterpret_cast<CImageComparatorDoc*>(m_pDocument); }
#endif

