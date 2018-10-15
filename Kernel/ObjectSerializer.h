// ObjectSerializer.h: interface for the CObjectSerializer class.
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

///////////////////////////////////////////////////////////////////////
// Class for serialization exceptions
class CSerializeException : public exception
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
	void write(const char* name, float value);
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
	void read(const char* name, float& value);
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
	typedef	std::map<unsigned long, void*> OBJECT_MAP; 
	OBJECT_MAP objectMap;

	std::string savedTag;	// allow 1 tag lookahead

	std::iostream* ps;	
	
	unsigned long currentID;  // holds ID of current object when reading.
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
