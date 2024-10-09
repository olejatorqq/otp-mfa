#include "ui_add_account.h"
#include "add_account_dialog.h"

AddAccountDialog::AddAccountDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::AddAccountDialog) {
    ui->setupUi(this);

    // Подключаем сигнал нажатия кнопок к соответствующим слотам
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &AddAccountDialog::on_buttonBox_accepted);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &AddAccountDialog::reject);
}

AddAccountDialog::~AddAccountDialog() {
    delete ui;
}

Account AddAccountDialog::getAccount() const {
    return account;
}

void AddAccountDialog::on_buttonBox_accepted() {
    // Сохраняем данные, введённые пользователем
    account.name = ui->nameLineEdit->text();
    account.secret = ui->secretLineEdit->text();
    account.algorithm = ui->algorithmComboBox->currentText();

    // Если все поля заполнены, закрываем диалог с результатом accepted
    if (!account.name.isEmpty() && !account.secret.isEmpty()) {
        accept(); // Закрытие диалога с результатом QDialog::Accepted
    } else {
        // Если поля не заполнены, диалог не закрывается
        // (Можно добавить сообщение об ошибке или выделить незаполненные поля)
    }
}
