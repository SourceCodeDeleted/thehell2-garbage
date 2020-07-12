//------------------------------------------------------------------------------
// <copyright file="Macros.cs" company="Company">
//     Copyright (c) Company.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

using System;
using System.ComponentModel.Design;
using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;
using System.Globalization;
using System.Runtime.InteropServices;
using Microsoft.VisualStudio;
using Microsoft.VisualStudio.OLE.Interop;
using Microsoft.VisualStudio.Shell;
using Microsoft.VisualStudio.Shell.Interop;
using Microsoft.Win32;
using System.ComponentModel.Composition;
using Microsoft.VisualStudio.Text.Editor;
using Microsoft.VisualStudio.Utilities;
using Microsoft.VisualStudio.Text.Outlining;
using EnvDTE;
using EnvDTE80;
using EnvDTE90;
using EnvDTE100;
//using SharpSvn; 

namespace VsTH
{

	[Export(typeof(IWpfTextViewCreationListener))]
    [ContentType("C/C++")]
    [TextViewRole(PredefinedTextViewRoles.Document)]
    class ViewCreationListener : IWpfTextViewCreationListener
	{
		//[DllImport("kernel32.dll", SetLastError = true, ExactSpelling = true)]
		//static extern IntPtr VirtualAlloc( IntPtr lpAddress, IntPtr dwSize, uint flAllocationType, uint flProtect );
		[DllImport("kernel32.dll", SetLastError = true, ExactSpelling = true)]
		static extern bool VirtualProtect( IntPtr lpAddress, IntPtr dwSize, uint flNewProtect, out uint lpflOldProtect );
		[DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
		static extern IntPtr GetModuleHandle( string lpModuleName );

		private unsafe void VA_X_2258_UTF8_fix()
		{
			try {
				//VA_X.dll:1F1FB1DA call    near ptr unk_1F22BF10
				//VA_X.dll:1F1FB1DF cmp     eax, 0Ah  // <------------- этот адрес патчить
				//VA_X.dll:1F1FB1E2 jl      short loc_1F1FB1EB
				//VA_X.dll:1F1FB1E4 mov     eax, 0FDE9h
				//VA_X.dll:1F1FB1E9 jmp     short loc_1F1FB1F1
                //
                // 83 F8 0A 7C 07 B8 E9 FD 00 00

                //IntPtr va_x = VirtualAlloc(, (IntPtr)4096, 0x00001000 /*MEM_COMMIT*/, 0x20 /*PAGE_EXECUTE_READ*/);
				IntPtr va_x = GetModuleHandle("VA_X.dll");
				//System.Windows.MessageBox.Show(va_x.ToString("X8"));
				va_x += (0x17B81071 - 0x17630000); // 0x1F21F701 - 0x1ED00000
				uint oldProtect;
				VirtualProtect(va_x, (IntPtr)4096, 0x40, out oldProtect);
				if ( Marshal.ReadInt32( va_x ) == 0x7C0AF883 ) {
					try {
						Marshal.WriteInt32( va_x, 0x7C64F883 );
					} catch (Exception e) { }	//System.Windows.MessageBox.Show(e.Message);
				}   //System.Windows.MessageBox.Show(VA_X_UTFcodeBug.ToString());
			} catch (Exception e) { } //System.Windows.MessageBox.Show(e.Message);
        }

        public void TextViewCreated(IWpfTextView textView)
        {
            if( ! Macros.vaFixed ){ //Macros.vs == null){
				//Macros.vs = (DTE2)System.Runtime.InteropServices.Marshal.GetActiveObject("VisualStudio.DTE");// .14.0");
				//System.Windows.MessageBox.Show("hello");
				Macros.vaFixed = true;
				VA_X_2258_UTF8_fix();
			}
			//System.Windows.MessageBox.Show(textView.TextBuffer.ContentType.ToString());
			//var opt = textView.Options.SupportedOptions;
			textView.Options.SetOptionValue(DefaultTextViewHostOptions.SuggestionMarginId, false);
            //if( ! ((IOutliningManager)textView.Properties[typeof(Microsoft.VisualStudio.Platform.IPlatformFactory).Assembly.GetType("Microsoft.VisualStudio.Text.Outlining.OutliningManager")]).Enabled )
            if (!(bool)Macros.vs.get_Properties("TextEditor", "C/C++ Specific").Item("EnterOutliningModeWhenFilesOpen").Value)
            {
                textView.Options.SetOptionValue(DefaultTextViewHostOptions.OutliningMarginId, false);
            }
        }
    }
    /// <summary>
    /// This is the class that implements the package exposed by this assembly.
    /// </summary>
    /// <remarks>
    /// <para>
    /// The minimum requirement for a class to be considered a valid package for Visual Studio
    /// is to implement the IVsPackage interface and register itself with the shell.
    /// This package uses the helper classes defined inside the Managed Package Framework (MPF)
    /// to do it: it derives from the Package class that provides the implementation of the
    /// IVsPackage interface and uses the registration attributes defined in the framework to
    /// register itself and its components with the shell. These attributes tell the pkgdef creation
    /// utility what data to put into .pkgdef file.
    /// </para>
    /// <para>
    /// To get loaded into VS, the package must be referred by &lt;Asset Type="Microsoft.VisualStudio.VsPackage" ...&gt; in .vsixmanifest file.
    /// </para>
    /// </remarks>
    [PackageRegistration(UseManagedResourcesOnly = true)]
    [InstalledProductRegistration("#110", "#112", "1.0", IconResourceID = 400)] // Info on this package for Help/About
    [Guid(Macros.PackageGuidString)]
	[ProvideAutoLoad(Microsoft.VisualStudio.VSConstants.UICONTEXT.NoSolution_string)]
	[SuppressMessage("StyleCop.CSharp.DocumentationRules", "SA1650:ElementDocumentationMustBeSpelledCorrectly", Justification = "pkgdef, VS and vsixmanifest are valid VS terms")]
    public sealed class Macros : Package
    {
        public static DTE2 vs = null;
		public static bool vaFixed = false;
		/// <summary>
		/// Macros GUID string.
		/// </summary>
		public const string PackageGuidString = "1e6d7141-a65e-4d56-90e2-3e9bd06bd844";

		internal class DteInitializer : IVsShellPropertyEvents
		{
			private IVsShell shellService;
			private uint cookie;
			private Action callback;

			internal DteInitializer(IVsShell shellService, Action callback)
			{
				int hr;

				this.shellService = shellService;
				this.callback = callback;

				// Set an event handler to detect when the IDE is fully initialized
				hr = this.shellService.AdviseShellPropertyChanges(this, out this.cookie);

				Microsoft.VisualStudio.ErrorHandler.ThrowOnFailure(hr);
			}

			int IVsShellPropertyEvents.OnShellPropertyChange(int propid, object var)
			{
				int hr;
				bool isShellInitialized = false;

				switch (propid)
				{
					// This was for VS 2005, 2008
					//case (int) __VSSPROPID.VSSPROPID_Zombie:

					//   isShellInitialized = !(bool)var;
					//   break;

					// This is for VS 2010 and higher
					case (int)__VSSPROPID4.VSSPROPID_ShellInitialized:

						isShellInitialized = (bool)var;
						break;
				}

				if (isShellInitialized)
				{
					// Release the event handler to detect when the IDE is fully initialized
					hr = this.shellService.UnadviseShellPropertyChanges(this.cookie);

					Microsoft.VisualStudio.ErrorHandler.ThrowOnFailure(hr);

					this.cookie = 0;

					this.callback();
				}
				return VSConstants.S_OK;
			}
		}

		//private DteInitializer m_dteInitializer;

		/// <summary>
		/// Initializes a new instance of the <see cref="Macros"/> class.
		/// </summary>
		public Macros()
        {
            //vs = (DTE2)(GetGlobalService(typeof(DTE)));
            // Inside this method you can place any initialization code that does not require
            // any Visual Studio service because at this point the package object is created but
            // not sited yet inside Visual Studio environment. The place to do all the other
            // initialization is the Initialize method.
        }

        #region Package Members

        /// <summary>
        /// Initialization of the package; this method is called right after the package is sited, so this is the place
        /// where you can put all the initialization code that rely on services provided by VisualStudio.
        /// </summary>
		protected override void Initialize()
		{
			//IVsShell shellService;

			base.Initialize();

			// Try to retrieve the DTE instance at this point
			InitializeDte();

			// If not retrieved, we must wait for the Visual Studio Shell to be initialized
			//if (vs == null){
			//	vs = (DTE2)GetService(typeof(DTE));
			//}
			if (vs == null)
			{
				// Note: if targetting only VS 2015 and higher, we could use this:
				KnownUIContexts.ShellInitializedContext.WhenActivated(() => this.InitializeDte());

				// For VS 2005 and higher, we use this:
				//shellService = this.GetService(typeof(SVsShell)) as IVsShell;

				//m_dteInitializer = new DteInitializer(shellService, this.InitializeDte);
			}
		}

		private void InitializeDte()
		{
			vs = this.GetService(typeof(SDTE)) as DTE2;

			//m_dteInitializer = null;
		}
		
		#endregion
	}
}
