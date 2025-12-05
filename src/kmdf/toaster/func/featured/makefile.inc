_LNG=$(LANGUAGE)
_INX=.
STAMP=stampinf -f $@ -a $(_BUILDARCH)

$(OBJ_PATH)\$(O)\$(INF_NAME).inf: $(_INX)\$(INF_NAME).inx
        copy $(_INX)\$(@B).inx $@
        $(STAMP)

mofcomp: $(OBJ_PATH)\$(O)\toaster.bmf

$(OBJ_PATH)\$(O)\toaster.bmf: toaster.mof
        mofcomp -WMI -B:$(OBJ_PATH)\$O\toaster.bmf toaster.mof
        wmimofck -m -h$(OBJ_PATH)\$O\ToasterMof.h -w$(OBJ_PATH)\$O\htm $(OBJ_PATH)\$(O)\toaster.bmf

