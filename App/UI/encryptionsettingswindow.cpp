#include "encryptionsettingswindow.h"
#include "ui_encryptionsettingswindow.h"
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include "constants.h"

EncryptionSettingsWindow::EncryptionSettingsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EncryptionSettingsWindow)
{
    ui->setupUi(this);

    if (QFile::exists("encryptionStatus.ini")) {
        QFile file("encryptionStatus.ini");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            for(auto line : file.readAll().split('\n'))
            {
                if(line.size() == 0) continue;
                if(line.chopped(2) == "ShowHashes")
                {
                    if(line.back() == '1')
                    {
                        ui->checkBoxShowNamesAsHashes->setChecked(true);
                    }
                } else if(line.chopped(2) == "LocalDataHashed")
                {
                    if(line.back() == '1')
                    {
                        ui->checkBoxHashStoredData->setChecked(true);
                        ui->checkBoxShowNamesAsHashes->setChecked(true);
                        // Disable buttons, because only redownloading data works
                        ui->checkBoxHashStoredData->setEnabled(false);
                        ui->checkBoxEncryptStoredData->setEnabled(false);
                        ui->checkBoxShowNamesAsHashes->setEnabled(false);
                        break;
                    }
                } else if(line.chopped(2) == "LocalDataEncrypted")
                {
                    if(line.back() == '1')
                    {
                        ui->checkBoxEncryptStoredData->setChecked(true);
                    }
                }
            }
        }
    } else
    { // Create settings file
        QFile file("encryptionStatus.ini");
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            file.write("ShowHashes=0\n"
                       "LocalDataHashed=0\n"
                       "LocalDataEncrypted=0");
        }
        file.close();
    }
}

EncryptionSettingsWindow::~EncryptionSettingsWindow()
{
    delete ui;
}

void EncryptionSettingsWindow::on_checkBoxHashStoredData_clicked()
{
    QMessageBox areYouSure;

    areYouSure.setWindowTitle("Are you sure?");
    areYouSure.setText("Are you sure?\n"
                           "This can not be undone without\n"
                           "redownloading all data.");

    areYouSure.setStandardButtons(QMessageBox::Yes);
    areYouSure.addButton(QMessageBox::No);
    areYouSure.setDefaultButton(QMessageBox::No);

    if(areYouSure.exec() == QMessageBox::Yes)
    {
        ui->checkBoxHashStoredData->setEnabled(false);
        ui->checkBoxEncryptStoredData->setEnabled(false);
        ui->checkBoxShowNamesAsHashes->setEnabled(false);
        ui->checkBoxShowNamesAsHashes->setChecked(true);
        ui->checkBoxEncryptStoredData->setChecked(false);
        if(ui->checkBoxEncryptStoredData->isChecked())
        {
            // Decrypt local data before hashing
            m_crypter.decryptLocalNames();
            editSetting("LocalDataEncrypted", "0");
        }
        m_crypter.hashLocalNames();
        editSetting("LocalDataHashed", "1");
    } else
    {
        ui->checkBoxHashStoredData->setChecked(false);
    }
}

void EncryptionSettingsWindow::on_checkBoxEncryptStoredData_stateChanged(int newState)
{
    if(newState)
    {
        m_crypter.encryptLocalNames();

        // Mark data as encrypted
        editSetting("LocalDataEncrypted", "1");

    } else
    {
        m_crypter.decryptLocalNames();

        // Mark data as unencrypted
        editSetting("LocalDataEncrypted", "0");
    }
}

void EncryptionSettingsWindow::on_checkBoxShowNamesAsHashes_stateChanged(int newState)
{
    if(newState)
    {
        editSetting("ShowHashes", "1");
    } else
    {
        editSetting("ShowHashes", "0");
    }
}

void EncryptionSettingsWindow::editSetting(QString setting, QString value)
{
    QFile file("encryptionStatus.ini");
    if (file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        auto all = file.readAll();
        QByteArray newData;
        for(auto line : all.split('\n'))
        {
            if(line.size() == 0) continue;
            if(line.chopped(2) == setting)
            {
                newData.append((setting + "=" + value + '\n').toUtf8());
            } else
            {
                newData.append(line + '\n');
            }
        }
        file.seek(0);
        file.write(newData);
        file.close();
    }
}

void EncryptionSettingsWindow::on_pushButtonDeleteStoredData_clicked()
{
    QMessageBox areYouSure;

    areYouSure.setWindowTitle("Are you sure?");
    areYouSure.setText("Are you sure you want to delete all locally stored data?\n"
                       "This can not be undone without redownloading all data.\n"
                       "This will also close the program.");

    areYouSure.setStandardButtons(QMessageBox::Yes);
    areYouSure.addButton(QMessageBox::No);
    areYouSure.setDefaultButton(QMessageBox::No);

    if(areYouSure.exec() == QMessageBox::Yes)
    {
        if(QDir(InternetExplorers::Constants::DATA_ROOT_NAME).exists())
        {
            QDir(InternetExplorers::Constants::DATA_ROOT_NAME).removeRecursively();
        }
        editSetting("ShowHashes", "0");
        editSetting("LocalDataHashed", "0");
        editSetting("LocalDataEncrypted", "0");
        ui->checkBoxHashStoredData->setEnabled(false);
        ui->checkBoxEncryptStoredData->setEnabled(false);
        ui->checkBoxShowNamesAsHashes->setEnabled(false);
        ui->checkBoxShowNamesAsHashes->setChecked(false);
        ui->checkBoxEncryptStoredData->setChecked(false);
        ui->checkBoxHashStoredData->setChecked(false);
        ui->pushButtonDeleteStoredData->setEnabled(false);
        emit closeProgram();
        this->close();
    }
}

void EncryptionSettingsWindow::on_pushButtonClose_clicked()
{
    this->close();
}
