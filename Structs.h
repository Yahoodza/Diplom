/**************************************************************************
*  Structs - модуль, содержащий структуры информационного массива tmypl
***************************************************************************
*  Название: Structs.
*  Разработал: Королёв Петр Олегович, группа ТМП-81.
*  Язык программирвоания: C++.
*  Фреймворк: Qt.
*  Среда разработки: Qt Creator.
*  Краткое описание:
*       Данный модуль содержит описания структур для их дальнейшего
*       использования при извлечении данных из информационного
*       массива tmypl.
*
*  Имеющиеся структуры в модуле:
*       SMID - справка о расположении данных;
*       ID0 - исходные данные по ветку съема;
*       MSI - массив справок, содержащих информацию о положении в комплекте
*       исходных данных по сеансам включений;
*       ID1 - исходные данные по сеансам включений в съёме;
*       SMsp_type - справочная информация "Справки...";
*       sSPRAV_type - cправка о положении i-го сеанса наблюдения в
*       информационном массиве tmyp;
*       MNI_type - формуляр массива навигационной информации (ФМНИ);
*       OutputSet_type - выходной набор данных, составленный из данных
*       файла tmypl.
*/

#ifndef STRUCTS_H
#define STRUCTS_H
#include <QString>

#pragma pack(push, 1)

/* SMID - справка о расположении данных.
*  Переменные структуры:
*       length - длина в байтах комплекта ИД;
*       n - планируемое количество сеансов исходной информации в съеме;
*       nf - число пакетов исходных данных для совместной обработки с КАПС “О”;
*       nrk -  число пакетов разовых команд;
*       cmidf - смещение от начала комплекта ИД для совместной
*       обработки с КАПС “О”;
*       cmmk - смещение от начала комплекта ИД первого пакета разовых команд;
*       cmminf - смещение от начала комплекта ИД информационного
*       массива изделия 14В18;
*       cmspridl - смещение от начала комплекта ИД массива справок, содержащих
*       информацию о положении в комплекте ИД исходных данных по
*       сеансам включений;
*       rez[] - резерв.
*/

typedef struct
{
    unsigned long length;

    short n;

    short nf;

    short nrk;

    unsigned long cmidf;

    unsigned long cmmk;

    unsigned long cmminf;

    unsigned long cmspridl;

    char rez[40];
} SMID;

/* ID0 - исходные данные по ветку съема.
*  Переменные структуры:
*       NKA - номер объекта;
*       NBC - номер витка съема;
*       Nprg - номер программы;
*       YNid - номер учетный комплекта ИД;
*       Year - год начала съема;
*       Month - месяц начала съема;
*       Day - день начала съема;
*       TC - московское время начала съема [сек];
*       tvi - длительность режима ВИ [сек];
*       RC - код режима текущего съема: хкккхррр;
*       RC_pred - код режима предыдущего съема: хкккхррр;
*       TR_VI[] - параметры работы изделия 14P18 в режиме ВИ;
*       Nppi - номер пункта;
*       NSR_L - номер СР, через который передается информация съема;
*       ks - количество сеансов информации в съеме;
*       rr[] - позиционная информация по сеансам включения изделия 14Р18:
*       16 разрядов для каждого сеанса - авгххббб|ррк*ппноу*;
*       RKO_tip - код типа НУ;
*       RKO - номер витка НУ, по которым рассчитаны ИД;
*       Nvz - номер витка закладки программы;
*       nrk - число комплектов массивов разовых команд;
*       nf - число комплектов ИД для совместной обработки КТС "О".
*/

typedef struct
{
    short NKA;

    short NBC;

    short Nprg;

    short YNid;

    short Year;

    unsigned char Month;

    unsigned char Day;

    float TC;

    short tvi;

    unsigned char RC;

    unsigned char RC_pred;

    unsigned char TR_VI[50];

    unsigned char Nppi;

    unsigned char NSR_L;

    unsigned char ks;

    unsigned short rr[100];

    unsigned char RKO_tip;

    short RKO;

    short Nvz;

    short nrk;

    short nf;
} ID0;

/* MSI - массив справок, содержащих информацию о положении в комплекте
*  исходных данных по сеансам включений.
*  Переменная структуры:
*       СM - справка, содержащая информацию о расположении сеансов.
*/

typedef struct MSI
{
    unsigned long CM;
} MSI;

/* ID1 - исходные данные по сеансам включений в съёме.
*  Переменные структуры:
*       rrs - номер режима работы изделия 14В18 в сеансе;
*       ps - признак потребителя;
*       tr_zi[] - параметры настройки работы изделия 14P18;
*       Year - год начала сеанса;
*       Month - месяц начала сеанса;
*       Day - число начала сеанса;
*       sutki - число суток в пределах текущего четырёхлетия на
*       момент начала сеанса;
*       nlet4 - число четырёхлетий (с 1-го января 1996 г.) на
*       момент начала сеанса;
*       chas - число часов на момент начала сеанса;
*       min - число минут на момент начала сеанса;
*       sec - число секунд на момент начала сеанса;
*       dt - длительность сеанса [сек];
*       bn - координаты начала сеанса - широта [рад];
*       ln - координаты начала сеанса - долгота [рад];
*       bk - координаты конца сеанса - широта [рад];
*       lk - координаты конца сеанса - долгота [рад];
*       uz - долгота узла [рад];
*       nr - перечень признаков наблюдения районов, на каждый
*       порядковый номер района отводится один бит,
*       нумерация битов производится слева направо.
*       В 1-ом бите размещается информация о задании
*       района с номером 1, в 32-ом бите - информация
*       о наблюдении района с номером 32. В случае
*       наблюдения района, соответствующий бит содержит
*       единицу, в противном случает - ноль;
*       nv - текущий номер витка, на котором производится включение;
*       nd - номер витка в ЦЗТ;
*       tuz - время прохождения узла [сек];
*       dt0 - время сеанса от узла [сек];
*       i - угол наклонения орбиты [рад];
*       x[] - коэффициенты аппроксимирующего полинома для
*       координат изделия 14Ф148 (по 8 на x, y и z
*       соответственно по убывании степени от 7 до 0 и t ср. сеанса);
*       mr[] - массив режимов изделия 14B18 для сеанса;
*       psop - признак совместной обработки с КАПС "О";
*       too - значение периода обращения, с;
*       n_zaj - номер заявки на проведение сеанса включения в
*       центре заявок;
*       abnnom - номер заявки на проведение сеанса включения
*       у абонента;
*       rez[] - резерв.
*/

typedef struct
{
    unsigned char rrs;

    unsigned char ps;

    unsigned char tr_zi[50];

    short Year;

    unsigned char Month;

    unsigned char Day;

    short sutki;

    char nlet4;

    char chas;

    char min;

    char sec;

    short dt;

    float bn;

    float ln;

    float bk;

    float lk;

    float uz;

    unsigned long nr;

    short nv;

    unsigned char nd;

    float tuz;

    float dt0;

    float i;

    double x[25];

    unsigned short mr[61];

    char psop;

    float too;

    unsigned long n_zaj;

    unsigned long abnnom;

    char rez[3];
} ID1;

/* SMsp_type - справочная информация "Справки...".
*  Переменные структуры:
*       length - длина в байтах "Справки….";
*       n - планируемое количество сеансов исходной информации в съеме;
*       nb - общее число информационных блоков, составляющих
*       информационный массив tmyp;
*       nbis - число искаженных информационных блоков в информационном
*       массиве tmyp;
*       cmsprav - смещение от начала "Справки…" к "Справки о расположении 1-го
*       сеанса наблюдения в информационном массиве tmyp".
*/

typedef struct
{
        unsigned long length;

        short n;

        short nb;

        short nbis;

        unsigned long cmsprav;
} SMsp_type;

/* sSPRAV_type - справка о положении i-го сеанса наблюдения в
*  информационном массиве tmyp.
*  Переменные структуры:
*       nc - номер сеанса исходной информации в съеме;
*       rrBk - номер режима работы БСА "Барс" в сеансе;
*       rrBCA - значение кода режима работы БСА "Барс" в сеансе;
*       rrBAPI - значение кода режима работы БАПИ в сеансе;
*       chas - номер текущего часа в момент начала сеанса;
*       min - номер текущей минуты в момент начала сеанса;
*       sec - номер текущей секунды в момент начала сеанса;
*       dt - длительность сеанса (сек);
*       kgrmr - количество групп от первой группы, определяющей
*       начало сеанса по информации БАПИ "Тракон" до
*       ближайшей маркерной группы;
*       kgrmb - количество групп от группы, определяющей начало
*       сеанса по группе "МВ" до ближайшей маркерной группы;
*       kgrchd - количество групп от первой группы, определяющей
*       начало сеанса по времени (из формуляра МО),
*       до ближайшей маркерной группы;
*       adrnm - адрес начала сеанса по информации БАПИ "Тракон";
*       adrkm - адрес конца сеанса по информации БАПИ "Тракон";
*       adrnMB - адрес начала сеанса по группе "МВ";
*       adrkMB - адрес конца сеанса по группе "МВ";
*       adrnb - адрес начала сеанса по времени (из формуляра МО);
*       adrkb - адрес конца сеанса по времени (из формуляра МО).
*/

typedef struct
{
        short nc;

        unsigned char rrBk;

        char rrBCA;

        char rrBAPI;

        char chas;

        char min;

        char sec;

        short dt;

        short kgrmr;

        short kgrmb;

        short kgrchd;

        unsigned long adrnm;

        unsigned long adrkm;

        unsigned long adrnMB;

        unsigned long adrkMB;

        unsigned long adrnb;

        unsigned long adrkb;
} sSPRAV_type;

/* MNI_type - формуляр массива навигационной информации (МНИ).
*  Переменные структуры:
*       TypeMes - тип сообщения;
*       qGroup - количество групп;
*       NumMas - номер массива МНИ;
*       cMV4l - текущее четырёхлетие;
*       cMVday - текущее число суток;
*       cMVhour - текущее число часов;
*       cMVmin - текущее число минут;
*       cMVsec - текущее число секунд;
*       coefA0X - коэффициент A0x;
*       coefA0Y - коэффициент A0y;
*       coefA0Z - коэффициент A0z;
*       coefA1X - коэффициент A1x;
*       coefA1Y - коэффициент A1y;
*       coefA1Z - коэффициент A1z;
*       coefA2X - коэффициент A2x;
*       coefA2Y - коэффициент A2y;
*       coefA2Z - коэффициент A2z;
*       KSMNI - контрольная сумма формуляра МНИ;
*       KS - рассчитываемая контрольная сумма формуляра МНИ.
*/

typedef struct
{
        char TypeMes;

        char qGroup;

        char NumMas;

        char cMV4l;

        short cMVday;

        char cMVhour, cMVmin, cMVsec;

        long coefA0X, coefA0Y, coefA0Z;

        long coefA1X, coefA1Y, coefA1Z;

        long coefA2X, coefA2Y, coefA2Z;

        unsigned short KSMNI, KS;
} MNI_type;

/* OutputSet_type - выходной набор данных, составленный из данных файла tmypl.
*  Переменные структуры:
*       NameMas - наименование массива исходной информации;
*       YNsd - учётный номер комплекта исходных данных;
*       n - количество сеансов в исходном массиве;
*       YearStart - год начала обработки;
*       MonthStart - месяц начала обработки;
*       DayStart - день начала обработки;
*       TimeStart - время (в секундах) начала обработки;
*       YearEnd - год окончания обработки;
*       MonthEnd - месяц окончания обработки;
*       DayEnd - день окончания обработки;
*       TimeEnd - время (в секундах) окончания обработки;
*       SizeTmypl - длина всего массива;
*       lengthSD - длина комплекта исходных данных;
*       lengthII - длина массива исходной информации;
*       kolIG - общее количество информационных групп;
*       kolIB - общее количество информационных блоков;
*       Mndi - количество групп со значением Мнди = 1;
*       Mn - количество групп со значением Мн = 1;
*       SesN - номер сеанса;
*       YearSes - год создания сеанса;
*       MonthSes - месяц создания сеанса;
*       DaySes - день создания сеанса;
*       chasSes - число часов на момент создания сеанса;
*       minSes - число минут на момент создания сеанса;
*       secSes - число секунд на момент создания сеанса;
*       YNsdSes - учётный номер комплекта исходных данных;
*       lengthSes - длина сеанса в исходном массиве;
*       kolIGses - общее количество информационных групп в сеансе;
*       kolIBses - общее количество информационных блоков в сеансе;
*       MndiSes -  количество групп со значением Мнди = 1 в сеансе;
*       MnSes -  количество групп со значением Мн = 1 в сеансе;
*       kolMNIses - количество выходных формуляров МНИ;
*       offset - смещение (в байтах от начала файла) области,
*       где расположены выходные формуляры МНИ.
*/

typedef struct
{
        char NameMas[50];

        short YNsd;

        short n;

        short YearStart;

        unsigned char MonthStart;

        unsigned char DayStart;

        float TimeStart;

        short YearEnd;

        unsigned char MonthEnd;

        unsigned char DayEnd;

        float TimeEnd;

        unsigned long SizeTmypl;

        unsigned long lengthSD;

        unsigned long lengthII;

        unsigned long kolIG;

        short kolIB;

        long Mndi;

        long Mn;

        short SesN;

        short YearSes;

        unsigned char MonthSes;

        unsigned char DaySes;

        char chasSes;

        char minSes;

        char secSes;

        short YNsdSes;

        unsigned long lengthSes;

        unsigned long kolIGses;

        short kolIBses;

        long MndiSes;

        long MnSes;

        short kolMNIses;

        unsigned long offset;
} OutputSet_type;

#pragma pack(pop)

#endif // STRUCTS_H
