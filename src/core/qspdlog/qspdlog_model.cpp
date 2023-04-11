//
// Created by 11851 on 2023/3/8.
//
#include "QBrush"
#include "QDateTime"
#include "QFile"
#include "QIcon"

#include "qspdlog_model.hpp"

namespace {
    static constexpr std::array<const char *, 6> icon_names = {
            ":/res/trace.png", ":/res/debug.png", ":/res/info.png",
            ":/res/warn.png", ":/res/error.png", ":/res/critical.png"
    };

    static constexpr std::array<const char *, 7> level_names = {
            "Trace", "Debug", "Info", "Warning", "Error", "Critical", "Off"
    };

    enum class Column {
        Level = 0,
        Thread,
        Logger,
        Time,
        Message,
        Last
    };

    static constexpr std::array<const char *, 5> column_names = {
            "Level", "Thread", "Logger", "Time", "Message"
    };
}

QSpdLogModel::QSpdLogModel(QObject *parent) : QAbstractListModel(parent) {}

void QSpdLogModel::addEntry(entry_t entry) {
    if (m_maxEntries > 0 && m_items.size() == m_maxEntries) {
        beginRemoveRows(QModelIndex(), 0, 0);
        m_items.pop_front();
        endRemoveRows();
    }

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_items.push_back(std::move(entry));
    endInsertRows();
}

void QSpdLogModel::setMaxEntries(std::optional<std::size_t> maxEntries) {
    m_maxEntries = maxEntries;
//    Incase the new maximum is below the current amount of items
    if (m_maxEntries > 0 && m_items.size() > m_maxEntries) {
        std::size_t offset = m_items.size() - m_maxEntries.value();
        beginRemoveRows(QModelIndex(), 0, offset - 1);
        m_items.erase(m_items.begin(), m_items.begin() + offset);
        endRemoveRows();
    }
}

std::optional<std::size_t> QSpdLogModel::getMaxEntries() const {
    return m_maxEntries;
}

void QSpdLogModel::clear() {
    beginResetModel();
    m_items.clear();
    endResetModel();
}

int QSpdLogModel::rowCount(const QModelIndex &parent) const {
    return m_items.size();
}

int QSpdLogModel::columnCount(const QModelIndex &parent) const {
    return static_cast<std::size_t >(Column::Last);
}

QVariant QSpdLogModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_items.size()) return QVariant();

    switch (role) {
        case Qt::DisplayRole: {
            auto const &item = m_items[index.row()];
            switch (static_cast<Column>(index.column())) {
                case Column::Level: {
                    return QString(level_names[item.level]);
                }
                case Column::Thread: {
                    return item.thread_id;
                }
                case Column::Logger: {
                    return QString::fromStdString(item.loggerName);
                }
                case Column::Time: {
                    return QDateTime::fromMSecsSinceEpoch(
                            std::chrono::duration_cast<std::chrono::milliseconds>(item.time).count());
                }
                case Column::Message: {
                    return QString::fromStdString(item.message);
                }
                default: {
                    break;
                }
            }
            break;
        }
        case Qt::DecorationRole: {
            if (index.column() == 0) {
                const auto &item = m_items[index.row()];
                if (item.level >= 0 && item.level < icon_names.size()) {
                    return QIcon(QString(icon_names[m_items[index.row()].level]));
                }
            }
            break;
        }
        case Qt::BackgroundRole: {
            std::string loggerName = m_items[index.row()].loggerName;
            if (m_backgroundMappings.count(loggerName))return m_backgroundMappings.at(loggerName);
            break;
        }
        case Qt::ForegroundRole: {
            std::string loggerName = m_items[index.row()].loggerName;
            if (m_foregroundMappings.count(loggerName))return m_foregroundMappings.at(loggerName);
            break;
        }
        default:
            break;
    }
    return QVariant();
}

QVariant QSpdLogModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) return QString(column_names[section]);
    return QVariant();
}

void QSpdLogModel::setLoggerForeground(std::string_view loggerName, std::optional<QColor> color) {
    int lastRow = rowCount() - 1;
    if (lastRow < 0) lastRow = 0;
    int lastColumn = columnCount() - 1;
    if (lastColumn < 0) lastColumn = 0;
    if (color.has_value()) {
        m_foregroundMappings[std::string(loggerName)] = color.value();
        emit dataChanged(this->index(0), this->index(lastRow, lastColumn), {Qt::ForegroundRole});
    } else if (m_foregroundMappings.count(std::string(loggerName))) {
        m_foregroundMappings.erase(std::string(loggerName));
        emit dataChanged(this->index(0), this->index(lastRow, lastColumn), {Qt::ForegroundRole});
    }
}

std::optional<QColor> QSpdLogModel::getLoggerForeground(std::string_view loggerName) const {
    if (m_foregroundMappings.count(std::string(loggerName)))return m_foregroundMappings.at(std::string(loggerName));
    return std::nullopt;
}

void QSpdLogModel::setLoggerBackground(std::string_view loggerName, std::optional<QBrush> brush) {
    int lastRow = rowCount() - 1;
    if (lastRow < 0) lastRow = 0;
    int lastColumn = columnCount() - 1;
    if (lastColumn < 0) lastColumn = 0;
    if (brush.has_value()) {
        m_backgroundMappings[std::string(loggerName)] = brush.value();
        emit dataChanged(this->index(0), this->index(lastRow, lastColumn), {Qt::BackgroundRole});
    } else if (m_backgroundMappings.count(std::string(loggerName))) {
        m_backgroundMappings.erase(std::string(loggerName));
        emit dataChanged(this->index(0), this->index(lastRow, lastColumn), {Qt::BackgroundRole});
    }
}

std::optional<QBrush> QSpdLogModel::getLoggerBackground(std::string_view loggerName) const {
    if (m_backgroundMappings.count(std::string(loggerName))) return m_backgroundMappings.at(std::string(loggerName));
    return std::nullopt;
}