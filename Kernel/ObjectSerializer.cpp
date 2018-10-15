// ObjectSerializer.cpp: implementation of the CObjectSerializer class.
//
//////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
#pragma warning(disable: 4786 4710)
#endif

#include <assert.h>
#include "ObjectSerializer.h"

using namespace std;

///////////////////////////////////////////////////////////////////////
// CSerializeException
const char* CSerializeException::what() const throw()
{
	return errMsg.c_str();
}


///////////////////////////////////////////////////////////////////////
// CObjectSerializer::CAbstractClassFactory

// singleton - will be initialised to 0 (see C++ PL2 r3.4)
CObjectSerializer::CAbstractClassFactory* CObjectSerializer::CAbstractClassFactory::_theFactory;

// Singleton accessor
CObjectSerializer::CAbstractClassFactory& CObjectSerializer::CAbstractClassFactory::getFactory()
{
	if(_theFactory == 0)
		_theFactory = new CAbstractClassFactory();
	return *_theFactory;
}

// Final tidy up to prevent memory leaks.
void CObjectSerializer::CAbstractClassFactory::closeFactory()
{
	delete _theFactory;
	_theFactory = 0;
}

// allows concrete class factories to register themselves with the singleton abstract factory.
void CObjectSerializer::CAbstractClassFactory::addFactory(CObjectFactoryBase* factory, const char* className)
{
	factories.insert(FACTORIES::value_type(className,factory));
}

// create an object corresponding to a given class name.
void* CObjectSerializer::CAbstractClassFactory::create(const char* className)
{
	CObjectFactoryBase* pof = 0;
	FACTORIES::iterator iter = factories.find(className);
	if(iter != factories.end())
	{
		pof = iter->second;
	}
	else // object isn't in the map
	{
		throw CSerializeException("Unknown class name during de-serialization");
	}

	return pof->create();	// 

}


//////////////////////////////////////////////////////////////////////
// CObjectSerializer

// create a serializer tied to a stream.
CObjectSerializer::CObjectSerializer(iostream* pstream)
: ps(pstream)
{
	assert(pstream);
}

CObjectSerializer::~CObjectSerializer()
{

}

// start serialization of an object
void CObjectSerializer::startSection(const char* pszName, const void* pObject)
{
	assert(this);
	*ps << "<" << pszName << ">" << endl;
	*ps << "<id>" << (unsigned long)pObject << "</id>" << endl;
	sectionNames.push(pszName);
}

// end serialization of an object
void CObjectSerializer::endSection()
{
	assert(this);
	string name = sectionNames.top(); 
	sectionNames.pop();
	*ps << "</" << name << ">" << endl;
}

// start serialization of a collection of objects
void CObjectSerializer::startCollection(const char* pszName, int count)
{
	assert(this);
	*ps << "<" << pszName << ">" << endl;
	*ps << "<count>" << count << "</count>" << endl;
	sectionNames.push(pszName);
}

// end serialization of a collection of objects
void CObjectSerializer::endCollection()
{
	assert(this);
	endSection();

}

// start serialization of an optional object
void CObjectSerializer::startOptional(const char* pszName)
{
	assert(this);
	assert(pszName);
	*ps << "<" << pszName << ">" << endl;
	sectionNames.push(pszName);
}

// end serialization of an optional object
void CObjectSerializer::endOptional()
{
	assert(this);
	endSection();
}

// write an integer
void CObjectSerializer::write(const char* name, int value)
{
	assert(this);
	*ps << "<" << name << ">" << value << "</" << name << ">" << endl;
}

// write a float
void CObjectSerializer::write(const char* name, float value)
{
	assert(this);
	*ps << "<" << name << ">" << value << "</" << name << ">" << endl;
}

// write a string
void CObjectSerializer::write(const char* name, const char* text)
{
	assert(this);
	*ps << "<" << name << ">" << text << "</" << name << ">" << endl;
}

// write a boolean
void CObjectSerializer::write(const char* name, bool value)
{
	assert(this);
	*ps << "<" << name << ">" << value << "</" << name << ">" << endl;
}

// write a reference or pointer to another object
void CObjectSerializer::writeReference(const char* name, const void* pObject)
{
	assert(this);
	*ps << "<" << name << ">" << (unsigned long)pObject << "</" << name << ">" << endl;
}

// read an integer
void CObjectSerializer::read(const char* name, int& value)
{
	assert(this);
	string element = startTag();
	if(element.compare(name) != 0)	
	{
		throw CSerializeException("Serialization - name mismatch reading integer");
	}
	*ps >> value;
	endTag();
}

// read a float
void CObjectSerializer::read(const char* name, float& value)
{
	assert(this);
	string element = startTag();
	if(element.compare(name) != 0)	
	{
		assert(false);
		throw CSerializeException("Serialization - name mismatch reading float");
	}
	*ps >> value;
	endTag();
}

// read a string
void CObjectSerializer::read(const char* name, std::string& text)
{
	assert(this);

	string element = startTag();
	if(element.compare(name) != 0)	
	{
		throw CSerializeException("Serialization - name mismatch reading string");
	}
	text = "";

	char ch;
	*ps >> ch; // will eat whitespace
	while(ch != '<')
	{
		text += ch;
		ch = ps->get(); // includes whitespace
	}
	ps->putback(ch);

	endTag();
}

// read a boolean
void CObjectSerializer::read(const char* name, bool& value)
{
	assert(this);
	assert(name);

	string element = startTag();
	if(element.compare(name) != 0)	
	{
		throw CSerializeException("Serialization - name mismatch reading boolean");
	}
	*ps >> value;
	endTag();
}

// read a reference to another object.  This other object must already be in memory and
// have been deserialized including a call to startReadSection
void* CObjectSerializer::readReference(const char* name)
{
	assert(this);
	assert(name);

	string element = startTag();
	if(element.compare(name) != 0)
	{
		assert(false);
		throw CSerializeException("Serialization - name mismatch reading reference");
	}

	unsigned long id;
	*ps >> id;

	void* pObject = 0;
	OBJECT_MAP::iterator iter = objectMap.find(id);
	if(iter != objectMap.end())
	{
		pObject = iter->second;
	}
	else // object isn't in the map
	{
		assert(false);
		throw CSerializeException("Serialization - can't find object for reference");
	}

	endTag();

	return pObject;
}

// Uses the next element in the input stream to create a new object
// of the appropriate type.  Used when we are setting up a collection
// of polymorphic types where the correct type needs to be allocated.
void* CObjectSerializer::createSubtype()
{
	assert(this);

	string element = nextTag();
	pushBackTag(element);

	CAbstractClassFactory& factory = CAbstractClassFactory::getFactory();
	void* pObject = factory.create(element.c_str());
	return pObject;
}

// Allows optional sections - only call corresponding endIfOptional if
// this returns true
bool CObjectSerializer::ifOptional(const char* name)
{
	assert(this);
	string element = nextTag();
	bool isTag = element.compare(name) == 0;
	if(isTag)
		sectionNames.push(element);
	else
		pushBackTag(element); // as will be start of next element
	return isTag;
}

// Allows 1 element lookahead for conditional reading
bool CObjectSerializer::ifExists(const char* name)
{
	assert(this);
	string element = nextTag();
	bool isTag = element.compare(name) == 0;
	pushBackTag(element); // as will be start of next element
	return isTag;
}

// terminates an optional section on reading
void CObjectSerializer::endIfOptional()
{
	assert(this);
	endTag();
}

// starts reading in a section and matches this object with the ID from
// the stream.
void CObjectSerializer::startReadSection(const char* pszName, void* pObject)
{
	assert(this);
	assert(pszName);
	assert(pObject);

	string element = startTag();

	if(element.compare(pszName) != 0)
	{
		assert(false);
		throw CSerializeException("Serialization - name mismatch reading section");
	}

	element = startTag(); // <id>
	if(element.compare("id") != 0)
	{
		// missing id
		throw CSerializeException("Serialization - missing id field reading section");
	}

	*ps >> currentID;
	endTag();

	// superclasses share ID with subclass - don't override!
	if(objectMap.find(currentID) == objectMap.end())
	{
		objectMap.insert(OBJECT_MAP::value_type(currentID, pObject));
	}


}

// ends reading a section
void CObjectSerializer::endReadSection()
{
	assert(this);
	endTag();
}

// starts reading a collection - returns the number of items in
// that collection
int CObjectSerializer::startReadCollection(const char* pszName)
{
	assert(this);
	string element = startTag();

	if(element.compare(pszName) != 0)
	{
		assert(false);
		throw CSerializeException("Serialization - name mismatch reading collection");
	}

	element = startTag(); // <count>
	if(element.compare("count") != 0)
	{
		// missing count
		throw CSerializeException("Serialization - missing count reading collection");
	}

	int count = 0;
	*ps >> count;
	endTag();

	return count;
}

// ends reading a collection
void CObjectSerializer::endReadCollection()
{
	assert(this);
	endTag();
}

//////////////////////////////////////////////////////////////////////
// private method to read in the start tag of a pair (e.g. <thing>)
// The tag is pushed onto a stack so it can be matched against the 
// corresponding end tag (e.g. </thing>).
std::string CObjectSerializer::startTag()
{
	assert(this);
	string element = nextTag();
	sectionNames.push(element);
	return element;
}

// Gets the enxt tag in the xml stream whether start or end.  Implements
// 1 level push-back with the savedTag member.
string CObjectSerializer::nextTag()
{
	char ch;

	string element = "";

	if(savedTag.size() > 0)
	{
		element = savedTag;
		savedTag.erase();
	}
	else
	{
		*ps >> ch;
		while(ps->good() && ch != '<')
			*ps >> ch;


		*ps >> ch;
		while(ps->good() && ch != '>')
		{
			element += ch;
			*ps >> ch;
		}
	}

	return element;
}

// Reads from the input, and verifies a terminating tag
void CObjectSerializer::endTag()
{
	assert(this);

	string element = nextTag();
	string required = sectionNames.top();
	sectionNames.pop();

	required = "/" + required; // need terminator

	if(element.compare(required) != 0)
	{
		throw CSerializeException("Serialization - end tag does not match start tag");
	}
}

// Implements single-level push-back to allow lookahead.
void CObjectSerializer::pushBackTag(const string& element)
{
	assert(this);
	if(savedTag.size() > 0)
		throw CSerializeException("too many levels of lookahead");
	savedTag = element;
}
