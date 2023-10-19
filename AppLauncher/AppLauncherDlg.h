
// AppLauncherDlg.h: archivo de encabezado
//

#pragma once


// Cuadro de diálogo de CAppLauncherDlg
class CAppLauncherDlg : public CDialogEx
{
// Construcción
public:
	CAppLauncherDlg(CWnd* pParent = nullptr);	// Constructor estándar

// Datos del cuadro de diálogo
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_APPLAUNCHER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// Compatibilidad con DDX/DDV


// Implementación
protected:
	HICON m_hIcon;

	// Funciones de asignación de mensajes generadas
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// Resolultion Combo Box Control
	CComboBox m_ResolutionCBoxCtrl;
	// Video Mode Combo Box Control
	CComboBox m_VideoModeCBoxCtrl;
	// Architecture Combo Box Control
	CComboBox m_ArchitectureCBoxCtrl;
	// Configuration Combo Box Control
	CComboBox m_ConfigCBoxCtrl;
	// Class Combo Box Control
	CComboBox m_ClassCBoxCtrl;
	// Edit Browse Control
	CMFCEditBrowseCtrl m_EditBrowseCtrl;
	// Ok Button
	CButton m_OkButton;

	afx_msg void OnCbnSelchangeResCombo();
	afx_msg void OnCbnSelchangeVideoCombo();
	afx_msg void OnCbnSelchangeArchCombo();
	afx_msg void OnCbnSelchangeConfigCombo();
	afx_msg void OnCbnSelchangeClassCombo();
	afx_msg void OnEnChangeDirectoryBrowser();
	afx_msg void OnBnClickedOk();
	
	
};
