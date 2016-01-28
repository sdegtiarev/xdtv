

CXX= g++
CC= g++
CXXFLAGS= -g -std=c++11 -I$(HOME)/include \
	-I/usr/include/qt4 -I/usr/include/qwt -I/usr/include/qt4/Qt
LDFLAGS= -g -std=c++11
LDLIBS= -lqwt -lQtGui -lQtCore 
LEX= flex


Q_OBJ= dtv.o 
OBJ= main.o
MOC_OBJ=$(Q_OBJ:.o=.moc.o)
MOC_SRC=$(MOC_OBJ:.o=.cc)

	
all: dtv

dtv: $(OBJ) $(Q_OBJ) $(MOC_OBJ)

$(MOC_SRC): %.moc.cc: %.h
	moc $< >$@

	
clean:
	rm -rf dtv $(OBJ) $(Q_OBJ) $(MOC_OBJ) $(MOC_SRC) core*


