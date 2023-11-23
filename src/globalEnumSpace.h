#ifndef GLOBALENUMSPACE_H
#define GLOBALENUMSPACE_H

#include <QMetaEnum>
#include <QString>
#include <QDebug>
#include <QtGlobal>

namespace EnumHelper {
   // Tool to convert enum values to/from QString
   template <typename E>
   E fromString(const QString &text)
   {
       bool ok;
       auto result = static_cast<E>(QMetaEnum::fromType<E>().keyToValue(text.toUtf8(), &ok));
       if (!ok)
       {
           qDebug() << "Failed to convert enum" << text;
           return {};
       }
       return result;
   }

   template <typename E>
   E fromStringEx(const QMetaEnum &metaEnum, const QString &text)
   {
       bool ok;
       auto result = static_cast<E>(metaEnum.keyToValue(text.toUtf8(), &ok));
       if (!ok)
       {
           qDebug() << "Failed to convert enum" << text;
           return {};
       }
       return result;
   }


   
   template <typename E>
   QString toString(E value)
   {
       const int intValue = static_cast<int>(value);
       return QString::fromUtf8(QMetaEnum::fromType<E>().valueToKey(intValue));
   }

   template <typename E>
   QString toStringEx(const QMetaEnum &metaEnum, E value)
   {
       const int intValue = static_cast<int>(value);
       return QString::fromUtf8(metaEnum.valueToKey(intValue));
   }

   template <typename E>
   QStringList toStringList()
   {
       int intValue=0;
       QStringList result;
       while (QMetaEnum::fromType<E>().valueToKey(intValue) != nullptr)
       {
           result.append(QString::fromUtf8(QMetaEnum::fromType<E>().valueToKey(intValue)));
           intValue++;
       }
       return result;
   }
}


namespace globalEnumSpace {

Q_NAMESPACE
//*******************************************
//****          crio 9056                 ***
//*******************************************
enum ModuleType {
    ANALOG  = 0,
    DIGITAL = 1,
    COUNTER = 2,
    CODER   = 3
};
Q_ENUM_NS (ModuleType)



//********************************************
//******        search type              *****
//********************************************
enum NumericSearchType {
                         strictlyInferior = 0,
                         strictlySuperior = 1,
                         inferiorOrEgal   = 2,
                         superiorOrEgal   = 3,
                         egal             = 4,
                         between          = 5
                       };
Q_ENUM_NS (NumericSearchType)
//********************************************
//******        interpolators            *****
//********************************************
enum InterpolationType { Linear                      = 0,
                         LinearRegression            = 1,
                         Multi_LinearRegression      = 2,
                         CubicSpline                 = 3,
                         Akima                       = 4,
                         Steffen                     = 5,
                         CalculationNoInterpolation  = 6
                       };
Q_ENUM_NS (InterpolationType)

/*enum splineType {
        lin             = 0,   // linear interpolation
        cspline         = 1,   // cubic splines (classical C^2)
        cspline_hermite = 2    // cubic hermite splines (local, only C^1)
    };
Q_ENUM_NS (splineType)*/

// boundary condition type for the spline end-points
enum splineBundaryType {
    first_deriv  = 0,
    second_deriv = 1
};
Q_ENUM_NS (splineBundaryType)

//********************************************
//********        Alarms               *******
//********************************************
enum AlarmMode
{
    exceeding  = 0,
    bellow     = 1,
    outOfRange = 2,
    inRange    = 3,
    smart      = 4
};
Q_ENUM_NS (AlarmMode)


//********************************************
//********        Charting             *******
//********************************************

enum BufferMode
     {
        appendValue,
        replaceBuffer,
        appendBuffer,
     };
Q_ENUM_NS(BufferMode)
//-------- Tracks ----------

enum TrackDrawMode
        {
           isLinear      = 0,
           isLogarythmic = 1
        };
Q_ENUM_NS(TrackDrawMode)


enum TrackType
       {
          line = 0,
          aera = 1,
          bar  = 2,
       };
Q_ENUM_NS(TrackType)

//------- Axis ----------

enum VersusAxisType      {
                           dateTime    = 0,
                           linear      = 1,
                           logarythmic = 2
                         };
Q_ENUM_NS(VersusAxisType)

enum VersusAxisTimeFormat {
                            completeDateTime = 0,
                            timeOnly         = 1
                         };
Q_ENUM_NS(VersusAxisTimeFormat)

//------ Chart -------------
enum ChartOrientation    {
                           verticalChart   = 0,
                           horizontalChart = 1
                         };
Q_ENUM_NS(ChartOrientation)

//********************************************
//********        Serialization        *******
//********************************************


enum globalJsonTreeModelMode     { generic          =0,
                                   rig              =1,
                                 };

Q_ENUM_NS(globalJsonTreeModelMode)



//----------------------------------

enum globalCalibrationType
        { twoPointLinear          =  0,
          nPointsMultiLinear      =  1,
          nPointsSpline           =  2,
          nPointsLeastSquare      =  3
        };
Q_ENUM_NS(globalCalibrationType)

enum globalCustomTankSum
        { customTankSum_1         =  0,
          customTankSum_2         =  1,
          customTankSum_1_and_2   =  2,
          customTankSum_none      =  3
      };
Q_ENUM_NS(globalCustomTankSum)

enum globalTankTypeEnum
       {    activPit              = 0,
            tripTank              = 1,
            reservePit            = 2,
            mixingPit             = 3,
            sandTrap              = 4,
            decantation           = 5,
            slugPit               = 6
       };
Q_ENUM_NS(globalTankTypeEnum)

enum globalErrorTypeEnum
        { ObjectIsNull             =  0 ,
          typeConversionError      =  1 ,
          fileOpenError            =  2 ,
          imageLoadingError        =  3 ,
          listRangeError           =  4 ,
          iteratorError            =  5 ,
          bufferLoadingError       =  6 ,
          dateNotValid             =  7 ,
          jsonSerializationError   =  8 ,
          jsonDeserializationError =  9 ,
          jsonDocumentNotObject    =  10,
          dbError                  =  11,
          globalUnknown            = 255};
Q_ENUM_NS(globalErrorTypeEnum)

enum geoConvertType
        { Geographic  = 0,
          DMS_0       = 1,
          DMS_2DOT    = 2,
          UTMUPS      = 3,
          MGRS        = 4,
          Convergence = 5,
          Tiles       = 6};
Q_ENUM_NS(geoConvertType)

// Mud Rheology
enum QMudRheologyEnum { eMudPowerLawDual = 0,
                        eMudBingham = 1,
                        eMudHerschelBulkley = 2,
                        eMudOstwald = 3,
                        eMudCasson = 4,
                        eMudPowerLawSimple = 5 } ;
Q_ENUM_NS(QMudRheologyEnum)


enum addingType
    {
        Add           = 0,
        InsertBefore  = 1,
        InsertAfter   = 2
    };
Q_ENUM_NS(addingType)

enum tubeTypes
    {
        Casing      = 12,
        HwDrillpe   = 6,
        DrillPipe   = 10,
        DrillCollar = 3
    };
Q_ENUM_NS(tubeTypes)

enum PipeRequestTypes
  {
        DefaultCasingTable       = 20  ,
        DefaultDrillPipeTable    =  0  ,
        DefaultDrillCollarTable = 10  ,

        OrdinaryCasingTable       = 21 ,
        OrdinaryDrillPipeTable    =  1 ,
        OrdinaryDrillCollarTable  = 11 ,

        UserCasingTable           = 22 ,
        UserDrillPipeTable        =  2 ,
        UserDrillCollarTable      = 12 ,

        DrillSiteCasingTable      = 23 ,
        DrillSiteDrillPipeTable   =  3 ,
        DrillSiteDrillCollarTable = 13
  };
Q_ENUM_NS(PipeRequestTypes)

enum tinyLedSate {ledoff=0,ledon=1,lederror=2};
Q_ENUM_NS(tinyLedSate)

enum InHouseWidgetTag {
     isInstrumentaionWorkShop,
     isEditor,
     isSPPGauge,
     isPitGauge,
     isWitsLibraryViewer,
     isConverterTool,
     isModBusViewer
                      };
Q_ENUM_NS(InHouseWidgetTag)

enum TValueFormat {eFormatAlphaNum,eFormatSingle, eFormatInt16, eFormatInt32};
Q_ENUM_NS(TValueFormat)

enum TValueCOMEvent  {eCOMReaderConnect,
                      eCOMReaderDisconnect,
                      eCOMWriterConnect,
                      eCOMWriterDisconnect,
                      eCOMPacketReceived,
                      eCOMPacketSent};
Q_ENUM_NS(TValueCOMEvent)

enum TValueTCPEvent {eTCPReaderConnect,
                     eTCPReaderDisconnect,
                     eTCPWriterConnect,
                     eTCPWriterDisconnect,
                     eTCPPacketReceived,
                     eTCPPacketSent};
Q_ENUM_NS(TValueTCPEvent)


enum TunitsSystemType {unitsSystemSargent,
                       unitsSystemMetric,
                       unitsSystemFPS,
                       unitsSystemSI,
                       unitsSystemCustom,
                       unitsSystemMetricCombined};
Q_ENUM_NS(TunitsSystemType)


enum TunitsFamilyType {uNoUnits=0,
                      uStrokes=1,
                      uPercent=2,
                      uLength=3,
                      uLengthDiameter=4,
                      uArea=5,
                      uVolume=6,
                      uMass=7,
                      uTime=8,
                      uViscosity=9,
                      uViscosityCp=10,
                      uViscosityLbf=11,
                      uForce=12,
                      uPower=13,
                      uAngle=14,
                      uPressure=15,
                      uVelocity=16,
                      uFlowRate=17,
                      uMudDensity=18,
                      uPumpRate=19,
                      uPumpOut=20,
                      uWeightLinear=21,
                      uVolumeLinear=22,
                      uPowerByArea=23,
                      uForceByArea=24,
                      uRotationalVelocity=25,
                      uTemperature=26,
                      uGasChrom=27,
                      uTorque30=30,
                      eConductance=32,
                      eResistance=33,
                      eConductivityResistivity=34,
                      ePenetrolog=35,
                      eDogLegSeverity=36,
                      ePressureYield=37,
                      eTurbinRate=38,
                      eGasOther=40,
                      eCost41=41,
                      eCostByDistance=42,
                      eCuttingsCEC=43,
                      ePermeability=44,
                      eMudFiltrate=45,
                      eFunnelViscosity=46,
                      eMudReport=47,
                      eVoltage=48,
                      eGammaRay=49,
                      eWeight50=50,
                      eTimeByDistance=51,
                      eAcceleration=52,
                      eSquareTime=53,
                      eCostByTime=54,
                      eAngleRevolution=55,
                      ePressureByLength=56,
                      ePressureByVolume=57,
                      eVolumeByPressure=58,
                      eCompressibility=59,
                      eGasCH4=60,
                      eGasH2S=61,
                      //uTimeForRopOld,uRateOfPenetrationOld, // old unit, no longer used, just for SVUnitConverter
                      eError, //index ?
                      eDateTime, //index ?
                      };
Q_ENUM_NS(TunitsFamilyType)

enum unitsStyle {unitsSingle,
                 unitsDivBy,
                 unitsMultBy};
Q_ENUM_NS(unitsStyle)

enum pumpTypeEnum {triplex = 0,
                  duplex   = 1};
Q_ENUM_NS(pumpTypeEnum)

enum pumpUsageEnum {notUsed   = 0,
                    drilling  = 1,
                    booster   = 2,
                    cementing = 3
                   };
Q_ENUM_NS(pumpUsageEnum)

enum pumpCalculationModeEnum{fullCompute =0,
                             theoritcalCompute =1
                            };
Q_ENUM_NS(pumpCalculationModeEnum)
}


#endif 
