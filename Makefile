##
# i
#
# @file
# @version 0.1

# tool macros
SUBDIRS := exercise_1 exercise_2 exercise_3

all: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@

# phony targets
clean:
	$(shell rm -rf $(OBJ_DIR) $(BUILD_DIR) *.o)

.PHONY: all clean $(SUBDIRS)

# end
