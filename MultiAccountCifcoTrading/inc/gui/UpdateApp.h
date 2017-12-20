class UpdateApp
{
public:
    static bool DoUpdate();
    static bool CheckVersion(const wxString&  strXmlName);
    static bool CheckNeedUpdate(const wxString& RemoteVer,const wxString& LocalVer);
    static void StartUpdateApp(bool bOnlyDownload);
    static void ReplaceUpdateApp();
    static void KillUpdateApp();
private:
	static long m_UpdaterPid;
};
