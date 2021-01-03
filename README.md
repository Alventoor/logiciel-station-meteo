## Présentation

Ce logiciel fait partie d'un mini-projet effectué lors de ma seconde année en BTS Systèmes
Numériques, option Informatique et Réseaux. Il permet la récupération depuis un capteur
BME280 de données météorologiques ainsi que leur envoi sur un serveur distant. Ce logiciel
a été conçu et testé pour fonctionner avec la distribution Linux Raspberry Pi OS.

Pour plus d'informations sur son fonctionnement, lire la documentation présente dans le
dossier **doc**.

## Dépendances

| Bibliothèque              | Version          |
| ------------------------- | ---------------- |
| Bibliothèque standard C++ | Supportant C++11 |
| Qt - module Qt Network    | >= 5             |

| Matériel       | Connexion |
| -------------- | --------- |
| Capteur BME280 | Bus I2C   |

Avec le code source du logiciel est aussi directement fournie une librairie permettant de
récupérer les données du capteur. Celle-ci, sous licence GPL, est trouvable à cette adresse :
https://github.com/PhilippeSimier/Capteurs_I2C/tree/master/BME280.

## Compilation

La compilation s'effectue à l'aide de l'application **QMake**. Une fois les dépendances
installées, il suffit de récupérer et compiler le projet.

```
git clone https://github.com/Alventoor/logiciel-station-meteo
mkdir station-meteo_build && cd station-meteo_build
qmake ../logiciel-station-meteo/station-meteo.pro
make
```