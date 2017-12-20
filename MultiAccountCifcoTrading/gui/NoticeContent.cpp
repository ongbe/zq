#include "StdAfx.h"
#include "NoticeContent.h"
#define USERNOTICECONTENT_COUNT 100
NoticeContent::NoticeContent():
m_bUserNoticeFinished(false)
{
    m_UserNoticeContent.SetCount(USERNOTICECONTENT_COUNT);
}
NoticeContent::~NoticeContent()
{
    m_UserNoticeContent.Clear();
}
void NoticeContent::SetField(CThostFtdcNoticeField& field)
{
    int index=atoi(field.SequenceLabel);
    if(index>=(int)m_UserNoticeContent.GetCount())
        m_UserNoticeContent.SetCount(index);
    m_UserNoticeContent[index]=field.Content;
}
wxString NoticeContent::GetContent()
{
    wxString strContent;
    for(size_t i=0;i<m_UserNoticeContent.GetCount();i++)
    {
        strContent+=m_UserNoticeContent[i];
    }
    return strContent;
}