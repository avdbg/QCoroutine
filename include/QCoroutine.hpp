#pragma once

#include <QObject>
#include <QTimer>
#include <coroutine.h>

class QCoroutine : public QObject
{
    //Q_OBJECT

private:
    QCoroutine(std::function<void()> f)
        : QObject(nullptr)
    {
        auto deleter = [f, this] {
            f();
            this->deleteLater();
        };

        m_rt = coroutine::create(deleter);
        coroutine::resume(m_rt);
    }
public:
    static void run(std::function<void()> f)
    {
        new QCoroutine(std::move(f));
    }

    ~QCoroutine()
    {
        coroutine::destroy(m_rt);
    }

    static void qYield()
    {
        const auto routine = coroutine::current();
        QTimer::singleShot(0, std::bind(&coroutine::resume, routine));
        coroutine::yield();
    }

    template <typename Sender, typename Func>
    static void qAwait(const Sender* sender, Func signal)
    {
        const auto routine = coroutine::current();
        const auto connection = QObject::connect(sender, signal, std::bind(&coroutine::resume, routine));
        coroutine::yield();
        QObject::disconnect(connection);
    }
private:
    coroutine::routine_t m_rt;
};
