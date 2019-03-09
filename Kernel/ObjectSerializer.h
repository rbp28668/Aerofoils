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
*/// ObjectSerializer.h: interface for the CObjectSerializer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(OBJECTSERIALIZER_H_INC)
#define OBJECTSERIALIZER_H_INC

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _MSC_VER
#pragma warning(disable: 4786) // id truncated to 255 chars in debug
#endif

#include <string>
#include <stack>
#include <map>
#include <iostream>
#include <exception>

///////////////////////////////////////////////////////////////////////
// Class for serialization exceptions
class CSerializeException : public std::exception
{
public:
	CSerializeException(const char* msg) : errMsg(msg) {}
    virtual const char *what() const throw();

private:
	std::string errMsg;
};


///////////////////////////////////////////////////////////////////////
// Base class for all concrete factories. Used by CObjectFactory template.
class CObjectFactoryBase
{
public:
	virtual void* create() = 0;
};

///////////////////////////////////////////////////////////////////////
// Main serialization support
class CObjectSerializer  
{
public:
	CObjectSerializer(std::iostream* pstream);
	~CObjectSerializer();

	void startSection(const char* pszName, const void* pObject);
	void endSection();

	void startCollection(const char* pszName, int count);
	void endCollection();

	void startOptional(const char* pszName);
	void endOptional();

	void write(const char* name, int value);
	void write(const char* name, unsigned int value);
	void write(const char* name, float value);
	void write(const char* name, double value);
	void write(const char* name, const char* text);
	void write(const char* name, bool value);
	void writeReference(const char* name, const void* pObject);

	void startReadSection(const char* pszName, void* pObject);
	void endReadSection();

	int startReadCollection(const char* pszName);
	void endReadCollection();

	bool ifOptional(const char* pszName);
	void endIfOptional();

	bool ifExists(const char* pszName);

	void read(const char* name, int& value);
	void read(const char* name, unsigned int& value);
	void read(const char* name, float& value);
	void read(const char* name, double& value);
	void read(const char* name, std::string& text);
	void read(const char* name, bool& value);
	void* readReference(const char* name);
	void* createSubtype();	// from next tag.

	// Singleton abstract class factory
	class CAbstractClassFactory
	{
	public:
		void* create(const char* className);
		void addFactory(CObjectFactoryBase* factory, const char* className);

		static CAbstractClassFactory& getFactory();
		static void closeFactory();

	private:
		typedef std::map<std::string, CObjectFactoryBase*> FACTORIES;
		FACTORIES factories;
		static CAbstractClassFactory* _theFactory;
	};

	private:

	std::string nextTag();
	std::string startTag();
	void endTag();
	void pushBackTag(const std::string& element);

	std::stack<std::string> sectionNames;	// to match start and end tags	

	// map objects by their IDs for resolving references
	typedef	std::map<uintptr_t, void*> OBJECT_MAP; 
	OBJECT_MAP objectMap;

	std::string savedTag;	// allow 1 tag lookahead

	std::iostream* ps;	
	
	uintptr_t currentID;  // holds ID of current object when reading.
};

///////////////////////////////////////////////////////////////////////
// Template for creating concrete object factories.
template <typename T> class CObjectFactory: public CObjectFactoryBase
{
public:
	CObjectFactory<T>(const char* className) 
	{
		CObjectSerializer::CAbstractClassFactory::getFactory().addFactory(this, className);
	}

	void* create() { return new T(); }
};

#endif // !defined(OBJECTSERIALIZER_H_INC)
