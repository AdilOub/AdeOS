# AdeOS (anciennement EmiOS)
Un OS 64bits codé en C et en Assembleur x86 (en utilisant un compilateur cpp, d'où les fichiers c++)
Le but est d'avoir un kernel et un système basique de commande et de création de fichier léger.
Le projet est actuellement en pause

Disclaimer: lors du début de ce projet je n'avais pas encore beaucoup de notion en informatique théorique (graphe, arbre, gestion de la mémoire). Ainsi certaine peuvent (et serons) grandement amélioré.

Avancée du projet:
-Bootloader ✅: permet de passer du 16bit real mode au 32 bit protected mode puis au 64 bit (en activant l'A20 et en chargeant la GDT) <br>
-Base du kernel ✅: Création d'un malloc et d'un free basique <br>
-Système de commandes (utilisant une hashtable) ✅ <br>
-Gestion des IDT  ✅: Gestion des interupts du clavier et du timer (puce PIT) (isq 0 et 1 remap avec le PIC sur les inteupts vecteur 32 33), ainsi que gestion basique des erreurs (div error, double fault) <br>
-Lecture/ecritude disque: 🚧 <br> Tentative, infructueuse d'utiliser un FDC (floppydisk controller) pour lire et écrire le disque.

![image](https://github.com/AdilOub/AdeOS/assets/57142734/9724936b-6f71-4132-8c31-5e9623104f65)

![image](https://github.com/AdilOub/AdeOS/assets/57142734/bc728ada-5d29-45f8-b41d-cb11deae2a6e)
