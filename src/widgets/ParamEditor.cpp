#include "Appearance.h"
#include "ParamEditor.h"
#include "UnitWidgets.h"
#include "helpers/OriWidgets.h"
#include "helpers/OriTools.h"
#include "widgets/OriInfoPanel.h"
#include "widgets/OriValueEdit.h"

#include <QBoxLayout>
#include <QDebug>
#include <QLabel>

////////////////////////////////////////////////////////////////////////////////
//                                ParamEditor
////////////////////////////////////////////////////////////////////////////////

ParamEditor::ParamEditor(Z::Parameter *param, bool showName) : QWidget(0)
{
    _param = param;

    int def_spacing = Ori::Gui::layoutSpacing();

    auto layout = Ori::Gui::layoutH(this, 0, 0, {});
    if (showName)
        Ori::Gui::populate(layout, {
            _labelName = new QLabel(param->name()),
            Ori::Gui::spacing(2 * def_spacing),
        });
    Ori::Gui::populate(layout, {
        _labelLabel = Z::Gui::symbolLabel(param->label() % " = "),
        _valueEditor = new Ori::Widgets::ValueEdit,
        Ori::Gui::spacing(Ori::Gui::borderWidth()),
        _unitsSelector = new UnitComboBox(_param->dim()),
    });

    Z::Gui::setValueFont(_valueEditor);

    // make some room around widgets to make highlighting visible
    int hs = def_spacing / 2 + 1;
    setContentsMargins(def_spacing, hs, hs, hs);

    populate();

    _valueEditor->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);

    connect(_valueEditor, SIGNAL(focused(bool)), this, SLOT(editorFocused(bool)));
    connect(_valueEditor, SIGNAL(keyPressed(int)), this, SLOT(editorKeyPressed(int)));
    connect(_unitsSelector, SIGNAL(focused(bool)), this, SLOT(editorFocused(bool)));

    _valueEditor->selectAll();
}

void ParamEditor::populate()
{
    _valueEditor->setValue(_param->value().value());
    _unitsSelector->setSelectedUnit(_param->value().unit());
}

void ParamEditor::apply()
{
    if (_valueEditor->ok())
        _param->setValue(Z::Value(_valueEditor->value(), _unitsSelector->selectedUnit()));

// TODO validate value
//    auto value = _editor->value();
//    auto result = _param->setValue(value, _param->unit);
//    if (result)
//        emit valueRejected(_param, value, result);
}

void ParamEditor::focus()
{
    _valueEditor->setFocus();
    _valueEditor->selectAll();
}

void ParamEditor::editorFocused(bool focus)
{
    QPalette p;

    if (focus)
    {
        p.setColor(QPalette::Button, Ori::Color::blend(p.color(QPalette::Button), p.color(QPalette::Light), 0.5));
        setAutoFillBackground(true);
        setBackgroundRole(QPalette::Button);
        emit focused();
    }
    else
        setAutoFillBackground(false);

    setPalette(p);
}

void ParamEditor::editorKeyPressed(int key)
{
    switch (key)
    {
    case Qt::Key_Up: emit goingFocusPrev(); break;
    case Qt::Key_Down: emit goingFocusNext(); break;
    default:;
    }
}

void ParamEditor::mousePressEvent(QMouseEvent *e)
{
    QWidget::mousePressEvent(e);
    focus();
}

QWidget* ParamEditor::labelName() const { return _labelName; }
QWidget* ParamEditor::labelLabel() const { return _labelLabel; }
QWidget* ParamEditor::valueEditor() const { return _valueEditor; }
QWidget* ParamEditor::unitsSelector() const { return _unitsSelector; }

////////////////////////////////////////////////////////////////////////////////
//                                ParamsEditor
////////////////////////////////////////////////////////////////////////////////

ParamsEditor::ParamsEditor(Z::Parameters *params, QWidget *parent) : QWidget(parent), _params(params)
{
    auto layoutParams = Ori::Gui::layoutV(0, 0, {});

    for (auto param : *_params)
    {
        if (!param->visible()) continue;
        auto editor = new ParamEditor(param, false);
        connect(editor, SIGNAL(focused()), this, SLOT(paramFocused()));
        connect(editor, SIGNAL(goingFocusNext()), this, SLOT(focusNextParam()));
        connect(editor, SIGNAL(goingFocusPrev()), this, SLOT(focusPrevParam()));
// TODO validate value
//        connect(editor, SIGNAL(valueRejected(Z::Parameter*,double,const char*)),
//            this, SLOT(valueRejected(Z::Parameter*,double,const char*)));
        _editors.append(editor);
        layoutParams->addWidget(editor);
    }

    auto layoutInfo = new QHBoxLayout;
    layoutInfo->addWidget(_infoPanel = new Ori::Widgets::InfoPanel);

    Ori::Gui::layoutV(this, 0, -1, {layoutParams, 0, layoutInfo});

    layoutInfo->setMargin(layoutInfo->spacing()/2+1);
}

void ParamsEditor::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);
    adjustEditors();
}

void ParamsEditor::adjustEditors()
{
    int w_name = 0, w_label = 0, w_unit = 0;
    for (auto editor: _editors)
    {
        if (editor->labelName())
            w_name = qMax(w_name, editor->labelName()->width());
        if (editor->labelLabel())
            w_label = qMax(w_label, editor->labelLabel()->width());
        w_unit = qMax(w_unit, editor->unitsSelector()->width());
    }
    for (auto editor: _editors)
    {
        if (w_name > 0) editor->labelName()->setFixedWidth(w_name);
        if (w_label > 0) editor->labelLabel()->setFixedWidth(w_label);
        if (w_unit > 0) editor->unitsSelector()->setFixedWidth(w_unit);
    }
}

void ParamsEditor::populate()
{
    for (auto editor : _editors) editor->populate();
}

void ParamsEditor::apply()
{
    for (auto editor : _editors) editor->apply();
}

void ParamsEditor::focus()
{
    if (_editors.size() > 0)
        _editors.at(0)->focus();
}

void ParamsEditor::focus(Z::Parameter *param)
{
    for (auto editor : _editors)
        if (editor->parameter() == param)
        {
            editor->focus();
            return;
        }
}

void ParamsEditor::paramFocused()
{
    for (auto editor : _editors)
        if (editor == sender())
        {
            auto p = editor->parameter();
            _infoPanel->setInfo("<b>" % p->name() % "</b><br>" % p->description());
            return;
        }
}

void ParamsEditor::focusNextParam()
{
    int count = _editors.size();
    if (count > 1)
    {
        void *param = sender();
        for (int i = 0; i < count; i++)
            if (_editors.at(i) == param)
            {
                int next = i+1;
                if (next > count-1) next = 0;
                _editors.at(next)->focus();
                return;
            }
    }
}

void ParamsEditor::focusPrevParam()
{
    int count = _editors.size();
    if (count > 1)
    {
        void *param = sender();
        for (int i = 0; i < count; i++)
            if (_editors.at(i) == param)
            {
                int prev = i-1;
                if (prev < 0) prev = count-1;
                _editors.at(prev)->focus();
                return;
            }
    }
}

// TODO validation
//void ParamsEditor::valueRejected(Z::Parameter *param, double value, const char *reason)
//{
//    emit paramRejected(param, value, reason);
//    reinterpret_cast<ParamEditor*>(sender())->populate(); // show unchanged value if rejected
//}

////////////////////////////////////////////////////////////////////////////////
//                              ParamsEditorAbcd
////////////////////////////////////////////////////////////////////////////////

ParamsEditorAbcd::ParamsEditorAbcd(const QString& title, const Z::Parameters &params) : QGroupBox(title, 0), _params(params)
{
    if (_params.size() != 4) return;

    auto editA = new Ori::Widgets::ValueEdit;
    auto editB = new Ori::Widgets::ValueEdit;
    auto editC = new Ori::Widgets::ValueEdit;
    auto editD = new Ori::Widgets::ValueEdit;

    _editors.append(editA);
    _editors.append(editB);
    _editors.append(editC);
    _editors.append(editD);

    auto layoutMain = new QGridLayout(this);
    layoutMain->addWidget(editA, 0, 0);
    layoutMain->addWidget(editB, 0, 1);
    layoutMain->addWidget(editC, 1, 0);
    layoutMain->addWidget(editD, 1, 1);
}

void ParamsEditorAbcd::populate()
{
    for (int i = 0; i < _params.size(); i++)
        _editors[i]->setValue(_params[i]->value().value());
}

void ParamsEditorAbcd::apply()
{
    for (int i = 0; i < _params.size(); i++)
        _params[i]->setValue(Z::Value(_editors[i]->value(), Z::Units::none()));
}

void ParamsEditorAbcd::focus()
{
    _editors.at(0)->setFocus();
}

