
// BaikuDemo.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "BaikuDemo.h"
#include "MainFrm.h"

#include "BaikuDemoDoc.h"
#include "BaikuDemoView.h"
#include "AboutDlg.h"
#include "LoginDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBaikuDemoApp

BEGIN_MESSAGE_MAP(CBaikuDemoApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CBaikuDemoApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CBaikuDemoApp construction

CBaikuDemoApp::CBaikuDemoApp()
{
	m_bHiColorIcons = TRUE;
	m_strAppPath = _T("");
	m_pTheme = NULL;

	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("BaikuDemo.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CBaikuDemoApp object

CBaikuDemoApp theApp;


// CBaikuDemoApp initialization

BOOL CBaikuDemoApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	// Gdiplus initialize:
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);


	CWinAppEx::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	TRACE(_T("start load theme here."));
	LoadTheme();

	CLoginDlg dlg;
	dlg.DoModal();
	if (!dlg.IsLogin()) {
		TRACE(_T("Don't login!!!"));
		return FALSE;
	}
	return TRUE;

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CBaikuDemoDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CBaikuDemoView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	return TRUE;
}

int CBaikuDemoApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	// Unload Theme
	UnloadTheme();
	GdiplusShutdown(m_gdiplusToken);

	return CWinAppEx::ExitInstance();
}

// CBaikuDemoApp message handlers

// App command to run the dialog
void CBaikuDemoApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
void CBaikuDemoApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}
void CBaikuDemoApp::LoadCustomState()
{
}
void CBaikuDemoApp::SaveCustomState()
{
}
void CBaikuDemoApp::LoadTheme()
{
	CString strPicPath = _T("");
	CString strThemePath = GetAppResPath();
	CBitmap*	pBitmap = NULL;
	CBitmap*	pBgBitmap = NULL;
	Image*		pMinBtnImage = NULL;
	Image*		pMaxBtnImage = NULL;
	Image*		pCloseBtnImage = NULL;
	Image*		pRestoreBtnImage = NULL;

	m_pTheme = new ThemeDialog();
	
	strPicPath = strThemePath + _T("\\dialog.bmp");
	pBitmap = LoadBmpImage(strPicPath);
	strPicPath = strThemePath + _T("\\background.bmp");
	pBgBitmap = LoadBmpImage(strPicPath);
	strPicPath = strThemePath + _T("\\max_btn_dlg.png");
	pMaxBtnImage = LoadPngImage(strPicPath);
	strPicPath = strThemePath + _T("\\min_btn_dlg.png");
	pMinBtnImage = LoadPngImage(strPicPath);
	strPicPath = strThemePath + _T("\\close_btn_dlg.png");
	pCloseBtnImage = LoadPngImage(strPicPath);
	strPicPath = strThemePath + _T("\\restore_btn_dlg.png");
	pRestoreBtnImage = LoadPngImage(strPicPath);
	
	m_pTheme->SetBorder(pBitmap, pBitmap);
	m_pTheme->SetBackground(pBgBitmap);
	m_pTheme->SetMargins(3, 3, 3, 3, 3, 3, 27, 3);
	m_pTheme->SetMinButton(1, CPoint(45, 6), pMinBtnImage);
	m_pTheme->SetMaxButton(1, CPoint(25, 6), pMaxBtnImage, pRestoreBtnImage);
	m_pTheme->SetCloseButton(1, CPoint(5, 6), pCloseBtnImage);
}
void CBaikuDemoApp::UnloadTheme()
{
	if (m_pTheme) {
		if (m_pTheme->m_pBmpActiveBorder) {
			delete m_pTheme->m_pBmpActiveBorder;
			m_pTheme->m_pBmpActiveBorder = 0;
		}
		if (m_pTheme->m_pBmpBackground) {
			delete m_pTheme->m_pBmpBackground;
			m_pTheme->m_pBmpBackground = 0;
		}
		delete m_pTheme;
		m_pTheme = 0;
	}
}
CString CBaikuDemoApp::GetAppPath()
{
	if (m_strAppPath.IsEmpty()) {
		TCHAR appPath[MAX_PATH] = {0};
		GetModuleFileName(NULL, appPath, sizeof(appPath));
		if (_tcslen(appPath) > 0) {
			*_tcsrchr(appPath, _T('\\')) = '\0';
			m_strAppPath = appPath;
		}
	}
	return m_strAppPath;
}
CString CBaikuDemoApp::GetAppResPath()
{
	if (m_strAppResPath.IsEmpty()) {
		CString str = GetAppPath();
		m_strAppResPath.Format(_T("%s\\resource\\themes\\default"), str);
	}
	return m_strAppResPath;
}