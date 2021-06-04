#include "pch.h"
#include "ImageComparisionHelper.h"
#include "GDIPlusHelper.h"
#include <fstream>






//------------------------------------------------------------------------------------
//
CImageComparisionHelper::CImageComparisionHelper()
{
	clp_Image1 = nullptr;
	clp_Image2 = nullptr;
	clp_DiffImage = nullptr;
	clp_DiffThresholdImage = nullptr;
	i_DiffThresholdValue = 10;
}




//------------------------------------------------------------------------------------
//
CImageComparisionHelper::~CImageComparisionHelper()
{
	fnDestroyData();
}




//------------------------------------------------------------------------------------
//
void CImageComparisionHelper::fnDestroyData()
{
	if (clp_Image1)
		delete clp_Image1;
	clp_Image1 = nullptr;

	if (clp_Image2)
		delete clp_Image2;
	clp_Image2 = nullptr;

	if (clp_DiffImage)
		delete clp_DiffImage;
	clp_DiffImage = nullptr;

	if (clp_DiffThresholdImage)
		delete clp_DiffThresholdImage;
	clp_DiffThresholdImage = nullptr;
}




//------------------------------------------------------------------------------------
//
Gdiplus::Bitmap* CImageComparisionHelper::fnOpenImage(CString claCSImagePath)
{
	if (claCSImagePath.IsEmpty())
		return false;

	CGDIPlusHelper::fnInitialiseGDIPlusHelper();

	Gdiplus::Bitmap* clpBitmapData = nullptr;

	// this is a bmp file hence open it normally and save the compressed data
	clpBitmapData = Gdiplus::Bitmap::FromFile(claCSImagePath);
	if (!clpBitmapData)
	{
		AfxMessageBox(_T("Not a valid image file."), MB_ICONERROR);
		return nullptr; // we are unable to open this image.
	}

	// We just support 24 bit bmp files.
	if (PixelFormat24bppRGB != clpBitmapData->GetPixelFormat())
	{
		// This file is not in RGB format hence lets convert this into RGB format first.
		int iPaletteEntries = 256;
		Gdiplus::ColorPalette* clpColorPalette = (Gdiplus::ColorPalette*)malloc(sizeof(Gdiplus::ColorPalette) + iPaletteEntries * sizeof(Gdiplus::ARGB));
		clpColorPalette->Count = iPaletteEntries; clpColorPalette->Flags = 0;

		Gdiplus::Bitmap::InitializePalette(clpColorPalette, Gdiplus::PaletteTypeOptimal, iPaletteEntries, FALSE, NULL);

		Gdiplus::Status enmStatus = clpBitmapData->ConvertFormat(PixelFormat24bppRGB
			, Gdiplus::DitherTypeSolid
			, Gdiplus::PaletteTypeCustom
			, clpColorPalette
			, 0
		);

		// Now free the color palette
		free(clpColorPalette);
		clpColorPalette = nullptr;
	}

	return clpBitmapData;
}



//------------------------------------------------------------------------------------
//
void CImageComparisionHelper::fnSetImagePath(CString claCSImagePath, bool baForImage1)
{
	Gdiplus::Bitmap** clppBitmap = baForImage1 ? &clp_Image1 : &clp_Image2;
	CString* clpCSImagePath = baForImage1 ? &cl_CSImage1Path : &cl_CSImage2Path;
	if (clpCSImagePath->CompareNoCase(claCSImagePath) == 0)
		return;

	if (*clppBitmap)
		delete* clppBitmap;
	*clppBitmap = nullptr;

	*clpCSImagePath = _T("");

	*clppBitmap = fnOpenImage(claCSImagePath);
	if (*clppBitmap)
		*clpCSImagePath = claCSImagePath;

	fnRegenerateDiffImage();
}



//------------------------------------------------------------------------------------
//
void CImageComparisionHelper::fnSetImagePath(CString claCSImage1Path, CString claCSImage2Path)
{
	if (clp_Image1)
		delete clp_Image1;
	clp_Image1 = nullptr;
	cl_CSImage1Path = _T("");

	if (clp_Image2)
		delete clp_Image2;
	clp_Image2 = nullptr;
	cl_CSImage2Path = _T("");

	clp_Image1 = fnOpenImage(claCSImage1Path);
	if (clp_Image1)
		cl_CSImage1Path = claCSImage1Path;

	clp_Image2 = fnOpenImage(claCSImage2Path);
	if (clp_Image2)
		cl_CSImage2Path = claCSImage2Path;

	fnRegenerateDiffImage();
}



//------------------------------------------------------------------------------------
//
void CImageComparisionHelper::fnRegenerateDiffImage()
{
	if (clp_DiffImage)
		delete clp_DiffImage;
	clp_DiffImage = nullptr;

	if (clp_DiffThresholdImage)
		delete clp_DiffThresholdImage;
	clp_DiffThresholdImage = nullptr;

	i_TotalPixels = 0; 
	i_DifferentPixels = 0; 


	if (!clp_Image1 || !clp_Image2)
	{
		if (clp_Image1)
		{
			cl_MasterImageSize.cx = clp_Image1->GetWidth();
			cl_MasterImageSize.cy = clp_Image1->GetHeight();
		}
		else
		{
			cl_MasterImageSize.cx = clp_Image2->GetWidth();
			cl_MasterImageSize.cy = clp_Image2->GetHeight();
		}
		return;
	}

	cl_MasterImageSize.cx = min(clp_Image1->GetWidth(), clp_Image2->GetWidth());
	cl_MasterImageSize.cy = min(clp_Image1->GetHeight(), clp_Image2->GetHeight());

	i_TotalPixels = cl_MasterImageSize.cx * cl_MasterImageSize.cy;
	i_DifferentPixels = 0;


	clp_DiffImage = new Gdiplus::Bitmap((INT)cl_MasterImageSize.cx, (INT)cl_MasterImageSize.cy, PixelFormat24bppRGB);
	clp_DiffThresholdImage = new Gdiplus::Bitmap((INT)cl_MasterImageSize.cx, (INT)cl_MasterImageSize.cy, PixelFormat24bppRGB);

	Gdiplus::BitmapData* bitmapDiffImage = new Gdiplus::BitmapData;
	Gdiplus::BitmapData* bitmapDiffThresholdImage = new Gdiplus::BitmapData;

	Gdiplus::BitmapData* bitmapImage1 = new Gdiplus::BitmapData;
	Gdiplus::BitmapData* bitmapImage2 = new Gdiplus::BitmapData;

	Gdiplus::Rect rect(0, 0, cl_MasterImageSize.cx, cl_MasterImageSize.cy);

	clp_DiffImage->LockBits(&rect, Gdiplus::ImageLockModeWrite, PixelFormat24bppRGB, bitmapDiffImage);
	clp_DiffThresholdImage->LockBits(&rect, Gdiplus::ImageLockModeWrite, PixelFormat24bppRGB, bitmapDiffThresholdImage);

	clp_Image1->LockBits(&rect, Gdiplus::ImageLockModeRead, PixelFormat24bppRGB, bitmapImage1);
	clp_Image2->LockBits(&rect, Gdiplus::ImageLockModeRead, PixelFormat24bppRGB, bitmapImage2);


	BYTE* ucpDiffImageBuffer = (BYTE*)bitmapDiffImage->Scan0;
	BYTE* ucpDiffThresholdImageBuffer = (BYTE*)bitmapDiffThresholdImage->Scan0;
	BYTE* ucpImage1Buffer = (BYTE*)bitmapImage1->Scan0;
	BYTE* ucpImage2Buffer = (BYTE*)bitmapImage2->Scan0;


	int iStrideDiffImageBuffer = bitmapDiffImage->Stride;
	int iStrideDiffThresholdImageBuffer = bitmapDiffThresholdImage->Stride;
	int iStrideImage1Buffer = bitmapImage1->Stride;
	int iStrideImage2Buffer = bitmapImage2->Stride;


	for (int iRow = 0; iRow < cl_MasterImageSize.cy; iRow++)
	{
		for (int iCol = 0; iCol < cl_MasterImageSize.cx; iCol++)
		{
			int iIndex = iCol * 3;

			ucpDiffImageBuffer[iIndex] = abs((int)ucpImage1Buffer[iIndex] - (int)ucpImage2Buffer[iIndex]);
			ucpDiffImageBuffer[iIndex+1] = abs((int)ucpImage1Buffer[iIndex+1] - (int)ucpImage2Buffer[iIndex+1]);
			ucpDiffImageBuffer[iIndex+2] = abs((int)ucpImage1Buffer[iIndex+2] - (int)ucpImage2Buffer[iIndex+2]);

			BYTE ucMax = max(max(ucpDiffImageBuffer[iIndex], ucpDiffImageBuffer[iIndex + 1]), ucpDiffImageBuffer[iIndex + 2]);
		
			if (ucMax >= i_DiffThresholdValue)
			{
				ucpDiffThresholdImageBuffer[iIndex] = 0;
				ucpDiffThresholdImageBuffer[iIndex + 1] = 0;
				ucpDiffThresholdImageBuffer[iIndex + 2] = 0;
				i_DifferentPixels++;
			}
			else
			{
				ucpDiffThresholdImageBuffer[iIndex] = 255;
				ucpDiffThresholdImageBuffer[iIndex + 1] = 255;
				ucpDiffThresholdImageBuffer[iIndex + 2] = 255;
			}
		}
		
		ucpDiffImageBuffer += iStrideDiffImageBuffer;
		ucpDiffThresholdImageBuffer += iStrideDiffThresholdImageBuffer;
		ucpImage1Buffer += iStrideImage1Buffer;
		ucpImage2Buffer += iStrideImage2Buffer;
	}

	
	clp_DiffImage->UnlockBits(bitmapDiffImage);
	clp_DiffThresholdImage->UnlockBits(bitmapDiffThresholdImage);
	clp_Image1->UnlockBits(bitmapImage1);
	clp_Image2->UnlockBits(bitmapImage2);

	::DeleteObject(bitmapDiffImage);	delete bitmapDiffImage;	bitmapDiffImage = nullptr;
	::DeleteObject(bitmapDiffThresholdImage);	delete bitmapDiffThresholdImage;	bitmapDiffThresholdImage = nullptr;
	::DeleteObject(bitmapImage1);	delete bitmapImage1;	bitmapImage1 = nullptr;
	::DeleteObject(bitmapImage2);	delete bitmapImage2;	bitmapImage2 = nullptr;
}


