#pragma once

#include "framework.h"

//------------------------------------------------------------------------------------
//
class CImageComparisionHelper
{
protected:

	CString cl_CSImage1Path;
	Gdiplus::Bitmap* clp_Image1 = nullptr;

	CString cl_CSImage2Path;
	Gdiplus::Bitmap* clp_Image2 = nullptr;


	Gdiplus::Bitmap* clp_DiffImage = nullptr;
	Gdiplus::Bitmap* clp_DiffThresholdImage = nullptr;

	int i_DiffThresholdValue = 10;
	CSize cl_MasterImageSize;

	void fnRegenerateDiffImage();
	Gdiplus::Bitmap* fnOpenImage( CString claCSImagePath);

	int i_TotalPixels = 1;
	int i_DifferentPixels = 0;

public :
	CSize fnGetMasterImageSize() { return cl_MasterImageSize; };
	int fnGetDiffThresholdValue() { return i_DiffThresholdValue; };

	int fnGetTotalPixels() { return i_TotalPixels; };
	int fnGetDifferentPixels() { return i_DifferentPixels; };


	CString fnGetImage1Path() { return cl_CSImage1Path; }
	CString fnGetImage2Path() { return cl_CSImage2Path; }



	Gdiplus::Bitmap* fnGetImage1() {
		return clp_Image1;
	}
	Gdiplus::Bitmap* fnGetImage2() {
		return clp_Image2;
	}
	Gdiplus::Bitmap* fnGetDiffImage() {
		return clp_DiffImage;
	}
	Gdiplus::Bitmap* fnGetDiffThresholdImage() {
		return clp_DiffThresholdImage;
	}


public :
	CImageComparisionHelper();
	~CImageComparisionHelper();

	void fnDestroyData();

	void fnSetImagePath(CString claCSImagePath, bool baForImage1);
	void fnSetImagePath(CString claCSImage1Path, CString claCSImage2Path);

	void fnSetDiffThresholdValue(int iaThresholdValue)
	{
		if (i_DiffThresholdValue != iaThresholdValue)
		{
			i_DiffThresholdValue = iaThresholdValue;
			fnRegenerateDiffImage();
		}
	}
};