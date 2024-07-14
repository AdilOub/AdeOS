#!/bin/bash

# On veut 2 fichiers
if [ $# -ne 2 ]; then
    echo "Usage: $0 file1 file2"
    exit 1
fi

file1=$1
file2=$2

# Vérifier que les fichiers existent
if [ ! -f "$file1" ] || [ ! -f "$file2" ]; then
    echo "Les fichiers $file1 et $file2 doivent exister."
    exit 1
fi

# Lire le contenu des fichiers
content1=$(cat "$file1")
content2=$(cat "$file2")

# Déterminer la longueur du contenu du premier fichier
len1=${#content1}

echo "taille fichier 1: " $len1

# Extraire la partie du second fichier après la longueur du premier fichier
part_to_keep=${content2:$len1}

# Combiner les contenus
new_content="${content1}${part_to_keep}"

# Écrire le nouveau contenu dans le second fichier (ou dans un nouveau fichier)
#echo -n "$new_content" > "$file2"

echo "Le fichier $file2 a été mis à jour avec le contenu : $new_content"
