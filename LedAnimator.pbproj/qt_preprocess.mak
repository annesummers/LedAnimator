#############################################################################
# Makefile for building: LedAnimator.app/Contents/MacOS/LedAnimator
# Generated by qmake (2.01a) (Qt 4.8.3) on: Thu Dec 20 22:15:41 2012
# Project:  LedAnimator.pro
# Template: app
# Command: /usr/bin/qmake -o LedAnimator.pbproj/project.pbxproj LedAnimator.pro
#############################################################################

MOC       = /Developer/Tools/Qt/moc
UIC       = /Developer/Tools/Qt/uic
LEX       = flex
LEXFLAGS  = 
YACC      = yacc
YACCFLAGS = -d
DEFINES       = -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
INCPATH       = -I/usr/local/Qt4.8/mkspecs/macx-xcode -I. -I/Library/Frameworks/QtCore.framework/Versions/4/Headers -I/usr/include/QtCore -I/Library/Frameworks/QtGui.framework/Versions/4/Headers -I/usr/include/QtGui -I/usr/include -I. -I/usr/local/include -I/System/Library/Frameworks/CarbonCore.framework/Headers -F/Library/Frameworks
DEL_FILE  = rm -f
MOVE      = mv -f

IMAGES = 
PARSERS =
preprocess: $(PARSERS) compilers
clean preprocess_clean: parser_clean compiler_clean

parser_clean:
check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compilers: ./moc_mainwindow.cpp ./moc_led.cpp ./moc_engine.cpp\
	 ./moc_animation.cpp ./moc_frame.cpp ./moc_ledgridwidget.cpp\
	 ./moc_playinfowidget.cpp ./moc_ledanimcodec.cpp ./moc_framewidgets.cpp\
	 ./moc_leddetailswidgets.cpp ./moc_ledwidgets.cpp ./moc_animationdetailswidgets.cpp
compiler_objective_c_make_all:
compiler_objective_c_clean:
compiler_moc_header_make_all: moc_mainwindow.cpp moc_led.cpp moc_engine.cpp moc_animation.cpp moc_frame.cpp moc_ledgridwidget.cpp moc_playinfowidget.cpp moc_ledanimcodec.cpp moc_framewidgets.cpp moc_leddetailswidgets.cpp moc_ledwidgets.cpp moc_animationdetailswidgets.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_mainwindow.cpp moc_led.cpp moc_engine.cpp moc_animation.cpp moc_frame.cpp moc_ledgridwidget.cpp moc_playinfowidget.cpp moc_ledanimcodec.cpp moc_framewidgets.cpp moc_leddetailswidgets.cpp moc_ledwidgets.cpp moc_animationdetailswidgets.cpp
moc_mainwindow.cpp: mainwindow.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ mainwindow.h -o moc_mainwindow.cpp

moc_led.cpp: led.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ led.h -o moc_led.cpp

moc_engine.cpp: engine.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ engine.h -o moc_engine.cpp

moc_animation.cpp: engine.h \
		animation.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ animation.h -o moc_animation.cpp

moc_frame.cpp: frame.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ frame.h -o moc_frame.cpp

moc_ledgridwidget.cpp: ledwidgets.h \
		led.h \
		ledgridwidget.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ ledgridwidget.h -o moc_ledgridwidget.cpp

moc_playinfowidget.cpp: playinfowidget.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ playinfowidget.h -o moc_playinfowidget.cpp

moc_ledanimcodec.cpp: ledanimcodec.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ ledanimcodec.h -o moc_ledanimcodec.cpp

moc_framewidgets.cpp: framewidgets.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ framewidgets.h -o moc_framewidgets.cpp

moc_leddetailswidgets.cpp: ledwidgets.h \
		led.h \
		leddetailswidgets.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ leddetailswidgets.h -o moc_leddetailswidgets.cpp

moc_ledwidgets.cpp: led.h \
		ledwidgets.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ ledwidgets.h -o moc_ledwidgets.cpp

moc_animationdetailswidgets.cpp: ledwidgets.h \
		led.h \
		animationdetailswidgets.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ animationdetailswidgets.h -o moc_animationdetailswidgets.cpp

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_rez_source_make_all:
compiler_rez_source_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean 
