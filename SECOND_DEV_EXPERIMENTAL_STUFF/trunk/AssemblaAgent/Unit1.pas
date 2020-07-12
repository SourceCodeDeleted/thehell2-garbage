unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, IdBaseComponent, IdComponent, IdCustomTCPServer,
  IdCustomHTTPServer, IdHTTPServer, IdContext, ExtCtrls, ComCtrls, ShellAPI, IniFiles,
  SynEditHighlighter, SynHighlighterBat, SynEdit, ImgList, JvBaseDlg,
  JvBrowseFolder, FolderDialog, Clipbrd, Registry, sSkinManager, Menus,
  sSkinProvider, ToolWin, thread_builder;

type
  TMainForm = class(TForm)
    HTTPServer: TIdHTTPServer;
    StatusBar1: TStatusBar;
    PageControl: TPageControl;
    TabSheet1: TTabSheet;
    SynBatSyn1: TSynBatSyn;
    Label18: TLabel;
    ImageList1: TImageList;
    FolderDialog1: TFolderDialog;
    OpenDialog1: TOpenDialog;
    sheet_log: TTabSheet;
    console_memo: TMemo;
    Panel1: TPanel;
    ed_log_file: TLabeledEdit;
    Panel2: TPanel;
    button_clear_log: TButton;
    TabSheet2: TTabSheet;
    GroupBox3: TGroupBox;
    Label5: TLabel;
    Label7: TLabel;
    Label8: TLabel;
    Label9: TLabel;
    Label10: TLabel;
    Label11: TLabel;
    Label12: TLabel;
    Label14: TLabel;
    Label13: TLabel;
    Label15: TLabel;
    ed_project_name: TEdit;
    ed_space_tool: TEdit;
    ed_secret_key: TEdit;
    ed_post_url: TEdit;
    ed_scm_url: TEdit;
    ed_scm_login: TEdit;
    ed_scm_password: TEdit;
    ed_config: TEdit;
    Button4: TButton;
    Button2: TButton;
    TabSheet3: TTabSheet;
    GroupBox2: TGroupBox;
    Label19: TLabel;
    Label17: TLabel;
    Label2: TLabel;
    Label21: TLabel;
    ed_project_path: TButtonedEdit;
    ed_configuration: TLabeledEdit;
    ed_repo: TButtonedEdit;
    button_checkout: TButton;
    button_info: TButton;
    button_clear: TButton;
    Button1: TButton;
    button_update: TButton;
    ed_version: TLabeledEdit;
    Button6: TButton;
    ed_msbuild: TButtonedEdit;
    ed_svn: TButtonedEdit;
    button_getpath: TButton;
    GroupBox1: TGroupBox;
    Label4: TLabel;
    Label3: TLabel;
    Label1: TLabel;
    Label6: TLabel;
    Label20: TLabel;
    lbl_response_count: TLabel;
    ed_command_deploy: TEdit;
    ed_port: TEdit;
    Edit2: TEdit;
    Edit4: TEdit;
    box_listen_on_run: TCheckBox;
    btn_listen: TButton;
    btn_dontlisten: TButton;
    button_build: TButton;
    SkinManager: TsSkinManager;
    ToolBar1: TToolBar;
    ToolButton1: TToolButton;
    ToolButton2: TToolButton;
    MainMenu1: TMainMenu;
    N1: TMenuItem;
    N2: TMenuItem;
    N3: TMenuItem;
    N4: TMenuItem;
    N5: TMenuItem;
    procedure HTTPServerCommandGet(AContext: TIdContext;
      ARequestInfo: TIdHTTPRequestInfo; AResponseInfo: TIdHTTPResponseInfo);
    procedure btn_listenClick(Sender: TObject);
    procedure btn_dontlistenClick(Sender: TObject);
    procedure btn_exitClick(Sender: TObject);
    procedure Button4Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure ed_portChange(Sender: TObject);
    procedure ed_repoRightButtonClick(Sender: TObject);
    procedure ed_repoChange(Sender: TObject);
    procedure ed_project_pathChange(Sender: TObject);
    procedure button_checkoutClick(Sender: TObject);
    procedure ed_scm_passwordChange(Sender: TObject);
    procedure box_listen_on_runClick(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure button_updateClick(Sender: TObject);
    procedure ed_log_fileChange(Sender: TObject);
    procedure ed_configurationChange(Sender: TObject);
    procedure ed_scm_loginChange(Sender: TObject);
    procedure button_infoClick(Sender: TObject);
    procedure button_clearClick(Sender: TObject);
    procedure button_clear_logClick(Sender: TObject);
    procedure ed_scm_urlChange(Sender: TObject);
    procedure Button6Click(Sender: TObject);
    procedure ed_versionChange(Sender: TObject);
    procedure button_getpathClick(Sender: TObject);
    procedure ed_msbuildChange(Sender: TObject);
    procedure ed_svnChange(Sender: TObject);
    procedure ed_project_pathRightButtonClick(Sender: TObject);
    procedure ed_msbuildRightButtonClick(Sender: TObject);
    procedure ed_svnRightButtonClick(Sender: TObject);
    procedure ed_repoKeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
    procedure ed_project_pathKeyUp(Sender: TObject; var Key: Word;
      Shift: TShiftState);
    procedure ed_msbuildKeyUp(Sender: TObject; var Key: Word;
      Shift: TShiftState);
    procedure ed_svnKeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
    procedure button_buildClick(Sender: TObject);
    procedure lbl_response_countClick(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

  procedure LogEvent( procedure_name, text:string );

  function GetDosOutput(CommandLine: string; Work: string = 'C:\'): TStringList;
  function DosToWin(S: AnsiString): String;
  function CheckCommandForExistence( cmd:string ):boolean;
  procedure MSBuild( framework:string; config:string; proj_file_path:string );

var
  MainForm: TMainForm;
  request_count : integer;
  deploy_cmd : string;
  root_dir : string;
  ini_configuration : TINIFile;
  temp_log : string;
  thread_bulder : TThread_builder;

implementation

uses request_frm, routine_strings, frm_deploy;

{$R *.dfm}

function CheckCommandForExistence( cmd:string ):boolean;
const
  not_found_rus = 'не является внутренней';
  not_found_eng = 'is not recognized as an internal';
var
  output : string;
begin
 result := true;
 output := GetDosOutput('msbuild').Text;
 if ( Pos( not_found_rus, output ) > 0 ) or
    ( Pos( not_found_eng, output ) > 0 )
   then
    result := False;
end;

function RemoveLastSlash( path:string ) : string;
var
  last:integer;
begin
  result := '';
  if Length( path ) > 0 then
  begin
    last := Length( path );
    if path[last] = '\' then
      SetLength( path, last - 1 );
    result := path;
  end;
end;

function FixDoubleSlashes( path:string ):string;
begin
  try
    result := Replace( path, '\\', '\' );
  except
    ON E:Exception DO
      LogEvent('FixDoubleSlashes', e.Message );

  end;
end;


function FixOEMLines( s: ansistring ) : string;
begin
  result := s;
  result := StringReplace( Result,  'd0d0',  #13,  [rfReplaceAll] );
  result := StringReplace( Result,  'd0',  #13,  [rfReplaceAll] );
end;

function DosToWin(S: AnsiString): String;
begin
  SetLength( Result,Length( S ) );
  S:=S+#0;
  if not OemToChar(PAnsiChar(S),PWideChar(Result)) then SetLength(Result,0);
end;

procedure LogEvent( procedure_name, text:string );
var
  temp_log : string;
  stringlist : TStringList;
begin
try

  MainForm.console_memo.Lines.Add( TimeToStr(Time) +' '+ DateToStr(date) + ' > ' + procedure_name + ' > ' + text );
  MainForm.console_memo.Lines.SaveToFile( MainForm.ed_log_file.Text )

except
  ON E:Exception DO
  begin
    stringlist := TStringList.Create;
    stringlist.LoadFromFile( temp_log );
    stringlist.Add( e.Message );
    stringlist.SaveToFile( temp_log );
  end;

end;

end;

function GetDosOutput(CommandLine: string; Work: string = 'C:\'): TStringList;
var
  SA: TSecurityAttributes;
  SI: TStartupInfo;
  PI: TProcessInformation;
  StdOutPipeRead, StdOutPipeWrite: THandle;
  WasOK: Boolean;
  Buffer: array[0..255] of AnsiChar;
  BytesRead: Cardinal;
  WorkDir: string;
  Handle: Boolean;
  process_string : string;
begin
try
  Result := TStringList.Create;
  with SA do begin
    nLength := SizeOf(SA);
    bInheritHandle := True;
    lpSecurityDescriptor := nil;
  end;

  CreatePipe(StdOutPipeRead, StdOutPipeWrite, @SA, 0);

    with SI do
    begin
      FillChar(SI, SizeOf(SI), 0);
      cb := SizeOf(SI);
      dwFlags := STARTF_USESHOWWINDOW or STARTF_USESTDHANDLES;
      wShowWindow := SW_HIDE;
      hStdInput := GetStdHandle(STD_INPUT_HANDLE); // don't redirect stdin
      hStdOutput := StdOutPipeWrite;
      hStdError := StdOutPipeWrite;
    end;

    WorkDir        := Work;
    process_string := 'cmd.exe  /C"' + CommandLine + '"';
    Handle         := CreateProcess(nil, PChar( process_string ),
                            nil, nil, True, 0, nil,
                            PChar( WorkDir ), SI, PI);

    CloseHandle( StdOutPipeWrite );
    if not Handle then
      LogEvent( 'GetDosOutput','Процесс не создан. :' + process_string );

    if Handle then
      try
        repeat
          WasOK := ReadFile(StdOutPipeRead, Buffer, 255, BytesRead, nil);
          if BytesRead > 0 then
          begin
            Buffer[BytesRead] := #0;
            Result.Text := Result.Text + Buffer;
            Result.Text := DosToWin( result.Text );
            Result.Text := FixOEMLines( result.text );
          end;
        until not WasOK or (BytesRead = 0);
        WaitForSingleObject(PI.hProcess, INFINITE);
      finally
        CloseHandle(PI.hThread);
        CloseHandle(PI.hProcess);
      end;

  except
    ON E:Exception DO
    begin
      LogEvent( 'exception > GetDosOutput: ' , e.Message  );
      CloseHandle(StdOutPipeRead);
    end;
  end;
end;

procedure TMainForm.btn_listenClick(Sender: TObject);
begin
{
pf := PChar(GetDOSEnvVar('ProgramFiles'));
ShellExecute(Application.Handle, 'Open', PChar(chr(34)+pf+'\Common Files\ASKUEComServer\'+'RegServers.bat'+chr(34)), '', '', SW_SHOWNORMAL);
}
  HTTPServer.Active := false;
  HTTPServer.DefaultPort := strtoint( ed_port.Text );
  HTTPServer.Active := true;


  if HTTPServer.active then
  begin
    btn_listen.enabled := False;
    btn_dontlisten.Enabled := true;
  end;

  ed_port.Enabled := False;

end;

procedure TMainForm.btn_dontlistenClick(Sender: TObject);
begin
  HTTPServer.Active := false;

  if not HTTPServer.active then
  begin
    btn_listen.enabled := true;
    btn_dontlisten.Enabled := false;
  end;

  ed_port.Enabled := true;
end;

procedure TMainForm.btn_exitClick(Sender: TObject);
begin
  HTTPServer.Active := false;
  close;
end;

procedure TMainForm.button_checkoutClick(Sender: TObject);
var
  command : string;
begin
//MAN по коммандной строке SVN http://svnbook.red-bean.com/en/1.0/re04.html
  command := 'svn.exe checkout --non-interactive "<url>" --username=<username> --password=<password> "<path>" ';
  command := Replace( command, '<url>', ed_scm_url.Text );
  command := Replace( command, '<username>', ed_scm_login.Text );
  command := Replace( command, '<password>', ed_scm_password.Text );
  command := Replace( command, '<path>', ed_repo.Text );

  pagecontrol.ActivePage := sheet_log;

  LogEvent( 'checkout', command );
  LogEvent( 'checkout', GetDosOutput( command ).Text );

end;

procedure RemoveFolder(Dir: String);
var
  Result: TSearchRec; Found: Boolean;
begin
  Found := False;
  if FindFirst(Dir + '\*', faAnyFile, Result) = 0 then
    while not Found do begin
      if (Result.Attr and faDirectory = faDirectory) AND (Result.Name <> '.') AND (Result.Name <> '..') then RemoveFolder(Dir + '\' + Result.Name)
      else if (Result.Attr and faAnyFile <> faDirectory) then DeleteFile(Dir + '\' + Result.Name);
      Found := FindNext(Result) <> 0;
    end;
  FindClose(Result); RemoveDir(Dir);
end;

procedure TMainForm.button_clearClick(Sender: TObject);
begin
ShellExecute( Application.Handle, 'open', PChar(ed_repo.Text), nil, nil, SW_SHOWNORMAL );

end;

procedure TMainForm.button_updateClick(Sender: TObject);
var
  command : string;
begin
//MAN по коммандной строке SVN http://svnbook.red-bean.com/en/1.0/re04.html
  command := 'svn.exe update --non-interactive "<path>" -r BASE --username=<username> --password=<password>  ';
  command := Replace( command, '<username>', ed_scm_login.Text );
  command := Replace( command, '<password>', ed_scm_password.Text );
  command := Replace( command, '<path>', ed_repo.Text );

  pagecontrol.ActivePage := sheet_log;

  LogEvent( 'update', command );
  LogEvent( 'update', GetDosOutput( command ).Text );
end;

procedure TMainForm.button_infoClick(Sender: TObject);
var
  command : string;
begin
//MAN по коммандной строке SVN http://svnbook.red-bean.com/en/1.0/re04.html
  command := 'svn.exe info --non-interactive "<url>" --username=<username> --password=<password> "<path>" ';
  command := Replace( command, '<url>', ed_scm_url.Text );
  command := Replace( command, '<username>', ed_scm_login.Text );
  command := Replace( command, '<password>', ed_scm_password.Text );
  command := Replace( command, '<path>', ed_repo.Text );

  pagecontrol.ActivePage := sheet_log;

  LogEvent( 'info', command );
  LogEvent( 'info', GetDosOutput( command ).Text );

end;
procedure TMainForm.Button2Click(Sender: TObject);
var
  str:string;
begin
  str := '?space_tool_id=' +  ed_space_tool.Text + '&secret_key=' + ed_secret_key.Text + '&opts=%7b%22start_time%22%3a%22Mon%20Feb%2008%2015%3a23%3a27%20-0500%202010%22%2c%22status%22%3a0%2c%22log%22%3a%22This%20is%20a%20long%20log%20message%22%2c%22end_time'+'%22%3a%22Mon%20Feb%2008%2016%3a23%3a27%20-0500%202010%22%2c%22comment%22%3a%22Short%20comment%22%7d';
  str := ed_post_url.text + str;
  Clipboard.AsText := str;
end;

procedure TMainForm.button_buildClick(Sender: TObject);
begin
//  MSBuild( ed_msbuild.Text, ed_configuration.Text, ed_project_path.Text );
  thread_bulder := TThread_builder.Create;
end;

procedure TMainForm.Button4Click(Sender: TObject);
begin
  if MessageBox( Application.Handle, 'Вы уверены?', 'Вы уверены?', mb_YesNoCancel or mb_iconquestion ) = mrYes then
  begin
    ed_project_name.Text := '';
    ed_project_name.Text := '';
    ed_space_tool.Text   := '';
    ed_secret_key.Text   := '';
    ed_post_url.Text     := '';
    ed_scm_url.Text      := '';
    ed_scm_login.Text    := '';
    ed_scm_password.Text := '';

    ini_configuration.WriteString( 'configure' ,'PROJECT_NAME', '' );
    ini_configuration.WriteString( 'configure' ,'SPACE_TOOL_ID', '' );
    ini_configuration.WriteString( 'configure' ,'SECRET_KEY', '' );
    ini_configuration.WriteString( 'configure' ,'POST_URL', '' );
    ini_configuration.WriteString( 'configure' ,'SCM_URL', '' );
    ini_configuration.WriteString( 'configure' ,'SCM_LOGIN', '' );
    ini_configuration.WriteString( 'configure' ,'SCM_PASSWORD', '' );
  end;
end;

procedure TMainForm.Button6Click(Sender: TObject);
var
  command : string;
  rev_pos : integer;
  buf : string;
  node_kind_pos : integer;
  version : string;
begin
//MAN по коммандной строке SVN http://svnbook.red-bean.com/en/1.0/re04.html
  command := ' svn.exe info -r HEAD --non-interactive "<url>" --username=<username> --password=<password> ';
  command := Replace( command, '<username>', ed_scm_login.Text );
  command := Replace( command, '<password>', ed_scm_password.Text );
  command := Replace( command, '<url>', ed_scm_url.Text );

  pagecontrol.ActivePage := sheet_log;

  LogEvent( 'update', command );
  LogEvent( 'update', GetDosOutput( command ).Text );
  rev_pos := Pos('Revision:',  GetDosOutput( command ).Text );
  if rev_pos > 0 then
  begin
    rev_pos := rev_pos + Length( 'Revision: ' );
    node_kind_pos := Pos('Node Kind:',  GetDosOutput( command ).Text );
    version := Copy( GetDosOutput( command ).Text, rev_pos, node_kind_pos - rev_pos );
    version := replace( version, #$D#$A, '' );
    ed_version.Text := version;
  end;

end;


function RegeditGetString( root:DWORD; path:string; param:string ):string;
var
   Reg: TRegistry;
//   RegKey: DWORD;
regkey : string;
   Key: string;
begin
 RegKey := '';
 Reg := TRegistry.Create;
 try
   Reg.RootKey := HKEY_LOCAL_MACHINE;
   Key := 'SOFTWARE\Microsoft\NET Framework Setup\NDP\v4\Client';
   if Reg.OpenKeyReadOnly(Key) then
   begin
     if Reg.ValueExists('InstallPath') then
     begin
       RegKey := Reg.ReadString('InstallPath');
       Reg.CloseKey;
     end;
   end;
 finally
   Reg.Free
 end;
 result := regkey;
end;


procedure TMainForm.button_getpathClick(Sender: TObject);
var
  Reg: TRegistry;
  regkey : string;
  Key: string;
begin
  Key := 'SOFTWARE\Microsoft\NET Framework Setup\NDP\v4\Client';
  RegKey := RegeditGetString( HKEY_LOCAL_MACHINE, Key, 'InstallPath' );
  ed_msbuild.Text := regkey;
end;



procedure TMainForm.button_clear_logClick(Sender: TObject);
begin
  console_memo.Clear;
  LogEvent('clear','Очистка журнала')
end;

procedure TMainForm.box_listen_on_runClick(Sender: TObject);
begin
 ini_configuration.WriteBool( 'general' ,'listen-on-run', (Sender as TCheckbox).Checked );
end;

procedure TMainForm.ed_project_pathChange(Sender: TObject);
begin
 ini_configuration.WriteString( 'build' ,'project_path', ed_project_path.Text  );
end;

procedure TMainForm.ed_project_pathKeyUp(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
  if key = 115 then
    (Sender as TButtonedEdit).OnRightButtonClick( Sender );
end;

procedure TMainForm.ed_project_pathRightButtonClick(Sender: TObject);
begin
  if OpenDialog1.Execute then
  begin
     ed_project_path.Text := OpenDialog1.FileName;
  end;
end;

procedure TMainForm.ed_repoChange(Sender: TObject);
begin
 ini_configuration.WriteString( 'build' ,'repo_path', ed_repo.Text  );
end;

procedure TMainForm.ed_repoKeyUp(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
  if key = 115 then
    ed_repo.OnRightButtonClick( ed_repo );
end;

procedure TMainForm.ed_repoRightButtonClick(Sender: TObject);
begin
  if OpenDialog1.Execute then
  begin
     ed_repo.Text := OpenDialog1.FileName;
  end;
end;

procedure TMainForm.ed_scm_loginChange(Sender: TObject);
begin
  ini_configuration.WriteString( 'configure' ,'SCM_LOGIN', ed_scm_login.Text );
end;

procedure TMainForm.ed_scm_passwordChange(Sender: TObject);
begin
  ini_configuration.WriteString( 'configure' ,'SCM_PASSWORD', ed_scm_password.Text );
end;

procedure TMainForm.ed_scm_urlChange(Sender: TObject);
begin
  ini_configuration.WriteString( 'configure' ,'SCM_URL', ed_scm_url.Text );
end;

procedure TMainForm.ed_svnChange(Sender: TObject);
begin
 ini_configuration.WriteString( 'build' ,'svn', ed_svn.Text  );
end;

procedure TMainForm.ed_svnKeyUp(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
  if key = 115 then
    (Sender as TButtonedEdit).OnRightButtonClick( Sender );
end;

procedure TMainForm.ed_svnRightButtonClick(Sender: TObject);
begin
  if OpenDialog1.Execute then
  begin
     ed_svn.Text := OpenDialog1.FileName;
  end;
end;

procedure TMainForm.ed_versionChange(Sender: TObject);
begin
  ini_configuration.WriteString( 'build' ,'version', ed_version.Text );
end;

procedure TMainForm.ed_configurationChange(Sender: TObject);
begin
  ini_configuration.WriteString( 'build' ,'configuration', ed_configuration.Text );
end;

procedure TMainForm.ed_log_fileChange(Sender: TObject);
begin
 ini_configuration.WriteString( 'debug' ,'log_path', ed_log_file.Text  );
end;

procedure TMainForm.ed_msbuildChange(Sender: TObject);
begin
 ini_configuration.WriteString( 'build' ,'msbuild', ed_msbuild.Text  );
end;

procedure TMainForm.ed_msbuildKeyUp(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
  if key = 115 then
    (Sender as TButtonedEdit).OnRightButtonClick( Sender );
end;

procedure TMainForm.ed_msbuildRightButtonClick(Sender: TObject);
begin
  if OpenDialog1.Execute then
  begin
     ed_msbuild.Text := OpenDialog1.FileName;
  end;
end;

procedure TMainForm.ed_portChange(Sender: TObject);
begin
 ini_configuration.WriteString( 'configure' ,'port', ed_port.Text  );
end;

procedure TMainForm.FormCreate(Sender: TObject);
begin
  SkinManager.Active := true;
  request_count := 0;
  root_dir := ExtractFilePath( paramstr(0) );
  ini_configuration := TIniFile.Create( root_dir + '\' + 'config.ini' );

  // лог----------------------------------------------------------------------
  temp_log  := GetEnvironmentVariable('temp') + '\assembla_agent_log.txt';
  temp_log  := FixDoubleSlashes( temp_log );

  ed_log_file.Text := ini_configuration.ReadString( 'debug' ,'log_path', '' );
  ed_log_file.Text := FixDoubleSlashes( ed_log_file.Text  );

  if Length( ed_log_file.Text ) = 0 then
    ed_log_file.Text := temp_log;
  if FileExists(  ed_log_file.Text ) then
    console_memo.Lines.LoadFromFile( ed_log_file.Text );

  LogEvent( 'start', 'Запуск' );

  //--------------------------------------------------------------------------

  ed_project_name.Text := ini_configuration.ReadString( 'configure' ,'PROJECT_NAME', '' );
  ed_space_tool.Text   := ini_configuration.ReadString( 'configure' ,'SPACE_TOOL_ID', '' );
  ed_secret_key.Text   := ini_configuration.ReadString( 'configure' ,'SECRET_KEY', '' );
  ed_post_url.Text     := ini_configuration.ReadString( 'configure' ,'POST_URL', '' );
  ed_scm_url.Text      := ini_configuration.ReadString( 'configure' ,'SCM_URL', '' );
  ed_scm_login.Text    := ini_configuration.ReadString( 'configure' ,'SCM_LOGIN', '' );
  ed_scm_password.Text := ini_configuration.ReadString( 'configure' ,'SCM_PASSWORD', '' );
  ed_version.Text      := ini_configuration.ReadString( 'build' ,'version', '' );

  ed_repo.Text         := ini_configuration.ReadString( 'build' ,'repo_path', '' );
  ed_repo.Text         := RemoveLastSlash( ed_repo.Text );
  ed_port.Text         := ini_configuration.ReadString( 'configure' ,'port', '' );
  ed_project_path.Text := ini_configuration.ReadString( 'build' ,'project_path', ''  );
  ed_configuration.Text:= ini_configuration.ReadString( 'build' ,'configuration', '' );

  ed_svn.Text     := ini_configuration.ReadString( 'build' ,'svn', ed_svn.Text  );
  ed_msbuild.Text := ini_configuration.ReadString( 'build' ,'msbuild', ed_msbuild.Text  );

  box_listen_on_run.Checked := ini_configuration.ReadBool( 'general' ,'listen-on-run', true);

//  if box_listen_on_run.Checked then
//    btn_listen.Click;



end;

//  cd C:\WINDOWS\Microsoft.NET\Framework\v4.0.30319\
//  msbuild.exe /p:PlatformToolset=Windows7.1SDK /property:Configuration=Debug "C:\thehell2\trunk\thehell2.vcxproj"
procedure MSBuild( framework:string; config:string; proj_file_path:string );
const
  command_template = 'msbuild.exe /p:PlatformToolset=Windows7.1SDK /property:Configuration=<config> "<proj_file_path>"';
var
  command : string;
begin

  command := command_template;
  command := Replace( command, '<config>', config );
  command := Replace( command, '<proj_file_path>', proj_file_path );
  LogEvent( 'MSBuild', GetDosOutput( command, framework ).Text );

end;

procedure TMainForm.HTTPServerCommandGet(AContext: TIdContext;
  ARequestInfo: TIdHTTPRequestInfo; AResponseInfo: TIdHTTPResponseInfo);
var
  params          : string;
  param           : string;
  configure_param : string;
  i               : integer;
  row_index       : integer;
  param_name      : string;
  param_value     : string;
begin
 deploy_cmd := ed_command_deploy.Text;
 // Сделать запись в таблице
 request_form.grid_reqs.RowCount := request_form.grid_reqs.RowCount + 1;
 row_index                       := request_form.grid_reqs.RowCount - 1;
 request_form.grid_reqs.Cells[ 0, row_index ] := IntToStr( row_index );
 request_form.grid_reqs.Cells[ 1, row_index ] := TimeToStr( time );
 request_form.grid_reqs.Cells[ 2, row_index ] := ARequestInfo.URI;
 request_form.grid_reqs.Cells[ 3, row_index ] := ARequestInfo.RemoteIP;
 // Параметры строкой для отладки
 params := '';
 for i := 0 to ARequestInfo.Params.Count -1 do
  params := params + ARequestInfo.Params[i] + ';';

 request_form.grid_reqs.Cells[ 4, row_index ] := params;

 // Надпись внизу
 lbl_response_count.Visible := true;
 request_count              := request_count + 1;
 lbl_response_count.Caption := 'Получено запросов: ' + inttostr( request_count );

  if ARequestInfo.URI = '/' then
 begin
     // Ответить 200 OK
   AResponseInfo.ResponseNo     :=  200;
   AResponseInfo.ContentEncoding:= 'windows-1251';
   AResponseInfo.ContentType    := 'Content-Type: text/plain';
   AResponseInfo.ContentText    := 'Hello';
   AResponseInfo.WriteHeader;
   AResponseInfo.WriteContent;
 end;

 // Выполнить deploy
 if ARequestInfo.URI = '/configure' then
 begin
   // Ответить 200 OK
   AResponseInfo.ResponseNo      := 200;
   AResponseInfo.ContentEncoding :='windows-1251';
   AResponseInfo.ContentType     :='Content-Type: text/plain';
   AResponseInfo.ContentText     :='Working';
   if ARequestInfo.Params.Count > 0 then
   begin
    configure_param := ARequestInfo.Params[0];
    configure_param := replace( configure_param, '#!/bin/bash', '' );
    configure_param := replace( configure_param, 'export' , ';' );

    for i := 0 to SubStringCount( configure_param, ';' ) do
      begin
        param := SubString( configure_param, ';', i + 1 );
        param_name := SubString( param, '=', 1 );
        param_value := SubString( param, '=', 2 );
        if StringAssigned( param_name ) then
          ini_configuration.WriteString( 'configure', param_name, param_value );
      end;

      ed_project_name.Text := ini_configuration.ReadString( 'configure' ,'PROJECT_NAME', '' );
      ed_space_tool.Text   := ini_configuration.ReadString( 'configure' ,'SPACE_TOOL_ID', '' );
      ed_secret_key.Text   := ini_configuration.ReadString( 'configure' ,'SECRET_KEY', '' );
      ed_post_url.Text     := ini_configuration.ReadString( 'configure' ,'POST_URL', '' );
      ed_scm_url.Text      := ini_configuration.ReadString( 'configure' ,'SCM_URL', '' );
      ed_scm_login.Text    := ini_configuration.ReadString( 'configure' ,'SCM_LOGIN', '' );
      ed_scm_password.Text := ini_configuration.ReadString( 'configure' ,'SCM_PASSWORD', '' );
   end;

 end;

 // Выполнить deploy
 if ARequestInfo.URI = deploy_cmd then
 begin


   AResponseInfo.ResponseNo     :=  200;
// AResponseInfo.ContentEncoding:= 'windows-1251';
// AResponseInfo.ContentType    := 'Content-Type: text/plain';
   AResponseInfo.ContentType     := 'application/json';
   AResponseInfo.ContentEncoding := 'utf-8';
//   AResponseInfo.ContentText     := 'json={":start_time":"Mon Feb 08 15:23:27 -0500 2010","end_time":"Mon Feb 08 16:23:27 -0500 2010","status":1,"log":"This is a long log message","comment":"Short comment"}';
   AResponseInfo.ContentText     := '{:space_tool_id=>"aCAraK1ner34TWeJe5cbLA", :secret_key=>"34cc4c78d8f5a687a76b2eb407b3cd5705ab143e", :opts=>"{"status":"pid 3324 exit 0","start_time":"2012-10-30 16:52:23 +0400","end_time":"2012-10-30 '+' 16:52:24 +0400","log":"Updating ''C:\\thehell2\\TheHell'':\nAt revision 1.\n"}"}';
   AResponseInfo.WriteHeader;
   AResponseInfo.WriteContent;


   if ARequestInfo.Params.Count > 0 then
   begin
      button_update.Click();
      button_build.Click();
   end;


 end;

end;

procedure TMainForm.lbl_response_countClick(Sender: TObject);
begin
  request_form.Show;
  request_form.BringToFront;
end;

end.
