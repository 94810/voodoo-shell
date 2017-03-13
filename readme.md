# Voodoo Shell
## Compilation et éxécution
Un makefile est fournis 
	make ou make shell pour compiler
	make run pour lancer 
## Fonction implémenté 
### Fonction du sujet
Tous les fonctions demandés sont présente soit

* Sortir avec ^D
* Sortir avec la commande exit
* change directories avec cd
* execution de commande simple
* redirection simple sortie standard, d'erreur et entrée standard ( >, 2>, <)
* pipe chainable
* mise en tâche de fond  	
### Fonction "Bonus"

* redirection en mode append (>>, 2>>)
* Interpréteur Brainfuck
	Il est accésible via la commande BF [Your Prog]
	[Your Prog] est le programme a interprété
	exemple voici un Hello World ! :
	++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>.

* Utilisation de GNU readline : on peut ainsi utiliser l'historique de la session courante, la complétion des noms de fichier, et le déplacement du curseur une ligne en cours
* Gestion simple des guillemets
* Affichage d'un fichier motd
## Source
* Unix man
* Le Cours
* Trial and Error
