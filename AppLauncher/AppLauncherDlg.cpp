
// AppLauncherDlg.cpp: archivo de implementación
//

#include "pch.h"
#include "framework.h"
#include "AppLauncher.h"
#include "AppLauncherDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cuadro de diálogo CAboutDlg utilizado para el comando Acerca de

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Datos del cuadro de diálogo
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV

// Implementación
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cuadro de diálogo de CAppLauncherDlg



CAppLauncherDlg::CAppLauncherDlg(CWnd* pParent /*= nullptr*/)
	: CDialogEx(IDD_APPLAUNCHER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAppLauncherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_ResolutionCBoxCtrl);
	DDX_Control(pDX, IDC_COMBO2, m_VideoModeCBoxCtrl);
	DDX_Control(pDX, IDC_COMBO3, m_ArchitectureCBoxCtrl);
	DDX_Control(pDX, IDC_COMBO4, m_ConfigCBoxCtrl);
	DDX_Control(pDX, IDC_COMBO5, m_ClassCBoxCtrl);
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDC_MFCEDITBROWSE1, m_EditBrowseCtrl);
}

BEGIN_MESSAGE_MAP(CAppLauncherDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_COMBO1, &CAppLauncherDlg::OnCbnSelchangeResCombo)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CAppLauncherDlg::OnCbnSelchangeVideoCombo)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CAppLauncherDlg::OnCbnSelchangeArchCombo)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CAppLauncherDlg::OnCbnSelchangeConfigCombo)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CAppLauncherDlg::OnCbnSelchangeClassCombo)
	ON_EN_CHANGE(IDC_MFCEDITBROWSE1, &CAppLauncherDlg::OnEnChangeDirectoryBrowser)
	ON_BN_CLICKED(IDOK, &CAppLauncherDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// Controladores de mensajes de CAppLauncherDlg

BOOL CAppLauncherDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Agregar el elemento de menú "Acerca de..." al menú del sistema.

	// IDM_ABOUTBOX debe estar en el intervalo de comandos del sistema.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Establecer el icono para este cuadro de diálogo.  El marco de trabajo realiza esta operación
	//  automáticamente cuando la ventana principal de la aplicación no es un cuadro de diálogo
	SetIcon(m_hIcon, TRUE);			// Establecer icono grande
	SetIcon(m_hIcon, FALSE);		// Establecer icono pequeño

	// TODO: agregar aquí inicialización adicional

	m_ResolutionCBoxCtrl.AddString(L"640x480");
	m_ResolutionCBoxCtrl.AddString(L"800x600");
	m_ResolutionCBoxCtrl.AddString(L"1024x768");
	m_ResolutionCBoxCtrl.AddString(L"1360x768");
	m_ResolutionCBoxCtrl.AddString(L"1920x1080");
	m_ResolutionCBoxCtrl.SetCurSel(2);
	

	m_VideoModeCBoxCtrl.AddString(L"Windowed");
	m_VideoModeCBoxCtrl.AddString(L"Fullscreen");
	m_VideoModeCBoxCtrl.SetCurSel(1);
	

	m_ArchitectureCBoxCtrl.AddString(L"x64");
	m_ArchitectureCBoxCtrl.AddString(L"x86");
	m_ArchitectureCBoxCtrl.SetCurSel(0);
	

	m_ConfigCBoxCtrl.AddString(L"Debug");
	m_ConfigCBoxCtrl.AddString(L"Release");
	m_ConfigCBoxCtrl.SetCurSel(0);
	

	m_ClassCBoxCtrl.AddString(L"CAppParcial2");
	m_ClassCBoxCtrl.AddString(L"CAppParcial1");
	m_ClassCBoxCtrl.AddString(L"CAppEmpty");
	m_ClassCBoxCtrl.AddString(L"CAppCubeTest");
	m_ClassCBoxCtrl.AddString(L"CAppGeometricFigures");
	m_ClassCBoxCtrl.AddString(L"CAppObjLoader");
	m_ClassCBoxCtrl.SetCurSel(5);
	

	return TRUE;  // Devuelve TRUE  a menos que establezca el foco en un control
}

void CAppLauncherDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// Si agrega un botón Minimizar al cuadro de diálogo, necesitará el siguiente código
//  para dibujar el icono.  Para aplicaciones MFC que utilicen el modelo de documentos y vistas,
//  esta operación la realiza automáticamente el marco de trabajo.

void CAppLauncherDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Contexto de dispositivo para dibujo

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Centrar icono en el rectángulo de cliente
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Dibujar el icono
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}

	//Creación de imagen. No tengo la menor idea qué sucede aquí.
	CStatic* pPCBox = (CStatic*)(GetDlgItem(IDC_STATIC_IMAGE));
	CRect rect;
	pPCBox->GetClientRect(rect);
	CClientDC dc(pPCBox);
	CImage image;
	image.Load(L"goeshard.jpg");
	CBitmap bitmap;
	bitmap.Attach(image.Detach());
	CDC memoryDC;
	memoryDC.CreateCompatibleDC(&dc);
	memoryDC.SelectObject(bitmap);
	BITMAP bmp;
	bitmap.GetBitmap(&bmp);
	dc.SetStretchBltMode(COLORONCOLOR);
	dc.StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), &memoryDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
}

// El sistema llama a esta función para obtener el cursor que se muestra mientras el usuario arrastra
//  la ventana minimizada.
HCURSOR CAppLauncherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CAppLauncherDlg::OnCbnSelchangeResCombo()
{
	// TODO: Agregue aquí su código de controlador de notificación de control
}


void CAppLauncherDlg::OnCbnSelchangeVideoCombo()
{
	// TODO: Agregue aquí su código de controlador de notificación de control
}


void CAppLauncherDlg::OnCbnSelchangeArchCombo()
{
	// TODO: Agregue aquí su código de controlador de notificación de control
}


void CAppLauncherDlg::OnCbnSelchangeConfigCombo()
{
	// TODO: Agregue aquí su código de controlador de notificación de control
}


void CAppLauncherDlg::OnCbnSelchangeClassCombo()
{
	// TODO: Agregue aquí su código de controlador de notificación de control
}


void CAppLauncherDlg::OnEnChangeDirectoryBrowser()
{
	// TODO:  Si éste es un control RICHEDIT, el control no
	// enviará esta notificación a menos que se invalide CDialogEx::OnInitDialog()
	// función y llamada CRichEditCtrl().SetEventMask()
	// con la marca ENM_CHANGE ORed en la máscara.

	// TODO:  Agregue aquí el controlador de notificación de controles
	//m_EditBrowseCtrl.GetWindowTextW(m_ProjectDir);
	m_OkButton.EnableWindow(true);
}


void CAppLauncherDlg::OnBnClickedOk()
{
	// TODO: Agregue aquí su código de controlador de notificación de control
	CDialogEx::OnOK();

	CString m_Resolution;
	CString m_VideoMode;
	CString m_Architecture;
	CString m_Configuration;
	CString m_Class;
	CString m_ProjectDir;

	m_ResolutionCBoxCtrl.GetWindowTextW(m_Resolution);
	m_VideoModeCBoxCtrl.GetWindowTextW(m_VideoMode);
	m_ArchitectureCBoxCtrl.GetWindowTextW(m_Architecture);
	m_ConfigCBoxCtrl.GetWindowTextW(m_Configuration);
	m_ClassCBoxCtrl.GetWindowTextW(m_Class);
	m_EditBrowseCtrl.GetWindowTextW(m_ProjectDir);

	CString resWidth;
	CString resHeight;
	if (m_Resolution == "640x480") {
		resWidth = "640";
		resHeight = "480";
	} else if (m_Resolution == "800x600") {
		resWidth = "800";
		resHeight = "600";
	} else if (m_Resolution == "1024x768") {
		resWidth = "1024";
		resHeight = "768";
	} else if (m_Resolution == "1360x768") {
		resWidth = "1360";
		resHeight = "678";
	} else if (m_Resolution == "1920x1080") {
		resWidth = "1920";
		resHeight = "1080";
	}

	CString exeDir;
	exeDir.Append(m_ProjectDir + L"\\Binaries\\Exe\\" + m_Architecture + L"\\" + m_Configuration + L"\\uad_progra3.exe");
	exeDir.Append(' ' + resWidth + ' ' + resHeight + ' ' + m_VideoMode + ' ' + m_Class);

	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));

	CreateProcessW(nullptr, CT2W(exeDir), nullptr, nullptr, false, 0, nullptr, nullptr, &si, &pi);
}
