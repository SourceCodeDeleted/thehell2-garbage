Imports System.ComponentModel.Composition
Imports Microsoft.VisualStudio.Text.Editor
Imports Microsoft.VisualStudio.Utilities
Imports System.Windows

''' <summary>
''' Establishes an <see cref="IAdornmentLayer"/> to place the adornment on and exports the <see cref="IWpfTextViewCreationListener"/>
''' that instantiates the adornment on the event of a <see cref="IWpfTextView"/>'s creation
''' </summary>
<Export(GetType(IWpfTextViewCreationListener))>
<ContentType("text")>
<TextViewRole(PredefinedTextViewRoles.Document)>
NotInheritable Class EditorAdornmentFactory
    Implements IWpfTextViewCreationListener

    ''' <summary>
    ''' Defines the adornment layer for the public corner box adornment. This layer is ordered 
    ''' after the selection layer in the Z-order
    ''' </summary>
    <Export(GetType(AdornmentLayerDefinition))>
	<Name(AutoScroller.ADORNER_NAME)>
	<Order(After:=PredefinedAdornmentLayers.Caret)>
	Public mEditorAdornmentLayer As AdornmentLayerDefinition

    ''' <summary>
    ''' Creates a AutoScroller manager when a textView is created
    ''' </summary>
    ''' <param name="textView">The <see cref="IWpfTextView"/> upon which the adornment should be placed</param>
    Public Sub TextViewCreated(ByVal textView As IWpfTextView) Implements IWpfTextViewCreationListener.TextViewCreated

        Try
            Dim instance = New AutoScroller(textView)
        Catch
            ' Calm down, dear. No need to cry
        End Try
    End Sub
End Class
