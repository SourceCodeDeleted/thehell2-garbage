unit routine_strings;

interface

  uses SysUtils, Clipbrd, ShellApi, Dialogs, Classes;

  function StringAssigned( str:string ):boolean;
  function RussianToUnicode(S: String): String;
//procedure TextInRussian(AText: WideString);
  function SubString( raw_string, delimeter:string; index:integer ):string;
  function SubStringCount( raw_string, sub_string:string ):integer;
  function Replace(source_string:string;search_string,replace_string:string):string;
  function FindSubString( source_string, substring:string ):boolean;
  function IsIdentChar(AChar: WideChar): Boolean;
  procedure CollectWord(Str: PWideChar; var words: TStringList);

  function PosAfterPos( the_string, search_string:string; position:integer ):integer;
  function CopyFromTo( the_string:string; pos1, pos2:integer ):string;
  function ConvertString(strOld: string): string;
  function ExtractAfter( the_string:string; position:integer ):string;
  function YesNoToBool( the_string:string ):boolean;
  function MinusOneIfNull( the_string:string ):string;


implementation

{
procedure InputString( str:string  );
 var
   Inp: TInput;
   I: Integer;
 begin
   Edit1.SetFocus;

   for I := 1 to Length(Str) do
   begin
     // press 
    Inp.Itype := INPUT_KEYBOARD;
     Inp.ki.wVk := Ord(UpCase(Str[i]));
     Inp.ki.dwFlags := 0;
     SendInput(1, Inp, SizeOf(Inp));

     // release 
    Inp.Itype := INPUT_KEYBOARD;
     Inp.ki.wVk := Ord(UpCase(Str[i]));
     Inp.ki.dwFlags := KEYEVENTF_KEYUP;
     SendInput(1, Inp, SizeOf(Inp));

     Application.ProcessMessages;
   end;
end;
}

{
Procedure TextInRussian(AText: WideString);
var
  Clp: TClipboard;
  LKL: array [0..1023] of char;
begin
  GetKeyboardLayoutName( LKL );                   // запоминаем кака€ была раскладка
  LoadKeyboardLayout( '00000419',KLF_ACTIVATE );  // переключаем на русский
  Clp := TClipboard.Create;
  Clp.AsText := AText;
  Clp.Free;
  LoadKeyboardLayout(LKL,KLF_ACTIVATE);         // возвращаемс€ к запомненному
end;
}



function IsIdentChar(AChar: WideChar): Boolean;
begin
  case AChar of
    '_', '0'..'9', 'a'..'z', 'A'..'Z', 'а'..'€', 'ј'..'я':
      Result := True;
    else
      Result := False;
  end;
end;
     {
procedure CollectWord(Str: PWideChar; var words: TStringList);
var
  i:integer;
  word : string;
begin

  word := '';
  for i := 0 to length( str ) -1 do
    begin
      if IsIdentChar( str[i] ) then
        word := word + str[i]
      else
      begin
        if Length(word)>0 then
          words.Add( word );

        word := '';
      end;
    end;
end;
}

procedure CollectWord(Str: PWideChar; var words: TStringList);
var
  i:integer;
begin
  words.Add('');
  for i := 0 to length( str ) -1 do
    begin
      if IsIdentChar( str[i] ) then
        words[ words.Count-1 ] := words[ words.Count-1 ] + str[i]
      else
      begin
        if length( words[words.Count-1] ) > 0 then
          words.Add( '' );
      end;
    end;
end;

function MinusOneIfNull( the_string:string ):string;
begin
  if not StringAssigned( the_string ) then result := '-1'
  else
    result := the_string;
end;

function YesNoToBool( the_string:string ):boolean;
begin
  if ( the_string = '≈' ) then
    result := true;

  if ( the_string = 'ƒ' ) or ( the_string = 'ƒа' ) then
    result := true;

  if ( the_string = 'Ќ' ) or ( the_string = 'Ќет' ) then
    result := false;

end;

function CopyFromTo( the_string:string; pos1, pos2:integer ):string;
begin
  result := Copy( the_string, pos1, pos2-pos1 );
end;

function ExtractAfter( the_string:string; position:integer ):string;
begin
  result := Copy( the_string, position, length( the_string ) - position );
end;

function PosAfterPos( the_string, search_string:string; position:integer ):integer;
var
  rest_string:string;
begin
  rest_string := Copy( the_string, position, length( the_string ) - position );
  result := position + Pos( search_string, rest_string );
end;

function Replace(source_string:string;search_string,replace_string:string):string;
var
  A:integer;
  S:string;
begin
  While Pos( search_string,LowerCase( source_string ))>0 Do
  begin
    A := Pos(search_string,Lowercase( source_string ));
    source_string:=Copy( source_string ,1,A-1) + replace_string + Copy(source_string,A+Length(search_string),Length(source_string) - (A+Length(search_string)) + 1);
  end;
  result := source_string;
end;


function FindSubString( source_string, substring:string ):boolean;
begin
  Result := Pos( AnsiLowerCase( substring ), AnsiLowerCase( ' ' + source_string ) ) > 0;
end;

function SubStringCount( raw_string, sub_string:string ):integer;
var
  position : integer;
  current_index:integer;
  middle_string:string;
  count:integer;
begin
  count := 0;
  current_index := 0;
  position := 1;

  while position <> 0 do
  begin
    middle_string := '';
    current_index := current_index + 1;
    position      := Pos( sub_string, raw_string );

    if position > 0 then
    begin
      middle_string := Copy( raw_string, 1, position - 1 );
      raw_string    := Copy( raw_string, position + 1, length( raw_string ) - position);
      count := count + 1;
    end;

  end;

  if count > 0 then
    count := count + 1;

  result := count;
end;


function SubString( raw_string, delimeter:string; index:integer ):string;
var
  position : integer;
  current_index:integer;
  middle_string:string;
begin
  current_index := 0;

  while current_index <> index do
  begin
    middle_string := '';
    current_index := current_index + 1;
    position      := Pos( delimeter, raw_string );

    if position > 0 then
    begin
      middle_string := Copy( raw_string, 1, position - 1 );
      raw_string    := Copy( raw_string, position + length(delimeter), length( raw_string ) - position );
    end;

    if position = 0 then
    begin
      if current_index = index then
        middle_string := raw_string
      else
      begin
        middle_string := '';
        break;
      end;
    end;

  end;

  result := middle_string;
end;

function RussianToUnicode(S: String): String;
var Wrd:Word;
  pW,pR:PWord;
  len:Integer;
begin
  pW:=@S[1];
  len:=Length(S);
  SetLength(Result,len);
  pR:=@Result[1];
  while Len<>0 do begin
    Wrd:=pW^;
    case Wrd of
      $C0..$DF,$E0..$FF:pR^:=Wrd+$0350;
      else pR^:=Wrd;
    end;
    inc(pW);
    inc(pR);
    dec(Len);
  end;
end;

  function StringAssigned( str:string ):boolean;
  begin
    if length( str ) > 0 then result := true
    else result := false;
  end;

function ConvertString(strOld: string): string;
 const
   strUni : string = '#1072#1073#1074#1075#1076#1077#1108#1078#1079#1080#1110#1111#1081#1082#1083'+
     '#1084#1085#1086#1087#1088#1089#1090#1091#1092#1093#1094#1095#1096#1097#1102#1103#1100'+
     '#1040#1041#1042#1043#1044#1045#1028#1046#1047#1048#1030#1031#1049#1050#1051#1052#1053#1054#1055#1056#1057#1058#1059#1060#1061#1062#1063#1064#1065#1070#1071#1068#8470#1099#1067#1105#1025#1101#1069#1098#1066';
     

   strWin : string = 'абвгде¶жзиђ©йклмнопрстуфхцчшщю€ьјЅ¬√ƒ≈L∆«»¶¶… ЋћЌќѕ–—“”‘’÷„Ўўёя№¶ыџЄ®эЁъЏ';
 var
   letCount, i: integer;
   strNew: string;            
 begin
   if pos('#',strOld) = 0 then
     Result := strOld
   else
   begin
     strNew := strOld;
     letCount := length(strWin);
     for i:=1 to letCount do
       strNew := stringReplace(strNew, copy(strUni, (i-1)*5+1, 5), ''''+copy(strWin, i, 1)+'''', [rfReplaceAll]);
     strNew := stringReplace(strNew, '''''', '', [rfReplaceAll]);
     Result := strNew;
   end;
 end;

end.
