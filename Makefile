# Variable definitions.

PLATFORM	= $(shell uname)
CC		= gcc-5
OBJDIR		= ./objs
TESTS		= ./tests
INCLUDES	= ./include
INCFLAGS	:= -I${INCLUDES} -I${TESTS}
#OPTFLAGS	:= -Os -DNDEBUG
OPTFLAGS	:= -g
CCFLAGS		:= -c ${OPTFLAGS} -Wall -std=c99

DEBUG := true

# The list of objects to include in the library
ifeq ($(DEBUG),true)
LIBEIOBJS	:= ./objs/freq_counter.o ./objs/ei_tools.o ./objs/ei_draw.o ./objs/ei_placer.o ./objs/ei_event.o ./objs/ei_widget.o ./objs/ei_widgetclass.o ./objs/ei_application.o ./objs/ei_frame.o ./objs/ei_button.o ./objs/ei_toplevel.o ./objs/ei_draw_polygon.o ./objs/ei_picking_list.o ./objs/ei_draw_button.o  ./objs/ei_draw_toplevel.o ./objs/ei_draw_content.o ./objs/ei_draw_text_lines.o ./objs/ei_intrsct_pile.o
CCFLAGS		:= ${CCFLAGS} -pg
else
LIBEIOBJS	:= ./objs/ei_draw.o ./objs/ei_placer.o ./objs/ei_event.o ./objs/ei_widget.o ./objs/ei_widgetclass.o ./objs/ei_application.o ./objs/ei_frame.o ./objs/ei_button.o ./objs/ei_toplevel.o ./objs/ei_tools.o ./objs/ei_draw_polygon.o ./objs/ei_picking_list.o ./objs/ei_draw_button.o  ./objs/ei_draw_toplevel.o ./objs/ei_draw_content.o ./objs/ei_draw_text_lines.o ./objs/ei_intrsct_pile.o
endif


# Platform specific definitions (OS X, Linux)

ifeq (${PLATFORM},Darwin)

	# Building for Mac OS X

	PLATDIR		= _osx
	INCFLAGS	:= ${INCFLAGS} -I/opt/local/include/SDL
	LINK		= ${CC}
	LIBEI		= ${OBJDIR}/libei.a
	LIBEIBASE	= ${PLATDIR}/libeibase.a
	LIBS		= ${LIBEIBASE} -L/opt/local/lib -lSDL -lSDL_ttf -lSDL_image -framework AppKit
	CCFLAGS		:= ${CCFLAGS} -D__OSX__

else

	# Building for Linux

	PLATDIR		= _x11
	INCFLAGS	:= ${INCFLAGS} -I/usr/include/SDL
	LINK		= ${CC}
#	ARCH	        = 32
	ARCH	        = 64
	LIBEI		= ${OBJDIR}/libei.a
	LIBEIBASE	= ${PLATDIR}/libeibase${ARCH}.a
	LIBS		= ${LIBEIBASE} -lSDL -lSDL_ttf -lSDL_image -lm
	CCFLAGS		:= ${CCFLAGS} -D__LINUX__ -m${ARCH}
	LDFLAGS		= -m${ARCH} -g

endif

ifeq ($(DEBUG),true)
LDFLAGS := -pg ${LDFLAGS}
endif

# Main target of the makefile. To build specific targets, call "make <target_name>"

TARGETS		=	${LIBEI} \
			minimal lines lines2 frame button hello_world top_level_ception puzzle two048 test_new_class
all : ${TARGETS}

########## Test-programs



# minimal

minimal : ${OBJDIR}/minimal.o ${LIBEIBASE}
	${LINK} -o minimal ${LDFLAGS} ${OBJDIR}/minimal.o ${LIBS}

${OBJDIR}/minimal.o : ${TESTS}/minimal.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/minimal.c -o ${OBJDIR}/minimal.o

# lines

lines : ${OBJDIR}/lines.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o lines ${OBJDIR}/lines.o ${LIBEI} ${LIBS}

${OBJDIR}/lines.o : ${TESTS}/lines.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/lines.c -o ${OBJDIR}/lines.o

lines2 : ${OBJDIR}/lines2.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o lines2 ${OBJDIR}/lines2.o ${LIBEI} ${LIBS}

${OBJDIR}/lines2.o : ${TESTS}/lines2.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/lines2.c -o ${OBJDIR}/lines2.o


# frame

frame : ${OBJDIR}/frame.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o frame ${LDFLAGS} ${OBJDIR}/frame.o ${LIBEI} ${LIBS}

${OBJDIR}/frame.o : ${TESTS}/frame.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/frame.c -o ${OBJDIR}/frame.o

# button

button : ${OBJDIR}/button.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o button ${LDFLAGS} ${OBJDIR}/button.o ${LIBEI} ${LIBS}

${OBJDIR}/button.o : ${TESTS}/button.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/button.c -o ${OBJDIR}/button.o

# hello_world

hello_world : ${OBJDIR}/hello_world.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o hello_world ${LDFLAGS} ${OBJDIR}/hello_world.o ${LIBEI} ${LIBS}

${OBJDIR}/hello_world.o : ${TESTS}/hello_world.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/hello_world.c -o ${OBJDIR}/hello_world.o

top_level_ception : ${OBJDIR}/top_level_ception.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o top_level_ception ${LDFLAGS} ${OBJDIR}/top_level_ception.o ${LIBEI} ${LIBS}

${OBJDIR}/top_level_ception.o : ${TESTS}/top_level_ception.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/top_level_ception.c -o ${OBJDIR}/top_level_ception.o


# puzzle

puzzle : ${OBJDIR}/puzzle.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o puzzle ${LDFLAGS} ${OBJDIR}/puzzle.o ${LIBEI} ${LIBS}

${OBJDIR}/puzzle.o : ${TESTS}/puzzle.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/puzzle.c -o ${OBJDIR}/puzzle.o


# two048

two048 : ${OBJDIR}/two048.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o two048 ${LDFLAGS} ${OBJDIR}/two048.o ${LIBEI} ${LIBS}

${OBJDIR}/two048.o : ${TESTS}/two048.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/two048.c -o ${OBJDIR}/two048.o

# test_new_class
test_new_class : all
	${LINK} -o test_ext_class -I./include -I/usr/include/SDL ${TESTS}/test_ext_class.c misc/ext_testclass.o ${OBJDIR}/libei.a _x11/libeibase64.a -L_x11 -lSDL -lSDL_ttf -lSDL_image -lm



# VERY IMPORTANT TO DEFAULT COMPILE .o
$(OBJDIR)/%.o: ./src/%.c
	${CC} ${CCFLAGS} ${INCFLAGS} $< -o $@

# Building of the library libei

${LIBEI} : ${LIBEIOBJS}
	ar rcs ${LIBEI} ${LIBEIOBJS}



# Building of the doxygen documentation.

doc :
	doxygen docs/doxygen.cfg



# Removing all built files.

clean:
	rm -f ${TARGETS}
	rm -f *.exe
	rm -f ${OBJDIR}/*
