program agent;

uses
  Forms,
  Unit1 in 'Unit1.pas' {MainForm},
  request_frm in 'request_frm.pas' {request_form},
  routine_strings in 'routine_strings.pas',
  frm_deploy in 'frm_deploy.pas' {form_edit_deploy},
  thread_builder in 'thread_builder.pas';

{$R *.res}

begin
  Application.Initialize;
  Application.MainFormOnTaskbar := True;
  Application.CreateForm(TMainForm, MainForm);
  Application.CreateForm(Trequest_form, request_form);
  Application.CreateForm(Tform_edit_deploy, form_edit_deploy);
  request_form.Visible := false; //TD:какой-то баг с дискрипторами возникает если сделать Visible = false изначально.

  Application.Run;
end.
