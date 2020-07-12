
// функция вызываемая компилятором неявно при преобразовании типа double к int
// можно удалить этот файл целиком из проекта 
// находится тут для примера сигнатуры хитрых usercall, код из ida : 
// signed __int64 __usercall _ftol<edx:eax>(double a1<st0>) { return (signed __int64)a1; }
#pragma optimize("gsy",on)
//----- (0047A570) --------------------------------------------------------
__int64 __ftol(double x) // __ftol (VC 6) __ftol2 (VS 2xxx)
{
	return ftol(x);
}
#pragma optimize("",on)
/*
.text:0047A570     __ftol          proc near               ; ...
.text:0047A570
.text:0047A570     var_C           = qword ptr -0Ch
.text:0047A570     var_4           = word ptr -4
.text:0047A570     var_2           = word ptr -2
.text:0047A570
.text:0047A570 000                 push    ebp
.text:0047A571 004                 mov     ebp, esp
.text:0047A573 004                 add     esp, 0FFFFFFF4h ; Add
.text:0047A576 010                 fstcw   [ebp+var_2]     ; Store Control Word
.text:0047A57A 010                 wait                    ; Wait until BUSY# Pin is Inactive (HIGH)
.text:0047A57B 010                 mov     ax, [ebp+var_2]
.text:0047A57F 010                 or      ah, 0Ch         ; Logical Inclusive OR
.text:0047A582 010                 mov     [ebp+var_4], ax
.text:0047A586 010                 fldcw   [ebp+var_4]     ; Load Control Word
.text:0047A589 010                 fistp   [ebp+var_C]     ; Store Integer and Pop
.text:0047A58C 010                 fldcw   [ebp+var_2]     ; Load Control Word
.text:0047A58F 010                 mov     eax, dword ptr [ebp+var_C]
.text:0047A592 010                 mov     edx, dword ptr [ebp+var_C+4]
.text:0047A595 010                 leave                   ; High Level Procedure Exit
.text:0047A596 000                 retn                    ; Return Near from Procedure
.text:0047A596     __ftol          endp
.text:0047A596
*/
