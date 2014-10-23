APP_NAME = HFR10

CONFIG += qt warn_on cascades10 
QT += network xml
LIBS += -lbbsystem -lbbpim -lbb -lbbdata -lbbnetwork -lunifieddatasourcec -lbbplatform

include(config.pri)
