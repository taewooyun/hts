#ifndef APIRESPONSE_H
#define APIRESPONSE_H

#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QList>

// 1. "item" 구조체
struct StockItem {
    QString basDt;
    QString srtnCd;
    QString isinCd;
    QString itmsNm;
    QString mrktCtg;
    QString clpr;
    QString vs;
    QString fltRt;
    QString mkp;
    QString hipr;
    QString lopr;
    QString trqu;
    QString trPrc;
    QString lstgStCnt;
    QString mrktTotAmt;

    static StockItem fromJson(const QJsonObject& json) {
        StockItem item;
        item.basDt = json["basDt"].toString();
        item.srtnCd = json["srtnCd"].toString();
        item.isinCd = json["isinCd"].toString();
        item.itmsNm = json["itmsNm"].toString();
        item.mrktCtg = json["mrktCtg"].toString();
        item.clpr = json["clpr"].toString();
        item.vs = json["vs"].toString();
        item.fltRt = json["fltRt"].toString();
        item.mkp = json["mkp"].toString();
        item.hipr = json["hipr"].toString();
        item.lopr = json["lopr"].toString();
        item.trqu = json["trqu"].toString();
        item.trPrc = json["trPrc"].toString();
        item.lstgStCnt = json["lstgStCnt"].toString();
        item.mrktTotAmt = json["mrktTotAmt"].toString();
        return item;
    }
};

// 2. "body" 구조체
struct ResponseBody {
    int numOfRows;
    int pageNo;
    int totalCount;
    QList<StockItem> items;

    static ResponseBody fromJson(const QJsonObject& json) {
        ResponseBody body;
        body.numOfRows = json["numOfRows"].toInt();
        body.pageNo = json["pageNo"].toInt();
        body.totalCount = json["totalCount"].toInt();

        auto itemsObj = json["items"].toObject();
        auto itemArray = itemsObj["item"].toArray();

        for (const auto& value : itemArray) {
            body.items.append(StockItem::fromJson(value.toObject()));
        }

        return body;
    }
};

// 3. "header" 구조체
struct ResponseHeader {
    QString resultCode;
    QString resultMsg;

    static ResponseHeader fromJson(const QJsonObject& json) {
        ResponseHeader header;
        header.resultCode = json["resultCode"].toString();
        header.resultMsg = json["resultMsg"].toString();
        return header;
    }
};

// 4. 최상위 "response" 구조체
struct ApiResponse {
    ResponseHeader header;
    ResponseBody body;

    static ApiResponse fromJson(const QJsonObject& json) {
        ApiResponse response;
        auto headerJson = json["header"].toObject();
        auto bodyJson = json["body"].toObject();
        response.header = ResponseHeader::fromJson(headerJson);
        response.body = ResponseBody::fromJson(bodyJson);
        return response;
    }
};

#endif
