object Form1: TForm1
  Left = 285
  Height = 522
  Top = 143
  Width = 372
  Caption = 'Matrix 8x8'
  ClientHeight = 502
  ClientWidth = 372
  Menu = MainMenu1
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
  object imageAmpliada: TImage
    Left = 288
    Height = 80
    Top = 192
    Width = 80
    Stretch = True
  end
  object imageOriginal: TImage
    Left = 320
    Height = 8
    Top = 168
    Width = 8
  end
  object OpenDialog1: TOpenDialog
    DefaultExt = '.bmp'
    Filter = 'Imagen BMP|*.bmp'
    Options = [ofReadOnly, ofPathMustExist, ofFileMustExist, ofEnableSizing, ofViewDetail]
    Left = 312
    Top = 15
  end
  object MainMenu1: TMainMenu
    Left = 312
    Top = 120
    object MenuItem1: TMenuItem
      Caption = 'Archivo'
      object MenuItem2: TMenuItem
        Caption = 'Cargar imagen...'
        OnClick = MenuItem2Click
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object MenuItem4: TMenuItem
        Caption = 'Salir'
        OnClick = MenuItem4Click
      end
    end
  end
end
