#
#$Id: Makefile,v 1.1 2011/12/27 17:13:35 liu1 Exp $
#


all: dev_console dev_fd765a dev_keyboard dev_ide ne2000 rs232c beep hcm_pci

kernlib:
	(cd ../kernlib/; make)

dev_console: kernlib
	(cd console; make)

dev_fd765a: kernlib
	(cd fd765a; make)

dev_ide: kernlib
	(cd ide; make)

dev_keyboard: kernlib
	(cd keyboard; make)

ne2000: kernlib
	(cd ne2000; make)

rs232c: kernlib
	(cd rs232c; make)

beep: kernlib
	(cd beep; make)

hcm_pci: kernlib
	(cd hcm_pci; make)

wconsole: kernlib
	(cd wconsole; make)

debug:
#	(cd aha152x; make) 
#	(cd dc390; make)
	(cd pcmcia; make)
#	(cd svga_cons; make)


clean:
	(cd console; make clean)
	(cd fd765a; make clean)
	(cd ide; make clean)
	(cd keyboard; make clean)
	(cd ne2000; make clean)
	(cd rs232c; make clean)
	(cd wconsole; make clean)
	(cd beep; make clean)
	(cd hcm_pci; make clean)
#	(cd aha152x; make clean)
#	(cd dc390; make clean)
	(cd pcmcia; make clean)
#	(cd svga_cons; make clean)


