// UIProxy.h: interface for the CUIProxy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UIPROXY_H__7C73CD15_C566_11D6_9554_00047660C89A__INCLUDED_)
#define AFX_UIPROXY_H__7C73CD15_C566_11D6_9554_00047660C89A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CObjectSerializer;

class CUIProxy  
{
public:
	CUIProxy();
	virtual ~CUIProxy();

	virtual const char* className() const;
	virtual void serializeTo(CObjectSerializer& os);
	virtual void serializeFrom(CObjectSerializer& os);

};

#endif // !defined(AFX_UIPROXY_H__7C73CD15_C566_11D6_9554_00047660C89A__INCLUDED_)
