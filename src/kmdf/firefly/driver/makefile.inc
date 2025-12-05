_LNG=$(LANGUAGE)
_INX=.
STAMP=stampinf -f $@ -a $(_BUILDARCH)

$(OBJ_PATH)\$(O)\$(INF_NAME).inf: $(_INX)\$(INF_NAME).inx 
    copy $(_INX)\$(@B).inx $@
    $(STAMP)


mofcomp: $(OBJ_PATH)\$(O)\firefly.bmf

$(OBJ_PATH)\$(O)\firefly.bmf: firefly.mof
        mofcomp -B:$(OBJ_PATH)\$(O)\firefly.bmf firefly.mof
    wmimofck -h$(OBJ_PATH)\$(O)\fireflymof.h $(OBJ_PATH)\$(O)\firefly.bmf

