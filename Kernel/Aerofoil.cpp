/* Aerofoil
Aerofoil plotting and CNC cutter driver
Kernel / core algorithms
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
*/// Aerofoil.cpp: implementation of the CAerofoil class.
//
//////////////////////////////////////////////////////////////////////
#pragma warning(disable: 4786)

#include <assert.h>
#include <string>
#include <sstream>

#include "Aerofoil.h"
#include "Spline.h"
#include "KernelError.h"
#include "ObjectSerializer.h"

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAerofoil::CAerofoil(const char* path, NumericT tmod)
: spline(0)
, list(0)
, npts(0)
, thick_mod(tmod)
{
	read(path);
	createSpline();
}

CAerofoil::CAerofoil(const CAerofoil& rhs)
: spline(0)
, list(0)
, npts(0)
{
	copy(rhs);
}

CAerofoil::CAerofoil()
: spline(0)
, list(0)
, npts(0)
, thick_mod(1.0f)
{
}

CAerofoil::~CAerofoil()
{
	delete spline;
	delete [] list;
}

PointT CAerofoil::operator[](int idx) const
{
	assert(this);
	assert(idx > 0);
	assert(idx < npts);

	return PointT(list[idx].fx, list[idx].fy * thick_mod);

}

void CAerofoil::modifyThickness(NumericT tm)
{
	thick_mod = tm;
	createSpline();
}

std::string CAerofoil::getName() const
{
	if(thick_mod == 1.0f)
		return name;
	else // want to cook up a composite name that warns of modifier
	{
		ostringstream ost;
		ost << name << " mod: " << thick_mod * 100 << "%" << ends;
		return ost.str();
	}
}

void CAerofoil::read(const char *path)
{
  FILE *infp;
  char lnbuff[256];
  int n;
  int i;
  NumericT x,y;
  NumericT maxx;

  infp=fopen(path,"r");
  
  if(infp == NULL)
	  throw new KernelError(KernelError::IDS_ERR_UNABLE_TO_OPEN_AEROFOIL_FILE);

  string spath(path);
  section = spath;

  name.erase();
  
  n=0;      /* no points read in yet */
  maxx=1.0f;
  while(fgets(lnbuff,256,infp) != NULL)
    {
	if(n == 0 && name.empty()) // looking for name.
	{
		if(lnbuff[0] == ';')	// an initial comment
		{
			name = lnbuff+1;	// skip ; comment & get rest as name.
			
			// Erase trailing newlines
			string::size_type pos = name.find('\n');
			if(pos != string::npos)
				name.erase(pos);	
		}
			
	}

    /* strip comments */
    for(i=0;i<256;++i)
      if(lnbuff[i] == ';')
        {
        lnbuff[i]='\0';
        break;
        }

    if (strstr(lnbuff,"end")!=NULL) break;

	istringstream is(lnbuff);
	is >> x >> ws >> y;
	if (!is.fail()) {
		++n;    /* valid coordinate pair */
		if (x > maxx)
			maxx = x;
	}
  
  }


  /* Now that we know how many points there are in the file */
  /* we can malloc memory and reset the file pointer for   */
  /* a second pass.  */

  rewind(infp);
  delete [] list;
  list = new PointT[n];
  
  n=0;
  while(fgets(lnbuff,256,infp) != NULL)
    {
    /* strip comments */
    for(i=0;i<256;++i)
      if(lnbuff[i] == ';')
        {
        lnbuff[i]='\0';
        break;
        }

    if (strstr(lnbuff,"end")!=NULL) break;

	istringstream is(lnbuff);
	is >> x >> ws >> y;
	if(!is.fail())
      {
      list[n].fx = x/maxx;
      list[n].fy = y/maxx;
      ++n;    /* valid coordinate pair */
      }
    }

  fclose(infp);
  npts=n;

  if(name.empty())	// we didn't find a name - create from path.
  {
	  string::size_type start = section.find_last_of('\\');
	  if(start == string::npos)
		  start = 0;
	  else
		  ++start;	// skip path seperator.

	  string::size_type finish = section.find_last_of('.');
	  if(finish == string::npos)
		  finish = section.length();

	  // ok, we want the bit from start(inclusive) to finish(exclusive)
	  name = section.substr(start, finish - start);
  }

  createSpline();
}




PointT CAerofoil::Point(NumericT u) const
{
	assert(this);
	return spline->Point(u);
}

PointT CAerofoil::Point(NumericT u, PointT& tangent) const
{
	assert(this);
	return spline->Point(u,tangent);
}

NumericT CAerofoil::FirstX(NumericT req_x, NumericT start, int dirn) const
{
	assert(this);
	return spline->FirstX(req_x, start, dirn);
}

void CAerofoil::createSpline()
{
	assert(this);
	assert(list);
	assert(npts > 2);

	delete spline; spline = 0;

	if(thick_mod == 1.0f)
		spline = new CSpline(list,npts);
	else
	{
		PointT* thick_list = new PointT[npts];
		for(int i=0; i<npts; ++i)
			thick_list[i] = PointT(list[i].fx, list[i].fy * thick_mod);

		spline = new CSpline(thick_list,npts);
		delete [] thick_list;
	}
}

bool CAerofoil::operator == (const CAerofoil& rhs) const
{
	assert(this);
	return section == rhs.section && thick_mod == rhs.thick_mod;
}

CAerofoil& CAerofoil::operator = (const CAerofoil& rhs)
{
	if(this == &rhs) // self assignment
		return *this;

	delete [] list; list = 0;
	delete spline;  spline = 0;

	copy(rhs);

	return *this;
}

void CAerofoil::copy(const CAerofoil& rhs)
{

	npts = rhs.npts;
	list = new PointT[npts];
	for(int i=0; i<npts; ++i)
		list[i] = rhs.list[i];

	name = rhs.name;
	section = rhs.section;
	thick_mod = rhs.thick_mod;

	createSpline();
}

void CAerofoil::serializeTo(CObjectSerializer& os)
{
	assert(this);
	os.startSection("aerofoil", this);
	os.write("section",section.c_str());
	os.write("thicknessModifier", thick_mod);
	os.endSection();
}

void CAerofoil::serializeFrom(CObjectSerializer& os)
{
	assert(this);
	os.startReadSection("aerofoil", this);
	os.read("section",section);
	os.read("thicknessModifier", thick_mod);
	os.endReadSection();

	read(section.c_str());
	createSpline();
}

