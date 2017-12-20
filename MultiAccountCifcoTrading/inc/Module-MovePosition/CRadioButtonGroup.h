#pragma once


class CRadioButtonGroup
{
public:
	CRadioButtonGroup() {}
	~CRadioButtonGroup() {}

	//要保证插入的顺序是正确的，因为之后的值设置都是按照这个顺序
	void InsertGroup(wxRadioButton* p)
	{
		m_vecRadioButton.push_back(p);
	}

	void SetGroupValue(int value)
	{
		if ((size_t)value < m_vecRadioButton.size())
		{
			wxRadioButton* p = (m_vecRadioButton[value]);
			if (p) p->SetValue(true);
		}
	}

	int GetGroupValue()
	{
		int val=0;
		std::vector<wxRadioButton*>::iterator iter = m_vecRadioButton.begin();
		for (; iter != m_vecRadioButton.end(); ++iter)
		{
			wxRadioButton* p = *iter;
			if (p && p->GetValue())	return val;
			else ++val;

		}
		return -1;
	}
protected:
private:
	std::vector<wxRadioButton*> m_vecRadioButton;
};