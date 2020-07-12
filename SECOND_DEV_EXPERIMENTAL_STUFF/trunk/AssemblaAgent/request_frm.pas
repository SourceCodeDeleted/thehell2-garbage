unit request_frm;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, Grids, AdvObj, BaseGrid, AdvGrid, AdvCGrid, ExtCtrls;

type
  Trequest_form = class(TForm)
    Label1: TLabel;
    Bevel1: TBevel;
    Panel1: TPanel;
    Bevel2: TBevel;
    Button1: TButton;
    Label2: TLabel;
    Bevel3: TBevel;
    grid_reqs: TAdvColumnGrid;
    request_memo: TMemo;
    procedure Label5Click(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure grid_reqsRowUpdate(Sender: TObject; OldRow, NewRow: Integer);
    procedure FormShow(Sender: TObject);
    procedure grid_reqsSelectCell(Sender: TObject; ACol, ARow: Integer;
      var CanSelect: Boolean);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  request_form: Trequest_form;

implementation

{$R *.dfm}

procedure Trequest_form.Button1Click(Sender: TObject);
begin
  close;
end;

procedure Trequest_form.FormShow(Sender: TObject);
begin
  grid_reqs.Cells[0,0] := '#';
  grid_reqs.Cells[1,0] := 'Время';
  grid_reqs.Cells[2,0] := 'Команда';
  grid_reqs.Cells[3,0] := 'Удал. хост';
  grid_reqs.Cells[4,0] := 'Параметры';
end;

procedure Trequest_form.grid_reqsRowUpdate(Sender: TObject; OldRow,
  NewRow: Integer);
begin
  if grid_reqs.RowCount > 1 then
    grid_reqs.FixedRows := 1;

end;

procedure Trequest_form.grid_reqsSelectCell(Sender: TObject; ACol,
  ARow: Integer; var CanSelect: Boolean);
  var
    str:string;
  begin
  str := grid_reqs.Cells[4, arow];
  request_memo.Lines.Text := StringReplace( str, ';', #13, [rfReplaceAll] );

end;

procedure Trequest_form.Label5Click(Sender: TObject);
begin
  grid_reqs.Clear;
end;

end.
