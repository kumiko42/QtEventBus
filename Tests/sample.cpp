#include <qeventbus.h>
#include <qsubscriber.h>

class TestReceiver : public QObject
{
public:
    QString test(int i, QString s)
    {
        qDebug() << "TestReceiver" << i << s;
        return s + QString::number(i);
    }
};

struct TestMessage
{
    int i;
    int j;
};

Q_MESSAGE_META_RESULT(TestMessage, false, false, "test", int)

struct VoidMessage
{
    int i;
    int j;
};

Q_MESSAGE_META(VoidMessage, false, false, "void")

void test()
{
    QEventBus bus;
    // SimpleMessage
    TestReceiver receiver;
    //
    //添加订阅主题：test_topic； 接收者：receiver 接受函数： QString(int, QString)
    bus.subscribe("test_topic", QSubscriber(&receiver, &TestReceiver::test));

    bus.unsubscribe("test_topic", QSubscriber(&receiver, &TestReceiver::test));


    /**
     * TODO
     * 鉴于这里无法 unsubscribe，我怀疑这个作者写完了感觉还行就放那里了，对于各种小bug并没有认真考察
     * 如果要自己实现一个QEventBus，还需要额外的时间
     * 因此暂时先放在这里，等到有机会了再说
     */

    //向test_topic发布消息（QVariant自动转换）："1", "2"
    bus .publish("test_topic", QStringList({"1", "2"}))
    //得到其QString的结果
        .then([](QVector<QVariant> const & result) {
                qDebug() << "SimpleMessage public result:" << result;
                qDebug() << QString::fromStdWString(L"receiver的test方法接受来自test_topic主题的消息");
            }, [](std::exception & e) {
                qDebug() << e.what();
            });

    // 自动类型转换器 TestMessange -> QVariantList
    QMetaType::registerConverter<TestMessage, QVariantList>([](auto & m) {
        return QVariantList{m.i, m.j};
    });

    // QVariantList 到 TestMessage
    QMetaType::registerConverter<QVariantList, TestMessage>([](auto & l) {
        return TestMessage{l[0].toInt(), l[1].toInt()};
    });

    // 订阅一个单独类型TestMessage结构 发送到匿名函数 返回值为其i + j
    bus.subscribe<TestMessage>([] (auto msg) {
        return QtPromise::resolve(msg.i + msg.j);
    }); //这个确实返回3

    // 订阅到test主题 仍然有receiver的test处理
    bus.subscribe("test", QSubscriber(&receiver, &TestReceiver::test)); //这个返回21

    // 订阅到匿名函数
    bus.subscribe("test", [](auto topic, auto message) {
        qDebug() << "test" << topic << message.toList();
        qDebug() << QString::fromStdWString(L"这里是匿名函数!");
    }); //这个返回0




    bus .publish(TestMessage{1, 2}) // -> QtPromise::resolve(3)
        .then([](QVector<int> const & result) {
            qDebug() << QString::fromStdWString(L"TestMessage public result:") << result;
        },[](std::exception & e) {
            qDebug() << e.what();
        });

    // 发送到test主题 然后得出结果
    bus .publish("test", QVariantList{1, 2})
        .then([](QVector<QVariant> const & result) {
                qDebug() << "TestMessage variant public result:" << result;
            }, [](std::exception & e) {
                qDebug() << e.what();
        });




    // VoidMessage (void)
    QMetaType::registerConverter<VoidMessage, QVariantList>([](auto & m) {
        return QVariantList{m.i, m.j};
    });

    QMetaType::registerConverter<QVariantList, VoidMessage>([](auto & l) {
        return VoidMessage{l[0].toInt(), l[1].toInt()};
    });


    bus.subscribe<VoidMessage>([] (auto) {
        return QtPromise::resolve();
    });

    bus.subscribe("void", QSubscriber(&receiver, &TestReceiver::test));


     bus.publish(VoidMessage{2, 3}).then([]() {
         qDebug() << "VoidMessage public result";
     }, [](std::exception & e) {
         qDebug() << e.what();
     });

    bus.publish("void", QVariantList{2, 3}).then([](QVector<QVariant> const & result) {
        qDebug() << "VoidMessage variant public result:" << result;
    }, [](std::exception & e) {
        qDebug() << e.what();
    });
}
