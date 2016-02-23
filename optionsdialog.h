#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>

namespace Ui {
class OptionsDialog;
}

class OptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionsDialog(QWidget *parent = 0);
    ~OptionsDialog();

    static void setTableName(const QString &);
    static QString getTableName();
private slots:
    void save();
    void browse();
private:
    Ui::OptionsDialog *ui;
    static QString tableName;

    void load();
};

#endif // OPTIONSDIALOG_H
