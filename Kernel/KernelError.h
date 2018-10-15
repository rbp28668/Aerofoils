// KernelError.h: interface for the KernelError class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KERNELERROR_H__A4E22B62_B474_11D6_AF75_002018BA082E__INCLUDED_)
#define AFX_KERNELERROR_H__A4E22B62_B474_11D6_AF75_002018BA082E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KernelError  
{
public:
	KernelError(int iErr);
	virtual ~KernelError();

	enum {
		IDS_ERR_SPLINE_SPLINE_MEM,
		IDS_ERR_SPLINE_BAD_XA,
		IDS_ERR_SPLINE_MEM,
		IDS_ERR_UNABLE_TO_OPEN_AEROFOIL_FILE,
		IDS_ERR_INVALID_SPAR_TYPE
	};
};

#endif // !defined(AFX_KERNELERROR_H__A4E22B62_B474_11D6_AF75_002018BA082E__INCLUDED_)
