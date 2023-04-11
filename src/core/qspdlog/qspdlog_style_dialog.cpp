//
// Created by 11851 on 2023/3/8.
//
#include "QBoxLayout"
#include "QDialogButtonBox"
#include "QLineEdit"

#include "qspdlog_model.hpp"
#include "qspdlog_style_dialog.hpp"

QSpdLogStyleDialog::QSpdLogStyleDialog(QWidget *parent) : QDialog(parent) {
    auto layout = new QVBoxLayout(this);
    auto loggerNameEdit = new QLineEdit(this);
    loggerNameEdit->setPlaceholderText("Logger name");
    loggerNameEdit->setObjectName("loggerNameEdit");
    auto backgroundColorEdit = new QLineEdit();
    backgroundColorEdit->setPlaceholderText("Background color");
    backgroundColorEdit->setObjectName("backgroundColorEdit");
    auto textColorEdit = new QLineEdit();
    textColorEdit->setPlaceholderText("Text color");
    textColorEdit->setObjectName("textColorEdit");

    layout->addWidget(loggerNameEdit);
    layout->addWidget(backgroundColorEdit);
    layout->addWidget(textColorEdit);

    auto buttonBox = new QDialogButtonBox(
            QDialogButtonBox::Ok | QDialogButtonBox::Cancel
    );
    layout->addWidget(buttonBox);
    buttonBox->setObjectName("buttonBox");

    connect(loggerNameEdit, &QLineEdit::textChanged, this,
            [this, backgroundColorEdit, textColorEdit](const QString &name) {
                std::string name_std_str = name.toStdString();
                auto bg = m_model->getLoggerBackground(name_std_str);
                auto fg = m_model->getLoggerForeground(name_std_str);

                if (bg) {
                    backgroundColorEdit->setText(bg.value().color().name());
                }

                if (fg) {
                    textColorEdit->setText(fg.value().name());
                }
            });

    connect(
            buttonBox, &QDialogButtonBox::accepted, this, [this, loggerNameEdit, backgroundColorEdit, textColorEdit]() {
                if (!loggerNameEdit->text().isEmpty()) {
                    reject();
                }

                m_result.loggerName = loggerNameEdit->text().toStdString();

                if (!backgroundColorEdit->text().isEmpty()){
                    m_result.backgroundColor=QColor(backgroundColorEdit->text());
                }
                else{
                    m_result.backgroundColor=std::nullopt;
                }

                if(!textColorEdit->text().isEmpty()){
                    m_result.textColor=QColor(textColorEdit->text());
                }
                else{
                    m_result.textColor=std::nullopt;
                }

                accept();
            }
    );

    connect(buttonBox, &QDialogButtonBox::rejected,this, [this](){
     reject();
    }
            );
}

QSpdLogStyleDialog::~QSpdLogStyleDialog()=default;

QSpdLogStyleDialog::Style QSpdLogStyleDialog::result() const {
    return m_result;
}

void QSpdLogStyleDialog::setModel(const QSpdLogModel *model) {
    m_model=model;
}