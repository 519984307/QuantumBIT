#pragma once

#include "QWidget"

namespace spdlog {
    class logger;
    namespace sinks {
        class sink;
    }
}
class QAbstractSpdLogToolBar;

class QMenu;

class QSpdLogModel;

class QSpdLogProxyModel;

class QTreeView;

enum class AutoScrollPolicy {
    AutoScrollPolicyDisabled = 0,// never scroll to the bottom, leave the scrollbar where it was
    AutoScrollPolicyEnabled = 1,// always scroll to the bottom, when new rows are inserted
    AutoScrollPolicyEnabledBottom = 2 // scroll to the bottom only if the scroll bar was at the bottom before inserting the new ones
};

class QSpdLog:public QWidget{
    Q_OBJECT
public:
    explicit QSpdLog(QWidget* parent= nullptr);

    ~QSpdLog();

public:
    /*clear the content of the model, the toolbar
     * The method will clear up all the cached messages
     * There is no way after this to restore them*/
    void clear();

    /*Register a toolbar
     * The toolbar will be setup for the current instance
     * Being set up means:
     *  all the actions from the toolbar will affect current instance
     *  the changes to one toolbar will be reflected in the other ones as well*/
    void registerToolbar(QAbstractSpdLogToolBar* toolbar);

    /*Remove a toolbar
     * The toolbar will be removed from the current instance
     * The toolbar will not be deleted*/
    void removeToolbar(QAbstractSpdLogToolBar* toolbar);

    /*get the sink
     * The sink should be used by the user to all it into any logger
     * whose output the user want's to see in the widget*/
    std::shared_ptr<spdlog::sinks::sink> sink();

    /*get the number of items in the widget*/
    std::size_t itemsCount()const;

    void setMaxEntries(std::optional<std::size_t> maxEntries);

    /*get the maximum number of items in the widget*/
    std::optional<std::size_t> getMaxEntries()const;

    /*set the foreground QBrush for the messages of the corresponding logger
     * loggerName: the name of the logger of which to get the foreground brush from brush*/
    void setLoggerForeground(std::string_view loggerName, std::optional<QColor> brush);

    /*get the foreground QBrush for the message of the corresponding logger*/
    std::optional<QColor> getLoggerForeground(std::string_view loggerName)const;

     /*set the background QBrush for the messages of the corresponding logger*/
     void setLoggerBackground(std::string_view, std::optional<QBrush>);

     /*get the background QBrush for the messages of the corresponding logger*/
     std::optional<QBrush> getLoggerBackground(std::string_view)const;

     /*set the policy of auto-scrolling feature*/
     void setAutoScrollPolicy(AutoScrollPolicy);

private slots:
    void filterData(const QString& text, bool isRegularExpression, bool isCaseSensitive);
    void updateAutoScrollPolicy(int index);
private:
    QSpdLogModel* m_sourceModel;
    QSpdLogProxyModel* m_proxyModel;
    QTreeView* m_view;
    bool m_scrollsAtBottom;
    QMetaObject::Connection m_scrollConnection;
    std::shared_ptr<spdlog::sinks::sink> m_sink;
    std::list<QAbstractSpdLogToolBar*> m_toolbars;
};
