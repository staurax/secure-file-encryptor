#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

#include <string>

class QLabel;
class QLineEdit;
class QPushButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void setEncryptMode();
    void setDecryptMode();
    void browseInputFile();
    void chooseOutputFile();
    void togglePasswordVisibility();
    void runFileOperation();

private:
    enum class Mode {
        Encrypt,
        Decrypt
    };

    void buildInterface();
    QWidget *createLeftPanel();
    QWidget *createRightPanel();
    QWidget *createFeatureCard(const QString &title, const QString &subtitle);
    void applyTheme();
    void updateMode();
    void showStatus(const QString &message, bool success);
    bool validateForm(QString *message) const;
    std::string toStdPath(const QString &path) const;

    Mode currentMode;
    QLineEdit *inputPathEdit;
    QLineEdit *outputPathEdit;
    QLineEdit *passwordEdit;
    QLineEdit *confirmPasswordEdit;
    QLabel *confirmPasswordLabel;
    QLabel *statusLabel;
    QPushButton *encryptModeButton;
    QPushButton *decryptModeButton;
    QPushButton *passwordToggleButton;
    QPushButton *actionButton;
};

#endif
