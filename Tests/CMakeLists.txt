include_directories(.)
include_directories(../QtEventBus)

add_executable(qt_event_bus_test
    main.cpp
    qtestmessage.h
    sample.cpp
)

add_dependencies(qt_event_bus_test qt_event_bus)

target_link_libraries(qt_event_bus_test qt_event_bus qt_composition qtpromise)

# remember to run windeployqt
