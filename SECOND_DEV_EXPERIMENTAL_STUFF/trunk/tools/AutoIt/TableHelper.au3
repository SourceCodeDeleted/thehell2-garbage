; линк на скачку автоита
; https://www.autoitscript.com/files/autoit3/autoit-v3-setup.exe
; http://www.autoitscript.com/cgi-bin/getfile.pl?../autoit3/scite/download/SciTE4AutoIt3.exe

#include <IE.au3>
#include <GUIConstantsEx.au3>
#include <WindowsConstants.au3>

; Register a customer error handler
;_IEErrorHandlerRegister("MyErrFunc")
; Do something
$oMyError = 1;= ObjEvent("AutoIt.Error","MyErrFunc")

Global $F5 = false
Local $oVBS = ObjCreate("ScriptControl")
$oVBS.language = "VBScript"
Global Const $Nothing = $oVBS.eval("Nothing")
$oVBS = $Nothing

Opt("WinTitleMatchMode", 1)
Opt("SendKeyDownDelay", Default)
Opt("SendKeyDelay", Default)
;Opt("GUICoordMode", 2)
Opt("GUIOnEventMode", 1)

$hGUI = GUICreate("ExcelVsIda",200,100)
GUISetOnEvent($GUI_EVENT_CLOSE, "SpecialEvents")
$idExit = GUICtrlCreateButton("Exit", 50, 50, 85, 25)
GUICtrlSetOnEvent(-1, "ExitPressed")
GUISetState(@SW_SHOW)

Local $sel = GetExcelWebApp()
ConsoleWrite( $sel.innertext & @CRLF )

;Присоединятся к COM интерфейсу первой запущеной студии
;для присоединения к другой нужно написать маршаллинг RunningObjectTable
Local $vsOpened = false
$vs = ObjGet( "", "VisualStudio.DTE.14.0" )
If @error Then
   $vs = ShellExecute( "devenv.exe" )
   For $i = 1 to 10
	  $vs = ObjGet( "", "VisualStudio.DTE.14.0" )
	  If @error Then
		 Sleep( 1000 )
	  Else
		 $vsOpened = true
		 ExitLoop
	  EndIf
   Next
   Sleep( 5000 )
Else
   ConsoleWrite( $vs.ActiveWindow.Caption & @CRLF )
   $vsOpened = true
EndIf
If $vsOpened = false Then Exit
$com = $vs.Commands
$findResult = $com.item( "Edit.GoToFindResults1Location" )

Local $oldVal = $sel.innertext
ConsoleWrite( $oldVal & @CRLF )
Local $newVal
While True
   Sleep( 50 )
   $newVal = $sel.innertext
   if @error <> 0 then Exit
   if $newVal <> $oldVal Then
	  $oldVal = $newVal
	  ConsoleWrite( $newVal & @CRLF )
	  PositionCell( $newVal )
   EndIf
WEnd

; Deregister the customer error handler
;_IEErrorHandlerDeRegister()
; Do something else
; Register the default IE.au3 COM Error Handler
;_IEErrorHandlerRegister()
; Do more work

Exit

Func ExitPressed()
    Exit
	;MsgBox(0, "OK Pressed", "ID=" & @GUI_CtrlId & " WinHandle=" & @GUI_WinHandle & " CtrlHandle=" & @GUI_CtrlHandle)
EndFunc   ;==>OKPressed

Func SpecialEvents()
    Select
        Case @GUI_CtrlId = $GUI_EVENT_CLOSE
            Exit
        ;Case @GUI_CtrlId = $GUI_EVENT_MINIMIZE
        ;Case @GUI_CtrlId = $GUI_EVENT_RESTORE
    EndSelect
EndFunc

Func GetExcelWebApp()
   Local $ie = _IEAttach( "https://excel.officeapps.live.com/x", "URL" )
   If @error <> 0 Then
	  ConsoleWrite("Откройте excel online вручную!");добавить обработку обычного excel, что ли
	  Exit
	  $ie = _IEAttach( "Global Funcs TH2", "Title" )
	  If @error <> 0 Then
		 $ie = _IECreate("https://onedrive.live.com/edit.aspx?cid=f19367ede5c041e3&page=view&resid=F19367EDE5C041E3!450&parId=F19367EDE5C041E3!440&app=Excel&wacqt=undefined")
		 Sleep( 200 )
	  EndIf
	  ;ControlSend( "Global Funcs TH2", "", "[CLASS:Internet Explorer_Server; INSTANCE:1]", "{F12}" )
	  ;Sleep( 200 )
	  ;_IELoadWait( $ie )
	  ;Sleep( 200 )
	  ;ControlSetText( "Global Funcs TH2", "", "[CLASS:Edit; INSTANCE:5]", "sdx_ow_iframe"" src" )
	  ;ControlSend( "Global Funcs TH2", "", "[CLASS:Edit; INSTANCE:5]", "{ENTER}" )
	  ;Sleep( 200 )
	  ;WinActivate( "Global Funcs TH2" )
	  ;ControlFocus( "Global Funcs TH2", "", "[CLASS:SysTreeView32; INSTANCE:7]" )
	  ;Sleep( 50 )
	  ;Send( "^c" )
	  ;Sleep( 50 )
	  ;Local $html = ClipGet()
	  ;ConsoleWrite( $html & @CRLF )
	  ;Local $frameSdxUrlBegin = StringInStr( $html, "http" )
	  ;Local $frameSdxUrlEnd = StringInStr( $html, """", 0, 1, $frameSdxUrlBegin )
	  ;Local $url = StringMid( $html, $frameSdxUrlBegin, $frameSdxUrlEnd - $frameSdxUrlBegin )
	  ;ControlSend( "Global Funcs TH2", "", "[CLASS:Internet Explorer_Server; INSTANCE:1]", "{F12}" )
	  ;ConsoleWrite( $url & @CRLF )
	  ;_IENavigate( $ie, $url, 1 )
	  ;Sleep( 1000 )
	  ;_IELoadWait( $ie )
	  ;Sleep( 1000 )
   EndIf
   ;return _IEGetObjByClass($ie, "m_excelWebRenderer_ewaCtl_gridReadingDiv")
   return _IEGetObjByClass ($ie, "ewa-gridReader")

EndFunc

Func PositionCell( ByRef $cell )
   ;Local $valBegin = StringInStr( $cell, " " ) + 1
   ;if $valBegin = 1 Then Return
   ;Local $len = StringLen( $cell )
   ;Local $val = StringRight( $cell, $len - $valBegin + 1)
   ;$val = StringStripWS( $val, 8 )
   ;if $val = "" then Return
   ;Local $ofs = StringLeft( $cell, $valBegin - 1 )
   ;if StringInStr( $ofs, ":" ) then return
   Local $celArr = StringSplit( $cell, " . ", 1 )
   Local $val = $celArr[1]
   Local $ofs = $celArr[2]
   Local $colArr = StringSplit( $ofs, " " )
   Local $col = $colArr[1]
   Local $excel
   if $col = "A" Then
	  ;тут имена - позиционируем студию по имени
	  ConsoleWrite( $val & @CRLF )
	  GotoVsName( $val )
	  WinActivate( "Excel Online" );Web App" )
   ElseIf $col = "B" Then
	  ;тут адреса - позиционируем иду по адресу
	  ;ConsoleWrite( StringIsXDigit( $val ) & @CRLF )
	  ConsoleWrite( $val & @CRLF )
	  If StringIsXDigit( $val ) = 1 Then
		 GotoIda( $val )
		 $excel = WinActivate( "Excel Online" );Web App" )
		 ControlFocus($excel,"",$excel)
	  EndIf
   ElseIf $col = "T" Then
	  ;тут адреса - позиционируем иду и студию по адресу
	  ;ConsoleWrite( StringIsXDigit( $val ) & @CRLF )
	  ConsoleWrite( $val & @CRLF )
	  If StringIsXDigit( $val ) = 1 Then
		 GotoIda( $val )
		 GotoVsAdr( $val )
		 $excel = WinActivate( "Excel Online" );Web App" )
		 ControlFocus($excel,"","[CLASS:Internet Explorer_Server]")
	  EndIf
   EndIf
EndFunc

Func GotoVsName( ByRef $name )
   If WinExists( "thehell2_vNext" ) Then
	  ;$vs.ExecuteCommand( "VAssistX.FindSymbolDialog" )
	  WinActivate( "thehell2_vNext" )
	  Send( "^!s" )
	  local $fs = WinWait( "Find Symbol", "", 3 )
	  ControlSetText( $fs, "", "[CLASS:Edit; INSTANCE:1]", $name )
	  ControlClick( $fs, "", "[CLASS:Button; INSTANCE:3]" )
	  WinWaitClose( $fs )
   EndIf
EndFunc

Func GotoVsAdr( ByRef $adr )
   If WinExists( "thehell2_vNext" ) Then
	  $vs.ExecuteCommand( "Edit.FindinFiles /reset" )
	  $vs.ExecuteCommand( "Edit.FindInFiles (" & $adr & ") /lookin:""Current Project""" )
	  Local $oErrorHandler = ObjEvent("AutoIt.Error", "_ErrFunc1")
	  ;$vs.ExecuteCommand( "View.FindResults1" )
	  For $i = 1 to 10
		 sleep( 100 )
		 If $findResult.IsAvailable Then
			$vs.ExecuteCommand( "Edit.GoToFindResults1Location" )
			ExitLoop
		 EndIf
	  Next
	  $oErrorHandler = ""
	  $vs.ExecuteCommand( "View.FindResults1" )
	  ;$vs.ExecuteCommand( "Window.CloseDocumentWindow" )
   EndIf
EndFunc

Func _ErrFunc1($oError)

EndFunc

Func GotoIda( Const ByRef $adr )
	If WinExists( "IDA" ) Then
		WinActivate( "IDA" )
		;$ida = WinWait("IDA", "", 10)
		Local $ida = WinGetHandle("[TITLE:IDA; CLASS:QWidget]")
		ConsoleWrite(WinGetTitle($ida) & @CRLF )
		ControlFocus($ida, "", $ida)
		Opt("SendKeyDelay", 10)
		if $F5 = true then ; закрываем предыдущее окно декомпила
			;ControlSend( $ida, "", $ida, "!3")
			ControlSend( $ida, "", $ida, "!{F3}" )
			$F5 = False
		EndIf
		;ControlSend($ida, "", $ida, "!2") ; переходим по новому адресу
		ControlSend($ida, "", $ida, "g")
		Local $jump = WinWait("[TITLE:Jump to address; CLASS:QWidget]","",1)
		if $jump = 0 Then
			ConsoleWrite("Ёпа мопа!" & @CRLF )
			;exit
			ControlSend($ida, "", $ida, "{ESC}")
			ControlSend($ida, "", $ida, "!2")
			ControlSend($ida, "", $ida, "g")
			$jump = WinWait("[TITLE:Jump to address; CLASS:QWidget]","",1)
		EndIf
		Opt("SendKeyDelay", Default)
		ConsoleWrite(WinGetTitle($jump) & @CRLF )
		;ControlSetText( $jump, "", $jump, "004743AB", 1 ) ;меняет текст у названия окна
		ControlFocus($jump, "", $jump)
		Opt("SendKeyDownDelay", 0)
		ControlSend($jump, "", $jump, $adr)
		Opt("SendKeyDownDelay", Default)
		ControlSend( $jump, "", $jump, "{Enter}{Enter}" )
		WinWaitClose( $jump )
		ControlSend( $ida, "", $ida, "{F5}" ); декомпил, все время открывает новые окна
		$F5 = true
		;ControlSend( $ida, "", $ida, "." ) ; Python or IDC command prompt
		;Можно добавить сверку сигнатур и замену командой прямо тут
	Else
	  ConsoleWrite( "IDA not found" & @crlf)
	EndIf
EndFunc

Func _IEGetObjByClass(ByRef $o_object, $s_Class, $i_index = 0)
    Local $i_found = 0
    ;
    $o_tags = _IETagNameAllGetCollection($o_object)
    For $o_tag In $o_tags
        If String($o_tag.className) = $s_Class Then
            If ($i_found = $i_index) Then
                SetError($_IEStatus_Success)
                Return $o_tag
            Else
                $i_found += 1
            EndIf
        EndIf
    Next
    ;
    ;__IEErrorNotify("Warning", "_IEGetObjByClass", "$_IEStatus_NoMatch", $s_Class)
    SetError($_IEStatus_NoMatch, 2)
    Return 0
EndFunc   ;==>_IEGetObjByClass

Func MyErrFunc()
  $HexNumber=hex($oMyError.number,8)
  ConsoleWrite("_______------- Error -------_______"       & @CRLF & _
             "err.description is: "    & @TAB & $oMyError.description    & @CRLF & _
             "err.windescription:"     & @TAB & $oMyError.windescription & @CRLF & _
             "err.number is: "         & @TAB & $HexNumber              & @CRLF & _
             "err.lastdllerror is: "   & @TAB & $oMyError.lastdllerror   & @CRLF & _
             "err.scriptline is: "     & @TAB & $oMyError.scriptline     & @CRLF & _
             "err.source is: "         & @TAB & $oMyError.source         & @CRLF & _
             "err.helpfile is: "       & @TAB & $oMyError.helpfile       & @CRLF & _
             "err.helpcontext is: "    & @TAB & $oMyError.helpcontext _
            )
  SetError(0)  ; to check for after this function returns
Endfunc