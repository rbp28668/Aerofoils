#pragma once
#include <afxwin.h>

class Cut;
class CutterGeometry;
class CCoordMap;
class PointT;

class CutPlanformControl :
	public CStatic
{
	Cut* cut;
	CutterGeometry* geometry;
	void dimension(CDC& dc, const CCoordMap& map, const PointT& start, const PointT& finish);
public:
	CutPlanformControl(Cut* cut, CutterGeometry* geometry);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
};

