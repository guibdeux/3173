all:
	gcc eviterSigInt-DurantTraitement.c -o eviterSigInt-DurantTraitement
	gcc gestionDirecte.c -o gestionDirecte
	gcc shellFifo.c -o shellFifo
	gcc shellFifo-2.c -o shellFifo-2
	gcc shellPipe.c -o shellPipe
	gcc traiterDeuxFoisSignal-RevenirDefault.c -o traiterDeuxFoisSignal-RevenirDefault
	gcc traiterUneFois-RevenirTraitementDefault.c -o traiterUneFois-RevenirTraitementDefault
	gcc verifierReception-Retarder0Crtl-C.c -o verifierReception-Retarder0Crtl-C

indent:
	@astyle  --style=allman -A2 *.c,*.h
	@rm *orig*

clean:
	@rm -rf eviterSigInt-DurantTraitement gestionDirecte shellFifo shellFifo-2 shellPipe traiterDeuxFoisSignal-RevenirDefault traiterUneFois-RevenirTraitementDefault verifierReception-Retarder0Crtl-C