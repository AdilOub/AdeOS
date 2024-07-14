boot = open("boot.bin", "rb")
boot_content = boot.read()
boot.close()

boot2 = open("boot2.bin", "a")
data_size = len(boot_content)
zeros = 65536 - data_size 

if(zeros <=  0):
    print("Erreur: l'OS est trop gros :c")
    exit()

for i in range(zeros):
    boot2.write("\12")

boot2.close()