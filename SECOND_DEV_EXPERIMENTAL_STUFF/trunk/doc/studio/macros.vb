Option Strict Off
Option Explicit Off
Imports System
Imports EnvDTE
Imports EnvDTE80
Imports EnvDTE90
Imports EnvDTE90a
Imports EnvDTE100
Imports System.Diagnostics

Public Module Module1

    Sub ConvertHex()
        line = DTE.ActiveDocument.Selection.TopPoint.Line
        col = DTE.ActiveDocument.Selection.TopPoint.LineCharOffset
        DTE.ExecuteCommand("Edit.SelectCurrentWord")
        strVal = DTE.ActiveDocument.Selection.Text
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
                DTE.ActiveDocument.Selection.Text = numVal
            Else
                DTE.ActiveDocument.Selection.Text = "0x" + Hex(numVal)
            End If
        End If
        DTE.ActiveDocument.Selection.MoveToLineAndOffset(line, col)
    End Sub

    Sub MultiPaste()
        count = Val(InputBox("Enter count of paste", "MiltiPaste", "2"))
        For i = 1 To count
            DTE.ActiveDocument.Selection.Paste()
        Next
    End Sub

    Sub CopyWord()
        DTE.ExecuteCommand("Edit.SelectCurrentWord")
        DTE.ExecuteCommand("Edit.Copy")
    End Sub

End Module
