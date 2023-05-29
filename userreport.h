#ifndef USERREPORT_H
#define USERREPORT_H

#include <QString>
#include <QDate>
#include <QTime>
#include <QImage>

class UserReport
{
public:
    UserReport();

    void operator<<(const UserReport& report)
    {
        qDebug() << "UserReport:" << Qt::endl;
        qDebug() << "OwnerId:" << report.ownerId << Qt::endl;
        qDebug() << "Email:" << report.email << Qt::endl;
        qDebug() << "PhoneNumber:" << report.phoneNumber << Qt::endl;
        qDebug() << "DriverName:" << report.driverName << Qt::endl;
        qDebug() << "CarName:" << report.carName << Qt::endl;
        qDebug() << "SemiTrailer:" << report.semiTrailer << Qt::endl;
        qDebug() << "ProjectName:" << report.projectName << Qt::endl;
        qDebug() << "OdometerReading:" << report.odometerReading << Qt::endl;
        qDebug() << "FuelInTank:" << report.fuelInTank << Qt::endl;
        qDebug() << "IsTollCollectProper:" << report.isTollCollectProper << Qt::endl;
        qDebug() << "TollCollectProperComment:" << report.tollCollectProperComment << Qt::endl;
        qDebug() << "DamageImage:" << report.damageImage << Qt::endl;
        qDebug() << "BriefcaseDocsPresent:" << report.briefcaseDocsPresent << Qt::endl;
        qDebug() << "CarRegCertPresent:" << report.carRegCertPresent << Qt::endl;
        qDebug() << "SemiTrailerRegCertPresent:" << report.semiTrailerRegCertPresent << Qt::endl;
        qDebug() << "CarInsurancePresent:" << report.carInsurancePresent << Qt::endl;
        qDebug() << "SemiTrailerInsurancePresent:" << report.semiTrailerInsurancePresent << Qt::endl;
        qDebug() << "LicensePresent:" << report.licensePresent << Qt::endl;
        qDebug() << "LicenseNumber:" << report.licenseNumber << Qt::endl;
        qDebug() << "DkvCardNumber:" << report.dkvCardNumber << Qt::endl;
        qDebug() << "HoyerCardNumber:" << report.hoyerCardNumber << Qt::endl;
        qDebug() << "OverallCabinState:" << report.overallCabinState << Qt::endl;
        qDebug() << "FridgeClean:" << report.fridgeClean << Qt::endl;
        qDebug() << "CabinDamages:" << report.cabinDamages << Qt::endl;
        qDebug() << "CabinDamagesComment:" << report.cabinDamagesComment << Qt::endl;
        qDebug() << "TiresCondition:" << report.tiresCondition << Qt::endl;
        qDebug() << "TiresConditionComment:" << report.tiresConditionComment << Qt::endl;
        qDebug() << "BrokenTires:" << report.brokenTires << Qt::endl;
        qDebug() << "LightsCondition:" << report.lightsCondition << Qt::endl;
        qDebug() << "BrokenLights:" << report.brokenLights << Qt::endl;
        qDebug() << "AdrPlateCondition:" << report.adrPlateCondition << Qt::endl;
        qDebug() << "VehicleClean:" << report.vehicleClean << Qt::endl;
        qDebug() << "VisibleOuterDamages:" << report.visibleOuterDamages << Qt::endl;
        qDebug() << "OuterDamages:" << report.outerDamages << Qt::endl;
        qDebug() << "EbTriangles:" << report.ebTriangles << Qt::endl;
        qDebug() << "EbGloves:" << report.ebGloves << Qt::endl;
        qDebug() << "EbColorfulAdrInstruction:" << report.ebColorfulAdrInstruction << Qt::endl;
        qDebug() << "EbReflectiveVest:" << report.ebReflectiveVest << Qt::endl;
        qDebug() << "EbProtectiveGoggles:" << report.ebProtectiveGoggles << Qt::endl;
        qDebug() << "EbFunctionalFlashlight:" << report.ebFunctionalFlashlight << Qt::endl;
        qDebug() << "EbBrushAndScoop:" << report.ebBrushAndScoop << Qt::endl;
        qDebug() << "EbWasteBox:" << report.ebWasteBox << Qt::endl;
        qDebug() << "EbRubberShoes:" << report.ebRubberShoes << Qt::endl;
        qDebug() << "EbWheelWedge:" << report.ebWheelWedge << Qt::endl;
        qDebug() << "EbManholeMat:" << report.ebManholeMat << Qt::endl;
        qDebug() << "FireExtinguisher6kg:" << report.fireExtinguisher6kg << Qt::endl;
        qDebug() << "ExpirationDateExting6kg1:" << report.expirationDateExting6kg1 << Qt::endl;
        qDebug() << "ExpirationDateExting6kg2:" << report.expirationDateExting6kg2 << Qt::endl;
        qDebug() << "FireExtinguisher2kg:" << report.fireExtinguisher2kg << Qt::endl;
        qDebug() << "ExpirationDateExting2kg:" << report.expirationDateExting2kg << Qt::endl;
        qDebug() << "Medkit:" << report.medkit << Qt::endl;
        qDebug() << "ExpirationDateMedkit:" << report.expirationDateMedkit << Qt::endl;
        qDebug() << "MaskAndFilters:" << report.maskAndFilters << Qt::endl;
        qDebug() << "Eyewash:" << report.eyewash << Qt::endl;
        qDebug() << "ExpirationDateMaskAndFilters:" << report.expirationDateMaskAndFilters << Qt::endl;
        qDebug() << "ExpirationDateEyewash:" << report.expirationDateEyewash << Qt::endl;
        qDebug() << "Comments:" << report.comments << Qt::endl;
        qDebug() << "Photos:" << report.photos << Qt::endl;
        qDebug() << "OtherComments:" << report.otherComments << Qt::endl;
        qDebug() << "TrailerClean:" << report.trailerClean << Qt::endl;
        qDebug() << "TrailerAdrCondition:" << report.trailerAdrCondition << Qt::endl;
        qDebug() << "TrailerWheelWedge:" << report.trailerWheelWedge << Qt::endl;
        qDebug() << "TrailerTireCondition:" << report.trailerTireCondition << Qt::endl;
        qDebug() << "TrailerTiresPhotos:" << report.trailerTiresPhotos << Qt::endl;
        qDebug() << "TrailerLightsCondition:" << report.trailerLightsCondition << Qt::endl;
        qDebug() << "TrailerLightsDamageComment:" << report.trailerLightsDamageComment << Qt::endl;
        qDebug() << "TrailerLightsDamagePhotos:" << report.trailerLightsDamagePhotos << Qt::endl;
        qDebug() << "TrailerVisibleDamages:" << report.trailerVisibleDamages << Qt::endl;
        qDebug() << "TrailerVisibleDamagesPhotos:" << report.trailerVisibleDamagesPhotos << Qt::endl;
        qDebug() << "TrailerWireCondition:" << report.trailerWireCondition << Qt::endl;
        qDebug() << "TrailerWireComments:" << report.trailerWireComments << Qt::endl;
        qDebug() << "TrailerBarNumber:" << report.trailerBarNumber << Qt::endl;
        qDebug() << "TrailerBeltNumber:" << report.trailerBeltNumber << Qt::endl;
        qDebug() << "TrailerPalletTruck:" << report.trailerPalletTruck << Qt::endl;
        qDebug() << "TrailerExpansionPole:" << report.trailerExpansionPole << Qt::endl;
        qDebug() << "TrailerCornerNumber:" << report.trailerCornerNumber << Qt::endl;
        qDebug() << "TrailerAntislipMap:" << report.trailerAntislipMap << Qt::endl;
        qDebug() << "TrailerLoadedElevator:" << report.trailerLoadedElevator << Qt::endl;
        qDebug() << "TrailerUdtElevatorUpToDate:" << report.trailerUdtElevatorUpToDate << Qt::endl;
        qDebug() << "TrailerLngCertificate:" << report.trailerLngCertificate << Qt::endl;
        qDebug() << "Eni:" << report.eni << Qt::endl;
        qDebug() << "EniNumber:" << report.eniNumber << Qt::endl;
        qDebug() << "Shell:" << report.shell << Qt::endl;
        qDebug() << "ShellNumber:" << report.shellNumber << Qt::endl;
        qDebug() << "Barmalgas:" << report.barmalgas << Qt::endl;
        qDebug() << "BarmalgasNumber:" << report.barmalgasNumber << Qt::endl;
        qDebug() << "Liqvis:" << report.liqvis << Qt::endl;
        qDebug() << "LiqvisNumber:" << report.liqvisNumber << Qt::endl;
        qDebug() << "Liquind:" << report.liquind << Qt::endl;
        qDebug() << "LiquindNumber:" << report.liquindNumber << Qt::endl;
        qDebug() << "E100:" << report.e100 << Qt::endl;
        qDebug() << "E100Number:" << report.e100Number << Qt::endl;
        qDebug() << "Aral:" << report.aral << Qt::endl;
        qDebug() << "AralNumber:" << report.aralNumber << Qt::endl;
        qDebug() << "SetConnectedToTrailer:" << report.setConnectedToTrailer << Qt::endl;
        qDebug() << "MissingBoxContent:" << report.missingBoxContent << Qt::endl;
        qDebug() << "MissinBoxContentComment:" << report.missinBoxContentComment << Qt::endl;
        qDebug() << "MissingDocsInCabin:" << report.missingDocsInCabin << Qt::endl;
        qDebug() << "MissingDocsInCabinComment:" << report.missingDocsInCabinComment << Qt::endl;
        qDebug() << "UploadDate:" << report.uploadDate << Qt::endl;
        qDebug() << "UploadTime:" << report.uploadTime << Qt::endl;
    }

    // cabin and truck related
    QString ownerId;
    QString email;
    QString phoneNumber;
    QString driverName;
    QString carName;
    QString semiTrailer; // naczepa
    QString projectName;
    int odometerReading; // km count
    int fuelInTank;
    bool isTollCollectProper; // if not then qlineedit for comment
    QString tollCollectProperComment;
    QImage damageImage;
    bool briefcaseDocsPresent;
    bool carRegCertPresent;
    bool semiTrailerRegCertPresent;
    bool carInsurancePresent;
    bool semiTrailerInsurancePresent;
    bool licensePresent;
    QString licenseNumber;
    QString dkvCardNumber;
    QString hoyerCardNumber;
    bool overallCabinState; // 1 - ok, 0 - not ok
    bool fridgeClean; // 1 - yes, 0 - no
    bool cabinDamages; // 1 - qlineedit for the comment, 0 - no
    QString cabinDamagesComment;
    bool tiresCondition; // 1 - good, 0 - qlineedit for the comment
    QString tiresConditionComment;
    QVector<QImage> brokenTires;
    bool lightsCondition;
    QVector<QImage> brokenLights;
    bool adrPlateCondition;
    bool vehicleClean;
    bool visibleOuterDamages;
    QVector<QImage> outerDamages;
    bool ebTriangles;
    bool ebGloves;
    bool ebColorfulAdrInstruction;
    bool ebReflectiveVest;
    bool ebProtectiveGoggles;
    bool ebFunctionalFlashlight;
    bool ebBrushAndScoop;
    bool ebWasteBox;
    bool ebRubberShoes;
    bool ebWheelWedge; // klin pod kola
    bool ebManholeMat; // mata na studzienke
    bool fireExtinguisher6kg;
    QDate expirationDateExting6kg1;
    QDate expirationDateExting6kg2;
    bool fireExtinguisher2kg;
    QDate expirationDateExting2kg;
    bool medkit;
    QDate expirationDateMedkit;
    bool maskAndFilters; // maska + filtry
    bool eyewash;
    QDate expirationDateMaskAndFilters;
    QDate expirationDateEyewash;
    QString comments; // qlineedit
    QVector<QImage> photos;
    QString otherComments;


    // semi trailer related
    bool trailerClean;
    bool trailerAdrCondition;
    bool trailerWheelWedge;
    bool trailerTireCondition;
    QVector<QImage> trailerTiresPhotos;
    bool trailerLightsCondition;
    QString trailerLightsDamageComment;
    QVector<QImage> trailerLightsDamagePhotos;
    bool trailerVisibleDamages;
    QVector<QImage> trailerVisibleDamagesPhotos;
    bool trailerWireCondition;
    QString trailerWireComments;
    int trailerBarNumber;
    int trailerBeltNumber;
    bool trailerPalletTruck;
    int trailerExpansionPole; // tyczka rozporowa
    int trailerCornerNumber;
    int trailerAntislipMap;
    bool trailerLoadedElevator;
    bool trailerUdtElevatorUpToDate;
    bool trailerLngCertificate;

    // different cards
    bool eni;
    QString eniNumber;
    bool shell;
    QString shellNumber;
    bool barmalgas;
    QString barmalgasNumber;
    bool liqvis;
    QString liqvisNumber;
    bool liquind;
    QString liquindNumber;
    bool e100;
    QString e100Number;
    bool aral;
    QString aralNumber;

    bool setConnectedToTrailer;
    bool missingBoxContent;
    QString missinBoxContentComment;
    bool missingDocsInCabin;
    QString missingDocsInCabinComment;



    QDate uploadDate;
    QTime uploadTime;
};

#endif // USERREPORT_H
