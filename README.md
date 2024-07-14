# Message pour mon moi futur
Après plusieurs heures de débug:
On a determiné que l'erreur ne provenais surement pas du linker, mais de la façon dont on charge l'OS en mémoire


Avancée du projet:
-Bootloader 🚧: permet de passer du 16bit real mode au 32 bit protected mode puis au 64 bit (en activant l'A20 et en chargeant la GDT etc...) UN BUG GRAVE A ETE TROUVE DANS LE BOOTLOADER <br>
-Base du kernel ✅: Création d'un malloc et d'un free basique <br>
-Système de commandes (utilisant une hashtable) ✅ <br>
-Gestion des IDT  ✅: Gestion des interupts du clavier et du timer (puce PIT) (isq 0 et 1 remap avec le PIC sur les inteupts vecteur 32 33), ainsi que gestion basique des erreurs (div error, double fault) <br>
-Lecture/ecritude disque: 🚧 <br> Tentative, infructueuse d'utiliser un FDC (floppydisk controller) pour lire et écrire le disque.


NOTE:
Faire très graffe aux options de compilation (-mcmodel) qui peuvent casser des trucs
On utilisera peut être mcmodel=kernel (ou large) si on veut mettre le code dans la partie haute de la mémoire (ex: .=0x10000)
Mais en attendant blk