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


NOTE 2:
TODO réorganiser les fichires (car on commence a en avoir bcp)
/: kernel.cpp drivers/ utils/ memory/ graphics/ core/
driver: tout ce qui utilise les IO pour communiquer avec le hardware (IO.cpp, IDT.cpp, Timer.cpp, DiskRead.cpp Keyboard.cpp)
memory: tout ce qui gère la mémoire (heap.cpp, memoru.cpp)
graphics: tout ce qui permet d'afficher (Textprint, Render)
utils: tout les fichiers qui sont pas directement lié à l'os (typedef, arbre, )
core: Command & Compilateur & FileSystem

NOTE3:
Pour simplifier le compilo j'utilise des syscalls pour appeler des fonctions de base (surement print, malloc, et des fonction lecture/ecriture)
int 0x80 (=128) en ref à unix
dans rax le numéro du syscall
le reste depend du syscall mais l'idée sera globalement: 
- soit il utilise quelque registre
- soit il utilise rbx et la pile


rax: description : argument=description
0 : affiche un msg de debug
1 : print : rbx: addresse de la chaine de caractère




Encore une note:
possibilité d'avoir des commandes compilé dans le file system et de les lancer en chargeant leur fichier en mémoire + en utilisant un handler tout simple (cf. la fonction asm_handler dans Command.cpp) qui gère le passage des arguments du code kernel (en C) à la fonction compilé (en asm)
Cette fonction compilé peut appeler des interupts sytème en utilisant la NOTE3, et est censé (TODO: test similaire au truc à la fin de Kernel.C) pouvoir appeler d'autre fonction par leur addresse (technique lea + call)

![image](https://github.com/user-attachments/assets/ef78049b-8935-44a0-b524-7940a00b854c)
