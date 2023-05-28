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
    QImage damageImage;
    bool tireCondition; // 1 - ok, 0 - not ok
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
    bool tiresCondition; // 1 - good, 0 - qlineedit for the comment
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
    bool trailerVireCondition;
    QString trailerVireComments;
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
