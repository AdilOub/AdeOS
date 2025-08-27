sudo ip tuntap add dev tap0_adeos mode tap user $(whoami)
sudo ip link set tap0_adeos up
sudo ip link set tap0_adeos promisc on
#test: ip link show tap0_adeos

#pour lire: sudo tcpdump -i tap0_adeos -XX


#pour creer le bridge:
sudo ip link add name br0_adeos type bridge
sudo ip link set dev br0_adeos up

sudo ip link set dev tap0_adeos master br0_adeos

#pour connecter le bridge a internet (via eth0):
#sudo ip link set dev eth0 master br0_adeos


#commande pour ping:
#arping -I tap0_adeos ff:ff:ff:ff:ff:ff