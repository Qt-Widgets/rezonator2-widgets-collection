#ifndef PROJECT_WINDOW_H
#define PROJECT_WINDOW_H

#include <QMainWindow>

#include "core/Schema.h"
#include "AppSettings.h"
#include "SchemaWindows.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QToolBar;
QT_END_NAMESPACE

class SchemaMdiArea;
class ProjectOperations;
class CalcManager;

namespace Ori {
    class Styler;
    class Translator;
    class MruFileList;

    namespace Widgets {
        class MruMenu;
    }
}

class ProjectWindow : public QMainWindow, public SchemaToolWindow
{
    Q_OBJECT

public:
    ProjectWindow();
    ~ProjectWindow();

    ///// inherits from SchemaWindow
    //WindowRole role() const { return ProjectWindowRole; }

    ///// inherits from SchemaListener
    void schemaChanged(Schema*);
    void schemaLoaded(Schema*);
    void schemaSaved(Schema*);
    void schemaParamsChanged(Schema*) { updateTitle(); }

protected:
    void closeEvent(class QCloseEvent*);

private:
    QMenu *menuFile, *menuEdit, *menuHelp, *menuWindow, *menuTools,
          *menuElement, *menuFunctions, *menuView,
          *menuLangs, *menuStyles;

    QAction *actnFileNew, *actnFileOpen, *actnFileExit, *actnFileSave, *actnFileSaveCopy,
            *actnFileSaveAs, /* *actnFileProp,*/ *actnFilePump, *actnFileLambda, *actnFileSummary;

    QAction *actnEditCut, *actnEditCopy,
            *actnEditPaste, *actnEditSelectAll;

    QAction *actnFuncRoundTrip, *actnFuncStabMap, *actnFuncStabMap2d,
            *actnFuncRepRate, *actnFuncMultFwd, *actnFuncMultBkwd,
            *actnFuncCaustic;

    QAction  /* *actnToolsBeamCalc,*/ *actnToolsCatalog, *actnToolsPrefs;

    QAction *actnWndClose, *actnWndCloseAll, *actnWndTile, *actnWndCascade,
            *actnWndSchema, *actnWndProtocol;

    QAction *actnHelpUpdates, *actnHelpHomepage, *actnHelpAbout;

    ProjectOperations* _operations;
    CalcManager* _calculations;
    SchemaMdiArea *_mdiArea;
    Ori::Styler* _styler;
    Ori::Translator* _translator;
    Ori::Widgets::MruMenu* _mruMenu;
    Ori::MruFileList *_mruList;

    void createActions();
    void createMenuBar();
    void createToolBars();
    void createStatusBar();

    void updateTitle();
    void updateStatusInfo();
    void updateStability();
    void updateActions();

    void loadSettings();
    void saveSettings();

private slots:
    void actionHelpAbout();
    void actionHelpHomePage();
    void actionHelpUpdate();

    void showElementsCatalog();
    void showPreferences();
    void showProtocolWindow();
    void showBeamCalculator();

    void updateMenuBar();
    void updateWindowMenu();
};

#endif // PROJECT_WINDOW_H
