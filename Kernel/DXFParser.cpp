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
#include <cmath>
#include <algorithm>
#include "trim.h"
#include "DXFParser.h" 
#include "StructureOutput.h"
#include "OutputDevice.h"
#include "ObjectSerializer.h"

DXFParser::DXFParser() {
	//ofs = new std::ofstream("E:/TEMP/dump.tmp");
}

DXFParser::~DXFParser() {
	//ofs->close();
	//delete ofs;
}

class Arc : public DXFItem {
	Coordinates centre;
	double r;
	double startAngle;
	double endAngle;
	public: Arc(): r(0), startAngle(0), endAngle(2 * pi) {}
	virtual void add(int code, const std::string& value);
	virtual DXFItem* clone();
	virtual void cut(StructureOutput * pOutput, COutputDevice * pdev);
	virtual void serializeTo(CObjectSerializer & os);
	virtual void serializeFrom(CObjectSerializer & os);

};

class Circle : public DXFItem {
	Coordinates centre;
	double r;
	public: Circle() : r(0) {}
	virtual void add(int code, const std::string& value);
	virtual DXFItem* clone();
	virtual void cut(StructureOutput * pOutput, COutputDevice * pdev);
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
	virtual void add(int code, const std::string& value);
	virtual DXFItem* clone();
	virtual void cut(StructureOutput * pOutput, COutputDevice * pdev);
	virtual void serializeTo(CObjectSerializer & os);
	virtual void serializeFrom(CObjectSerializer & os);
};

class Line : public DXFItem {
	Coordinates start;
	Coordinates end;
public:
	virtual void add(int code, const std::string& value);
	virtual DXFItem* clone();
	virtual void cut(StructureOutput * pOutput, COutputDevice * pdev);
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
	virtual void add(int code, const std::string& value);
	virtual DXFItem* clone();
	virtual void cut(StructureOutput * pOutput, COutputDevice * pdev);
	virtual void serializeTo(CObjectSerializer & os);
	virtual void serializeFrom(CObjectSerializer & os);
};

class Point : public DXFItem {
	Coordinates point;
public:
	virtual void add(int code, const std::string& value);
	virtual DXFItem* clone();
	virtual void cut(StructureOutput * pOutput, COutputDevice * pdev);
	virtual void serializeTo(CObjectSerializer & os);
	virtual void serializeFrom(CObjectSerializer & os);
};


static CObjectFactory<Arc> arcFactory("dxfArc");
static CObjectFactory<Circle> circleFactory("dxfCircle");
static CObjectFactory<Ellipse> ellipseFactory("dxfEllipse");
static CObjectFactory<Line> lineFactory("dxfLine");
static CObjectFactory<LWPolyLine> lWPolyLineFactory("dxfLWPolyLine");
static CObjectFactory<Point> pointFactory("dxfPoint");


void DXFItem::add(int code, const std::string& value) {
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


//  ARC
void Arc::add(int code, const std::string& value) {
	DXFItem::add(code, value);
	switch (code) {
	case 10: centre.x = std::stod(value);	break;
	case 20: centre.y = std::stod(value);	break;
	case 40: r = std::stod(value);	break;
	case 50: startAngle = std::stod(value); break;
	case 51: endAngle = std::stod(value); break;
	}
}

DXFItem* Arc::clone() { return new Arc(*this); }

void Arc::cut(StructureOutput * pOutput, COutputDevice * pdev){

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
	pOutput->move(pdev, pt, pt);

	for (double theta = startRadians + dt; theta < endRadians; theta += dt) {
		x = centre.x + r* cos(theta);
		y = centre.y + r* sin(theta);
		pt.fx = x;
		pt.fy = y;
		pOutput->line(pdev, pt, pt);
	}

	pt.fx = centre.x + r* cos(endRadians);
	pt.fy = centre.y + r* sin(endRadians);
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


// CIRCLE

void Circle::add(int code, const std::string& value) {
	DXFItem::add(code,value);
	switch (code) {
	case 10: centre.x = std::stod(value);	break;
	case 20: centre.y = std::stod(value);	break;
	case 40: r = std::stod(value);	break;
	}
}

DXFItem* Circle::clone() { return new Circle(*this); }

void Circle::cut(StructureOutput * pOutput, COutputDevice * pdev) {

	double circumference = 2 * pi * r;
	int steps = int(ceil(circumference)); // use 1mm lines (arbitrary)
	steps = std::max(steps, 16);  // and a minimum of 16 steps however short the lines.
	double dt = 2 * pi / steps;   // delta theta for a single step

	double x = centre.x + r;
	double y = centre.y;
	PointT pt(x, y);
	pOutput->move(pdev, pt, pt);

	for (double theta = dt; theta < 2 * pi; theta += dt) {
		x = centre.x + r* cos(theta);
		y = centre.y + r* sin(theta);
		pt.fx = x;
		pt.fy = y;
		pOutput->line(pdev, pt, pt);
	}

	pt.fx = centre.x + r;
	pt.fy = centre.y;
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


// ELLIPSE

void Ellipse::add(int code, const std::string& value) {
	DXFItem::add(code,value);
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

void Ellipse::cut(StructureOutput * pOutput, COutputDevice * pdev) {
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
	pOutput->move(pdev, pt, pt);

	for (double theta = startRadians + dt; theta < endRadians; theta += dt) {
		x = majorRadius * cos(theta);
		y = minorRadius * sin(theta);
		rx = centre.x + x*cr - y*sr; // rotate and offset
		ry = centre.y + y*cr + x*sr;
		pt.fx = rx;
		pt.fy = ry;
		pOutput->line(pdev, pt, pt);
	}

	x = majorRadius * cos(endRadians);
	y = minorRadius * sin(endRadians);
	rx = centre.x + x*cr - y*sr; // rotate and offset
	ry = centre.y + y*cr + x*sr;
	pt.fx = rx;
	pt.fy = ry;

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


// LINE

void Line::add(int code, const std::string& value) {
	DXFItem::add(code, value);
	switch (code) {
	case 10: start.x = std::stod(value);	break;
	case 20: start.y = std::stod(value);	break;
	case 11: end.x = std::stod(value);	break;
	case 21: end.y = std::stod(value);	break;
	}
}

DXFItem* Line::clone() { return new Line(*this); }

void Line::cut(StructureOutput * pOutput, COutputDevice * pdev) {
	PointT startPoint(start.x, start.y);
	PointT endPoint(end.x, end.y);
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


// LWPOLYLINE

void LWPolyLine::add(int code, const std::string& value) {
	DXFItem::add(code,value);
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

void LWPolyLine::cut(StructureOutput * pOutput, COutputDevice * pdev) {

	bool isFirst = true;
	for (std::vector<Coordinates>::iterator iter = vertices.begin();
		iter != vertices.end();
		++iter) {

		PointT p(iter->x, iter->y);
		if (isFirst) {
			pOutput->move(pdev, p, p);
			isFirst = false;
		} else {
			pOutput->line(pdev, p, p);
		}
	}

	if (isClosed) {
		PointT p(vertices.front().x, vertices.front().y);
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


// POINT

void Point::add(int code, const std::string& value) {
	DXFItem::add(code, value);
	switch (code) {
	case 10: point.x = std::stod(value);	break;
	case 20: point.y = std::stod(value);	break;
	}
}

DXFItem* Point::clone() { return new Point(*this); }

void Point::cut(StructureOutput * pOutput, COutputDevice * pdev) {
	PointT pt(point.x, point.y);
	pOutput->line(pdev, pt, pt);
}

class DXFItemFactory {
	typedef std::map<std::string, DXFItem*> PrototypeMap;
	PrototypeMap prototypes;
public:
	DXFItemFactory();
	DXFItem* create(const std::string& type);
};


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


static Arc arcPrototype;
static Circle circlePrototype;
static Ellipse ellipsePrototype;
static Line linePrototype;
static LWPolyLine lwPolyLinePrototype;
static Point pointPrototype;

DXFItemFactory::DXFItemFactory() {
	prototypes.insert(std::make_pair("ARC",&arcPrototype));
	prototypes.insert(std::make_pair("CIRCLE", &circlePrototype));
	prototypes.insert(std::make_pair("ELLIPSE", &ellipsePrototype));
	prototypes.insert(std::make_pair("LINE", &linePrototype));
	prototypes.insert(std::make_pair("LWPOLYLINE", &lwPolyLinePrototype));
	prototypes.insert(std::make_pair("POINT", &pointPrototype));
}

DXFItem* DXFItemFactory::create(const std::string& type) {
	PrototypeMap::iterator iter = prototypes.find(type);
	return (iter != prototypes.end()) ? iter->second->clone() : 0;
}

static DXFItemFactory factory;

class DXFSectionList {
	std::set<std::string> sections;
public:
	DXFSectionList();
	bool isRequiredSection(const std::string& name) {
		return sections.find(name) !=sections.end();
	}
};

/*
/* DXFSectionList determines whether a section should be processed. */
DXFSectionList::DXFSectionList() {
	// Uncomment the sections we want to process
	//sections.insert("HEADER");
	//sections.insert("CLASSES");
    //sections.insert("TABLES");
	//sections.insert("BLOCKS");
	sections.insert("ENTITIES"); 
	//sections.insert("OBJECTS");
}

static DXFSectionList sectionsToProcess;

static DXFParser::CodeT readCodes(std::istream& is) {
	std::string code;
	std::string value;
	std::getline(is, code);
	std::getline(is, value);
	trim(code);
	trim(value);
	return make_pair(code, value);
}

void DXFParser::readDxf(std::istream& is, DXFItemReceiver* pReceiver) {
	assert(this);
	assert(pReceiver);

	std::string currentSection; 
	std::string currentObj; 
	DXFItem* pCurrentObj = 0;
	CodeT codes = readCodes(is);

	while (codes.second != "EOF") {
		// Looking for a section 
		if (codes.first == "0" && codes.second == "SECTION") { 
			codes = readCodes(is);
			currentSection = codes.second;
			if (sectionsToProcess.isRequiredSection(currentSection)) {
				codes = readCodes(is);
				while (codes.second != "ENDSEC") {
					 if (codes.first == "0") {
						 currentObj = codes.second;
						 pCurrentObj = factory.create(currentObj);
						 if (pCurrentObj) {
							 pReceiver->add(pCurrentObj);
						 }
					 }

					// do something with code in section / object
					//(*ofs) << currentSection
					//<< " : " << currentObj << " --> " << codes.first << "=" <<
					//codes.second << std::endl;
					if (pCurrentObj) {
						pCurrentObj->add(std::stoi(codes.first), codes.second);
					}

					// Read another code/value pair
					codes = readCodes(is);
				}
			} else { // not interested so just hoover it up.
				codes = readCodes(is);
				while (codes.second != "ENDSEC") {
					codes = readCodes(is);
				}
			}
		} else { // no section, just keep going
					codes = readCodes(is);
		}
	} // while not EOF
}

