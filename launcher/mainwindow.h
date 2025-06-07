#pragma once

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QProgressBar>
#include <QLabel>
#include <QPushButton>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QNetworkAccessManager* networkManager;
    QNetworkReply* currentDownload;
    QLabel* label;
    QProgressBar* progressBar;
    QPushButton* retryButton;

    void startDownload();
    void extractZip(const QString& zipFilePath, const QString& extractPath);
    void onDownloadProgress(qint64 received, qint64 total);
    void setError(const QString& message);

private slots:
    void onRetryClicked();
};
