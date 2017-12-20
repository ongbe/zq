#pragma once

#include <string>
#include <wx/bitmap.h>


class VerifyCode
{
public:

	VerifyCode(void);
	~VerifyCode(void);

	bool Verify(wxString & code);

	const wxBitmap & GetVerifyCodeBitmap();

private:
	void RefreshCode();
	bool rand_str(int len);
	bool CreateBmp();

private:
	wxBitmap m_VerifyCodeBitmap;
	wxString m_VerifyCodeText;
};