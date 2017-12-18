class UserNoticeDlg;
class NoticeContent
{
public:
    NoticeContent();
    ~NoticeContent();
    void SetField(CThostFtdcNoticeField& field);
    void SetFinished(){m_bUserNoticeFinished=true;}
    bool GetFinished(){return m_bUserNoticeFinished;}
    wxString GetContent();   
private:
    wxArrayString m_UserNoticeContent;//最大一位数，100个
    bool m_bUserNoticeFinished; 
};