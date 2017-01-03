AUTOMAKE_OPTIONS = foreign
lib_LIBRARIES = libvispace.a

libvispace_a_SOURCES = ../unit/unitutil/unit.c ../unit/otu/otu.c ../unit/tap/tap.c \
../unit/olp/olp.c ../unit/ddm/ddm.c ../unit/edfa/edfa.c ../unit/etu/etu.c \
../unit/passive/passive.c ../unit/dvoa/dvoa.c ../unit/chassis/chassis.c\
../visipc/ipcutil/sigmsg.c ../visipc/ipcutil/shmutil.c ../visipc/ipcutil/mbusshm.c \
../sys/sysutil/sysinfo.c  ../sys/sysutil/sysdata.c ../sys/sysutil/snmputil.c ../sys/sysutil/logutil.c \
../visdb/dbutil/dbutil.c ../visdb/dbutil/almutil.c ../visstr/visstr.c \
../utils/iniconf.c ../utils/shutils.c ../utils/conv.c ../utils/appconf.c \
../infoset/infoset.c ../infoset/setinfoset.c \
../io/gpio.c ../io/ioutil.c ../alarm/alarm.c
#LIBS= ../../visstr/libvisstr.a
#libutils_a_LDFLAGS = -version-info 0:0:0
INCLUDES=-I$(CURRENTPATH)./../include
