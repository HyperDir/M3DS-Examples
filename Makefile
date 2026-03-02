SUBDIRS     := $(wildcard */Makefile)
SUBDIRS     := $(patsubst %/Makefile,%,$(SUBDIRS))

MAKEFLAGS   := --silent -j

TARGETS     := all run 3dsx clean zip cia 3ds

.PHONY: $(TARGETS) $(SUBDIRS)

$(TARGETS): $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)
