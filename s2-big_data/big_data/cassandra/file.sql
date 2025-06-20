CREATE KEYSPACE IF NOT EXISTS ecole1 WITH REPLICATION={'class':'SimpleStrategy', 'replication_factor':3};
USE ecole1;

CREATE TABLE Cours1(idCours INT, Intitule VARCHAR, Responsable INT, Niveau VARCHAR, nbHeuresMax INT, PRIMARY KEY(idCours));
CREATE INDEX fk_Enseignement_Enseignant1_idx ON Cours1(Responsable);

CREATE TABLE Enseignant1(idEnseignant INT, Nom VARCHAR, Prenom VARCHAR, Departement VARCHAR, PRIMARY KEY (idEnseignant));
DESC ecole1;

-------> 
INSERT INTO Cours1(idCours, Intitule, Responsable, Niveau, nbHeuresMax)  VALUES (1, 'Infographics', 1, 'S1', 40);
INSERT INTO Cours1(idCours, Intitule, Responsable, Niveau, nbHeuresMax) VALUES (2, 'Metaheuristics', 2, 'S1', 50);
INSERT INTO Cours1(idCours, Intitule, Responsable, Niveau, nbHeuresMax) VALUES (3, 'Recherche Operationnelle', 3, 'S1', 45);
INSERT INTO Cours1(idCours, Intitule, Responsable, Niveau, nbHeuresMax) VALUES (4, 'Stochastics', 4, 'S1', 40);
INSERT INTO Cours1(idCours, Intitule, Responsable, Niveau, nbHeuresMax) VALUES (5, 'Probability', 5, 'S1', 35);
INSERT INTO Cours1(idCours, Intitule, Responsable, Niveau, nbHeuresMax) VALUES (6, 'Traitement d image', 6, 'S1', 50);
INSERT INTO Cours1(idCours, Intitule, Responsable, Niveau, nbHeuresMax) VALUES (7, 'Langue Francaise / Anglaise', 7, 'S1', 30);
INSERT INTO Cours1(idCours, Intitule, Responsable, Niveau, nbHeuresMax) VALUES (8, 'Gouvernance des donnees', 8, 'S1', 45);
INSERT INTO Cours1(idCours, Intitule, Responsable, Niveau, nbHeuresMax) VALUES (9, 'Base de donnees avancee', 1, 'S2', 50);
INSERT INTO Cours1(idCours, Intitule, Responsable, Niveau, nbHeuresMax) VALUES (10, 'Reseaux de neurones', 2, 'S2', 50);
INSERT INTO Cours1(idCours, Intitule, Responsable, Niveau, nbHeuresMax) VALUES (11, 'Deep Learning', 3, 'S2', 60);
INSERT INTO Cours1(idCours, Intitule, Responsable, Niveau, nbHeuresMax) VALUES (12, 'Smart Cities', 4, 'S2', 40);
INSERT INTO Cours1(idCours, Intitule, Responsable, Niveau, nbHeuresMax) VALUES (13, 'Internet of Things (IoT)', 5, 'S2', 40);
INSERT INTO Cours1(idCours, Intitule, Responsable, Niveau, nbHeuresMax) VALUES (14, 'Power BI', 6, 'S2', 35);
INSERT INTO Cours1(idCours, Intitule, Responsable, Niveau, nbHeuresMax) VALUES (15, 'Business Intelligence', 7, 'S2', 45);
INSERT INTO Cours1(idCours, Intitule, Responsable, Niveau, nbHeuresMax) VALUES (16, 'Big Data', 8, 'S2', 50);
INSERT INTO Cours1(idCours, Intitule, Responsable, Niveau, nbHeuresMax) VALUES (17, 'Data Mining', 9, 'S2', 50);
INSERT INTO Cours1(idCours, Intitule, Responsable, Niveau, nbHeuresMax) VALUES (18, 'Langue Anglaise / Francaise', 10, 'S2', 30);

SELECT  * FROM Cours1;

-------> 
INSERT INTO Enseignant1 (idEnseignant, Nom, Prenom, Departement) VALUES (1, 'AHERRAHROU', 'PR. NOURA', 'INFO');
INSERT INTO Enseignant1 (idEnseignant, Nom, Prenom, Departement) VALUES (2, 'EL BETTEOUI', 'PR. ISMAIL', 'INFO');
INSERT INTO Enseignant1 (idEnseignant, Nom, Prenom, Departement) VALUES (3, 'ENHNAHI', 'PR. NOURDINE', 'INFO');
INSERT INTO Enseignant1 (idEnseignant, Nom, Prenom, Departement) VALUES (4, 'EL BOURKADI', 'PR. DOUNIA', 'INFO');
INSERT INTO Enseignant1 (idEnseignant, Nom, Prenom, Departement) VALUES (5, 'EL FIZAZY', 'PR. KHALID', 'INFO');
INSERT INTO Enseignant1 (idEnseignant, Nom, Prenom, Departement) VALUES (6, 'EL HAOUSSI', 'PR. FATIMA', 'INFO');
INSERT INTO Enseignant1 (idEnseignant, Nom, Prenom, Departement) VALUES (7, 'SATORI', 'PR. HASSAN', 'INFO');
INSERT INTO Enseignant1 (idEnseignant, Nom, Prenom, Departement) VALUES (8, 'SATORI', 'PR. KHALID', 'INFO');
INSERT INTO Enseignant1 (idEnseignant, Nom, Prenom, Departement) VALUES (9, 'CHAKIR', 'PR. LOQMAN', 'INFO');
INSERT INTO Enseignant1 (idEnseignant, Nom, Prenom, Departement) VALUES (10, 'MOHAMMED', 'PR. TAHIRI', 'INFO');
INSERT INTO Enseignant1 (idEnseignant, Nom, Prenom, Departement) VALUES (11, 'EL FAR', 'PR. MOHAMED', 'INFO');
INSERT INTO Enseignant1 (idEnseignant, Nom, Prenom, Departement) VALUES (12, 'EL BEQQALI', 'PR. OMAAR', 'INFO');
INSERT INTO Enseignant1 (idEnseignant, Nom, Prenom, Departement) VALUES (13, 'EL BOUAID', 'PR. ANAS', 'INFO');
INSERT INTO Enseignant1 (idEnseignant, Nom, Prenom, Departement) VALUES (14, 'BOUMHIDI', 'PR. JAWAD', 'INFO');
INSERT INTO Enseignant1 (idEnseignant, Nom, Prenom, Departement) VALUES (15, 'BEN LAHBIB', 'PR. ABDESSAMAD', 'INFO');

SELECT * FROM Enseignant1;


------->
1- Lister les intitulés de cours.
SELECT intitule FROM Cours1;

2- Nom de l’enseignant n°4.
SELECT nom FROM Enseignant1 WHERE idEnseignant = 4;

3- Intitulé des cours du responsable n°7.
SELECT intitule FROM Cours1 WHERE responsable = 7 ALLOW FILTERING;


run this first -> CREATE INDEX ON Cours1(nbHeuresMax);
4- Intitulé des cours dont le nombre d’heures maximum est égal à 50.
SELECT intitule FROM Cours1 WHERE nbHeuresMax = 50 ALLOW FILTERING;

