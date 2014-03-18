# Auto-generated by IDE. Any changes made by user will be lost!
BASEDIR =  $$quote($$_PRO_FILE_PWD_)

device {
    CONFIG(debug, debug|release) {
        SOURCES +=  $$quote($$BASEDIR/src/Globals.cpp) \
                 $$quote($$BASEDIR/src/ListFavoriteController.cpp) \
                 $$quote($$BASEDIR/src/LoginController.cpp) \
                 $$quote($$BASEDIR/src/Network/CookieJar.cpp) \
                 $$quote($$BASEDIR/src/Network/HFRNetworkAccessManager.cpp) \
                 $$quote($$BASEDIR/src/applicationui.cpp) \
                 $$quote($$BASEDIR/src/main.cpp)

        HEADERS +=  $$quote($$BASEDIR/src/DataObjects.h) \
                 $$quote($$BASEDIR/src/Globals.h) \
                 $$quote($$BASEDIR/src/ListFavoriteController.hpp) \
                 $$quote($$BASEDIR/src/LoginController.hpp) \
                 $$quote($$BASEDIR/src/Network/CookieJar.hpp) \
                 $$quote($$BASEDIR/src/Network/HFRNetworkAccessManager.hpp) \
                 $$quote($$BASEDIR/src/applicationui.hpp)
    }

    CONFIG(release, debug|release) {
        SOURCES +=  $$quote($$BASEDIR/src/Globals.cpp) \
                 $$quote($$BASEDIR/src/ListFavoriteController.cpp) \
                 $$quote($$BASEDIR/src/LoginController.cpp) \
                 $$quote($$BASEDIR/src/Network/CookieJar.cpp) \
                 $$quote($$BASEDIR/src/Network/HFRNetworkAccessManager.cpp) \
                 $$quote($$BASEDIR/src/applicationui.cpp) \
                 $$quote($$BASEDIR/src/main.cpp)

        HEADERS +=  $$quote($$BASEDIR/src/DataObjects.h) \
                 $$quote($$BASEDIR/src/Globals.h) \
                 $$quote($$BASEDIR/src/ListFavoriteController.hpp) \
                 $$quote($$BASEDIR/src/LoginController.hpp) \
                 $$quote($$BASEDIR/src/Network/CookieJar.hpp) \
                 $$quote($$BASEDIR/src/Network/HFRNetworkAccessManager.hpp) \
                 $$quote($$BASEDIR/src/applicationui.hpp)
    }
}

simulator {
    CONFIG(debug, debug|release) {
        SOURCES +=  $$quote($$BASEDIR/src/Globals.cpp) \
                 $$quote($$BASEDIR/src/ListFavoriteController.cpp) \
                 $$quote($$BASEDIR/src/LoginController.cpp) \
                 $$quote($$BASEDIR/src/Network/CookieJar.cpp) \
                 $$quote($$BASEDIR/src/Network/HFRNetworkAccessManager.cpp) \
                 $$quote($$BASEDIR/src/applicationui.cpp) \
                 $$quote($$BASEDIR/src/main.cpp)

        HEADERS +=  $$quote($$BASEDIR/src/DataObjects.h) \
                 $$quote($$BASEDIR/src/Globals.h) \
                 $$quote($$BASEDIR/src/ListFavoriteController.hpp) \
                 $$quote($$BASEDIR/src/LoginController.hpp) \
                 $$quote($$BASEDIR/src/Network/CookieJar.hpp) \
                 $$quote($$BASEDIR/src/Network/HFRNetworkAccessManager.hpp) \
                 $$quote($$BASEDIR/src/applicationui.hpp)
    }
}

INCLUDEPATH +=  $$quote($$BASEDIR/src/Network) \
         $$quote($$BASEDIR/src)

CONFIG += precompile_header

PRECOMPILED_HEADER =  $$quote($$BASEDIR/precompiled.h)

lupdate_inclusion {
    SOURCES +=  $$quote($$BASEDIR/../src/*.c) \
             $$quote($$BASEDIR/../src/*.c++) \
             $$quote($$BASEDIR/../src/*.cc) \
             $$quote($$BASEDIR/../src/*.cpp) \
             $$quote($$BASEDIR/../src/*.cxx) \
             $$quote($$BASEDIR/../src/Helper/*.c) \
             $$quote($$BASEDIR/../src/Helper/*.c++) \
             $$quote($$BASEDIR/../src/Helper/*.cc) \
             $$quote($$BASEDIR/../src/Helper/*.cpp) \
             $$quote($$BASEDIR/../src/Helper/*.cxx) \
             $$quote($$BASEDIR/../src/Network/*.c) \
             $$quote($$BASEDIR/../src/Network/*.c++) \
             $$quote($$BASEDIR/../src/Network/*.cc) \
             $$quote($$BASEDIR/../src/Network/*.cpp) \
             $$quote($$BASEDIR/../src/Network/*.cxx) \
             $$quote($$BASEDIR/../assets/*.qml) \
             $$quote($$BASEDIR/../assets/*.js) \
             $$quote($$BASEDIR/../assets/*.qs) \
             $$quote($$BASEDIR/../assets/images/*.qml) \
             $$quote($$BASEDIR/../assets/images/*.js) \
             $$quote($$BASEDIR/../assets/images/*.qs) \
             $$quote($$BASEDIR/../assets/model/*.qml) \
             $$quote($$BASEDIR/../assets/model/*.js) \
             $$quote($$BASEDIR/../assets/model/*.qs)

    HEADERS +=  $$quote($$BASEDIR/../src/*.h) \
             $$quote($$BASEDIR/../src/*.h++) \
             $$quote($$BASEDIR/../src/*.hh) \
             $$quote($$BASEDIR/../src/*.hpp) \
             $$quote($$BASEDIR/../src/*.hxx)
}

TRANSLATIONS =  $$quote($${TARGET}.ts)
