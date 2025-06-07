#include "mainwindow.h"
#include "ConfigManager.h"

#include <QVBoxLayout>
#include <QFile>
#include <QDir>
#include <QProcess>
#include <QTextStream>
#include <QTimer>
#include <QApplication>
#include <zip.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), networkManager(new QNetworkAccessManager(this)) {
    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);

    label = new QLabel("Checking for installation or update...", this);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("font-size: 16px; padding: 10px;");

    progressBar = new QProgressBar(this);
    progressBar->setRange(0, 100);
    progressBar->setTextVisible(false);
    progressBar->setStyleSheet(
        "QProgressBar { height: 20px; border: 1px solid #aaa; border-radius: 10px; background-color: #eee; }"
        "QProgressBar::chunk { background-color: #3498db; border-radius: 10px; }"
    );

    retryButton = new QPushButton("Retry", this);
    retryButton->setVisible(false);
    connect(retryButton, &QPushButton::clicked, this, &MainWindow::onRetryClicked);

    layout->addWidget(label);
    layout->addWidget(progressBar);
    layout->addWidget(retryButton);
    layout->setAlignment(Qt::AlignCenter);
    setCentralWidget(central);
    setFixedSize(400, 160);
    setWindowTitle(ConfigManager::instance().appExecutableName());

    startDownload();
}

MainWindow::~MainWindow() = default;

void MainWindow::startDownload() {
    retryButton->setVisible(false);
    progressBar->setValue(0);
    progressBar->setVisible(true);
    label->setText("Checking for installation or update...");
    qApp->processEvents();

    QString versionFileUrl = ConfigManager::instance().versionFileUrl();
    QNetworkRequest request{ QUrl(versionFileUrl) };

    QNetworkReply *versionReply = networkManager->get(request);
    connect(versionReply, &QNetworkReply::finished, this, [this, versionReply]() {
        if (versionReply->error() != QNetworkReply::NoError) {
            setError("Failed to fetch version information.");
            return;
        }

        QString serverVersion = QString::fromUtf8(versionReply->readAll()).trimmed();
        QString localVersion = ConfigManager::instance().readLocalVersion();

        QString zipUrl;
        if (localVersion.isEmpty()) {
            zipUrl = ConfigManager::instance().baseUrl() + "/basic.zip";
        } else if (localVersion != serverVersion) {
            zipUrl = ConfigManager::instance().baseUrl() + "/" + serverVersion + ".zip";
        } else {
            label->setText("Up to date. Launching app...");
            qApp->processEvents();
            QTimer::singleShot(1500, this, [=]() {
                QProcess::startDetached(ConfigManager::instance().appExecutableName());
                QApplication::quit();
            });
            return;
        }

        QNetworkRequest zipRequest{ QUrl(zipUrl) };
        currentDownload = networkManager->get(zipRequest);

        connect(currentDownload, &QNetworkReply::downloadProgress, this, &MainWindow::onDownloadProgress);

        connect(currentDownload, &QNetworkReply::finished, this, [this, serverVersion]() {
            if (currentDownload->error() != QNetworkReply::NoError) {
                setError("Download failed: " + currentDownload->errorString());
                return;
            }

            QString zipFilePath = QDir::temp().filePath("update.zip");
            QFile file(zipFilePath);
            if (file.open(QIODevice::WriteOnly)) {
                file.write(currentDownload->readAll());
                file.close();

                QString extractPath = ConfigManager::instance().extractPath();
                extractZip(zipFilePath, extractPath);

                ConfigManager::instance().writeLocalVersion(serverVersion);
                label->setText("Extraction complete. Launching app...");
                qApp->processEvents();
                QTimer::singleShot(1500, this, [=]() {
                    QProcess::startDetached(ConfigManager::instance().appExecutableName());
                    QApplication::quit();
                });
            } else {
                setError("Failed to save temporary file.");
            }
        });
    });
}

void MainWindow::extractZip(const QString& zipFilePath, const QString& extractPath) {
    label->setText("Extracting files...");
    qApp->processEvents();

    int err = 0;
    zip* archive = zip_open(zipFilePath.toUtf8().constData(), ZIP_RDONLY, &err);
    if (!archive) {
        setError("Failed to open ZIP archive.");
        return;
    }

    zip_int64_t entries = zip_get_num_entries(archive, 0);
    for (zip_uint64_t i = 0; i < entries; ++i) {
        struct zip_stat st;
        zip_stat_index(archive, i, 0, &st);

        zip_file* zf = zip_fopen_index(archive, i, 0);
        if (!zf) continue;

        QString outPath = extractPath + "/" + QString::fromUtf8(st.name);
        QFileInfo fileInfo(outPath);
        QDir().mkpath(fileInfo.path());

        QFile outFile(outPath);
        if (outFile.open(QIODevice::WriteOnly)) {
            QByteArray buffer(st.size, Qt::Uninitialized);
            zip_fread(zf, buffer.data(), buffer.size());
            outFile.write(buffer);
            outFile.close();
        }

        zip_fclose(zf);
    }

    zip_close(archive);
}

void MainWindow::onDownloadProgress(qint64 received, qint64 total) {
    if (total > 0) {
        int percent = static_cast<int>((received * 100) / total);
        progressBar->setValue(percent);
    }
}

void MainWindow::setError(const QString& message) {
    label->setText("Error: " + message);
    qApp->processEvents();
    progressBar->setVisible(false);
    retryButton->setVisible(true);
}

void MainWindow::onRetryClicked() {
    startDownload();
}
