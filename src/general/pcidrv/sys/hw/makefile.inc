mofcomp: pcidrv.bmf

pcidrv.bmf: ..\pcidrv.mof
        mofcomp -B:$(OBJ_PATH)\$(O)\pcidrv.bmf ..\pcidrv.mof
	wmimofck $(OBJ_PATH)\$(O)\pcidrv.bmf


        




