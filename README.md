# Message pour mon moi futur
Après plusieurs heures de débug:
On a determiné que l'erreur ne provenais surement pas du linker, mais de la façon dont on charge l'OS en mémoire


Idée:
Charger 32M dans 0x8000
Puis le reste dans 0x8000+0x8000
etc...



NOTE:
Faire très graffe aux options de compilation (-mcmodel) qui peuvent casser des trucs
On utilisera peut être mcmodel=kernel (ou large) si on veut mettre le code dans la partie haute de la mémoire (ex: .=0x10000)
Mais en attendant blk