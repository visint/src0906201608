AUTOMAKE_OPTIONS = foreign
lib_LIBRARIES = libinfoset.a

libinfoset_a_SOURCES = ../unit/unitutil/unit.c ../unit/otu/otu.c ../unit/tap/tap.c \
../unit/olp/olp.c ../unit/ddm/ddm.c ../unit/edfa/edfa.c ../unit/etu/etu.c \
../unit/passive/passive.c ../unit/dvoa/dvoa.c ../unit/chassis/chassis.c\
../visipc/ipcutil/sigmsg.c ../visipc/ipcutil/shmutil.c ../visipc/ipcutil/mbusshm.c \
../sys/sysutil/sysinfo.c  ../visstr/visstr.c \
../utils/iniconf.c ../utils/shutils.c ../infoset/infoset.c ../infoset/setinfoset.c ../alarm/alarm.c
#LIBS= ../../visstr/libvisstr.a
#libutils_a_LDFLAGS = -version-info 0:0:0
INCLUDES=-I$(CURRENTPATH)./../include
