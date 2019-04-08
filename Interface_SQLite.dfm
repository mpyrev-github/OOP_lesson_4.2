object sqliteForm: TsqliteForm
  Left = 0
  Top = 0
  Caption = #1056#1077#1076#1072#1082#1090#1080#1088#1086#1074#1072#1085#1080#1077' '#1041#1044' SQLite'
  ClientHeight = 336
  ClientWidth = 637
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object statusLabel: TLabel
    Left = 8
    Top = 305
    Width = 3
    Height = 13
  end
  object VST_SQLite: TVirtualStringTree
    Left = 0
    Top = 0
    Width = 637
    Height = 257
    Header.AutoSizeIndex = 0
    Header.Font.Charset = DEFAULT_CHARSET
    Header.Font.Color = clWindowText
    Header.Font.Height = -11
    Header.Font.Name = 'Tahoma'
    Header.Font.Style = []
    Header.Options = [hoColumnResize, hoDrag, hoShowSortGlyphs, hoVisible]
    TabOrder = 0
    TreeOptions.SelectionOptions = [toFullRowSelect, toMultiSelect]
    OnFreeNode = VST_SQLiteFreeNode
    OnGetText = VST_SQLiteGetText
    Columns = <
      item
        Position = 0
        Width = 59
        WideText = 'ID'
      end
      item
        Position = 1
        Width = 238
        WideText = 'URL'
      end
      item
        Position = 2
        Width = 582
        WideText = 'DATA'
      end>
  end
  object pullButton: TButton
    Left = 0
    Top = 258
    Width = 177
    Height = 41
    Caption = #1047#1072#1087#1086#1083#1085#1080#1090#1100' '#1076#1072#1085#1085#1099#1084#1080' '#1080#1079' '#1041#1044
    TabOrder = 1
    OnClick = pullButtonClick
  end
  object delRowButton: TButton
    Left = 183
    Top = 258
    Width = 259
    Height = 41
    Caption = #1059#1076#1072#1083#1080#1090#1100' '#1089#1090#1088#1086#1082#1091
    TabOrder = 2
    OnClick = delRowButtonClick
  end
  object clearTableButton: TButton
    Left = 448
    Top = 258
    Width = 189
    Height = 41
    Caption = #1054#1095#1080#1089#1090#1080#1090#1100' '#1090#1072#1073#1083#1080#1094#1091
    TabOrder = 3
    OnClick = clearTableButtonClick
  end
  object closeButton: TButton
    Left = 448
    Top = 305
    Width = 189
    Height = 25
    Caption = #1047#1072#1082#1088#1099#1090#1100' '#1087#1088#1086#1075#1088#1072#1084#1084#1091
    TabOrder = 4
    OnClick = closeButtonClick
  end
end
