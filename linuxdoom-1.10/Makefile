################################################################
#
# $Id:$
#
# $Log:$
#
CC=  gcc  # gcc or g++

CFLAGS=-g -Wall -DNORMALUNIX -DLINUX -DSNDSERV -DSNDSRV # -DUSEASM 
LDFLAGS=-L/usr/X11R6/lib
LIBS=-lXext -lX11 -lnsl -lm

# subdirectory for objects
BUILD_DIR=build

# not too sophisticated dependency
OBJS=				\
		$(BUILD_DIR)/doomdef.o		\
		$(BUILD_DIR)/doomstat.o		\
		$(BUILD_DIR)/dstrings.o		\
		$(BUILD_DIR)/i_system.o		\
		$(BUILD_DIR)/i_sound.o		\
		$(BUILD_DIR)/i_video.o		\
		$(BUILD_DIR)/i_net.o			\
		$(BUILD_DIR)/tables.o			\
		$(BUILD_DIR)/f_finale.o		\
		$(BUILD_DIR)/f_wipe.o 		\
		$(BUILD_DIR)/d_main.o			\
		$(BUILD_DIR)/d_net.o			\
		$(BUILD_DIR)/d_items.o		\
		$(BUILD_DIR)/g_game.o			\
		$(BUILD_DIR)/m_menu.o			\
		$(BUILD_DIR)/m_misc.o			\
		$(BUILD_DIR)/m_argv.o  		\
		$(BUILD_DIR)/m_bbox.o			\
		$(BUILD_DIR)/m_fixed.o		\
		$(BUILD_DIR)/m_swap.o			\
		$(BUILD_DIR)/m_cheat.o		\
		$(BUILD_DIR)/m_random.o		\
		$(BUILD_DIR)/am_map.o			\
		$(BUILD_DIR)/p_ceilng.o		\
		$(BUILD_DIR)/p_doors.o		\
		$(BUILD_DIR)/p_enemy.o		\
		$(BUILD_DIR)/p_floor.o		\
		$(BUILD_DIR)/p_inter.o		\
		$(BUILD_DIR)/p_lights.o		\
		$(BUILD_DIR)/p_map.o			\
		$(BUILD_DIR)/p_maputl.o		\
		$(BUILD_DIR)/p_plats.o		\
		$(BUILD_DIR)/p_pspr.o			\
		$(BUILD_DIR)/p_setup.o		\
		$(BUILD_DIR)/p_sight.o		\
		$(BUILD_DIR)/p_spec.o			\
		$(BUILD_DIR)/p_switch.o		\
		$(BUILD_DIR)/p_mobj.o			\
		$(BUILD_DIR)/p_telept.o		\
		$(BUILD_DIR)/p_tick.o			\
		$(BUILD_DIR)/p_saveg.o		\
		$(BUILD_DIR)/p_user.o			\
		$(BUILD_DIR)/r_bsp.o			\
		$(BUILD_DIR)/r_data.o			\
		$(BUILD_DIR)/r_draw.o			\
		$(BUILD_DIR)/r_main.o			\
		$(BUILD_DIR)/r_plane.o		\
		$(BUILD_DIR)/r_segs.o			\
		$(BUILD_DIR)/r_sky.o			\
		$(BUILD_DIR)/r_things.o		\
		$(BUILD_DIR)/w_wad.o			\
		$(BUILD_DIR)/wi_stuff.o		\
		$(BUILD_DIR)/v_video.o		\
		$(BUILD_DIR)/st_lib.o			\
		$(BUILD_DIR)/st_stuff.o		\
		$(BUILD_DIR)/hu_stuff.o		\
		$(BUILD_DIR)/hu_lib.o			\
		$(BUILD_DIR)/s_sound.o		\
		$(BUILD_DIR)/z_zone.o			\
		$(BUILD_DIR)/info.o				\
		$(BUILD_DIR)/sounds.o

EXECUTABLE=linuxxdoom

all:	 make_dir $(BUILD_DIR)/$(EXECUTABLE)
	cp $(BUILD_DIR)/$(EXECUTABLE) .

make_dir:
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf *.o *~ *.flc
	rm -rf $(BUILD_DIR)
	rm -rf $(EXECUTABLE)

$(BUILD_DIR)/$(EXECUTABLE):	$(OBJS) $(BUILD_DIR)/i_main.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(BUILD_DIR)/i_main.o \
	-o $(BUILD_DIR)/$(EXECUTABLE) $(LIBS)

$(BUILD_DIR)/%.o:	%.c
	$(CC) $(CFLAGS) -c $< -o $@

#############################################################
#
#############################################################