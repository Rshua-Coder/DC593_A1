include MakeOption.txt

module_Common := $(dir $(wildcard ../Common/Makefile))
module_Drv := $(dir $(wildcard ../Drv/Makefile))
module_DrvExt := $(dir $(wildcard ../DrvExt/Makefile))
module_Lib := $(dir $(wildcard ../Lib/Makefile))
module_LibExt := $(dir $(wildcard ../LibExt/Makefile))
module_App := $(dir $(wildcard ../App/Makefile))
module_AppExt := $(dir $(wildcard ../AppExt/Makefile))

-include ../Project/ProjectConfig.txt

ifneq "$(ACTIVE_PROJECT)" ""
module_Prj := ../Project/$(ACTIVE_PROJECT)/
else
# Find the first folder in ../Project/ that contains "Makefile"
module_Prj := $(dir $(firstword $(sort $(shell find ../Project -name \'Makefile\'))))
endif

module_all := \
	$(module_Common) \
	$(module_Drv) \
	$(module_DrvExt) \
	$(module_Lib) \
	$(module_LibExt) \
	$(module_App) \
	$(module_AppExt) \
	$(module_Prj)

define copy_log
	@-cp \
	$(module_Common)log*.txt \
	$(module_Drv)log*.txt \
	$(module_DrvExt)log*.txt \
	$(module_Lib)log*.txt \
	$(module_LibExt)log*.txt \
	$(module_App)log*.txt \
	$(module_AppExt)log*.txt \
	$(module_Prj)log*.txt \
	.
endef

ifeq "$(ISOLATE_DEP)" "ON"
.PHONY: all rm_log $(module_all) clean rebuild debug release dep

all: rm_log
	@make dep
	@make debug
	@make release
	$(call copy_log)
else
.PHONY: all rm_log $(module_all) clean rebuild debug release

all: rm_log $(module_all)
	$(call copy_log)
endif

$(module_all):
	@$(MAKE) --directory=$@ $(MAKECMDGOALS)

rm_log:
	@-rm -f log*.txt

clean: rm_log $(module_all)

ifeq "$(ISOLATE_DEP)" "ON"
rebuild:
	@make clean
	@make dep
	@make debug
	@make release
	$(call copy_log)

dep: $(module_all)
else
rebuild: rm_log $(module_all)
	$(call copy_log)
endif

debug: rm_log $(module_all)
	$(call copy_log)

release: rm_log $(module_all)
	$(call copy_log)

rm_chk_log:
	@-rm -f chk*.txt

chk: rm_chk_log $(module_all)

chkclean: $(module_all)

rm_lint_log:
	@-rm -f lint*.txt

lint: rm_lint_log $(module_all)

lintclean: $(module_all)
