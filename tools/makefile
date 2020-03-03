# This top-level makefile is exported into the Pin kit and is referenced by
# several external scripts.  Be careful when changing the names of the makefile
# targets so as not to break anything that depends on them.

# All directories which contain tests should be placed here.
# Please maintain alphabetical order.

ALL_TEST_DIRS := 	\
  Demo			\

# All directories which contain utilities for the test system should be placed here.
# Please maintain alphabetical order.

ALL_UTILS_DIRS :=

# Some of these targets are referenced in the User Guide and/or are commonly used by Pin users.
# Others are used by the nightly system.

all: build

build:
	-$(MAKE) -k $(ALL_TEST_DIRS:%=%.build)

install:
	-$(MAKE) -k $(ALL_TEST_DIRS:%=%.install)

clean: $(ALL_TEST_DIRS:%=%.clean)

# These are directory-specific template targets.

$(ALL_TEST_DIRS:%=%.build):
	-$(MAKE) -k -C $(@:%.build=%)

$(ALL_TEST_DIRS:%=%.install):
	-$(MAKE) -k -C $(@:%.install=%) install

$(ALL_TEST_DIRS:%=%.test):
	-$(MAKE) -k -C $(@:%.test=%) test

$(ALL_TEST_DIRS:%=%.sanity):
	-$(MAKE) -k -C $(@:%.sanity=%) sanity

$(ALL_TEST_DIRS:%=%.clean):
	-$(MAKE) -k -C $(@:%.clean=%) clean

.PHONY: all build install clean
.PHONY: $(ALL_TEST_DIRS:%=%.build) $(ALL_TEST_DIRS:%=%.install) $(ALL_TEST_DIRS:%=%.test)
.PHONY: $(ALL_TEST_DIRS:%=%.sanity) $(ALL_TEST_DIRS:%=%.clean)
