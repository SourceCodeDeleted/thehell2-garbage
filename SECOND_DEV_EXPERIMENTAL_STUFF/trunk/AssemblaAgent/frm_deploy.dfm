object form_edit_deploy: Tform_edit_deploy
  Left = 0
  Top = 0
  Caption = #1056#1077#1076#1072#1082#1090#1080#1088#1086#1074#1072#1085#1080#1077' '#1086#1073#1088#1072#1073#1086#1090#1082#1080' deploy'
  ClientHeight = 408
  ClientWidth = 799
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object SynEdit: TSynEdit
    Left = 0
    Top = 0
    Width = 799
    Height = 408
    Align = alClient
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Courier New'
    Font.Style = []
    TabOrder = 0
    Gutter.Font.Charset = DEFAULT_CHARSET
    Gutter.Font.Color = clWindowText
    Gutter.Font.Height = -11
    Gutter.Font.Name = 'Courier New'
    Gutter.Font.Style = []
    Highlighter = SynBatSyn1
    Lines.Strings = (
      'rem '#1054#1073#1085#1086#1074#1080#1090#1100' '#1089#1086#1088#1089#1099
      'rem -------------------------------------------------------'
      
        '"c:\Program Files\TortoiseSVN\bin\svn.exe" update c:\thehell2 --' +
        'user deploybot1286762543 --password Deploy7011846'
      ''
      'rem '#1057#1086#1073#1088#1072#1090#1100' '#1089#1086#1088#1089#1099
      'rem -------------------------------------------------------'
      'cd C:\WINDOWS\Microsoft.NET\Framework\v4.0.30319\'
      
        'msbuild.exe /p:PlatformToolset=Windows7.1SDK /property:Configura' +
        'tion=Debug "C:\thehell2\trunk\thehell2.vcxproj"'
      
        '"c:\Program Files\WinRAR\WinRAR.exe" a "c:\thehell2\upload\uploa' +
        'd.rar" "c:\thehell2\upload\" ')
    WordWrap = True
    ExplicitLeft = 3
    ExplicitTop = 3
    ExplicitWidth = 788
    ExplicitHeight = 397
  end
  object SynBatSyn1: TSynBatSyn
    Left = 288
    Top = 232
  end
end
