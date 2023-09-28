#include "protocolform.h"
#include <QDate>
#include <QGraphicsDropShadowEffect>
#include <QGroupBox>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPropertyAnimation>
#include <QScrollBar>
#include <QScroller>
#include <QScrollerProperties>
#include <QTime>
#include <QTimer>
#include "fineloguser.h"
#include "inputmanager.h"
#include "ui_protocolform.h"
#include "userreport.h"
#define ipcs isPositiveChoiseSelected
#define mprb markProperRadioButton

ProtocolForm::ProtocolForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProtocolForm)
{
    ui->setupUi(this);

    // protocol fields scroll area

    ui->protocol_fields->setWidgetResizable(true);
    ui->protocol_fields->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->protocol_fields->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    QScroller::grabGesture(ui->protocol_fields->viewport(), QScroller::TouchGesture); // Enable touch scrolling

    // Configure the scrolling behavior
    QScrollerProperties scrollerProperties = QScroller::scroller(ui->protocol_fields->viewport())
                                                 ->scrollerProperties();
    scrollerProperties.setScrollMetric(QScrollerProperties::DragVelocitySmoothingFactor, 0.6);
    scrollerProperties.setScrollMetric(QScrollerProperties::MinimumVelocity, 0.0);
    scrollerProperties.setScrollMetric(QScrollerProperties::MaximumVelocity, 0.6);
    scrollerProperties.setScrollMetric(QScrollerProperties::AcceleratingFlickMaximumTime, 0.4);
    scrollerProperties.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy,
                                       QScrollerProperties::OvershootAlwaysOff);
    QScroller::scroller(ui->protocol_fields->viewport())->setScrollerProperties(scrollerProperties);

    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect;
    shadowEffect->setBlurRadius(20);
    shadowEffect->setColor(QColor(0, 0, 0, 80));
    shadowEffect->setOffset(0, 0);
    ui->protocol_fields->setGraphicsEffect(shadowEffect);
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

    QJsonObject inputsData = dbHandler.getFormInputData(currentUser->getIdToken());

    // need to somehow modify the json array which is stored in the inputsData QJSonObject
    // passing by reference does not work dunno
    foreach (const QString &key, inputsData.keys()) {
        if (key == "Kierowca") {
            for (int i = 0; i < inputsData.value(key).toArray().count(); i++) {
                ui->driverCombo->addItem(inputsData.value(key).toArray()[i].toString());
            }
        }
        if (key == "Auto") {
            for (int i = 0; i < inputsData.value(key).toArray().count(); i++) {
                ui->carCombo->addItem(inputsData.value(key).toArray()[i].toString());
            }
        }
        if (key == "Naczepa") {
            for (int i = 0; i < inputsData.value(key).toArray().count(); i++) {
                ui->trailerCombo->addItem(inputsData.value(key).toArray()[i].toString());
            }
        }
        if (key == "Projekt") {
            for (int i = 0; i < inputsData.value(key).toArray().count(); i++) {
                ui->projectCombo->addItem(inputsData.value(key).toArray()[i].toString());
            }
        }
    }

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

// disable all fields
void ProtocolForm::prepareFormToInspect()
{
    for(QObject* o : ui->protocol_fields->widget()->children()) {
        QWidget* w = dynamic_cast<QWidget*>(o);
        if(w) {
            w->setDisabled(true);
        }
    }
}

void ProtocolForm::hideSendOptions()
{
    ui->sendForm->setDisabled(true);
    ui->sendForm->hide();

    ui->formSendConfirmCheck->setDisabled(true);
    ui->formSendConfirmCheck->hide();

    ui->formErrorLabel->setText("");
}

bool ProtocolForm::initializeFormData(const QString &contentName, QLabel* projectTitle)
{
    //qDebug() << "content name received: " << contentName;
    QString path = "Reports/Content/" + contentName;
    QJsonObject formData = dbHandler.performAuthenticatedGET(path, currentUser->getIdToken());

    // proceed with setting up the form data from db into the ui

    // #define mprb markProperRadioButton

    projectTitle->setText(formData.value("projectName").toString());

    ui->emailEdit->setText(formData.value("owner_email").toString());
    ui->phoneEdit->setText(formData.value("owner_phone").toString());

    ui->driverCombo->addItem(formData.value("driver").toString());
    ui->carCombo->addItem(formData.value("carName").toString());
    ui->trailerCombo->addItem(formData.value("semiTrailer").toString());
    ui->projectCombo->addItem(formData.value("projectName").toString());

    ui->driverCombo->setCurrentText(formData.value("driverName").toString());
    ui->carCombo->setCurrentText(formData.value("carName").toString());
    ui->trailerCombo->setCurrentText(formData.value("semiTrailer").toString());
    ui->projectCombo->setCurrentText(formData.value("projectName").toString());

    ui->odometerSpin->setValue(formData.value("odometerReading").toDouble());
    ui->fuelInTankSpin->setValue(formData.value("fuelInTank").toDouble());
    mprb(ui->tollCollectProper,formData.value("isTollCollectProper").toBool());
    ui->tollCollectProperEdit->setText(formData.value("tollCollectProperComment").toString());
    mprb(ui->lightsCondition, formData.value("lightsCondition").toBool());
    mprb(ui->briefCaseDocsPresent, formData.value("briefcaseDocsPresent").toBool());
    mprb(ui->carRegCertPresent, formData.value("carRegCertPresent").toBool());
    mprb(ui->trailerRegCertPresent, formData.value("semiTrailerRegCertPresent").toBool());
    mprb(ui->carInsurancePresent, formData.value("carInsurancePresent").toBool());
    mprb(ui->trailerInsurancePresent, formData.value("semiTrailerInsurancePresent").toBool());
    mprb(ui->licensePresent, formData.value("licensePresent").toBool());
    ui->licenseNumberEdit->setText(formData.value("licenseNumber").toString());
    ui->dkvCardNumber->setText(formData.value("dkvCardNumber").toString());
    ui->hoyerCardNumber->setText(formData.value("hoyerCardNumber").toString());
    mprb(ui->overallCabinState, formData.value("overallCabinState").toBool());
    mprb(ui->fridgeClean, formData.value("fridgeClean").toBool());
    mprb(ui->cabinDamages, formData.value("cabinDamages").toBool());
    ui->cabinDamagesEdit->setText(formData.value("cabinDamagesComment").toString());
    mprb(ui->tireCondition, formData.value("tiresCondition").toBool());
    ui->tireConditionEdit->setText(formData.value("tiresConditionComment").toString());
    mprb(ui->adrPlateCondition, formData.value("adrPlateCondition").toBool());
    mprb(ui->vehicleClean, formData.value("vehicleClean").toBool());
    mprb(ui->visibleOuterDamages, formData.value("visibleOuterDamages").toBool());
    mprb(ui->ebTriangles, formData.value("ebTriangles").toBool());
    mprb(ui->ebGloves, formData.value("ebGloves").toBool());
    mprb(ui->adrColorfulInstruction, formData.value("ebColorfulAdrInstruction").toBool());
    mprb(ui->ebReflectiveVest, formData.value("ebReflectiveVest").toBool());
    mprb(ui->ebProtectiveGoogles, formData.value("ebProtectiveGoggles").toBool());
    mprb(ui->ebFunctionalFlashlight, formData.value("ebFunctionalFlashlight").toBool());
    mprb(ui->ebBrushAndScoop, formData.value("ebBrushAndScoop").toBool());
    mprb(ui->ebWasteBox, formData.value("ebWasteBox").toBool());
    mprb(ui->ebRubberShoes, formData.value("ebRubberShoes").toBool());
    mprb(ui->ebWheelWedge, formData.value("ebWheelWedge").toBool());
    mprb(ui->ebManholeMat, formData.value("ebManholeMat").toBool());
    mprb(ui->fireExting6kg, formData.value("fireExtinguisher6kg").toBool());
    ui->fireExting6kg1Date->setDate(QDate::fromString(formData.value("expirationDateExting6kg1").toString(), "yyyy-MM-dd"));
    ui->fireExting6kg2Date->setDate(QDate::fromString(formData.value("expirationDateExting6kg2").toString(), "yyyy-MM-dd"));
    mprb(ui->fireExting2kg, formData.value("fireExtinguisher2kg").toBool());
    ui->fireExting2kgDate->setDate(QDate::fromString(formData.value("expirationDateExting2kg").toString(), "yyyy-MM-dd"));
    mprb(ui->medkit, formData.value("medkit").toBool());
    ui->medkitDate->setDate(QDate::fromString(formData.value("expirationDateMedkit").toString(), "yyyy-MM-dd"));
    mprb(ui->maskAndFilters, formData.value("maskAndFilters").toBool());
    mprb(ui->eyewash, formData.value("eyewash").toBool());
    ui->maskAndFiltersDate->setDate(QDate::fromString(formData.value("expirationDateMaskAndFilters").toString(), "yyyy-MM-dd"));
    ui->eyewashDate->setDate(QDate::fromString(formData.value("expirationDateEyewash").toString(), "yyyy-MM-dd"));
    ui->truckComments->setText(formData.value("comments").toString());

    // semi trailer related (probably)

    mprb(ui->trailerAdrCondition, formData.value("trailerAdrCondition").toBool());
    mprb(ui->trailerClean, formData.value("trailerClean").toBool());
    mprb(ui->trailerWheelWedge, formData.value("trailerWheelWedge").toBool());
    mprb(ui->trailerTireCondition, formData.value("trailerTireCondition").toBool());
    mprb(ui->trailerLightCondition, formData.value("trailerLightsCondition").toBool());
    ui->trailerLightDamagesEdit->setText(formData.value("trailerLightsDamageComment").toString());
    mprb(ui->trailerVisibleDamages, formData.value("trailerVisibleDamages").toBool());
    mprb(ui->trailerWireCondition, formData.value("trailerWireCondition").toBool());
    ui->trailerWireConditionEdit->setText(formData.value("trailerWireComments").toString());
    ui->trailerBarSpin->setValue(formData.value("trailerBarNumber").toDouble());
    ui->trailerBeltSpin->setValue(formData.value("trailerBeltNumber").toDouble());
    mprb(ui->trailerPalletTruck, formData.value("trailerPalletTruck").toBool());
    ui->trailerExpansionPoleSpin->setValue(formData.value("trailerExpansionPole").toDouble());
    ui->trailerCornerSpin->setValue(formData.value("trailerCornerNumber").toDouble());
    ui->trailerAntislipMapSpin->setValue(formData.value("trailerAntislipMap").toDouble());
    mprb(ui->trailerLoadedElevator, formData.value("trailerLoadedElevator").toBool());
    mprb(ui->trailerUdtElevatorUpdToDate, formData.value("trailerUdtElevatorUpToDate").toBool());
    mprb(ui->trailerLngCertificate, formData.value("trailerLngCertificate").toBool());
    mprb(ui->eni, formData.value("eni").toBool());
    ui->eniNumberEdit->setText(formData.value("eniNumber").toString());
    mprb(ui->shell, formData.value("shell").toBool());
    ui->shellNumberEdit->setText(formData.value("shellNumber").toString());
    mprb(ui->barmalgas, formData.value("barmalgas").toBool());
    ui->barmalgasNumberEdit->setText(formData.value("barmalgasNumber").toString());
    mprb(ui->liqvis, formData.value("liqvis").toBool());
    ui->liqvisNumberEdit->setText(formData.value("liqvisNumber").toString());
    mprb(ui->liquind, formData.value("liquind").toBool());
    ui->liquindNumberEdit->setText(formData.value("liquindNumber").toString());
    mprb(ui->e100, formData.value("e100").toBool());
    ui->e100NumberEdit->setText(formData.value("e100Number").toString());
    mprb(ui->aral, formData.value("aral").toBool());
    ui->aralNumberEdit->setText(formData.value("aralNumber").toString());
    mprb(ui->setConnectedToTrailer, formData.value("setConnectedToTrailer").toBool());
    mprb(ui->missingBoxContent, formData.value("missingBoxContent").toBool());
    ui->missingBoxContentEdit->setText(formData.value("missinBoxContentComment").toString());
    mprb(ui->missingDocsInCabin, formData.value("missingDocsInCabin").toBool());
    ui->missingDocsInCabinEdit->setText(formData.value("missingDocsInCabinComment").toString());




    return true;
}

void ProtocolForm::markProperRadioButton(QGroupBox* box, bool val)
{
    if(!box) return;

    QRadioButton* radio1 = nullptr;
    QRadioButton* radio2 = nullptr;
    for(QRadioButton* r : box->findChildren<QRadioButton*>()) {
        if(!radio1) radio1 = r;
        else radio2 = r;
    }

    QRadioButton* positiveOption = nullptr;
    QRadioButton* negativeOption = nullptr;

    if(!radio1 || !radio2) return;

    if(radio1->text() == "Ok" || radio1->text() == "Tak") {
        positiveOption = radio1;
        negativeOption = radio2;
    }
    else {
        positiveOption = radio2;
        negativeOption = radio1;
    }

    if(val) positiveOption->setChecked(true);
    else negativeOption->setChecked(true);
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

    //    if(!allFilled) {
    //        ui->formErrorLabel->setText("Nie wszystkie pola zostały wypełnione");
    //        return;
    //    }
    //    ui->formErrorLabel->setText("");

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
