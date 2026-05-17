#include "MainWindow.h"

#include "crypto.h"

#include <QApplication>
#include <QColor>
#include <QFileDialog>
#include <QFileInfo>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QStyle>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      currentMode(Mode::Encrypt),
      inputPathEdit(nullptr),
      outputPathEdit(nullptr),
      passwordEdit(nullptr),
      confirmPasswordEdit(nullptr),
      confirmPasswordLabel(nullptr),
      statusLabel(nullptr),
      encryptModeButton(nullptr),
      decryptModeButton(nullptr),
      passwordToggleButton(nullptr),
      actionButton(nullptr)
{
    buildInterface();
    applyTheme();
    updateMode();
}

void MainWindow::buildInterface()
{
    setWindowTitle("Secure File Encryptor");
    resize(1200, 720);
    setMinimumSize(980, 640);

    QWidget *root = new QWidget(this);
    root->setObjectName("RootWidget");
    setCentralWidget(root);

    QHBoxLayout *rootLayout = new QHBoxLayout(root);
    rootLayout->setContentsMargins(36, 36, 36, 36);
    rootLayout->setSpacing(32);

    rootLayout->addWidget(createLeftPanel(), 1);
    rootLayout->addWidget(createRightPanel(), 1);
}

QWidget *MainWindow::createLeftPanel()
{
    QWidget *panel = new QWidget(this);
    panel->setObjectName("LeftPanel");

    QVBoxLayout *layout = new QVBoxLayout(panel);
    layout->setContentsMargins(16, 18, 16, 18);
    layout->setSpacing(18);

    QLabel *logo = new QLabel("SE", panel);
    logo->setObjectName("LogoBadge");
    logo->setAlignment(Qt::AlignCenter);
    logo->setFixedSize(68, 68);

    QLabel *title = new QLabel("Secure File Encryptor", panel);
    title->setObjectName("HeroTitle");
    title->setWordWrap(true);

    QLabel *subtitle = new QLabel("Encrypt and decrypt your files with password protection.", panel);
    subtitle->setObjectName("HeroSubtitle");
    subtitle->setWordWrap(true);

    layout->addWidget(logo);
    layout->addSpacing(24);
    layout->addWidget(title);
    layout->addWidget(subtitle);
    layout->addSpacing(20);

    layout->addWidget(createFeatureCard("File Encryption", "Protect documents, archives, and private files."));
    layout->addWidget(createFeatureCard("Password Protection", "Access is guarded by your chosen password."));
    layout->addWidget(createFeatureCard("Local Processing", "Files stay on this computer from start to finish."));
    layout->addStretch();

    QLabel *footer = new QLabel("© 2026 Secure Encryptor", panel);
    footer->setObjectName("FooterText");
    layout->addWidget(footer);

    return panel;
}

QWidget *MainWindow::createFeatureCard(const QString &title, const QString &subtitle)
{
    QFrame *card = new QFrame(this);
    card->setObjectName("FeatureCard");

    QHBoxLayout *layout = new QHBoxLayout(card);
    layout->setContentsMargins(16, 14, 16, 14);
    layout->setSpacing(12);

    QLabel *dot = new QLabel(card);
    dot->setObjectName("FeatureDot");
    dot->setFixedSize(12, 12);

    QVBoxLayout *textLayout = new QVBoxLayout();
    textLayout->setContentsMargins(0, 0, 0, 0);
    textLayout->setSpacing(2);

    QLabel *titleLabel = new QLabel(title, card);
    titleLabel->setObjectName("FeatureTitle");

    QLabel *subtitleLabel = new QLabel(subtitle, card);
    subtitleLabel->setObjectName("FeatureSubtitle");
    subtitleLabel->setWordWrap(true);

    textLayout->addWidget(titleLabel);
    textLayout->addWidget(subtitleLabel);

    layout->addWidget(dot, 0, Qt::AlignTop);
    layout->addLayout(textLayout);

    return card;
}

QWidget *MainWindow::createRightPanel()
{
    QWidget *container = new QWidget(this);
    QVBoxLayout *outerLayout = new QVBoxLayout(container);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->addStretch();

    QFrame *card = new QFrame(container);
    card->setObjectName("ToolCard");
    card->setMinimumWidth(500);

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(card);
    shadow->setBlurRadius(42);
    shadow->setOffset(0, 18);
    shadow->setColor(QColor(74, 52, 145, 54));
    card->setGraphicsEffect(shadow);

    QVBoxLayout *layout = new QVBoxLayout(card);
    layout->setContentsMargins(34, 34, 34, 34);
    layout->setSpacing(16);

    QLabel *heading = new QLabel("File Security Tool", card);
    heading->setObjectName("CardHeading");

    QLabel *caption = new QLabel("Choose a file, set a password, and run the operation locally.", card);
    caption->setObjectName("CardCaption");
    caption->setWordWrap(true);

    QFrame *segment = new QFrame(card);
    segment->setObjectName("SegmentControl");
    segment->setMinimumHeight(50);
    QHBoxLayout *segmentLayout = new QHBoxLayout(segment);
    segmentLayout->setContentsMargins(4, 4, 4, 4);
    segmentLayout->setSpacing(4);

    encryptModeButton = new QPushButton("Encrypt", segment);
    decryptModeButton = new QPushButton("Decrypt", segment);
    encryptModeButton->setCheckable(true);
    decryptModeButton->setCheckable(true);
    encryptModeButton->setFocusPolicy(Qt::NoFocus);
    decryptModeButton->setFocusPolicy(Qt::NoFocus);
    encryptModeButton->setMinimumHeight(40);
    decryptModeButton->setMinimumHeight(40);
    encryptModeButton->setObjectName("SegmentButton");
    decryptModeButton->setObjectName("SegmentButton");
    segmentLayout->addWidget(encryptModeButton);
    segmentLayout->addWidget(decryptModeButton);

    connect(encryptModeButton, &QPushButton::clicked, this, &MainWindow::setEncryptMode);
    connect(decryptModeButton, &QPushButton::clicked, this, &MainWindow::setDecryptMode);

    QLabel *inputLabel = new QLabel("Input file", card);
    inputLabel->setObjectName("FieldLabel");
    inputPathEdit = new QLineEdit(card);
    inputPathEdit->setPlaceholderText("Select a file to process");
    inputPathEdit->setMinimumHeight(44);
    QPushButton *browseButton = new QPushButton("Browse", card);
    browseButton->setObjectName("SecondaryButton");
    browseButton->setMinimumHeight(44);
    connect(browseButton, &QPushButton::clicked, this, &MainWindow::browseInputFile);

    QHBoxLayout *inputLayout = new QHBoxLayout();
    inputLayout->setSpacing(10);
    inputLayout->addWidget(inputPathEdit, 1);
    inputLayout->addWidget(browseButton);

    QLabel *outputLabel = new QLabel("Output file", card);
    outputLabel->setObjectName("FieldLabel");
    outputPathEdit = new QLineEdit(card);
    outputPathEdit->setPlaceholderText("Choose where to save the result");
    outputPathEdit->setMinimumHeight(44);
    QPushButton *saveAsButton = new QPushButton("Save As", card);
    saveAsButton->setObjectName("SecondaryButton");
    saveAsButton->setMinimumHeight(44);
    connect(saveAsButton, &QPushButton::clicked, this, &MainWindow::chooseOutputFile);

    QHBoxLayout *outputLayout = new QHBoxLayout();
    outputLayout->setSpacing(10);
    outputLayout->addWidget(outputPathEdit, 1);
    outputLayout->addWidget(saveAsButton);

    QLabel *passwordLabel = new QLabel("Password", card);
    passwordLabel->setObjectName("FieldLabel");
    passwordEdit = new QLineEdit(card);
    passwordEdit->setPlaceholderText("Enter password");
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setMinimumHeight(44);
    passwordToggleButton = new QPushButton("Show", card);
    passwordToggleButton->setObjectName("SecondaryButton");
    passwordToggleButton->setFixedWidth(82);
    passwordToggleButton->setMinimumHeight(44);
    connect(passwordToggleButton, &QPushButton::clicked, this, &MainWindow::togglePasswordVisibility);

    QHBoxLayout *passwordLayout = new QHBoxLayout();
    passwordLayout->setSpacing(10);
    passwordLayout->addWidget(passwordEdit, 1);
    passwordLayout->addWidget(passwordToggleButton);

    confirmPasswordLabel = new QLabel("Confirm password", card);
    confirmPasswordLabel->setObjectName("FieldLabel");
    confirmPasswordEdit = new QLineEdit(card);
    confirmPasswordEdit->setPlaceholderText("Re-enter password");
    confirmPasswordEdit->setEchoMode(QLineEdit::Password);
    confirmPasswordEdit->setMinimumHeight(44);

    actionButton = new QPushButton("Encrypt File", card);
    actionButton->setObjectName("PrimaryButton");
    actionButton->setMinimumHeight(48);
    connect(actionButton, &QPushButton::clicked, this, &MainWindow::runFileOperation);

    statusLabel = new QLabel("Ready to protect your files.", card);
    statusLabel->setObjectName("StatusInfo");
    statusLabel->setWordWrap(true);
    statusLabel->setMinimumHeight(42);

    QLabel *watermarkLabel = new QLabel("Build by Hannan Asghar (@Staurax)", card);
    watermarkLabel->setObjectName("WatermarkText");
    watermarkLabel->setAlignment(Qt::AlignCenter);

    layout->addWidget(heading);
    layout->addWidget(caption);
    layout->addSpacing(8);
    layout->addWidget(segment);
    layout->addSpacing(8);
    layout->addWidget(inputLabel);
    layout->addLayout(inputLayout);
    layout->addWidget(outputLabel);
    layout->addLayout(outputLayout);
    layout->addWidget(passwordLabel);
    layout->addLayout(passwordLayout);
    layout->addWidget(confirmPasswordLabel);
    layout->addWidget(confirmPasswordEdit);
    layout->addSpacing(12);
    layout->addWidget(actionButton);
    layout->addWidget(statusLabel);
    layout->addWidget(watermarkLabel);

    outerLayout->addWidget(card);
    outerLayout->addStretch();

    return container;
}

void MainWindow::applyTheme()
{
    qApp->setStyleSheet(R"(
        QWidget#RootWidget {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #6A50C6, stop:0.48 #A99DDC, stop:1 #F7F8FC);
            color: #2E2E35;
            font-family: "Segoe UI", "Inter", Arial, sans-serif;
            font-size: 14px;
        }

        QWidget#LeftPanel {
            background: transparent;
        }

        QLabel#LogoBadge {
            background: rgba(255, 255, 255, 71);
            border: 1px solid rgba(255, 255, 255, 128);
            border-radius: 20px;
            color: #FFFFFF;
            font-size: 22px;
            font-weight: 800;
        }

        QLabel#HeroTitle {
            color: #FFFFFF;
            font-size: 42px;
            font-weight: 800;
        }

        QLabel#HeroSubtitle {
            color: rgba(255, 255, 255, 219);
            font-size: 18px;
            line-height: 140%;
        }

        QFrame#FeatureCard {
            background: rgba(255, 255, 255, 51);
            border: 1px solid rgba(255, 255, 255, 87);
            border-radius: 16px;
        }

        QLabel#FeatureDot {
            background: #FFFFFF;
            border-radius: 6px;
        }

        QLabel#FeatureTitle {
            color: #FFFFFF;
            font-size: 15px;
            font-weight: 700;
        }

        QLabel#FeatureSubtitle {
            color: rgba(255, 255, 255, 199);
            font-size: 13px;
        }

        QLabel#FooterText {
            color: rgba(255, 255, 255, 184);
            font-size: 13px;
        }

        QLabel#WatermarkText {
            color: #8A8499;
            font-size: 12px;
            font-weight: 600;
            padding-top: 2px;
        }

        QFrame#ToolCard {
            background: rgba(255, 255, 255, 224);
            border: 1px solid rgba(255, 255, 255, 189);
            border-radius: 28px;
        }

        QLabel#CardHeading {
            color: #2E2E35;
            font-size: 30px;
            font-weight: 800;
        }

        QLabel#CardCaption {
            color: #6E6A7C;
            font-size: 14px;
        }

        QFrame#SegmentControl {
            background: #F0EEF9;
            border: 1px solid #E0DCF2;
            border-radius: 16px;
            min-height: 50px;
        }

        QPushButton#SegmentButton {
            background: transparent;
            border: none;
            border-radius: 12px;
            color: #6E6A7C;
            font-size: 14px;
            font-weight: 700;
            min-height: 38px;
            padding: 0 18px;
            text-align: center;
        }

        QPushButton#SegmentButton:checked {
            background: #FFFFFF;
            color: #5636E8;
            border: 1px solid #E8E4F7;
        }

        QLabel#FieldLabel {
            color: #2E2E35;
            font-size: 13px;
            font-weight: 700;
        }

        QLineEdit {
            background: #FFFFFF;
            border: 1px solid #E4E2EC;
            border-radius: 14px;
            color: #2E2E35;
            min-height: 24px;
            padding: 9px 14px;
            selection-background-color: #A99DDC;
        }

        QLineEdit:focus {
            border: 1px solid #6A50C6;
        }

        QPushButton#SecondaryButton {
            background: #F7F8FC;
            border: 1px solid #E1DEEA;
            border-radius: 14px;
            color: #5636E8;
            font-weight: 700;
            min-height: 24px;
            padding: 9px 16px;
        }

        QPushButton#SecondaryButton:hover {
            background: #F0EEF9;
        }

        QPushButton#PrimaryButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #6A50C6, stop:1 #5636E8);
            border: none;
            border-radius: 16px;
            color: #FFFFFF;
            font-size: 15px;
            font-weight: 800;
            padding: 13px 18px;
        }

        QPushButton#PrimaryButton:hover {
            background: #5636E8;
        }

        QLabel#StatusInfo {
            background: #F7F8FC;
            border: 1px solid #E7E5EF;
            border-radius: 14px;
            color: #6E6A7C;
            padding: 11px 13px;
        }

        QLabel#StatusSuccess {
            background: #F1FBF6;
            border: 1px solid #BDE8CF;
            border-radius: 14px;
            color: #24744A;
            padding: 11px 13px;
        }

        QLabel#StatusError {
            background: #FFF5F5;
            border: 1px solid #F0C2C2;
            border-radius: 14px;
            color: #A33A3A;
            padding: 11px 13px;
        }

        QMessageBox {
            background: #F7F8FC;
        }

        QMessageBox QLabel {
            color: #2E2E35;
        }

        QMessageBox QPushButton {
            background: #6A50C6;
            border: none;
            border-radius: 10px;
            color: #FFFFFF;
            min-width: 82px;
            padding: 9px 14px;
            font-weight: 700;
        }
    )");
}

void MainWindow::setEncryptMode()
{
    currentMode = Mode::Encrypt;
    updateMode();
}

void MainWindow::setDecryptMode()
{
    currentMode = Mode::Decrypt;
    updateMode();
}

void MainWindow::updateMode()
{
    const bool encryptMode = currentMode == Mode::Encrypt;

    encryptModeButton->setChecked(encryptMode);
    decryptModeButton->setChecked(!encryptMode);
    confirmPasswordLabel->setVisible(encryptMode);
    confirmPasswordEdit->setVisible(encryptMode);
    actionButton->setText(encryptMode ? "Encrypt File" : "Decrypt File");
    statusLabel->setText(encryptMode ? "Ready to encrypt a file." : "Ready to decrypt an encrypted file.");
    statusLabel->setObjectName("StatusInfo");
    statusLabel->style()->unpolish(statusLabel);
    statusLabel->style()->polish(statusLabel);
}

void MainWindow::browseInputFile()
{
    const QString filePath = QFileDialog::getOpenFileName(this, "Choose Input File");
    if (!filePath.isEmpty())
        inputPathEdit->setText(filePath);
}

void MainWindow::chooseOutputFile()
{
    const QString title = currentMode == Mode::Encrypt
        ? "Save Encrypted File"
        : "Save Decrypted File";
    const QString suggested = currentMode == Mode::Encrypt
        ? "encrypted.enc"
        : "decrypted_file";

    const QString filePath = QFileDialog::getSaveFileName(this, title, suggested);
    if (!filePath.isEmpty())
        outputPathEdit->setText(filePath);
}

void MainWindow::togglePasswordVisibility()
{
    const bool showing = passwordEdit->echoMode() == QLineEdit::Normal;
    const QLineEdit::EchoMode nextMode = showing ? QLineEdit::Password : QLineEdit::Normal;

    passwordEdit->setEchoMode(nextMode);
    confirmPasswordEdit->setEchoMode(nextMode);
    passwordToggleButton->setText(showing ? "Show" : "Hide");
}

void MainWindow::runFileOperation()
{
    QString validationMessage;
    if (!validateForm(&validationMessage)) {
        showStatus(validationMessage, false);
        QMessageBox::warning(this, "Check File Details", validationMessage);
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

    const std::string inputPath = toStdPath(inputPathEdit->text());
    const std::string outputPath = toStdPath(outputPathEdit->text());
    const std::string password = passwordEdit->text().toStdString();

    const bool success = currentMode == Mode::Encrypt
        ? encryptFile(inputPath, outputPath, password)
        : decryptFile(inputPath, outputPath, password);

    QApplication::restoreOverrideCursor();

    if (success) {
        const QString message = currentMode == Mode::Encrypt
            ? "Your file was encrypted successfully."
            : "Your file was decrypted successfully.";
        showStatus(message, true);
        QMessageBox::information(this, "Operation Complete", message);
    } else {
        const QString message = currentMode == Mode::Encrypt
            ? "Encryption failed. Check the input file and output location."
            : "Decryption failed. Check the file format, password, and output location.";
        showStatus(message, false);
        QMessageBox::critical(this, "Operation Failed", message);
    }
}

bool MainWindow::validateForm(QString *message) const
{
    const QString inputPath = inputPathEdit->text().trimmed();
    const QString outputPath = outputPathEdit->text().trimmed();
    const QString password = passwordEdit->text();

    if (inputPath.isEmpty()) {
        *message = "Please choose an input file.";
        return false;
    }

    if (outputPath.isEmpty()) {
        *message = "Please choose an output file path.";
        return false;
    }

    if (password.isEmpty()) {
        *message = "Password cannot be empty.";
        return false;
    }

    QFileInfo inputInfo(inputPath);
    if (!inputInfo.exists() || !inputInfo.isFile()) {
        *message = "The selected input file does not exist.";
        return false;
    }

    if (currentMode == Mode::Encrypt && inputInfo.size() == 0) {
        *message = "Empty files cannot be encrypted.";
        return false;
    }

    if (currentMode == Mode::Encrypt && password != confirmPasswordEdit->text()) {
        *message = "Passwords do not match.";
        return false;
    }

    return true;
}

void MainWindow::showStatus(const QString &message, bool success)
{
    statusLabel->setText(message);
    statusLabel->setObjectName(success ? "StatusSuccess" : "StatusError");
    statusLabel->style()->unpolish(statusLabel);
    statusLabel->style()->polish(statusLabel);
}

std::string MainWindow::toStdPath(const QString &path) const
{
    return path.toLocal8Bit().constData();
}
