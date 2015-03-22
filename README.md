# Innocence

## Description

* Stealth Remote Administration Tool
* Audit tool for desktops

## Technologies

* Qt, création des GUI
* Windows API (Winsock, COM, Win32)
* SDK Windows (drivers)
* Jpeg library (pour les screenshots)
* OTL Sql library

## Dev Env

* Visual Studio 2012 (version 11), IDE
* CMake
* GraphViz
* Doxygen
* Vmware vSphere

## Workflow

1. Détection version de Windows
2. Escalade des privilèges
3. Installation
4. Copie exécutable
5. Exécution du slave
6. Effacement exécutable d’origine
7. Création IE caché
8. Démarrage d’un IE avec interface COM
9. Installation du service et du driver pour cacher les processus
10. Injection dans l’IE caché
11. Installation clé de registre de démarrage
12. Reverse connection to Master
13. Traitement des commandes
14. Déconnexion
15. Désinstallation
16. Effacement clé de registre de démarrage
17. Kill de l’hôte IE et de sa DLL injectée
18. Auto-effacement de l’exécutable

## Tools

* bin2array : transforme un fichier en tableau C
* buildnum : incrémente le numéro de build a chaque compilation de la solution
* bnhide.sys : driver pour cacher des objets Windows (uniquement les processus pour le moment, ne fonctionne que pour Windows 32bit)
* NetCat : lancé par thesleeper pour le shell à distance
* UPX : pack les exécutables finaux pour réduire leur taille et mitiger la signature

## Test protocol

* création d’un lab de test virtuel, avec toutes les versions de Windows en 32 et 64bit
* création de modules de test pour chaque librairie
* automatisation des tests, on crée un serveur de test qui agglomère les résultats des tests effectués sur la plateforme cible
* tests de niveau plateforme : création d’un programme “banc de test” qui va faire tourner plusieurs tests et observer leur comportement
* tests du niveau sécurité : on prend une plateforme de référence et on lance les tests sensés être détectés par les antivirus et observer le comportement du système
* tests utilisateurs (comportement étrange de la session, ralentissements, etc)

## Librairies

* imd : innocence malicious driver, contient les techniques de furtivité et d’injection de paquets et de capture réseau
* common : logger(debugging par le réseau, fichier et console), design patterns, en gros ce qui est commun a toutes les libraires et executables
* network : encapsulation de l’api réseau, linux et windows
* blaspheme : gestion des session (blaspheme c’est le nom du protocole), authentification, transfert de fichiers
* system : encapsulation des api de gestion du systeme, gestionnaire de processus, fichiers, registres, avec des fonctions avancées
* elevation32.exe et elevation64.exe : exploit d’escalade des privillèges
* elevation32.dll et elevation64.dll : DLL contenant l’exploit
* malicious : librairie la plus “interessante”, au menu, injecteur de DLL, lanceur d’IE caché, keylogger et détournement du clavier, vol de mot de passe, camouflage des processus, intégrateur de ressource binaire (mettre un exécutable dans un autre)

## Modules

* inhibiter : c’est une sorte d’installateur, contient le “slave_dll”, transporte la charge utile
* slave_dll : c’est le nom donné à la DLL principale d’innocence, qui contient le trojan
* master : interface console et graphique pour controller un groupe de slave
* logger_gui : interface graphique pour voir les messages de debugging
* logger : idem mais version console (pour éviter de déployer les DLL de Qt)

## Audit tests

* isinjected : simple DLL pour tester l’injection (malicious)
* isadmin : simple exécutable pour voir si l’on est administrateur
* elevation : test elevation de privilege sur windows 32 et 64bit (malicious)
* rshell : shell à distance
* mail : test d’envoi de mail
* transfer : test la librairie blaspheme pour le transfert de fichiers
* passwords : récupère les mots de passe
* pipe : test de l’IPC pipe de windows (inclu dans la librairie network)
* session : tests des sessions blaspheme
* keyhooker : test du keylogger (malicious)
* hideproc : test le camouflage de processus (malicious)
* hidewin : test du lanceur d’IE (malicious)
* injection : test de l’injecteur de DLL (malicious)
* registre : test de l’api de gestion du registre (system)
* screenshot : test de la prise de screenshot (malicious)
* mail : test envoi de mail (malicious)

## Dev priorities

1. Correction des bugs
2. Créer des tests unitaires sous forme de mini projects exécutables (en cours)
3. Tester les modules existants sur les systèmes, par ordre : Seven, XP, 8, Vista, pour cela créer une sorte de lab, sur lequel on peut créer n’importe quelle combinaison : OS, Service Pack, Antivirus, Antispyware
4. Développement des fonctionnalités de camouflage
5. Développement des fonctionnalités “fun”

## ToDo

* Développement d’un driver unifié ou morcelé pour cacher des fichiers, processus, clés de registre, sockets, keylogger, et pour injecter des paquets sur le réseau (NDIS driver, s’inspirer de NPF de WinPCAP)
* Ajouter des types de protocole : mode ligne, mode message, mode binaire
* Taille des fichier lors du listing (pour les transferts)
* Amélioration des transferts de fichiers (utiliser un outil existant par exemple)
* Remplacement d’une adresse IP stockée en nom de domaine, voire stocker une liste d’IP ou de domaines sur lesquels le trojan doit tenter de se connecter
* Compléter documentation du projet / Générer schéma workflow
* Créer site web / bugtracker
* Gestion de plusieurs langages utilisateurs (pour toutes les interfaces graphiques)
* Decouple logging library
* Test cluster in PCC

## Ideas

* Réécriture du module de vol de mots de passe (défectueux)
* Patcher : permet de patcher un exécutable qui se lance au démarrage de windows pour qu’il lance le trojan sans avoir a ecrire de clé de registre dans RUN
* Ajout d’un module scanning de réseau
* Ajout d’un module sniffing du réseau
* Ajout d’un module de proxy

