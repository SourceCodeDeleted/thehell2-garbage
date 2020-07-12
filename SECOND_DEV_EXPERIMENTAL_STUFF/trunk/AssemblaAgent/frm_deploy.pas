unit frm_deploy;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, SynEditHighlighter, SynHighlighterBat, SynEdit;

type
  Tform_edit_deploy = class(TForm)
    SynEdit: TSynEdit;
    SynBatSyn1: TSynBatSyn;
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  form_edit_deploy: Tform_edit_deploy;

implementation

{$R *.dfm}

end.
