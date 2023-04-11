//
// Created by 11851 on 2023/3/8.
//

#ifndef QSPDLOG_QSPDLOG_STYLE_DIALOG_HPP
#define QSPDLOG_QSPDLOG_STYLE_DIALOG_HPP
#include "QDialog"
#include "optional"
#include "string"

class QSpdLogModel;

class QSpdLogStyleDialog:public QDialog{
    Q_OBJECT
public:
    struct Style{
        std::string loggerName;
        std::optional<QColor> backgroundColor;
        std::optional<QColor> textColor;
    };

public:
    explicit QSpdLogStyleDialog(QWidget* parent= nullptr);
    ~QSpdLogStyleDialog() override;

    Style result()const;
    void setModel(const QSpdLogModel* model);

private:
    Style m_result;
    const QSpdLogModel* m_model;
};


#endif //QSPDLOG_QSPDLOG_STYLE_DIALOG_HPP
