; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{ADE23CA2-5613-4DEE-9E36-1374B20760FE}
AppName=风控管理终端
AppVerName=风控管理终端 1.0.3.2
AppPublisher=CIFCO ITSER VICES CO.,LTD
AppPublisherURL=http://www.cifco.net
AppSupportURL=http://www.cifco.net
AppUpdatesURL=http://www.cifco.net
DefaultDirName={sd}\RiskManage
DisableDirPage=no
DefaultGroupName=风控管理终端
OutputDir=.\output
OutputBaseFilename=RiskManageSetup
Compression=lzma
SolidCompression=yes
VersionInfoCompany=CIFCO ITSER VICES CO.,LTD
VersionInfoTextVersion=1.0.3.2
VersionInfoVersion=1.0.3.2
[Languages]
;Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "chs"; MessagesFile: "compiler:ChineseSimp.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: ".\Release\RiskManage.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: ".\Release\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{group}\风控管理终端"; Filename: "{app}\RiskManage.exe"; WorkingDir:"{app}"
Name: "{group}\{cm:UninstallProgram,风控管理终端}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\风控管理终端"; Filename: "{app}\RiskManage.exe"; Tasks: desktopicon; WorkingDir:"{app}"

[Run]
Filename: "{app}\RiskManage.exe"; Description: "{cm:LaunchProgram,风控管理终端}"; Flags: nowait postinstall skipifsilent
Filename: "{app}\vcredist_x86.exe"; Parameters: /q; WorkingDir: {app}; Flags: skipifdoesntexist; StatusMsg: "Installing Microsoft Visual C++ Runtime ..."; Check: NeedInstallVC9SP1

[Code]
var vc9SP1Missing: Boolean;

function NeedInstallVC9SP1(): Boolean;
begin
  Result := vc9SP1Missing;
end;

function InitializeSetup(): Boolean;
var version: Cardinal;
begin
  vc9SP1Missing := true;
  if RegQueryDWordValue(HKLM, 'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{ADE23CA2-5613-4DEE-9E36-1374B20760FE}', 'Version', version) = true then
    begin
      vc9SP1Missing := false;
    end;
  result := true;
end;

procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);

begin

if CurUninstallStep = usUninstall then

//if MsgBox('您是否要删除用户配置信息？', mbConfirmation, MB_YESNO) = IDYES then

//删除 {app} 文件夹及其中所有文件

DelTree(ExpandConstant('{app}'), True, True, True);

end;






































