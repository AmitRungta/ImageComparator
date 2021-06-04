#pragma once

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")



//------------------------------------------------------------------------------------
// Added for having a helper function for GDI Plus usage.....
// This class has been taken from the CImage implementation.. 
//
class CGDIPlusHelper
{

private :
	CGDIPlusHelper() throw();

public:
	~CGDIPlusHelper() throw();

protected :
	bool fnInitialiseGDIPlus() throw();
	void fnDeInitialiseGDIPlus() throw();

private:
	ULONG_PTR m_dwToken;
	CRITICAL_SECTION m_sect;
	DWORD m_dwLastError;

private :
	static CGDIPlusHelper* fnGetGDIPlusHelper()
	{
#pragma warning(suppress: 4640) // will always be initialized on entry thread by CImageStaticInitializer
		static CGDIPlusHelper gdiPlus;
		return &gdiPlus;
	}

public :
	static bool fnInitialiseGDIPlusHelper() { return fnGetGDIPlusHelper()->fnInitialiseGDIPlus(); }
	static void fnDeInitialiseGDIPlusHelper() { fnGetGDIPlusHelper()->fnDeInitialiseGDIPlus(); }
};





//------------------------------------------------------------------------------------
//
inline CGDIPlusHelper::CGDIPlusHelper() throw() :
	m_dwToken(0), m_dwLastError(S_OK)
{
	if (!_AtlInitializeCriticalSectionEx(&m_sect, 0, 0))
	{
		m_dwLastError = HRESULT_FROM_WIN32(GetLastError());
	}
}



//------------------------------------------------------------------------------------
//
inline CGDIPlusHelper::~CGDIPlusHelper() throw()
{
	fnDeInitialiseGDIPlus();
}



//------------------------------------------------------------------------------------
//
inline bool CGDIPlusHelper::fnInitialiseGDIPlus() throw()
{
	if (m_dwLastError != S_OK)
	{
		return false;
	}

	EnterCriticalSection(&m_sect);
	bool fRet = true;

	if (m_dwToken == 0)
	{
		Gdiplus::GdiplusStartupInput input;
		Gdiplus::GdiplusStartupOutput output;
		Gdiplus::Status status = Gdiplus::GdiplusStartup(&m_dwToken, &input, &output);
		if (status != Gdiplus::Ok)
			fRet = false;
	}
	LeaveCriticalSection(&m_sect);
	return fRet;
}



//------------------------------------------------------------------------------------
//
inline void CGDIPlusHelper::fnDeInitialiseGDIPlus() throw()
{
	EnterCriticalSection(&m_sect);
	if (m_dwToken != 0)
	{
		Gdiplus::GdiplusShutdown(m_dwToken);
	}
	m_dwToken = 0;
	LeaveCriticalSection(&m_sect);
}
