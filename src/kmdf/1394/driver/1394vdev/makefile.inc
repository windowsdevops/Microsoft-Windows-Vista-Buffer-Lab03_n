_LNG=$(LANGUAGE)
_INX=..
STAMP=stampinf -f $@ -a $(_BUILDARCH)

$(OBJ_PATH)\$(O)\$(TARGETNAME).inf: $(_INX)\1394samp.inx
    copy $(_INX)\1394samp.inx $@
    $(STAMP)
