
major = $(word 1,$(subst ., ,$1))
minor = $(word 2,$(subst ., ,$1))
patch = $(word 3,$(subst ., ,$1))

define version_gt
$(if $(shell [ $(call major,$1) -gt $(call major,$2) ] && echo 1),1,\
$(if $(shell [ $(call major,$1) -lt $(call major,$2) ] && echo 1),2,\
$(if $(shell [ $(call minor,$1) -gt $(call minor,$2) ] && echo 1),1,\
$(if $(shell [ $(call minor,$1) -lt $(call minor,$2) ] && echo 1),2,\
$(if $(shell [ $(call patch,$1) -gt $(call patch,$2) ] && echo 1),1,\
$(if $(shell [ $(call patch,$1) -lt $(call patch,$2) ] && echo 1),2,0))))))
endef

define version_ge
$(if $(shell [ $(call major,$1) -gt $(call major,$2) ] && echo 1),1,\
$(if $(shell [ $(call major,$1) -lt $(call major,$2) ] && echo 1),0,\
$(if $(shell [ $(call minor,$1) -gt $(call minor,$2) ] && echo 1),1,\
$(if $(shell [ $(call minor,$1) -lt $(call minor,$2) ] && echo 1),0,\
$(if $(shell [ $(call patch,$1) -ge $(call patch,$2) ] && echo 1),1,\
$(if $(shell [ $(call patch,$1) -lt $(call patch,$2) ] && echo 1),0,0))))))
endef



v1 := 5.4.13
v2 := 5.4.12

#res := $(strip $(call version_gt,$(v1),$(v2)))
res := $(strip $(call version_ge,$(v1),$(v2)))

test:
	@echo "Compare $(v1) and $(v2): result=$(res)"
	@if [ "$(res)" = "1" ]; then \
		echo "$(v1) >= $(v2)"; \
	elif [ "$(res)" = "2" ]; then \
		echo "$(v1) < $(v2)"; \
	else \
		echo "$(v1) < $(v2)"; \
	fi
