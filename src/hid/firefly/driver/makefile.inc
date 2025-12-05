mofcomp: firefly.bmf

firefly.bmf: firefly.mof
        mofcomp -B:$(OBJ_PATH)\$(O)\firefly.bmf firefly.mof
        wmimofck $(OBJ_PATH)\$(O)\firefly.bmf





