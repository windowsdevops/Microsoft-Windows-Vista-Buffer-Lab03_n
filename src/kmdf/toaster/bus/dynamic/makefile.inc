LNG=$(LANGUAGE)
_INX=.
STAMP=stampinf -f $@ -a $(_BUILDARCH)

$(OBJ_PATH)\$(O)\$(INF_NAME).inf: $(_INX)\$(INF_NAME).inx 
    copy $(_INX)\$(@B).inx $@
    $(STAMP)

mofcomp: busenum.bmf

busenum.bmf: busenum.mof
        mofcomp -B:$(OBJ_PATH)\$O\busenum.bmf busenum.mof
        wmimofck $(OBJ_PATH)\$O\busenum.bmf





