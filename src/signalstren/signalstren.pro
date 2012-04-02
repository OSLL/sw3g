######################################################################
# Automatically generated by qmake (2.01a) Tue Sep 13 01:03:33 2011
######################################################################

maemo5 {
  CONFIG += mobility12
} else {
  CONFIG += mobility
}
MOBILITY = systeminfo
TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += bat.h \
    util.h \
    core/scanner.h \
    core/scanner.h \
    core/network.h \
    impl/umts/umts_scanner.h \
    impl/wlan/wlan_scanner.h \
    core/net_info.h \
    core/parameter.h \
    core/unit.h \
    core/measurer.h \
    impl/umts/umts_measurer.h \
    core/series.h \
    impl/wlan/wlan_measurer.h \
    core/evaluator.h \
    impl/eval/single/signal_strength_evaluator.h \
    impl/eval/multi/nff_eval.h \
    core/registry.h \
    impl/umts/reg.h \
    impl/wlan/reg.h \
    impl/config.h \
    core/registries.h \
    impl/eval/single/reg.h \
    impl/unit/signal.h \
    impl/param/signal.h \
    impl/param/reg.h
SOURCES += bat.cpp main.cpp

CONFIG(debug, debug|release){
    DESTDIR = bin/debug
    OBJECTS_DIR = bin/debug
    TARGET=signalstren
}

CONFIG(release, debug|release){
    DESTDIR = bin/release
    OBJECTS_DIR = bin/release
    TARGET=signalstren
}

### WLAN detection and parameter measurement scripts
scripts.path = $${DESTDIR}/script/
scripts.files = script/*.sh

INSTALLS += scripts

### Protocol Buffers
PB_FILES += core/persist/*.proto

# generate headers and sources for persistence
pb.output = ${QMAKE_FILE_IN_PATH}/${QMAKE_FILE_IN_BASE}.pb.h
pb.input = PB_FILES
pb.commands = protoc --cpp_out . ${QMAKE_FILE_IN}
pb.clean_commands = rm ${QMAKE_FILE_IN_PATH}/${QMAKE_FILE_IN_BASE}.pb.h
pb.variable_out = HEADERS
pb.name = PB
QMAKE_EXTRA_COMPILERS += pb

# dummy action to add generated sources to makefile
pb_dummy.output = ${QMAKE_FILE_IN_PATH}/${QMAKE_FILE_IN_BASE}.pb.cc
pb_dummy.input = PB_FILES
pb_dummy.commands = echo ${QMAKE_FILE_IN}
pb_dummy.clean_commands = rm ${QMAKE_FILE_IN_PATH}/${QMAKE_FILE_IN_BASE}.pb.cc
pb_dummy.variable_out = SOURCES
pb_dummy.name = PB_DUMMY
QMAKE_EXTRA_COMPILERS += pb_dummy

LIBS += -lprotobuf-lite

### List of all non-C++ files
OTHER_FILES += \
    script/wlan_measure.sh \
    script/wlan_scan.sh \
    script/wlan_active_net.sh \
    core/persist/net_info.proto
