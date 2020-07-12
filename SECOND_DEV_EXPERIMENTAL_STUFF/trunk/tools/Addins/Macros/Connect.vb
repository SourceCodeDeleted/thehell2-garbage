Imports System
Imports Microsoft.VisualStudio.CommandBars
Imports Extensibility
Imports EnvDTE
Imports EnvDTE80

Public Class Connect

    Implements IDTExtensibility2
    Implements IDTCommandTarget

    Private _applicationObject As DTE2
    Private _addInInstance As AddIn

    '''<summary>Implements the constructor for the Add-in object. Place your initialization code within this method.</summary>
    Public Sub New()

    End Sub

    '''<summary>Implements the OnConnection method of the IDTExtensibility2 interface. Receives notification that the Add-in is being loaded.</summary>
    '''<param name='application'>Root object of the host application.</param>
    '''<param name='connectMode'>Describes how the Add-in is being loaded.</param>
    '''<param name='addInInst'>Object representing this Add-in.</param>
    '''<remarks></remarks>
    Public Sub OnConnection(ByVal application As Object, ByVal connectMode As ext_ConnectMode, ByVal addInInst As Object, ByRef custom As Array) Implements IDTExtensibility2.OnConnection
        _applicationObject = CType(application, DTE2)
        _addInInstance = CType(addInInst, AddIn)
        Macros.VS = _applicationObject
        Dim props = Macros.VS.Properties("TextEditor", "C/C++ Specific")
        'Dim out As String = ""
        'For i = 1 To props.Count
        '    out = out & props.Item(i).Name & vbCr
        'Next
        'MsgBox(out)
        Dim rh = props.Item("DisableReferenceHighlighting")
        rh.Value = True
        Reg("ConvertHex")
        Reg("CopyWord")
        Reg("MultiPaste")
        Reg("ChangeTabSize")
        Reg("GotoTypeDefinition")
    End Sub

    Private Sub Reg(command As String)
        Dim myCommand As Command = Nothing
        Try
            myCommand = _applicationObject.Commands.Item(_addInInstance.ProgID & "." & command)
        Catch
        End Try
        If myCommand Is Nothing Then
            _applicationObject.Commands.AddNamedCommand(_addInInstance, command, command, command, False)
        End If

    End Sub

    Public Sub Exec(ByVal cmdName As String, ByVal executeOption As vsCommandExecOption, _
      ByRef varIn As Object, ByRef varOut As Object, ByRef handled As Boolean) _
      Implements IDTCommandTarget.Exec

        handled = False

        If (executeOption = vsCommandExecOption.vsCommandExecOptionDoDefault) Then

            Select Case cmdName
                Case "Macros.Connect.ConvertHex"
                    handled = True
                    Macros.ConvertHex()
                Case "Macros.Connect.MultiPaste"
                    handled = True
                    Macros.MultiPaste()
                Case "Macros.Connect.CopyWord"
                    handled = True
                    Macros.CopyWord()
                Case "Macros.Connect.ChangeTabSize"
                    handled = True
                    Macros.ChangeTabSize()
                Case "Macros.Connect.GotoTypeDefinition"
                    handled = True
                    Macros.GotoTypeDefinition()
            End Select

        End If

    End Sub

    Public Sub QueryStatus(ByVal cmdName As String, ByVal neededText As vsCommandStatusTextWanted, _
       ByRef statusOption As vsCommandStatus, ByRef commandText As Object) _
       Implements IDTCommandTarget.QueryStatus

        If neededText = vsCommandStatusTextWanted.vsCommandStatusTextWantedNone Then

            If True Then
                statusOption = CType(vsCommandStatus.vsCommandStatusEnabled + _
                   vsCommandStatus.vsCommandStatusSupported, vsCommandStatus)
            Else
                statusOption = vsCommandStatus.vsCommandStatusUnsupported
            End If

        End If

    End Sub



    '''<summary>Implements the OnDisconnection method of the IDTExtensibility2 interface. Receives notification that the Add-in is being unloaded.</summary>
    '''<param name='disconnectMode'>Describes how the Add-in is being unloaded.</param>
    '''<param name='custom'>Array of parameters that are host application specific.</param>
    '''<remarks></remarks>
    Public Sub OnDisconnection(ByVal disconnectMode As ext_DisconnectMode, ByRef custom As Array) Implements IDTExtensibility2.OnDisconnection
    End Sub

    '''<summary>Implements the OnAddInsUpdate method of the IDTExtensibility2 interface. Receives notification that the collection of Add-ins has changed.</summary>
    '''<param name='custom'>Array of parameters that are host application specific.</param>
    '''<remarks></remarks>
    Public Sub OnAddInsUpdate(ByRef custom As Array) Implements IDTExtensibility2.OnAddInsUpdate
    End Sub

    '''<summary>Implements the OnStartupComplete method of the IDTExtensibility2 interface. Receives notification that the host application has completed loading.</summary>
    '''<param name='custom'>Array of parameters that are host application specific.</param>
    '''<remarks></remarks>
    Public Sub OnStartupComplete(ByRef custom As Array) Implements IDTExtensibility2.OnStartupComplete
    End Sub

    '''<summary>Implements the OnBeginShutdown method of the IDTExtensibility2 interface. Receives notification that the host application is being unloaded.</summary>
    '''<param name='custom'>Array of parameters that are host application specific.</param>
    '''<remarks></remarks>
    Public Sub OnBeginShutdown(ByRef custom As Array) Implements IDTExtensibility2.OnBeginShutdown
    End Sub

End Class
