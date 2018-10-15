// AerofoilDoc.h : interface of the CAerofoilDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_AEROFOILDOC_H__BBA1B6AD_B27A_11D6_AF75_002018BA082E__INCLUDED_)
#define AFX_AEROFOILDOC_H__BBA1B6AD_B27A_11D6_AF75_002018BA082E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BackgroundGrid.hpp"
#include "kernel\plot.h"

class CPlotOrderDlg;

class CAerofoilDoc : public CDocument
{
protected: // create from serialization only
	CAerofoilDoc();
	DECLARE_DYNCREATE(CAerofoilDoc)

// Attributes
public:

	CPlot& getPlot() {return plot;}
	float sizeX() const {return size_x;}
	float sizeY() const {return size_y;}
	CBackgroundGrid& getGrid() {return grid;}


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAerofoilDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

	void UpdateNow();
	void RedrawNow();

	void setSelection(CPlotStructure* ps);
	void clearSelection();
	CPlotStructure* getSelection() {return selected;}

	void selectWing(CWing* pw);
	void selectEllipses(CEllipsePair* pep);
	void selectPointStructure(CPointStructure* pps);

	void closePlotOrderDialog();

// Implementation
public:
	virtual ~CAerofoilDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

private:

	void setDocSize(float width, float height);
	void updatePlacePosition();


	CPlot plot;		// THE plot

	// Currently selected things
	CWing* currentWing;	
	CEllipsePair* currentEllipse;
	CPointStructure* currentPoint;

	CPlotStructure* selected;

	CPlotOrderDlg* plotOrderDialog;

	float size_x;	// document size in mm
	float size_y;

	CBackgroundGrid grid;

	float place_x;	// where to put the next section.
	float place_y;

	std::string cncHost;
	unsigned int cncPort;
	bool cncIsSetup;

// Generated message map functions
protected:
	//{{AFX_MSG(CAerofoilDoc)
	afx_msg void OnWingFlags();
	afx_msg void OnUpdateWingFlags(CCmdUI* pCmdUI);
	afx_msg void OnWingNew();
	afx_msg void OnWingNewCore();
	afx_msg void OnUpdateWingNewCore(CCmdUI* pCmdUI);
	afx_msg void OnWingNewSection();
	afx_msg void OnUpdateWingNewSection(CCmdUI* pCmdUI);
	afx_msg void OnWingEdit();
	afx_msg void OnUpdateCurrentWingSet(CCmdUI* pCmdUI);
	afx_msg void OnFilePlotSize();
	afx_msg void OnEllipseEdit();
	afx_msg void OnUpdateEllipseSelected(CCmdUI* pCmdUI);
	afx_msg void OnEllipseFlags();
	afx_msg void OnEllipseNew();
	afx_msg void OnEllipseNewSection();
	afx_msg void OnFileDxf();
	afx_msg void OnFileLaserjet();
	afx_msg void OnFilePostscript();
	afx_msg void OnEditPosition();
	afx_msg void OnUpdateItemIsSelected(CCmdUI* pCmdUI);
	afx_msg void OnEditDelete();
	afx_msg void OnEditStructure();
	afx_msg void OnEditPlotflags();
	afx_msg void OnPointNew();
	afx_msg void OnEditPlotorder();
	afx_msg void OnUpdateEditPlotorder(CCmdUI* pCmdUI);
	afx_msg void OnFileCncoutput();
	afx_msg void OnFileCncsetup();
	afx_msg void OnEditDeletestructure();
	afx_msg void OnFileSetgrid();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AEROFOILDOC_H__BBA1B6AD_B27A_11D6_AF75_002018BA082E__INCLUDED_)
