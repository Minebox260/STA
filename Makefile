all: Serveur.exe Client.exe

Serveur.exe:
	@echo "Compilation du serveur..."
	gcc -pthread -o Serveur.exe ./Serveur/Serveur.c ./Serveur/Serveur_communication.c ./Serveur/Serveur_utils.c

Client.exe:
	@echo "Compilation du client..."
	gcc -pthread -o Client.exe ./Client/Client.c

clean:
	@echo "Suppression de Serveur.exe et Client.exe..."
	@rm -f Serveur.exe
	@rm -f Client.exe