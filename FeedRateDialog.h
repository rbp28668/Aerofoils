#pragma once


// FeedRateDialog dialog

class FeedRateDialog : public CDialogEx
{
	DECLARE_DYNAMIC(FeedRateDialog)

public:
	FeedRateDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~FeedRateDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FEED_RATE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	// feed rate in mm per second
	double feedRate;
	BOOL useFeedRate;
};
