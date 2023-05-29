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

    friend QDebug operator<<(QDebug debug, const UserReport& report)
    {
        debug << "UserReport:" << Qt::endl;
        debug << "OwnerId:" << report.ownerId << Qt::endl;
        debug << "Email:" << report.email << Qt::endl;
        debug << "PhoneNumber:" << report.phoneNumber << Qt::endl;
        debug << "DriverName:" << report.driverName << Qt::endl;
        debug << "CarName:" << report.carName << Qt::endl;
        debug << "SemiTrailer:" << report.semiTrailer << Qt::endl;
        debug << "ProjectName:" << report.projectName << Qt::endl;
        debug << "OdometerReading:" << report.odometerReading << Qt::endl;
        debug << "FuelInTank:" << report.fuelInTank << Qt::endl;
        debug << "IsTollCollectProper:" << report.isTollCollectProper << Qt::endl;
        debug << "TollCollectProperComment:" << report.tollCollectProperComment << Qt::endl;
        debug << "DamageImage:" << report.damageImage << Qt::endl;
        debug << "BriefcaseDocsPresent:" << report.briefcaseDocsPresent << Qt::endl;
        debug << "CarRegCertPresent:" << report.carRegCertPresent << Qt::endl;
        debug << "SemiTrailerRegCertPresent:" << report.semiTrailerRegCertPresent << Qt::endl;
        debug << "CarInsurancePresent:" << report.carInsurancePresent << Qt::endl;
        debug << "SemiTrailerInsurancePresent:" << report.semiTrailerInsurancePresent << Qt::endl;
        debug << "LicensePresent:" << report.licensePresent << Qt::endl;
        debug << "LicenseNumber:" << report.licenseNumber << Qt::endl;
        debug << "DkvCardNumber:" << report.dkvCardNumber << Qt::endl;
        debug << "HoyerCardNumber:" << report.hoyerCardNumber << Qt::endl;
        debug << "OverallCabinState:" << report.overallCabinState << Qt::endl;
        debug << "FridgeClean:" << report.fridgeClean << Qt::endl;
        debug << "CabinDamages:" << report.cabinDamages << Qt::endl;
        debug << "CabinDamagesComment:" << report.cabinDamagesComment << Qt::endl;
        debug << "TiresCondition:" << report.tiresCondition << Qt::endl;
        debug << "TiresConditionComment:" << report.tiresConditionComment << Qt::endl;
        debug << "BrokenTires:" << report.brokenTires << Qt::endl;
        debug << "LightsCondition:" << report.lightsCondition << Qt::endl;
        debug << "BrokenLights:" << report.brokenLights << Qt::endl;
        debug << "AdrPlateCondition:" << report.adrPlateCondition << Qt::endl;
        debug << "VehicleClean:" << report.vehicleClean << Qt::endl;
        debug << "VisibleOuterDamages:" << report.visibleOuterDamages << Qt::endl;
        debug << "OuterDamages:" << report.outerDamages << Qt::endl;
        debug << "EbTriangles:" << report.ebTriangles << Qt::endl;
        debug << "EbGloves:" << report.ebGloves << Qt::endl;
        debug << "EbColorfulAdrInstruction:" << report.ebColorfulAdrInstruction << Qt::endl;
        debug << "EbReflectiveVest:" << report.ebReflectiveVest << Qt::endl;
        debug << "EbProtectiveGoggles:" << report.ebProtectiveGoggles << Qt::endl;
        debug << "EbFunctionalFlashlight:" << report.ebFunctionalFlashlight << Qt::endl;
        debug << "EbBrushAndScoop:" << report.ebBrushAndScoop << Qt::endl;
        debug << "EbWasteBox:" << report.ebWasteBox << Qt::endl;
        debug << "EbRubberShoes:" << report.ebRubberShoes << Qt::endl;
        debug << "EbWheelWedge:" << report.ebWheelWedge << Qt::endl;
        debug << "EbManholeMat:" << report.ebManholeMat << Qt::endl;
        debug << "FireExtinguisher6kg:" << report.fireExtinguisher6kg << Qt::endl;
        debug << "ExpirationDateExting6kg1:" << report.expirationDateExting6kg1 << Qt::endl;
        debug << "ExpirationDateExting6kg2:" << report.expirationDateExting6kg2 << Qt::endl;
        debug << "FireExtinguisher2kg:" << report.fireExtinguisher2kg << Qt::endl;
        debug << "ExpirationDateExting2kg:" << report.expirationDateExting2kg << Qt::endl;
        debug << "Medkit:" << report.medkit << Qt::endl;
        debug << "ExpirationDateMedkit:" << report.expirationDateMedkit << Qt::endl;
        debug << "MaskAndFilters:" << report.maskAndFilters << Qt::endl;
        debug << "Eyewash:" << report.eyewash << Qt::endl;
        debug << "ExpirationDateMaskAndFilters:" << report.expirationDateMaskAndFilters << Qt::endl;
        debug << "ExpirationDateEyewash:" << report.expirationDateEyewash << Qt::endl;
        debug << "Comments:" << report.comments << Qt::endl;
        debug << "Photos:" << report.photos << Qt::endl;
        debug << "OtherComments:" << report.otherComments << Qt::endl;
        debug << "TrailerClean:" << report.trailerClean << Qt::endl;
        debug << "TrailerAdrCondition:" << report.trailerAdrCondition << Qt::endl;
        debug << "TrailerWheelWedge:" << report.trailerWheelWedge << Qt::endl;
        debug << "TrailerTireCondition:" << report.trailerTireCondition << Qt::endl;
        debug << "TrailerTiresPhotos:" << report.trailerTiresPhotos << Qt::endl;
        debug << "TrailerLightsCondition:" << report.trailerLightsCondition << Qt::endl;
        debug << "TrailerLightsDamageComment:" << report.trailerLightsDamageComment << Qt::endl;
        debug << "TrailerLightsDamagePhotos:" << report.trailerLightsDamagePhotos << Qt::endl;
        debug << "TrailerVisibleDamages:" << report.trailerVisibleDamages << Qt::endl;
        debug << "TrailerVisibleDamagesPhotos:" << report.trailerVisibleDamagesPhotos << Qt::endl;
        debug << "TrailerWireCondition:" << report.trailerWireCondition << Qt::endl;
        debug << "TrailerWireComments:" << report.trailerWireComments << Qt::endl;
        debug << "TrailerBarNumber:" << report.trailerBarNumber << Qt::endl;
        debug << "TrailerBeltNumber:" << report.trailerBeltNumber << Qt::endl;
        debug << "TrailerPalletTruck:" << report.trailerPalletTruck << Qt::endl;
        debug << "TrailerExpansionPole:" << report.trailerExpansionPole << Qt::endl;
        debug << "TrailerCornerNumber:" << report.trailerCornerNumber << Qt::endl;
        debug << "TrailerAntislipMap:" << report.trailerAntislipMap << Qt::endl;
        debug << "TrailerLoadedElevator:" << report.trailerLoadedElevator << Qt::endl;
        debug << "TrailerUdtElevatorUpToDate:" << report.trailerUdtElevatorUpToDate << Qt::endl;
        debug << "TrailerLngCertificate:" << report.trailerLngCertificate << Qt::endl;
        debug << "Eni:" << report.eni << Qt::endl;
        debug << "EniNumber:" << report.eniNumber << Qt::endl;
        debug << "Shell:" << report.shell << Qt::endl;
        debug << "ShellNumber:" << report.shellNumber << Qt::endl;
        debug << "Barmalgas:" << report.barmalgas << Qt::endl;
        debug << "BarmalgasNumber:" << report.barmalgasNumber << Qt::endl;
        debug << "Liqvis:" << report.liqvis << Qt::endl;
        debug << "LiqvisNumber:" << report.liqvisNumber << Qt::endl;
        debug << "Liquind:" << report.liquind << Qt::endl;
        debug << "LiquindNumber:" << report.liquindNumber << Qt::endl;
        debug << "E100:" << report.e100 << Qt::endl;
        debug << "E100Number:" << report.e100Number << Qt::endl;
        debug << "Aral:" << report.aral << Qt::endl;
        debug << "AralNumber:" << report.aralNumber << Qt::endl;
        debug << "SetConnectedToTrailer:" << report.setConnectedToTrailer << Qt::endl;
        debug << "MissingBoxContent:" << report.missingBoxContent << Qt::endl;
        debug << "MissinBoxContentComment:" << report.missinBoxContentComment << Qt::endl;
        debug << "MissingDocsInCabin:" << report.missingDocsInCabin << Qt::endl;
        debug << "MissingDocsInCabinComment:" << report.missingDocsInCabinComment << Qt::endl;
        debug << "UploadDate:" << report.uploadDate << Qt::endl;
        debug << "UploadTime:" << report.uploadTime << Qt::endl;

        return debug;
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
