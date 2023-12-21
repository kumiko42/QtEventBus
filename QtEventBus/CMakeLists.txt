include_directories(.)

add_library(qt_event_bus SHARED
        qeventbus.cpp
        qeventbusqml.cpp
        qmessage.cpp
        qproperty.cpp
        qsubscriber.cpp
        QtEventBus_global.h
        qeventbus.h
        qeventbusqml.h
        qeventqueue.h
        qmessage.h
        qmessageresult.h
        qproperty.h
        qsubscriber.h
)

add_compile_definitions(QTEVENTBUS_LIBRARY)

target_link_libraries(qt_event_bus  ${QT_LIBS_WITH_PREFIX} qtpromise qt_composition)
