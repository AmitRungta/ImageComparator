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

// ImageComparatorView.cpp : implementation of the CImageComparatorView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ImageComparator.h"
#endif

#include "ImageComparatorDoc.h"
#include "ImageComparatorView.h"
#include "ImageComparisionHelper.h"
#include "MainFrm.h"
#include "GDIPlusHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CImageComparisionHelper __gclComparisionHelperData;



// Loopup table for zoom view.
float __gfpZoomFactor[] = { 1.f / 10.f
							, 1.f / 8.f
							, 1.f / 6.f
							, 1.f / 4.f
							, 1.f / 3.f
							, 1.f / 2.f
							, 2.f / 3.f
							, 1.f
							, 2.f
							, 3.f
							, 4.f
							, 5.f
							, 6.f
							, 7.f
							, 8.f
							, 9.f
							, 10.f
};



// CImageComparatorView

IMPLEMENT_DYNCREATE(CImageComparatorView, CScrollView)

BEGIN_MESSAGE_MAP(CImageComparatorView, CScrollView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_FILEOPEN_1, &CImageComparatorView::OnFileopen1)
	ON_COMMAND(ID_FILEOPEN_2, &CImageComparatorView::OnFileopen2)
	ON_COMMAND(ID_THRESHOLDLIMIT_INCREASE, &CImageComparatorView::OnThresholdlimitIncrease)
	ON_UPDATE_COMMAND_UI(ID_THRESHOLDLIMIT_INCREASE, &CImageComparatorView::OnUpdateThresholdlimitIncrease)
	ON_COMMAND(ID_THRESHOLDLIMIT_DECREASE, &CImageComparatorView::OnThresholdlimitDecrease)
	ON_UPDATE_COMMAND_UI(ID_THRESHOLDLIMIT_DECREASE, &CImageComparatorView::OnUpdateThresholdlimitDecrease)
	ON_COMMAND(ID_ZOOMIN, &CImageComparatorView::OnZoomIn)
	ON_UPDATE_COMMAND_UI(ID_ZOOMIN, &CImageComparatorView::OnUpdateZoomIn)
	ON_COMMAND(ID_ZOOMOUT, &CImageComparatorView::OnZoomOut)
	ON_UPDATE_COMMAND_UI(ID_ZOOMOUT, &CImageComparatorView::OnUpdateZoomOut)
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CImageComparatorView construction/destruction



//------------------------------------------------------------------------------------
//
CImageComparatorView::CImageComparatorView() noexcept
{
	// Added by AmitRungta  for initialising the scroll bars
	SetScrollSizes(MM_TEXT, CSize(0, 0));

	i_ZoomIndex = 7;
	const int iZoomFactorCount = sizeof(__gfpZoomFactor) / sizeof(__gfpZoomFactor[0]);
	for (int iI = 0; iI < iZoomFactorCount; iI++)
	{
		if (fabs(__gfpZoomFactor[iI] - 1.f) < 0.001)
		{
			i_ZoomIndex = iI;
			break;
		}
	}
}



//------------------------------------------------------------------------------------
//
CImageComparatorView::~CImageComparatorView()
{
}



//------------------------------------------------------------------------------------
//
BOOL CImageComparatorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

// CImageComparatorView drawing




//------------------------------------------------------------------------------------
//
void CImageComparatorView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}



//------------------------------------------------------------------------------------
//
void CImageComparatorView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
}


// CImageComparatorView diagnostics

#ifdef _DEBUG
void CImageComparatorView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImageComparatorView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CImageComparatorDoc* CImageComparatorView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageComparatorDoc)));
	return (CImageComparatorDoc*)m_pDocument;
}
#endif //_DEBUG


// CImageComparatorView message handlers


//------------------------------------------------------------------------------------
//
void CImageComparatorView::OnFileopen1()
{
	static TCHAR BASED_CODE szFilter[] = _T("All Supported Files (*.bmp;*.gif;*.png;*.tiff;*.tif;*.jpeg;*.jpg)|*.bmp;*.gif;*.png;*.tiff;*.tif;*.jpeg;*.jpg||");
	CFileDialog clFileDialog(TRUE, NULL, __gclComparisionHelperData.fnGetImage1Path(), OFN_FILEMUSTEXIST, szFilter);
	if (clFileDialog.DoModal() != IDOK)
		return;
	CString clCSFilePath = clFileDialog.GetPathName();

	CMainFrame* clpMainFame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
	if (clpMainFame)
		clpMainFame->fnUpdateStatusBarText(_T(""));

	__gclComparisionHelperData.fnSetImagePath(clCSFilePath, true);
	ResyncScrollSizes();
	Invalidate();
}



//------------------------------------------------------------------------------------
//
void CImageComparatorView::OnFileopen2()
{
	static TCHAR BASED_CODE szFilter[] = _T("All Supported Files (*.bmp;*.gif;*.png;*.tiff;*.tif;*.jpeg;*.jpg)|*.bmp;*.gif;*.png;*.tiff;*.tif;*.jpeg;*.jpg||");
	CFileDialog clFileDialog(TRUE, NULL, __gclComparisionHelperData.fnGetImage2Path(), OFN_FILEMUSTEXIST, szFilter);
	if (clFileDialog.DoModal() != IDOK)
		return;
	CString clCSFilePath = clFileDialog.GetPathName();

	CMainFrame* clpMainFame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
	if (clpMainFame)
		clpMainFame->fnUpdateStatusBarText(_T(""));

	__gclComparisionHelperData.fnSetImagePath(clCSFilePath, false);
	ResyncScrollSizes();
	Invalidate();
}



//------------------------------------------------------------------------------------
//
void CImageComparatorView::OnThresholdlimitIncrease()
{
	__gclComparisionHelperData.fnSetDiffThresholdValue(__gclComparisionHelperData.fnGetDiffThresholdValue() + 1);
	ResyncScrollSizes();
	Invalidate();
}



//------------------------------------------------------------------------------------
//
void CImageComparatorView::OnUpdateThresholdlimitIncrease(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(__gclComparisionHelperData.fnGetDiffThresholdValue() < 100);
}



//------------------------------------------------------------------------------------
//
void CImageComparatorView::OnThresholdlimitDecrease()
{
	__gclComparisionHelperData.fnSetDiffThresholdValue(__gclComparisionHelperData.fnGetDiffThresholdValue() - 1);
	ResyncScrollSizes();
	Invalidate();
}



//------------------------------------------------------------------------------------
//
void CImageComparatorView::OnUpdateThresholdlimitDecrease(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(__gclComparisionHelperData.fnGetDiffThresholdValue() > 1);
}



//------------------------------------------------------------------------------------
//
float CImageComparatorView::fnGetZoomFactor() const
{
	return __gfpZoomFactor[i_ZoomIndex];
}



//------------------------------------------------------------------------------------
//
Gdiplus::InterpolationMode fnGetDesiredInterpolationMethod(const float nfaZoomFactor)
{
	if (nfaZoomFactor < .1)
		return Gdiplus::InterpolationModeHighQualityBicubic;
	else if (nfaZoomFactor < .33)
		return Gdiplus::InterpolationModeHighQualityBilinear;
	else if (nfaZoomFactor < .66)
		return Gdiplus::InterpolationModeBicubic;
	else if (nfaZoomFactor < 1.)
		return Gdiplus::InterpolationModeBilinear;

	return Gdiplus::InterpolationModeNearestNeighbor;
}




//------------------------------------------------------------------------------------
//
void CImageComparatorView::OnDraw(CDC* pDC)
{
	if (!pDC || pDC->IsPrinting())
	{
		// Currently we donot support the printing option.
		ASSERT(FALSE);
		return;
	}

	CImageComparatorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CMainFrame* clpMainFame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd());

	CRect clClientRect;
	GetClientRect(clClientRect);
	clClientRect.OffsetRect(GetScrollPosition());

	CMemDC clMemDC(*pDC, this);
	clMemDC.GetDC().FillSolidRect(clClientRect, RGB(255, 255, 255));

	// Added by AmitRungta first of all initialsie the GDI library
	CGDIPlusHelper::fnInitialiseGDIPlusHelper();

	fnDrawImage(&clMemDC.GetDC(), 0);
	fnDrawImage(&clMemDC.GetDC(), 1);
	fnDrawImage(&clMemDC.GetDC(), 2);
	fnDrawImage(&clMemDC.GetDC(), 3);


	{
		int iSplitterWidth = clClientRect.Width();
		if (iSplitterWidth < GetTotalSize().cx)
			iSplitterWidth = GetTotalSize().cx;

		int iSplitterHeight = clClientRect.Height();
		if (iSplitterHeight < GetTotalSize().cy)
			iSplitterHeight = GetTotalSize().cy;

		int iCenterX = clClientRect.Width() / 2 + GetScrollPosition().x;
		int iCenterY = clClientRect.Height() / 2 + GetScrollPosition().y;

		// Draw Vert Line.
		clMemDC.GetDC().FillSolidRect(iCenterX - 3, 0, 7, iSplitterHeight, RGB(255, 255, 255));
		clMemDC.GetDC().FillSolidRect(iCenterX - 2, 0, 5, iSplitterHeight, RGB(0, 0, 0));

		// Draw Horz Line.
		clMemDC.GetDC().FillSolidRect(0, iCenterY - 3, iSplitterWidth, 7, RGB(255, 255, 255));
		clMemDC.GetDC().FillSolidRect(0, iCenterY - 2, iSplitterWidth, 5, RGB(0, 0, 0));
	}


	fnUpdateStatusText();
}


//------------------------------------------------------------------------------------
//
void CImageComparatorView::fnUpdateStatusText()
{
	CImageComparatorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CMainFrame* clpMainFame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd());

	if (__gclComparisionHelperData.fnGetMasterImageSize().cx < 1 || __gclComparisionHelperData.fnGetMasterImageSize().cy < 1)
	{
		if (clpMainFame)
			clpMainFame->fnUpdateStatusBarText(_T(""));
		return;
	}


	const float nfZoomFactor = fnGetZoomFactor();

	CSize sizeDoc = __gclComparisionHelperData.fnGetMasterImageSize();
	CString clCSPaneText;
	clCSPaneText.Format(_T("Image Dim : %d x %d  -  Zoom Factor : %.2f%%"), sizeDoc.cx, sizeDoc.cy, (float)(nfZoomFactor * 100.f));

	// Now lets add the compression status.
	{
		CString clCSThreshold;
		clCSThreshold.Format(_T("Threshold : %d"), __gclComparisionHelperData.fnGetDiffThresholdValue());
		clCSPaneText += _T("  --  ") + clCSThreshold;
	}

	// Now lets add the compression status.
	if ( __gclComparisionHelperData.fnGetTotalPixels() > 0 )
	{
		CString clCSThreshold;
		clCSThreshold.Format(_T("Difference Pixels : %0.2f%%"), ( __gclComparisionHelperData.fnGetDifferentPixels() * 100.f ) / __gclComparisionHelperData.fnGetTotalPixels() );
		clCSPaneText += _T("  --  ") + clCSThreshold;
	}

	if (clpMainFame)
		clpMainFame->fnUpdateStatusBarText(clCSPaneText);
}



//------------------------------------------------------------------------------------
//
void CImageComparatorView::OnInitialUpdate()
{
	ResyncScrollSizes();
	CScrollView::OnInitialUpdate();
}



//------------------------------------------------------------------------------------
//
void CImageComparatorView::ResyncScrollSizes()
{
	CClientDC dc(NULL);
	OnPrepareDC(&dc);
	CSize sizeDoc = __gclComparisionHelperData.fnGetMasterImageSize();

	const float nfZoomFactor = fnGetZoomFactor();
	sizeDoc.cx = (int)((sizeDoc.cx * 2 * nfZoomFactor) + 0.5) + 3; // 2 px for splitter
	sizeDoc.cy = (int)((sizeDoc.cy * 2 * nfZoomFactor) + 0.5) + 3; // 2 px for splitter
	dc.LPtoDP(&sizeDoc);
	SetScrollSizes(MM_TEXT, sizeDoc);
}



//------------------------------------------------------------------------------------
//
void CImageComparatorView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	CImageComparatorDoc* pDoc = GetDocument();
	CScrollView::OnPrepareDC(pDC, pInfo);

	const float nfZoomFactor = fnGetZoomFactor();


	pDC->SetMapMode(MM_TEXT);
	CSize sizeDoc = __gclComparisionHelperData.fnGetMasterImageSize();
	if (sizeDoc.cx < 1)
		sizeDoc.cx = 1;
	if (sizeDoc.cy < 1)
		sizeDoc.cy = 1;
	sizeDoc.cy = -sizeDoc.cy;

	sizeDoc.cx = (int)((sizeDoc.cx * 2 * nfZoomFactor) + 0.5) + 3; // 2 px for splitter
	sizeDoc.cy = (int)((sizeDoc.cy * 2 * nfZoomFactor) + 0.5) + 3; // 2 px for splitter

	pDC->SetWindowExt(sizeDoc);
	pDC->SetViewportExt((int)sizeDoc.cx, (int)-sizeDoc.cy);
}



//------------------------------------------------------------------------------------
//
void CImageComparatorView::OnSize(UINT nType, int cx, int cy)
{
	ResyncScrollSizes();        // ensure that scroll info is up-to-date
	Invalidate();
	CScrollView::OnSize(nType, cx, cy);
}



//------------------------------------------------------------------------------------
//
void CImageComparatorView::fnZoomIn()
{
	const int iZoomFactorCount = sizeof(__gfpZoomFactor) / sizeof(__gfpZoomFactor[0]);
	if (i_ZoomIndex < (iZoomFactorCount - 1))
	{
		i_ZoomIndex++;
		ResyncScrollSizes();        // ensure that scroll info is up-to-date
		Invalidate();
	}
}



//------------------------------------------------------------------------------------
//
void CImageComparatorView::OnZoomIn()
{
	fnZoomIn();
}



//------------------------------------------------------------------------------------
//
void CImageComparatorView::OnUpdateZoomIn(CCmdUI* pCmdUI)
{
	const int iZoomFactorCount = sizeof(__gfpZoomFactor) / sizeof(__gfpZoomFactor[0]);
	pCmdUI->Enable(i_ZoomIndex < (iZoomFactorCount - 1));
}



//------------------------------------------------------------------------------------
//
void CImageComparatorView::fnZoomOut()
{
	if (i_ZoomIndex >= 1)
	{
		i_ZoomIndex--;
		ResyncScrollSizes();        // ensure that scroll info is up-to-date
		Invalidate();
	}
}



//------------------------------------------------------------------------------------
//
void CImageComparatorView::OnZoomOut()
{
	fnZoomOut();
}



//------------------------------------------------------------------------------------
//
void CImageComparatorView::OnUpdateZoomOut(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(i_ZoomIndex >= 1);
}



//------------------------------------------------------------------------------------
//
BOOL CImageComparatorView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	if ((nFlags & MK_CONTROL) == MK_CONTROL)
	{
		if (zDelta < 0)
			fnZoomOut();
		else
			fnZoomIn();
		return TRUE;
	}

	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}



//------------------------------------------------------------------------------------
//
void CImageComparatorView::OnLButtonDown(UINT nFlags, CPoint point)
{
	__super::OnLButtonDown(nFlags, point);
	SetCapture();       // Capture the mouse until button up.
	cl_LastProcessedPoint = point;   // Serves as the MoveTo() anchor point for the

	return;
}



//------------------------------------------------------------------------------------
//
void CImageComparatorView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// Mouse button up is interesting in the Scribble application
	// only if the user is currently drawing a new stroke by dragging
	// the captured mouse.

	if (GetCapture() == this)
	{
		ReleaseCapture();   // Release the mouse capture established at
							// the beginning of the mouse drag.
	}
	__super::OnLButtonUp(nFlags, point);
}



//------------------------------------------------------------------------------------
//
void CImageComparatorView::OnMouseMove(UINT nFlags, CPoint point)
{
	// Mouse movement is interesting in the Scribble application
	// only if the user is currently drawing a new stroke by dragging
	// the captured mouse.

	if (GetCapture() == this)
	{
		CPoint clOffset = point - cl_LastProcessedPoint;
		cl_LastProcessedPoint = point;

		OnScrollBy(-clOffset);
	}

	__super::OnMouseMove(nFlags, point);
}


//------------------------------------------------------------------------------------
//
BOOL CImageComparatorView::OnScrollBy(CSize sizeScroll, BOOL bDoScroll /*= TRUE*/)
{
	if (bDoScroll)
		Invalidate();

	return __super::OnScrollBy(sizeScroll, bDoScroll);
}


BOOL CImageComparatorView::OnEraseBkgnd(CDC* pDC)
{
	//return CScrollView::OnEraseBkgnd(pDC);
	return TRUE;
}


//------------------------------------------------------------------------------------
//
void CImageComparatorView::fnDrawImage(CDC* pDC, int iaIndex)
{
	// Added by AmitRungta first of all initialsie the GDI library
	CGDIPlusHelper::fnInitialiseGDIPlusHelper();

	CRect clDestRect;
	CRect clSrcRect;
	const float nfZoomFactor = fnGetZoomFactor();

	CRect clClientRect;
	GetClientRect(clClientRect);
	const int niClientWidth = clClientRect.Width() / 2;
	const int niClientHeight = clClientRect.Height() / 2 ;


	{
		clDestRect.left = GetScrollPosition().x / 2;
		clDestRect.right = GetTotalSize().cx / 2;
		clDestRect.top = GetScrollPosition().y / 2 ;
		clDestRect.bottom = GetTotalSize().cy / 2;

		if (clDestRect.Width() > niClientWidth)
			clDestRect.right = clDestRect.left + niClientWidth;
		if (clDestRect.Height() > niClientHeight)
			clDestRect.bottom = clDestRect.top + niClientHeight;
	}


	// Get the source rect from the destination rect
	clSrcRect.left = (int)((clDestRect.left / nfZoomFactor));
	clSrcRect.right = (int)((clDestRect.right / nfZoomFactor) + 0.5);
	clSrcRect.top = (int)((clDestRect.top / nfZoomFactor));
	clSrcRect.bottom = (int)((clDestRect.bottom / nfZoomFactor) + 0.5);

	{
		clDestRect.left += GetScrollPosition().x / 2;
		clDestRect.right += GetScrollPosition().x / 2;
		clDestRect.top += GetScrollPosition().y / 2;
		clDestRect.bottom += GetScrollPosition().y / 2;
	}


	Gdiplus::Bitmap* clpBitmap = nullptr;
	if (0 == iaIndex)
	{
		clpBitmap = __gclComparisionHelperData.fnGetImage1();
	}
	else if (1 == iaIndex)
	{
		clpBitmap = __gclComparisionHelperData.fnGetImage2();
		clDestRect.left += niClientWidth;
		clDestRect.right += niClientWidth;
	}
	else if (2 == iaIndex)
	{
		clpBitmap = __gclComparisionHelperData.fnGetDiffImage();

		clDestRect.top += niClientHeight;
		clDestRect.bottom += niClientHeight;
	}
	else if (3 == iaIndex)
	{
		clpBitmap = __gclComparisionHelperData.fnGetDiffThresholdImage();

		clDestRect.left += niClientWidth;
		clDestRect.right += niClientWidth;
		clDestRect.top += niClientHeight;
		clDestRect.bottom += niClientHeight;
	}

	Gdiplus::RectF destRect((Gdiplus::REAL)clDestRect.left, (Gdiplus::REAL)clDestRect.top, (Gdiplus::REAL)clDestRect.Width(), (Gdiplus::REAL)clDestRect.Height());

	if (!clpBitmap)
	{
		Gdiplus::Graphics dcDst(pDC->GetSafeHdc());

		Gdiplus::SolidBrush solidBrush(Gdiplus::Color(255, 255, 255, 255));
		Gdiplus::Status status = dcDst.FillRectangle(&solidBrush, destRect);
		return;
	}


	{
		Gdiplus::Graphics dcDst(pDC->GetSafeHdc());
		Gdiplus::InterpolationMode enmOldInterpolationMethod = dcDst.GetInterpolationMode();
		Gdiplus::InterpolationMode enmDesiredInterpolationMethod = fnGetDesiredInterpolationMethod(nfZoomFactor);

		dcDst.SetInterpolationMode(enmDesiredInterpolationMethod);
		Gdiplus::RectF destRect((Gdiplus::REAL)clDestRect.left, (Gdiplus::REAL)clDestRect.top, (Gdiplus::REAL)clDestRect.Width(), (Gdiplus::REAL)clDestRect.Height());
		Gdiplus::RectF sourceRect((Gdiplus::REAL)clSrcRect.left, (Gdiplus::REAL)clSrcRect.top, (Gdiplus::REAL)clSrcRect.Width(), (Gdiplus::REAL)clSrcRect.Height());

		Gdiplus::Status status = dcDst.DrawImage(clpBitmap, destRect, sourceRect, Gdiplus::UnitPixel);

		// now restore the original interpolation method.
		dcDst.SetInterpolationMode(enmOldInterpolationMethod);
	}

	return;
}





