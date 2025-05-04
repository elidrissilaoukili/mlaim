# Projet SUMO : Optimisation de la mobilité urbaine intelligente

Ce projet utilise SUMO (Simulation of Urban MObility) pour modéliser et optimiser le trafic dans un quartier résidentiel, avec un focus particulier sur la gestion des feux de circulation et la priorisation des transports en commun, notamment dans un scénario de grève de bus.

## Objectifs du projet

- Modéliser un quartier urbain avec 4 intersections et 8 routes principales
- Simuler du trafic mixte (voitures, bus, vélos)
- Implémenter un système de priorisation des transports publics
- Détecter la congestion et adapter les feux de circulation
- Collecter des données sur les émissions de CO₂
- Comparer différents scénarios (normal vs grève de bus)

## Structure des fichiers

```
.
├── quartier.nod.xml        # Définition des nœuds (intersections)
├── quartier.edg.xml        # Définition des arêtes (routes)
├── quartier.con.xml        # Connexions entre les routes
├── quartier.det.xml        # Définition des détecteurs
├── quartier.add.xml        # Éléments additionnels (arrêts de bus, détecteurs)
├── quartier.rou.xml        # Définition des véhicules et itinéraires
├── quartier.net.xml        # Réseau complet (généré automatiquement)
├── quartier.sumocfg        # Configuration de la simulation
├── gui-settings.xml        # Paramètres d'affichage pour SUMO-GUI
├── traffic_controller.py   # Contrôleur intelligent des feux en Python
└── dashboard.py            # Tableau de bord pour visualiser les données en temps réel
```

## Prérequis

- SUMO (version 1.12.0 ou supérieure)
- Python 3.7+
- Bibliothèques Python : traci, matplotlib, numpy

## Installation

1. Installez SUMO : [https://sumo.dlr.de/docs/Downloads.php](https://sumo.dlr.de/docs/Downloads.php)
2. Configurez la variable d'environnement SUMO_HOME
3. Installez les dépendances Python :
   ```bash
   pip install matplotlib numpy
   ```

## Préparation du projet

1. Générez le réseau à partir des fichiers de définition :
   ```bash
   netconvert --node-files quartier.nod.xml --edge-files quartier.edg.xml --connection-files quartier.con.xml -o quartier.net.xml
   ```

## Exécution du projet

### Mode 1 : Lancer SUMO avec le contrôleur Python intégré

```bash
python traffic_controller.py
```

Cette commande démarre SUMO-GUI avec le contrôleur de trafic intelligent intégré et un tableau de bord pour visualiser les métriques en temps réel.

### Mode 2 : Exécution séparée (pour le débogage)

1. Démarrez SUMO en mode serveur :
   ```bash
   sumo-gui -c quartier.sumocfg --remote-port 8813
   ```

2. Dans un autre terminal, lancez le tableau de bord :
   ```bash
   python dashboard.py
   ```

## Fonctionnalités implémentées

- **Priorisation des bus** : Détecte la présence des bus et leur donne immédiatement un feu vert
- **Gestion de la congestion** : Adapte les feux en fonction du nombre de véhicules
- **Simulation de grève** : Réduction drastique du nombre de bus après un temps défini
- **Tableau de bord en temps réel** :
  - Temps d'attente moyen des véhicules
  - Nombre de bus passés
  - Niveau de congestion
  - Émissions de CO₂

## Analyse des résultats

À la fin de la simulation, un graphique comparatif est généré montrant l'évolution des métriques avant et pendant la grève de bus. Le fichier est enregistré sous `resultats_simulation.png`.

## Extension du projet

Ce projet peut être étendu de plusieurs façons :
- Ajout de carrefours