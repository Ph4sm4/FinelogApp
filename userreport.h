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
        debug << "UserReport:" ;
        debug << "OwnerId:" << report.ownerId ;
        debug << "Email:" << report.email ;
        debug << "PhoneNumber:" << report.phoneNumber ;
        debug << "DriverName:" << report.driverName ;
        debug << "CarName:" << report.carName ;
        debug << "SemiTrailer:" << report.semiTrailer ;
        debug << "ProjectName:" << report.projectName ;
        debug << "OdometerReading:" << report.odometerReading ;
        debug << "FuelInTank:" << report.fuelInTank ;
        debug << "IsTollCollectProper:" << report.isTollCollectProper ;
        debug << "TollCollectProperComment:" << report.tollCollectProperComment ;
        debug << "DamageImage:" << report.damageImage ;
        debug << "BriefcaseDocsPresent:" << report.briefcaseDocsPresent ;
        debug << "CarRegCertPresent:" << report.carRegCertPresent ;
        debug << "SemiTrailerRegCertPresent:" << report.semiTrailerRegCertPresent ;
        debug << "CarInsurancePresent:" << report.carInsurancePresent ;
        debug << "SemiTrailerInsurancePresent:" << report.semiTrailerInsurancePresent ;
        debug << "LicensePresent:" << report.licensePresent ;
        debug << "LicenseNumber:" << report.licenseNumber ;
        debug << "DkvCardNumber:" << report.dkvCardNumber ;
        debug << "HoyerCardNumber:" << report.hoyerCardNumber ;
        debug << "OverallCabinState:" << report.overallCabinState ;
        debug << "FridgeClean:" << report.fridgeClean ;
        debug << "CabinDamages:" << report.cabinDamages ;
        debug << "CabinDamagesComment:" << report.cabinDamagesComment ;
        debug << "TiresCondition:" << report.tiresCondition ;
        debug << "TiresConditionComment:" << report.tiresConditionComment ;
        debug << "BrokenTires:" << report.brokenTires ;
        debug << "LightsCondition:" << report.lightsCondition ;
        debug << "BrokenLights:" << report.brokenLights ;
        debug << "AdrPlateCondition:" << report.adrPlateCondition ;
        debug << "VehicleClean:" << report.vehicleClean ;
        debug << "VisibleOuterDamages:" << report.visibleOuterDamages ;
        debug << "OuterDamages:" << report.outerDamages ;
        debug << "EbTriangles:" << report.ebTriangles ;
        debug << "EbGloves:" << report.ebGloves ;
        debug << "EbColorfulAdrInstruction:" << report.ebColorfulAdrInstruction ;
        debug << "EbReflectiveVest:" << report.ebReflectiveVest ;
        debug << "EbProtectiveGoggles:" << report.ebProtectiveGoggles ;
        debug << "EbFunctionalFlashlight:" << report.ebFunctionalFlashlight ;
        debug << "EbBrushAndScoop:" << report.ebBrushAndScoop ;
        debug << "EbWasteBox:" << report.ebWasteBox ;
        debug << "EbRubberShoes:" << report.ebRubberShoes ;
        debug << "EbWheelWedge:" << report.ebWheelWedge ;
        debug << "EbManholeMat:" << report.ebManholeMat ;
        debug << "FireExtinguisher6kg:" << report.fireExtinguisher6kg ;
        debug << "ExpirationDateExting6kg1:" << report.expirationDateExting6kg1 ;
        debug << "ExpirationDateExting6kg2:" << report.expirationDateExting6kg2 ;
        debug << "FireExtinguisher2kg:" << report.fireExtinguisher2kg ;
        debug << "ExpirationDateExting2kg:" << report.expirationDateExting2kg ;
        debug << "Medkit:" << report.medkit ;
        debug << "ExpirationDateMedkit:" << report.expirationDateMedkit ;
        debug << "MaskAndFilters:" << report.maskAndFilters ;
        debug << "Eyewash:" << report.eyewash ;
        debug << "ExpirationDateMaskAndFilters:" << report.expirationDateMaskAndFilters ;
        debug << "ExpirationDateEyewash:" << report.expirationDateEyewash ;
        debug << "Comments:" << report.comments ;
        debug << "Photos:" << report.photos ;
        debug << "OtherComments:" << report.otherComments ;
        debug << "TrailerClean:" << report.trailerClean ;
        debug << "TrailerAdrCondition:" << report.trailerAdrCondition ;
        debug << "TrailerWheelWedge:" << report.trailerWheelWedge ;
        debug << "TrailerTireCondition:" << report.trailerTireCondition ;
        debug << "TrailerTiresPhotos:" << report.trailerTiresPhotos ;
        debug << "TrailerLightsCondition:" << report.trailerLightsCondition ;
        debug << "TrailerLightsDamageComment:" << report.trailerLightsDamageComment ;
        debug << "TrailerLightsDamagePhotos:" << report.trailerLightsDamagePhotos ;
        debug << "TrailerVisibleDamages:" << report.trailerVisibleDamages ;
        debug << "TrailerVisibleDamagesPhotos:" << report.trailerVisibleDamagesPhotos ;
        debug << "TrailerWireCondition:" << report.trailerWireCondition ;
        debug << "TrailerWireComments:" << report.trailerWireComments ;
        debug << "TrailerBarNumber:" << report.trailerBarNumber ;
        debug << "TrailerBeltNumber:" << report.trailerBeltNumber ;
        debug << "TrailerPalletTruck:" << report.trailerPalletTruck ;
        debug << "TrailerExpansionPole:" << report.trailerExpansionPole ;
        debug << "TrailerCornerNumber:" << report.trailerCornerNumber ;
        debug << "TrailerAntislipMap:" << report.trailerAntislipMap ;
        debug << "TrailerLoadedElevator:" << report.trailerLoadedElevator ;
        debug << "TrailerUdtElevatorUpToDate:" << report.trailerUdtElevatorUpToDate ;
        debug << "TrailerLngCertificate:" << report.trailerLngCertificate ;
        debug << "Eni:" << report.eni ;
        debug << "EniNumber:" << report.eniNumber ;
        debug << "Shell:" << report.shell ;
        debug << "ShellNumber:" << report.shellNumber ;
        debug << "Barmalgas:" << report.barmalgas ;
        debug << "BarmalgasNumber:" << report.barmalgasNumber ;
        debug << "Liqvis:" << report.liqvis ;
        debug << "LiqvisNumber:" << report.liqvisNumber ;
        debug << "Liquind:" << report.liquind ;
        debug << "LiquindNumber:" << report.liquindNumber ;
        debug << "E100:" << report.e100 ;
        debug << "E100Number:" << report.e100Number ;
        debug << "Aral:" << report.aral ;
        debug << "AralNumber:" << report.aralNumber ;
        debug << "SetConnectedToTrailer:" << report.setConnectedToTrailer ;
        debug << "MissingBoxContent:" << report.missingBoxContent ;
        debug << "MissinBoxContentComment:" << report.missinBoxContentComment ;
        debug << "MissingDocsInCabin:" << report.missingDocsInCabin ;
        debug << "MissingDocsInCabinComment:" << report.missingDocsInCabinComment ;
        debug << "UploadDate:" << report.uploadDate ;
        debug << "UploadTime:" << report.uploadTime ;

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
