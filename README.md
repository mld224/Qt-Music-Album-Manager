# Qt-Music-Album-Manager
Application de bureau développée en C++ avec Qt Framework. Permet la gestion complète d'une discothèque (CRUD) avec persistance des données en JSON/XML et support multilingue (FR/EN).
# Gestionnaire d'Albums Musicaux

## Description
Cette application de bureau est un gestionnaire de collection musicale développé en C++ avec le framework Qt. Elle offre une interface complète pour cataloguer des albums et des pistes, intégrant des fonctionnalités de persistance des données (Sauvegarde/Chargement) et des statistiques en temps réel.

Ce projet démontre l'utilisation de la programmation orientée objet, la gestion de fichiers (sérialisation JSON/XML) et le développement d'interfaces graphiques avec Qt Widgets.

## Fonctionnalités Principales
- **Gestion des albums** : Création, modification et suppression d'albums avec détails (titre, artiste, année, prix et catégorie).
- **Gestion des pistes** : Ajout de morceaux individuels avec durée spécifique et note d'appréciation.
- **Persistance des données** : 
  - Sauvegarde et chargement des collections au format JSON (`QJsonDocument`).
  - Sauvegarde et chargement des collections au format XML (`QDomDocument`).
- **Recherche et Tri** : Filtrage en temps réel par nom d'album et tri alphabétique automatique.
- **Internationalisation** : Interface bilingue (Français/Anglais) gérée via `QTranslator`.
- **Statistiques** : Calcul automatique de la durée totale des albums basé sur les pistes.

## Environnement Technique
- **Langage** : C++ (Standard C++17)
- **Framework** : Qt 6.x
- **Interface** : Qt Widgets (QMainWindow, QTableWidget, QFormLayout)
- **Système de build** : QMake

## Installation et Utilisation
1. Cloner le dépôt.
2. Ouvrir le fichier `S2-01.pro` dans **Qt Creator**.
3. Configurer le projet avec un kit Desktop (MinGW 64-bit recommandé).
4. Compiler et lancer le projet.

## Auteur
**Mamadou Lamine DIALLO** [Profil LinkedIn](https://www.linkedin.com/in/mamadou-lamine-diallo-507347357/)
