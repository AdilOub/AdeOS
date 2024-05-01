# AdeOS (anciennement EmiOS)
Un OS 64bits codé en C et en Assembleur x86 (en utilisant un compilateur cpp, d'où les fichiers c++)
Le but est d'avoir un kernel et un système basique de commande et de création de fichier léger.
~~Le projet est actuellement en pause~~

Disclaimer: lors du début de ce projet je n'avais pas encore beaucoup de notion en informatique théorique (graphe, arbre, gestion de la mémoire). Ainsi certaine peuvent (et serons) grandement amélioré.

Avancée du projet:
-Bootloader ✅: permet de passer du 16bit real mode au 32 bit protected mode puis au 64 bit (en activant l'A20 et en chargeant la GDT etc...) <br>
-Base du kernel ✅: Création d'un malloc et d'un free basique <br>
-Système de commandes (utilisant une hashtable) ✅ <br>
-Gestion des IDT  ✅: Gestion des interupts du clavier et du timer (puce PIT) (isq 0 et 1 remap avec le PIC sur les inteupts vecteur 32 33), ainsi que gestion basique des erreurs (div error, double fault) <br>
-Lecture/ecritude disque: 🚧 <br> Tentative, infructueuse d'utiliser un FDC (floppydisk controller) pour lire et écrire le disque.

![image](https://github.com/AdilOub/AdeOS/assets/57142734/9724936b-6f71-4132-8c31-5e9623104f65)

![image](https://github.com/AdilOub/AdeOS/assets/57142734/bc728ada-5d29-45f8-b41d-cb11deae2a6e)



WARNING: Il y a actuellement un problème lors du chargement de l'OS, le noyau a atteint une taille >512*64 (donnée chargée par le bootloader, dans readDisk.asm)
Cette version est instable (le bootloader charge plus de donnée qu'il est censé pouvoir le faire, la version fonctionne en ignorant l'erreur lors de l'appel à l'interupt 13h et en démarrant quemu avec l'option -hda)
Le noyau est en cours de ré-écriture pour charger les 512\*64bytes important, puis charger le reste en utilisant le floppydisk controller (pour l'instant sur une autre branche)
