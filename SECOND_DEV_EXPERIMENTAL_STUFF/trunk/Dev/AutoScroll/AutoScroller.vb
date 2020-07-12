Imports System.Windows
Imports System.Windows.Controls
Imports System.Windows.Media
Imports Microsoft.VisualStudio.Text.Editor
Imports System.Math

''' <summary>
''' Provides support for basic auto-scrolling in VS2010.
''' </summary>
Public Class AutoScroller

    Public Const ADORNER_NAME As String = "AutoScrollerOfWin"

    Private WithEvents mView As IWpfTextView
    Private ReadOnly mAdornmentLayer As IAdornmentLayer

	Private Horizontal As Boolean = True
	Private WithEvents mScrollerOverlay As ScrollUICover ' UI
    Private WithEvents mUpdateTimer As New Forms.Timer() With {.Interval = 17} ' Update ticker, 60 fps-ish
    Private mCurrentOffset As Point ' Offset to use for scrolling

#Region " .ctor "

    ''' <summary>
    ''' Creates a new instance of <see cref="AutoScroller" />.
    ''' </summary>
    ''' <param name="view">The <see cref="IWpfTextView"/> upon which the adornment will be drawn</param>
    Public Sub New(ByVal view As IWpfTextView)

        mView = view
        mScrollerOverlay = New ScrollUICover(view.VisualElement)

        ' Grab a reference to the adornment layer that this adornment should be added to
        mAdornmentLayer = mView.GetAdornmentLayer(ADORNER_NAME)
        mAdornmentLayer.AddAdornment(AdornmentPositioningBehavior.ViewportRelative, Nothing, Nothing, mScrollerOverlay, Nothing)
    End Sub

#End Region

#Region " Event Handlers "

    Private Sub mScrollerOverlay_Started(ByVal sender As Object, ByVal e As System.EventArgs) Handles mScrollerOverlay.Started

        ' Reset and start moving (sorta)
        mCurrentOffset = New Point(0, 0)
        mUpdateTimer.Start()
    End Sub

    Private Sub mScrollerOverlay_OffsetChanged(ByVal sender As Object, ByVal e As OffsetEventArgs) Handles mScrollerOverlay.OffsetChanged

        ' Mouse has moved, update our current offset to work with
        mCurrentOffset = New Point(Me.DoSomeMagic(e.Offset.X), Me.DoSomeMagic(e.Offset.Y))

        ' Accept if we have an offset
        e.OffsetAccepted = mCurrentOffset.X <> 0 OrElse mCurrentOffset.Y <> 0
    End Sub

    Private Sub mScrollerOverlay_Finished(ByVal sender As Object, ByVal e As System.EventArgs) Handles mScrollerOverlay.Finished

        ' Stopped auto-scrolling
        mUpdateTimer.Stop()
    End Sub

    Private Sub mUpdateTimer_Tick(ByVal sender As Object, ByVal e As System.EventArgs) Handles mUpdateTimer.Tick

		'ViewportRight   403.89417989417996	Double
		'MaxTextRightCoordinate  401	Double
		' Move the view
		Dim limit, Hor As Double
		If mScrollerOverlay.mJustStartScrolling Then
			Hor = Abs(mCurrentOffset.X) - Abs(mCurrentOffset.Y)
			If Abs(Hor) < 0.01 Then
				Exit Sub
			End If
			Horizontal = Hor >= 0
			mScrollerOverlay.mJustStartScrolling = False
		End If
		If Horizontal Then
			limit = mView.MaxTextRightCoordinate - mView.ViewportRight
			If mCurrentOffset.X > 0 Then
				If limit > 0 Then
					If mCurrentOffset.X < limit Then
						limit = mCurrentOffset.X
					End If
					mView.ViewScroller.ScrollViewportHorizontallyByPixels(limit)
				End If
			Else
				mView.ViewScroller.ScrollViewportHorizontallyByPixels(mCurrentOffset.X)
			End If
		Else
			mView.ViewScroller.ScrollViewportVerticallyByPixels(mCurrentOffset.Y)
		End If

	End Sub

    Private Sub OnSizeChange() Handles mView.ViewportHeightChanged, mView.ViewportWidthChanged

        ' Adjust overlay size to match view
        ' Note: I don't like using an overlay but IWpfTextView doesn't give mouse events?
        mScrollerOverlay.Width = mView.ViewportWidth
        mScrollerOverlay.Height = mView.ViewportHeight
    End Sub

    Private Sub mView_LostAggregateFocus(ByVal sender As Object, ByVal e As System.EventArgs) Handles mView.LostAggregateFocus, mView.Closed

        ' Make sure that scrolling stops, as we have lost focus or closed
        If mScrollerOverlay.IsScrolling Then
            mScrollerOverlay.EndScrolling()
        End If
    End Sub

#End Region

#Region " Helpers "

    ''' <summary>
    ''' Will refactor the raw pixel offset to whatever scroller shall move by every tick.
    ''' </summary>
    ''' <param name="value">Raw pixel offset.</param>
    Private Function DoSomeMagic(ByVal value As Double) As Double

        ' Do not move while inside the scroller graphic's area
        If Math.Abs(value) < mScrollerOverlay.ScrollerRadius Then Return 0

        ' Make it a bit smaller.. meh
        value /= 3

        ' Round so we don't move to sub pixel locations
        Return Math.Round(value, 0)
    End Function

#End Region

End Class