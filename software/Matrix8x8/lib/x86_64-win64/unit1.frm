object Form1: TForm1
  Left = 285
  Height = 522
  Top = 143
  Width = 372
  Caption = 'Matrix 8x8'
  ClientHeight = 522
  ClientWidth = 372
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  LCLVersion = '7.2'
  object ColorButton1: TColorButton
    Left = 288
    Height = 25
    Top = 80
    Width = 75
    BorderWidth = 2
    ButtonColorSize = 16
    ButtonColor = clBlack
  end
  object Panel1: TPanel
    Left = 8
    Height = 272
    Top = 64
    Width = 272
    TabOrder = 0
  end
  object Label1: TLabel
    Left = 288
    Height = 15
    Top = 64
    Width = 32
    Caption = 'Color:'
    ParentColor = False
  end
  object bCodigo: TButton
    Left = 288
    Height = 25
    Top = 304
    Width = 75
    Caption = 'Código'
    OnClick = bCodigoClick
    TabOrder = 1
  end
  object Memo1: TMemo
    Left = 3
    Height = 175
    Top = 344
    Width = 360
    ScrollBars = ssAutoBoth
    TabOrder = 2
  end
  object LabeledEdit1: TLabeledEdit
    Left = 8
    Height = 23
    Top = 24
    Width = 272
    EditLabel.Height = 15
    EditLabel.Width = 272
    EditLabel.Caption = 'Nombre:'
    EditLabel.ParentColor = False
    TabOrder = 3
  end
end
