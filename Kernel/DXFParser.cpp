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

/*
DXF Reference: https://images.autodesk.com/adsk/files/autocad_2012_pdf_dxf-reference_enu.pdf
*/

#include <assert.h>
#include <string> 
#include <utility> 
#include <vector> 
#include <map> 
#include <set>
#include <list>
#include <cmath>
#include <algorithm>
#include "trim.h"
#include "DXFParser.h" 
#include "StructureOutput.h"
#include "OutputDevice.h"
#include "ObjectSerializer.h"

const double pi = 3.14159265358979323846264338;

DXFParser::DXFParser() 
#ifdef DXF_DEBUG
	: ofs(0)
#endif
{
#if DXF_DEBUG
	ofs = new std::ofstream("dump.tmp");
#endif
}

DXFParser::~DXFParser() {
#if DXF_DEBUG
	ofs->close();
	delete ofs;
#endif
}

class Vertex;
class Polyline;

// ===========================================================================
// Context for DXF Parser
// ===========================================================================
class DXFParserContext {
	typedef std::map<std::string, DXFItem*> ItemMap;
	ItemMap itemLookup;
	DXFItem* currentItem;
	Polyline* currentPolyline;
public:
	DXFParserContext();
	void registerItem(const std::string& name, DXFItem* item);
	DXFItem* lookupItem(const std::string& name);
	void startItem(DXFItem* item);
	void addVertex(Vertex* vertex);
	void startPolyline(Polyline* polyline);


};

// ===========================================================================
// DXFTransform determines how to draw rotated/scaled/translated items such
// as blocks.
// ===========================================================================
class DXFTransform {
	Coordinates base;
	Coordinates location;
	Coordinates scale;
	double cosRotation; 
	double sinRotation;
	const DXFTransform* downstream;

public:
	DXFTransform();
	void setBase(Coordinates base) { this->base = base; }
	void setPosition(Coordinates location, Coordinates scale, double rotation);
	void setDownstream(const DXFTransform* downstream) { this->downstream = downstream; }
	void transform(PointT& pt) const;
};

// ===========================================================================
// Entity classes
// ===========================================================================

class Arc : public DXFItem {
	Coordinates centre;
	double r;
	double startAngle;
	double endAngle;
	public: Arc(): r(0), startAngle(0), endAngle(2 * pi) {}
	virtual void add(int code, const std::string& value, DXFParserContext* context);
	virtual DXFItem* clone();
	virtual void cut(StructureOutput * pOutput, COutputDevice * pdev, const DXFTransform* transform);
	virtual void serializeTo(CObjectSerializer & os);
	virtual void serializeFrom(CObjectSerializer & os);

};

class Circle : public DXFItem {
	Coordinates centre;
	double r;
	public: Circle() : r(0) {}
	virtual void add(int code, const std::string& value, DXFParserContext* context);
	virtual DXFItem* clone();
	virtual void cut(StructureOutput * pOutput, COutputDevice * pdev, const DXFTransform* transform);
	virtual void serializeTo(CObjectSerializer & os);
	virtual void serializeFrom(CObjectSerializer & os);
};

class Ellipse : public DXFItem {
	Coordinates centre;
	Coordinates masterAxis;
	double ratio;
	double start;
	double end;
	public: Ellipse() : ratio(1), start(0), end(360) {}
	virtual void add(int code, const std::string& value, DXFParserContext* context);
	virtual DXFItem* clone();
	virtual void cut(StructureOutput * pOutput, COutputDevice * pdev, const DXFTransform* transform);
	virtual void serializeTo(CObjectSerializer & os);
	virtual void serializeFrom(CObjectSerializer & os);
};

class Line : public DXFItem {
	Coordinates start;
	Coordinates end;
public:
	virtual void add(int code, const std::string& value, DXFParserContext* context);
	virtual DXFItem* clone();
	virtual void cut(StructureOutput * pOutput, COutputDevice * pdev, const DXFTransform* transform);
	virtual void serializeTo(CObjectSerializer & os);
	virtual void serializeFrom(CObjectSerializer & os);
};

class LWPolyLine : public DXFItem {
	int vertexCount;
	bool isClosed;
	std::vector<Coordinates> vertices;
	Coordinates p;		// build coords in multiple calls here
	bool hasX;			// if x coordinate set
	bool hasY;			// if y coordinate set
public:
	LWPolyLine() : vertexCount(0),isClosed(false), hasX(false), hasY(false) {}
	virtual void add(int code, const std::string& value, DXFParserContext* context);
	virtual DXFItem* clone();
	virtual void cut(StructureOutput * pOutput, COutputDevice * pdev, const DXFTransform* transform);
	virtual void serializeTo(CObjectSerializer & os);
	virtual void serializeFrom(CObjectSerializer & os);
};

class Point : public DXFItem {
	Coordinates point;
public:
	virtual void add(int code, const std::string& value, DXFParserContext* context);
	virtual DXFItem* clone();
	virtual void cut(StructureOutput * pOutput, COutputDevice * pdev, const DXFTransform* transform);
	virtual void serializeTo(CObjectSerializer & os);
	virtual void serializeFrom(CObjectSerializer & os);
};

class Block : public DXFItem {
	std::string name;
	Coordinates base;
	int flags;
	std::list<DXFItem*> items;
public:
	virtual ~Block();
	virtual void add(int code, const std::string& value, DXFParserContext* context);
	virtual DXFItem* clone();
	virtual void cut(StructureOutput* pOutput, COutputDevice* pdev, const DXFTransform* transform);
	virtual void serializeTo(CObjectSerializer& os);
	virtual void serializeFrom(CObjectSerializer& os);

	const std::string& getName() const { return name; }
	void addItem(DXFItem* item);
	void cutItem(StructureOutput* pOutput, COutputDevice* pdev, DXFTransform* transform);
};

class Insert : public DXFItem {
	std::string name;
	Coordinates insertionPoint;
	Coordinates scaleFactors;
	double rotationAngle;
	int columnCount;
	int rowCount;
	double columnSpacing;
	double rowSpacing;
	Block* blockRef;

public:
	Insert();
	virtual ~Insert();
	virtual void add(int code, const std::string& value, DXFParserContext* context);
	virtual DXFItem* clone();
	virtual void cut(StructureOutput* pOutput, COutputDevice* pdev, const DXFTransform* transform);
	virtual void serializeTo(CObjectSerializer& os);
	virtual void serializeFrom(CObjectSerializer& os);

	void setBlockRef(Block* block);
};

class Vertex : public DXFItem {
	Coordinates location;
	int flags;
public:
	bool isFitVertex() const { return (flags & 8) != 0; }
	bool isControlPoint() const { return (flags & 8) != 0; }

	Vertex();
	virtual ~Vertex();
	virtual void add(int code, const std::string& value, DXFParserContext* context);
	virtual DXFItem* clone();
	virtual bool onStart(DXFParserContext* context);
	virtual void cut(StructureOutput* pOutput, COutputDevice* pdev, const DXFTransform* transform);
	virtual void serializeTo(CObjectSerializer& os);
	virtual void serializeFrom(CObjectSerializer& os);

	const Coordinates& getLocation() const { return location; }
};

class Polyline : public DXFItem {
	enum LineType {
		unsmoothed = 0,
		quadraticBSpline = 5,
		cubicBSpline = 6,
		bezier = 8
	};

	std::list<Vertex*> vertices;
	int flags;

	LineType lineType;

	bool isClosed() { return (flags & 1) != 0; }
	bool isCurveFit() { return (flags & 2) != 0; }
	bool isSplineFit() { return (flags & 4) != 0; }

public:
	Polyline();
	virtual ~Polyline();
	virtual void add(int code, const std::string& value, DXFParserContext* context);
	virtual DXFItem* clone();
	virtual bool onStart(DXFParserContext* context);
	virtual void cut(StructureOutput* pOutput, COutputDevice* pdev, const DXFTransform* transform);
	virtual void serializeTo(CObjectSerializer& os);
	virtual void serializeFrom(CObjectSerializer& os);

	void addVertex(Vertex* vertex);
};



// ===========================================================================
// ===========================================================================
DXFTransform::DXFTransform() 
: base(0,0)
, location(0,0)
, scale(1,1)
, cosRotation(1) 
, sinRotation(0)
, downstream(0)
{

}
void DXFTransform::setPosition(Coordinates location, Coordinates scale, double rotation) {
	this->location = location;
	this->scale = scale;
	cosRotation = cos(rotation * pi / 180);
	sinRotation = sin(rotation * pi / 180);
}

void DXFTransform::transform(PointT& pt) const {
	// Convert to relative from block base
	double x = pt.fx - base.x;
	double y = pt.fy - base.y;

	// Scale around the base
	x *= scale.x;
	y *= scale.y;

	// Add any rotation
	double rx = x * cosRotation - y * sinRotation;
	double ry = x * sinRotation + y * cosRotation;

	// Translate to get proper location.
	pt.fx = location.x + rx;
	pt.fy = location.y + ry;

	// Allow chaining in case blocks are in blocks.
	if (downstream) {
		downstream->transform(pt);
	}
	return;
}


// ===========================================================================
// Object factories to deserialise the various DXF items.
// ===========================================================================
static CObjectFactory<Arc> arcFactory("dxfArc");
static CObjectFactory<Circle> circleFactory("dxfCircle");
static CObjectFactory<Ellipse> ellipseFactory("dxfEllipse");
static CObjectFactory<Line> lineFactory("dxfLine");
static CObjectFactory<LWPolyLine> lWPolyLineFactory("dxfLWPolyLine");
static CObjectFactory<Point> pointFactory("dxfPoint");
static CObjectFactory<Block> blockFactory("dxfBlock");
static CObjectFactory<Insert> insertFactory("dxfInsert");

// ===========================================================================
// Entity class definitions
// ===========================================================================

void DXFItem::add(int code, const std::string& value, DXFParserContext* context) {
	switch (code) {
	case 8: layer = value; break;
	}
}

void DXFItem::serializeTo(CObjectSerializer & os) {
	os.write("layer", layer.c_str());
}

void DXFItem::serializeFrom(CObjectSerializer & os) {
	os.read("layer", layer);
}


// ===========================================================================
//  ARC

void Arc::add(int code, const std::string& value, DXFParserContext* context) {
	DXFItem::add(code, value, context);
	switch (code) {
	case 10: centre.x = std::stod(value);	break;
	case 20: centre.y = std::stod(value);	break;
	case 40: r = std::stod(value);	break;
	case 50: startAngle = std::stod(value); break;
	case 51: endAngle = std::stod(value); break;
	}
}

DXFItem* Arc::clone() { return new Arc(*this); }

void Arc::cut(StructureOutput * pOutput, COutputDevice * pdev, const DXFTransform* transform){

	double circumference = 2 * pi * r; // of the circle overlaying the arc
	int steps = int(ceil(circumference)); // use 1mm lines (arbitrary)
	steps = std::max(steps, 16);  // and a minimum of 16 steps however short the lines.
	double dt = 2 * pi / steps;   // delta theta for a single step

	double startRadians = startAngle * pi / 180;
	double endRadians = endAngle * pi / 180;

	// sweep of angle may go through 360 degrees so...
	if (startRadians > endRadians) {
		endRadians += 2 * pi;
	}

	double x = centre.x + r* cos(startRadians);
	double y = centre.y + r* sin(startRadians);
	PointT pt(x, y);
	transform->transform(pt);
	pOutput->move(pdev, pt, pt);

	for (double theta = startRadians + dt; theta < endRadians; theta += dt) {
		x = centre.x + r* cos(theta);
		y = centre.y + r* sin(theta);
		pt.fx = x;
		pt.fy = y;
		transform->transform(pt);
		pOutput->line(pdev, pt, pt);
	}

	pt.fx = centre.x + r* cos(endRadians);
	pt.fy = centre.y + r* sin(endRadians);
	transform->transform(pt);
	pOutput->line(pdev, pt, pt);

}


void Arc::serializeTo(CObjectSerializer & os) {
	os.startSection("dxfArc", this);
	DXFItem::serializeTo(os);
	os.write("x", centre.x);
	os.write("y", centre.y);
	os.write("r", r);
	os.write("startAngle", startAngle);
	os.write("endAngle", endAngle);
	os.endSection();
}

void Arc::serializeFrom(CObjectSerializer & os) {
	os.startReadSection("dxfArc", this);
	DXFItem::serializeFrom(os);
	os.read("x", centre.x);
	os.read("y", centre.y);
	os.read("r", r);
	os.read("startAngle", startAngle);
	os.read("endAngle", endAngle);
	os.endReadSection();
}


// ===========================================================================
// CIRCLE

void Circle::add(int code, const std::string& value, DXFParserContext* context) {
	DXFItem::add(code,value, context);
	switch (code) {
	case 10: centre.x = std::stod(value);	break;
	case 20: centre.y = std::stod(value);	break;
	case 40: r = std::stod(value);	break;
	}
}

DXFItem* Circle::clone() { return new Circle(*this); }

void Circle::cut(StructureOutput * pOutput, COutputDevice * pdev, const DXFTransform* transform) {

	double circumference = 2 * pi * r;
	int steps = int(ceil(circumference)); // use 1mm lines (arbitrary)
	steps = std::max(steps, 16);  // and a minimum of 16 steps however short the lines.
	double dt = 2 * pi / steps;   // delta theta for a single step

	double x = centre.x + r;
	double y = centre.y;
	PointT pt(x, y);
	transform->transform(pt);
	pOutput->move(pdev, pt, pt);

	for (double theta = dt; theta < 2 * pi; theta += dt) {
		x = centre.x + r* cos(theta);
		y = centre.y + r* sin(theta);
		pt.fx = x;
		pt.fy = y;
		transform->transform(pt);
		pOutput->line(pdev, pt, pt);
	}

	pt.fx = centre.x + r;
	pt.fy = centre.y;
	transform->transform(pt);
	pOutput->line(pdev, pt, pt);
}


void Circle::serializeTo(CObjectSerializer & os) {
	os.startSection("dxfCircle", this);
	DXFItem::serializeTo(os);
	os.write("x", centre.x);
	os.write("y", centre.y);
	os.write("r", r);
	os.endSection();
}

void Circle::serializeFrom(CObjectSerializer & os) {
	os.startReadSection("dxfCircle", this);
	DXFItem::serializeFrom(os);
	os.read("x", centre.x);
	os.read("y", centre.y);
	os.read("r", r);
	os.endReadSection();
}


// ===========================================================================
// ELLIPSE

void Ellipse::add(int code, const std::string& value, DXFParserContext* context) {
	DXFItem::add(code,value, context);
	switch (code) {
	case 10: centre.x = std::stod(value);	break;
	case 20: centre.y = std::stod(value);	break;
	case 11: masterAxis.x = std::stod(value); break;
	case 21: masterAxis.y = std::stod(value);	break;
	case 40: ratio = std::stod(value);	break;
	case 41: start = std::stod(value);	break;
	case 42: end = std::stod(value);	break;
	}
}

DXFItem* Ellipse::clone() { return new Ellipse(*this); }

void Ellipse::cut(StructureOutput * pOutput, COutputDevice * pdev, const DXFTransform* transform) {
	double dx = masterAxis.x; // major axis a relative value.
	double dy = masterAxis.y;
	double majorRadius = sqrt(dx*dx + dy*dy);
	double minorRadius = majorRadius * ratio;

	// pick angle to step through based on equivalent circle
	double circumference = 2 * pi * majorRadius;
	int steps = int(ceil(circumference)); // use 1mm lines (arbitrary)
	steps = std::max(steps, 16);  // and a minimum of 16 steps however short the lines.
	double dt = 2 * pi / steps;   // delta theta for a single step

	double startRadians = start; // ellipse angles in radians
	double endRadians = end;

	// sweep of angle may go through 360 degrees so...
	if (startRadians > endRadians) {
		endRadians += 2 * pi;
	}

    // Work out rotation angle and useful factors for later.
	// x' = x.cosT − y.sinT
	// y' = y.cosT + x.sinT
	double rotate = atan2(dy, dx);  // angular offset as ellipse may be slanted
	double cr = cos(rotate);
	double sr = sin(rotate);

	// move to start point
	double x = majorRadius * cos(startRadians);
	double y = minorRadius * sin(startRadians);
	double rx = centre.x + x*cr - y*sr; // rotate and offset
	double ry = centre.y + y*cr + x*sr;

	PointT pt(rx, ry);
	transform->transform(pt);
	pOutput->move(pdev, pt, pt);

	for (double theta = startRadians + dt; theta < endRadians; theta += dt) {
		x = majorRadius * cos(theta);
		y = minorRadius * sin(theta);
		rx = centre.x + x*cr - y*sr; // rotate and offset
		ry = centre.y + y*cr + x*sr;
		pt.fx = rx;
		pt.fy = ry;
		transform->transform(pt);
		pOutput->line(pdev, pt, pt);
	}

	x = majorRadius * cos(endRadians);
	y = minorRadius * sin(endRadians);
	rx = centre.x + x*cr - y*sr; // rotate and offset
	ry = centre.y + y*cr + x*sr;
	pt.fx = rx;
	pt.fy = ry;

	transform->transform(pt);
	pOutput->line(pdev, pt, pt);
}


void Ellipse::serializeTo(CObjectSerializer & os) {
	os.startSection("dxfEllipse", this);
	DXFItem::serializeTo(os);
	os.write("x", centre.x);
	os.write("y", centre.y);
	os.write("mx", masterAxis.x);
	os.write("my", masterAxis.y);
	os.write("ratio", ratio);
	os.write("start", start);
	os.write("end", end);
	os.endSection();
}

void Ellipse::serializeFrom(CObjectSerializer & os) {
	os.startReadSection("dxfEllipse", this);
	DXFItem::serializeFrom(os);
	os.read("x", centre.x);
	os.read("y", centre.y);
	os.read("mx", masterAxis.x);
	os.read("my", masterAxis.y);
	os.read("ratio", ratio);
	os.read("start", start);
	os.read("end", end);
	os.endReadSection();
}


// ===========================================================================
// LINE

void Line::add(int code, const std::string& value, DXFParserContext* context) {
	DXFItem::add(code, value, context);
	switch (code) {
	case 10: start.x = std::stod(value);	break;
	case 20: start.y = std::stod(value);	break;
	case 11: end.x = std::stod(value);	break;
	case 21: end.y = std::stod(value);	break;
	}
}

DXFItem* Line::clone() { return new Line(*this); }

void Line::cut(StructureOutput * pOutput, COutputDevice * pdev, const DXFTransform* transform) {
	PointT startPoint(start.x, start.y);
	PointT endPoint(end.x, end.y);
	transform->transform(startPoint);
	transform->transform(endPoint);
	pOutput->move(pdev, startPoint, startPoint);
	pOutput->line(pdev, endPoint, endPoint);
}


void Line::serializeTo(CObjectSerializer & os) {
	os.startSection("dxfLine", this);
	DXFItem::serializeTo(os);
	os.write("sx", start.x);
	os.write("sy", start.y);
	os.write("ex", end.x);
	os.write("ey", end.y);
	os.endSection();
}

void Line::serializeFrom(CObjectSerializer & os) {
	os.startReadSection("dxfLine", this);
	DXFItem::serializeFrom(os);
	os.read("sx", start.x);
	os.read("sy", start.y);
	os.read("ex", end.x);
	os.read("ey", end.y);
	os.endReadSection();

}


// ===========================================================================
// LWPOLYLINE

void LWPolyLine::add(int code, const std::string& value, DXFParserContext* context) {
	DXFItem::add(code,value, context);
	int flags;

	switch (code) {
	case 70:
		flags = std::stoi(value);
		isClosed = (flags & 1) != 0;
		break;
	case 90:
		vertexCount = std::stoi(value);
		vertices.reserve(vertexCount);
		break;
	case 10:
		p.x = std::stod(value);
		hasX = true;
		if (hasX && hasY) {
			vertices.push_back(p);
			hasX = hasY = false;
			p.x = p.y = 0;
		}
		break;
	case 20:
		p.y = std::stod(value);
		hasY = true;
		if (hasX && hasY) {
			vertices.push_back(p);
			hasX = hasY = false;
			p.x = p.y = 0;
		}
		break;
	}
}

DXFItem* LWPolyLine::clone() { return new LWPolyLine(*this); }

void LWPolyLine::cut(StructureOutput * pOutput, COutputDevice * pdev, const DXFTransform* transform) {

	bool isFirst = true;
	for (std::vector<Coordinates>::iterator iter = vertices.begin();
		iter != vertices.end();
		++iter) {

		PointT p(iter->x, iter->y);
		transform->transform(p);
		if (isFirst) {
			pOutput->move(pdev, p, p);
			isFirst = false;
		} else {
			pOutput->line(pdev, p, p);
		}
	}

	if (isClosed) {
		PointT p(vertices.front().x, vertices.front().y);
		transform->transform(p);
		pOutput->line(pdev, p, p);
	}

}


void LWPolyLine::serializeTo(CObjectSerializer & os) {
	os.startSection("dxfLWPolyLine", this);
	DXFItem::serializeTo(os);
	os.write("closed", isClosed);
	os.startCollection("vertices", (int)vertices.size());
	for (std::vector<Coordinates>::iterator iter = vertices.begin();
		iter != vertices.end();
		++iter) {
		os.write("x", iter->x);
		os.write("y", iter->y);
	}
	os.endCollection();
	os.endSection();
}

void LWPolyLine::serializeFrom(CObjectSerializer & os) {
	os.startReadSection("dxfLWPolyLine", this);
	DXFItem::serializeFrom(os);
	if (os.ifExists("closed")) {
		os.read("closed", isClosed);
	}
	int vertexCount = os.startReadCollection("vertices");
	vertices.resize(vertexCount);
	for(int i=0; i<vertexCount; ++i){
		Coordinates c;
		os.read("x", c.x);
		os.read("y", c.y);
		vertices[i] = c;
	}
	os.endReadCollection();
	os.endReadSection();
}


// ===========================================================================
// POINT

void Point::add(int code, const std::string& value, DXFParserContext* context) {
	DXFItem::add(code, value, context);
	switch (code) {
	case 10: point.x = std::stod(value);	break;
	case 20: point.y = std::stod(value);	break;
	}
}

DXFItem* Point::clone() { return new Point(*this); }

void Point::cut(StructureOutput * pOutput, COutputDevice * pdev, const DXFTransform* transform) {
	PointT pt(point.x, point.y);
	transform->transform(pt);
	pOutput->line(pdev, pt, pt);
}


void Point::serializeTo(CObjectSerializer & os) {
	os.startSection("dxfPoint", this);
	DXFItem::serializeTo(os);
	os.write("x", point.x);
	os.write("y", point.y);
	os.endSection();
}

void Point::serializeFrom(CObjectSerializer & os) {
	os.startReadSection("dxfPoint", this);
	DXFItem::serializeFrom(os);
	os.read("x", point.x);
	os.read("y", point.y);
	os.endReadSection();
}


// ===========================================================================
// BLOCK

Block::~Block()
{
	for (auto iter = items.begin(); iter != items.end(); ++iter) {
		delete (*iter);
	}
	items.clear();
}

void Block::add(int code, const std::string& value, DXFParserContext* context)
{
	DXFItem::add(code, value, context);
	switch (code) {
	case 2: name = value;	break;
	case 10: base.x = std::stod(value);	break;
	case 20: base.y = std::stod(value);	break;
	//case 30: base.z = std::stod(value); break;
	case 70: flags = std::stoi(value);	break;
	}

	return;
}

DXFItem* Block::clone()
{
	assert(this);
	assert(items.empty()); // only do shallow clone for factory.
	return new Block(*this);
}

void Block::cut(StructureOutput* pOutput, COutputDevice* pdev, const DXFTransform* transform)
{
	// Blocks don't actually cut anything by themselves.
	//- cutItem does the cutting when called from an Insert
}

void Block::serializeTo(CObjectSerializer& os)
{
	os.startSection("dxfBlock", this);
	DXFItem::serializeTo(os);
	os.write("x", base.x);
	os.write("y", base.y);
	os.startCollection("items", (int)items.size());
	for (auto iter = items.begin(); iter != items.end(); ++iter) {
		(*iter)->serializeTo(os);
	}
	os.endCollection();
	os.endSection();
}

void Block::serializeFrom(CObjectSerializer& os)
{
	os.startReadSection("dxfBlock", this);
	DXFItem::serializeFrom(os);
	os.read("x", base.x);
	os.read("y", base.y);
	int itemCount = os.startReadCollection("items");
	for (int i = 0; i < itemCount; ++i) {
		DXFItem* pItem = static_cast<DXFItem*>(os.createSubtype());
		pItem->serializeFrom(os);
		addItem(pItem);
	}
	os.endReadCollection();
	os.endReadSection();

}

void Block::addItem(DXFItem* item)
{
	assert(this);
	assert(item);
	items.push_back(item);
}

void Block::cutItem(StructureOutput* pOutput, COutputDevice* pdev, DXFTransform* transform)
{
	transform->setBase(base);
	for (auto iter = items.begin(); iter != items.end(); ++iter) {
		(*iter)->cut(pOutput, pdev, transform); 
	}
}

// ===========================================================================
// INSERT

Insert::Insert()
	: blockRef(0)
	, scaleFactors(1,1)
	, rotationAngle(0)
	, columnCount(1)
	, rowCount(1)
	, columnSpacing(0)
	, rowSpacing(0)
{
}

Insert::~Insert()
{
	blockRef = 0;
}

void Insert::add(int code, const std::string& value, DXFParserContext* context)
{
	DXFItem::add(code, value, context);
	switch (code) {
	case 2: {
		name = value;
		blockRef = static_cast<Block*>(context->lookupItem(name));
		assert(blockRef);
	}
	break;

	case 10: insertionPoint.x = std::stod(value);	break;
	case 20: insertionPoint.y = std::stod(value);	break;
	//case 30: insertionPoint.z = std::stod(value); break;
	case 41: scaleFactors.x = std::stod(value);	break;
	case 42: scaleFactors.y = std::stod(value);	break;
    //case 43: scaleFactors.z = std::stod(value); break;
	
	case 44: columnSpacing = std::stod(value);	break;
	case 45: rowSpacing = std::stod(value);	break;

	case 50: rotationAngle = std::stod(value);	break;

	case 70: columnCount = std::stoi(value);	break;
	case 71: rowCount = std::stoi(value);	break;
	}
}

DXFItem* Insert::clone()
{
	return new Insert(*this);
}

void Insert::cut(StructureOutput* pOutput, COutputDevice* pdev, const DXFTransform* transform)
{
	assert(this);
	assert(blockRef);
	for (int ic = 0; ic < columnCount; ++ic) {
		for (int ir = 0; ir < rowCount; ++ir) {
			Coordinates here = insertionPoint;
			here.x += ic * columnSpacing;
			here.y += ir * rowSpacing;
			DXFTransform t;
			t.setPosition(here, scaleFactors, rotationAngle);
			t.setDownstream(transform);
			blockRef->cutItem(pOutput, pdev, &t);
		}
	}
}

void Insert::serializeTo(CObjectSerializer& os)
{
	os.startSection("dxfInsert", this);
	DXFItem::serializeTo(os);
	os.write("name", name.c_str());
	os.write("x", insertionPoint.x);
	os.write("y", insertionPoint.y);
	os.write("scalex", scaleFactors.x);
	os.write("scaley", scaleFactors.y);
	os.write("rotation", rotationAngle);
	os.write("columnCount", columnCount);
	os.write("rowCount", rowCount);
	os.write("columnSpacing", columnSpacing);
	os.write("rowSpacing", rowSpacing);
	os.writeReference("blockRef", blockRef);
	os.endSection();
}

void Insert::serializeFrom(CObjectSerializer& os)
{
	os.startReadSection("dxfInsert", this);
	DXFItem::serializeFrom(os);
	os.read("name", name);
	os.read("x", insertionPoint.x);
	os.read("y", insertionPoint.y);
	os.read("scalex", scaleFactors.x);
	os.read("scaley", scaleFactors.y);
	os.read("rotation", rotationAngle);
	os.read("columnCount", columnCount);
	os.read("rowCount", rowCount);
	os.read("columnSpacing", columnSpacing);
	os.read("rowSpacing", rowSpacing);
	blockRef = static_cast<Block*>(os.readReference("blockRef"));
	os.endReadSection();
}

void Insert::setBlockRef(Block* block)
{
	assert(this);
	assert(block);
	blockRef = block;
}

// ===========================================================================
// VERTEX


Vertex::Vertex()
	: flags(0)
{
}

Vertex::~Vertex()
{
}

void Vertex::add(int code, const std::string& value, DXFParserContext* context)
{
	DXFItem::add(code, value, context);
	switch (code) {
	case 10: location.x = atof(value.c_str()); break;
	case 20: location.y = atof(value.c_str()); break;
		//case 30: location.z = atoi(value.c_str()); break;
	case 70: flags = atoi(value.c_str()); break;
	}
}

DXFItem* Vertex::clone()
{
	return new Vertex(*this);
}

bool Vertex::onStart(DXFParserContext* context)
{
	context->addVertex(this);
	return false; // don't store directly
}

void Vertex::cut(StructureOutput* pOutput, COutputDevice* pdev, const DXFTransform* transform)
{
}

void Vertex::serializeTo(CObjectSerializer& os)
{
	os.startSection("dxfVertex", this);
	DXFItem::serializeTo(os);
	os.write("flags", flags);
	os.write("x", location.x);
	os.write("y", location.y);
	os.endSection();
}

void Vertex::serializeFrom(CObjectSerializer& os)
{
	os.startReadSection("dxfVertex", this);
	DXFItem::serializeFrom(os);
	os.read("flags", flags);
	os.read("x", location.x);
	os.read("y", location.y);
	os.endReadSection();
}

// ===========================================================================
// POLYLINE


Polyline::Polyline()
	: flags(0)
{
}

Polyline::~Polyline()
{
	for (auto iter = vertices.begin(); iter != vertices.end(); ++iter) {
		delete(*iter);
	}
}

void Polyline::add(int code, const std::string& value, DXFParserContext* context)
{
	DXFItem::add(code, value, context);
	switch (code) {
	case 70: flags = atoi(value.c_str()); break;
	}
}

DXFItem* Polyline::clone()
{
	assert(this);
	assert(vertices.empty());
	return new Polyline(*this);
}

bool Polyline::onStart(DXFParserContext* context)
{
	context->startPolyline(this);
	return true;
}

// Use any fit vertices to draw an approximation. Ignore control points
void Polyline::cut(StructureOutput* pOutput, COutputDevice* pdev, const DXFTransform* transform)
{
	bool isFirst = true;
	PointT firstPoint;
	for(auto iter = vertices.begin();iter != vertices.end(); ++iter){
		Vertex* v = *iter;
		if (v->isFitVertex()) {
			PointT pt(v->getLocation().x, v->getLocation().y);
			transform->transform(pt);
			if (isFirst) {
				pOutput->move(pdev, pt, pt);
				firstPoint = pt;
				isFirst = false;
			}
			else {
				pOutput->line(pdev, pt, pt);
			}
		}
	}
	if (isClosed() && !isFirst) {
		pOutput->line(pdev, firstPoint, firstPoint);
	}
}

void Polyline::serializeTo(CObjectSerializer& os)
{
	os.startSection("dxfPolyLine", this);
	DXFItem::serializeTo(os);
	os.write("flags", flags);
	os.write("lineType", lineType);
	os.startCollection("vertices", (int)vertices.size());
	for (auto iter = vertices.begin(); iter != vertices.end(); ++iter) {
		(*iter)->serializeTo(os);
	}
	os.endCollection();
	os.endSection();
}


void Polyline::serializeFrom(CObjectSerializer& os)
{
	os.startReadSection("dxfPolyLine", this);
	DXFItem::serializeFrom(os);
	os.read("flags", flags);
	int lt;
	os.read("lineType", lt);
	lineType = static_cast<LineType>(lt);
	int count = os.startReadCollection("vertices");
	for (int i = 0; i < count; ++i) {
		Vertex* v = static_cast<Vertex*>(os.createSubtype());
		v->serializeFrom(os);
		addVertex(v);
	}
	os.endReadCollection();
	os.endReadSection();

}

void Polyline::addVertex(Vertex* vertex)
{
	assert(this);
	assert(vertex);
	vertices.push_back(vertex);
}


// ===========================================================================
// DXFParserContext implementation
// ===========================================================================


DXFParserContext::DXFParserContext()
	: currentItem(0)
	, currentPolyline(0)
{
}

void DXFParserContext::registerItem(const std::string& name, DXFItem* item) {
	itemLookup.insert(std::make_pair(name, item));
}
DXFItem* DXFParserContext::lookupItem(const std::string& name) {
	auto iter = itemLookup.find(name);
	if (iter != itemLookup.end()) {
		return iter->second;
	}
	assert(false);
	return 0;
}

void DXFParserContext::startItem(DXFItem* item)
{
	currentItem = item;
}

void DXFParserContext::addVertex(Vertex* vertex)
{
	if (currentPolyline) {
		currentPolyline->addVertex(vertex);
	}
}

void DXFParserContext::startPolyline(Polyline* polyline)
{
	assert(this);
	assert(polyline);
	currentPolyline = polyline;
}


//================================================================================
// DXFItemFactory
// By using a set of prototype objects this knows how to create a DXFItem object given
// the name of an object.
//================================================================================
class DXFItemFactory {
	typedef std::map<std::string, DXFItem*> PrototypeMap;
	PrototypeMap prototypes;
public:
	DXFItemFactory();
	DXFItem* create(const std::string& type);
};

static Arc arcPrototype;
static Circle circlePrototype;
static Ellipse ellipsePrototype;
static Line linePrototype;
static LWPolyLine lwPolyLinePrototype;
static Point pointPrototype;
static Insert insertPrototype;
static Polyline polyLinePrototype;
static Vertex vertexPrototype;

DXFItemFactory::DXFItemFactory() {
	prototypes.insert(std::make_pair("ARC",&arcPrototype));
	prototypes.insert(std::make_pair("CIRCLE", &circlePrototype));
	prototypes.insert(std::make_pair("ELLIPSE", &ellipsePrototype));
	prototypes.insert(std::make_pair("LINE", &linePrototype));
	prototypes.insert(std::make_pair("LWPOLYLINE", &lwPolyLinePrototype));
	prototypes.insert(std::make_pair("POINT", &pointPrototype));
	prototypes.insert(std::make_pair("INSERT", &insertPrototype));
	prototypes.insert(std::make_pair("POLYLINE", &polyLinePrototype));
	prototypes.insert(std::make_pair("VERTEX", &vertexPrototype));
}

DXFItem* DXFItemFactory::create(const std::string& type) {
	PrototypeMap::iterator iter = prototypes.find(type);
	return (iter != prototypes.end()) ? iter->second->clone() : 0;
}

static DXFItemFactory factory;

// ===========================================================================
// Helper function to read a code & value.
// ===========================================================================
static DXFParser::CodeT readCodes(std::istream& is) {
	std::string code;
	std::string value;
	std::getline(is, code);
	std::getline(is, value);
	trim(code);
	trim(value);
	return make_pair(code, value);
}


//================================================================================
// SectionReader
// SectionReader and its derived types know to to read the section of a DXF file.
// Use IgnoreReader for any sections we don't care about.
//================================================================================


class SectionReader {
public:
	virtual void read(std::istream& is, DXFItemReceiver* pReceiver, DXFParserContext* pContext) = 0;
};

class IgnoreReader : public SectionReader{
	virtual void read(std::istream& is, DXFItemReceiver* pReceiver, DXFParserContext* pContext);
};

class EntityReader : public SectionReader {
	virtual void read(std::istream& is, DXFItemReceiver* pReceiver, DXFParserContext* pContext);
};

class BlockReader : public SectionReader {
	virtual void read(std::istream& is, DXFItemReceiver* pReceiver, DXFParserContext* pContext);
};

void IgnoreReader::read(std::istream& is, DXFItemReceiver* pReceiver, DXFParserContext* pContext) {
	DXFParser::CodeT codes = readCodes(is);
	while (codes.second != "ENDSEC") {
		codes = readCodes(is);
	}
}

void EntityReader::read(std::istream& is, DXFItemReceiver* pReceiver, DXFParserContext* pContext) {
	std::string currentSection;
	std::string currentObj;
	DXFItem* pCurrentObj = 0;
	DXFParser::CodeT codes = readCodes(is);
	while (codes.second != "ENDSEC") {
		if (codes.first == "0") {
			currentObj = codes.second; // e.g. ARC, POINT, LINE, LWPOLYLINE
			pCurrentObj = factory.create(currentObj);
			if (pCurrentObj) {
				if (pCurrentObj->onStart(pContext)) {
					pReceiver->add(pCurrentObj);
				}
				
			}
		}

		// do something with code in section / object
		if (pCurrentObj) {
			pCurrentObj->add(std::stoi(codes.first), codes.second, pContext);
		}

		// Read another code/value pair
		codes = readCodes(is);
	}

}

void BlockReader::read(std::istream& is, DXFItemReceiver* pReceiver, DXFParserContext* pContext) {

	Block* pBlock = 0;					// current block (if any)
	DXFItem* pCurrentObj = 0;			// current item in block

	DXFParser::CodeT codes = readCodes(is);
	while (codes.second != "ENDSEC") {
		if (codes.first == "0") {
			std::string typeTag = codes.second; // BLOCK, ENDBLK or contents
			if (typeTag == "BLOCK") {
				pBlock = new Block();
				pCurrentObj = 0; // so read block info
			}
			else if (typeTag == "ENDBLK") {
				assert(pBlock);
				if (pBlock) {
					pContext->registerItem(pBlock->getName(), pBlock);
					pReceiver->addSharedItem(pBlock);
				}
				pBlock = 0;
			}
			else { // it's content such as ARC, LINE, LWPOLYLINE etc
				pCurrentObj = factory.create(typeTag);
				if (pCurrentObj) {
					pBlock->addItem(pCurrentObj);
				}
			}
		}
		else {
			if (pCurrentObj) {
				pCurrentObj->add(std::stoi(codes.first), codes.second, pContext);
			} else if (pBlock) {
				pBlock->add(std::stoi(codes.first), codes.second, pContext);
			}
		}
		// Read another code/value pair
		codes = readCodes(is);
	}
}

// ===========================================================================
// DXFSectionList determines whether & how a section should be processed.
// By keeping a map of readers by section name it can find the appropriate
// reader for a given section name
// ===========================================================================

static IgnoreReader ignoreReader;
static EntityReader entityReader;
static BlockReader blockReader;

class DXFSectionList {
	typedef std::map<std::string, SectionReader*> SectionMap;
	SectionMap sections;

public:
	DXFSectionList();
	bool isRequiredSection(const std::string& name) {
		return sections.find(name) != sections.end();
	}
	SectionReader* getReader(const std::string& name);
};

// Given the name of a section, get the appropriate reader for that section
// If not found, return the ignore reader which just hoovers up and discards
// that section.
SectionReader* DXFSectionList::getReader(const std::string& name)
{
	SectionReader* reader = &ignoreReader;
	SectionMap::iterator iter = sections.find(name);
	if (iter != sections.end()) {
		reader = iter->second;
	}
	return reader;
}

DXFSectionList::DXFSectionList() {
	// Uncomment the sections we want to process
	//sections.insert("HEADER");
	//sections.insert("CLASSES");
	//sections.insert("TABLES");
	sections.insert(std::make_pair("BLOCKS",&blockReader));
	sections.insert(std::make_pair("ENTITIES",&entityReader));
	//sections.insert("OBJECTS");
}


static DXFSectionList sectionsToProcess;




// ===========================================================================
// Core DXF Parser, albeit most of the heavy lifting is done by the classes above.
// ===========================================================================

void DXFParser::readDxf(std::istream& is, DXFItemReceiver* pReceiver) {
	assert(this);
	assert(pReceiver);

	DXFParserContext context;

	std::string currentSection; 
	SectionReader* sectionReader;

	CodeT codes = readCodes(is);
	while (codes.second != "EOF") {
		// Looking for a section 
		if (codes.first == "0" && codes.second == "SECTION") { 
			codes = readCodes(is);					//section name
			currentSection = codes.second;			// e.g. HEADER, BLOCKS, ENTITIES
			sectionReader = sectionsToProcess.getReader(currentSection);
			sectionReader->read(is, pReceiver, &context);
		} else { // no section, just keep going
			codes = readCodes(is);
		}
	} // while not EOF
}

const static DXFTransform noOpTransform;

const DXFTransform* DXFParser::noOpTransform()
{
	return &::noOpTransform;
}


