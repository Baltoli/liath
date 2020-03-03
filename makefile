DIRS := 	\
  apps		\
  tools		\

all: build

build:
	-$(MAKE) -k $(DIRS:%=%.build)

clean: $(DIRS:%=%.clean)

$(DIRS:%=%.build):
	-$(MAKE) -k -C $(@:%.build=%)

$(DIRS:%=%.clean):
	-$(MAKE) -k -C $(@:%.clean=%)

.PHONY: all build clean
.PHONY: $(DIRS:%=%.build) $(DIRS:%=%.clean)
