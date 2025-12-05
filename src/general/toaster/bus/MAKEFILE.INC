mofcomp: busenum.bmf

busenum.bmf: busenum.mof
        mofcomp -B:$(OBJ_PATH)\$(O)\busenum.bmf busenum.mof
        wmimofck $(OBJ_PATH)\$(O)\busenum.bmf





