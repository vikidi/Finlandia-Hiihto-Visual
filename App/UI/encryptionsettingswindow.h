#ifndef ENCRYPTIONSETTINGSWINDOW_HH
#define ENCRYPTIONSETTINGSWINDOW_HH

#include <QWidget>
#include <QString>
#include "crypter.h"

namespace Ui {
class EncryptionSettingsWindow;
}

/*!
 * \brief
 * Handles GDBR reasoned data encryptions and decryptions.
 */
class EncryptionSettingsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit EncryptionSettingsWindow(QWidget *parent = nullptr);
    ~EncryptionSettingsWindow();

signals:
    /*!
     * \brief
     * Closes this window
     */
    void closeProgram();

private slots:
    void on_checkBoxHashStoredData_clicked();

    void on_checkBoxEncryptStoredData_stateChanged(int newState);

    void on_checkBoxShowNamesAsHashes_stateChanged(int newState);

    void on_pushButtonDeleteStoredData_clicked();

    void on_pushButtonClose_clicked();

private:
    Ui::EncryptionSettingsWindow *ui;
    InternetExplorers::Crypter m_crypter;

    void editSetting(QString setting, QString value);

    bool programStarted;
};

#endif // ENCRYPTIONSETTINGSWINDOW_HH
