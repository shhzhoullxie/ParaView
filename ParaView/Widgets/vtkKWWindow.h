/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkKWWindow.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

Copyright (c) 2000-2001 Kitware Inc. 469 Clifton Corporate Parkway,
Clifton Park, NY, 12065, USA.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * Neither the name of Kitware nor the names of any contributors may be used
   to endorse or promote products derived from this software without specific 
   prior written permission.

 * Modified source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
// .NAME vtkKWWindow - a window superclass which holds one or more views
// .SECTION Description
// This class represents a top level window with menu bar and status
// line. It is designed to hold one or more vtkKWViews in it.

#ifndef __vtkKWWindow_h
#define __vtkKWWindow_h

#include "vtkKWWidget.h"

class vtkKWApplication;
class vtkKWCheckButton;
class vtkKWFrame;
class vtkKWImageLabel;
class vtkKWLabel;
class vtkKWLabeledFrame;
class vtkKWLoadSaveDialog;
class vtkKWMenu;
class vtkKWMessageDialog;
class vtkKWNotebook;
class vtkKWProgressGauge;
class vtkKWRadioButton;
class vtkKWSplitFrame;
class vtkKWView;
class vtkKWViewCollection;

//BTX
class vtkKWWindowMenuEntry;
template<class DType> class vtkVector;
//ETX

#define VTK_KW_PREFERENCES_PAGE_LABEL "Preferences"
#define VTK_KW_PAGE_SETUP_MENU_LABEL "Page Setup"
#define VTK_KW_SAVE_WINDOW_GEOMETRY_REG_KEY "SaveWindowGeometry"

class VTK_EXPORT vtkKWWindow : public vtkKWWidget
{
public:
  static vtkKWWindow* New();
  vtkTypeRevisionMacro(vtkKWWindow,vtkKWWidget);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Create a Tk widget
  virtual void Create(vtkKWApplication *app, char *args);

  // Description::
  // Exit this application closing all windows.
  virtual void Exit();

  // Description::
  // Close this window, possibly exiting the application if no more
  // windows are open.
  virtual void Close();
  virtual void CloseNoPrompt();

  // Description::
  // Display help info for this window.
  virtual void DisplayHelp();

  // Description::
  // Display about info for this window.
  virtual void DisplayAbout();

  // Description:
  // Set the text for the status bar of this window.
  void SetStatusText(const char *);
  const char *GetStatusText();
  
  // Description:
  // Load in a Tcl based script to drive the application. If called
  // without an argument it will open a file dialog.
  virtual void LoadScript();
  virtual void LoadScript(const char *name);

  // Description:
  // Popup the vtk warning message
  virtual void WarningMessage(const char* message);
  
  // Description:
  // Popup the vtk error message
  virtual void ErrorMessage(const char* message);

  // Description:
  // Show or hide the error / warning icon in the tray.
  // 2 - red icon, 1 - icon, 0 - hide.
  virtual void SetErrorIcon(int);
  vtkBooleanMacro(ErrorIcon, int); 

  // Description:
  // Process the click on the error icon.
  virtual void ProcessErrorClick();
  
  // Description:
  // Allow windows to get at the different menu entries. In some
  // cases the menu entry may be created if it doesn't already
  // exist.
  vtkGetObjectMacro(Menu,vtkKWMenu);
  vtkGetObjectMacro(MenuFile,vtkKWMenu);
  vtkKWMenu *GetMenuEdit();
  vtkKWMenu *GetMenuView();
  vtkKWMenu *GetMenuWindow();
  
  // Description:
  // Operations on the views.
  void AddView(vtkKWView *);
  void RemoveView(vtkKWView *);
  virtual void SetSelectedView(vtkKWView *);
  vtkGetObjectMacro(SelectedView,vtkKWView);
  vtkKWViewCollection *GetViews() {return this->Views;};
  vtkGetObjectMacro(ViewFrame,vtkKWWidget);
  
  // Description:
  // Proiperties may be bound to the window or view or
  // something else. The CreateDefaultPropertiesParent method
  // will create an attachment point for the properties at
  // the window level.
  vtkGetObjectMacro(PropertiesParent,vtkKWWidget);
  void SetPropertiesParent(vtkKWWidget*);
  void CreateDefaultPropertiesParent();

  // Description:
  // Create preferences properties page in the notebook.
  void CreatePreferencesProperties();

  // Description:
  // Provide hide/show functionality of properties
  void HideProperties();
  void ShowProperties();
  void OnToggleProperties();
  
  // Description:
  // Callback to display window properties (usually, application settings)
  void ShowWindowProperties();

  // Description::
  // Override Unregister since widgets have loops.
  virtual void UnRegister(vtkObjectBase *o);

  // Description::
  // Add to the menu a list of recently used files. Specify a menu entry,
  // which will be used to position menu. Pick NULL if you want to use
  // "Close". The command
  // is the command to execute when a file is selected.
  virtual void AddRecentFilesToMenu(const char *menuEntry, vtkKWObject *target);
  virtual void AddRecentFile(const char *key, const char *name, vtkKWObject *target,
                             const char *command);
  
  // Description:
  // Return the index of the entry above the MRU File list
  // in the file menu. This is useful because most menu options
  // go above the MRU list, hence above this index.
  int GetFileMenuIndex();

  // Description:
  // Install a menu bar into this window.
  void InstallMenu(vtkKWMenu* menu);

  // Description:
  // Callbacks used to set the print quality.
  void OnPrint(int propagate, int resolution);
  vtkGetMacro(PrintTargetDPI,float);
  
  // Description:
  // Allow access to the notebook object.
  vtkGetObjectMacro(Notebook,vtkKWNotebook);

  // Description:
  // This toolbar frame is below the menu. It is empty initially.
  // Subclasses can add toolbars buttons as necessary.
  vtkGetObjectMacro(ToolbarFrame, vtkKWWidget);

  // Description:
  // The status frame.
  vtkGetObjectMacro(StatusFrame, vtkKWWidget);

  // Description:
  // Get the progress gauge widget.  The progress gauge is displayed
  // in the Status frame on the bottom right of the window.
  vtkGetObjectMacro(ProgressGauge, vtkKWProgressGauge);
 
  // Description:
  // Will the window add a help menu?
  vtkSetClampMacro( SupportHelp, int, 0, 1 );
  vtkGetMacro( SupportHelp, int );
  vtkBooleanMacro( SupportHelp, int );

  // Description:
  // Class of the window. Passed to the toplevel command.
  vtkSetStringMacro(WindowClass);
  vtkGetStringMacro(WindowClass);

  // Description:
  // Title of the window (if empty, try to use the app name). 
  virtual void SetTitle(const char*);
  virtual char* GetTitle();

  //Description:
  // Set/Get PromptBeforeClose
  vtkSetMacro(PromptBeforeClose, int);
  vtkGetMacro(PromptBeforeClose, int);

  // Description:
  // The extension used in LoadScript. Default is .tcl.
  vtkSetStringMacro(ScriptExtension);
  vtkGetStringMacro(ScriptExtension);

  // Description:
  // The type name used in LoadScript. Default is Tcl.
  vtkSetStringMacro(ScriptType);
  vtkGetStringMacro(ScriptType);

  // Description:
  // Call render on all views
  void Render();

  //Description:
  // Set/Get Number of recent files in the menu.
  vtkSetClampMacro(NumberOfRecentFiles, unsigned int, 4, 10);
  vtkGetMacro(NumberOfRecentFiles, unsigned int);

//BTX
  //Description:
  // Set or get the registery value for the application.
  // When storing multiple arguments, separate with spaces.
  // If the level is lower than current registery level, operation 
  // will be successfull.
  /*
  int SetWindowRegisteryValue(int level, const char* subkey, const char* key, 
                        const char* format, ...);
  int GetWindowRegisteryValue(int level, const char* subkey, const char* key, 
                        char*value);
  int DeleteRegisteryValue(int level, const char* subkey, const char* key);
  */
  
  // Description:
  // Get float registery value (zero if not found).
  // If the level is lower than current registery level, operation 
  // will be successfull.
  float GetFloatRegisteryValue(int level, const char* subkey, 
                               const char* key);
  int   GetIntRegisteryValue(int level, const char* subkey, const char* key);

  // Description:
  // Perform a boolean check of the value in registery. If the value 
  // at the key is trueval, then return true, otherwise return false.
  int BooleanRegisteryCheck(int level, const char* subkey, const char* key, const char* trueval);
  
  // Description:
  // Save or retrieve color from registery. If color does not 
  // exist, it will retrieve -1, -1 ,-1.
  void SaveColor(int level, const char*, float rgb[3]);
  void RetrieveColor(int level, const char*, float rgb[3]);

  // Description:
  // Save or retrieve the last path of the dialog to the registery.
  // The string argument is the registery key.
  void SaveLastPath(vtkKWLoadSaveDialog *, const char*);
  void RetrieveLastPath(vtkKWLoadSaveDialog *, const char*);

//ETX
  
  // Description:
  // Print a list of recent files to the standard output for debug
  // purposes.
  void PrintRecentFiles();
  
  // Description:
  // Callback to handle dialog settings change
  void OnDialogSettingsChange();

protected:
  vtkKWWindow();
  ~vtkKWWindow();
  virtual void SerializeRevision(ostream& os, vtkIndent indent);

  void InsertRecentFileToMenu(const char *filename, 
                              vtkKWObject *taret, 
                              const char *command);
  void UpdateRecentMenu(const char *key);
  void StoreRecentMenuToRegistery(const char *key);

  unsigned int NumberOfRecentFiles;

  // Description:
  // This is the menu entry that recent files will be above.
  // If it is null, then pick "Close"
  vtkSetStringMacro(RecentFilesMenuTag);
  vtkGetStringMacro(RecentFilesMenuTag);
  char *RecentFilesMenuTag;

  // Description:
  // Display the exit dialog.
  int ExitDialog();


  virtual void CreateStatusImage();

  int NumberOfMRUFiles;
  int RealNumberOfMRUFiles;

  vtkKWNotebook *Notebook;

  vtkKWView           *SelectedView;
  vtkKWViewCollection *Views;
  vtkKWSplitFrame     *MiddleFrame; // Contains view frame & properties parent.

  vtkKWMenu *Menu;
  vtkKWMenu *MenuFile;
  vtkKWMenu *MenuEdit;
  vtkKWMenu *MenuView;
  vtkKWMenu *MenuWindow;
  vtkKWMenu *MenuHelp;
  vtkKWMenu *PageMenu;

  vtkKWWidget *StatusFrame;
  vtkKWWidget *StatusImage;

  vtkKWProgressGauge *ProgressGauge;
  vtkKWWidget        *ProgressFrame;

  vtkKWFrame      *TrayFrame;
  vtkKWImageLabel *TrayImage;

  vtkKWLabel *StatusLabel;
  char       *StatusImageName;

  vtkKWWidget *PropertiesParent;
  vtkKWWidget *ViewFrame;
  vtkKWWidget *ToolbarFrame;

  vtkKWMessageDialog *ExitDialogWidget;

  float PrintTargetDPI;
  char  *ScriptExtension;
  char  *ScriptType;
  int   SupportHelp;
  char  *WindowClass;
  char  *Title;
  int   PromptBeforeClose;
  int   InExit;

  vtkKWLabeledFrame *DialogSettingsFrame;
  vtkKWCheckButton  *DialogSettingsConfirmExitCheck;
  vtkKWCheckButton  *DialogSettingsSaveWindowGeometry;
  vtkKWCheckButton  *DialogSettingsShowSplashScreenCheck;
  
//BTX
  // Description:
  // This vector holds the list of most recently used files.
  vtkVector<vtkKWWindowMenuEntry*> *RecentFilesVector;
//ETX

private:
  vtkKWWindow(const vtkKWWindow&); // Not implemented
  void operator=(const vtkKWWindow&); // Not implemented
};


#endif


