
#include <QApplication>
#include <QPlainTextEdit>
#include <QCoroutine.hpp>
#include <QtNetwork>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPlainTextEdit edit;
    edit.show();

    QCoroutine::run([&edit]() {
        {
            QUrl url("http://download.qt.io/online/qt5/linux/x64/online_repository/Updates.xml");
            QNetworkRequest request(url);
            QNetworkAccessManager manager;
            auto reply = manager.get(request);
            QCoroutine::qAwait(reply, &QNetworkReply::finished);
            edit.setPlainText(reply->readAll());
            reply->deleteLater();
        }
        {
            QUrl url("http://www.baidu.com");
            QNetworkRequest request(url);
            QNetworkAccessManager manager;
            auto reply = manager.get(request);
            QCoroutine::qAwait(reply, &QNetworkReply::finished);
            edit.setPlainText(reply->readAll());
            reply->deleteLater();
        }
    });

    return a.exec();
}
