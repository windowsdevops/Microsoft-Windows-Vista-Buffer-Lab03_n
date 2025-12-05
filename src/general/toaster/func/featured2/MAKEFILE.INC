mofcomp: toaster.bmf

toaster.bmf: toaster.mof
        mofcomp -B:$(OBJ_PATH)\$(O)\toaster.bmf toaster.mof
        wmimofck $(OBJ_PATH)\$(O)\toaster.bmf





