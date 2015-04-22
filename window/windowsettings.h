#ifndef WINDOWSETTINGS_H
#define WINDOWSETTINGS_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class WindowSettings;
}

class WindowSettings : public QDialog
{
    Q_OBJECT
    
public:
    explicit WindowSettings(QWidget *parent = 0);
    ~WindowSettings();

    void closeEvent ( QCloseEvent * event );
    void accept();

    bool Errors;
    
private slots:
    void on_NoLimitsRootDir_textEdited(const QString &arg1);

    void on_pushButton_clicked();

private:
    Ui::WindowSettings *ui;
};

#endif // WINDOWSETTINGS_H
