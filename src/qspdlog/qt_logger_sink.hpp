//
// Created by 11851 on 2023/3/8.
//

#ifndef QSPDLOG_QT_LOGGER_SINK_HPP
#define QSPDLOG_QT_LOGGER_SINK_HPP

#include "spdlog/sinks/base_sink.h"
#include "qspdlog_model.hpp"

template<typename Mutex>
class qt_logger_sink : public spdlog::sinks::base_sink<Mutex> {
public:
    qt_logger_sink(QSpdLogModel *model) : m_model(model) {

    }

    void invalidate() { m_model = nullptr; }

protected:
    void sink_it_(const spdlog::details::log_msg &msg) override {
        if (!m_model)return;
        m_model->addEntry(
                {msg.time.time_since_epoch(), msg.level, fmt::to_string(msg.payload), fmt::to_string(msg.logger_name)});
    }

    void flush_()override{}

private:
    QSpdLogModel *m_model;
};

#include "mutex"
#include "spdlog/details/null_mutex.h"
using qt_logger_sink_mt=qt_logger_sink<std::mutex>;
using qt_logger_sink_st=qt_logger_sink<spdlog::details::null_mutex>;

#endif //QSPDLOG_QT_LOGGER_SINK_HPP
