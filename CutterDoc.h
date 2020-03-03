/* Aerofoil
Aerofoil plotting and CNC cutter driver
Copyright(C) 1995-2019 R Bruce Porteous

This program is free software : you can redistribute it and / or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once
#include <string>
#include "BackgroundGrid.hpp"
#include "Kernel\CutterGeometry.h"
#include "kernel\cut.h"

// CutterDoc document
// Main MFC document class to hold structure / cuts for driving CNC cutter as 
// distinct from plotting on paper which see CAerofoilDoc.

class CutterDoc : public CDocument
{
	DECLARE_DYNCREATE(CutterDoc)
	
	Cut cut;
	CBackgroundGrid grid;
	CutterGeometry geometry;

	std::string cncHost;
	unsigned int cncPort;
	bool cncIsSetup;


public:
	CutterDoc();
	virtual ~CutterDoc();

#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	void UpdateNow();
	void RedrawNow();
	void geometryUpdated();

	double sizeX() const { return geometry.getXTravel(); }
	double sizeY() const { return geometry.getYTravel(); }
	const CutterGeometry& getGeometry() { return geometry; }

	CBackgroundGrid& getGrid() { return grid; }


	CWing* newWing(const char* pszRootName, float rootThickness, const char* pszTipName, float tipThickness);
	CEllipsePair* newEllipse(CEllipsePair& example);
	CPointStructure* newPoint(CPointStructure& example);
	GCodeSnippet* newGcodeSnippet(const char* text);
	DXFObject* newDxfObject(const char* pszPath);
	Cut::StructureIterator beginStructures() { return cut.beginStructures(); }
	Cut::StructureIterator endStructures() { return cut.endStructures(); }
	void deleteStructure(CStructure* pStructure);

	CPathCutter* newPathCutter(CWing* pStructure);
	EllipseCutter* newElipseCutter(CEllipsePair* pEllipses);
	PointCutter* newPointCutter(CPointStructure* pPoints);
	DXFObjectCutter* newDXFObjectCutter(DXFObject* pdxf);
	GCodeSnippetCutter* newGCodeSnippetCutter(GCodeSnippet* pdxf);
	HomeCutter* newHomePosition();
	inline void moveUp(CutStructure* cs) { cut.moveUp(cs); }
	inline void moveDown(CutStructure* cs) { cut.moveDown(cs); }
	inline int indexOf(CutStructure* cs) { return cut.indexOf(cs); }
	Cut::CutIterator beginCutStructures() { return cut.beginCutStructures(); }
	Cut::CutIterator endCutStructures() { return cut.endCutStructures(); }
	void deleteCut(CutStructure* pCut);


	void runCut(COutputDevice& pdev);

protected:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileSetgrid();
	afx_msg void OnFileCncoutput();
	afx_msg void OnFileCncsetup();
	afx_msg void OnFileGcode();

	afx_msg void OnCutterTooloffset();
	afx_msg void OnCutterFeedrate();
};
