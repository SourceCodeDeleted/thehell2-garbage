Option Strict Off
Option Explicit Off
Imports System
Imports EnvDTE
Imports EnvDTE80
Imports EnvDTE90
Imports EnvDTE100

Module Macros

    Public VS As DTE2

    Sub ConvertHex()
        line = VS.ActiveDocument.Selection.TopPoint.Line()
        col = VS.ActiveDocument.Selection.TopPoint.LineCharOffset
        VS.ExecuteCommand("Edit.SelectCurrentWord")
        strVal = VS.ActiveDocument.Selection.Text
        If Left(strVal, 2) = "0x" Then
            strVal = "&H" + Right(strVal, Len(strVal) - 2)
        Else
            If Right(strVal, 1) = "h" Then
                strVal = "&H" + Left(strVal, Len(strVal) - 1)
            End If
        End If
        numVal = Val(strVal)
        If numVal <> 0 Then
            If Left(strVal, 2) = "&H" Then
                If Len(strVal) = 6 And Val(Left(strVal, 3)) > 7 Then
                    numVal = Val("&H1" + Right(strVal, 4)) - 65536
                End If
                VS.ActiveDocument.Selection.Text = numVal
            Else
                VS.ActiveDocument.Selection.Text = "0x" + Hex(numVal)
            End If
        End If
        VS.ActiveDocument.Selection.MoveToLineAndOffset(line, col)
    End Sub

    Sub MultiPaste()
        count = Val(InputBox("Enter count of paste", "MiltiPaste", "2"))
        For i = 1 To count
            VS.ActiveDocument.Selection.Paste()
        Next
    End Sub

    Sub CopyWord()
        VS.ExecuteCommand("Edit.SelectCurrentWord")
        VS.ExecuteCommand("Edit.Copy")
    End Sub

    Sub ChangeTabSize()
        props = VS.Properties("TextEditor", "C/C++")
        ts = props.Item("TabSize")
        ins = props.Item("IndentSize")
        If ts.value <= 4 Then
            ts.value = 20
            ins.Value = 20
        ElseIf ts.value <= 20 Then
            ts.value = 40
            ins.Value = 40
        ElseIf ts.value <= 40 Then
            ts.value = 60
            ins.Value = 60
        ElseIf ts.value <= 60 Then
            ts.value = 4
            ins.Value = 4
        End If
    End Sub

    Declare Sub Sleep Lib "kernel32.dll" (ByVal Milliseconds As Integer)
    Sub GotoTypeDefinition()
        VS.ExecuteCommand("VAssistX.GotoImplementation")
        'Sleep(100)
        VS.ExecuteCommand("Edit.WordPrevious")
        VS.ExecuteCommand("Edit.WordPrevious")
        'Sleep(400)
        VS.ExecuteCommand("VAssistX.GotoImplementation")
        'Dim selection As TextSelection
        'selection = VS.ActiveDocument.Selection()
        'Dim var As CodeVariable
        'var = selection.TopPoint.CodeElement(vsCMElement.vsCMElementVariable)
        'If Not IsNothing(var) Then
        '    Dim type As CodeTypeRef
        '    type = var.Type
        '
        'End If
        '
        '
    End Sub
End Module
