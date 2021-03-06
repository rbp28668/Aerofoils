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
#include <utility>
#include <vector>
#include <istream>
#include <fstream>

#define DXF_DEBUG 1

class StructureOutput;
class COutputDevice;
class CObjectSerializer;

class DXFParserContext;
class DXFTransform;

struct Coordinates {
	double x;
	double y;
	Coordinates(double x = 0, double y=0) : x(x), y(y) {}
};

// Items generated by the parser.
class DXFItem 
{
	std::string layer;
protected:
	const double pi = 3.14159265358979323846;
public:
	virtual ~DXFItem() {}
	virtual void add(int code, const std::string& value, DXFParserContext* context) = 0;
	virtual DXFItem* clone() = 0;
	virtual bool onStart(DXFParserContext* context) { return true; } // return false to not store this item directly
	virtual void cut(StructureOutput * pCut, COutputDevice * pdev, const DXFTransform* transform) = 0;
	virtual void serializeTo(CObjectSerializer & os);
	virtual void serializeFrom(CObjectSerializer & os);

	bool isConstruction() const;
	static bool itemIsConstruction(const DXFItem* item) { return item->isConstruction(); }
};

// Abstract class to be implemented by anything that uses the parser to 
// receive DXFItems.
class DXFItemReceiver {
public:
	virtual void add(DXFItem* item) = 0;
	virtual void addSharedItem(DXFItem* item) = 0;
};

// Basic DFX parser that will read
// ARC,
// CIRCLE,
// ELLIPSE,
// LINE,
// LWPOLYLINE
// POINT.
// BLOCK & INSERT
class DXFParser
{
#if DXF_DEBUG
	std::ofstream* ofs;
#endif

public:
	DXFParser();
	~DXFParser();

	typedef std::pair<std::string, std::string> CodeT;
	void readDxf(std::istream& is, DXFItemReceiver* pReceiver);
	static const DXFTransform* noOpTransform(); 
};
