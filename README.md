# Flow
Chiffrement à flôt basé sur le générateur de Geffe, offrant une implémentation d'une attaque par correlation


## Installation
Le programme est distributé sous forme d'une archive .zip, pour executer à partir de cette archive il faut :
```bash
unzip Mohamed_Samba_Diallo.zip
cd Mohamed_Samba_Diallo/
make
```


## Usage

```bash
./Flow -h #affiche l'aide
./Flow -c [fichier] [n] #Génére une suite chiffrante de N bits
./Flow -d [fichier] [n] # Attaque par correlation pour générer K
./Flow -g [fichier] [n] #Génére dans fichier toutes les combinaisons possbile sur n bits\n");
./Flow -F #Affiche sur la sortie standart les correlations entre les sorties du LFSR
```
Le fichier à utiliser pour une attaque par correlation est le fichier test_d et le fichier à utiliser pour une suite chiffrante est test_c
