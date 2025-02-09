#include "includes/Timer.h"

//l'idée est d'avoir un cureseur qui clignotte '_' pour montrer que le système est en vie


/*diffrent channel du pit:
0: c'est des interupts (irq0 généré en high)
1: c'était pour refresh la ram, plus utilisé
2: c'est pour le haut parleur

*/
void initTimer(){
    /*
    d'après la doc de osdev.org on utilise la puce PIT qui utilise le port 0x43 en write pour le mode 
    et le port 0x40 pour la fréquence (on va utiliser le channel 0)
    Pour mettre le mode on doit utiliser un 8 bits 
    Ici on veut: 00 11 000 0 (channel 0, lobyte/hibyte, interrupt on terminal count) 
    */
    PrintString("Timer init...\n\r", FOREGROUND_GREEN);

    uint16_t divisor = 0xFFFF;  // Fréquence maximale du PIT (en Hz)
    uint8_t lobyte = (uint8_t)(divisor & 0xFF);  // Partie basse du diviseur
    uint8_t hibyte = (uint8_t)((divisor >> 8) & 0xFF);  // Partie haute du diviseur

    // Envoi de la commande de configuration du PIT
    outb(PIT_COMMAND_PORT, PIT_MODE);

    // Envoi de la valeur du diviseur au canal 0 du PIT (partie basse puis haute)
    outb(PIT_CHANNEL0_PORT, lobyte);
    outb(PIT_CHANNEL0_PORT, hibyte);
}

