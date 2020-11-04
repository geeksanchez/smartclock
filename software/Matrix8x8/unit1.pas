unit Unit1;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, Forms, Controls, Graphics, Dialogs, ExtCtrls, ColorBox,
  StdCtrls, Menus;

type

  TShapeArray = array[0..63] of TShape;

  { TForm1 }

  TForm1 = class(TForm)
    bCodigo: TButton;
    ColorButton1: TColorButton;
    imageAmpliada: TImage;
    imageOriginal: TImage;
    Label1: TLabel;
    LabeledEdit1: TLabeledEdit;
    MainMenu1: TMainMenu;
    Memo1: TMemo;
    MenuItem1: TMenuItem;
    MenuItem2: TMenuItem;
    N1: TMenuItem;
    MenuItem4: TMenuItem;
    OpenDialog1: TOpenDialog;
    Panel1: TPanel;

    procedure MenuItem2Click(Sender: TObject);
    procedure MenuItem4Click(Sender: TObject);
    procedure paintImage(colour: TColor; posX, posY: Integer);
    procedure bCodigoClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure ShapeMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure ShapeMouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
  private

  public
    Shapes: TShapeArray;

  end;

var
  Form1: TForm1;

implementation

{$R *.frm}

uses
  StrUtils;

{ TForm1 }

procedure TForm1.paintImage(colour: TColor; posX, posY: Integer);
begin
  imageOriginal.Canvas.Pixels[posX, posY] := colour;
  imageAmpliada.Picture := imageOriginal.Picture;
end;

procedure TForm1.MenuItem2Click(Sender: TObject);
var
  posX, posY: Integer;
begin
  if (OpenDialog1.Execute) then
  begin
    LabeledEdit1.Text := AnsiReplaceStr(ExtractFileName(OpenDialog1.FileName), '.bmp', '');
    imageOriginal.Picture.LoadFromFile(OpenDialog1.FileName);
    imageAmpliada.Picture := imageOriginal.Picture;
    for posX := 0 to 7 do
      for posY := 0 to 7 do
        begin
          Shapes[posX + posY * 8].Brush.Color := imageOriginal.Canvas.Pixels[posX, posY];
        end;
  end;
end;

procedure TForm1.MenuItem4Click(Sender: TObject);
begin
  Close;
end;

procedure TForm1.bCodigoClick(Sender: TObject);
var
  r, g, b: byte;
  data: String;
  i, j: Integer;
begin
  imageOriginal.Picture.SaveToFile(LabeledEdit1.Text + '.bmp');
  Memo1.Lines.Add('const uint32_t ' + LabeledEdit1.Text + '[] PROGMEM =');
  Memo1.Lines.Add('    {');
  for j:=0 to 7 do
  begin
    data := '';
    for i:=0 to 7 do
    begin
      b := (Shapes[i+j*8].Brush.Color shr 16) AND $FF;
      g := (Shapes[i+j*8].Brush.Color shr 8) AND $FF;
      r := (Shapes[i+j*8].Brush.Color) AND $FF;
      if ((j = 7) and (i = 7)) then
      begin
        data := data + '0xFF' + IntToHex(b, 2) + IntToHex(g, 2) + IntToHex(r, 2) + '};';
      end
      else
      begin
        data := data + '0xFF' + IntToHex(b, 2) + IntToHex(g, 2) + IntToHex(r, 2) + ', ';
      end;
    end;
    Memo1.Lines.Add(data);
  end;
end;

procedure TForm1.FormCreate(Sender: TObject);
var
  i, j: Integer;
begin
  for j:=0 to 7 do
    for i:=0 to 7 do
    begin
      Shapes[i+j*8] := TShape.Create(Panel1);
      with Shapes[i+j*8] do
      begin
        Parent := Panel1;
        Height := 32;
        Width := 32;
        Left := 8 + i*Width;
        Top := 8 + j*Height;
        OnMouseUp := @ShapeMouseUp;
        onMouseMove := @ShapeMouseMove;
        Shape := stRectangle;
        Name := 'shapeX' + IntToStr(i) + 'Y' + IntToStr(j);
        Brush.Color := clBlack;
        Pen.Color := clWhite;
        Visible := True;
      end;
    end;
end;

procedure TForm1.FormDestroy(Sender: TObject);
var
  i, j: Integer;
begin
  for j:=0 to 7 do
    for i:=0 to 7 do
    begin
      Shapes[i+j*8].Free;
    end;
end;

procedure TForm1.ShapeMouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Integer);
var
  posX, posY: Integer;
begin
  if (Shift = [ssShift]) then
  begin
    with (Sender as TShape) do
    begin
      Brush.Color := ColorButton1.ButtonColor;
      posX := StrToInt(copy(Name, Pos('X', Name) + 1, Pos('Y', Name) - Pos('X', Name) - 1));
      posY := StrToInt(copy(Name, Pos('Y', name) + 1, Name.length - Pos('Y', Name)));
      paintImage(Brush.Color, posX, posY);
    end;
  end
  else if (Shift = [ssCtrl]) then
  begin
    with (Sender as TShape) do
    begin
      Brush.Color := clBlack;
      posX := StrToInt(copy(Name, Pos('X', Name) + 1, Pos('Y', Name) - Pos('X', Name) - 1));
      posY := StrToInt(copy(Name, Pos('Y', name) + 1, Name.length - Pos('Y', Name)));
      paintImage(Brush.Color, posX, posY);
    end;
  end;
end;

procedure TForm1.ShapeMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
var
  posX, posY: Integer;
begin
  with (Sender as TShape) do
  begin
    if (Button = mbLeft) then
    begin
      Brush.Color := ColorButton1.ButtonColor;
      posX := StrToInt(copy(Name, Pos('X', Name) + 1, Pos('Y', Name) - Pos('X', Name) - 1));
      posY := StrToInt(copy(Name, Pos('Y', name) + 1, Name.length - Pos('Y', Name)));
      paintImage(Brush.Color, posX, posY);
    end
    else
    begin
      Brush.Color := clBlack;
      posX := StrToInt(copy(Name, Pos('X', Name) + 1, Pos('Y', Name) - Pos('X', Name) - 1));
      posY := StrToInt(copy(Name, Pos('Y', name) + 1, Name.length - Pos('Y', Name)));
      paintImage(Brush.Color, posX, posY);
    end;
  end;
end;

end.

