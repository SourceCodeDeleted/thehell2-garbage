object MainForm: TMainForm
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = #1040#1075#1077#1085#1090
  ClientHeight = 359
  ClientWidth = 540
  Color = clBtnFace
  Font.Charset = RUSSIAN_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  ShowHint = True
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Label18: TLabel
    Left = 27
    Top = 212
    Width = 25
    Height = 13
    Caption = #1055#1086#1088#1090
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 340
    Width = 540
    Height = 19
    Panels = <>
  end
  object PageControl: TPageControl
    Left = 0
    Top = 38
    Width = 540
    Height = 296
    ActivePage = TabSheet3
    Align = alCustom
    DoubleBuffered = False
    ParentDoubleBuffered = False
    TabOrder = 1
    object TabSheet1: TTabSheet
      Caption = #1054#1073#1097#1077#1077
      object GroupBox1: TGroupBox
        Left = 3
        Top = 6
        Width = 526
        Height = 259
        Caption = #1057#1077#1088#1074#1077#1088
        TabOrder = 0
        object Label4: TLabel
          Left = 8
          Top = 19
          Width = 61
          Height = 13
          Hint = #1050#1086#1084#1084#1072#1085#1076#1072' '#1089#1073#1086#1088#1082#1080
          Caption = #1050#1086#1084'. '#1089#1073#1086#1088#1082#1080
        end
        object Label3: TLabel
          Left = 8
          Top = 101
          Width = 25
          Height = 13
          Caption = #1055#1086#1088#1090
        end
        object Label1: TLabel
          Left = 8
          Top = 46
          Width = 79
          Height = 26
          Hint = #1050#1086#1084#1084#1072#1085#1076#1072' '#1089#1073#1086#1088#1082#1080
          Caption = #1050#1086#1084'. '#1085#1072#1089#1090#1088#1086#1081#1082#1080#13#10
        end
        object Label6: TLabel
          Left = 8
          Top = 73
          Width = 98
          Height = 26
          Hint = #1050#1086#1084#1084#1072#1085#1076#1072' '#1089#1073#1086#1088#1082#1080
          Caption = #1050#1086#1084'. '#1087#1086#1083'. '#1085#1072#1089#1090#1088#1086#1077#1082#13#10
        end
        object Label20: TLabel
          Left = 12
          Top = 207
          Width = 3
          Height = 13
        end
        object lbl_response_count: TLabel
          Left = 268
          Top = 150
          Width = 111
          Height = 13
          Cursor = crHandPoint
          Caption = #1055#1086#1083#1091#1095#1077#1085#1086' '#1079#1072#1087#1088#1086#1089#1086#1074': 0'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = [fsUnderline]
          ParentFont = False
          OnClick = lbl_response_countClick
        end
        object ed_command_deploy: TEdit
          Left = 167
          Top = 16
          Width = 200
          Height = 21
          Color = clBtnFace
          Ctl3D = True
          ParentCtl3D = False
          ReadOnly = True
          TabOrder = 0
          Text = '/deploy'
        end
        object ed_port: TEdit
          Left = 167
          Top = 98
          Width = 200
          Height = 21
          Ctl3D = True
          ParentCtl3D = False
          TabOrder = 1
          Text = '8080'
          OnChange = ed_portChange
        end
        object Edit2: TEdit
          Left = 167
          Top = 43
          Width = 200
          Height = 21
          Color = clBtnFace
          Ctl3D = True
          ParentCtl3D = False
          ReadOnly = True
          TabOrder = 2
          Text = '/configure'
        end
        object Edit4: TEdit
          Left = 167
          Top = 71
          Width = 200
          Height = 21
          Color = clBtnFace
          Ctl3D = True
          ParentCtl3D = False
          ReadOnly = True
          TabOrder = 3
          Text = '/get'
        end
        object box_listen_on_run: TCheckBox
          Left = 6
          Top = 129
          Width = 139
          Height = 17
          Caption = #1057#1083#1091#1096#1072#1090#1100' '#1087#1088#1080' '#1079#1072#1087#1091#1089#1082#1077
          TabOrder = 4
          OnClick = box_listen_on_runClick
        end
        object btn_listen: TButton
          Left = 449
          Top = 12
          Width = 70
          Height = 24
          Caption = #1057#1083#1091#1096#1072#1090#1100
          TabOrder = 5
          OnClick = btn_listenClick
        end
        object btn_dontlisten: TButton
          Left = 449
          Top = 42
          Width = 70
          Height = 24
          Caption = #1053#1077' '#1089#1083#1091#1096#1072#1090#1100
          Enabled = False
          TabOrder = 6
          OnClick = btn_dontlistenClick
        end
      end
    end
    object sheet_log: TTabSheet
      Caption = #1046#1091#1088#1085#1072#1083
      ImageIndex = 4
      object console_memo: TMemo
        Left = 0
        Top = 41
        Width = 532
        Height = 192
        ParentCustomHint = False
        Align = alClient
        Ctl3D = True
        ParentCtl3D = False
        ScrollBars = ssBoth
        TabOrder = 0
      end
      object Panel1: TPanel
        Left = 0
        Top = 0
        Width = 532
        Height = 41
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 1
        object ed_log_file: TLabeledEdit
          Left = 0
          Top = 14
          Width = 529
          Height = 21
          Ctl3D = True
          EditLabel.Width = 22
          EditLabel.Height = 13
          EditLabel.Caption = #1051#1086#1075':'
          ParentCtl3D = False
          TabOrder = 0
          OnChange = ed_log_fileChange
        end
      end
      object Panel2: TPanel
        Left = 0
        Top = 233
        Width = 532
        Height = 35
        Align = alBottom
        BevelOuter = bvNone
        TabOrder = 2
        object button_clear_log: TButton
          Left = 0
          Top = 6
          Width = 121
          Height = 25
          Caption = #1054#1095#1080#1089#1090#1080#1090#1100' '#1078#1091#1088#1085#1072#1083
          TabOrder = 0
          OnClick = button_clear_logClick
        end
      end
    end
    object TabSheet2: TTabSheet
      Caption = #1056#1077#1087#1086#1079#1080#1090#1072#1088#1080#1081
      ImageIndex = 2
      object GroupBox3: TGroupBox
        Left = 3
        Top = 0
        Width = 526
        Height = 265
        TabOrder = 0
        object Label5: TLabel
          Left = 12
          Top = 16
          Width = 79
          Height = 13
          Hint = #1050#1086#1084#1084#1072#1085#1076#1072' '#1089#1073#1086#1088#1082#1080
          Caption = 'PROJECT_NAME'
        end
        object Label7: TLabel
          Left = 12
          Top = 43
          Width = 82
          Height = 13
          Hint = #1050#1086#1084#1084#1072#1085#1076#1072' '#1089#1073#1086#1088#1082#1080
          Caption = 'SPACE_TOOL_ID'
        end
        object Label8: TLabel
          Left = 12
          Top = 70
          Width = 62
          Height = 13
          Hint = #1050#1086#1084#1084#1072#1085#1076#1072' '#1089#1073#1086#1088#1082#1080
          Caption = 'SECRET_KEY'
        end
        object Label9: TLabel
          Left = 12
          Top = 97
          Width = 51
          Height = 13
          Hint = #1050#1086#1084#1084#1072#1085#1076#1072' '#1089#1073#1086#1088#1082#1080
          Caption = 'POST_URL'
        end
        object Label10: TLabel
          Left = 12
          Top = 124
          Width = 46
          Height = 13
          Hint = #1050#1086#1084#1084#1072#1085#1076#1072' '#1089#1073#1086#1088#1082#1080
          Caption = 'SCM_URL'
        end
        object Label11: TLabel
          Left = 12
          Top = 151
          Width = 58
          Height = 13
          Hint = #1050#1086#1084#1084#1072#1085#1076#1072' '#1089#1073#1086#1088#1082#1080
          Caption = 'SCM_LOGIN'
        end
        object Label12: TLabel
          Left = 12
          Top = 178
          Width = 84
          Height = 13
          Hint = #1050#1086#1084#1084#1072#1085#1076#1072' '#1089#1073#1086#1088#1082#1080
          Caption = 'SCM_PASSWORD'
        end
        object Label14: TLabel
          Left = 12
          Top = 205
          Width = 29
          Height = 13
          Hint = #1050#1086#1084#1084#1072#1085#1076#1072' '#1089#1073#1086#1088#1082#1080
          Caption = 'config'
        end
        object Label13: TLabel
          Left = 12
          Top = 233
          Width = 322
          Height = 13
          Caption = #1042#1099#1087#1086#1083#1085#1080#1090#1077' '#1089#1073#1088#1086#1089' '#1095#1090#1086#1073#1099' '#1086#1090#1087#1088#1072#1074#1080#1090#1100' '#1085#1086#1074#1099#1077' '#1085#1072#1089#1090#1088#1086#1081#1082#1080' configure.'
        end
        object Label15: TLabel
          Left = 12
          Top = 247
          Width = 361
          Height = 13
          Caption = 
            #1042#1099#1087#1086#1083#1085#1080#1090#1077' configure '#1085#1072' '#1089#1077#1088#1074#1077#1088#1077' assembla '#1095#1090#1086#1073#1099' '#1079#1072#1087#1086#1083#1085#1080#1090#1100' '#1085#1072#1089#1090#1088#1086#1081#1082 +
            #1080'.'
        end
        object ed_project_name: TEdit
          Left = 114
          Top = 13
          Width = 200
          Height = 21
          BevelInner = bvNone
          BevelOuter = bvRaised
          Color = clBtnFace
          Ctl3D = True
          ParentCtl3D = False
          ReadOnly = True
          TabOrder = 0
        end
        object ed_space_tool: TEdit
          Left = 114
          Top = 40
          Width = 200
          Height = 21
          BevelInner = bvNone
          BevelOuter = bvNone
          Color = clBtnFace
          Ctl3D = True
          ParentCtl3D = False
          ReadOnly = True
          TabOrder = 1
        end
        object ed_secret_key: TEdit
          Left = 114
          Top = 67
          Width = 200
          Height = 21
          BevelInner = bvNone
          BevelOuter = bvNone
          Color = clBtnFace
          Ctl3D = True
          ParentCtl3D = False
          ReadOnly = True
          TabOrder = 2
        end
        object ed_post_url: TEdit
          Left = 114
          Top = 96
          Width = 200
          Height = 21
          BevelInner = bvNone
          BevelOuter = bvNone
          Color = clBtnFace
          Ctl3D = True
          ParentCtl3D = False
          ReadOnly = True
          TabOrder = 3
        end
        object ed_scm_url: TEdit
          Left = 114
          Top = 121
          Width = 200
          Height = 21
          BevelInner = bvNone
          BevelOuter = bvNone
          Color = clBtnFace
          Ctl3D = True
          ParentCtl3D = False
          TabOrder = 4
          OnChange = ed_scm_urlChange
        end
        object ed_scm_login: TEdit
          Left = 114
          Top = 148
          Width = 200
          Height = 21
          BevelInner = bvNone
          BevelOuter = bvNone
          Ctl3D = True
          ParentCtl3D = False
          TabOrder = 5
          OnChange = ed_scm_loginChange
        end
        object ed_scm_password: TEdit
          Left = 114
          Top = 175
          Width = 200
          Height = 21
          BevelInner = bvNone
          BevelOuter = bvNone
          Ctl3D = True
          ParentCtl3D = False
          TabOrder = 6
          OnChange = ed_scm_passwordChange
        end
        object ed_config: TEdit
          Left = 114
          Top = 202
          Width = 200
          Height = 21
          BevelInner = bvNone
          BevelOuter = bvNone
          Color = clBtnFace
          Ctl3D = True
          ParentCtl3D = False
          ReadOnly = True
          TabOrder = 7
        end
        object Button4: TButton
          Left = 444
          Top = 14
          Width = 75
          Height = 25
          Caption = #1057#1073#1088#1086#1089#1080#1090#1100
          TabOrder = 8
          OnClick = Button4Click
        end
        object Button2: TButton
          Left = 444
          Top = 45
          Width = 75
          Height = 25
          Caption = #1055#1086#1089#1090' '#1079#1072#1087#1088#1086#1089
          TabOrder = 9
          Visible = False
          OnClick = Button2Click
        end
      end
    end
    object TabSheet3: TTabSheet
      Caption = #1057#1073#1086#1088#1082#1072
      ImageIndex = 3
      object GroupBox2: TGroupBox
        Left = 3
        Top = 3
        Width = 526
        Height = 263
        TabOrder = 0
        object Label19: TLabel
          Left = 10
          Top = 16
          Width = 65
          Height = 13
          Caption = #1056#1077#1087#1086#1079#1080#1090#1072#1088#1080#1081
        end
        object Label17: TLabel
          Left = 10
          Top = 69
          Width = 71
          Height = 13
          Caption = #1060#1072#1081#1083' '#1087#1088#1086#1077#1082#1090#1072
        end
        object Label2: TLabel
          Left = 10
          Top = 97
          Width = 35
          Height = 13
          Caption = 'msbuild'
        end
        object Label21: TLabel
          Left = 10
          Top = 124
          Width = 17
          Height = 13
          Caption = 'svn'
        end
        object ed_project_path: TButtonedEdit
          Left = 92
          Top = 67
          Width = 200
          Height = 21
          BevelInner = bvNone
          BevelOuter = bvNone
          Ctl3D = True
          Images = ImageList1
          ParentCtl3D = False
          RightButton.HotImageIndex = 0
          RightButton.ImageIndex = 0
          RightButton.PressedImageIndex = 0
          RightButton.Visible = True
          TabOrder = 0
          OnChange = ed_project_pathChange
          OnKeyUp = ed_project_pathKeyUp
          OnRightButtonClick = ed_project_pathRightButtonClick
        end
        object ed_configuration: TLabeledEdit
          Left = 92
          Top = 40
          Width = 200
          Height = 21
          BevelInner = bvNone
          BevelOuter = bvNone
          Ctl3D = True
          EditLabel.Width = 74
          EditLabel.Height = 13
          EditLabel.Caption = #1050#1086#1085#1092#1080#1075#1091#1088#1072#1094#1080#1103
          EditLabel.Transparent = False
          LabelPosition = lpLeft
          LabelSpacing = 8
          ParentCtl3D = False
          TabOrder = 1
          OnChange = ed_configurationChange
        end
        object ed_repo: TButtonedEdit
          Left = 92
          Top = 13
          Width = 200
          Height = 21
          BevelInner = bvNone
          BevelOuter = bvNone
          Ctl3D = True
          Images = ImageList1
          ParentCtl3D = False
          RightButton.HotImageIndex = 0
          RightButton.ImageIndex = 0
          RightButton.PressedImageIndex = 0
          RightButton.Visible = True
          TabOrder = 2
          OnChange = ed_repoChange
          OnKeyUp = ed_repoKeyUp
          OnRightButtonClick = ed_repoRightButtonClick
        end
        object button_checkout: TButton
          Left = 382
          Top = 12
          Width = 130
          Height = 24
          Caption = 'checkout'
          TabOrder = 3
          OnClick = button_checkoutClick
        end
        object button_info: TButton
          Left = 382
          Top = 197
          Width = 130
          Height = 25
          Caption = #1050#1086#1084#1084#1072#1085#1076#1072' info'
          TabOrder = 4
          OnClick = button_infoClick
        end
        object button_clear: TButton
          Left = 382
          Top = 42
          Width = 130
          Height = 25
          Caption = #1054#1095#1080#1089#1090#1080#1090#1100' '#1088#1077#1087#1086#1079#1080#1090#1072#1088#1080#1081
          TabOrder = 5
          OnClick = button_clearClick
        end
        object Button1: TButton
          Left = 382
          Top = 73
          Width = 130
          Height = 25
          Caption = #1055#1086#1082#1072#1079#1072#1090#1100' '#1088#1077#1087#1086#1079#1080#1090#1072#1088#1080#1081
          TabOrder = 6
          OnClick = button_clearClick
        end
        object button_update: TButton
          Left = 382
          Top = 135
          Width = 130
          Height = 25
          Caption = #1054#1073#1085#1086#1074#1080#1090#1100' '#1080#1089#1093#1086#1076#1085#1080#1082#1080
          TabOrder = 7
          OnClick = button_updateClick
        end
        object ed_version: TLabeledEdit
          Left = 92
          Top = 148
          Width = 200
          Height = 21
          BevelInner = bvNone
          BevelOuter = bvNone
          Ctl3D = True
          EditLabel.Width = 35
          EditLabel.Height = 13
          EditLabel.Caption = #1042#1077#1088#1089#1080#1103
          LabelPosition = lpLeft
          LabelSpacing = 48
          ParentCtl3D = False
          ReadOnly = True
          TabOrder = 8
          OnChange = ed_versionChange
        end
        object Button6: TButton
          Left = 382
          Top = 166
          Width = 130
          Height = 25
          Caption = #1055#1086#1083#1091#1095#1080#1090#1100' '#1074#1077#1088#1089#1080#1102
          TabOrder = 9
          OnClick = Button6Click
        end
        object ed_msbuild: TButtonedEdit
          Left = 92
          Top = 94
          Width = 200
          Height = 21
          BevelInner = bvNone
          BevelOuter = bvNone
          Ctl3D = True
          Images = ImageList1
          ParentCtl3D = False
          RightButton.HotImageIndex = 0
          RightButton.ImageIndex = 0
          RightButton.PressedImageIndex = 0
          RightButton.Visible = True
          TabOrder = 10
          OnChange = ed_msbuildChange
          OnKeyUp = ed_msbuildKeyUp
          OnRightButtonClick = ed_msbuildRightButtonClick
        end
        object ed_svn: TButtonedEdit
          Left = 92
          Top = 121
          Width = 200
          Height = 21
          BevelInner = bvNone
          BevelOuter = bvNone
          Ctl3D = True
          Images = ImageList1
          ParentCtl3D = False
          RightButton.HotImageIndex = 0
          RightButton.ImageIndex = 0
          RightButton.PressedImageIndex = 0
          RightButton.Visible = True
          TabOrder = 11
          OnChange = ed_svnChange
          OnKeyUp = ed_svnKeyUp
          OnRightButtonClick = ed_svnRightButtonClick
        end
        object button_getpath: TButton
          Left = 382
          Top = 104
          Width = 130
          Height = 25
          Caption = #1055#1086#1083#1091#1095#1080#1090#1100' '#1087#1091#1090#1100' .NET'
          TabOrder = 12
          OnClick = button_getpathClick
        end
        object button_build: TButton
          Left = 382
          Top = 228
          Width = 130
          Height = 25
          Caption = #1057#1086#1073#1088#1072#1090#1100
          TabOrder = 13
          OnClick = button_buildClick
        end
      end
    end
  end
  object ToolBar1: TToolBar
    Left = 0
    Top = 0
    Width = 540
    Height = 25
    ButtonHeight = 21
    ButtonWidth = 56
    Caption = 'ToolBar1'
    EdgeBorders = [ebTop]
    ShowCaptions = True
    TabOrder = 2
    Transparent = False
    object ToolButton1: TToolButton
      Left = 0
      Top = 0
      Caption = #1044#1077#1081#1089#1090#1074#1080#1103
      ImageIndex = 0
      Style = tbsDropDown
    end
    object ToolButton2: TToolButton
      Left = 69
      Top = 0
      Caption = #1054#1073#1084#1077#1085
      ImageIndex = 1
    end
  end
  object HTTPServer: TIdHTTPServer
    Bindings = <>
    OnCommandGet = HTTPServerCommandGet
    Left = 339
    Top = 202
  end
  object SynBatSyn1: TSynBatSyn
    Left = 335
    Top = 50
  end
  object ImageList1: TImageList
    Left = 335
    Top = 259
    Bitmap = {
      494C010101000300040010001000FFFFFFFFFF00FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000400000001000000001002000000000000010
      000000000000000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFFFFC8D0D400C8D0D400C8D0
      D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0
      D400C8D0D400C8D0D400C8D0D400FFFFFFFF0000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFFFFC8D0D400C8D0D400C8D0
      D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0
      D400C8D0D400C8D0D400C8D0D400FFFFFFFF0000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFFFFC8D0D400C8D0D400C8D0
      D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0
      D400C8D0D400C8D0D400C8D0D400FFFFFFFF0000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFFFFC8D0D400C8D0D400C8D0
      D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0
      D400C8D0D400C8D0D400C8D0D400FFFFFFFF0000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFFFFC8D0D400C8D0D400C8D0
      D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0
      D400C8D0D400C8D0D400C8D0D400FFFFFFFF0000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFFFFC8D0D400C8D0D400C8D0
      D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0
      D400C8D0D400C8D0D400C8D0D400FFFFFFFF0000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFFFFC8D0D400C8D0D400C8D0
      D4000000000000000000C8D0D400C8D0D4000000000000000000C8D0D400C8D0
      D4000000000000000000C8D0D400FFFFFFFF0000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFFFFC8D0D400C8D0D400C8D0
      D4000000000000000000C8D0D400C8D0D4000000000000000000C8D0D400C8D0
      D4000000000000000000C8D0D400FFFFFFFF0000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFFFFC8D0D400C8D0D400C8D0
      D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0
      D400C8D0D400C8D0D400C8D0D400FFFFFFFF0000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFFFFC8D0D400C8D0D400C8D0
      D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0
      D400C8D0D400C8D0D400C8D0D400FFFFFFFF0000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFFFFC8D0D400C8D0D400C8D0
      D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0
      D400C8D0D400C8D0D400C8D0D400FFFFFFFF0000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFFFFC8D0D400C8D0D400C8D0
      D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0
      D400C8D0D400C8D0D400C8D0D400FFFFFFFF0000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFFFFC8D0D400C8D0D400C8D0
      D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0D400C8D0
      D400C8D0D400C8D0D400C8D0D400FFFFFFFF0000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00FFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000040000000100000000100010000000000800000000000000000000000
      000000000000000000000000FFFFFF0000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000}
  end
  object FolderDialog1: TFolderDialog
    DialogX = 0
    DialogY = 0
    Version = '1.1.0.1'
    Left = 338
    Top = 103
  end
  object OpenDialog1: TOpenDialog
    Ctl3D = False
    DefaultExt = '*.*'
    FileName = '*.*'
    Filter = '*.*'
    InitialDir = 'c:\'
    Left = 336
    Top = 154
  end
  object SkinManager: TsSkinManager
    Active = False
    InternalSkins = <>
    MenuSupport.IcoLineSkin = 'ICOLINE'
    MenuSupport.ExtraLineFont.Charset = DEFAULT_CHARSET
    MenuSupport.ExtraLineFont.Color = clWindowText
    MenuSupport.ExtraLineFont.Height = -11
    MenuSupport.ExtraLineFont.Name = 'Tahoma'
    MenuSupport.ExtraLineFont.Style = []
    SkinDirectory = 'D:\SkinEditor2013\Work'
    SkinName = 'enterprise'
    SkinInfo = 'N/A'
    ThirdParty.ThirdEdits = 
      'TButtonedEdit'#13#10'TEdit'#13#10'TMemo'#13#10'TMaskEdit'#13#10'TLabeledEdit'#13#10'THotKey'#13#10'T' +
      'ListBox'#13#10'TCheckListBox'#13#10'TRichEdit'#13#10'TSpinEdit'#13#10'TDateTimePicker'#13#10'T' +
      'CategoryButtons'#13#10'TFileListBox'#13#10
    ThirdParty.ThirdButtons = 'TButton'#13#10
    ThirdParty.ThirdBitBtns = 'TBitBtn'#13#10
    ThirdParty.ThirdCheckBoxes = 'TCheckBox'#13#10'TRadioButton'#13#10'TGroupButton'#13#10
    ThirdParty.ThirdGroupBoxes = 'TGroupBox'#13#10'TRadioGroup'#13#10
    ThirdParty.ThirdListViews = 'TListView'#13#10
    ThirdParty.ThirdPanels = 'TPanel'#13#10'TPage'#13#10'TGridPanel'#13#10
    ThirdParty.ThirdGrids = 'TStringGrid'#13#10'TDrawGrid'#13#10'TValueListEditor'#13#10
    ThirdParty.ThirdTreeViews = 'TTreeView'#13#10
    ThirdParty.ThirdComboBoxes = 'TCombobox'#13#10'TComboBox'#13#10'TComboBoxEx'#13#10'TColorBox'#13#10
    ThirdParty.ThirdWWEdits = ' '#13#10
    ThirdParty.ThirdVirtualTrees = ' '#13#10
    ThirdParty.ThirdGridEh = ' '#13#10
    ThirdParty.ThirdPageControl = 'TPageControl'#13#10
    ThirdParty.ThirdTabControl = 'TTabControl'#13#10
    ThirdParty.ThirdToolBar = 'TToolBar'#13#10
    ThirdParty.ThirdStatusBar = 'TStatusBar'#13#10
    ThirdParty.ThirdSpeedButton = 'TSpeedButton'#13#10
    ThirdParty.ThirdScrollControl = 'TScrollBox'#13#10
    ThirdParty.ThirdUpDown = 'TSpinButton'#13#10'TUpDown'#13#10
    ThirdParty.ThirdScrollBar = 'TScrollBar'#13#10
    Left = 48
    Top = 240
  end
  object MainMenu1: TMainMenu
    Left = 408
    Top = 48
    object N1: TMenuItem
      Caption = #1057#1083#1091#1078#1073#1072
      object N2: TMenuItem
        Caption = #1059#1089#1090#1072#1085#1086#1074#1080#1090#1100
      end
      object N3: TMenuItem
        Caption = #1059#1076#1072#1083#1080#1090#1100
      end
      object N4: TMenuItem
        Caption = #1047#1072#1087#1091#1089#1090#1080#1090#1100
      end
      object N5: TMenuItem
        Caption = #1054#1089#1090#1072#1085#1086#1074#1080#1090#1100
      end
    end
  end
end
