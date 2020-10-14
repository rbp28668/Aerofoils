#include "stdafx.h"
#include <assert.h>
#include <limits>
#include <sstream>
#include <algorithm>
#include "CutPlanformControl.h"
#include "CoordMap.h"
#include "Kernel/PointT.h"
#include "Kernel/RectT.h"
#include "Kernel/OutputDevice.h"
#include "Kernel/Cut.h"
#include "Kernel/CutterGeometry.h"
#include "Kernel/PointCutter.h"

// Replaced by <algorithm>
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif


BEGIN_MESSAGE_MAP(CutPlanformControl, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()


class PlanformBounds :
	public COutputDevice
{
	NumericT minlx, maxlx, minrx, maxrx, minz, maxz;
	PointT lastMove[2];
	bool lastOpIsMove[2];

public:
	PlanformBounds();
	~PlanformBounds();

public:

	virtual void MoveTo(int iStream, const PointT& pt);
	virtual void LineTo(int iStream, const PointT& pt);
	virtual void Label(int iStream, const char* psz) {};
	virtual void Home() {};
	virtual void Flush() {};
	virtual PointT position(int iStream) { return PointT(); }

	void reset();
	
	NumericT minLeft() const { return minlx; }
	NumericT maxLeft() const { return maxlx; }
	NumericT minRight() const { return minrx; }
	NumericT maxRight() const { return maxrx; }
	NumericT minZ() const { return minz; }
	NumericT maxZ() const { return maxz; }

private:

};

PlanformBounds::PlanformBounds()
{
	reset();
}

PlanformBounds::~PlanformBounds()
{
}

void PlanformBounds::MoveTo(int iStream, const PointT& pt)
{
	assert(this);
	assert(iStream == 0 || iStream == 1);
	lastMove[iStream] = pt;
	lastOpIsMove[iStream] = true;
}
void PlanformBounds::LineTo(int iStream, const PointT& pt)
{
	assert(this);
	assert(iStream == 0 || iStream == 1);
	if (lastOpIsMove[iStream])
	{
		lastOpIsMove[iStream] = false;
		LineTo(iStream, lastMove[iStream]);
	}

	assert(pt.fx != std::numeric_limits<float>::infinity());
	assert(pt.fy != std::numeric_limits<float>::infinity());
	assert(pt.fx != -std::numeric_limits<float>::infinity());
	assert(pt.fy != -std::numeric_limits<float>::infinity());

	// As dealing with planform we can ignore y but track left and 
	// right separately.
	if (iStream == 0) {
		if (pt.fx < minlx) minlx = pt.fx;
		if (pt.fx > maxlx) maxlx = pt.fx;
	}
	else {
		if (pt.fx < minrx) minrx = pt.fx;
		if (pt.fx > maxrx) maxrx = pt.fx;
	}

	if (pt.fz < minz) minz = pt.fz;
	if (pt.fz > maxz) maxz = pt.fz;

}

void PlanformBounds::reset()
{
	minlx = minrx = minz = std::numeric_limits<NumericT>::max();
	maxlx = maxrx = maxz = -std::numeric_limits<NumericT>::max();
	lastOpIsMove[0] = lastOpIsMove[1] = false;
	lastMove[0] = lastMove[1] = PointT();
}



class PlanformDrawDevice : public COutputDevice
{
public:
	PlanformDrawDevice(CutterGeometry* geometry, CDC* pdc, const CCoordMap* map);
	virtual ~PlanformDrawDevice();

	void setDrawMoves(bool drawMoves) { this->drawMoves = drawMoves; }

	// COutputDevice
	virtual void MoveTo(int iStream, const PointT& pt);
	virtual void LineTo(int iStream, const PointT& pt);
	virtual void Label(int iStream, const char* psz);
	virtual void Home();
	virtual void Flush();
	virtual PointT position(int iStream);

private:
	PointT toPlanform(int iStream, const PointT& pt);

	CutterGeometry* geometry;
	CDC* plotDC;
	const CCoordMap* map;

	PointT current[2];	// last position for each stream
	bool hasRoot;
	bool hasTip;

	CPen* penGrey;
	CPen* penBlack;
	CPen* penOriginal;

	bool drawMoves;

};


PlanformDrawDevice::PlanformDrawDevice(CutterGeometry* geometry, CDC* pdc, const CCoordMap* map)
	: map(map)
	, geometry(geometry)
	, plotDC(pdc)
	, penBlack(0)
	, penGrey(0)
	, drawMoves(false)
	, hasRoot(false)
	, hasTip(false)
	
{
	penGrey = new CPen(PS_SOLID, 0, RGB(128, 128, 128));
	penBlack = new CPen(PS_SOLID, 0, RGB(0, 0, 0));
	penOriginal = pdc->SelectObject(penBlack);
}

PlanformDrawDevice::~PlanformDrawDevice()
{
	plotDC->SelectObject(penOriginal);
	delete penBlack;
	delete penGrey;
}

void PlanformDrawDevice::MoveTo(int iStream, const PointT& pt)
{
	assert(this);
	assert(iStream == 0 || iStream == 1);

	current[iStream] = pt;
	if (iStream == 0) hasRoot = true;
	if (iStream == 1) hasTip = true;

	if(hasRoot && hasTip)
		{
		if (drawMoves) {
			plotDC->SelectObject(penGrey);
			plotDC->MoveTo(map->toDevice(toPlanform(0, current[0])));
			plotDC->LineTo(map->toDevice(toPlanform(1, current[1])));
		}
		hasRoot = hasTip = 0; 
	}
}

void PlanformDrawDevice::LineTo(int iStream, const PointT& pt)
{
	assert(this);
	assert(iStream == 0 || iStream == 1);

	current[iStream] = pt;
	if (iStream == 0) hasRoot = true;
	if (iStream == 1) hasTip = true;

	if (hasRoot && hasTip)
	{
		plotDC->SelectObject(penBlack);
		plotDC->MoveTo(map->toDevice(toPlanform(0, current[0])));
		plotDC->LineTo(map->toDevice(toPlanform(1, current[1])));
		hasRoot = hasTip = 0;
	}
}

void PlanformDrawDevice::Label(int iStream, const char* psz)
{
	assert(this);
}

void PlanformDrawDevice::Home()
{
	assert(this);
}

void PlanformDrawDevice::Flush()
{
	assert(this);
}

PointT PlanformDrawDevice::position(int iStream)
{
	assert(this);
	assert(iStream == 0 || iStream == 1);
	return current[iStream];
}

PointT PlanformDrawDevice::toPlanform(int iStream, const PointT& pt) {
	PointT p;
	// Note swap of coordinates as want z/x plot to appear as x/y
	p.fx = geometry->getBlockLeft() + pt.fz;

	// If z on RHS is zero then not been set so assume RHS of block.
	if (iStream == 1 && pt.fz == 0) {
		p.fx = geometry->getBlockRight(); 
	}
	p.fy = pt.fx;
	return p;
}




CutPlanformControl::CutPlanformControl(Cut* cut, CutterGeometry* geometry)
	: cut(cut)
	, geometry(geometry)
{
}

void CutPlanformControl::dimension(CDC& dc, const CCoordMap& map, const PointT& start, const PointT& finish) {

	bool isHorizontal = (start.fy == finish.fy);

	CFont font;
	font.CreateFont(
		12,                       // nHeight
		0,                        // nWidth
		0,                        // nEscapement
		isHorizontal ? 0 : 900,   // nOrientation
		FW_NORMAL,                // nWeight
		FALSE,                    // bItalic
		FALSE,                    // bUnderline
		0,                        // cStrikeOut
		ANSI_CHARSET,             // nCharSet
		OUT_DEFAULT_PRECIS,       // nOutPrecision
		CLIP_DEFAULT_PRECIS,      // nClipPrecision
		DEFAULT_QUALITY,          // nQuality
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily
		_T("Arial"));            // lpszFacename

	CFont* def_font = dc.SelectObject(&font);

	dc.SetTextColor(RGB(255, 0, 0));

	NumericT dist = (isHorizontal) ? (finish.fx - start.fx) : (finish.fy - start.fy);
	if (dist < 0) dist = -dist;

	std::ostringstream str;
	str << dist << "mm" << std::ends;
	RECT rSize;
	dc.DrawText(str.str().c_str(), &rSize, DT_CALCRECT | DT_SINGLELINE);

	CPen redPen(PS_SOLID, 1, RGB(255,0,0));
	CPen* pOldPen = dc.SelectObject(&redPen);
	
	POINT ps = map.toDevice(start);
	POINT pf = map.toDevice(finish);
	dc.MoveTo(ps);
	dc.LineTo(pf);

	const int arrow = 6;
	if (isHorizontal) {
		dc.MoveTo(ps);
		dc.LineTo(ps.x + arrow, ps.y + arrow);
		dc.MoveTo(ps);
		dc.LineTo(ps.x + arrow, ps.y - arrow);
		dc.MoveTo(pf);
		dc.LineTo(pf.x - arrow, pf.y + arrow);
		dc.MoveTo(pf);
		dc.LineTo(pf.x - arrow, pf.y - arrow);
	}
	else {
		dc.MoveTo(ps);
		dc.LineTo(ps.x + arrow, ps.y - arrow);
		dc.MoveTo(ps);
		dc.LineTo(ps.x - arrow, ps.y - arrow);
		dc.MoveTo(pf);
		dc.LineTo(pf.x + arrow, pf.y + arrow);
		dc.MoveTo(pf);
		dc.LineTo(pf.x - arrow, pf.y + arrow);
	}

	PointT p((start.fx + finish.fx) / 2, (start.fy + finish.fy) / 2);
	POINT pt = map.toDevice(p);
	RECT rClip;
	int width = rSize.right - rSize.left;
	int height = rSize.top - rSize.bottom;
	rClip.left = pt.x - width / 2;
	rClip.right = pt.x + width / 2;
	rClip.top = pt.y + height / 2;
	rClip.bottom = pt.y - height / 2;

	dc.DrawText(str.str().c_str(), &rClip, DT_SINGLELINE);

	dc.SelectObject(def_font);
	font.DeleteObject();

	dc.SelectObject(pOldPen);

}

void CutPlanformControl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
    CBrush background;
    //background.CreateSolidBrush(RGB(255, 200, 200));
    background.CreateSysColorBrush(COLOR_3DFACE); // Face color for three-dimensional display elements and for dialog box backgrounds.
    RECT r;
    GetWindowRect(&r);
    ScreenToClient(&r);

    dc.FillRect(&r, &background);


	PlanformBounds bounds;
	CutStructure::Context context;
	for (auto it = cut->beginCutStructures(); it != cut->endCutStructures(); ++it) {
		CutStructure* pStruct = (*it);
		if (pStruct->getType() == PointCutter::TYPE) continue;
		pStruct->cut(&bounds, context);
	}

	const NumericT carriageWidth = 100;
	// Set up bounds in mm to draw cutter and leave top + buttom space
	RectT b(-carriageWidth,geometry->getXTravel()+100, geometry->getWidth()+carriageWidth, -50);

	CCoordMap map(b, r);


	// Draw carriages left and right
	RectT leftCarriage(-carriageWidth, geometry->getXTravel(), 0, 0);
	RectT rightCarriage(geometry->getWidth(), geometry->getXTravel(), geometry->getWidth() + carriageWidth, 0);

	dc.SetBkColor(RGB(192, 192, 192));
	CBrush hardwareBrush;
	hardwareBrush.CreateHatchBrush(HS_DIAGCROSS,RGB(255, 255, 255));
	CBrush blackBrush;
	blackBrush.CreateSolidBrush(RGB(0, 0, 0));

	RECT carriage = map.toDevice(leftCarriage);
	dc.FillRect(&carriage, &hardwareBrush);
	dc.FrameRect(&carriage, &blackBrush);
	carriage = map.toDevice(rightCarriage);
	dc.FillRect(&carriage, &hardwareBrush);
	dc.FrameRect(&carriage, &blackBrush);

	LOGBRUSH logbrush;
	background.GetLogBrush(&logbrush);
	dc.SetBkColor(logbrush.lbColor);

	// Draw block based on bounds
	NumericT top_y = std::max(bounds.maxLeft(), bounds.maxRight());
	NumericT bottom_y = std::min(bounds.minLeft(), bounds.minRight());
	RectT block(geometry->getBlockLeft(), top_y, geometry->getBlockRight(), bottom_y);
	RECT blockPhysical = map.toDevice(block);
	CBrush whiteBrush;
	whiteBrush.CreateSolidBrush(RGB(255,255,255));
	dc.FillRect(&blockPhysical, &whiteBrush);
	dc.FrameRect(&blockPhysical, &blackBrush);

	// Draw cut.  Don't show moves on the assumption they're just positioning.
	PlanformDrawDevice output(geometry, &dc, &map);
	for (auto it = cut->beginCutStructures(); it != cut->endCutStructures(); ++it) {
		CutStructure* pStruct = (*it);
		if (pStruct->getType() == PointCutter::TYPE) continue;
		pStruct->cut(&output, context);
	}



	PointT start(0,geometry->getXTravel() + 50); // 50 is half of top margin
	PointT finish(geometry->getWidth(), geometry->getXTravel() + 50); // 50 is half of top margin
	dimension(dc, map, start, finish);

	NumericT topBlock = std::fmax(bounds.maxLeft(), bounds.maxRight());
	PointT blockStart(geometry->getBlockLeft(), topBlock + 50);
	PointT blockEnd(geometry->getBlockRight(), topBlock + 50);
	dimension(dc, map, blockStart, blockEnd);

	PointT bottom(-carriageWidth/2, 0);
	PointT top(-carriageWidth / 2, geometry->getXTravel());
	dimension(dc, map, bottom, top);

	bottom.fx = top.fx = geometry->getWidth() + carriageWidth / 2;
	bottom.fy = 0;
	top.fy = top_y;
	dimension(dc, map, bottom, top);
}
