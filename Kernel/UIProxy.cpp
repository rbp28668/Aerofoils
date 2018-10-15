// UIProxy.cpp: implementation of the CUIProxy class.
//
//////////////////////////////////////////////////////////////////////
#include <assert.h>
#include "UIProxy.h"
#include "ObjectSerializer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUIProxy::CUIProxy()
{

}

CUIProxy::~CUIProxy()
{

}

// gets the name of the class - override this if you want to use
// the default serialization & there is no extra data to serialize,
// otherwise, override serializeTo and serializeFrom.
const char* CUIProxy::className() const
{
	assert(this);
	assert(false);  // This should never be called - only derived classes
	return "UNKNOWN";
}

void CUIProxy::serializeTo(CObjectSerializer& os)
{
	assert(this);
	os.startSection(className(), this);
	os.endSection();
}

void CUIProxy::serializeFrom(CObjectSerializer& os)
{
	assert(this);
	os.startReadSection(className(), this);
	os.endReadSection();
}
