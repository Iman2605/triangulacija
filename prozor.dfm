object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 673
  ClientWidth = 1073
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  TextHeight = 13
  object Slika: TImage
    Left = 73
    Top = 56
    Width = 761
    Height = 449
    OnMouseDown = SlikaMouseDown
    OnMouseMove = SlikaMouseMove
  end
  object LabelXmin: TLabel
    Left = 635
    Top = 595
    Width = 29
    Height = 13
    Caption = 'X min:'
  end
  object LabelXmax: TLabel
    Left = 633
    Top = 627
    Width = 33
    Height = 13
    Caption = 'X max:'
  end
  object LabelYmin: TLabel
    Left = 703
    Top = 595
    Width = 29
    Height = 13
    Caption = 'Y min:'
  end
  object LabelYmax: TLabel
    Left = 703
    Top = 627
    Width = 33
    Height = 13
    Caption = 'Y max:'
  end
  object ButtonProstiMnogougao: TButton
    Left = 72
    Top = 536
    Width = 113
    Height = 33
    Caption = 'Prosti mnogougao'
    TabOrder = 0
    OnClick = ButtonProstiMnogougaoClick
  end
  object ButtonOcisti: TButton
    Left = 864
    Top = 536
    Width = 105
    Height = 33
    Caption = 'O'#269'isti'
    TabOrder = 1
    OnClick = ButtonOcistiClick
  end
  object ButtonGenerisiTacke: TButton
    Left = 944
    Top = 56
    Width = 121
    Height = 33
    Caption = 'Generi'#353'i random ta'#269'ke'
    TabOrder = 2
    OnClick = ButtonGenerisiTackeClick
  end
  object EditBrojTacaka: TEdit
    Left = 840
    Top = 56
    Width = 90
    Height = 21
    TabOrder = 3
    Text = '20'
  end
  object RadioDodajTacku: TRadioButton
    Left = 856
    Top = 192
    Width = 113
    Height = 17
    Caption = 'Dodaj ta'#269'ku'
    Checked = True
    TabOrder = 4
    TabStop = True
  end
  object RadioDodajDuz: TRadioButton
    Left = 856
    Top = 232
    Width = 113
    Height = 17
    Caption = 'Dodaj du'#382
    TabOrder = 5
  end
  object RadioDodajTrokut: TRadioButton
    Left = 856
    Top = 272
    Width = 113
    Height = 17
    Caption = 'Dodaj trokut'
    TabOrder = 6
  end
  object ButtonPresjekDuzi: TButton
    Left = 208
    Top = 540
    Width = 129
    Height = 29
    Caption = 'Da li se du'#382'i sijeku'
    TabOrder = 7
    OnClick = ButtonPresjekDuziClick
  end
  object ButtonUvijanjePoklona: TButton
    Left = 360
    Top = 540
    Width = 121
    Height = 29
    Caption = 'Uvijanje poklona'
    TabOrder = 8
    OnClick = ButtonUvijanjePoklonaClick
  end
  object ButtomGrahamScan: TButton
    Left = 504
    Top = 540
    Width = 105
    Height = 29
    Caption = 'Graham Scan'
    TabOrder = 9
    OnClick = ButtomGrahamScanClick
  end
  object RadioTackaUnutarKonveksnog: TRadioButton
    Left = 856
    Top = 312
    Width = 154
    Height = 17
    Caption = 'Tacka unutar konveksnog'
    TabOrder = 10
  end
  object RadioTangente: TRadioButton
    Left = 856
    Top = 352
    Width = 113
    Height = 17
    Caption = 'Povuci tangente'
    TabOrder = 11
  end
  object ButtonKonveksniInduktivno: TButton
    Left = 632
    Top = 540
    Width = 121
    Height = 29
    Caption = 'Konveksni induktivno'
    TabOrder = 12
    OnClick = ButtonKonveksniInduktivnoClick
  end
  object RadioCrtajPoligon: TRadioButton
    Left = 856
    Top = 392
    Width = 113
    Height = 17
    Caption = 'Crtaj poligon'
    TabOrder = 13
  end
  object ButtonTriangulacija: TButton
    Left = 72
    Top = 592
    Width = 113
    Height = 33
    Caption = 'Triangulacija'
    TabOrder = 14
    OnClick = ButtonTriangulacijaClick
  end
  object TextKoordinate: TEdit
    Left = 864
    Top = 480
    Width = 105
    Height = 21
    Enabled = False
    TabOrder = 15
    Text = 'TextKoordinate'
  end
  object ButtonGenerisiHorVerDuzi: TButton
    Left = 944
    Top = 104
    Width = 121
    Height = 33
    Caption = 'Generi'#353'i hor. i ver. du'#382'i'
    TabOrder = 16
    OnClick = ButtonGenerisiHorVerDuziClick
  end
  object ButtonPresjekHorVerDuzi: TButton
    Left = 208
    Top = 592
    Width = 129
    Height = 33
    Caption = 'Presjek hor. i ver. du'#382'i'
    TabOrder = 17
    OnClick = ButtonPresjekHorVerDuziClick
  end
  object ButtonNadjiPresjekeDuzi: TButton
    Left = 360
    Top = 592
    Width = 121
    Height = 33
    Caption = 'Presjek du'#382'i'
    TabOrder = 18
    OnClick = ButtonNadjiPresjekeDuziClick
  end
  object ButtonGenerisiDuzi: TButton
    Left = 944
    Top = 152
    Width = 121
    Height = 34
    Caption = 'Generi'#353'i du'#382'i'
    TabOrder = 19
    OnClick = ButtonGenerisiDuziClick
  end
  object p_xmin: TEdit
    Left = 672
    Top = 592
    Width = 25
    Height = 21
    TabOrder = 20
    Text = '100'
  end
  object p_ymin: TEdit
    Left = 738
    Top = 592
    Width = 25
    Height = 21
    TabOrder = 21
    Text = '100'
  end
  object p_xmax: TEdit
    Left = 672
    Top = 624
    Width = 25
    Height = 21
    TabOrder = 22
    Text = '500'
  end
  object p_ymax: TEdit
    Left = 738
    Top = 624
    Width = 25
    Height = 21
    TabOrder = 23
    Text = '300'
  end
  object ButtonKDStablo: TButton
    Left = 504
    Top = 595
    Width = 105
    Height = 30
    Caption = 'KDStablo'
    TabOrder = 24
    OnClick = ButtonKDStabloClick
  end
  object DugmeTriangulacijaInduktivno: TButton
    Left = 72
    Top = 640
    Width = 123
    Height = 33
    Caption = 'Triangulacija induktivno'
    TabOrder = 25
    OnClick = DugmeTriangulacijaInduktivnoClick
  end
  object DugmeTriangulacijaSaKoracima: TButton
    Left = 224
    Top = 640
    Width = 185
    Height = 33
    Caption = 'Triangulcija induktivno sa koracima'
    TabOrder = 26
    OnClick = DugmeTriangulacijaSaKoracimaClick
  end
end
