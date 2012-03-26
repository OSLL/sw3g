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
    impl/units.h \
    impl/parameters.h \
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
    impl/eval/single/reg.h
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

scripts.path = $${DESTDIR}/script
scripts.files += script/getnetdata.sh
INSTALLS += scripts
