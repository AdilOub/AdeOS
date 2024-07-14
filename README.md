# Message pour mon moi futur
Après plusieurs heures de débug:
On a determiné que l'erreur ne provenais surement pas du linker, mais de la façon dont on charge l'OS en mémoire


Avancée du projet:
-Bootloader ✅: permet de passer du 16bit real mode au 32 bit protected mode puis au 64 bit (en activant l'A20 et en chargeant la GDT etc...) Il y a toujours un petit bug dans le bootloader <br>
-Base du kernel ✅: Gestion de la mémoire avec la création d'un malloc et d'un free basique, gestion du clavier etc... <br>
-Système de commandes (utilisant une hashtable) ✅ <br>
-Gestion des IDT  ✅: Gestion des interupts du clavier et du timer (puce PIT) (isq 0 et 1 remap avec le PIC sur les inteupts vecteur 32 33), ainsi que gestion basique des erreurs (div error, double fault) <br>
-Lecture/ecritude disque: ✅ <br> Tentative, infructueuse d'utiliser un FDC (floppydisk controller) pour lire et écrire le disque.
Les floppy étant mal gérer, j'ai décidé d'utiliser un disque dur à la place. La lecture et l'écriture disque fonctionne (avec ATA PIO). Le code doit être nettoyer et s'assurer qu'il ne génére pas d'erreur.
-Ecriture d'un système de fichier: 🚧 WIP


NOTE:
Faire très attention aux options de compilation (-mcmodel) qui peuvent casser des trucs
On utilisera peut être mcmodel=kernel (ou large) si on veut mettre le code dans la partie haute de la mémoire (ex: .=0x10000)
