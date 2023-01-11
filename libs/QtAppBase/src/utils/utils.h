#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#ifndef _UTILS_H_
#define _UTILS_H_

#include <QString>
#include <QCryptographicHash>
#include <QFile>
#include <QRegularExpression>
#include <QDebug>
#include <QUrl>
#include <QHostAddress>
#include <QPushButton>

QString Utils_Uint8ToHexQStr(uint8_t in);
QString Utils_Uint8ArrToHexQStr(uint8_t *arr, ssize_t len);
QString Utils_QByteArrayToHexQStr(const QByteArray& bytes_arr);
QByteArray Utils_RawHexStrToQByteArr(QString in_hexstr);
bool Utils_RawHexStrToArr(QString in_hexstr, uint8_t *out_arr, ssize_t *out_len, ssize_t max_len);
bool Utils_Sha512(uint8_t *in_data, ssize_t in_len, uint8_t outData[64]);

QString Utils_BytesToPrintableAsciiString(const QByteArray *in_arr);
QString Utils_BytesToAlphanumericString(const QByteArray *in_arr);
QString Utils_BytesToBinaryString(const QByteArray *in_arr);
QString Utils_BytesToDECString(const QByteArray *in_arr);

QString ParseCertOrCsrFromFileToHexStr(const QString& fileName);

QStringList Utils_ExtractAllUrls(const QString& inputText);
QStringList Utils_ExtractAllHosts(const QString &input);

bool Utils_IsValidIPv4(const QString &input);
bool Utils_IsValidIPv6(const QString &input);

void Utils_NumericListSort(QStringList *list);
bool Utils_FileExists(const QString& fileName);
QStringList Utils_ParseCsvLine(const QString &string);
QString Util_EncodeForCSV(const QString &string);
QString Utils_FloatWithDigitsPrecision(float number, int precision = 2);

template <class T>
static QString join(const QList<T> &list,
                    const QString &separator,
                    const std::function< QString (const typename QList<T>::value_type &) > toStringFunction);

void Utils_PushButtonStartLoading(QPushButton *button);
void Utils_PushButtonEndLoading(QPushButton *button);
void Utils_Alert(const QString &title, const QString &message);
void Utils_MsgBox(const QString &title, const QString &message);
void Utils_RichTextBoxPopup(const QString &title, const QString &content);

// Data structures utils
void Utils_PrintNestedQMap(const QVariantMap &map, int level = 0);
void Utils_PrintNestedQMap_AsQStrings(const QVariantMap &map, int level = 0);

QByteArrayList SplitByteArray(const QByteArray &in, QByteArray delimiter);

void SleepMs(quint64 ms);

#endif // _UTILS_H_

#pragma clang diagnostic pop