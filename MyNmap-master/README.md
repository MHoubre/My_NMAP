# Projet RSA - Baptiste CHOCOT & Maël HOUBRE

## Sommaire

* [Introduction](#intro)
* [Réseau](#res)
* [Compiler et exécuter le projet](#compil)

## Introduction <a id="intro"></a>

MyNmap est un outil de scan réseau et potentiellement d'aide à la pénétration.

Toutes les fonctions seront documentées en utilisant des tags [Javadoc](https://fr.wikipedia.org/wiki/Javadoc#Tags_Javadoc)-like.
Par exemple :
```
/**
 *
 * @author James Bond
 * @param letter correspond to the char you want to be printed
 * @return this only return 0
 */
 int whatTheHellAmIWriting(char letter) {
     printf("%c", letter);
     return 0;
 }
 ```
## Réseau <a id="res"></a>

Pour se connecter au réseau de tests de l'outil, on utilise la commande suivante :
`ssh -p 10027   montrouge@tardis.telecomnancy.univ-lorraine.fr`,
le mot de passe étant `maelestbeau`.

Pour rappel :
Le réseau virtuel est le 192.168.150.0/24.
Les « portes d’entrées » tournent sur les IP de .1 à .36.
Les « vrais » serveurs tournent à partir de .37.

## Compiler et exécuter le projet <a id="compil"></a>

* Ouvrir un terminal et taper `make`, et laissezz vous guider.
* Différents tests sont disponibles, par exemple `make TestActiveDevice\.
* Pour téléverser l'exécutable sur le réseau, tapez `make send`.
* Pour se connecter à la machine du réseau, tapez `make connect`.