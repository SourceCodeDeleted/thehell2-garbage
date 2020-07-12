Imports System.Windows.Controls
Imports System.Windows.Input
Imports System.Windows
Imports System.Windows.Media.Animation

''' <summary>
''' Provides the UI for the scroller.
''' </summary>
''' <remarks></remarks>
Friend Class ScrollUICover

    Private WithEvents mBackingUi As FrameworkElement ' UI that we are scrolling through (or something close)

    Private mScrolling As Boolean = False ' Actively scrolling around (UI is visible)
	Public mJustStartScrolling As Boolean = False '
    Private mHasScrolled As Boolean = False ' Where or not this instance of scrolling has moved the view
    Private mFreeScrolling As Boolean = False ' Scrolling while button is not held down
    Private mLastScrollTime As DateTime ' Recording of the last time the scroller was scrolled (not auto-scrolled)

    Private mStartLocation As Point ' Initial click location
    Private mCurrentOffset As Point ' Position of mouse relative to start location

    ' Configurable things
    ' TODO: Make UI to edit this
    Private mScrollerRadius As Double = 1 '15 ' Size if scroller UI & dead zone
    Private mIsHeavyHanded As Boolean = False 'True ' Whether or not to wait a certain time after scrolling to auto-scroll
	Private mPrettyFadeInEffect As Boolean = False 'True ' Slight fade in effect for auto-scrolling symbol
    Private mIgnoreControlClicks As Boolean = True ' True to ignore all ctrl + middle clicks

#Region " Events "

    ''' <summary>
    ''' Raised when scrolling UI is first displayed, not necessarily scrolling.
    ''' </summary>
    ''' <remarks></remarks>
    Friend Event Started As EventHandler

    ''' <summary>
    ''' Raised when the offset to scroll by has changed.
    ''' </summary>
    ''' <remarks></remarks>
    Friend Event OffsetChanged As EventHandler(Of OffsetEventArgs)

    ''' <summary>
    ''' Raised when scrolling has completed and UI has been hidden.
    ''' </summary>
    ''' <remarks></remarks>
    Friend Event Finished As EventHandler

#End Region

#Region " .ctor "

    Public Sub New(ByVal backingUi As FrameworkElement)

        ' This call is required by the designer.
        InitializeComponent()

        ' Update scroller control (lazy)
        Me.ScrollerRadius = mScrollerRadius

        ' Store backing UI
        mBackingUi = backingUi

        ' Add handlers for wheel scrolling that will ignore handled flag
        mBackingUi.[AddHandler](UIElement.MouseWheelEvent, New MouseWheelEventHandler(AddressOf mBackingUi_MouseWheel), True)
        BackgroundGrid.[AddHandler](UIElement.MouseWheelEvent, New MouseWheelEventHandler(AddressOf mBackingUi_MouseWheel), True)
    End Sub

#End Region

#Region " Properties "

    ''' <summary>
    ''' Size of the scroller control.
    ''' Used as a dead-zone for no movement as well (kind of lazy).
    ''' </summary>
    Friend Property ScrollerRadius As Double
        Get
            Return mScrollerRadius
        End Get
        Set(ByVal value As Double)
            mScrollerRadius = value
            Me.ScrollerThing.Width = mScrollerRadius * 2
            Me.ScrollerThing.Height = mScrollerRadius * 2
        End Set
    End Property

    ''' <summary>
    ''' Whether or not scrolling mode is active.
    ''' </summary>
    Friend ReadOnly Property IsScrolling As Boolean
        Get
            Return mScrolling
        End Get
    End Property

#End Region

#Region " Event Handlers "

    Private Sub BackgroundGrid_MouseDown(ByVal sender As Object, ByVal e As System.Windows.Input.MouseButtonEventArgs) Handles mBackingUi.MouseDown, BackgroundGrid.MouseDown

        ' NOTE: mBackingUi will not raise left & right clicks (swallowed when changing caret position etc ¬_¬) but we don't need them anyway
        '       Middle Mouse works though! That, we need.
        If mScrolling Then

            ' Finished, no matter what button was pressed
            Me.EndScrolling()

            ' Prevent more events being given from this mouse down
            ' as we have 2 listeners, we only want 1 handling
            e.Handled = True

		ElseIf e.ChangedButton = MouseButton.Middle AndAlso
			   (Not mIgnoreControlClicks OrElse (Keyboard.Modifiers And ModifierKeys.Control) = ModifierKeys.None) Then

			Dim timeSinceLastScroll As TimeSpan = DateTime.Now - mLastScrollTime

			If Not mIsHeavyHanded OrElse timeSinceLastScroll > TimeSpan.FromMilliseconds(200) Then

				mStartLocation = e.GetPosition(Me.BackgroundGrid)
				mScrolling = True
				mJustStartScrolling = True

                ' Update scroller orb thing's location
                Me.ScrollerThing.SetValue(Canvas.LeftProperty, mStartLocation.X - mScrollerRadius)
				Me.ScrollerThing.SetValue(Canvas.TopProperty, mStartLocation.Y - mScrollerRadius)

				If mPrettyFadeInEffect Then

                    ' Fade in effect so heavy handed people dont flicker so much.. and its pretty
                    Dim doubleAnim As New DoubleAnimation(fromvalue:=0.0,
														  toValue:=1.0,
														  Duration:=New Duration(TimeSpan.FromMilliseconds(150.0)),
														  FillBehavior:=FillBehavior.Stop)
					Me.ScrollerThing.BeginAnimation(OpacityProperty, doubleAnim)
				End If

                ' Show UI
                'Me.ScrollerThing.Visibility = Windows.Visibility.Visible

                ' All set up - let listeners know
                RaiseEvent Started(Me, EventArgs.Empty)

                ' Follow mouse all the time
                BackgroundGrid.CaptureMouse()
			End If
		End If
	End Sub

    Private Sub BackgroundGrid_MouseMove(ByVal sender As Object, ByVal e As System.Windows.Input.MouseEventArgs) Handles BackgroundGrid.MouseMove

        If mScrolling Then

            mCurrentOffset = mStartLocation - e.GetPosition(Me.BackgroundGrid)

            ' Invert horizontal
            mCurrentOffset.X *= -1

            Dim args As New OffsetEventArgs(mCurrentOffset)

            ' Let listeners know of change
            RaiseEvent OffsetChanged(Me, args)

            ' Determine if we are free scrolling or not
            If Not mHasScrolled AndAlso args.OffsetAccepted Then
                mHasScrolled = True
            End If
        End If
    End Sub

    Private Sub BackgroundGrid_MouseUp(ByVal sender As Object, ByVal e As System.Windows.Input.MouseButtonEventArgs) Handles BackgroundGrid.MouseUp

		'If mHasScrolled AndAlso e.ChangedButton = MouseButton.Middle Then
		If e.ChangedButton = MouseButton.Middle Then

			' Finished
			Me.EndScrolling()

			'ElseIf Not mHasScrolled AndAlso mScrolling Then

			' Have not scrolled, allow free scrolling
			'mFreeScrolling = True
		End If
    End Sub

    Private Sub mBackingUi_MouseWheel(ByVal sender As Object, ByVal e As System.Windows.Input.MouseWheelEventArgs)

        If mIsHeavyHanded Then

            If Not mScrolling Then

                ' Record scrolling
                mLastScrollTime = DateTime.Now

            ElseIf Not mHasScrolled Then

                ' user has click mouse button down then scrolled it, they are heavy handed
                Me.EndScrolling()
            End If
        End If
    End Sub

#End Region

#Region " Helper Methods "

    Friend Sub EndScrolling()

        ' Reset state vars
        mScrolling = False
        mHasScrolled = False
        mFreeScrolling = False

        ' Hide UI
        Me.ScrollerThing.Opacity = 1
        Me.ScrollerThing.Visibility = Windows.Visibility.Hidden

        ' Finished - let listeners know
        RaiseEvent Finished(Me, EventArgs.Empty)

        ' Clean event capture
        BackgroundGrid.ReleaseMouseCapture()
    End Sub

#End Region

End Class

#Region " Support Classes "

Friend Class OffsetEventArgs
    Inherits EventArgs

    Private mOffset As Point

    Friend Sub New(ByVal offset As Point)
        mOffset = offset
    End Sub

    ''' <summary>
    ''' Whether or not listener(s) count the offset as a movement.
    ''' </summary>
    ''' <value>True if accepted.</value>
    ''' <remarks>
    ''' Upon the first accepted offset, the scroller will end scrolling on the next mouse up. 
    ''' Otherwise it will wait for the next mouse down.
    ''' </remarks>
    Public Property OffsetAccepted As Boolean

    ''' <summary>
    ''' Relative distance offset from start location in pixels.
    ''' </summary>
    Public ReadOnly Property Offset As Point
        Get
            Return mOffset
        End Get
    End Property

End Class

#End Region