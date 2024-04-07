# AdeOS (anciennement EmiOS)
Un OS 64bits codé en C et en Assembleur x86
Le but est d'avoir un kernel et un système basique de commande et de création de fichier léger.
Le projet est actuellement en pause

Disclaimer: lors du début de ce projet je n'avais pas encore beaucoup de notion en informatique théorique (graphe, arbre, gestion de la mémoire). Ainsi certaine peuvent (et serons) grandement amélioré.

Avancée du projet:
-Bootloader ✅: permet de passer du 16bit real mode au 32 bit protected mode puis au 64 bit (en activant l'A20 et en chargeant la GDT)
-Base du kernel ✅: Création d'un malloc et d'un free basique 
-Gestion des IDT 🚧: Work in progress

![image](https://github.com/AdilOub/AdeOS/assets/57142734/9724936b-6f71-4132-8c31-5e9623104f65)
