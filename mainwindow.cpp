#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QDebug>
#include <QUrlQuery>
#include <QHttpMultiPart>
#include <QNetworkProxy>
#include <QFile>
#include <QtWebEngineWidgets>
#include <string>
#include <sstream>
#include <vector>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //auto start = std::chrono::system_clock::now();

    ui->setupUi(this);

    m_manager = new QNetworkAccessManager(this);

    // no proxy
    QNetworkProxyFactory::setUseSystemConfiguration(true);
    m_manager->setProxy(QNetworkProxy(QNetworkProxy::NoProxy));
    QNetworkProxy::setApplicationProxy(QNetworkProxy(QNetworkProxy::NoProxy));

    std::vector<std::string> matkat = {
        "P50",
        "V50",
        "P100",
        "P32",
        "V20",
        "V32",
        "V20jun",
        "P42",
        "V42",
        "P20",
        "P30",
        "P44",
        "P60",
        "P62",
        "P25",
        "P35",
        "P45",
        "P52",
        "P53",
        "P75",
        "V30",
        "V45",
        "V53",
        "V75"
    };

    for(int i = 1974; i < 2020; ++i) {
        for(auto m : matkat) {
            std::cout << i << " " << m << std::endl;
            doThings(QString::number(i), QString::fromStdString(m));
        }
    }
    std::cout << "done" << std::endl;

    sumSize(m_data);

    //std::cout << "väli time: " << elap.count() << std::endl;
    //
    //auto end = std::chrono::system_clock::now();
    //std::chrono::duration<double> elapsed_seconds = end-start;
    //std::cout << "elapsed time: " << elapsed_seconds.count() << std::endl;
    //
    //sumSize(parsedData);
}

void MainWindow::doThings(QString year, QString matka) {
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart textPart8;
    textPart8.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"__EVENTTARGET\""));
    textPart8.setBody(QString("dnn$ctr1025$Etusivu$cmdHaeTulokset").toLatin1());
    multiPart->append(textPart8);

    QHttpPart textPart9;
    textPart9.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"__EVENTARGUMENT\""));
    textPart9.setBody(QString().toLatin1());
    multiPart->append(textPart9);

    const std::string viewstate =
            "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";
    QHttpPart textPart10;
    textPart10.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"__VIEWSTATE\""));
    textPart10.setBody(QByteArray(viewstate.c_str(), static_cast<int>(viewstate.length())));
    multiPart->append(textPart10);

    QHttpPart textPart16;
    textPart16.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"__VIEWSTATEGENERATOR\""));
    textPart16.setBody(QString("CA0B0334").toLatin1());
    multiPart->append(textPart16);

    QHttpPart textPart11;
    textPart11.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"__EVENTVALIDATION\""));
    textPart11.setBody(QString("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").toLatin1());
    multiPart->append(textPart11);

    QHttpPart textPart1;
    textPart1.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"dnn$ctr1025$Etusivu$ddlVuosi2x\""));
    textPart1.setBody(year.toLatin1());
    multiPart->append(textPart1);

    QHttpPart textPart2;
    textPart2.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"dnn$ctr1025$Etusivu$ddlMatka2x\""));
    textPart2.setBody(matka.toLatin1());
    multiPart->append(textPart2);

    QHttpPart textPart3;
    textPart3.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"dnn$ctr1025$Etusivu$chkLstSukupuoli2\""));
    textPart3.setBody(QString("kaikki").toLatin1());
    multiPart->append(textPart3);

    QHttpPart textPart4;
    textPart4.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"dnn$ctr1025$Etusivu$ddlIkaluokka2\""));
    textPart4.setBody(QString("kaikki").toLatin1());
    multiPart->append(textPart4);

    QHttpPart textPart5;
    textPart5.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"dnn$ctr1025$Etusivu$txtHakuEtunimi2\""));
    textPart5.setBody(QString().toLatin1());
    multiPart->append(textPart5);

    QHttpPart textPart6;
    textPart6.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"dnn$ctr1025$Etusivu$txtHakuSukunimi2\""));
    textPart6.setBody(QString().toLatin1());
    multiPart->append(textPart6);

    QHttpPart textPart15;
    textPart15.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"dnn$ctr1025$Etusivu$txtHakuJoukkue2\""));
    textPart15.setBody(QString().toLatin1());
    multiPart->append(textPart15);

    QHttpPart textPart7;
    textPart7.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"dnn$ctr1025$Etusivu$ddlKansalaisuus2x\""));
    textPart7.setBody(QString("0").toLatin1());
    multiPart->append(textPart7);

    QHttpPart textPart13;
    textPart13.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"ScrollTop\""));
    textPart13.setBody(QString().toLatin1());
    multiPart->append(textPart13);

    QHttpPart textPart12;
    textPart12.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"__dnnVariable\""));
    textPart12.setBody(QString().toLatin1());
    multiPart->append(textPart12);

    const QUrl url("https://www.finlandiahiihto.fi/Tulokset/Tulosarkisto");

    QNetworkRequest request;
    QSslConfiguration config = QSslConfiguration::defaultConfiguration();
    config.setProtocol(QSsl::TlsV1_2);
    request.setSslConfiguration(config);
    request.setUrl(url);

    m_reply = m_manager->post(request, multiPart);

    multiPart->setParent(m_reply);

    QEventLoop loop;
    connect(m_reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    std::string data = m_reply->readAll().toStdString();
    m_reply->deleteLater();

    //auto vali = std::chrono::system_clock::now();
    //std::chrono::duration<double> elap = vali-start;

    std::size_t ind = data.find("id=\"dnn_ctr1025_Etusivu_dgrTulokset_ctl00\"");
    std::size_t startind = data.find(">", ind + 1);
    std::size_t endind = data.find("</table", startind + 1);

    // Get the result table as string
    std::string stringdata = data.substr(startind + 1, endind - startind - 1);

    startind = stringdata.find("<tbody>");

    stringdata = stringdata.substr(startind + 7, stringdata.length() - startind - 22);

    stringdata = "<div>" + stringdata + "</div>";

    stringdata = escape(stringdata);
    escapeBR(stringdata);
    escapeSpaces(stringdata);

    std::vector<std::vector<std::string>> parsedData;

    QXmlStreamReader reader(QString::fromStdString(stringdata));
    while (!reader.atEnd()) {
        reader.readNext();
        if (reader.isStartElement()) {
            if(reader.name() == "tr") {
                std::vector<std::string> newRow;
                parsedData.emplace_back(newRow);
            }
            if(reader.name() == "td") {
                parsedData.back().emplace_back(reader.readElementText().toStdString());
            }
        }
    }

    if (reader.hasError()) {
        std::cout << reader.errorString().toStdString() << std::endl;
        std::cout << reader.lineNumber() << std::endl;
    } else {
        //printResults(parsedData);
        m_data.emplace_back(parsedData);
    }
}

void MainWindow::printResults(std::vector<std::vector<std::string>>& src) const {
    std::cout << std::endl;
    for(std::vector<std::vector<std::string>>::const_iterator it = src.begin(); it != src.end(); ++it) {
        std::cout << "----" << std::endl << std::endl;

        std::vector<std::string> row = *it;
        for(std::vector<std::string>::const_iterator rowit = row.begin(); rowit != row.end(); ++rowit) {
            std::cout << *rowit << " | ";
        }

        std::cout << std::endl << std::endl;
    }
    std::cout << "----" << std::endl;
}

void MainWindow::sumSize(std::vector<std::vector<std::vector<std::string>>> data) {
    unsigned long long sum = 0;
    for(auto it = data.begin(); it != data.end(); ++it) {
        for(auto test : (*it)) {
            for(auto t : test) {
                sum += t.size();
            }
        }
    }

    std::cout << "Size: " << sum << std::endl;
}

void MainWindow::escapeSpaces(std::string& src) {
    std::size_t found = src.find("&amp;nbsp;");
    while(found != std::string::npos) {
        src = src.replace(found, 10, "");
        found = src.find("&amp;nbsp;", found);
    }
}

std::string MainWindow::escape(const std::string& src) {
    std::stringstream dst;
    for (char ch : src) {
        switch (ch) {
            case '&': dst << "&amp;"; break;
            default: dst << ch; break;
        }
    }
    return dst.str();
}

void MainWindow::escapeBR(std::string& src) {
    std::size_t found = src.find("<br>");
    while(found != std::string::npos) {
        src.insert(found + 4, "</br>");
        found = src.find("<br>", found + 5);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
