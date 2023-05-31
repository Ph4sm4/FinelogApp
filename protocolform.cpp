#include "protocolform.h"
#include "ui_protocolform.h"
#include "inputmanager.h"
#include "fineloguser.h"
#include "userreport.h"
#include <QDate>
#include <QTime>
#include <QJsonDocument>
#include <QScroller>
#include <QScrollerProperties>
#include <QJsonObject>
#include <QScrollBar>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QTimer>
#include <QGroupBox>
#define ipcs isPositiveChoiseSelected

ProtocolForm::ProtocolForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProtocolForm)
{
    ui->setupUi(this);

    // protocol fields scroll area

    ui->protocol_fields->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded); // Show vertical scroll bar as needed
    ui->protocol_fields->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Disable horizontal scroll bar

    ui->protocol_fields->setWidgetResizable(true);
    ui->protocol_fields->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->protocol_fields->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    QScroller::grabGesture(ui->protocol_fields->viewport(), QScroller::TouchGesture); // Enable touch scrolling

    // Configure the scrolling behavior
    QScrollerProperties scrollerProperties2 = QScroller::scroller(
                                                  ui->protocol_fields->viewport())->scrollerProperties();
    scrollerProperties2.setScrollMetric(QScrollerProperties::DragVelocitySmoothingFactor, 0.6);
    scrollerProperties2.setScrollMetric(QScrollerProperties::MinimumVelocity, 0.0);
    scrollerProperties2.setScrollMetric(QScrollerProperties::MaximumVelocity, 0.6);
    scrollerProperties2.setScrollMetric(QScrollerProperties::AcceleratingFlickMaximumTime, 0.4);
    scrollerProperties2.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);
    QScroller::scroller(ui->protocol_fields->viewport())->setScrollerProperties(scrollerProperties2);

    QGraphicsDropShadowEffect* shadowEffect2 = new QGraphicsDropShadowEffect;
    shadowEffect2->setBlurRadius(20);
    shadowEffect2->setColor(QColor(0, 0, 0, 80));
    shadowEffect2->setOffset(0, 0);
    ui->protocol_fields->setGraphicsEffect(shadowEffect2);
}

ProtocolForm::~ProtocolForm()
{
    delete ui;
}

bool ProtocolForm::isPositiveChoiseSelected(QGroupBox *box)
{
    for(QRadioButton* radio : box->findChildren<QRadioButton*>()) {
        qDebug() << radio;
        if(!radio) {
            return false;
        }
        qDebug() << radio->isChecked();
        if(!radio->isChecked()) continue;
        if(radio->text() == "Ok" || radio->text() == "Tak") {
            return true;
        }
        else {
            return false;
        }
    }
    return false;
}

void ProtocolForm::prepareForm()
{
    // preparation of the form
    InputManager::disableButton(ui->sendForm);
    ui->formErrorLabel->setText("");
    ui->emailEdit->setText(currentUser->getEmail());
    ui->phoneEdit->setText(currentUser->getPhoneNumber());

    // temporary data, later we wanna get them from db, and upload them via admin
    ui->driverCombo->addItem("Kierowca 1");
    ui->driverCombo->addItem("Kierowca 2");
    ui->driverCombo->addItem("Kierowca 3");
    ui->driverCombo->addItem("Kierowca 4");

    ui->carCombo->addItem("Numer rejestracyjny 1");
    ui->carCombo->addItem("Numer rejestracyjny 2");
    ui->carCombo->addItem("Numer rejestracyjny 3");
    ui->carCombo->addItem("Numer rejestracyjny 4");

    ui->trailerCombo->addItem("Numer rejestracyjny naczepa 1");
    ui->trailerCombo->addItem("Numer rejestracyjny naczepa 2");
    ui->trailerCombo->addItem("Numer rejestracyjny naczepa 3");
    ui->trailerCombo->addItem("Numer rejestracyjny naczepa 4");

    ui->projectCombo->addItem("Projekt 1");
    ui->projectCombo->addItem("Projekt 2");
    ui->projectCombo->addItem("Projekt 3");
    ui->projectCombo->addItem("Projekt 4");

    // blocking all edits
    for(QLineEdit* edit : ui->protocol_fields->findChildren<QLineEdit*>()) {
        if(edit && edit->objectName() != "qt_spinbox_lineedit") {
            edit->setDisabled(true);
        }
    }

    // blocking all date edits
    for(QDateEdit* edit : ui->protocol_fields->findChildren<QDateEdit*>()) {
        if(edit && edit->objectName() != "qt_spinbox_lineedit"){
            edit->setDisabled(true);
            edit->setDisplayFormat("dd.MM.yyyy");
            edit->setDate(QDate(2000, 1, 1));
        }
    }
    // enabling only those that should be always enabled

    ui->dkvCardNumber->setEnabled(true);
    ui->hoyerCardNumber->setEnabled(true);
    ui->truckComments->setEnabled(true);
}

void ProtocolForm::hideSendOptions()
{
    ui->sendForm->setDisabled(true);
    ui->sendForm->hide();

    ui->formSendConfirmCheck->setDisabled(true);
    ui->formSendConfirmCheck->hide();
}

bool ProtocolForm::initializeFormData(const QString &contentName)
{
    //qDebug() << "content name received: " << contentName;
    QString path = "Reports/Content/" + contentName;
    QJsonObject formData = dbHandler.performAuthenticatedGET(path, currentUser->getIdToken());


    return true;
}

void ProtocolForm::on_sendForm_clicked()
{
    bool allFilled = true;
    for(QGroupBox* box : ui->protocol_fields->findChildren<QGroupBox*>()) {
        bool singleFilled = false;
        for(QRadioButton* button : box->findChildren<QRadioButton*>()) {
            if(button->isChecked()) {
                singleFilled = true;
                break;
            }
        }
        if(!singleFilled) {
            box->setStyleSheet("QGroupBox { border: 2px solid red; }");
        }
        else {
            box->setStyleSheet("QGroupBox { border: none; }");
        }
        if(!singleFilled) {
            allFilled = false;
        }
    }

    // check for the combo boxes and start setting up the admin panel!!!


    if(!allFilled) {
        ui->formErrorLabel->setText("Nie wszystkie pola zostały wypełnione");
        return;
    }
    ui->formErrorLabel->setText("");

    UserReport report;
    // #define ipcs isPositiveChoiseSelected (there is this define on top)
    report.email = ui->emailEdit->text();
    report.phoneNumber = ui->phoneEdit->text();
    report.driverName = ui->driverCombo->currentText();
    report.carName = ui->carCombo->currentText();
    report.semiTrailer = ui->trailerCombo->currentText();
    report.projectName = ui->projectCombo->currentText();
    report.odometerReading = ui->odometerSpin->value();
    report.fuelInTank = ui->fuelInTankSpin->value();
    report.isTollCollectProper = ipcs(ui->tollCollectProper);
    report.tollCollectProperComment = ui->tollCollectProperEdit->text();
    report.lightsCondition = ipcs(ui->lightsCondition);
    report.briefcaseDocsPresent = ipcs(ui->briefCaseDocsPresent);
    report.carRegCertPresent = ipcs(ui->carRegCertPresent);
    report.semiTrailerRegCertPresent = ipcs(ui->trailerRegCertPresent);
    report.carInsurancePresent = ipcs(ui->carInsurancePresent);
    report.semiTrailerInsurancePresent = ipcs(ui->trailerInsurancePresent);
    report.licensePresent = ipcs(ui->licensePresent);
    report.licenseNumber = ui->licenseNumberEdit->text();
    report.dkvCardNumber = ui->dkvCardNumber->text();
    report.hoyerCardNumber = ui->hoyerCardNumber->text();
    report.overallCabinState = ipcs(ui->overallCabinState);
    report.fridgeClean = ipcs(ui->fridgeClean);
    report.cabinDamages = ipcs(ui->cabinDamages);
    report.cabinDamagesComment = ui->cabinDamagesEdit->text();
    report.tiresCondition = ipcs(ui->tireCondition);
    report.tiresConditionComment = ui->tireConditionEdit->text();
    report.adrPlateCondition = ipcs(ui->adrPlateCondition);
    report.vehicleClean = ipcs(ui->vehicleClean);
    report.visibleOuterDamages = ipcs(ui->visibleOuterDamages);
    report.ebTriangles = ipcs(ui->ebTriangles);
    report.ebGloves = ipcs(ui->ebGloves);
    report.ebColorfulAdrInstruction = ipcs(ui->adrColorfulInstruction);
    report.ebReflectiveVest = ipcs(ui->ebReflectiveVest);
    report.ebProtectiveGoggles = ipcs(ui->ebProtectiveGoogles);
    report.ebFunctionalFlashlight = ipcs(ui->ebFunctionalFlashlight);
    report.ebBrushAndScoop = ipcs(ui->ebBrushAndScoop);
    report.ebWasteBox = ipcs(ui->ebWasteBox);
    report.ebRubberShoes = ipcs(ui->ebRubberShoes);
    report.ebWheelWedge = ipcs(ui->ebWheelWedge);
    report.ebManholeMat = ipcs(ui->ebManholeMat);
    report.fireExtinguisher6kg = ipcs(ui->fireExting6kg);
    report.expirationDateExting6kg1 = ui->fireExting6kg1Date->date();
    report.expirationDateExting6kg2 = ui->fireExting6kg2Date->date();
    report.fireExtinguisher2kg = ipcs(ui->fireExting2kg);
    report.expirationDateExting2kg = ui->fireExting2kgDate->date();
    report.medkit = ipcs(ui->medkit);
    report.expirationDateMedkit = ui->medkitDate->date();
    report.maskAndFilters = ipcs(ui->maskAndFilters);
    report.eyewash = ipcs(ui->eyewash);
    report.expirationDateMaskAndFilters = ui->maskAndFiltersDate->date();
    report.expirationDateEyewash = ui->eyewashDate->date();
    report.comments = ui->truckComments->text();

    // semi trailer related (probably)

    report.trailerAdrCondition = ipcs(ui->trailerAdrCondition);
    report.trailerClean = ipcs(ui->trailerClean);
    report.trailerWheelWedge = ipcs(ui->trailerWheelWedge);
    report.trailerTireCondition = ipcs(ui->trailerTireCondition);
    report.trailerLightsCondition = ipcs(ui->trailerLightCondition);
    report.trailerLightsDamageComment = ui->trailerLightDamagesEdit->text();
    report.trailerVisibleDamages = ipcs(ui->trailerVisibleDamages);
    report.trailerWireCondition = ipcs(ui->trailerWireCondition);
    report.trailerWireComments = ui->trailerWireConditionEdit->text();
    report.trailerBarNumber = ui->trailerBarSpin->value();
    report.trailerBeltNumber = ui->trailerBeltSpin->value();
    report.trailerPalletTruck = ipcs(ui->trailerPalletTruck);
    report.trailerExpansionPole = ui->trailerExpansionPoleSpin->value();
    report.trailerCornerNumber = ui->trailerCornerSpin->value();
    report.trailerAntislipMap = ui->trailerAntislipMapSpin->value();
    report.trailerLoadedElevator = ipcs(ui->trailerLoadedElevator);
    report.trailerUdtElevatorUpToDate = ipcs(ui->trailerUdtElevatorUpdToDate);
    report.trailerLngCertificate = ipcs(ui->trailerLngCertificate);
    report.eni = ipcs(ui->eni);
    report.eniNumber = ui->eniNumberEdit->text();
    report.shell = ipcs(ui->shell);
    report.shellNumber = ui->shellNumberEdit->text();
    report.barmalgas = ipcs(ui->barmalgas);
    report.barmalgasNumber = ui->barmalgasNumberEdit->text();
    report.liqvis = ipcs(ui->liqvis);
    report.liqvisNumber = ui->liqvisNumberEdit->text();
    report.liquind = ipcs(ui->liquind);
    report.liquindNumber = ui->liquindNumberEdit->text();
    report.e100 = ipcs(ui->e100);
    report.e100Number = ui->e100NumberEdit->text();
    report.aral = ipcs(ui->aral);
    report.aralNumber = ui->aralNumberEdit->text();
    report.setConnectedToTrailer = ipcs(ui->setConnectedToTrailer);
    report.missingBoxContent = ipcs(ui->missingBoxContent);
    report.missinBoxContentComment = ui->missingBoxContentEdit->text();
    report.missingDocsInCabin = ipcs(ui->missingDocsInCabin);
    report.missingDocsInCabinComment = ui->missingDocsInCabinEdit->text();

    bool success = dbHandler.uploadProtocol(currentUser, report);
    if(!success) {
        ui->formErrorLabel->setText("An error occurred while submitting the form");
        return;
    }

    currentUser->fetchHeadlines();

    emit formSubmitted();
}


void ProtocolForm::on_formSendConfirmCheck_stateChanged(int arg1)
{
    if(arg1) {
        InputManager::enableButton(ui->sendForm, "QPushButton {background-color:  rgb(249, 115, 22);height: 30px;border-radius: 10px;color: white;padding: 4px 0;}");
    }
    else {
        InputManager::disableButton(ui->sendForm);
    }
}

void ProtocolForm::setFormEditState(bool checked, QLineEdit *edit)
{
    if(!edit) return;

    if(checked) {
        edit->setEnabled(true);
    }
    else {
        edit->setDisabled(true);
    }
}

void ProtocolForm::setFormDateEditState(bool checked, QDateEdit *edit)
{
    if(!edit) return;

    if(checked) {
        edit->setEnabled(true);
    }
    else {
        edit->setDisabled(true);
    }
}

// is toll collect proper
void ProtocolForm::on_radioButton_26_toggled(bool checked)
{
    setFormEditState(checked, ui->tollCollectProperEdit);
}

// is there a license in the cabin
void ProtocolForm::on_radioButton_23_toggled(bool checked)
{
    setFormEditState(checked, ui->licenseNumberEdit);
}

// cabin damages
void ProtocolForm::on_radioButton_29_toggled(bool checked)
{
    setFormEditState(checked, ui->cabinDamagesEdit);
}

// tire condition
void ProtocolForm::on_radioButton_4_toggled(bool checked)
{
    setFormEditState(checked, ui->tireConditionEdit);
}

// fire extinguisher 6kg
void ProtocolForm::on_radioButton_63_toggled(bool checked)
{
    setFormDateEditState(checked, ui->fireExting6kg1Date);
    setFormDateEditState(checked, ui->fireExting6kg2Date);
}

// fire extinguisher 2kg
void ProtocolForm::on_radioButton_67_toggled(bool checked)
{
    setFormDateEditState(checked, ui->fireExting2kgDate);
}

// medkit
void ProtocolForm::on_radioButton_69_toggled(bool checked)
{
    setFormDateEditState(checked, ui->medkitDate);
}

// mask and filters
void ProtocolForm::on_radioButton_71_toggled(bool checked)
{
    setFormDateEditState(checked, ui->maskAndFiltersDate);
}

// eyewash
void ProtocolForm::on_radioButton_73_toggled(bool checked)
{
    setFormDateEditState(checked, ui->eyewashDate);
}

// trailer light condition
void ProtocolForm::on_radioButton_84_toggled(bool checked)
{
    setFormEditState(checked, ui->trailerLightDamagesEdit);
}

// trailer wire condition
void ProtocolForm::on_radioButton_88_toggled(bool checked)
{
    setFormEditState(checked, ui->trailerWireConditionEdit);
}

// eni
void ProtocolForm::on_radioButton_97_toggled(bool checked)
{
    setFormEditState(checked, ui->eniNumberEdit);
}

// shell
void ProtocolForm::on_radioButton_99_toggled(bool checked)
{
    setFormEditState(checked, ui->shellNumberEdit);
}

// barmalgas
void ProtocolForm::on_radioButton_101_toggled(bool checked)
{
    setFormEditState(checked, ui->barmalgasNumberEdit);
}

// liqvis
void ProtocolForm::on_radioButton_103_toggled(bool checked)
{
    setFormEditState(checked, ui->liqvisNumberEdit);
}

// liquind
void ProtocolForm::on_radioButton_105_toggled(bool checked)
{
    setFormEditState(checked, ui->liquindNumberEdit);
}

// e100
void ProtocolForm::on_radioButton_107_toggled(bool checked)
{
    setFormEditState(checked, ui->e100NumberEdit);
}

// aral
void ProtocolForm::on_radioButton_109_toggled(bool checked)
{
    setFormEditState(checked, ui->aralNumberEdit);
}

// missing content in the box
void ProtocolForm::on_radioButton_113_toggled(bool checked)
{
    setFormEditState(checked, ui->missingBoxContentEdit);
}

// missing docs in cabin
void ProtocolForm::on_radioButton_115_toggled(bool checked)
{
    setFormEditState(checked, ui->missingDocsInCabinEdit);
}
